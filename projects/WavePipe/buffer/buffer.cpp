// buffer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#define BUFF_LEN	1024

int main(int argc, char* argv[])
{
	int readBytes = 1;
	char buff[BUFF_LEN];
	int pos = 0;

	if( -1 == setmode ( fileno ( stdin ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	if( -1 == setmode ( fileno ( stdout ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	while(readBytes > 0) {
		readBytes = fread( buff+pos, 1, BUFF_LEN-pos, stdin );
		
		fwrite(buff+pos, 1, readBytes, stdout);
	}

	return 0;
}
