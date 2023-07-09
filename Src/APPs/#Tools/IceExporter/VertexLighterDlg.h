#if !defined(AFX_VERTEXLIGHTERDLG_H__A264C942_7659_11D7_AD2A_00E018BDA063__INCLUDED_)
#define AFX_VERTEXLIGHTERDLG_H__A264C942_7659_11D7_AD2A_00E018BDA063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VertexLighterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// VertexLighterDlg dialog

class VertexLighterDlg : public CDialog
{
// Construction
public:
			VertexLighterDlg(CWnd* pParent = NULL);   // standard constructor

	void	SaveSettings();
	void	LoadSettings();

// Dialog Data
	//{{AFX_DATA(VertexLighterDlg)
	enum { IDD = IDD_VERTEX_LIGHTER };
	BOOL	m_ComputeShadows;
	int		m_ColorSmoothing;
	BOOL	m_Specular;
	BOOL	m_Clamp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(VertexLighterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(VertexLighterDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERTEXLIGHTERDLG_H__A264C942_7659_11D7_AD2A_00E018BDA063__INCLUDED_)
