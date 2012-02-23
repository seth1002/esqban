// TrayIcon.cpp: implementation of the CTrayIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "TrayIcon.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrayIcon::CTrayIcon(HINSTANCE hInstance, HWND hWndParent)
{
	char *szTip = "CallerID application";
	memset(&m_nid, 0 , sizeof(NOTIFYICONDATA));
	m_nid.cbSize = sizeof(NOTIFYICONDATA);
	m_nid.hWnd = hWndParent; 
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nid.uID = 1;
	m_nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	m_nid.uCallbackMessage = WM_MOUSEMOVE;
	lstrcpyn(m_nid.szTip, szTip, sizeof(m_nid.szTip));
	//Shell_NotifyIcon(NIM_ADD, &nid);
}

CTrayIcon::~CTrayIcon()
{
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

// On_MYWM_NOTIFYICON - processes callback messages for taskbar icons. 
// wParam - first message parameter of the callback message. 
// lParam - second message parameter of the callback message. 
void CTrayIcon::OnMessage(WPARAM wParam, LPARAM lParam) 
{ 
    UINT uID; 
    UINT uMouseMsg; 
 
    uID = (UINT) wParam; 
    uMouseMsg = (UINT) lParam; 

	if(uID != 1)
		return;

	switch(uMouseMsg)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	}
} 

void CTrayIcon::Show(BOOL bShow)
{
	if(bShow)
		Shell_NotifyIcon(NIM_ADD, &m_nid);
	else
		Shell_NotifyIcon(NIM_DELETE, &m_nid);
}
