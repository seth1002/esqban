// Echo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>


#define MAX_COMB_FILTERS 3;

int work(short int *samples, int numsamples, int bps, int nch, int srate)
{
/*	int  i, j, s, x;
	int delays[MAX_COMB_FILTERS]; // delays for the comb filters

	//Setup the various delays for each comb filter in samples.
	delays[0] = 0;		// Use "random" values for the delays
	delays[1] = 10;		// so that the different delayed samples
	delays[2] = 124;	// are incoherent.
	//...

	for(i=0; i <= delay_size;i++){ 
		if (i >= delay) 
			j = i - delay;    // work out buffer position
		else 
			j = i - delay + delay_size + 1;

		x = delay_buffer[j+delays[0]]*decay1;	// add first comb filter output
		x += delay_buffer[j+delays[1]]*decay2;	// then second
		//...									// and so on...

		s = input + x*decay0 ;					// scale the sum and add to input

		// store the result in the delay buffer, and output
		delay_buffer[i] = s;
		samples[i] = s;
	}
*/
	return numsamples;
}

int main(int argc, char* argv[])
{
	if( -1 == setmode( fileno ( stdin ), O_BINARY ) )
	{
		perror( "Cannot set input file translation mode" );
		return -1;
	}

	if( -1 == setmode( fileno ( stdout ), O_BINARY ) )
	{
		perror( "Cannot set output file translation mode" );
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

		int x = work((short*)buffer, readBytes/2, 16, 2, 44100);
		//int x = voiceRemoval((short*)buffer, readBytes/2, 16, 2, 44100);
		
		//UINT n = fwrite( buffer, 1, sizeof(buffer), stdout );
		UINT n = fwrite( buffer, 1, x*2, stdout );
	}

	return 0;
}

