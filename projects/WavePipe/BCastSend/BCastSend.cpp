// LanPhone.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <io.h>
#include <assert.h>

#include "sender.h"

#include "args.h"

#define IP_ADDRESS			"225.6.7.8"
#define SOURCE_PORT			0     
#define DESTINATION_PORT	2000  
#define MAX_DATA_LEN		80
#define MAX_USER_LEN		10


typedef struct tagData
{
	char user[MAX_USER_LEN];
	char data[MAX_DATA_LEN];
	int  size;
} DataStruct;


char	gUser[MAX_USER_LEN]	= {0};
int		gSourcePort			= SOURCE_PORT;
int		gDestPort			= DESTINATION_PORT;
char	szIP[15]			= IP_ADDRESS;


int main(int argc, char* argv[])
{
	int readBytes = 1;

	if( setmode ( fileno ( stdin ), O_BINARY ) == -1 )
	{
		fprintf(stderr, "Cannot set mode\n" );
		return -1;
	}

	char *tmp = GetArg(argc, argv, "u", "sqba");
	int len = (strlen(tmp) > MAX_USER_LEN ? MAX_USER_LEN : strlen(tmp));
	strncpy(gUser, tmp, len);

	gSourcePort = GetArg(argc, argv, "src", SOURCE_PORT);
	gDestPort = GetArg(argc, argv, "dst", DESTINATION_PORT);
	
	tmp = GetArg(argc, argv, "ip", IP_ADDRESS);
	len = (strlen(tmp) > 15 ? 15 : strlen(tmp));
	strncpy(szIP, tmp, len);

	// Initialize Winsock. 
	WSADATA WSAData;	// Contains details of the Winsock implementation
	if (WSAStartup (MAKEWORD(1,1), &WSAData) != 0) 
	{
		TCHAR szError[100]={0};	// Error message string
		wsprintf (szError, TEXT("WSAStartup failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}

	CSender sender(gSourcePort, gDestPort, szIP);

	//FILE *fdbg = fopen("send.raw", "w+b");

	DataStruct d;
	strcpy(d.user, gUser);
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
