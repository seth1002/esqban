// LanPhoneDlg.h : header file
//

#if !defined(AFX_LANPHONEDLG_H__37051929_138F_42A0_8C3D_253C0BEF14D6__INCLUDED_)
#define AFX_LANPHONEDLG_H__37051929_138F_42A0_8C3D_253C0BEF14D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLanPhoneDlg dialog

class CLanPhoneDlg : public CDialog
{
// Construction
public:
	CLanPhoneDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLanPhoneDlg)
	enum { IDD = IDD_LANPHONE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLanPhoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLanPhoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANPHONEDLG_H__37051929_138F_42A0_8C3D_253C0BEF14D6__INCLUDED_)
