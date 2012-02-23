// Echo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>


float Threshold		= 65536 * 16.0;
float Clamp			= 65536 * 16.0;
float NegThreshold	= -65536 * 16.0;
float NegClamp		= -65536 * 16.0;
float Amount		= 1.0;


bool work(short *psamples, int numsamples)
{
	double const drymix = 1.0 - Amount;
	float const clamp = (float)(Amount * Clamp);
	double const threshold = Threshold;
	float negclamp;
	double negthreshold; 

	bool symmetric = true;

	if (symmetric)
	{
		negthreshold = -threshold;
		negclamp = -clamp;
	}
	else
	{
		negthreshold = NegThreshold;
		negclamp = (float)(Amount * NegClamp);
	}


	if (drymix < 0.001)
	{
		do 
		{
			double const s = *psamples;

			if (s >= threshold)
				*psamples = clamp;
			else if (s <= negthreshold)
				*psamples = negclamp;

			psamples++;
			
		} while(--numsamples);
	}
	else
	{
		do 
		{
			double const s = *psamples;

			if (s >= threshold)
				*psamples = (float)(s * drymix + clamp);
			else if (s <= negthreshold)
				*psamples = (float)(s * drymix + negclamp);

			psamples++;
			
		} while(--numsamples);
	}

	return true;
}

int main(int argc, char* argv[])
{
	char buffer[1024];	// intermediate buffer for reading
	int result;
	int readBytes = 1;
	int bufflen = sizeof(buffer);
	
	result = setmode ( fileno ( stdin ), O_BINARY );
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

	while(readBytes > 0) {
		readBytes = fread( buffer, sizeof(char), bufflen, stdin );

		if(readBytes == 0)
			break;

		if(readBytes < bufflen) 
			memset(buffer + readBytes, 0, bufflen - readBytes);

		work((short*)buffer, readBytes/2);
		
		UINT n = fwrite( buffer, 1, bufflen, stdout );
	}

	return 0;
}

