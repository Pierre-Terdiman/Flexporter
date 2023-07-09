///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLX_EDITBOX_H__BFA69E81_79C3_4924_8FA0_CBB24F159DB9__INCLUDED_)
#define AFX_FLX_EDITBOX_H__BFA69E81_79C3_4924_8FA0_CBB24F159DB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FLX_EditBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FLX_EditBox dialog

class FLX_EditBox : public CDialog
{
// Construction
public:
	FLX_EditBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(FLX_EditBox)
	enum { IDD = IDD_ADD_USER_PROP };
	CString	m_Msg0;
	CString	m_EditedString;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FLX_EditBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FLX_EditBox)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLX_EDITBOX_H__BFA69E81_79C3_4924_8FA0_CBB24F159DB9__INCLUDED_)
