// LanPhone.h : main header file for the LANPHONE application
//

#if !defined(AFX_LANPHONE_H__381686C2_CAB5_409F_9C70_5DEEF239242E__INCLUDED_)
#define AFX_LANPHONE_H__381686C2_CAB5_409F_9C70_5DEEF239242E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLanPhoneApp:
// See LanPhone.cpp for the implementation of this class
//

class CLanPhoneApp : public CWinApp
{
public:
	CLanPhoneApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLanPhoneApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLanPhoneApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANPHONE_H__381686C2_CAB5_409F_9C70_5DEEF239242E__INCLUDED_)
