// Echo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>





int main(int argc, char* argv[])
{
	char buffer[1024];	// intermediate buffer for reading
	int readBytes = 1;
	int result;
	int bufflen = sizeof(buffer);
	int i=0;

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

	while(readBytes > 0) 
	{
		readBytes = fread( buffer, sizeof(char), bufflen, stdin );

		for(i=0; i<readBytes; i++)
		{
			buffer[i] = 65535 - buffer[i];
		}

		UINT n = fwrite( buffer, 1, readBytes, stdout );
	}

	return 0;
}

