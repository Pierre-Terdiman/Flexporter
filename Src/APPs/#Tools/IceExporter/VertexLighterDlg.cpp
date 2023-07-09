// VertexLighterDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "IceExporter.h"
//#include "VertexLighterDlg.h"

using namespace IceExporter;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// VertexLighterDlg dialog


VertexLighterDlg::VertexLighterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(VertexLighterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(VertexLighterDlg)
	m_ComputeShadows = FALSE;
	m_ColorSmoothing = -1;
	m_Specular = FALSE;
	m_Clamp = FALSE;
	//}}AFX_DATA_INIT
}


void VertexLighterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(VertexLighterDlg)
	DDX_Check(pDX, IDC_PRELIGHTER_SHADOWS, m_ComputeShadows);
	DDX_CBIndex(pDX, IDC_COLOR_SMOOTHING, m_ColorSmoothing);
	DDX_Check(pDX, IDC_PRELIGHTER_SPECULAR, m_Specular);
	DDX_Check(pDX, IDC_PRELIGHTER_CLAMP, m_Clamp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(VertexLighterDlg, CDialog)
	//{{AFX_MSG_MAP(VertexLighterDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// VertexLighterDlg message handlers

void VertexLighterDlg::SaveSettings()
{
	// Save settings to registry
	Save("Flexporter", "VertexLighter", "ComputeShadows",	(udword)m_ComputeShadows);
	Save("Flexporter", "VertexLighter", "ColorSmoothing",	(udword)m_ColorSmoothing);
	Save("Flexporter", "VertexLighter", "Specular",			(udword)m_Specular);
	Save("Flexporter", "VertexLighter", "Clamp",			(udword)m_Clamp);
}

void VertexLighterDlg::LoadSettings()
{
	// Load settings from registry
	udword Value;
	if(Load("Flexporter", "VertexLighter", "ComputeShadows",	Value))	m_ComputeShadows	= Value;
	if(Load("Flexporter", "VertexLighter", "ColorSmoothing",	Value))	m_ColorSmoothing	= Value;
	if(Load("Flexporter", "VertexLighter", "Specular",			Value))	m_Specular			= Value;
	if(Load("Flexporter", "VertexLighter", "Clamp",				Value))	m_Clamp				= Value;
}
