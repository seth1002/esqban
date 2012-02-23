// CallerID.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include "resource.h"
#include "callerid.h"

#include "SerialPort\SerialPort.h"
#include "application.h"
#include "listview.h"
#include "trayicon.h"

#define LOG_FILE_NAME	"CALLERID.TXT"
#define	COM_PORT		3
#define	BAUD_RATE		9600//115200

#define MAX_LOADSTRING	100

//#define ID_LIST		0

// kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib Shell32.lib 

// Global Variables:
//HINSTANCE hInst;							// current instance
TCHAR szTitle[MAX_LOADSTRING] = "CallerID";	// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING] = "CallerID";		// The title bar text
//TCHAR szAppName[] = TEXT ("CallerID") ;


FILE *fpLog		= NULL;
char szBuff[100]={0};
int iSize		= 0;

char szDate[10]	= {0};
char szTime[10]	= {0};
char szNmbr[20]	= {0};

char szDay[3]	= {0};
char szMonth[3]	= {0};

char szHour[3]	= {0};
char szMin[3]	= {0};


// Foward declarations of functions included in this code module:
//ATOM				MyRegisterClass(HINSTANCE hInstance);
//BOOL				InitInstance(HINSTANCE, int);
//LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


void InitModem();
void OnReceiveStatus(int status);
void OnReceiveChar(const char ch);
//void ErrorHandlerEx( WORD wLine, LPSTR lpszFile );


CApplication *m_pApp   = NULL;
CListView *m_pListView = NULL;
CTrayIcon *m_pTrayIcon = NULL;



int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;

	char szPort[10] = {0};
	sprintf(szPort, " (COM%d)", COM_PORT);
	strcat(szTitle, szPort);

	InitCommonControls();

	m_pApp = new CApplication(hInstance, nCmdShow, szWindowClass, szTitle);
	m_pListView = new CListView(hInstance, m_pApp->m_hWnd);
	m_pTrayIcon = new CTrayIcon(hInstance, m_pApp->m_hWnd);
/*
	if (!MyRegisterClass(hInstance))
	{
		return FALSE;
	}

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}
*/
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


/*
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
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASS     wndclass ;

	hInst = hInstance;
	
	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, MAKEINTRESOURCE(IDI_ICON1)) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szWindowClass ;

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow( szWindowClass, 
						szTitle, 
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, 
						CW_USEDEFAULT, 
						206, 
						306, 
						NULL, 
						NULL, 
						hInstance, 
						NULL );

   if (!hWnd)
   {
	   ErrorHandlerEx(__LINE__, __FILE__);
      return FALSE;
   }


   m_pListView = new CListView(hInstance, hWnd);
   //CreateListView(hWnd);

   m_pTrayIcon = new CTrayIcon(hInstance, hWnd);
   //CreateTrayIcon(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
*/
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
				if(m_pTrayIcon)
					m_pTrayIcon->Show(FALSE);
				//Shell_NotifyIcon(NIM_DELETE, &nid);
				break;
			case SIZE_MINIMIZED:
				if(m_pTrayIcon)
					m_pTrayIcon->Show(TRUE);
				//Shell_NotifyIcon(NIM_ADD, &nid);
				break;
			default:
				if(m_pListView) {
					RECT  rc;
					GetClientRect(hWnd, &rc);
					m_pListView->Move(  rc.left,
										rc.top,
										rc.right - rc.left,
										rc.bottom - rc.top );
				}
			}
			break;
		case WM_MOUSEMOVE:
			if(m_pTrayIcon)
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

void OnReceiveChar(const char ch)
{
	if(ch!=13) {
		if(ch!=10 && iSize<=sizeof(szBuff))
			szBuff[iSize++] = ch;
	}
	else {
		if(iSize > 7) {
			if(0 == strncmp(szBuff, "DATE", 4)) {
				memset(szDate, 0, sizeof(szDate));
				strncpy(szDate, szBuff + 7, strlen(szBuff + 7));
				if(strlen(szDate) >= 4) {
					szDay[0] = szDate[0];
					szDay[1] = szDate[1];

					szMonth[0] = szDate[2];
					szMonth[1] = szDate[3];
				}
			}
			else if(0 == strncmp(szBuff, "TIME", 4)) {
				memset(szTime, 0, sizeof(szTime));
				strncpy(szTime, szBuff + 7, strlen(szBuff + 7));
				if(strlen(szTime) >= 4) {
					szHour[0] = szTime[0];
					szHour[1] = szTime[1];

					szMin[0] = szTime[2];
					szMin[1] = szTime[3];
				}
			}
			else if(0 == strncmp(szBuff, "NMBR", 4)) {
				memset(szNmbr, 0, sizeof(szNmbr));
				strncpy(szNmbr, szBuff + 7, strlen(szBuff + 7));

				char text[100] = {0};
				//printf("%s\t%s\t%s\n", szDate, szTime, szNmbr);
				sprintf(text, "%s/%s %s:%s %s", szDay, szMonth, szHour, szMin, szNmbr);
				
				fprintf(stdout, "%s\n", text);
				fprintf(fpLog,  "%s\n", text);

				//SendMessage (m_hwndList, LB_ADDSTRING,	0, (LPARAM) text) ;
				//SendMessage (hwndList, LB_SETITEMDATA,	i, (LPARAM) msgs[i].msg) ;

				fflush( fpLog );
			}
		}
		memset(szBuff, 0, sizeof(szBuff));
		iSize = 0;
	}

	//int n = putc(ch, fpLog);
	//n = fflush( fpLog );
	//printf("%c", ch);
}

void OnReceiveStatus(int status)
{
	switch(status)
	{
	case EV_CTS:
		break;
	case EV_RXFLAG:
		break;
	case EV_BREAK:
		break;
	case EV_ERR:
		break;
	case EV_RING:
		break;
	}
}

void InitModem()
{
	CSerialPort serialPort;
	
	serialPort.SetOnReceiveChar( OnReceiveChar );
	serialPort.SetOnReceiveStatus( OnReceiveStatus );
	
	memset(szBuff, 0, sizeof(szBuff));

	printf("CallerID, Esqban S. Simon, 2004.\n\n");

	printf("Opening COM%d: ", COM_PORT);
	if(serialPort.InitPort(COM_PORT, BAUD_RATE)) {
		printf("OK\n");

		char init[100];
		memset(&init, 0, sizeof(init));
		strcpy(init, "AT +VCID=1");
		init[strlen(init)] = 13;
		printf("Activating CallerID: ");
		serialPort.SendData( init, strlen(init) );
		printf("OK\n");

		printf("Listening on COM%d: ", COM_PORT);
		if(serialPort.StartMonitoring()) {
			printf("OK\n");

//			for(int x=0; x<20; x++) {
//				SendMessage (hwndList, LB_ADDSTRING,	0, (LPARAM) "Test") ;
				//SendMessage (hwndList, LB_SETITEMDATA,	x, (LPARAM) "Test") ;
				//SendMessage (hwndList, WM_PAINT,	0, 0) ;
//			}

			printf("Opening log file (%s): ", LOG_FILE_NAME);
			fpLog = fopen( LOG_FILE_NAME, "a+");
			if(!fpLog)
				printf("failed!\n");
		}
		else // StartMonitoring
			printf("failed!\n");
	}
	else // InitPort
		printf("failed!\n");
}
/*
void ErrorHandlerEx( WORD wLine, LPSTR lpszFile )
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
*/