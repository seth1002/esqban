// WaveIn.h: interface for the CWaveIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEIN_H__9B65CFE3_0A8C_4D66_9570_036A07B1E60A__INCLUDED_)
#define AFX_WAVEIN_H__9B65CFE3_0A8C_4D66_9570_036A07B1E60A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <mmsystem.h>
#include <fcntl.h>
#include <io.h>

class CWaveIn  
{
public:
	CWaveIn(int (*OnSendData)(const char*, int));
	virtual ~CWaveIn();

	int Start();
	void SetOnSendData(int (*OnSendData)(const char*, int));
	int OnSendData(const char *data, int size);

private:
	void PrintWaveErrorMsg(DWORD err, TCHAR * str);
	static DWORD WINAPI waveInProc(LPVOID arg);

private:
	HWAVEIN m_WaveInHandle;	// Handle to the WAVE In Device
	HMIXER	m_MixerHandle;	// Handle to Mixer for WAVE In Device

	// We use two WAVEHDR's for recording (ie, double-buffering) in this example
	WAVEHDR	m_WaveHeader[2];

	BOOL	m_bRecording;// Variable used to indicate whether we are in record

	// Variable used by recording thread to indicate whether we are in record
	unsigned char	m_DoneAll;

	int (*m_OnSendData)(const char *data, int size);
};

#endif // !defined(AFX_WAVEIN_H__9B65CFE3_0A8C_4D66_9570_036A07B1E60A__INCLUDED_)
