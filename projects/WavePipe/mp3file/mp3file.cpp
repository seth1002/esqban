#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <stdlib.h>

#define BUFF_LEN	20480


typedef BOOL	(FAR PASCAL *FilterGetActualFormat)	(HANDLE hInput,DWORD dwOptions,long * plSamprate, WORD * pwChannels, WORD * pwBitsPerSample);
typedef BOOL	(FAR PASCAL *FilterUnderstandsFormat)(LPSTR filename);
typedef BOOL	(FAR PASCAL *DIALOGMsgProc)			(HWND hWndDlg, UINT Message, WPARAM wParam, LPARAM lParam);
typedef DWORD	(FAR PASCAL *FilterGetOptions)		(HWND hWnd, HINSTANCE hInst, long lSamprate, WORD wChannels, WORD wBitsPerSample, DWORD dwOptions); // return 0 if no options box
typedef DWORD	(FAR PASCAL *FilterSetOptions2)		(HANDLE hInput, DWORD dwOptions, long * plSamprate, WORD * pwChannels, WORD * pwBitsPerSample);
typedef DWORD	(FAR PASCAL *FilterOptions)			(HANDLE hInput);
typedef DWORD	(FAR PASCAL *FilterOptionsString)	(HANDLE hInput, LPSTR szString);
typedef HANDLE	(FAR PASCAL *OpenFilterOutput)		(LPSTR lpstrFilename,long lSamprate,WORD wBitsPerSample,WORD wChannels,long lSize,long far *lpChunkSize,DWORD dwOptions);
typedef void	(FAR PASCAL *CloseFilterOutput)		(HANDLE hOutput);
typedef DWORD	(FAR PASCAL *WriteFilterOutput)		(HANDLE hOutput, unsigned char far *buf, long lBytes);
typedef void	(FAR PASCAL *GetSuggestedSampleType)(long far *lSugSamprate,WORD far *wSugBitsPerSample, WORD far *wSugChannels);

typedef long	(FAR PASCAL *FilterGetFileSize)		(HANDLE hInput);
typedef HANDLE	(FAR PASCAL *OpenFilterInput)		(LPSTR lpstrFilename, long *lSamprate, WORD *wBitsPerSample, WORD *wChannels, HWND hWnd, long *lChunkSize);
typedef void	(FAR PASCAL *CloseFilterInput)		(HANDLE hInput);
typedef DWORD	(FAR PASCAL *ReadFilterInput)		(HANDLE hInput, unsigned char *bufout, long lBytes);


//HWND m_hWnd;
HINSTANCE m_hDllInstance;
HINSTANCE m_hinstLib;
//WAVEFORMATEX m_wfx;
TCHAR m_szFileName[MAX_PATH];

HANDLE m_hInput;

FilterGetOptions	m_FilterGetOptions;
FilterGetFileSize	m_FilterGetFileSize;
OpenFilterInput		m_OpenFilterInput;
CloseFilterInput	m_CloseFilterInput;
ReadFilterInput		m_ReadFilterInput;
FilterSetOptions2	m_FilterSetOptions2;




void read(char *filename)
{

	DWORD readBytes = 1;
//	BYTE buffer[BUFF_LEN];
	BYTE *pbuffer = new BYTE[BUFF_LEN];

	long lSamprate = 0;
	WORD wBitsPerSample = 0;
	WORD wChannels = 0;
	long lChunkSize = 0;

	m_hInput = m_OpenFilterInput(filename, &lSamprate, &wBitsPerSample, &wChannels, NULL, &lChunkSize);

	if(m_hInput != NULL)
	{
		/*m_wfx.wFormatTag = WAVE_FORMAT_PCM;
		m_wfx.nChannels = wChannels;
		m_wfx.nSamplesPerSec = lSamprate;
		m_wfx.wBitsPerSample = wBitsPerSample;
		m_wfx.nBlockAlign = m_wfx.nChannels * m_wfx.wBitsPerSample / 8;
		m_wfx.nAvgBytesPerSec = m_wfx.nSamplesPerSec * m_wfx.nBlockAlign;*/

		m_FilterSetOptions2(m_hInput, 0, &lSamprate, &wChannels, &wBitsPerSample);
	}

	int size = BUFF_LEN * sizeof(BYTE);

	do {
		memset(pbuffer, 0, BUFF_LEN);
		readBytes = m_ReadFilterInput(m_hInput, pbuffer, size);

		int samples = readBytes / sizeof(short);
		
		fwrite(pbuffer, 1, readBytes, stdout);
	} while(readBytes > 0);

	delete pbuffer;

	if(m_hInput!=NULL)
		m_CloseFilterInput(m_hInput);
}

void write(char *filename, BOOL bOut)
{
}

int main(int argc, char* argv[])
{
	if( -1 == setmode ( fileno ( stdout ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return -1;
	}


	m_hInput = NULL;
	m_hinstLib = NULL;

//	m_hWnd = hWnd;
	m_hDllInstance = GetModuleHandle(NULL);

	m_hinstLib = LoadLibrary("mp3.flt");
	if (m_hinstLib != NULL) 
	{ 
		m_OpenFilterInput = (OpenFilterInput)GetProcAddress(m_hinstLib, "OpenFilterInput"); 
		m_CloseFilterInput = (CloseFilterInput)GetProcAddress(m_hinstLib, "CloseFilterInput"); 
		m_ReadFilterInput = (ReadFilterInput)GetProcAddress(m_hinstLib, "ReadFilterInput"); 

		m_FilterGetOptions = (FilterGetOptions)GetProcAddress(m_hinstLib, "FilterGetOptions");
		m_FilterGetFileSize = (FilterGetFileSize)GetProcAddress(m_hinstLib, "FilterGetFileSize");

		m_FilterSetOptions2 = (FilterSetOptions2)GetProcAddress(m_hinstLib, "FilterSetOptions2");
	}

	char *filename = (argc==2?argv[1]:TEXT("in.mp3"));

	read( filename );

	if(m_hinstLib!=NULL)
		FreeLibrary(m_hinstLib); 

	return 0;
}
