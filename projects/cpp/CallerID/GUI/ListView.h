// ListView.h: interface for the CListView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTVIEW_H__5A41A7BA_5FA5_450E_AA96_331DD7994EAF__INCLUDED_)
#define AFX_LISTVIEW_H__5A41A7BA_5FA5_450E_AA96_331DD7994EAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <commctrl.h>

#include <stdio.h>

#define ID_LIST		0

class CListView  
{
public:
	CListView(HINSTANCE hInstance, HWND hWndParent);
	virtual ~CListView();

	void AddItem(LPSTR lpszText);
	void Move(int left, int top, int right, int bottom);

private:
	HWND		m_hwndList;
	//HINSTANCE	m_hInstance;
};

#endif // !defined(AFX_LISTVIEW_H__5A41A7BA_5FA5_450E_AA96_331DD7994EAF__INCLUDED_)
