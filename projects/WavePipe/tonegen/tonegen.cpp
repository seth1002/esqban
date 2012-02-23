// tonegen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>
#include <conio.h>


#define M_PI 3.14159265358979323846


DWORD WINAPI ThreadProc(LPVOID pParam);
int atoi(char *s);


BOOL bStop = FALSE;
HANDLE ghThread;
DWORD gdwThreadId;

double freq			= 2600.0f;
double dangle;// = M_PI * (double)nChannels * freq / (double)nSamplesPerSec / 2.0;

int nSamplesPerSec	= 44100;
int nChannels		= 2;
int nBitsPerSample		= 16;

int main(int argc, char* argv[])
{

	if( -1 == setmode ( fileno ( stdout ), O_BINARY ) )
	{
		perror( "Cannot set output file translation mode" );
		return -1;
	}

	if(argc == 2)
		freq = atoi(argv[1]);

	dangle = M_PI * (double)nChannels * freq / (double)nSamplesPerSec / 2.0;

    ghThread = CreateThread(NULL,			// no security attributes 
							0,				// use default stack size  
							ThreadProc,		// thread function 
							NULL,			// argument to thread function 
							0,				// use default creation flags 
							&gdwThreadId);	// returns the thread identifier 

	fprintf(stderr, "Generator has started. Press ENTER key to stop...\n");
	int c = 0;
	while((c=getch()) != '\n')
	{
		switch(c)
		{
		case'+':
		case'a':
		case'A':
			//freq++;
			dangle = M_PI * (double)nChannels * ++freq / (double)nSamplesPerSec / 2.0;
			//fprintf(stderr, "freq: %fHz\n", freq);
			break;
		case'-':
		case'z':
		case'Z':
			//freq--;
			dangle = M_PI * (double)nChannels * --freq / (double)nSamplesPerSec / 2.0;
			//fprintf(stderr, "freq: %fHz\n", freq);
			break;
		default:
			bStop = TRUE;
			return 0;
		}
	}

	bStop = TRUE;

	return 0;
}

int atoi(char *s)
{
	int n=0;
	while (s != NULL && *s >= '0' && *s <= '9')
	{
		n*=10;
		n += *s++ -'0';
	}
	return n;
}

DWORD WINAPI ThreadProc(LPVOID pParam)
{
	//short sample_buffer[576*2];
	short sample_buffer[8*2];

	double angle=0.0;
	int l = sizeof(sample_buffer)/2, x=l/2;
	//double dangle = M_PI * (double)nChannels * freq / (double)nSamplesPerSec / 2.0;

	double max = (pow(2, nBitsPerSample) - 1.0) / 2.0;

	while (!bStop) 
	{
		x=l/2;
		memset(sample_buffer, 0, sizeof(sample_buffer));
		short *s=sample_buffer;
		while (x--)
		{
			int i;
			double d = sin(angle)*max;
			//double d = sin(angle)*32766.5;
			__asm {
				fld d
				fistp i
			}
			*s++=i;
			angle += dangle;
		}

		fwrite(sample_buffer, 1, l, stdout);
	}

	return 0;
}
