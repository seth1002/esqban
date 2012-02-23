// Echo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>


short echo_buf[65536], echo_buf2[65536];


int delay(short int *input, int numsamples, int bps, int nch, int srate)
{
/*	For Each Sample we have in the Array
		 If CurrentSample - DelayLength < 0 then 
			  'We have nothing to delay yet, so do nothing to the samples.
          
		 ElseIf CurrentSample - DelayLength * 2 < 0 then
			  'We have something to delay, but only for the first echo.
			  Output (CurrentSamples) = Input (CurrentSamples) + _
			   Volume1 * (Input (CurrentSample - DelayLength))
		 Else
			  'We have something to delay for both buff
			  Output (CurrentSamples) = Input (CurrentSamples) + _
			   Volume1 * (Input (CurrentSample - DelayLength)) + _
			   Volume2 * (Input (CurrentSample - (DelayLength * 2)))
		 End If
	Next Sample*/
	int DelayLength = 100;
	long DelayPos=0;
	DelayLength = DelayLength * nch;
	short int *output = (short int *)malloc(numsamples*nch);
	int Volume1, Volume2;
	for(int i=0; i<numsamples; i++)
	{
		if(input[i] - DelayLength < 0)
		{
			// We have nothing to delay yet, so do nothing to the samples.
		}
		else if(input[i] - DelayLength * 2 < 0)
		{
			// We have something to delay, but only for the first echo.
			output[i] = input[i] + Volume1 * (input[i - DelayLength]);
		}
		else
		{
			  // We have something to delay for both buff
			  output[i] = input[i] + Volume1 * (input[i - DelayLength]) + Volume2 * (input[i - (DelayLength * 2)]);
		}
		DelayPos = (DelayPos + 1) % DelayLength;
	}
	delete output;
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

		int x = delay((short*)buffer, readBytes/2, 16, 2, 44100);
		//int x = voiceRemoval((short*)buffer, readBytes/2, 16, 2, 44100);
		
		//UINT n = fwrite( buffer, 1, sizeof(buffer), stdout );
		UINT n = fwrite( buffer, 1, x*2, stdout );
	}

	return 0;
}

