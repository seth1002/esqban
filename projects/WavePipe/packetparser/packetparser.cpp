// packetparser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#define MAX_DATA_LEN		10
#define MAX_USER_LEN		10


typedef struct tagData
{
	char user[MAX_USER_LEN];
	char data[MAX_DATA_LEN];
	int size;
} DataStruct;


int main(int argc, char* argv[])
{
	int readBytes = 1;

	if( -1 == setmode ( fileno ( stdin ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	DataStruct data;
	while(readBytes > 0) {
		readBytes = fread( &data, 1, sizeof(DataStruct), stdin );
		
		fwrite(data.data, 1, data.size, stdout);
	}

	return 0;
}
