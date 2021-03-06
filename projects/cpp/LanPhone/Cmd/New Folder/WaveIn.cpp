// WaveIn.cpp: implementation of the CWaveIn class.
//
//////////////////////////////////////////////////////////////////////

#include "WaveIn.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaveIn::CWaveIn(int (*OnSendData)(const char*, int))
{
	m_OnSendData = OnSendData;
	m_bRecording = FALSE;
	m_OnSendData = NULL;
}

CWaveIn::~CWaveIn()
{

}

int CWaveIn::OnSendData(const char *data, int size) 
{
	if(m_OnSendData)
		return m_OnSendData(data, size);
	return 0;
}

void CWaveIn::SetOnSendData(int (*OnSendData)(const char*, int))
{
	m_OnSendData = OnSendData;
}

//*********************** PrintWaveErrorMsg() **************************
// Retrieves and displays an error message for the passed Wave In error
// number. It does this using mciGetErrorString().
//**********************************************************************
void CWaveIn::PrintWaveErrorMsg(DWORD err, TCHAR * str)
{
#define BUFFERSIZE 128
	char	buffer[BUFFERSIZE];
	
	fprintf(stderr, "ERROR 0x%08X: %s\r\n", err, str);
	if (mciGetErrorString(err, &buffer[0], sizeof(buffer)))
	{
		fprintf(stderr, "%s\r\n", &buffer[0]);
	}
	else
	{
		fprintf(stderr, "0x%08X returned!\r\n", err);
	}
}


DWORD WINAPI CWaveIn::waveInProc(LPVOID arg)
{
	MSG		msg;

	CWaveIn *waveIn = (CWaveIn*)arg;

	// Wait for a message sent to me by the audio driver
	while (GetMessage(&msg, 0, 0, 0) == 1)
	{
		// Figure out which message was sent
		switch (msg.message)
		{
			// A buffer has been filled by the driver
			case MM_WIM_DATA:
			{
				WAVEHDR *pWaveHeader = (WAVEHDR *)msg.lParam;
				DWORD dwBytesRecorded = pWaveHeader->dwBytesRecorded;
				if (dwBytesRecorded)
				{
					UINT n = waveIn->OnSendData(pWaveHeader->lpData, dwBytesRecorded/sizeof(char));
					if(n != dwBytesRecorded)
					{
						fprintf(stderr, "Error writing to output!\n");
						ExitProcess(0);
					}
				}

				if (waveIn->m_bRecording)
				{
					// requeue this buffer so the driver can use it for another block of 
					// audio data. NOTE: We shouldn't need to waveInPrepareHeader() a 
					// WAVEHDR that has already been prepared once
					waveInAddBuffer(waveIn->m_WaveInHandle, pWaveHeader, sizeof(WAVEHDR));
				}
				else
				{
					// We aren't recording, so another WAVEHDR has been returned to us 
					// after recording has stopped. When we get all of them back, 
					// m_DoneAll will be equal to how many WAVEHDRs we queued
					++waveIn->m_DoneAll;
				}

				// Keep waiting for more messages
                continue;
			}

			// Our main thread is opening the WAVE device
			case MM_WIM_OPEN:
			{
				// Re-initialize 'm_DoneAll'
				waveIn->m_DoneAll = 0;

				// Keep waiting for more messages
                continue;
			}

			// Our main thread is closing the WAVE device
			case MM_WIM_CLOSE:
			{
				// Terminate this thread (by return'ing)
				break;
			}
		}
	}

	return(0);
}

int CWaveIn::Start()
{
	{
		MMRESULT		err = 0;
//		WAVEFORMATEX	waveFormat;
//		MIXERLINE		mixerLine;
		HANDLE			waveInThread;
//		unsigned long	numSrc;

		waveInThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)waveInProc, this, 0, &m_dwThreadId);
		if (!waveInThread)
		{
			fprintf(stderr, "Can't create WAVE recording thread! -- %08X\n", GetLastError());
			return(-1);
		}
		CloseHandle(waveInThread);
	}
	{
		DWORD dwThreadId = 0;
		HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)threadProc, this, 0, &dwThreadId);
		if (!hThread)
		{
			//fprintf(stderr, "Can't create threadProc! -- %08X\n", GetLastError());
			return FALSE;
		}
		CloseHandle(hThread);
	}

	return TRUE;
}

DWORD WINAPI CWaveIn::threadProc(LPVOID arg)
{
	MMRESULT		err = 0;
	WAVEFORMATEX	waveFormat;
	MIXERLINE		mixerLine;
//	HANDLE			waveInThread;
	unsigned long	numSrc;

//	DWORD dwThreadId;

	CWaveIn *pWaveIn = (CWaveIn*)arg;

	// Create our thread that we use to store our incoming "blocks" of digital audio 
	// data (sent to us from the driver. The main procedure of this thread is 
	// waveInProc(). We need to get the threadID and pass that to waveInOpen(). 
	// CreateThread() will give it to us
/*	waveInThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)waveInProc, pWaveIn, 0, &dwThreadId);
	if (!waveInThread)
	{
		fprintf(stderr, "Can't create WAVE recording thread! -- %08X\n", GetLastError());
		return(-1);
	}
	CloseHandle(waveInThread);
*/
	// Clear out both of our WAVEHDRs. At the very least, waveInPrepareHeader() expects 
	// the dwFlags field to be cleared
	ZeroMemory(&pWaveIn->m_WaveHeader[0], sizeof(WAVEHDR) * 2);

	// Initialize the WAVEFORMATEX for 16-bit, 44KHz, stereo. That's what I want to record
	//waveFormat.nChannels		= 2;
	//waveFormat.nSamplesPerSec	= 44100;
	//waveFormat.wBitsPerSample	= 16;
	waveFormat.nChannels = 1;				//1 or 2
	waveFormat.nSamplesPerSec	= 22050;	//11.025 kHz, 22.05 kHz, and 44.1 kHz.
	waveFormat.wBitsPerSample	= 8;		//8 or 16

	waveFormat.wFormatTag		= WAVE_FORMAT_PCM;
	waveFormat.nBlockAlign		= waveFormat.nChannels * (waveFormat.wBitsPerSample/8);
	waveFormat.nAvgBytesPerSec	= waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize			= 0;

	// Open the default WAVE In Device, specifying my callback. Note that if this device 
	// doesn't support 16-bit, 44KHz, stereo recording, then Windows will attempt to open 
	// another device that does. So don't make any assumptions about the name of the 
	// device that opens. After waveInOpen returns the handle, use waveInGetID to fetch 
	// its ID, and then waveInGetDevCaps to retrieve the actual name
	err = waveInOpen(&pWaveIn->m_WaveInHandle, WAVE_MAPPER, &waveFormat, pWaveIn->m_dwThreadId, 0, CALLBACK_THREAD);
	if (err)
	{
		pWaveIn->PrintWaveErrorMsg(err, "Can't open WAVE In Device!");
		return(-2);
	}
	else
	{
	    // Open the mixer associated with the WAVE In device opened above. Note that
		// I pass the handle obtained via waveInOpen() above and specify MIXER_OBJECTF_HWAVEIN
		err = mixerOpen(&pWaveIn->m_MixerHandle, (DWORD)pWaveIn->m_WaveInHandle, 0, 0, MIXER_OBJECTF_HWAVEIN);
		if (err)
		{
			// This card's driver may not support the Mixer API, so this error check is
			// very important. NOTE: Hopefully, the card ships with some sort of proprietary
			// software to set the recording level, or the card has a hardware knob that the
			// user can adjust. In either case, setting the recording level is not doable here.
			// You may want to inform the user of that.
			fprintf(stderr, "Device does not have mixer support! -- %08X\n", err);
		}

		else
		{
			// This device should have a WAVEIN destination line. Let's get its ID so
			// that we can determine what source lines are available to record from
			mixerLine.cbStruct = sizeof(MIXERLINE);
			mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
			err = mixerGetLineInfo((HMIXEROBJ)pWaveIn->m_MixerHandle, &mixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE);
			if (err)
				fprintf(stderr, "Device does not have a WAVE recording control! -- %08X\n", err);
			else
			{
				// Get how many source lines are available from which to record. 
				// For example, there could be a Mic In 
				// (MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE), 
				// Line In (MIXERLINE_COMPONENTTYPE_SRC_LINE),
				// and/or SPDIF In (MIXERLINE_COMPONENTTYPE_SRC_DIGITAL)
				numSrc = mixerLine.cConnections;

				// If the WAVEIN destination line has a mute switch, then make sure that it is not muted. So too, you should
				// make sure that the volume is turned up. This would be the master volume and master mute for all input
				// source lines. But you may wish to save the current settings (by fetching them with mixerGetControlDetails)
				// and subsequently restore them (when your program exits) so that you don't permanently alter his settings.
				// Here we only unmute the line, but don't set its volume. Take a look at set_mute() to see how we adjust the
				// value of any mute control. You can modify this easily to create a set_volume() function that sets the value
				// of any MIXERCONTROL_CONTROLTYPE_VOLUME type of control for the line. The only thing that you may need to do
				// is make sure the value is within the MIXERCONTROLDETAILS' dwMinimum and dwMaximum range. (And you need to
				// fetch the current value to get a filled in MIXERCONTROLDETAILS)
				mixerLine.dwSource = mixerLine.dwDestination;

				// Make sure that there is at least one WAVEIN source line available. If not, then just go with the
				// WAVEIN master volume and mute above
				if (!numSrc)
				{
					fprintf(stderr, "ERROR: There are no WAVE inputs to adjust!\n");
				}
			}

			// Close the mixer
			mixerClose(pWaveIn->m_MixerHandle);
		}

		// Now that the inputs are all set up, we can record an audio file using waveInPrepareHeader(),
		// waveInAddBuffer(), and waveInStart().
		

		// First, let's create a file on disk where we'll store the audio data we record. We'll name it
		// test.snd in the current directory. We'll save the data in raw format, with no headers on it,
		// just for simplicity. Normally, you would instead save it in WAVE file format so any program
		// can read it and know the sample rate and number of channels, etc.
//		if(argc == 2)
//			gOutput = fopen(argv[1], "wb");

		// Allocate, prepare, and queue two buffers that the driver can use to record blocks of audio data.
		// (ie, We're using double-buffering. You can use more buffers if you'd like, and you should do that
		// if you suspect that you may lag the driver when you're writing a buffer to disk and are too slow
		// to requeue it with waveInAddBuffer. With more buffers, you can take your time requeueing
		// each).
		//
		// I'll allocate 2 buffers large enough to hold 2 seconds worth of waveform data at 44Khz. NOTE:
		// Just to make it easy, I'll use 1 call to VirtualAlloc to allocate both buffers, but you can
		// use 2 separate calls since the buffers do NOT need to be contiguous. You should do the latter if
		// using many, large buffers
		pWaveIn->m_WaveHeader[1].dwBufferLength = pWaveIn->m_WaveHeader[0].dwBufferLength = waveFormat.nAvgBytesPerSec / 100;//<< 1;
		if (!(pWaveIn->m_WaveHeader[0].lpData = (char *)VirtualAlloc(0, pWaveIn->m_WaveHeader[0].dwBufferLength * 2, MEM_COMMIT, PAGE_READWRITE)))
		{
			fprintf(stderr, "ERROR: Can't allocate memory for WAVE buffer!\n");
		}
		else
		{
			// Fill in WAVEHDR fields for buffer starting address. We've already filled in the size fields above
			pWaveIn->m_WaveHeader[1].lpData = pWaveIn->m_WaveHeader[0].lpData + pWaveIn->m_WaveHeader[0].dwBufferLength;

			// Leave other WAVEHDR fields at 0

			// Prepare the 2 WAVEHDR's
			if ((err = waveInPrepareHeader(pWaveIn->m_WaveInHandle, &pWaveIn->m_WaveHeader[0], sizeof(WAVEHDR))))
			{
				fprintf(stderr, "Error preparing WAVEHDR 1! -- %08X\n", err);
			}
			else
			{
				if ((err = waveInPrepareHeader(pWaveIn->m_WaveInHandle, &pWaveIn->m_WaveHeader[1], sizeof(WAVEHDR))))
				{
					fprintf(stderr, "Error preparing WAVEHDR 2! -- %08X\n", err);
				}
				else
				{
					// Queue first WAVEHDR (recording hasn't started yet)
					if ((err = waveInAddBuffer(pWaveIn->m_WaveInHandle, &pWaveIn->m_WaveHeader[0], sizeof(WAVEHDR))))
					{
						fprintf(stderr, "Error queueing WAVEHDR 1! -- %08X\n", err);
					}
					else
					{
						// Queue second WAVEHDR
						if ((err = waveInAddBuffer(pWaveIn->m_WaveInHandle, &pWaveIn->m_WaveHeader[1], sizeof(WAVEHDR))))
						{
							fprintf(stderr, "Error queueing WAVEHDR 2! -- %08X\n", err);
							pWaveIn->m_DoneAll = 1;
						}
						else
						{
							// Start recording. Our secondary thread will now be receiving
							// and storing audio data to disk
							
							pWaveIn->m_bRecording = TRUE;
							if ((err = waveInStart(pWaveIn->m_WaveInHandle)))
							{
								fprintf(stderr, "Error starting record! -- %08X\n", err);
							}
							else
							{
								// Wait for user to stop recording
								fprintf(stderr, "Recording has started. Press ENTER key to stop recording...\n");
								getchar();
							}
						}

						// Tell our recording thread not to queue any WAVEHDRs it gets back via MM_WIM_DONE
						pWaveIn->m_bRecording = FALSE;
						
						// Stop recording and tell the driver to unqueue/return all of our WAVEHDRs.
						// The driver will return any partially filled buffer that was currently
						// recording. Because we use waveInReset() instead of waveInStop(),
						// all of the other WAVEHDRs will also be returned via MM_WIM_DONE too
						waveInReset(pWaveIn->m_WaveInHandle);

						// Wait for the recording thread to receive the MM_WIM_DONE for each
						// of our queued WAVEHDRs. It will count them off, and after processing
						// them all, 'm_DoneAll' will be equal to how many WAVEHDRs we used. We
						// can then unprepare the WAVEHDRs, close our WAVE device and our disk
						// file, and free our buffers
						while (pWaveIn->m_DoneAll < 2) Sleep(100);
					}

					// Unprepare second WAVEHDR. NOTE: It should be ok to unprepare a WAVEHDR
					// that was never successfully prepared, as long as you initially cleared
					// out the dwFlags field (unless the audio driver is badly written)
					if ((err = waveInPrepareHeader(pWaveIn->m_WaveInHandle, &pWaveIn->m_WaveHeader[1], sizeof(WAVEHDR))))
					{
						fprintf(stderr, "Error unpreparing WAVEHDR 2! -- %08X\n", err);
					}
				}

				// Unprepare first WAVEHDR
				if ((err = waveInPrepareHeader(pWaveIn->m_WaveInHandle, &pWaveIn->m_WaveHeader[0], sizeof(WAVEHDR))))
				{
					fprintf(stderr, "Error unpreparing WAVEHDR 1! -- %08X\n", err);
				}
			}
		}
	}

	// Close the WAVE In device
	do
	{
		err = waveInClose(pWaveIn->m_WaveInHandle);
		if (err) pWaveIn->PrintWaveErrorMsg(err, "Can't close WAVE In Device!");
	} while (err);

	// Close the disk file if it opened
//	if(gOutput != stdout)
//		fclose(gOutput);

	// Free any memory allocated for our buffers
	if (pWaveIn->m_WaveHeader[0].lpData) VirtualFree(pWaveIn->m_WaveHeader[0].lpData, 0, MEM_RELEASE);

	return(0);
}
