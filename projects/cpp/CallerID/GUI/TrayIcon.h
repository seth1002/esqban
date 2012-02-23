// TrayIcon.h: interface for the CTrayIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAYICON_H__BFCB3AD5_670F_4331_97F4_EEDC7A4A6131__INCLUDED_)
#define AFX_TRAYICON_H__BFCB3AD5_670F_4331_97F4_EEDC7A4A6131__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class CTrayIcon  
{
public:
	CTrayIcon(HINSTANCE hInstance, HWND hWndParent);
	virtual ~CTrayIcon();

	void OnMessage(WPARAM wParam, LPARAM lParam) ;
	void Show(BOOL bShow);

private:
	NOTIFYICONDATA m_nid;
};

#endif // !defined(AFX_TRAYICON_H__BFCB3AD5_670F_4331_97F4_EEDC7A4A6131__INCLUDED_)
