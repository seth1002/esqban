// dxview.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <math.h>


#define BUF_LENGTH	1024
#define FRAME_SIZE	BUF_LENGTH/4


BOOL	debug = FALSE;			// Shows debug message
short	buffer[BUF_LENGTH*2];	// intermediate buffer for reading
float	lbuffer[BUF_LENGTH];	// left FFT buffer
float	rbuffer[BUF_LENGTH];	// right FFT buffer
int		readBytes = 1;
DWORD	gdwThreadId;
HANDLE	ghThread;
CRITICAL_SECTION	gCriticalSection;	// synchronisation object


void	smbFft(float *fftBuffer, long fftFrameSize, long sign);
BOOL	Draw(HWND);
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
void	DrawFFT(HDC, RECT&);
DWORD	WINAPI ThreadProc(LPVOID pParam);
void	ProcessFFT(short *buff);


int main(int argc, char* argv[])
{
	static TCHAR szAppName[] = TEXT ("GLFFTView");
	HWND		hwnd;
	MSG			msg;
	WNDCLASS	wndclass;
	HINSTANCE	hInstance;


	if( -1 == setmode( fileno ( stdin ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	if( -1 == setmode( fileno ( stdout ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return -1;
	}


	hInstance = GetModuleHandle(NULL);

	memset(&wndclass, 0, sizeof(WNDCLASS));

	wndclass.style         = CS_OWNDC;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.hInstance     = hInstance ;
	//wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
					szAppName, MB_ICONERROR) ;
		return 0 ;
	}

	char *name = ( argc == 2 ? argv[1] : TEXT("untitled") );
	char caption[100] = {0};
	sprintf(caption, "FFT View - %s\0", name);

	hwnd = CreateWindow(szAppName,				// window class name
						caption,				// window caption
						WS_OVERLAPPEDWINDOW,	// window style
						CW_USEDEFAULT,			// initial x position
						CW_USEDEFAULT,			// initial y position
						600,					// initial x size
						150,					// initial y size
						NULL,					// parent window handle
						NULL,					// window menu handle
						hInstance,				// program instance handle
						NULL) ;					// creation parameters


	// initialize critical section
	InitializeCriticalSection(&gCriticalSection);

    ghThread = CreateThread(NULL,			// no security attributes 
							0,				// use default stack size  
							ThreadProc,		// thread function 
							NULL,			// argument to thread function 
							0,				// use default creation flags 
							&gdwThreadId);	// returns the thread identifier 


	ShowWindow (hwnd, SW_SHOW) ;
	UpdateWindow (hwnd) ;

	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;

	return 0;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		return 0 ;
		
	case WM_PAINT:
		Draw(hwnd);
		return 0 ;
	
	// Double buffering
	case WM_ERASEBKGND:
		return (LRESULT)1; // Say we handled it.

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
		case VK_SPACE:
		case VK_RETURN:
		case '\n':
			exit(0);
			break ;

		case 'd':
		case 'D':
			debug = !debug;
			break ;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage (0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

BOOL Draw(HWND hwnd)
{
	HDC         hdc ;
	PAINTSTRUCT ps ;
	RECT        rc ;

    HDC			hdcMem;
    HBITMAP		hbmMem, hbmOld;
    HBRUSH		hbrBkGnd;

	hdc = BeginPaint (hwnd, &ps) ;
	GetClientRect (hwnd, &rc) ;
	InvalidateRect (hwnd, NULL, TRUE) ;


    hdcMem = CreateCompatibleDC(ps.hdc);

    // Create a bitmap big enough for our client rectangle.
    hbmMem = CreateCompatibleBitmap(ps.hdc, rc.right-rc.left, rc.bottom-rc.top);

    // Select the bitmap into the off-screen DC.
    hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

    // Erase the background.
//	hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
    hbrBkGnd = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdcMem, &rc, hbrBkGnd);
    DeleteObject(hbrBkGnd);

    SetBkMode(hdcMem, TRANSPARENT);

	
	DrawFFT(hdcMem, rc);


    BitBlt(ps.hdc,
           rc.left, rc.top,
           rc.right-rc.left, rc.bottom-rc.top,
           hdcMem,
           0, 0,
           SRCCOPY);

    // Done with off-screen bitmap and DC.
    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);


	EndPaint (hwnd, &ps) ;

	return TRUE;
}

void DrawFFT(HDC hdcMem, RECT &rc)
{
	short localBuffer[BUF_LENGTH*2];

	EnterCriticalSection(&gCriticalSection);

	if(readBytes == 0) {
		LeaveCriticalSection(&gCriticalSection);
		exit(0);
	} else if (readBytes == 0) {
		LeaveCriticalSection(&gCriticalSection);
		return;
	}

	memcpy(localBuffer, buffer, sizeof(buffer));

	LeaveCriticalSection(&gCriticalSection);


	ProcessFFT(localBuffer);


	HPEN hpen1, hpen2, hpenOld;
    hpen1 = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    hpen2 = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    hpenOld = (HPEN)SelectObject(hdcMem, hpen1);


	int x=0, i=0;
	int samples = FRAME_SIZE;
	int mid		= (rc.bottom - rc.top) / 2;
	int width	= rc.right - rc.left;
	int step	= (samples > width ? samples / width : width / samples);
	int lprev	= mid-20;
	int rprev	= mid+20;
	int ly=0, ry=0;

	for(i=1; i<samples; i++)
	{
		x = i * step;

		ly = (int)(mid-20.f-abs(lbuffer[i]/65535.f));
		MoveToEx(hdcMem, x-step, lprev, NULL);
		LineTo(hdcMem, x, ly);

		ry = (int)(mid+20.f+abs(rbuffer[i]/65535.f));
		MoveToEx(hdcMem, x-step, rprev, NULL);
		LineTo(hdcMem, x, ry);

		lprev=ly;
		rprev=ry;
	}

	short lamp=0, ramp=0;
	for(x=0; x<BUF_LENGTH; x+=2)
	{
		if(buffer[x] > lamp)
			lamp = buffer[x];
		if(buffer[x+1] > ramp)
			ramp = buffer[x+1];
	}
	int dbLeft  = 10 * log(lamp / 32767.f);
	int dbRight = 10 * log(ramp / 32767.f);
	//int db = 20 * log(amp / 65535.f);

	SelectObject(hdcMem, hpen2);

	MoveToEx(hdcMem, 0, mid-1, NULL);
	LineTo(hdcMem, width-abs(dbLeft)*10, mid-1);

	MoveToEx(hdcMem, 0, mid+1, NULL);
	LineTo(hdcMem, width-abs(dbRight)*10, mid+1);

	if(debug)
	{
		char txt[100] = "";
		sprintf(txt, "%d samples, left: %ddb right: %d", samples, dbLeft, dbRight);
		DrawText (hdcMem, txt, -1, &rc, DT_SINGLELINE | DT_BOTTOM | DT_LEFT | DT_VCENTER) ;
	}


    // Do not forget to clean up.
    SelectObject(hdcMem, hpenOld);
    DeleteObject(hpen1);
    DeleteObject(hpen2);
}


DWORD WINAPI ThreadProc(LPVOID pParam)
{
	while(readBytes > 0) 
	{
		EnterCriticalSection(&gCriticalSection);
		readBytes = fread( buffer, 1, sizeof(buffer), stdin );
		UINT n = fwrite( buffer, 1, readBytes, stdout );
		LeaveCriticalSection(&gCriticalSection);
	}

	return 0;
}

void ProcessFFT(short *buff)
{
	int i=0, n=0;

	int readSamples = readBytes / sizeof(short);

	n=0;
	for(i=0; i<readSamples; i+=2)
	{
		lbuffer[n++] = (float)buff[i];
	}
	smbFft(lbuffer, FRAME_SIZE, -1);


	n=0;
	for(i=1; i<readSamples; i+=2)
	{
		rbuffer[n++] = (float)buff[i];
	}
	smbFft(rbuffer, FRAME_SIZE, -1);
}
