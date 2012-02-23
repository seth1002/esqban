// Application.h: interface for the CApplication class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPLICATION_H__77D6AC52_CD04_404A_8CDD_71DD03CC6D8B__INCLUDED_)
#define AFX_APPLICATION_H__77D6AC52_CD04_404A_8CDD_71DD03CC6D8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

#define MAX_LOADSTRING	100

class CApplication  
{
public:
	CApplication(HINSTANCE, int, char*, char*);
	virtual ~CApplication();

	void ErrorHandlerEx( WORD wLine, LPSTR lpszFile );
	ATOM MyRegisterClass(HINSTANCE hInstance, char*);
	BOOL InitInstance(HINSTANCE, int, char*, char*);

public:
	HWND m_hWnd;

private:
	HINSTANCE m_hInstance;
};

#endif // !defined(AFX_APPLICATION_H__77D6AC52_CD04_404A_8CDD_71DD03CC6D8B__INCLUDED_)
