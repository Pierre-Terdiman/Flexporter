///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLX_HYPERLINK_H__0DE9CD05_2BA6_11D6_8B0F_0050BAC83302__INCLUDED_)
#define AFX_FLX_HYPERLINK_H__0DE9CD05_2BA6_11D6_8B0F_0050BAC83302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FLX_HyperLink.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FLX_HyperLink window

class FLX_HyperLink : public CStatic
{
// Construction
public:
	FLX_HyperLink();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FLX_HyperLink)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~FLX_HyperLink();

// Operations
public:

    virtual void SetURL(CString strURL);
    virtual CString GetURL() const;

    virtual void SetColors(COLORREF crLinkColor, COLORREF crVisitedColor, 
                    COLORREF crHoverColor = -1);
    virtual COLORREF GetLinkColor() const;
    virtual COLORREF GetVisitedColor() const;
    virtual COLORREF GetHoverColor() const;

    virtual void SetVisited(BOOL bVisited = TRUE);
    virtual BOOL GetVisited() const;

    virtual void SetLinkCursor(HCURSOR hCursor);
    virtual HCURSOR GetLinkCursor() const;

    virtual void SetUnderline(BOOL bUnderline = TRUE);
    virtual BOOL GetUnderline() const;

    virtual void SetAutoSize(BOOL bAutoSize = TRUE);
    virtual BOOL GetAutoSize() const;

    virtual HINSTANCE GotoURL(LPCTSTR url, int showcmd);

// Implementation
protected:
    virtual void ReportError(int nError);
    virtual LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
    virtual void PositionWindow();
    virtual void SetDefaultCursor();

// Protected attributes
protected:
    COLORREF m_crLinkColor, m_crVisitedColor;  
    COLORREF m_crHoverColor; 
    BOOL     m_bOverControl;  
    BOOL     m_bVisited;   
    BOOL     m_bUnderline;     
    BOOL     m_bAdjustToFit;  
    CString  m_strURL; 
    CFont    m_Font;   
    HCURSOR  m_hLinkCursor;   
    CToolTipCtrl m_ToolTip; 

	// Generated message map functions
protected:
	//{{AFX_MSG(FLX_HyperLink)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLX_HYPERLINK_H__0DE9CD05_2BA6_11D6_8B0F_0050BAC83302__INCLUDED_)
