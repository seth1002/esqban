// WaveOut.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <tchar.h>


struct WAVEFORM
{
	WORD  wFormatTag;		//format of digital sound
	WORD  nChannels;		//Number of channels (1 for mono and 2 for stereo)
	DWORD nSamplesPerSec;	//Number of samples per second
	DWORD nAvgBytesPerSec;	//Average number bytes of data per second
	WORD  nBlockAlign;		//Minimal data size for playing
	WORD  wBitsPerSample;	//Bits per sample (8 or 16)
};



struct RIFF
{
	_TCHAR riffID[4];		//contains identifier "RIFF"
	DWORD  riffSIZE;		//File size minus 8 bytes
	_TCHAR riffFORMAT[4];	//contains identifier "WAVE"
};

struct FMT
{
	_TCHAR   fmtID[4];		//contains identifier: "fmt " (with space)
	DWORD    fmtSIZE;		//contains the size of this block (for WAVE PCM 16)
	WAVEFORM fmtFORMAT;		//structure WAVEFORMATEX but without cbSize field
};

struct DATA
{
	_TCHAR dataID[4];		//contains identifier: "data"
	DWORD  dataSIZE;		//data size
};


void writeWav(char *filename, BOOL bOut)
{
	short buffer[1024];	// intermediate buffer for reading
	int readBytes=1;

	FILE *fp = fopen(filename, "wb");

	int i = fseek(fp, 0, SEEK_SET);

	RIFF riff;
	memset(&riff, 0, sizeof(RIFF));
	strcpy(riff.riffID, TEXT("RIFF"));
	//riff.riffSIZE = 0;//File size minus 8 bytes
	strcpy(riff.riffFORMAT, TEXT("WAVE"));
	UINT written = fwrite(&riff, sizeof(RIFF), 1, fp);

	FMT fmt;
	memset(&fmt, 0, sizeof(FMT));
	strcpy(fmt.fmtID, TEXT("fmt "));
	fmt.fmtSIZE = 16;

	memset(&fmt.fmtFORMAT, 0, sizeof(WAVEFORM));

	// set up the WAVEFORM structure.
	fmt.fmtFORMAT.nSamplesPerSec  = 44100;	// sample rate
	fmt.fmtFORMAT.wBitsPerSample  = 16;		// sample size
	fmt.fmtFORMAT.nChannels       = 2;		// channels
	fmt.fmtFORMAT.wFormatTag      = WAVE_FORMAT_PCM;
	fmt.fmtFORMAT.nBlockAlign     = (fmt.fmtFORMAT.wBitsPerSample * fmt.fmtFORMAT.nChannels) >> 3;
	fmt.fmtFORMAT.nAvgBytesPerSec = fmt.fmtFORMAT.nBlockAlign * fmt.fmtFORMAT.nSamplesPerSec;
		
	fwrite(&fmt, sizeof(FMT), 1, fp);

	DATA data;
	strcpy(data.dataID, TEXT("data"));
	data.dataSIZE = 0;
	fwrite(&data, sizeof(DATA), 1, fp);

	while(readBytes > 0) {
		readBytes = fread( buffer, sizeof( char ), sizeof(buffer), stdin );
		if(bOut)
			fwrite(buffer, readBytes, 1, stdout);
		written = fwrite(buffer, readBytes, 1, fp);
		data.dataSIZE += written * readBytes;
		fflush(fp);
	}

	riff.riffSIZE = data.dataSIZE + sizeof(RIFF) + sizeof(FMT) + sizeof(DATA);
	fseek(fp, 0, SEEK_SET);
	fwrite(&riff, sizeof(RIFF), 1, fp);
	fwrite(&fmt, sizeof(FMT), 1, fp);
	fwrite(&data, sizeof(DATA), 1, fp);

	fclose(fp);
}

void readWav(char *filename)
{
	short buffer[1024];	// intermediate buffer for reading
	int readBytes=1;

	RIFF riff;
	FMT fmt;
	DATA data;

	FILE *fp = fopen(filename, "rb");

	fread( &riff, 1, sizeof(RIFF), fp );
	fread( &fmt, 1, sizeof(FMT), fp );
	fread( &data, 1, sizeof(DATA), fp );

	fprintf(stderr, "\nFile: %s\n", filename);
	fprintf(stderr, "RIFF\n");
	fprintf(stderr, " riffID           = %s\n", riff.riffID );
	fprintf(stderr, " riffSIZE         = %d\n", riff.riffSIZE );
	fprintf(stderr, " riffFORMAT       = %s\n", riff.riffFORMAT );

	fprintf(stderr, "FMT\n");
	fprintf(stderr, " fmtFORMAT        = %s\n", fmt.fmtID );
	fprintf(stderr, " fmtSIZE          = %d\n", fmt.fmtSIZE);
	
	fprintf(stderr, " WAVEFORM\n");
	fprintf(stderr, "  wFormatTag      = %d\n", fmt.fmtFORMAT.wFormatTag);
	fprintf(stderr, "  nChannels       = %d\n", fmt.fmtFORMAT.nChannels);
	fprintf(stderr, "  nSamplesPerSec  = %d\n", fmt.fmtFORMAT.nSamplesPerSec);
	fprintf(stderr, "  nAvgBytesPerSec = %d\n", fmt.fmtFORMAT.nAvgBytesPerSec);
	fprintf(stderr, "  nBlockAlign     = %d\n", fmt.fmtFORMAT.nBlockAlign);
	fprintf(stderr, "  wBitsPerSample  = %d\n", fmt.fmtFORMAT.wBitsPerSample);

	fprintf(stderr, "DATA\n");
	fprintf(stderr, " dataID           = %s\n", data.dataID);
	fprintf(stderr, " dataSIZE         = %d\n", data.dataSIZE);

	while(readBytes > 0) {
		readBytes = fread( buffer, 1, sizeof(buffer), fp );
		fwrite(buffer, 1, readBytes, stdout);
	}

	fclose(fp);
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

	char *filename;
	
	if(argc == 3 && argv[1][0] == 'i')
	{
		filename = argv[2];
		readWav( filename );
	}
	else
	{
		BOOL bOut = FALSE;
		if(argc == 3 && argv[1][0] == 'o')
		{
			bOut = TRUE;
			filename = argv[2];;
		}
		else
			filename = (argc==2?argv[1]:TEXT("out.wav"));
		writeWav( filename, bOut );
	}

	return 0;
}
