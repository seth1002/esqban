
#include <windows.h>
#include "resource.h"

#define ID_LIST		0
#define ID_HANDLE	1
#define ID_CAPTION	2
#define ID_CLASS	3
#define ID_STYLE	4
#define ID_RECT		5
#define ID_WPARAM	6
#define ID_LPARAM	7
#define ID_POST		8
#define ID_SETTEXT	9
#define ID_TARGET	10
#define ID_WTEXT	11

struct Control{
	TCHAR * szName ;
	int     iStyle ;
	TCHAR * szText ;
	int x, y, width, height;
	HWND hwnd;
	int ID;
} ;

Control button[] = {
//	szName		iStyle			szText						x		y		width	height	hwnd	ID
//	-------------------------------------------------------------------------------------------------------------
	"listbox",	LBS_STANDARD,	NULL,						0,		0,		200,	300,	NULL,	ID_LIST,
	"static",	SS_SIMPLE,		TEXT("Handle:"),			210,	0,		50,		60,		NULL,	8,
	"Edit",		WS_BORDER,		TEXT("Handle"),				270,	0,		200,	20,		NULL,	ID_HANDLE,
	"static",	SS_SIMPLE,		TEXT("Caption:"),			210,	25,		60,		60,		NULL,	8,
	"Edit",		WS_BORDER,		TEXT("Caption"),			270,	25,		200,	20,		NULL,	ID_CAPTION,
	"static",	SS_SIMPLE,		TEXT("Class:"),				210,	50,		50,		60,		NULL,	8,
	"Edit",		WS_BORDER,		TEXT("Class"),				270,	50,		200,	20,		NULL,	ID_CLASS,
	"static",	SS_SIMPLE,		TEXT("Style:"),				210,	75,		50,		60,		NULL,	8,
	"Edit",		WS_BORDER,		TEXT("Style"),				270,	75,		200,	20,		NULL,	ID_STYLE,
	"static",	SS_SIMPLE,		TEXT("Rect:"),				210,	100,	50,		60,		NULL,	8,
	"Edit",		WS_BORDER,		TEXT("Rect"),				270,	100,	200,	20,		NULL,	ID_RECT,
	"static",	SS_SIMPLE,		TEXT("wParam:"),			210,	125,	60,		60,		NULL,	7,
	"Edit",		WS_BORDER,		TEXT("0"),					270,	125,	100,	20,		NULL,	ID_WPARAM,
	"static",	SS_SIMPLE,		TEXT("lParam:"),			210,	150,	50,		60,		NULL,	8,
	"Edit",		WS_BORDER,		TEXT("0"),					270,	150,	100,	20,		NULL,	ID_LPARAM,
	"static",	SS_SIMPLE,		TEXT("Text:"),				210,	175,	50,		60,		NULL,	8,
	"Edit",		WS_BORDER,		TEXT("Window text"),		270,	175,	100,	20,		NULL,	ID_WTEXT,
	"button",	BS_PUSHBUTTON,  TEXT("PostMessage()"),		210,	210,	150,	20,		NULL,	ID_POST,
	"button",	BS_PUSHBUTTON,  TEXT("SetWindowText()"),	210,	240,	150,	20,		NULL,	ID_SETTEXT,
	"static",	SS_GRAYFRAME,	NULL,						400,	210,	50,		50,		NULL,	ID_TARGET,
} ;

#define NUM (sizeof button / sizeof button[0])

#define MSG_LEN 30
struct wmsg{
	char name[MSG_LEN];
	UINT msg;
};


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
void PrintMessage(HWND hwnd, char *c, int n);
void DrawButtons(HWND hwnd, LPARAM lParam);
void HighlightWindow(HWND hWnd);
void FillListBox (HWND hwndList);
Control GetControl(int ID);
void SetEditText(int ID, char *szBuffer);
void GetEditText(int ID, char *szBuffer);
void DisplayWindowInfo(HWND hWnd);
DWORD GetEditDouble(int ID);
DWORD GetSelectedMessage();

static int   cxChar, cyChar ;
static RECT  rect ;
//static HWND  hwndButton[NUM] ;
static HWND	 prevHwnd;
HINSTANCE hInst ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("Shatter2") ;
	HWND         hwnd ;
	MSG          msg ;
	WNDCLASS     wndclass ;

	hInst = hInstance;
	
	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;
	
	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("Program requires Windows NT!"), szAppName, MB_ICONERROR) ;
		return 0 ;
	}
	
	hwnd = CreateWindow (	szAppName, 
							TEXT ("Shatter 2"), 
							WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 
							CW_USEDEFAULT, CW_USEDEFAULT, 
							500, 315, 
							NULL, NULL, 
							hInstance, 
							NULL);
	
	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;
	
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR	szBuffer[50] ;
	
	static POINT	pt;
	LPRECT			rectx = NULL;
	static BOOL		bFind = FALSE;
	HWND			hwndx = NULL;
	int				xPos, yPos;
	Control			ctrl;
	HWND			hParent;
	HDC				hdc ;
	PAINTSTRUCT		ps ;

	switch (message)
	{
	case WM_CREATE :
		cxChar = LOWORD (GetDialogBaseUnits ()) ;
		cyChar = HIWORD (GetDialogBaseUnits ()) ;
		DrawButtons(hwnd, lParam);
		
	case WM_SIZE :
		rect.left   = 24 * cxChar ;
		rect.top    =  2 * cyChar ;
		rect.right  = LOWORD (lParam) ;
		rect.bottom = HIWORD (lParam) ;
		//MoveWindow (hwndEdit, 0, 0, LOWORD (lParam), HIWORD (lParam), TRUE) ;
		return 0 ;
		
	case WM_PAINT :
		InvalidateRect (hwnd, &rect, TRUE) ;
		hdc = BeginPaint (hwnd, &ps) ;

		EndPaint (hwnd, &ps) ;
		
		return 0 ;
		
	case WM_LBUTTONDOWN :
		xPos = LOWORD(lParam);  
		yPos = HIWORD(lParam);  

		ctrl = GetControl(ID_TARGET);
		if (((xPos >= ctrl.x) && (xPos <= ctrl.x+ctrl.width)) &&
			((yPos >= ctrl.y) && (yPos <= ctrl.y+ctrl.height))) {
			bFind = TRUE;
			SetCapture (hwnd) ;
			SetCursor (LoadCursor (NULL, IDC_CROSS)) ;
			//SetCursor (LoadCursor (hInst, MAKEINTRESOURCE (IDC_WINDOWFINDER))); 
			prevHwnd = NULL;
			return 0 ;
		}

		
	case WM_MOUSEMOVE :
		ctrl = GetControl(ID_TARGET);
		xPos = LOWORD(lParam);  
		yPos = HIWORD(lParam);  

		if (bFind) {
			//SetCursor (LoadCursor (NULL, IDC_CROSS)) ;
			//SetCursor (LoadCursor (hInst, MAKEINTRESOURCE (IDC_WINDOWFINDER)));

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			ClientToScreen(hwnd, &pt);
			if (pt.x >= 65536) pt.x = pt.x - 65536;
			if (pt.y >= 65536) pt.y = pt.y - 65536;

			hwndx = WindowFromPoint(pt);
			hParent = GetParent(hwndx);

			if((prevHwnd) && (prevHwnd != hwndx)) {
				HighlightWindow(prevHwnd);
				prevHwnd = NULL;
			}
			if((hwndx != prevHwnd) && (hwndx != hwnd) && (hParent != hwnd)) {
				HighlightWindow(hwndx);
				DisplayWindowInfo(hwndx);
				prevHwnd = hwndx;
			}
		}
		else if (((xPos >= ctrl.x) && (xPos <= ctrl.x+ctrl.width)) &&
				((yPos >= ctrl.y) && (yPos <= ctrl.y+ctrl.height))) {
			SetCursor (LoadCursor (NULL, IDC_CROSS)) ;
			//SetCursor (LoadCursor (hInst, MAKEINTRESOURCE (IDC_WINDOWFINDER))); 
		}
		else
			SetCursor (LoadCursor (NULL, IDC_ARROW)) ;
		return 0 ;
		
	case WM_LBUTTONUP :
		if (bFind) {
			ReleaseCapture () ;
			SetCursor (LoadCursor (NULL, IDC_ARROW)) ;
			if (prevHwnd) HighlightWindow(prevHwnd);
			bFind = FALSE;
		}
		return 0 ;
		
	case WM_CHAR :
		if (bFind) {
			ReleaseCapture () ;
			SetCursor (LoadCursor (NULL, IDC_ARROW)) ;
			bFind = FALSE;
		}
		return 0 ;

	case WM_DRAWITEM :
	case WM_COMMAND :
		switch(LOWORD(wParam)) {
		case ID_POST:
			if (!PostMessage(prevHwnd, GetSelectedMessage(),GetEditDouble(ID_WPARAM), GetEditDouble(ID_LPARAM)))
				MessageBox(hwnd, "Message failed!","Error:", MB_ICONWARNING|MB_OK);
			break;
		case ID_SETTEXT:
			GetEditText(ID_WTEXT, szBuffer);
			SetWindowText(prevHwnd, (LPCTSTR)szBuffer);
			break;
		}
		break ;
		
	case WM_DESTROY :
		PostQuitMessage (0) ;
		return 0 ;

	case WM_ACTIVATE:
		break;
	}

	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

DWORD GetSelectedMessage()
{
	HWND hWnd = GetControl(ID_LIST).hwnd;
	int index = SendMessage (hWnd, LB_GETCURSEL, 0, 0) ;
	return SendMessage (hWnd, LB_GETITEMDATA , (WPARAM) index, 0) ;
}

void DisplayWindowInfo(HWND hWnd)
{
	TCHAR szBuffer[50] ;

	int n = wsprintf(szBuffer, "%04X", hWnd);
	SetEditText(ID_HANDLE, szBuffer);

	DWORD lStyle = GetWindowLong(hWnd, GWL_STYLE);
	n = wsprintf(szBuffer, "%X", lStyle);
	SetEditText(ID_STYLE, szBuffer);

	GetWindowText(hWnd, szBuffer, sizeof(szBuffer));
	SetEditText(ID_CAPTION, szBuffer);

	n = GetClassName(hWnd, szBuffer, 50);
	SetEditText(ID_CLASS, szBuffer);

//	n = GetWindowText(hWnd, (LPTSTR)szBuffer, 50);
//	//int l = SendMessage (hWnd, EM_LINELENGTH, 0, 0) ;
//	//n = SendMessage (hWnd, WM_GETTEXT , l+1, (LPARAM) szBuffer) ;
//	SetEditText(ID_SETTEXT, szBuffer);
}

void SetEditText(int ID, char *szBuffer)
{
	int l = SendMessage (GetControl(ID).hwnd, EM_LINELENGTH, 0, 0) ;
	int i = SendMessage (GetControl(ID).hwnd, EM_SETSEL, 0, l) ;
	SendMessage (GetControl(ID).hwnd, EM_REPLACESEL, 0, (LPARAM) szBuffer) ;
}

void GetEditText(int ID, char *szBuffer)
{
	//strset(szBuffer, '\0');
	HWND hWnd = GetControl(ID_WTEXT).hwnd;
	int l = SendMessage (hWnd, EM_LINELENGTH, 0, 0) ;
	int n = SendMessage (hWnd, WM_GETTEXT , l+1, (LPARAM) szBuffer) ;
}

DWORD GetEditDouble(int ID)
{
	//strset(szBuffer, '\0');
	TCHAR szBuffer[20];
	HWND hWnd = GetControl(ID_WTEXT).hwnd;
	int l = SendMessage (hWnd, EM_LINELENGTH, 0, 0) ;
	int n = SendMessage (hWnd, WM_GETTEXT , l+1, (LPARAM) szBuffer) ;
	return atol(szBuffer);
}

void DrawButtons(HWND hwnd, LPARAM lParam) 
{
	for (int i = 0 ; i < NUM ; i++)
		button[i].hwnd = CreateWindow (	button[i].szName, button[i].szText, 
										WS_CHILD | WS_VISIBLE | button[i].iStyle, 
										button[i].x, button[i].y, 
										button[i].width, button[i].height, 
										hwnd, 
										(HMENU) button[i].ID, 
										((LPCREATESTRUCT) lParam)->hInstance, 
										NULL) ;
	FillListBox(GetControl(ID_LIST).hwnd);

	PAINTSTRUCT  ps ;
	Control ctrl = GetControl(ID_TARGET);
	HDC hdc = BeginPaint (hwnd, &ps) ;
	DrawIcon(hdc, ctrl.x, ctrl.y, LoadIcon(hInst, MAKEINTRESOURCE(IDB_WINDOWFINDER1)));
	EndPaint (hwnd, &ps) ;
}

Control GetControl(int ID)
{
	for (int i = 0 ; i < NUM ; i++) {
		if(button[i].ID == ID) return button[i];
	}
	return button[0];
}

void HighlightWindow(HWND hWnd)
{
	RECT stRect;

	GetWindowRect(hWnd, &stRect);	// Get the coordinates of the window on the screen

	HDC hDC = GetWindowDC(hWnd);	// Get a handle to the window's device context

	SetROP2(hDC, R2_NOT);			// Create an inverse pen that is the size of the window border
	
	HPEN hPen = CreatePen(PS_INSIDEFRAME, 3 * GetSystemMetrics(SM_CXBORDER), RGB(0,0,0));
	
	// Draw the rectangle around the window
	HPEN   hOldPen   = (HPEN)SelectObject(hDC, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
	
	Rectangle(hDC, 0, 0, stRect.right - stRect.left, stRect.bottom - stRect.top);
	
	SelectObject(hDC, hOldBrush);
	SelectObject(hDC, hOldPen);

	ReleaseDC(hWnd, hDC); //Give the window its device context back, and destroy our pen
	
	DeleteObject(hPen);
}

void FillListBox (HWND hwndList) 
{
	wmsg msgs[] = { 
					"EM_CANUNDO",				EM_CANUNDO,
					"EM_CHARFROMPOS",			EM_CHARFROMPOS,			
					"EM_EMPTYUNDOBUFFER",		EM_EMPTYUNDOBUFFER,
					"EM_FMTLINES",				EM_FMTLINES,
					"EM_GETFIRSTVISIBLELINE",	EM_GETFIRSTVISIBLELINE,	
					"EM_GETHANDLE",				EM_GETHANDLE,
//					"EM_GETIMESTATUS",			EM_GETIMESTATUS,	
					"EM_GETLIMITTEXT",			EM_GETLIMITTEXT,			
					"EM_GETLINE",				EM_GETLINE,
					"EM_GETLINECOUNT",			EM_GETLINECOUNT,	
					"EM_GETMARGINS",			EM_GETMARGINS,		
					"EM_GETMODIFY",				EM_GETMODIFY,			
					"EM_GETPASSWORDCHAR",		EM_GETPASSWORDCHAR,
					"EM_GETRECT",				EM_GETRECT,			
					"EM_GETSEL",				EM_GETSEL,	
					"EM_GETTHUMB",				EM_GETTHUMB,	
					"EM_GETWORDBREAKPROC",		EM_GETWORDBREAKPROC,		
					"EM_LIMITTEXT",				EM_LIMITTEXT,
					"EM_LINEFROMCHAR",			EM_LINEFROMCHAR,	
					"EM_LINEINDEX",				EM_LINEINDEX,			
					"EM_LINELENGTH",			EM_LINELENGTH,
					"EM_LINESCROLL",			EM_LINESCROLL,
					"EM_POSFROMCHAR",			EM_POSFROMCHAR,	
					"EM_REPLACESEL",			EM_REPLACESEL,	
					"EM_SCROLL",				EM_SCROLL,
					"EM_SCROLLCARET",			EM_SCROLLCARET,
					"EM_SETHANDLE",				EM_SETHANDLE,
//					"EM_SETIMESTATUS",			EM_SETIMESTATUS,
					"EM_SETLIMITTEXT",			EM_SETLIMITTEXT,	
					"EM_SETMARGINS",			EM_SETMARGINS,
					"EM_SETMODIFY",				EM_SETMODIFY,	
					"EM_SETPASSWORDCHAR",		EM_SETPASSWORDCHAR,	
					"EM_SETREADONLY",			EM_SETREADONLY,
					"EM_SETRECT",				EM_SETRECT,
					"EM_SETRECTNP",				EM_SETRECTNP,			
					"EM_SETSEL",				EM_SETSEL,
					"EM_SETTABSTOPS",			EM_SETTABSTOPS,
					"EM_SETWORDBREAKPROC",		EM_SETWORDBREAKPROC,
					"EM_UNDO",					EM_UNDO,
					"EN_CHANGE",				EN_CHANGE,	
					"EN_ERRSPACE",				EN_ERRSPACE,				
					"EN_HSCROLL",				EN_HSCROLL,
					"EN_KILLFOCUS",				EN_KILLFOCUS,		
					"EN_MAXTEXT",				EN_MAXTEXT,		
					"EN_SETFOCUS",				EN_SETFOCUS,
					"EN_UPDATE",				EN_UPDATE,
					"EN_VSCROLL",				EN_VSCROLL,	
					"WM_COMMAND",				WM_COMMAND,
					"WM_COPY",					WM_COPY,
					"WM_CTLCOLOREDIT",			WM_CTLCOLOREDIT,		
					"WM_CUT",					WM_CUT,
					"WM_PASTE",					WM_PASTE,	
					"WM_UNDO",					WM_UNDO,
					"WM_GETFONT",				WM_GETFONT, 
					"WM_SETFONT",				WM_SETFONT,
					"BM_CLICK",					BM_CLICK,
					"BM_GETCHECK",				BM_GETCHECK,
					"BM_GETIMAGE",				BM_GETIMAGE,
					"BM_GETSTATE",				BM_GETSTATE,
					"BM_SETCHECK",				BM_SETCHECK,
					"BM_SETIMAGE",				BM_SETIMAGE,
					"BM_SETSTATE",				BM_SETSTATE,
					"BM_SETSTYLE",				BM_SETSTYLE,
					"BN_CLICKED",				BN_CLICKED,
					"BN_DBLCLK",				BN_DBLCLK,
					"BN_DISABLE",				BN_DISABLE,
					"BN_DOUBLECLICKED",			BN_DOUBLECLICKED,
					"BN_HILITE",				BN_HILITE,
					"BN_KILLFOCUS",				BN_KILLFOCUS,
					"BN_PAINT",					BN_PAINT,
					"BN_PUSHED",				BN_PUSHED,
					"BN_SETFOCUS",				BN_SETFOCUS,
					"BN_UNHILITE",				BN_UNHILITE,
					"BN_UNPUSHED",				BN_UNPUSHED,
					"WM_CTLCOLORBTN",			WM_CTLCOLORBTN,
					"CB_ADDSTRING",				CB_ADDSTRING,
					"CB_DELETESTRING",			CB_DELETESTRING,
					"CB_DIR",					CB_DIR,
					"CB_FINDSTRING",			CB_FINDSTRING,
					"CB_FINDSTRINGEXACT",		CB_FINDSTRINGEXACT,
					"CB_GETCOUNT",				CB_GETCOUNT,
					"CB_GETCURSEL",				CB_GETCURSEL,
					"CB_GETDROPPEDCONTROLRECT",	CB_GETDROPPEDCONTROLRECT,
					"CB_GETDROPPEDSTATE",		CB_GETDROPPEDSTATE,
					"CB_GETDROPPEDWIDTH",		CB_GETDROPPEDWIDTH,
					"CB_GETEDITSEL",			CB_GETEDITSEL,
					"CB_GETEXTENDEDUI",			CB_GETEXTENDEDUI,
					"CB_GETHORIZONTALEXTENT",	CB_GETHORIZONTALEXTENT,
					"CB_GETITEMDATA",			CB_GETITEMDATA,
					"CB_GETITEMHEIGHT",			CB_GETITEMHEIGHT,
					"CB_GETLBTEXT",				CB_GETLBTEXT,
					"CB_GETLBTEXTLEN",			CB_GETLBTEXTLEN,
					"CB_GETLOCALE",				CB_GETLOCALE,
					"CB_GETTOPINDEX",			CB_GETTOPINDEX,
					"CB_INITSTORAGE",			CB_INITSTORAGE,
					"CB_INSERTSTRING",			CB_INSERTSTRING,
					"CB_LIMITTEXT",				CB_LIMITTEXT,
					"CB_RESETCONTENT",			CB_RESETCONTENT,
					"CB_SELECTSTRING",			CB_SELECTSTRING,
					"CB_SETCURSEL",				CB_SETCURSEL,
					"CB_SETDROPPEDWIDTH",		CB_SETDROPPEDWIDTH,
					"CB_SETEDITSEL",			CB_SETEDITSEL,
					"CB_SETEXTENDEDUI",			CB_SETEXTENDEDUI,
					"CB_SETHORIZONTALEXTENT",	CB_SETHORIZONTALEXTENT,
					"CB_SETITEMDATA",			CB_SETITEMDATA,
					"CB_SETITEMHEIGHT",			CB_SETITEMHEIGHT,
					"CB_SETLOCALE",				CB_SETLOCALE,
					"CB_SETTOPINDEX",			CB_SETTOPINDEX,
					"CB_SHOWDROPDOWN",			CB_SHOWDROPDOWN,
					"CBN_CLOSEUP",				CBN_CLOSEUP,
					"CBN_DBLCLK",				CBN_DBLCLK,
					"CBN_DROPDOWN",				CBN_DROPDOWN,
					"CBN_EDITCHANGE",			CBN_EDITCHANGE,
					"CBN_EDITUPDATE",			CBN_EDITUPDATE,
					"CBN_ERRSPACE",				CBN_ERRSPACE,
					"CBN_KILLFOCUS",			CBN_KILLFOCUS,
					"CBN_SELCHANGE",			CBN_SELCHANGE,
					"CBN_SELENDCANCEL",			CBN_SELENDCANCEL,
					"CBN_SELENDOK",				CBN_SELENDOK,
					"CBN_SETFOCUS",				CBN_SETFOCUS,
					"WM_COMPAREITEM",			WM_COMPAREITEM,
					"WM_DRAWITEM",				WM_DRAWITEM,
					"WM_MEASUREITEM",			WM_MEASUREITEM,
					"LB_ADDFILE",				LB_ADDFILE,
					"LB_ADDSTRING",				LB_ADDSTRING,
					"LB_DELETESTRING",			LB_DELETESTRING,
					"LB_DIR",					LB_DIR,
					"LB_FINDSTRING",			LB_FINDSTRING,
					"LB_FINDSTRINGEXACT",		LB_FINDSTRINGEXACT,
					"LB_GETANCHORINDEX",		LB_GETANCHORINDEX,
					"LB_GETCARETINDEX",			LB_GETCARETINDEX,
					"LB_GETCOUNT",				LB_GETCOUNT,
					"LB_GETCURSEL",				LB_GETCURSEL,
					"LB_GETHORIZONTALEXTENT",	LB_GETHORIZONTALEXTENT,
					"LB_GETITEMDATA",			LB_GETITEMDATA,
					"LB_GETITEMHEIGHT",			LB_GETITEMHEIGHT,
					"LB_GETITEMRECT",			LB_GETITEMRECT,
					"LB_GETLOCALE",				LB_GETLOCALE,
					"LB_GETSEL",				LB_GETSEL,
					"LB_GETSELCOUNT",			LB_GETSELCOUNT,
					"LB_GETSELITEMS",			LB_GETSELITEMS,
					"LB_GETTEXT",				LB_GETTEXT,
					"LB_GETTEXTLEN",			LB_GETTEXTLEN,
					"LB_GETTOPINDEX",			LB_GETTOPINDEX,
					"LB_INITSTORAGE",			LB_INITSTORAGE,
					"LB_INSERTSTRING",			LB_INSERTSTRING,
					"LB_ITEMFROMPOINT",			LB_ITEMFROMPOINT,
					"LB_RESETCONTENT",			LB_RESETCONTENT,
					"LB_SELECTSTRING",			LB_SELECTSTRING,
					"LB_SELITEMRANGE",			LB_SELITEMRANGE,
					"LB_SELITEMRANGEEX",		LB_SELITEMRANGEEX,
					"LB_SETANCHORINDEX",		LB_SETANCHORINDEX,
					"LB_SETCARETINDEX",			LB_SETCARETINDEX,
					"LB_SETCOLUMNWIDTH",		LB_SETCOLUMNWIDTH,
					"LB_SETCOUNT",				LB_SETCOUNT,
					"LB_SETCURSEL",				LB_SETCURSEL,
					"LB_SETHORIZONTALEXTENT",	LB_SETHORIZONTALEXTENT,
					"LB_SETITEMDATA",			LB_SETITEMDATA,
					"LB_SETITEMHEIGHT",			LB_SETITEMHEIGHT,
					"LB_SETLOCALE",				LB_SETLOCALE,
					"LB_SETSEL",				LB_SETSEL,
					"LB_SETTABSTOPS",			LB_SETTABSTOPS,
					"LB_SETTOPINDEX",			LB_SETTOPINDEX,
					"LBN_DBLCLK",				LBN_DBLCLK,
					"LBN_ERRSPACE",				LBN_ERRSPACE,
					"LBN_KILLFOCUS",			LBN_KILLFOCUS,
					"LBN_SELCANCEL",			LBN_SELCANCEL,
					"LBN_SELCHANGE",			LBN_SELCHANGE,
					"LBN_SETFOCUS",				LBN_SETFOCUS,
					"WM_CHARTOITEM",			WM_CHARTOITEM,
					"WM_CTLCOLORLISTBOX",		WM_CTLCOLORLISTBOX,
					"WM_DELETEITEM",			WM_DELETEITEM,
					"WM_VKEYTOITEM",			WM_VKEYTOITEM,
					"SBM_ENABLE_ARROWS",		SBM_ENABLE_ARROWS,
					"SBM_GETPOS",				SBM_GETPOS,
					"SBM_GETRANGE",				SBM_GETRANGE,
					"SBM_GETSCROLLINFO",		SBM_GETSCROLLINFO,
					"SBM_SETPOS",				SBM_SETPOS,
					"SBM_SETRANGE",				SBM_SETRANGE,
					"SBM_SETRANGEREDRAW",		SBM_SETRANGEREDRAW,
					"SBM_SETSCROLLINFO",		SBM_SETSCROLLINFO,
					"WM_CTLCOLORSCROLLBAR",		WM_CTLCOLORSCROLLBAR,
					"WM_HSCROLL",				WM_HSCROLL,
					"WM_VSCROLL",				WM_VSCROLL,
					"STM_GETICON",				STM_GETICON,
					"STM_GETIMAGE",				STM_GETIMAGE,
					"STM_SETICON",				STM_SETICON,
					"STM_SETIMAGE",				STM_SETIMAGE,
					"WM_PAINT",					WM_PAINT,
					"WM_SETREDRAW",				WM_SETREDRAW,
					"WM_SETTEXT",				WM_SETTEXT,
					"WM_ACTIVATE",				WM_ACTIVATE,
					"WM_CHAR",					WM_CHAR,
					"WM_DEADCHAR",				WM_DEADCHAR,
					"WM_GETHOTKEY",				WM_GETHOTKEY,
					"WM_HOTKEY",				WM_HOTKEY,
					"WM_KEYDOWN",				WM_KEYDOWN,
					"WM_KEYUP",					WM_KEYUP,
					"WM_KILLFOCUS",				WM_KILLFOCUS,
					"WM_SETFOCUS",				WM_SETFOCUS,
					"WM_SETHOTKEY",				WM_SETHOTKEY,
					"WM_SYSCHAR",				WM_SYSCHAR,
					"WM_SYSDEADCHAR",			WM_SYSDEADCHAR,
					"WM_SYSKEYDOWN",			WM_SYSKEYDOWN,
					"WM_SYSKEYUP",				WM_SYSKEYUP
				};

	for(int i=0; i<sizeof(msgs)/sizeof(wmsg); i++) {
		SendMessage (hwndList, LB_ADDSTRING,	0, (LPARAM) msgs[i].name) ;
		SendMessage (hwndList, LB_SETITEMDATA,	i, (LPARAM) msgs[i].msg) ;
	}
}
