// dxview.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <math.h>

#include "DDrawSystem.h"

CDDrawSystem dd;



/*
//#include <ddraw.h>
//#include <d3drm.h>



HWND		hwnd = NULL;
HINSTANCE	hInstance = NULL;




LPDIRECTDRAW pDD;                // DirectDraw object
LPDIRECTDRAWSURFACE pDDSPrimary; // DirectDraw primary surface
LPDIRECTDRAWSURFACE pDDSBack;    // DirectDraw back surface
LPDIRECTDRAWPALETTE pDDPal;      // Palette (coming sometime soonish)
LPDIRECTDRAWCLIPPER pClipper;    // Clipper for windowed mode
HWND ddWnd;                      // Handle of window
BOOL bFullScreen;                // are we in fullscreen mode?
//BOOL bAnimating;                 // are we animating?



LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL Draw();
UINT FlipSurfaces();
BOOL CheckSurfaces();
UINT CreateClipper();
UINT CreatePrimarySurface();
BOOL SetMode();
BOOL InitDDraw();
void InitDirectXGlobals();


void InitDirectXGlobals()
{
	pDD = NULL;
	pDDSPrimary = NULL;
	pDDSBack = NULL;
	pDDPal = NULL;
	pClipper = NULL;
	bFullScreen = FALSE;
//	bAnimating = FALSE;
}

BOOL InitDDraw()
{
	HRESULT hr;
	
	// Create the DirectDraw object
	// The first NULL means use the active display driver
	// The last parameter must be NULL
	hr = DirectDrawCreate(NULL, &pDD, NULL);
	if (FAILED(hr)) {
		printf("Unable to create DDraw object\n");
		return FALSE;
	}
	
	// The DirectDraw object initialized successfully
	return TRUE;
}

BOOL SetMode()
{
	HRESULT hr;
	if (bFullScreen) {
		// Set the "cooperative level" so we can use full-screen mode
		hr = pDD->SetCooperativeLevel(hwnd,
			DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_NOWINDOWCHANGES);
		if (FAILED(hr)) {
			pDD->Release();
			return FALSE;
		}
		// Set 640x480x256 full-screen mode
		hr = pDD->SetDisplayMode(640, 480, 8);
		if (FAILED(hr)) {
			printf("Error setting display mode: %d\n", int(LOWORD(hr)));
			pDD->Release();
			return FALSE;
		}
	} else {
		// Set DDSCL_NORMAL to use windowed mode
		hr = pDD->SetCooperativeLevel(hwnd, DDSCL_NORMAL);
	}
	
	// Success
	return TRUE;
}

UINT CreatePrimarySurface()
{
	DDSURFACEDESC ddsd; // A structure to describe the surface we want
	HRESULT hr;         // Holds return values for function calls
	
	// Screw the full-screen mode (for now)
	
	// This clears all fields of the DDSURFACEDESC structure to 0
	memset(&ddsd, 0, sizeof(ddsd));
	// The first parameter of the structure must contain the
	// size of the structure.
	ddsd.dwSize = sizeof(ddsd);
	
	// The dwFlags paramater tell DirectDraw which DDSURFACEDESC
	// fields will contain valid values
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	
	// Create the primary surface
	hr = pDD->CreateSurface(&ddsd, &pDDSPrimary, NULL);
	if (FAILED(hr))
	{
		printf("Error: pDD->CreateSurface (primary)\n");
		//TraceErrorDD(hr);
		pDD->Release();
		pDD = NULL;
		return 1;
	}
	
	// Now to create the back buffer
	ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	// Make our off-screen surface 320x240
	ddsd.dwWidth = 320;
	ddsd.dwHeight = 240;
	
	// Create an offscreen surface
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	
	hr = pDD->CreateSurface(&ddsd, &pDDSBack, NULL);
	if (FAILED(hr)) {
		printf("Error: CreateSurface (back)\n");
		//TraceErrorDD(hr);
		return 2;
	}
	
	// success
	return 0;
}

UINT CreateClipper()
{
	HRESULT hr;
	
	// Create the clipper using the DirectDraw object
	hr = pDD->CreateClipper(0, &pClipper, NULL);
	if (FAILED(hr)) {
		printf("Error: CreateClipper\n");
		//TraceErrorDD(hr);
		return 1;
	}
	
	// Assign your window's HWND to the clipper
	ddWnd = hwnd;
	hr = pClipper->SetHWnd(0, ddWnd);
	if (FAILED(hr)) {
		printf("Error: SetHWnd\n");
		//TraceErrorDD(hr);
		return 2;
	}
	
	// Attach the clipper to the primary surface
	hr = pDDSPrimary->SetClipper(pClipper);
	if (FAILED(hr)) {
		printf("Error: SetClipper\n");
		//TraceErrorDD(hr);
		return 3;
	}
	// success
	return 0;
}

// Checks if the memory associated with surfaces
// is lost and restores if necessary.
// Not sure about fullscreen/windowed;
// I'll get back to you people on this :)
BOOL CheckSurfaces()
{
	// Check the primary surface
	if (pDDSPrimary) {
		if (pDDSPrimary->IsLost() == DDERR_SURFACELOST) {
			pDDSPrimary->Restore();
			return FALSE;
		}
	}
	return TRUE;
}

UINT FlipSurfaces()
{
	HRESULT hr;
	// source and destination rectangles
	RECT rcSrc;
	RECT rcDest;
	POINT p;
	
	// Make sure no surfaces have been lost
	CheckSurfaces();
	
	// if we're windowed do the blit, else just Flip
	if (!bFullScreen)
	{
		// find out where on the primary surface our window lives
		p.x = 0; p.y = 0;
		::ClientToScreen(ddWnd, &p);
		::GetClientRect(ddWnd, &rcDest);
		OffsetRect(&rcDest, p.x, p.y);
		SetRect(&rcSrc, 0, 0, 320, 240);
		hr = pDDSPrimary->Blt(&rcDest, pDDSBack, &rcSrc, DDBLT_WAIT, NULL);
	} else {
		hr = pDDSPrimary->Flip(NULL, DDFLIP_WAIT);
	}
	
	// success!
	return 0;
}


BOOL Draw()
{
	// Variables for the blocks color, in RGB format
	static r = 0;
	static g = 100;
	static b = 150;
	r++;
	g += 3;
	b += 2;
	if (r > 255) r = 0;
	if (g > 255) g = 0;
	if (b > 255) b = 0;
	
	// The destination rectangle on my 320x240 off-screen surface
	RECT rc = {10,30,100,70};
	
	// We draw the rectangle using a blit.
	// This structure describes how to do the blit.
	DDBLTFX fx;
	// Zero out all fields of the blit effects structure
	memset(&fx, 0, sizeof(fx));
	// The dwSize field must contain the size of the structure
	fx.dwSize = sizeof(DDBLTFX);
	
	// Set the color we want to draw the rectangle
	fx.dwFillColor = RGB(r,g,b);
	
	// Blit. Note that we blit to the back buffer
	HRESULT hr;
	hr = pDDSBack->Blt(&rc, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx);
	if (FAILED(hr)) {
		printf("Error blitting: ");
		//TraceErrorDD(hr);
		return FALSE;
	}
	
	// Call our routine for flipping the surfaces
	FlipSurfaces();
	
	// No major errors
	return TRUE;
}
*/

void Draw()
{
	dd.TestDraw(100, 100);
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		return 0 ;
		
	case WM_PAINT:
		Draw(); ////////////////////
		//Sleep(50); ////////////////////
		return 0 ;
		
	case WM_DESTROY:
		PostQuitMessage (0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

int main(int argc, char* argv[])
{
	static TCHAR szAppName[] = TEXT ("dxView") ;
	HWND		hwnd ;
	MSG			msg ;
	WNDCLASS	wndclass ;
	HINSTANCE	hInstance = NULL;


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
	sprintf(caption, "DirectX View - %s\0", name);

	hwnd = CreateWindow(szAppName,				// window class name
						caption,				// window caption
						WS_OVERLAPPEDWINDOW,	// window style
						CW_USEDEFAULT,			// initial x position
						CW_USEDEFAULT,			// initial y position
						CW_USEDEFAULT,			// initial x size
						CW_USEDEFAULT,			// initial y size
						NULL,					// parent window handle
						NULL,					// window menu handle
						hInstance,				// program instance handle
						NULL) ;					// creation parameters

/*
	InitDirectXGlobals();
	InitDDraw();
	SetMode();
	CreatePrimarySurface();
	CreateClipper();
*/

	dd.Init(hwnd);

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
