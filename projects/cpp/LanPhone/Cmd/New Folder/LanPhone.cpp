// LanPhone.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <math.h>

#include "sender.h"
#include "receiver.h"

#include "wavein.h"
#include "waveout.h"

#include "args.h"

#define IP_ADDRESS			"225.6.7.8"
#define SOURCE_PORT			0     
#define DESTINATION_PORT	2000  
#define MAX_DATA_LEN		10
#define MAX_USER_LEN		10

typedef struct tagData
{
	char user[MAX_USER_LEN];
	char data[MAX_DATA_LEN];
	int size;
} DataStruct;


CSender		*gpSender;
CReceiver	*gpReceiver;
//CWaveIn		*gpWaveIn;
CWaveOut	*gpWaveOut;
char gUser[MAX_USER_LEN]={0};
int gSourcePort = SOURCE_PORT;
int gDestPort = DESTINATION_PORT;
char szIP[15] = IP_ADDRESS;




void OnReceiveData(const char *data, int size)
{
	DataStruct *d = (DataStruct*)data;
	int n = strlen(gUser);
	if(0 != strncmp(d->user, gUser, strlen(gUser)))
	{
		gpWaveOut->Write(d->data, d->size);
		//fprintf(stdout, "%s", d->data);
	}
}

int OnSendData(const char *data, int size)
{
	int result=0;
	char *ptr = (char*)data;

	div_t div_result = div(size, MAX_DATA_LEN);

	for(int i=0; i<div_result.quot; i++)
	{
		DataStruct d;
		memset(&d, 0, sizeof(DataStruct));
		strncpy(d.user, gUser, strlen(gUser));
		memcpy(d.data, ptr, MAX_DATA_LEN);
		d.size = MAX_DATA_LEN;
		ptr += MAX_DATA_LEN;
		result += d.size;
		gpSender->Send((char*)&d, sizeof(DataStruct));
	}

	if(div_result.rem > 0)
	{
		DataStruct d;
		memset(&d, 0, sizeof(DataStruct));
		strncpy(d.user, gUser, strlen(gUser));
		memcpy(d.data, data, div_result.rem);
		d.size = div_result.rem;
		result += d.size;
		gpSender->Send((char*)&d, sizeof(DataStruct));
	}

	return result;
}

void Start()
{
	CWaveIn wavein(OnSendData);
	wavein.SetOnSendData(OnSendData);
	wavein.Start();
/*
	char szMessage[100] = {0};
	while(NULL != fgets(szMessage, 100, stdin))
	{
		OnSendData(szMessage, strlen(szMessage));
		strnset(szMessage, 0, 100);
	}
*/
}

int main(int argc, char* argv[])
{
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

//	if(argc > 1 && argv[1][0] == 's')
//	{
		CSender sender(gSourcePort, gDestPort, szIP);
		gpSender = &sender;
		Start();
//	}
//	else {
		CWaveOut waveout;
		gpWaveOut = &waveout;

		CReceiver receiver(gSourcePort, gDestPort, szIP);
		gpReceiver = &receiver;
		receiver.SetOnReceiveData( OnReceiveData );
		receiver.Start();
//	}

	while(1){}

	WSACleanup ();

	return TRUE;
}
