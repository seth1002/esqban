// Echo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>


// pitch value
int g_pitch=110;

// pitch control window
HWND pitch_control_hwnd;

// auxilary pitch buffer (for resampling from)
short *pitch_buffer=NULL;
int pitch_buffer_len=0;
int quit_pitch=0;


int pitchTempoControl(short int *samples, int numsamples, int bps, int nch, int srate)
{
	int pitch=g_pitch;
	int rlen =numsamples*bps/8*nch;
	int index=0, x;
	int n; 
	int dindex; 
	if (quit_pitch || g_pitch==100) return numsamples;
	if (g_pitch > 200) g_pitch=200;
	if (g_pitch < 50) g_pitch=50;
	pitch = 100000/pitch;
	n=(numsamples*pitch)/1000;
	dindex=(numsamples<<14)/n;
	if (pitch_buffer_len < rlen) 
	{
		pitch_buffer_len = rlen;
		GlobalFree(pitch_buffer);
		pitch_buffer=(short*)GlobalAlloc(GMEM_FIXED,rlen);
	}
	if (bps == 16 && nch == 2)
	{
		short *buf=pitch_buffer;
		memcpy(buf,samples,rlen);
		for (x = 0; x < n; x ++)
		{
			int p=(index>>14)<<1;
			index+=dindex;
			samples[0] = buf[p];
			samples[1] = buf[p+1];
			samples+=2;
		}
		return n;
	}
	else if (bps == 16 && nch == 1)
	{
		short *buf=pitch_buffer;
		memcpy(buf,samples,rlen);
		for (x = 0; x < n; x ++)
		{
			int p=(index>>14);
			index+=dindex;
			*samples++ = buf[p];
		}
		return n;
	}
	return numsamples;
}

int main(int argc, char* argv[])
{
	int result = setmode ( fileno ( stdin ), O_BINARY );
	if( result == -1 )
	{
		perror( "Cannot set mode" );
		return -1;
	}
	result = setmode ( fileno ( stdout ), O_BINARY );
	if( result == -1 )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	char buffer[1024];	// intermediate buffer for reading

	DWORD readBytes = 1;
	// playback loop
	while(readBytes > 0) {
		readBytes = fread( buffer, sizeof( char ), sizeof(buffer), stdin );
		if(readBytes == 0)
			break;

		if(readBytes < sizeof(buffer)) 
			memset(buffer + readBytes, 0, sizeof(buffer) - readBytes);

		int x = pitchTempoControl((short*)buffer, readBytes/2, 16, 2, 44100);
		//int x = voiceRemoval((short*)buffer, readBytes/2, 16, 2, 44100);
		
		//UINT n = fwrite( buffer, 1, sizeof(buffer), stdout );
		UINT n = fwrite( buffer, 1, x*2, stdout );
	}

	return 0;
}

