///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICEEXPORTDLG_H__6DF06AC1_1487_11D4_8B0F_0050BAC83302__INCLUDED_)
#define AFX_ICEEXPORTDLG_H__6DF06AC1_1487_11D4_8B0F_0050BAC83302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IceExportDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// IceExportDlg dialog

class IceExportDlg : public CDialog
{
// Construction
public:
	IceExportDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(IceExportDlg)
	enum { IDD = IDD_EXPORT };
	CProgressCtrl	m_Progress;
	CString	m_ExtStatus;
	CString	m_Status;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(IceExportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(IceExportDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICEEXPORTDLG_H__6DF06AC1_1487_11D4_8B0F_0050BAC83302__INCLUDED_)
