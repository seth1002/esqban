// LanPhone.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <io.h>
#include <assert.h>

#include "sender.h"

#include "args.h"

#define HOSTNAME			"localhost"
#define DESTINATION_PORT	5000  
#define MAX_DATA_LEN		80
#define MAX_USER_LEN		10


typedef struct tagData
{
	char user[MAX_USER_LEN];
	char data[MAX_DATA_LEN];
	int  size;
} DataStruct;



int main(int argc, char* argv[])
{
	int readBytes = 1;
	char *hostname;
	char *username;
	int port;

	if( setmode ( fileno ( stdin ), O_BINARY ) == -1 )
	{
		fprintf(stderr, "Cannot set mode\n" );
		return -1;
	}

	username = GetArg(argc, argv, "u", "sqba");
	hostname = GetArg(argc, argv, "h", HOSTNAME);
	port = GetArg(argc, argv, "p", DESTINATION_PORT);
	
//	tmp = GetArg(argc, argv, "ip", IP_ADDRESS);
//	len = (strlen(tmp) > 15 ? 15 : strlen(tmp));
//	strncpy(szIP, tmp, len);

	// Initialize Winsock. 
	WSADATA WSAData;	// Contains details of the Winsock implementation
	if (WSAStartup (MAKEWORD(1,1), &WSAData) != 0) 
	{
		TCHAR szError[100]={0};	// Error message string
		wsprintf (szError, TEXT("WSAStartup failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}

	CSender sender(hostname, port);

	//FILE *fdbg = fopen("send.raw", "w+b");

	DataStruct d;
	strncpy(d.user, username, 10);
	while(readBytes > 0) {
		memset(d.data, 0, MAX_DATA_LEN);
		readBytes = fread( d.data, sizeof(char), MAX_DATA_LEN, stdin );
		
		if(readBytes > 0)
		{
			//if(readBytes < MAX_DATA_LEN) 
			//	memset(d.data + readBytes, 0, MAX_DATA_LEN - readBytes);

			d.size = readBytes;
			sender.Send((char*)&d, sizeof(DataStruct));

			//fwrite(&d, 1, sizeof(DataStruct), fdbg);
		}
	}

	//fclose(fdbg);

	WSACleanup ();

	return TRUE;
}
