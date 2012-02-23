// WaveOut.cpp: implementation of the CWaveOut class.
//
//////////////////////////////////////////////////////////////////////

#include "WaveOut.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaveOut::CWaveOut()
{
	Init();
}

CWaveOut::~CWaveOut()
{
	Close();
}


void CALLBACK CWaveOut::waveOutProc(
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

WAVEHDR* CWaveOut::allocateBlocks(int size, int count)
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

void CWaveOut::freeBlocks(WAVEHDR* blockArray)
{
	// and this is why allocateBlocks works the way it does
	HeapFree(GetProcessHeap(), 0, blockArray);
}

void CWaveOut::writeAudio(HWAVEOUT hWaveOut, LPSTR data, int size)
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

int CWaveOut::Init()
{
	int result;

	result = setmode ( fileno ( stdin ), O_BINARY );
	if( result == -1 )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	// initialise the module variables
	waveBlocks         = allocateBlocks(BLOCK_SIZE, BLOCK_COUNT);
	waveFreeBlockCount = BLOCK_COUNT;
	waveCurrentBlock   = 0;
	
	InitializeCriticalSection(&waveCriticalSection);

	// set up the WAVEFORMATEX structure.
	//wfx.nSamplesPerSec  = 44100;	// sample rate
	//wfx.wBitsPerSample  = 16;		// sample size
	//wfx.nChannels       = 2;		// channels
	wfx.nChannels = 1;				//1 or 2
	wfx.nSamplesPerSec	= 22050;	//11.025 kHz, 22.05 kHz, and 44.1 kHz.
	wfx.wBitsPerSample	= 8;		//8 or 16
	
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
		fprintf(stderr, "Unable to open wave mapper device\n");
		ExitProcess(1);
	}

	return 0;
}

void CWaveOut::Close()
{
	int i;

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
}

void CWaveOut::Write(const char *buf, int size)
{
	if(size > 0)
	{
//		if(readBytes < sizeof(buf)) 
//			memset(buffer + readBytes, 0, sizeof(buffer) - readBytes);
		writeAudio(hWaveOut, (char*)buf, size);
	}
}
