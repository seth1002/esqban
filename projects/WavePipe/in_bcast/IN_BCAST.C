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
#include "receiver.h"

#define WM_WA_MPEG_EOF WM_USER+2


extern In_Module mod;
char lastfn[MAX_PATH];
short sample_buffer[576*2];


// avoid CRT. Evil. Big. Bloated.
BOOL WINAPI _DllMainCRTStartup(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}

int paused;
//DWORD WINAPI __stdcall PlayThread(void *b);

char username[10];
CReceiver *receiver;

//int killDecodeThread=0;
//HANDLE thread_handle=INVALID_HANDLE_VALUE;

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
	char buf[8];
	memcpy(buf,fn,7);
	buf[7]=0;
	return lstrcmp(buf,"bcast://")?0:1; 
}

void OnReceiveData(const char *data, int size);

int play(char *fn) { 
	int maxlatency;
	int tmp;
	strcpy(lastfn,fn);
	// simple atoi() inline so that we don't have to use libc (for a nice 4k dll)
	{
		char *s=fn+8;
		memset(username, 0, 10);
		strcpy(username, s);
	}
	paused=0;
	memset(sample_buffer,0,sizeof(sample_buffer));

	maxlatency = mod.outMod->Open(44100,1,16, -1,-1);
	if (maxlatency < 0)
	{
		return 1;
	}
	mod.SetInfo(0,44,1,1);
	mod.SAVSAInit(maxlatency,44100);
	mod.VSASetInfo(44100,1);
	mod.outMod->SetVolume(-666);		
/*
	killDecodeThread=0;
	thread_handle = (HANDLE) CreateThread(NULL,0,(LPTHREAD_START_ROUTINE) PlayThread,(void *) &killDecodeThread,0,&tmp);
*/
	receiver = new CReceiver(gSourcePort, gDestPort, szIP);
	receiver->SetOnReceiveData( OnReceiveData );
	receiver->Start();
	
	return 0; 
}

void OnReceiveData(const char *data, int size)
{
	DataStruct *d = (DataStruct*)data;
	//fwrite(d, 1, sizeof(DataStruct), fdbg);
	int n = strlen(gUser);
	if(0 == strncmp(d->user, gUser, strlen(gUser)))
	{
		UINT n = fwrite( d->data, sizeof(char), d->size, stdout );
		//assert(n==size);
	}
}

void pause() { paused=1; mod.outMod->Pause(1); }
void unpause() { paused=0; mod.outMod->Pause(0); }
int ispaused() { return paused; }

void stop() { 
/*
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
*/
	delete receiver;

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
