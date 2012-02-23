// PitchShift.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>


#define BUF_LENGTH	8192
//#define BUF_LENGTH	1024
//#define FRAME_SIZE	BUF_LENGTH/4

void smbPitchShift(float pitchShift, long numSampsToProcess, long fftFrameSize, long osamp, float sampleRate, float *indata, float *outdata);

int main(int argc, char* argv[])
{
	short buffer[BUF_LENGTH*2];	// intermediate buffer for reading
	float lbuffer[BUF_LENGTH];	// left
	float rbuffer[BUF_LENGTH];	// right
	int readBytes = 1, readSamples = 0;
	int bufflen = sizeof(buffer);
	int i=0, n=0;

	if( setmode ( fileno ( stdin ), O_BINARY ) == -1 )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	if( setmode ( fileno ( stdout ), O_BINARY ) == -1 )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	long semitones = 3;									// shift up by 3 semitones
	float pitchShift = (float)pow(2., semitones/12.);	// convert semitones to factor

	int nSampleRate = 44100;

	while(readBytes > 0) 
	{
		readBytes = fread( buffer, sizeof(char), bufflen, stdin );

		readSamples = readBytes / sizeof(short);

		n=0;
		for(i=0; i<readSamples; i+=2)
		{
			lbuffer[n++] = (float)buffer[i];
		}

		// --------------------------------- call smbPitchShift() ---------------------------------
		//smbPitchShift(pitchShift, BUF_LENGTH, 2048, 4, nSampleRate, data, data);
		smbPitchShift(pitchShift, BUF_LENGTH, BUF_LENGTH/4, 4, nSampleRate, lbuffer, lbuffer);
		// ----------------------------------------------------------------------------------------

		n=0;
		for(i=1; i<readSamples; i+=2)
		{
			rbuffer[n++] = (float)buffer[i];
		}

		// --------------------------------- call smbPitchShift() ---------------------------------
		//smbPitchShift(pitchShift, BUF_LENGTH, 2048, 4, nSampleRate, data, data);
		smbPitchShift(pitchShift, BUF_LENGTH, BUF_LENGTH/4, 4, nSampleRate, rbuffer, rbuffer);
		// ----------------------------------------------------------------------------------------

		for(i=0; i<readSamples/2; i++)
		{
			buffer[i]   = (short)lbuffer[i];
			buffer[i+1] = (short)rbuffer[i];
		}

		fwrite( buffer, 1, readBytes, stdout );
	}

	return 0;
}
