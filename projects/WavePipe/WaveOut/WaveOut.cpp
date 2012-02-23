// WaveOut.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

// some good values for block size and count
#define BLOCK_SIZE  8192
#define BLOCK_COUNT 20

// function prototypes
static void CALLBACK	waveOutProc(HWAVEOUT, UINT, DWORD, DWORD, DWORD);
static WAVEHDR*			allocateBlocks(int size, int count);
static void				freeBlocks(WAVEHDR* blockArray);
static void				writeAudio(HWAVEOUT hWaveOut, LPSTR data, int size);

// module level variables
static CRITICAL_SECTION waveCriticalSection;
static WAVEHDR*         waveBlocks;
static volatile int     waveFreeBlockCount;
static int              waveCurrentBlock;


static void CALLBACK waveOutProc(
    HWAVEOUT hWaveOut, 
    UINT uMsg, 
    DWORD dwInstance,  
    DWORD dwParam1,    
    DWORD dwParam2     
)
{
	// pointer to free block counter
	int* freeBlockCounter = (int*)dwInstance;
	
	// ignore calls that occur due to openining and closing the device.
	if(uMsg != WOM_DONE)
		return;
	
	EnterCriticalSection(&waveCriticalSection);
	(*freeBlockCounter)++;
	LeaveCriticalSection(&waveCriticalSection);
}

WAVEHDR* allocateBlocks(int size, int count)
{
	unsigned char* buffer;
	int i;
	WAVEHDR* blocks;
	DWORD totalBufferSize = (size + sizeof(WAVEHDR)) * count;
	
	// allocate memory for the entire set in one go
	if((buffer = (unsigned char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, totalBufferSize)) == NULL)
	{
		fprintf(stderr, "Memory allocation error\n");
		ExitProcess(1);
	}
	
	// and set up the pointers to each bit
	blocks = (WAVEHDR*)buffer;
	buffer += sizeof(WAVEHDR) * count;
	for(i = 0; i < count; i++) {
		blocks[i].dwBufferLength = size;
		blocks[i].lpData = (char*)buffer;
		buffer += size;
	}
	
	return blocks;
}

void freeBlocks(WAVEHDR* blockArray)
{
	// and this is why allocateBlocks works the way it does
	HeapFree(GetProcessHeap(), 0, blockArray);
}

void writeAudio(HWAVEOUT hWaveOut, LPSTR data, int size)
{
	WAVEHDR* current;
	int remain;
	
	current = &waveBlocks[waveCurrentBlock];
	
	while(size > 0) {
		// first make sure the header we're going to use is unprepared
		if(current->dwFlags & WHDR_PREPARED) 
			waveOutUnprepareHeader(hWaveOut, current, sizeof(WAVEHDR));
		
		if(size < (int)(BLOCK_SIZE - current->dwUser)) {
			memcpy(current->lpData + current->dwUser, data, size);
			current->dwUser += size;
			break;
		}
		
		remain = BLOCK_SIZE - current->dwUser;
		memcpy(current->lpData + current->dwUser, data, remain);
		size -= remain;
		data += remain;
		current->dwBufferLength = BLOCK_SIZE;
		
		waveOutPrepareHeader(hWaveOut, current, sizeof(WAVEHDR));
		waveOutWrite(hWaveOut, current, sizeof(WAVEHDR));
		
		EnterCriticalSection(&waveCriticalSection);
		waveFreeBlockCount--;
		LeaveCriticalSection(&waveCriticalSection);
		
		// wait for a block to become free
		while(!waveFreeBlockCount)
			Sleep(10);
		
		// point to the next block
		waveCurrentBlock++;
		waveCurrentBlock %= BLOCK_COUNT;
		
		current = &waveBlocks[waveCurrentBlock];
		current->dwUser = 0;
	}
}

void initWFX(int argc, char ** argv, WAVEFORMATEX *wfx)
{
	int i, n;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0] == '-')
		{
			char c = argv[i][1];
			i++;
			if(i>=argc)
				break;
			n = atoi(argv[i]);
			switch(c)
			{
			case 'C':
			case 'c':
				wfx->nChannels = n;
				break;
			case 'S':
			case 's':
				wfx->nSamplesPerSec	= n;
				break;
			case 'B':
			case 'b':
				wfx->wBitsPerSample	= n;
				break;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	HWAVEOUT hWaveOut;	// device handle
	WAVEFORMATEX wfx;	// look this up in your documentation
	char buffer[1024];	// intermediate buffer for reading
	int i, readBytes=1;

	if( -1 == setmode ( fileno ( stdin ), O_BINARY ) )
	{
		perror( "Cannot set input file translation mode" );
		return -1;
	}

	// initialise the module variables
	waveBlocks         = allocateBlocks(BLOCK_SIZE, BLOCK_COUNT);
	waveFreeBlockCount = BLOCK_COUNT;
	waveCurrentBlock   = 0;
	
	InitializeCriticalSection(&waveCriticalSection);

	// set up the WAVEFORMATEX structure.
	wfx.nSamplesPerSec  = 44100;	// sample rate
	wfx.wBitsPerSample  = 16;		// sample size
	wfx.nChannels       = 2;		// channels
	
	initWFX(argc, argv, &wfx);
	
	wfx.cbSize          = 0;		// size of _extra_ info
	wfx.wFormatTag      = WAVE_FORMAT_PCM;
	wfx.nBlockAlign     = (wfx.wBitsPerSample * wfx.nChannels) >> 3;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
	
	// try to open the default wave device. WAVE_MAPPER is
	// a constant defined in mmsystem.h, it always points to the
	// default wave device on the system (some people have 2 or
	// more sound cards).
	if(waveOutOpen(
		&hWaveOut, 
		WAVE_MAPPER, 
		&wfx, 
		(LONG)waveOutProc, 
		(LONG)&waveFreeBlockCount, 
		CALLBACK_FUNCTION
		) != MMSYSERR_NOERROR) {
		fprintf(stderr, "%s: unable to open wave mapper device\n", argv[0]);
		ExitProcess(1);
	}

	while(readBytes > 0) {
		readBytes = fread( buffer, sizeof( char ), sizeof(buffer), stdin );
		
		if(readBytes == 0)
			break;

		if(readBytes < sizeof(buffer)) 
			memset(buffer + readBytes, 0, sizeof(buffer) - readBytes);
		
		writeAudio(hWaveOut, buffer, sizeof(buffer));
	}
	
	// wait for all blocks to complete
	while(waveFreeBlockCount < BLOCK_COUNT)
		Sleep(10);
	
	// unprepare any blocks that are still prepared
	for(i = 0; i < waveFreeBlockCount; i++) 
	{
		if(waveBlocks[i].dwFlags & WHDR_PREPARED)
			waveOutUnprepareHeader(hWaveOut, &waveBlocks[i], sizeof(WAVEHDR));
	}
		
	DeleteCriticalSection(&waveCriticalSection);
	freeBlocks(waveBlocks);
	waveOutClose(hWaveOut);
	
	return 0;
}
