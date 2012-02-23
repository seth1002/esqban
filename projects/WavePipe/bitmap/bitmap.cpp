// Echo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>



/*
// BITMAPFILEHEADER Aray
typedef struct tagBITMAPFILEHEADER { 
  WORD    bfType;		// Specifies the file type, must be BM.
  DWORD   bfSize;		// Specifies the size, in bytes, of the bitmap file.
  WORD    bfReserved1;	// Reserved; must be zero.
  WORD    bfReserved2;	// Reserved; must be zero.
  DWORD   bfOffBits;	// Specifies the offset, in bytes, from the BITMAPFILEHEADER structure to the bitmap bits.
} BITMAPFILEHEADER, *PBITMAPFILEHEADER; 

// BITMAPINFOHEADER Aray
typedef struct tagBITMAPINFOHEADER{
  DWORD  biSize;			// Specifies the number of bytes required by the structure. 
  LONG   biWidth;			// Specifies the width of the bitmap, in pixels.
  LONG   biHeight; 
  WORD   biPlanes;			// Specifies the number of planes for the target device. This value must be set to 1.
  WORD   biBitCount; 
  DWORD  biCompression; 
  DWORD  biSizeImage; 
  LONG   biXPelsPerMeter; 
  LONG   biYPelsPerMeter; 
  DWORD  biClrUsed; 
  DWORD  biClrImportant; 
} BITMAPINFOHEADER, *PBITMAPINFOHEADER; 

// RGBQUAD Aray
typedef struct tagRGBQUAD {
  BYTE    rgbBlue;		// Specifies the intensity of blue in the color.
  BYTE    rgbGreen;		// Specifies the intensity of green in the color.
  BYTE    rgbRed;		// Specifies the intensity of red in the color.
  BYTE    rgbReserved;	// Reserved; must be zero.
} RGBQUAD; 

// Color-index Aray
*/

/*
// WriteDIB		- Writes a DIB to file
// Returns		- TRUE on success
// szFile		- Name of file to write to
// hDIB			- Handle of the DIB
BOOL WriteDIB( LPTSTR szFile, HANDLE hDIB)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;

	if (!hDIB)
		return FALSE;

//	CFile file;
//	if( !file.Open( szFile, CFile::modeWrite|CFile::modeCreate) )
//		return FALSE;
	FILE *file = fopen(szFile, "w");
	if(NULL == file)
		return FALSE;

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	int nColors = 1 << lpbi->biBitCount;

	// Fill in the fields of the file header 
	hdr.bfType		= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize		= GlobalSize (hDIB) + sizeof( hdr );
	hdr.bfReserved1 	= 0;
	hdr.bfReserved2 	= 0;
	hdr.bfOffBits		= (DWORD) (sizeof( hdr ) + lpbi->biSize +
						nColors * sizeof(RGBQUAD));

	// Write the file header 
//	file.Write( &hdr, sizeof(hdr) );

	// Write the DIB header and the bits 
//	file.Write( lpbi, GlobalSize(hDIB) );

	fclose(file);

	return TRUE;
}
*/

int main(int argc, char* argv[])
{
	char buffer[10240];	// intermediate buffer for reading
	int readBytes = 1;
	int result;
	int bufflen = sizeof(buffer);
	
	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;
	RGBQUAD rq[2] = {{0,0,0}, {255,255,255}}; // BW

	memset(&fh, 0, sizeof(BITMAPFILEHEADER));
	memset(&ih, 0, sizeof(BITMAPINFOHEADER));

	fh.bfType = 0x4d42;
	fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(rq);
	//fh.bfSize 
	fh.bfReserved1 = 0;
	fh.bfReserved2 = 0;

/*

	ih.biBitCount =
	ih.biClrImportant =
	ih.biClrUsed =
*/

	result = setmode ( fileno ( stdin ), O_BINARY );
	if( result == -1 )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	result = setmode ( fileno ( stdout ), O_BINARY );
	if( result == -1 )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	FILE *fp = fopen("test.bmp", "r");
	readBytes = fread( &fh, 1, sizeof(BITMAPFILEHEADER), fp );

	readBytes = fread( &ih, 1, sizeof(BITMAPINFOHEADER), fp );

	RGBQUAD q[2];
	readBytes = fread( q, 1, 2*sizeof(RGBQUAD), fp );

	int i = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	int n = (fh.bfOffBits - i) / sizeof(RGBQUAD);
	//fseek(fp, fh.bfOffBits, SEEK_SET);
	readBytes = fread( buffer, sizeof(char), bufflen, fp );
	fclose(fp);

	while(readBytes > 0) 
	{
		readBytes = fread( buffer, sizeof(char), bufflen, stdin );

		UINT n = fwrite( buffer, 1, readBytes, stdout );
	}

	return 0;
}

