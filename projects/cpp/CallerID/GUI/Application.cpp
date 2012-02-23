// Application.cpp: implementation of the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#include "callerid.h"
#include "Application.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CApplication::CApplication(HINSTANCE hInstance, int nCmdShow,
						   char *pszWindowClass, char *pszTitle)
{
	if (MyRegisterClass(hInstance, pszWindowClass))
	{
		InitInstance (hInstance, nCmdShow, pszWindowClass, pszTitle);
	}
}

CApplication::~CApplication()
{

}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM CApplication::MyRegisterClass(HINSTANCE hInstance, char *pszWindowClass)
{
	WNDCLASS     wndclass ;

	m_hInstance = hInstance;
	
	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, MAKEINTRESOURCE(IDI_ICON1)) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = pszWindowClass ;

	return RegisterClass(&wndclass);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL CApplication::InitInstance(HINSTANCE hInstance, int nCmdShow,
								char *pszWindowClass, char *pszTitle)
{
   m_hInstance = hInstance; // Store instance handle in our global variable

   m_hWnd = CreateWindow( pszWindowClass, 
						pszTitle, 
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, 
						CW_USEDEFAULT, 
						206, 
						306, 
						NULL, 
						NULL, 
						hInstance, 
						NULL );

   if (!m_hWnd)
   {
	   ErrorHandlerEx(__LINE__, __FILE__);
      return FALSE;
   }

/*
   m_pListView = new CListView(hInstance, hWnd);
   //CreateListView(hWnd);

   m_pTrayIcon = new CTrayIcon(hInstance, hWnd);
   //CreateTrayIcon(hWnd);
*/
   ShowWindow(m_hWnd, nCmdShow);
   UpdateWindow(m_hWnd);

   return TRUE;
}

void CApplication::ErrorHandlerEx(WORD wLine, LPSTR lpszFile)
{
	LPVOID lpvMessage;
	DWORD  dwError;
	char szBuffer[256];
	
	// Allow FormatMessage() to look up the error code returned by GetLastError
	dwError = FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
								FORMAT_MESSAGE_FROM_SYSTEM, 
								NULL, 
								GetLastError(), 
								MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
								(LPTSTR)&lpvMessage, 
								0, 
								NULL );
	
	// Check to see if an error occured calling FormatMessage()
	if (0 == dwError)
	{
		wsprintf(   szBuffer, 
					"An error occured calling FormatMessage()."
					"Error Code %d", 
					GetLastError() );
		MessageBox( NULL, 
					szBuffer, 
					"Generic", 
					MB_ICONSTOP | MB_ICONEXCLAMATION );
		return;
	}
	
	// Display the error infromation along with the place the error happened.
	wsprintf(szBuffer, "Generic, Line=%d, File=%s", wLine, lpszFile);
	MessageBox(NULL, (char*)lpvMessage, szBuffer, MB_ICONEXCLAMATION | MB_OK);
}
/*
//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message) 
	{
		case WM_CREATE:
			InitModem();
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			//switch (wmId)
			//{
			//	case IDM_EXIT:
			//	   DestroyWindow(hWnd);
			//	   break;
			//	default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			//}
			break;
		case WM_PAINT:
			RECT rt;
			GetClientRect(hWnd, &rt);
			break;
		case WM_SIZE:
			switch(wParam)
			{
			case SIZE_RESTORED:
				m_pTrayIcon->Show(FALSE);
				//Shell_NotifyIcon(NIM_DELETE, &nid);
				break;
			case SIZE_MINIMIZED:
				m_pTrayIcon->Show(TRUE);
				//Shell_NotifyIcon(NIM_ADD, &nid);
				break;
			default:
				RECT  rc;
				
				GetClientRect(hWnd, &rc);
				m_pListView->Move(  rc.left,
									rc.top,
									rc.right - rc.left,
									rc.bottom - rc.top );
			}
			break;
		case WM_MOUSEMOVE:
			m_pTrayIcon->OnMessage(wParam, lParam);
			break;
		case WM_DESTROY:
			if(fpLog) {
				printf("Closing log.txt: ");
				fclose( fpLog );
				printf("OK\n");
			}
			delete m_pListView;
			delete m_pTrayIcon;
			//Shell_NotifyIcon(NIM_DELETE, &m_nid);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}
*/