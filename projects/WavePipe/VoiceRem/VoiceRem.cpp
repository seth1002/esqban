// Echo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>


int voiceRemoval(short int *samples, int numsamples, int bps, int nch, int srate)
{
	int x = numsamples;
	if (bps == 16)
	{
		short *a = samples;
		if (nch == 2) while (x--)
		{
			int l, r;
			l = a[1]-a[0];
			r = a[0]-a[1];		
			if (l < -32768)	l = -32768;
			if (l > 32767)	l = 32767;
			if (r < -32768)	r = -32768;
			if (r > 32767)	r = 32767;
			a[0] = l;
			a[1] = r;
			a+=2;
		}
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

		int x = voiceRemoval((short*)buffer, readBytes/2, 16, 2, 44100);
		
		//UINT n = fwrite( buffer, 1, sizeof(buffer), stdout );
		UINT n = fwrite( buffer, 1, x*2, stdout );
	}

	return 0;
}

