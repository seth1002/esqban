// DDrawSystem.h: interface for the CDDrawSystem class.
//
////////////////////////////////////////////////////////////////

#if !defined(AFX_DDRAWSYSTEM_H__1E152EB4_ED1D_4079_BDD4_773383DD98C8__INCLUDED_)
#define AFX_DDRAWSYSTEM_H__1E152EB4_ED1D_4079_BDD4_773383DD98C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ddraw.h>

//#define _CHARACTORBUILDER_
//#include "../GameLib/Image.h"

class CDDrawSystem
{
public:
	HRESULT GetDC(HDC *hdc);
	CDDrawSystem();
	virtual ~CDDrawSystem();

	BOOL Init(HWND hWnd);
	void Terminate();
	void Clear();
	void TestDraw(int x, int y);
	void Display();

protected:
	LPDIRECTDRAW7 m_pDD;
	LPDIRECTDRAWSURFACE7 m_pddsFrontBuffer;
	LPDIRECTDRAWSURFACE7 m_pddsStoreBuffer;
	LPDIRECTDRAWCLIPPER pcClipper;

	HWND m_hWnd;
};

#endif //!defined(AFX_DDRAWSYSTEM_H__1E152EB4_ED1D_4079_BDD4_773383DD98C8__INCLUDED_)
