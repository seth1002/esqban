// WaveOut.h: interface for the CWaveOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEOUT_H__8D2BD850_A180_4516_91AE_B647054986B9__INCLUDED_)
#define AFX_WAVEOUT_H__8D2BD850_A180_4516_91AE_B647054986B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

// some good values for block size and count
#define BLOCK_SIZE  8192
#define BLOCK_COUNT 20

// module level variables
static CRITICAL_SECTION waveCriticalSection;
static WAVEHDR*         waveBlocks;
static volatile int     waveFreeBlockCount;
static int              waveCurrentBlock;

class CWaveOut  
{
public:
	CWaveOut();
	virtual ~CWaveOut();

	void Write(const char *buf, int size);

private:
	int Init();
	void Close();

private:
	// function prototypes
	static void CALLBACK	waveOutProc(HWAVEOUT, UINT, DWORD, DWORD, DWORD);
	static WAVEHDR*			allocateBlocks(int size, int count);
	static void				freeBlocks(WAVEHDR* blockArray);
	static void				writeAudio(HWAVEOUT hWaveOut, LPSTR data, int size);

	HWAVEOUT hWaveOut;	// device handle
	WAVEFORMATEX wfx;	// look this up in your documentation
};

#endif // !defined(AFX_WAVEOUT_H__8D2BD850_A180_4516_91AE_B647054986B9__INCLUDED_)
