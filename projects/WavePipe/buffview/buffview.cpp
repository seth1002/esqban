// dxview.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <math.h>


BOOL debug = FALSE;

short buffer[1024];	// intermediate buffer for reading
int readBytes = 1;

DWORD				gdwThreadId;
HANDLE				ghThread;

// synchronisation objects
CRITICAL_SECTION	gCriticalSection;


BOOL Draw(HWND);
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
void DrawBuffer(HDC, RECT&);
DWORD WINAPI ThreadProc(LPVOID pParam);
void StartThread();
BOOL StopThread();


int main(int argc, char* argv[])
{
	static TCHAR szAppName[] = TEXT ("BuffView");
	HWND		hwnd;
	MSG			msg;
	WNDCLASS	wndclass;
	HINSTANCE	hInstance;


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
	sprintf(caption, "Buffer View - %s\0", name);

	hwnd = CreateWindow(szAppName,				// window class name
						caption,				// window caption
						WS_OVERLAPPEDWINDOW,	// window style
						//CW_USEDEFAULT,			// initial x position
						//CW_USEDEFAULT,			// initial y position
						100,					// initial x position
						100,					// initial y position
						200,					// initial x size
						150,					// initial y size
						NULL,					// parent window handle
						NULL,					// window menu handle
						hInstance,				// program instance handle
						NULL) ;					// creation parameters


	StartThread();


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

		case VK_RIGHT:
			//SendMessage (hwnd, WM_HSCROLL, SB_PAGEDOWN, 0) ;
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


    //
    // Create a compatible DC.
    //

    hdcMem = CreateCompatibleDC(ps.hdc);

    //
    // Create a bitmap big enough for our client rectangle.
    //

    hbmMem = CreateCompatibleBitmap(ps.hdc,
                                    rc.right-rc.left,
                                    rc.bottom-rc.top);

    //
    // Select the bitmap into the off-screen DC.
    //

    hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

    //
    // Erase the background.
    //

//	hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
    hbrBkGnd = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdcMem, &rc, hbrBkGnd);
    DeleteObject(hbrBkGnd);

    SetBkMode(hdcMem, TRANSPARENT);



	DrawBuffer(hdcMem, rc);



    BitBlt(ps.hdc,
           rc.left, rc.top,
           rc.right-rc.left, rc.bottom-rc.top,
           hdcMem,
           0, 0,
           SRCCOPY);

    //
    // Done with off-screen bitmap and DC.
    //

    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);



	EndPaint (hwnd, &ps) ;

	return TRUE;
}

void DrawBuffer(HDC hdcMem, RECT &rc)
{
//	short buffer[1024];	// intermediate buffer for reading
//	int readBytes = 1;
//	int bufflen = sizeof(buffer);
	int x=0, i=0;
	int yl, yr, ylPrev=0, yrPrev=0;

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	int bitsPerSample = sizeof(short) * 8;
	int sampleSize = sizeof(short) * 8;
	int maxSample = (int)pow(2, bitsPerSample) / 2;
	int scale = maxSample / height * 2;

	short localBuffer[1024];

	EnterCriticalSection(&gCriticalSection);

//	readBytes = fread( buffer, sizeof(char), bufflen, stdin );

	if(readBytes == 0)
	{
		LeaveCriticalSection(&gCriticalSection);
		exit(0);
	} else if (readBytes == 0)
	{
		LeaveCriticalSection(&gCriticalSection);
		return;
	}

	memcpy(localBuffer, buffer, sizeof(buffer));

	LeaveCriticalSection(&gCriticalSection);


	HPEN hpen, hpenOld;
    hpen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    hpenOld = (HPEN)SelectObject(hdcMem, hpen);

//	fwrite( buffer, 1, readBytes, stdout );

	int samples = (readBytes*8) / sampleSize / 2;

	//int step = 3;//(rc.right - rc.left) / samples;
	//int step = (rc.right - rc.left) / samples;
	int step = samples / (rc.right - rc.left);
	if(step <= 0)
		step = 3;

	int mid = (rc.bottom - rc.top) / 2;

	for(i=0 ; i < samples; i+=2)  // display Input
	{
		yl = localBuffer[i] / scale;
		yr = localBuffer[i+1] / scale;

		if(i != 0)
		{
			MoveToEx(hdcMem, x, mid + 20 + ylPrev, NULL);
			LineTo(hdcMem, x+step, mid + 20 + yl);
		}

		if(i != 0)
		{
			MoveToEx(hdcMem, x, mid - 20 - yrPrev, NULL);
			LineTo(hdcMem, x+step, mid - 20 - yr);
		}

		ylPrev = yl;
		yrPrev = yr;

		x += step;
	}

	if(debug)
	{
		char txt[100] = "";
		sprintf(txt, "scale=%d step=%d width=%d height=%d samples=%d", scale, step, width, height, samples);
		DrawText (hdcMem, txt, -1, &rc, DT_SINGLELINE | DT_BOTTOM | DT_LEFT | DT_VCENTER) ;
	}

    // Do not forget to clean up.
    SelectObject(hdcMem, hpenOld);
    DeleteObject(hpen);

	//DeleteCriticalSection (&gCriticalSection) ;
}


DWORD WINAPI ThreadProc(LPVOID pParam)
{
	while(readBytes > 0) 
	{
		EnterCriticalSection(&gCriticalSection);

		readBytes = fread( buffer, sizeof(char), sizeof(buffer), stdin );
		UINT n = fwrite( buffer, 1, readBytes, stdout );

		LeaveCriticalSection(&gCriticalSection);
	}

	return 0;
}

void StartThread()
{
	// initialize critical section
	InitializeCriticalSection(&gCriticalSection);

    ghThread = CreateThread(NULL,			// no security attributes 
							0,				// use default stack size  
							ThreadProc,		// thread function 
							NULL,			// argument to thread function 
							0,				// use default creation flags 
							&gdwThreadId);	// returns the thread identifier 
}

BOOL StopThread()
{
	return( SuspendThread(ghThread) != -1 );
}
