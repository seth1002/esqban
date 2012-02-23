// Echo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>


char *gBuffer;
float fDelay		= 0.5f;
float fFeedBack		= 0.5f;
long cursor			= 0;
long delay			= 0;


bool init()
{
	if( -1 == setmode ( fileno ( stdin ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return false;
	}

	if( -1 == setmode ( fileno ( stdout ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return false;
	}

	return true;
}

int work(char *samples, int numsamples, int bps, int nch, int srate)
{
	char *in=samples, *out1=samples, *out2=samples+1;
	int n=numsamples;//*nch;

	while (--n >= 0)
	{
		char x = *in++;
		char y = gBuffer[cursor];
		gBuffer[cursor++] = x + y * fFeedBack;
		if (cursor >= delay)
			cursor = 0;
		*out1++ = y;
		if (out2)
			*out2++ = y;
	}
	return numsamples;
}

int main(int argc, char* argv[])
{
	char buffer[1024];	// intermediate buffer for reading
	int readBytes = 1;
	int buffsize = sizeof(buffer);

	if(!init())
		return -1;

	long size = 44100;
	gBuffer = new char[size];
	delay = (long)(fDelay * (float)(size - 1));

	while(readBytes > 0) {
		readBytes = fread( buffer, sizeof(char), buffsize, stdin );
		
		if(readBytes == 0)
			break;

		if(readBytes < buffsize) 
			memset(buffer + readBytes, 0, buffsize - readBytes);

		cursor = 0;
		memset (gBuffer, 0, size * sizeof(char));
		int x = work(buffer, readBytes/2, 16, 2, 44100);
		
		UINT n = fwrite( buffer, 1, buffsize, stdout );
		//UINT n = fwrite( buffer, 1, x*2, stdout );
	}

	delete gBuffer;

	return 0;
}

