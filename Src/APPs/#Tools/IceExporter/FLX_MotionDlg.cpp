///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// IceMotionDlg dialog


IceMotionDlg::IceMotionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IceMotionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(IceMotionDlg)
	m_MotionName = _T("");
	m_MotionType = _T("");
	//}}AFX_DATA_INIT
}


void IceMotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(IceMotionDlg)
	DDX_Text(pDX, IDC_MOTIONNAME, m_MotionName);
	DDX_CBString(pDX, IDC_COMBOBOXEX1, m_MotionType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(IceMotionDlg, CDialog)
	//{{AFX_MSG_MAP(IceMotionDlg)
	ON_CBN_SELCHANGE(IDC_COMBOBOXEX1, OnSelchangeComboboxex1)
	ON_CBN_EDITCHANGE(IDC_COMBOBOXEX1, OnEditchangeComboboxex1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// IceMotionDlg message handlers

void IceMotionDlg::OnSelchangeComboboxex1() 
{
	DOWNLOAD_DATA	
}

void IceMotionDlg::OnEditchangeComboboxex1() 
{
	DOWNLOAD_DATA	
}
