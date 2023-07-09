///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICEMOTIONDLG_H__2C4FF8DF_FCAC_11D4_8B0F_0050BAC83302__INCLUDED_)
#define AFX_ICEMOTIONDLG_H__2C4FF8DF_FCAC_11D4_8B0F_0050BAC83302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IceMotionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// IceMotionDlg dialog

class IceMotionDlg : public CDialog
{
// Construction
public:
	IceMotionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(IceMotionDlg)
	enum { IDD = IDD_DIALOG2 };
	CString	m_MotionName;
	CString	m_MotionType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(IceMotionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(IceMotionDlg)
	afx_msg void OnSelchangeComboboxex1();
	afx_msg void OnEditchangeComboboxex1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICEMOTIONDLG_H__2C4FF8DF_FCAC_11D4_8B0F_0050BAC83302__INCLUDED_)
