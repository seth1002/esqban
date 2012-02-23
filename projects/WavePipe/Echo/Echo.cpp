// Echo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>


short echo_buf[65536], echo_buf2[65536];

/*
int work2(short int *samples, int numsamples, int bps, int nch, int srate)
{
	int  i, j;
	int delay_size = 10;
	int delay_buffer[10];
	int delay = 12;
	int decay = 6;

	for(i=0; i <= delay_size;i++)
	{ 
		if (i >= delay)
			j = i - delay;    // work out the buffer position
		else 
			j = i - delay + delay_size + 1;

		// add the delayed sample to the input sample
		delay_buffer[i] = samples[i] + delay_buffer[j]*decay;

		// store the result in the delay buffer, and output
		samples[i] = delay_buffer[i];
	}
	return numsamples;
}
*/	
int work(short int *samples, int numsamples, int bps, int nch, int srate)
{
	int max = (int)pow(2, bps); // 32768
	// echo doesn't support 8 bit right now cause I'm lazy.
	//if (bps==16)
	{
        int x,s;
        s = numsamples*nch;

        memcpy(echo_buf2,       echo_buf,       s*2);
        memcpy(echo_buf,        echo_buf+s,     s*2);
        memcpy(echo_buf+s,      echo_buf+s*2,	s*2);
        memcpy(echo_buf+s*2,	echo_buf+s*3,	s*2);
        memcpy(echo_buf+s*3,	samples,		s*2);

        for (x = 0; x < s; x ++)
        {
                int s = samples[x]/2 + echo_buf2[x]/2;
                samples[x] = (s > max ? max : (s < -max ? -max : s));
        }
	}
	return numsamples;
}

int main(int argc, char* argv[])
{
	short buffer[1024];	// intermediate buffer for reading
	int readBytes = 1;
	int bufflen = sizeof(buffer);
	
	if( setmode ( fileno ( stdin ), O_BINARY ) == -1 )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	if( setmode ( fileno ( stdout ), O_BINARY ) == -1 )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	while(readBytes > 0) 
	{
		readBytes = fread( buffer, sizeof(char), bufflen, stdin );
		
		if(readBytes == 0)
			break;

		if(readBytes < bufflen) 
			memset(buffer + readBytes, 0, bufflen - readBytes);

		int x = work(buffer, readBytes/2, 16, 2, 44100);
		//int x = work2((short*)buffer, readBytes/2, 16, 2, 44100);
		
		//UINT n = fwrite( buffer, 1, sizeof(buffer), stdout );
		UINT n = fwrite( buffer, 1, x*2, stdout );
	}

	return 0;
}

