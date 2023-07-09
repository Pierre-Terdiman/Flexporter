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
// IceMessage dialog


IceMessage::IceMessage(CWnd* pParent /*=NULL*/)
	: CDialog(IceMessage::IDD, pParent)
{
	//{{AFX_DATA_INIT(IceMessage)
	m_Text = _T("");
	//}}AFX_DATA_INIT
}


void IceMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(IceMessage)
	DDX_Text(pDX, IDC_EDIT, m_Text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(IceMessage, CDialog)
	//{{AFX_MSG_MAP(IceMessage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// IceMessage message handlers
