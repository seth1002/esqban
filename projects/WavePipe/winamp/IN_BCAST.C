/*
** Broadcast receiver plug-in
** Copyright (c) 1998, sqba.
*/

//kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib MSVCRT.LIB wsock32.lib 

#include <windows.h>
#include <mmreg.h>
#include <msacm.h>
#include <math.h>

#include "in2.h"

#define WM_WA_MPEG_EOF WM_USER+2


//#define IP_ADDRESS			"225.6.7.8"
//#define SOURCE_PORT			0     
//#define DESTINATION_PORT	2000  
#define MAX_DATA_LEN		80
#define MAX_USER_LEN		10
//#define BUFFER_SIZE			576*2


typedef struct tagData
{
	char user[MAX_USER_LEN];
	char data[MAX_DATA_LEN];
	int size;
} DataStruct;



extern In_Module mod;
char lastfn[MAX_PATH];
//short sample_buffer[576*2];
//char sample_buffer[BUFFER_SIZE];
//int buff_pos;


// avoid CRT. Evil. Big. Bloated.
BOOL WINAPI _DllMainCRTStartup(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}

int paused;
DWORD WINAPI __stdcall PlayThread(void *b);


int iSrcPort=0;
int iDestPort=2000;
char *pszIP="225.6.7.8";
char username[10];
SOCKET m_Sock;			// Datagram window socket
SOCKADDR_IN local_sin,	// Local socket's address
			recv_sin;	// Holds the source address upon 
						// recvfrom function returns


int killDecodeThread=0;
HANDLE thread_handle=INVALID_HANDLE_VALUE;

void config(HWND hwndParent)
{
	MessageBox(hwndParent,"To use, open 'bcast://user'.","Configuration",MB_OK);
}
void about(HWND hwndParent)
{
	MessageBox(hwndParent,"Broadcast Receiver, by sqba","About Broadcast Receiver",MB_OK);
}

void init() { }

void quit() { }

int isourfile(char *fn) 
{ 
	char buf[9];
	memcpy(buf,fn,8);
	buf[8]=0;
	return lstrcmp(buf,"bcast://")?0:1; 
}

void OnReceiveData(const char *data, int size);

int play(char *fn)
{ 
	int index = 0;					// Integer index
	char szMessage[100]={0};		// ASCII string 
	TCHAR szError[100]={0};			// Error message string

	struct ip_mreq mreq;			// Used in adding or dropping 
									// multicasting addresses
//	DWORD dwThreadId = 0;
//	HANDLE hThread;

	int maxlatency;
	int tmp;
	strcpy(lastfn,fn);
	{
		char *s=fn+8;
		memset(username, 0, 10);
		strcpy(username, s);
	}
	paused=0;
//	memset(sample_buffer,0,sizeof(sample_buffer));
//	buff_pos=0;

	maxlatency = mod.outMod->Open(44100,2,16, -1,-1);
	if (maxlatency < 0)
	{
		return 1;
	}
	mod.SetInfo(0,44,1,1);
	mod.SAVSAInit(maxlatency,44100);
	mod.VSASetInfo(44100,2);
	//mod.VSASetInfo(2, 44100);
	mod.outMod->SetVolume(-666);		

//	killDecodeThread=0;
//	thread_handle = (HANDLE) CreateThread(NULL,0,(LPTHREAD_START_ROUTINE) PlayThread,(void *) &killDecodeThread,0,&tmp);


	//////////////////////////////////////////////////////////////////////////////////
	m_Sock = INVALID_SOCKET;


	// Create a datagram socket, Sock.
	if ((m_Sock = socket (AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) 
	{
		wsprintf (szError, TEXT("Allocating socket failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}
	
	// Fill out the local socket's address information.
	local_sin.sin_family = AF_INET;
	local_sin.sin_port = htons (iDestPort);  
	local_sin.sin_addr.s_addr = htonl (INADDR_ANY);
	
	// Associate the local address with Sock.
	if (bind (m_Sock, (struct sockaddr FAR *) &local_sin, sizeof (local_sin)) == SOCKET_ERROR) 
	{
		wsprintf (szError, TEXT("Binding socket failed! Error: %d"), WSAGetLastError ());
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_Sock);
		return FALSE;
	}
	
	// Join the multicast group from which to receive datagrams.
	mreq.imr_multiaddr.s_addr = inet_addr (pszIP);
	mreq.imr_interface.s_addr = INADDR_ANY;
	
	if (setsockopt (m_Sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char FAR *)&mreq, sizeof (mreq)) == SOCKET_ERROR)
	{
		wsprintf (szError, TEXT("setsockopt failed! Error: %d"), WSAGetLastError ());
		//ErrorHandlerEx(__LINE__, __FILE__);
		MessageBox (NULL, szError, TEXT("Error"), MB_OK);
		closesocket (m_Sock);
		return FALSE;
	}


	killDecodeThread=0;
//	hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)PlayThread, (void *) &killDecodeThread, 0, &dwThreadId);
	thread_handle = (HANDLE) CreateThread(NULL,0,(LPTHREAD_START_ROUTINE) PlayThread,(void *) &killDecodeThread,0,&tmp);
	//////////////////////////////////////////////////////////////////////////////////


	return 0; 
}

void pause() { paused=1; mod.outMod->Pause(1); }
void unpause() { paused=0; mod.outMod->Pause(0); }
int ispaused() { return paused; }

void stop()
{
	// Disable receiving on Sock before closing it.
	shutdown (m_Sock, 0x00);
	
	// Close Sock.
	closesocket (m_Sock);

	if (thread_handle != INVALID_HANDLE_VALUE)
	{
		killDecodeThread=1;
		if (WaitForSingleObject(thread_handle,INFINITE) == WAIT_TIMEOUT)
		{
			MessageBox(mod.hMainWindow,"error asking thread to die!\n","error killing decode thread",0);
			TerminateThread(thread_handle,0);
		}
		CloseHandle(thread_handle);
		thread_handle = INVALID_HANDLE_VALUE;
	}

	mod.outMod->Close();
	mod.SAVSADeInit();
}

int getlength() { return -1000; }
int getoutputtime() { return mod.outMod->GetOutputTime(); }
void setoutputtime(int time_in_ms) {  }

void setvolume(int volume) { mod.outMod->SetVolume(volume); }
void setpan(int pan) { mod.outMod->SetPan(pan); }

int infoDlg(char *fn, HWND hwnd)
{
	return 0;
}

void getfileinfo(char *filename, char *title, int *length_in_ms)
{
	if (!filename || !*filename) filename=lastfn;
	if (title) 
	{
		char *s=filename+8;
		wsprintf(title,"User: %s",s);
	}
	if (length_in_ms) *length_in_ms=-1000;
}

void eq_set(int on, char data[10], int preamp) 
{ 
}


In_Module mod = 
{
	IN_VER,
	"Broadcast Receiver v0.5 "
#ifdef __alpha
	"(AXP)"
#else
	"(x86)"
#endif
	,
	0,	// hMainWindow
	0,  // hDllInstance
	"\0"
	,
	0,	// is_seekable
	1, // uses output
	config,
	about,
	init,
	quit,
	getfileinfo,
	infoDlg,
	isourfile,
	play,
	pause,
	unpause,
	ispaused,
	stop,
	
	getlength,
	getoutputtime,
	setoutputtime,

	setvolume,
	setpan,

	0,0,0,0,0,0,0,0,0, // vis stuff


	0,0, // dsp

	eq_set,

	NULL,		// setinfo

	0 // out_mod

};

__declspec( dllexport ) In_Module * winampGetInModule2()
{
	return &mod;
}

int _fltused=0;


DWORD WINAPI PlayThread(void *b)
{
	int iRecvLen;				// Length of recv_sin
	char buff[100] = {0};
	int l;

	iRecvLen = sizeof(recv_sin);
	
	//while(0 != strncmp(buff, "exit", 4))
	while (! *((int *)b) ) 
	{
		// Receive data from the multicasting group server.
		if (SOCKET_ERROR == recvfrom(m_Sock, buff, 100, 0, (struct sockaddr FAR *) &recv_sin, &iRecvLen))
		{
			TCHAR szError[100] = {0};	// Error message string
			//ErrorHandlerEx(__LINE__, __FILE__);
			wsprintf (szError, TEXT("recvfrom failed! Error: %d"), WSAGetLastError ());
			MessageBox (NULL, szError, TEXT("Error"), MB_OK);
			closesocket (m_Sock);
			return(0);
		}
		else
		{
			const char *data = buff;//, strlen(buff)
			DataStruct *d = (DataStruct*)data;
			int n = strlen(username);
			if(0 == strncmp(d->user, username, strlen(username)))
			{
				/*if(buff_pos <= (BUFFER_SIZE - d->size)) {
					memcpy(sample_buffer, d->data, d->size);
					buff_pos += d->size;
				} else {
					if (mod.outMod->CanWrite() >= ((sizeof(sample_buffer)/2)<<(mod.dsp_isactive()?1:0))) {
						{
							int t=mod.outMod->GetWrittenTime();
							mod.SAAddPCMData((char *)sample_buffer,2,16,t);
							mod.VSAAddPCMData((char *)sample_buffer,2,16,t);
						}
						l=sizeof(sample_buffer);
						l=mod.dsp_dosamples(sample_buffer,l,16,2,44100)*2;
						mod.outMod->Write(sample_buffer,l);

						memset(sample_buffer,0,sizeof(sample_buffer));
						buff_pos=0;
					} else
						Sleep(50);
				}*/

				if (mod.outMod->CanWrite() >= ((sizeof(d->data)/2)<<(mod.dsp_isactive()?1:0))) {
					//{
					//	int t=mod.outMod->GetWrittenTime();
					//	mod.SAAddPCMData(d->data,2,16,t);
					//	mod.VSAAddPCMData(d->data,2,16,t);
					//}
					l=d->size/2;
					l=mod.dsp_dosamples(d->data,l,16,2,44100)*2;
					mod.outMod->Write(d->data,l);
				} else
					Sleep(50);

			}
		}
	}

	return(0);
}


/*
DWORD WINAPI __stdcall PlayThread(void *b)
{
	double angle=0.0,dangle=3.14159*2.0*(double)gen_freq/(double)44100.0;
	while (! *((int *)b) ) 
	{
		if (mod.outMod->CanWrite() >= ((sizeof(sample_buffer)/2)<<(mod.dsp_isactive()?1:0)))
		{	
			int l=sizeof(sample_buffer)/2,x=l/2;
			short *s=sample_buffer;
			while (x--)
			{
				int i;
				double d = sin(angle)*32766.5;
				__asm {
					fld d
					fistp i
				}
				*s++=i;
				angle += dangle;
			}
			{
				int t=mod.outMod->GetWrittenTime();
				mod.SAAddPCMData((char *)sample_buffer,1,16,t);
				mod.VSAAddPCMData((char *)sample_buffer,1,16,t);
			}
			l=mod.dsp_dosamples(sample_buffer,l/2,16,1,44100)*2;
			mod.outMod->Write(sample_buffer,l);
		}
		else Sleep(50);
	}
	return 0;
}
*/
