// Mp3Encoder.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <tchar.h>
#include "lame\lame_enc.h"


BEINITSTREAM	beInitStream;
BEENCODECHUNK	beEncodeChunk;
BEDEINITSTREAM	beDeinitStream;
BECLOSESTREAM	beCloseStream;
BEVERSION		beVersion;

HINSTANCE m_hDllInstance;
HINSTANCE m_hinstLib;

FILE *out = NULL;

void encode()
{
	// initialization of Mp3 encoder
	BE_CONFIG bc;
	memset(&bc, 0, sizeof(BE_CONFIG));
	bc.dwConfig=BE_CONFIG_MP3;
	bc.format.mp3.dwSampleRate=44100;
	bc.format.mp3.byMode=BE_MP3_MODE_STEREO;
	bc.format.mp3.wBitrate=192;
	bc.format.mp3.bCopyright=false;
	bc.format.mp3.bCRC=false;
	bc.format.mp3.bOriginal=false;
	bc.format.mp3.bPrivate=false;

	DWORD dwNumberOfSamples=0;
	DWORD dwOutputBufferLength=0;
	HBE_STREAM hStream;
	if(BE_ERR_SUCCESSFUL != beInitStream(&bc,&dwNumberOfSamples,&dwOutputBufferLength,&hStream))
	{
		return;
	}

	char *Mp3Buffer = new char[dwOutputBufferLength];
	SHORT *InputBuffer=new SHORT[dwNumberOfSamples];      //SHORT=short=16 bits

	DWORD dwNumberOfSamplesEncoded;

	int readBytes = 1;
	do {
		readBytes = fread( InputBuffer, 1, dwNumberOfSamples*sizeof(SHORT), stdin );

		if(BE_ERR_SUCCESSFUL != beEncodeChunk(hStream,readBytes/sizeof(SHORT),InputBuffer,(BYTE*)Mp3Buffer,&dwNumberOfSamplesEncoded))
		{
			delete Mp3Buffer;
			delete InputBuffer;
			return;
		}
		//fprintf(stderr, "dwOutputBufferLength=%d dwNumberOfSamplesEncoded=%d\n", dwOutputBufferLength, dwNumberOfSamplesEncoded);
		fwrite(Mp3Buffer, 1, dwNumberOfSamplesEncoded, out);
	} while(readBytes > 0);

	beDeinitStream(hStream,(BYTE*)Mp3Buffer,&dwNumberOfSamplesEncoded);
	beCloseStream(hStream);
	
	delete Mp3Buffer;
	delete InputBuffer;
}

int main(int argc, char* argv[])
{
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


	out = (argc==2 ? fopen(argv[1], "w+b") : stdout);
	
	m_hinstLib = NULL;

	m_hDllInstance = GetModuleHandle(NULL);

	m_hinstLib = LoadLibrary("LAME_ENC.DLL");
	if (m_hinstLib != NULL) 
	{ 
		beInitStream = (BEINITSTREAM)GetProcAddress(m_hinstLib, TEXT_BEINITSTREAM); 
		beEncodeChunk = (BEENCODECHUNK)GetProcAddress(m_hinstLib, TEXT_BEENCODECHUNK); 
		beDeinitStream = (BEDEINITSTREAM)GetProcAddress(m_hinstLib, TEXT_BEDEINITSTREAM); 
		beCloseStream = (BECLOSESTREAM)GetProcAddress(m_hinstLib, TEXT_BECLOSESTREAM);
		beVersion = (BEVERSION)GetProcAddress(m_hinstLib, TEXT_BEVERSION);

		encode();

		FreeLibrary(m_hinstLib); 
	}

	if(out!=NULL && out!=stdout)
		fclose(out);

	return 0;
}
