///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICEOPTIONS_H__B0AFB003_155E_11D4_8B0F_0050BAC83302__INCLUDED_)
#define AFX_ICEOPTIONS_H__B0AFB003_155E_11D4_8B0F_0050BAC83302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IceOptions.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// IceOptions dialog

class IceOptions : public CDialog
{
// Construction
public:
						IceOptions(CWnd* pParent = NULL);   // standard constructor

			IceOptions&	UploadExporter(udword i);
			IceOptions&	DownloadExporter(udword i);
			void		SetupFeatures(udword i);

	virtual	void		OnOK();
	virtual	void		OnCancel();

// Dialog Data
	//{{AFX_DATA(IceOptions)
	enum { IDD = IDD_OPTIONS };
	CSpinButtonCtrl	m_ScaleSpinner;
	CString	m_Plugname;
	BOOL	m_UseSmg;
	BOOL	m_RemoveScaling;
	BOOL	m_ForceDif;
	BOOL	m_ForceAmb;
	BOOL	m_ExpVtxCol;
	BOOL	m_ExpUVW;
	BOOL	m_DiscardW;
	BOOL	m_ExpCompleteScene;
	BOOL	m_CvtD3D;
	BOOL	m_ExpCameras;
	BOOL	m_ExpLights;
	BOOL	m_AbsolutePRS;
	BOOL	m_OpaInAlpha;
	BOOL	m_TexnameOnly;
	BOOL	m_KeepTexBum;
	BOOL	m_KeepTexDif;
	BOOL	m_KeepTexDsp;
	BOOL	m_KeepTexFil;
	BOOL	m_KeepTexOpa;
	BOOL	m_KeepTexRfl;
	BOOL	m_KeepTexRfr;
	BOOL	m_KeepTexShi;
	BOOL	m_KeepTexSlf;
	BOOL	m_KeepTexSpe;
	BOOL	m_KeepTexStr;
	BOOL	m_KeepTexAmb;
	CString	m_MaxTexSize;
	BOOL	m_BoundingSphere;
	BOOL	m_ConvexHull;
	BOOL	m_InertiaTensor;
	CString	m_Extension;
	BOOL	m_Consolidate;
	BOOL	m_FaceNormals;
	BOOL	m_VtxNormals;
	BOOL	m_ExpNormInfo;
	CString	m_Message;
	BOOL	m_Target;
	BOOL	m_ExpHidden;
	CString	m_SRate;
	int		m_SamplingRate;
	BOOL	m_VisTrack;
	BOOL	m_EdgeVis;
	int		m_Quality;
	CString	m_NbBits;
	int		m_Compression;
	BOOL	m_MakeManifold;
	BOOL	m_ExpShapes;
	BOOL	m_ExpBIPED;
	BOOL	m_FlipHorizontal;
	BOOL	m_FlipVertical;
	BOOL	m_WeightByArea;
	BOOL	m_AlwaysExportMotion;
	BOOL	m_QuantizeTextures;
	BOOL	m_ExpFOVControl;
	BOOL	m_ExpCamCtrl;
	BOOL	m_ExpLitCtrl;
	BOOL	m_MtlAnim;
	BOOL	m_ExpHelpers;
	BOOL	m_ExpPhysique;
	BOOL	m_AutoUnload;
	BOOL	m_ShapeToMesh;
	BOOL	m_ForceSpe;
	BOOL	m_ForceFil;
	BOOL	m_AutoLoad;
	BOOL	m_CacheOptimize;
	float	m_GlobalScale;
	BOOL	m_DiscardInstances;
	BOOL	m_ExpProjMaps;
	BOOL	m_ExportIK;
	int		m_Locale;
	BOOL	m_ExpSkin;
	BOOL	m_MaterialSharing;
	int		m_Sampling;
	int		m_SingleFrame;
	BOOL	m_ExpWarp;
	BOOL	m_DiscardTextureAlpha;
	BOOL	m_AutoCopyTextures;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(IceOptions)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(IceOptions)
	afx_msg void OnNextplug();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnLoadsettings();
	afx_msg void OnSavesettings();
	afx_msg void OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawQuality(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCompression();
	afx_msg void OnConsolidate();
	afx_msg void OnExpuvw();
	afx_msg void OnTexnameonly();
	afx_msg void OnComputevcol();
	afx_msg void OnSampling();
	afx_msg void OnSingleframe();
	afx_msg void OnExpshapes();
	afx_msg void OnPaint();
	afx_msg void OnAboutPlug();
	afx_msg void OnSingleFrame();
	afx_msg void OnDeltaposScaleSpinner(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICEOPTIONS_H__B0AFB003_155E_11D4_8B0F_0050BAC83302__INCLUDED_)
