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

#define	DlgEnable(x)	GetDlgItem(x)->ModifyStyle(WS_DISABLED, null)
#define	DlgDisable(x)	GetDlgItem(x)->ModifyStyle(null, WS_DISABLED)

//#define DISCARD_PLUGINS

#ifndef DISCARD_PLUGINS
//	extern udword			gSelectedExporter;
//	extern udword			gNbExporters;
//	extern FormatDescriptor	gPlugs[256];
	extern	PluginManager	gPlugManager;
#endif

/////////////////////////////////////////////////////////////////////////////
// IceOptions dialog


IceOptions::IceOptions(CWnd* pParent /*=NULL*/)
	: CDialog(IceOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(IceOptions)
	m_Plugname = _T("");
	m_UseSmg = FALSE;
	m_RemoveScaling = FALSE;
	m_ForceDif = FALSE;
	m_ForceAmb = FALSE;
	m_ExpVtxCol = FALSE;
	m_ExpUVW = FALSE;
	m_DiscardW = FALSE;
	m_ExpCompleteScene = FALSE;
	m_CvtD3D = FALSE;
	m_ExpCameras = FALSE;
	m_ExpLights = FALSE;
	m_AbsolutePRS = FALSE;
	m_OpaInAlpha = FALSE;
	m_TexnameOnly = FALSE;
	m_KeepTexBum = FALSE;
	m_KeepTexDif = FALSE;
	m_KeepTexDsp = FALSE;
	m_KeepTexFil = FALSE;
	m_KeepTexOpa = FALSE;
	m_KeepTexRfl = FALSE;
	m_KeepTexRfr = FALSE;
	m_KeepTexShi = FALSE;
	m_KeepTexSlf = FALSE;
	m_KeepTexSpe = FALSE;
	m_KeepTexStr = FALSE;
	m_KeepTexAmb = FALSE;
	m_MaxTexSize = _T("");
	m_BoundingSphere = FALSE;
	m_ConvexHull = FALSE;
	m_InertiaTensor = FALSE;
	m_Extension = _T("");
	m_Consolidate = FALSE;
	m_FaceNormals = FALSE;
	m_VtxNormals = FALSE;
	m_ExpNormInfo = FALSE;
	m_Message = _T("");
	m_Target = FALSE;
	m_ExpHidden = FALSE;
	m_SRate = _T("");
	m_SamplingRate = 0;
	m_VisTrack = FALSE;
	m_EdgeVis = FALSE;
	m_Quality = 0;
	m_NbBits = _T("");
	m_Compression = -1;
	m_MakeManifold = FALSE;
	m_ExpShapes = FALSE;
	m_ExpBIPED = FALSE;
	m_FlipHorizontal = FALSE;
	m_FlipVertical = FALSE;
	m_WeightByArea = FALSE;
	m_AlwaysExportMotion = FALSE;
	m_QuantizeTextures = FALSE;
	m_ExpFOVControl = FALSE;
	m_ExpCamCtrl = FALSE;
	m_ExpLitCtrl = FALSE;
	m_MtlAnim = FALSE;
	m_ExpHelpers = FALSE;
	m_ExpPhysique = FALSE;
	m_AutoUnload = FALSE;
	m_ShapeToMesh = FALSE;
	m_ForceSpe = FALSE;
	m_ForceFil = FALSE;
	m_AutoLoad = FALSE;
	m_CacheOptimize = FALSE;
	m_GlobalScale = 0.0f;
	m_DiscardInstances = FALSE;
	m_ExpProjMaps = FALSE;
	m_ExportIK = FALSE;
	m_Locale = -1;
	m_ExpSkin = FALSE;
	m_MaterialSharing = FALSE;
	m_Sampling = -1;
	m_SingleFrame = -1;
	m_ExpWarp = FALSE;
	m_DiscardTextureAlpha = FALSE;
	m_AutoCopyTextures = FALSE;
	//}}AFX_DATA_INIT
}


void IceOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(IceOptions)
	DDX_Control(pDX, IDC_SCALE_SPINNER, m_ScaleSpinner);
	DDX_Text(pDX, IDC_PLUGNAME, m_Plugname);
	DDX_Check(pDX, IDC_USESMG, m_UseSmg);
	DDX_Check(pDX, IDC_REMOVESCALING, m_RemoveScaling);
	DDX_Check(pDX, IDC_FORCEDIF, m_ForceDif);
	DDX_Check(pDX, IDC_FORCEAMB, m_ForceAmb);
	DDX_Check(pDX, IDC_EXPVTXCOL, m_ExpVtxCol);
	DDX_Check(pDX, IDC_EXPUVW, m_ExpUVW);
	DDX_Check(pDX, IDC_DISCARDW, m_DiscardW);
	DDX_Check(pDX, IDC_EXPCOMPLETE, m_ExpCompleteScene);
	DDX_Check(pDX, IDC_CVTD3D, m_CvtD3D);
	DDX_Check(pDX, IDC_EXPCAMERAS, m_ExpCameras);
	DDX_Check(pDX, IDC_EXPLIGHTS, m_ExpLights);
	DDX_Check(pDX, IDC_ABSOLUTEPRS, m_AbsolutePRS);
	DDX_Check(pDX, IDC_OPAINALPHA, m_OpaInAlpha);
	DDX_Check(pDX, IDC_TEXNAMEONLY, m_TexnameOnly);
	DDX_Check(pDX, DC_KEEPTEXBUM, m_KeepTexBum);
	DDX_Check(pDX, DC_KEEPTEXDIF, m_KeepTexDif);
	DDX_Check(pDX, DC_KEEPTEXDSP, m_KeepTexDsp);
	DDX_Check(pDX, DC_KEEPTEXFIL, m_KeepTexFil);
	DDX_Check(pDX, DC_KEEPTEXOPA, m_KeepTexOpa);
	DDX_Check(pDX, DC_KEEPTEXRFL, m_KeepTexRfl);
	DDX_Check(pDX, DC_KEEPTEXRFR, m_KeepTexRfr);
	DDX_Check(pDX, DC_KEEPTEXSHI, m_KeepTexShi);
	DDX_Check(pDX, DC_KEEPTEXSLF, m_KeepTexSlf);
	DDX_Check(pDX, DC_KEEPTEXSPE, m_KeepTexSpe);
	DDX_Check(pDX, DC_KEEPTEXSTR, m_KeepTexStr);
	DDX_Check(pDX, IDC_KEEPTEXAMB, m_KeepTexAmb);
	DDX_CBString(pDX, IDC_MAXTEXSIZE, m_MaxTexSize);
	DDX_Check(pDX, IDC_BSPHERE, m_BoundingSphere);
	DDX_Check(pDX, IDC_CONVEXHULL, m_ConvexHull);
	DDX_Check(pDX, IDC_INERTIA, m_InertiaTensor);
	DDX_Text(pDX, IDC_EXTENSION, m_Extension);
	DDX_Check(pDX, IDC_CONSOLIDATE, m_Consolidate);
	DDX_Check(pDX, IDC_FACENORMALS, m_FaceNormals);
	DDX_Check(pDX, IDC_VTXNORMALS, m_VtxNormals);
	DDX_Check(pDX, IDC_EXPNORMINFO, m_ExpNormInfo);
	DDX_Text(pDX, IDC_MESSAGE, m_Message);
	DDX_Check(pDX, IDC_TARGET, m_Target);
	DDX_Check(pDX, IDC_EXPHIDDEN, m_ExpHidden);
	DDX_Text(pDX, IDC_SRATE, m_SRate);
	DDX_Slider(pDX, IDC_SLIDER1, m_SamplingRate);
	DDX_Check(pDX, IDC_VISTRACK, m_VisTrack);
	DDX_Check(pDX, IDC_EDGEVIS, m_EdgeVis);
	DDX_Slider(pDX, IDC_QUALITY, m_Quality);
	DDX_Text(pDX, IDC_NBBITS, m_NbBits);
	DDX_CBIndex(pDX, IDC_COMPRESSION, m_Compression);
	DDX_Check(pDX, IDC_MANIFOLD, m_MakeManifold);
	DDX_Check(pDX, IDC_EXPSHAPES, m_ExpShapes);
	DDX_Check(pDX, IDC_EXPBIPED, m_ExpBIPED);
	DDX_Check(pDX, IDC_FLIPH, m_FlipHorizontal);
	DDX_Check(pDX, IDC_FLIPV, m_FlipVertical);
	DDX_Check(pDX, IDC_WEIGHTBYAREA, m_WeightByArea);
	DDX_Check(pDX, IDC_ALWAYSEXPORTMOTION, m_AlwaysExportMotion);
	DDX_Check(pDX, IDC_QUANTIZE, m_QuantizeTextures);
	DDX_Check(pDX, IDC_FOVTRACK, m_ExpFOVControl);
	DDX_Check(pDX, IDC_EXPCAMCTRL, m_ExpCamCtrl);
	DDX_Check(pDX, IDC_EXPLITCTRL, m_ExpLitCtrl);
	DDX_Check(pDX, IDC_MTLANIM, m_MtlAnim);
	DDX_Check(pDX, IDC_EXPHELPERS, m_ExpHelpers);
	DDX_Check(pDX, IDC_EXPPHYSIQUE, m_ExpPhysique);
	DDX_Check(pDX, IDC_AUTOUNLOAD, m_AutoUnload);
	DDX_Check(pDX, IDC_SHAPETOMESH, m_ShapeToMesh);
	DDX_Check(pDX, IDC_FORCESPE, m_ForceSpe);
	DDX_Check(pDX, IDC_FORCEFIL, m_ForceFil);
	DDX_Check(pDX, IDC_AUTOLOAD, m_AutoLoad);
	DDX_Check(pDX, IDC_CACHEOPTIMIZE, m_CacheOptimize);
	DDX_Text(pDX, IDC_GLOBAL_SCALE, m_GlobalScale);
	DDX_Check(pDX, IDC_DISCARD_INSTANCES, m_DiscardInstances);
	DDX_Check(pDX, IDC_PROJECTOR_MAPS, m_ExpProjMaps);
	DDX_Check(pDX, IDC_EXPORT_IK, m_ExportIK);
	DDX_CBIndex(pDX, IDC_LOCALE, m_Locale);
	DDX_Check(pDX, IDC_EXPSKIN, m_ExpSkin);
	DDX_Check(pDX, IDC_MATERIAL_SHARING, m_MaterialSharing);
	DDX_Radio(pDX, IDC_KEYFRAMING, m_Sampling);
	DDX_Radio(pDX, IDC_ALLFRAMES, m_SingleFrame);
	DDX_Check(pDX, IDC_EXPWARP, m_ExpWarp);
	DDX_Check(pDX, IDC_DISCARD_ALPHA, m_DiscardTextureAlpha);
	DDX_Check(pDX, IDC_AUTO_COPY_TEXTURES, m_AutoCopyTextures);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(IceOptions, CDialog)
	//{{AFX_MSG_MAP(IceOptions)
	ON_BN_CLICKED(IDC_NEXTPLUG, OnNextplug)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_LOADSETTINGS, OnLoadsettings)
	ON_BN_CLICKED(IDC_SAVESETTINGS, OnSavesettings)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, OnCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_QUALITY, OnCustomdrawQuality)
	ON_CBN_SELCHANGE(IDC_COMPRESSION, OnSelchangeCompression)
	ON_BN_CLICKED(IDC_CONSOLIDATE, OnConsolidate)
	ON_BN_CLICKED(IDC_EXPUVW, OnExpuvw)
	ON_BN_CLICKED(IDC_TEXNAMEONLY, OnTexnameonly)
	ON_BN_CLICKED(IDC_SAMPLING, OnSampling)
	ON_BN_CLICKED(IDC_SINGLEFRAME, OnSingleframe)
	ON_BN_CLICKED(IDC_EXPSHAPES, OnExpshapes)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ABOUT_PLUG, OnAboutPlug)
	ON_BN_CLICKED(IDC_ALLFRAMES, OnSingleFrame)
	ON_BN_CLICKED(IDC_KEYFRAMING, OnSampling)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SCALE_SPINNER, OnDeltaposScaleSpinner)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// IceOptions message handlers

// Called when user clicks on "Next plug-in"
void IceOptions::OnNextplug()
{
#ifndef DISCARD_PLUGINS
	// Save options for current exporter
	DownloadExporter(gPlugManager.GetCurrentExporter());

	// Jump to next exporter
	gPlugManager.SelectNextExporter();

	// Update dialog
	UploadExporter(gPlugManager.GetCurrentExporter());
#endif
}

BOOL IceOptions::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Init dialog
#ifndef DISCARD_PLUGINS
	UploadExporter(gPlugManager.GetCurrentExporter());
#endif
	return TRUE;
}

IceOptions& IceOptions::UploadExporter(udword i)
{
#ifndef DISCARD_PLUGINS
	m_Message			= "";

//	m_Plugname			= gExporterNames[i];
	m_Plugname			= gPlugManager.GetExporter(i).GetName();
	String Extension("(*.");
//	Extension += gFormats[i];
	Extension += gPlugManager.GetExporter(i).GetExtension();
	Extension += " files)";
	m_Extension			= Extension;

	// General settings
//	m_Compress			= gPlugManager.GetExporter(i).GetSettings()->Compressed;
//	m_Compression		= gPlugManager.GetExporter(i).GetSettings()->mCompression;
	m_ExpCompleteScene	= gPlugManager.GetExporter(i).GetSettings()->mExportWholeScene;
	m_VisTrack			= gPlugManager.GetExporter(i).GetSettings()->mExportVisTrack;
	m_ExpHidden			= gPlugManager.GetExporter(i).GetSettings()->mExportHiddenNodes;
	m_AutoUnload		= gPlugManager.GetExporter(i).GetSettings()->mAutoUnload;
	m_AutoLoad			= gPlugManager.GetExporter(i).GetSettings()->mAutoLoad;
	m_GlobalScale		= gPlugManager.GetExporter(i).GetSettings()->mGlobalScale;
	m_Locale			= gPlugManager.GetExporter(i).GetSettings()->mLocale;
	m_ExpPhysique		= gPlugManager.GetExporter(i).GetSettings()->mExportPHYSIQUE;
	m_ExpSkin			= gPlugManager.GetExporter(i).GetSettings()->mExportSKIN;
	m_ExpBIPED			= gPlugManager.GetExporter(i).GetSettings()->mExportBIPED;
	m_AlwaysExportMotion= gPlugManager.GetExporter(i).GetSettings()->mAlwaysExportMotion;
/*
	m_Quality			= gPlugManager.GetExporter(i).GetSettings()->mNbBits;
//	m_Quality+=1;
//	m_Quality/=100;
//	m_Quality*=14;
	m_Quality--;
	m_Quality*=100;
	m_Quality/=14;

	char Buffer[256];
	sprintf(Buffer, "%d", gPlugManager.GetExporter(i).GetSettings()->mNbBits);
	strcat(Buffer, " bits/c");
	m_NbBits = Buffer;
*/
	// Meshes settings
	m_ExpUVW			= gPlugManager.GetExporter(i).GetSettings()->mExpUVW;
	m_DiscardW			= gPlugManager.GetExporter(i).GetSettings()->mDiscardW;
	m_ExpVtxCol			= gPlugManager.GetExporter(i).GetSettings()->mExpVTXColor;
	m_UseSmg			= gPlugManager.GetExporter(i).GetSettings()->mUseSmgrp;
	m_RemoveScaling		= gPlugManager.GetExporter(i).GetSettings()->mRemoveScaling;
	m_CvtD3D			= gPlugManager.GetExporter(i).GetSettings()->mConvertToD3D;
	m_AbsolutePRS		= gPlugManager.GetExporter(i).GetSettings()->mAbsolutePRS;
	m_ConvexHull		= gPlugManager.GetExporter(i).GetSettings()->mConvexHull;
	m_BoundingSphere	= gPlugManager.GetExporter(i).GetSettings()->mBoundingSphere;
	m_InertiaTensor		= gPlugManager.GetExporter(i).GetSettings()->mInertiaTensor;
	m_EdgeVis			= gPlugManager.GetExporter(i).GetSettings()->mEdgeVis;
	m_MakeManifold		= gPlugManager.GetExporter(i).GetSettings()->mMakeManifold;
	m_Target			= gPlugManager.GetExporter(i).GetSettings()->mExpTargetNodes;
	m_DiscardInstances	= gPlugManager.GetExporter(i).GetSettings()->mDiscardInstances;

	// Consolidation settings
	m_Consolidate		= gPlugManager.GetExporter(i).GetSettings()->mConsolidateMesh;
	m_FaceNormals		= gPlugManager.GetExporter(i).GetSettings()->mComputeFaceNormals;
	m_VtxNormals		= gPlugManager.GetExporter(i).GetSettings()->mComputeVertexNormals;
	m_ExpNormInfo		= gPlugManager.GetExporter(i).GetSettings()->mExportNormalInfo;
	m_WeightByArea		= gPlugManager.GetExporter(i).GetSettings()->mWeightNormalsByArea;
	m_CacheOptimize		= gPlugManager.GetExporter(i).GetSettings()->mCacheOptimize;

	// Material settings
	m_ForceAmb			= gPlugManager.GetExporter(i).GetSettings()->mForceAmbient;
	m_ForceDif			= gPlugManager.GetExporter(i).GetSettings()->mForceDiffuse;
	m_ForceSpe			= gPlugManager.GetExporter(i).GetSettings()->mForceSpecular;
	m_ForceFil			= gPlugManager.GetExporter(i).GetSettings()->mForceFilter;
	m_MtlAnim			= gPlugManager.GetExporter(i).GetSettings()->mExpMtlCtrl;
	m_MaterialSharing	= gPlugManager.GetExporter(i).GetSettings()->mMaterialSharing;

	// Texture settings
	m_OpaInAlpha		= gPlugManager.GetExporter(i).GetSettings()->mOpacityInAlpha;
	m_TexnameOnly		= gPlugManager.GetExporter(i).GetSettings()->mTexnameOnly;
	m_KeepTexAmb		= gPlugManager.GetExporter(i).GetSettings()->mKeepAmbientTexture;
	m_KeepTexDif		= gPlugManager.GetExporter(i).GetSettings()->mKeepDiffuseTexture;
	m_KeepTexSpe		= gPlugManager.GetExporter(i).GetSettings()->mKeepSpecularTexture;
	m_KeepTexShi		= gPlugManager.GetExporter(i).GetSettings()->mKeepShininessTexture;
	m_KeepTexStr		= gPlugManager.GetExporter(i).GetSettings()->mKeepShiningStrengthTexture;
	m_KeepTexSlf		= gPlugManager.GetExporter(i).GetSettings()->mKeepSelfIllumTexture;
	m_KeepTexOpa		= gPlugManager.GetExporter(i).GetSettings()->mKeepOpacityTexture;
	m_KeepTexFil		= gPlugManager.GetExporter(i).GetSettings()->mKeepFilterTexture;
	m_KeepTexBum		= gPlugManager.GetExporter(i).GetSettings()->mKeepBumpTexture;
	m_KeepTexRfl		= gPlugManager.GetExporter(i).GetSettings()->mKeepReflexionTexture;
	m_KeepTexRfr		= gPlugManager.GetExporter(i).GetSettings()->mKeepRefractionTexture;
	m_KeepTexDsp		= gPlugManager.GetExporter(i).GetSettings()->mKeepDisplacementTexture;
	m_FlipHorizontal	= gPlugManager.GetExporter(i).GetSettings()->mFlipHorizontal;
	m_FlipVertical		= gPlugManager.GetExporter(i).GetSettings()->mFlipVertical;
	m_QuantizeTextures	= gPlugManager.GetExporter(i).GetSettings()->mQuantizeTextures;
	m_DiscardTextureAlpha	= gPlugManager.GetExporter(i).GetSettings()->mDiscardTextureAlpha;
	m_AutoCopyTextures		= gPlugManager.GetExporter(i).GetSettings()->mAutoCopyTextures;

	udword tms = gPlugManager.GetExporter(i).GetSettings()->mTexMaxSize;
			if(tms==2048)	m_MaxTexSize = "2048*2048";
	else	if(tms==1024)	m_MaxTexSize = "1024*1024";
	else	if(tms==512)	m_MaxTexSize = "512*512";
	else	if(tms==256)	m_MaxTexSize = "256*256";
	else	if(tms==128)	m_MaxTexSize = "128*128";
	else	if(tms==64)		m_MaxTexSize = "64*64";
	else	if(tms==32)		m_MaxTexSize = "32*32";
	else	if(tms==16)		m_MaxTexSize = "16*16";
	else	if(tms==2)		m_MaxTexSize = "Nearest power of 2";
	else					m_MaxTexSize = "Unlimited";

	// Camera settings
	m_ExpCameras		= gPlugManager.GetExporter(i).GetSettings()->mExpCameras;
	m_ExpFOVControl		= gPlugManager.GetExporter(i).GetSettings()->mExpFOVCtrl;
	m_ExpCamCtrl		= gPlugManager.GetExporter(i).GetSettings()->mExpCamCtrl;

	// Light settings
	m_ExpLights			= gPlugManager.GetExporter(i).GetSettings()->mExpLights;
	m_ExpLitCtrl		= gPlugManager.GetExporter(i).GetSettings()->mExpLitCtrl;
	m_ExpProjMaps		= gPlugManager.GetExporter(i).GetSettings()->mExpProjMaps;
//	m_ComputeVCol		= gPlugManager.GetExporter(i).GetSettings()->mComputeVtxColors;
//	m_Shadows			= gPlugManager.GetExporter(i).GetSettings()->mComputeShadows;
//	m_ColorSmoothing	= gPlugManager.GetExporter(i).GetSettings()->mColorSmoothing;

	// Animation settings
	m_SingleFrame		= gPlugManager.GetExporter(i).GetSettings()->mSingleFrame;
	m_Sampling			= gPlugManager.GetExporter(i).GetSettings()->mSampling;
	m_SamplingRate		= gPlugManager.GetExporter(i).GetSettings()->mSamplingRate;

	char Buffer[256];
	sprintf(Buffer, "%d", gPlugManager.GetExporter(i).GetSettings()->mSamplingRate);
	strcat(Buffer, " frames/sample");
	m_SRate = Buffer;

	// Shape settings
	m_ExpShapes			= gPlugManager.GetExporter(i).GetSettings()->mExpShapes;
	m_ShapeToMesh		= gPlugManager.GetExporter(i).GetSettings()->mShapeToMesh;

	// Helper settings
	m_ExpHelpers		= gPlugManager.GetExporter(i).GetSettings()->mExpHelpers;

	// Space warps settings
	m_ExpWarp			= gPlugManager.GetExporter(i).GetSettings()->mExpSpaceWarps;

	// IK settings
	m_ExportIK			= gPlugManager.GetExporter(i).GetSettings()->mExportIK;

	// Override
//	m_Sampling	= true;

	UPLOAD_DATA
#endif

	// Setup features for the new exporter
	SetupFeatures(i);

	return *this;
}

IceOptions& IceOptions::DownloadExporter(udword i)
{
#ifndef DISCARD_PLUGINS
	DOWNLOAD_DATA

	// General settings
//	gPlugManager.GetExporter(i).GetSettings()->mCompressed					= m_Compress!=0;
//	gPlugManager.GetExporter(i).GetSettings()->mCompression					= m_Compression;
	gPlugManager.GetExporter(i).GetSettings()->mExportWholeScene			= m_ExpCompleteScene!=0;
	gPlugManager.GetExporter(i).GetSettings()->mExportVisTrack				= m_VisTrack!=0;
	gPlugManager.GetExporter(i).GetSettings()->mExportHiddenNodes			= m_ExpHidden!=0;
	gPlugManager.GetExporter(i).GetSettings()->mAutoUnload					= m_AutoUnload!=0;
	gPlugManager.GetExporter(i).GetSettings()->mAutoLoad					= m_AutoLoad!=0;
	gPlugManager.GetExporter(i).GetSettings()->mGlobalScale					= m_GlobalScale;
	gPlugManager.GetExporter(i).GetSettings()->mLocale						= m_Locale;
	gPlugManager.GetExporter(i).GetSettings()->mExportPHYSIQUE				= m_ExpPhysique!=0;
	gPlugManager.GetExporter(i).GetSettings()->mExportSKIN					= m_ExpSkin!=0;
	gPlugManager.GetExporter(i).GetSettings()->mExportBIPED					= m_ExpBIPED!=0;
	gPlugManager.GetExporter(i).GetSettings()->mAlwaysExportMotion			= m_AlwaysExportMotion!=0;
/*
	long DefValue = m_Quality;
	DefValue*=14;
	DefValue/=100;
	DefValue++;
	gPlugManager.GetExporter(i).GetSettings()->mNbBits = DefValue;
*/
	// Meshes settings
	gPlugManager.GetExporter(i).GetSettings()->mExpUVW						= m_ExpUVW!=0;
	gPlugManager.GetExporter(i).GetSettings()->mDiscardW					= m_DiscardW!=0;
	gPlugManager.GetExporter(i).GetSettings()->mExpVTXColor					= m_ExpVtxCol!=0;
	gPlugManager.GetExporter(i).GetSettings()->mUseSmgrp					= m_UseSmg!=0;
	gPlugManager.GetExporter(i).GetSettings()->mRemoveScaling				= m_RemoveScaling!=0;
	gPlugManager.GetExporter(i).GetSettings()->mConvertToD3D				= m_CvtD3D!=0;
	gPlugManager.GetExporter(i).GetSettings()->mAbsolutePRS					= m_AbsolutePRS!=0;
	gPlugManager.GetExporter(i).GetSettings()->mConvexHull					= m_ConvexHull!=0;
	gPlugManager.GetExporter(i).GetSettings()->mBoundingSphere				= m_BoundingSphere!=0;
	gPlugManager.GetExporter(i).GetSettings()->mInertiaTensor				= m_InertiaTensor!=0;
	gPlugManager.GetExporter(i).GetSettings()->mEdgeVis						= m_EdgeVis!=0;
	gPlugManager.GetExporter(i).GetSettings()->mMakeManifold				= m_MakeManifold!=0;
	gPlugManager.GetExporter(i).GetSettings()->mExpTargetNodes				= m_Target!=0;
	gPlugManager.GetExporter(i).GetSettings()->mDiscardInstances			= m_DiscardInstances!=0;

	// Consolidation settings
	gPlugManager.GetExporter(i).GetSettings()->mConsolidateMesh				= m_Consolidate!=0;
	gPlugManager.GetExporter(i).GetSettings()->mComputeFaceNormals			= m_FaceNormals!=0;
	gPlugManager.GetExporter(i).GetSettings()->mComputeVertexNormals		= m_VtxNormals!=0;
	gPlugManager.GetExporter(i).GetSettings()->mExportNormalInfo			= m_ExpNormInfo!=0;
	gPlugManager.GetExporter(i).GetSettings()->mWeightNormalsByArea			= m_WeightByArea!=0;
	gPlugManager.GetExporter(i).GetSettings()->mCacheOptimize				= m_CacheOptimize!=0;

	// Material settings
	gPlugManager.GetExporter(i).GetSettings()->mForceAmbient				= m_ForceAmb!=0;
	gPlugManager.GetExporter(i).GetSettings()->mForceDiffuse				= m_ForceDif!=0;
	gPlugManager.GetExporter(i).GetSettings()->mForceSpecular				= m_ForceSpe!=0;
	gPlugManager.GetExporter(i).GetSettings()->mForceFilter					= m_ForceFil!=0;
	gPlugManager.GetExporter(i).GetSettings()->mExpMtlCtrl					= m_MtlAnim!=0;
	gPlugManager.GetExporter(i).GetSettings()->mMaterialSharing				= m_MaterialSharing!=0;

	// Texture settings
	gPlugManager.GetExporter(i).GetSettings()->mOpacityInAlpha				= m_OpaInAlpha!=0;
	gPlugManager.GetExporter(i).GetSettings()->mTexnameOnly					= m_TexnameOnly!=0;
	gPlugManager.GetExporter(i).GetSettings()->mKeepAmbientTexture			= m_KeepTexAmb!=0;
	gPlugManager.GetExporter(i).GetSettings()->mKeepDiffuseTexture			= m_KeepTexDif!=0;
	gPlugManager.GetExporter(i).GetSettings()->mKeepSpecularTexture			= m_KeepTexSpe!=0;
	gPlugManager.GetExporter(i).GetSettings()->mKeepShininessTexture		= m_KeepTexShi!=0;
	gPlugManager.GetExporter(i).GetSettings()->mKeepShiningStrengthTexture	= m_KeepTexStr!=0;
	gPlugManager.GetExporter(i).GetSettings()->mKeepSelfIllumTexture		= m_KeepTexSlf!=0;
	gPlugManager.GetExporter(i).GetSettings()->mKeepOpacityTexture			= m_KeepTexOpa!=0;
	gPlugManager.GetExporter(i).GetSettings()->mKeepFilterTexture			= m_KeepTexFil!=0;
	gPlugManager.GetExporter(i).GetSettings()->mKeepBumpTexture				= m_KeepTexBum!=0;
	gPlugManager.GetExporter(i).GetSettings()->mKeepReflexionTexture		= m_KeepTexRfl!=0;
	gPlugManager.GetExporter(i).GetSettings()->mKeepRefractionTexture		= m_KeepTexRfr!=0;
	gPlugManager.GetExporter(i).GetSettings()->mKeepDisplacementTexture		= m_KeepTexDsp!=0;
	gPlugManager.GetExporter(i).GetSettings()->mFlipHorizontal				= m_FlipHorizontal!=0;
	gPlugManager.GetExporter(i).GetSettings()->mFlipVertical				= m_FlipVertical!=0;
	gPlugManager.GetExporter(i).GetSettings()->mQuantizeTextures			= m_QuantizeTextures!=0;
	gPlugManager.GetExporter(i).GetSettings()->mDiscardTextureAlpha			= m_DiscardTextureAlpha!=0;
	gPlugManager.GetExporter(i).GetSettings()->mAutoCopyTextures			= m_AutoCopyTextures!=0;

	String tms(m_MaxTexSize);
			if(tms==String("Unlimited"))			gPlugManager.GetExporter(i).GetSettings()->mTexMaxSize = 0;
	else	if(tms==String("2048*2048"))			gPlugManager.GetExporter(i).GetSettings()->mTexMaxSize = 2048;
	else	if(tms==String("1024*1024"))			gPlugManager.GetExporter(i).GetSettings()->mTexMaxSize = 1024;
	else	if(tms==String("512*512"))				gPlugManager.GetExporter(i).GetSettings()->mTexMaxSize = 512;
	else	if(tms==String("256*256"))				gPlugManager.GetExporter(i).GetSettings()->mTexMaxSize = 256;
	else	if(tms==String("128*128"))				gPlugManager.GetExporter(i).GetSettings()->mTexMaxSize = 128;
	else	if(tms==String("64*64"))				gPlugManager.GetExporter(i).GetSettings()->mTexMaxSize = 64;
	else	if(tms==String("32*32"))				gPlugManager.GetExporter(i).GetSettings()->mTexMaxSize = 32;
	else	if(tms==String("16*16"))				gPlugManager.GetExporter(i).GetSettings()->mTexMaxSize = 16;
	else	if(tms==String("Nearest power of 2"))	gPlugManager.GetExporter(i).GetSettings()->mTexMaxSize = 2;

	// Camera settings
	gPlugManager.GetExporter(i).GetSettings()->mExpCameras					= m_ExpCameras!=0;
	gPlugManager.GetExporter(i).GetSettings()->mExpFOVCtrl					= m_ExpFOVControl!=0;
	gPlugManager.GetExporter(i).GetSettings()->mExpCamCtrl					= m_ExpCamCtrl!=0;

	// Light settings
	gPlugManager.GetExporter(i).GetSettings()->mExpLights					= m_ExpLights!=0;
	gPlugManager.GetExporter(i).GetSettings()->mExpLitCtrl					= m_ExpLitCtrl!=0;
	gPlugManager.GetExporter(i).GetSettings()->mExpProjMaps					= m_ExpProjMaps!=0;
//	gPlugManager.GetExporter(i).GetSettings()->mComputeVtxColors			= m_ComputeVCol!=0;
//	gPlugManager.GetExporter(i).GetSettings()->mComputeShadows				= m_Shadows!=0;
//	gPlugManager.GetExporter(i).GetSettings()->mColorSmoothing				= m_ColorSmoothing!=0;

	// Animation settings
	gPlugManager.GetExporter(i).GetSettings()->mSingleFrame					= m_SingleFrame!=0;
	gPlugManager.GetExporter(i).GetSettings()->mSampling					= m_Sampling!=0;
	gPlugManager.GetExporter(i).GetSettings()->mSamplingRate				= m_SamplingRate;

	// Shape settings
	gPlugManager.GetExporter(i).GetSettings()->mExpShapes					= m_ExpShapes!=0;
	gPlugManager.GetExporter(i).GetSettings()->mShapeToMesh					= m_ShapeToMesh!=0;

	// Helper settings
	gPlugManager.GetExporter(i).GetSettings()->mExpHelpers					= m_ExpHelpers!=0;

	// Space warps settings
	gPlugManager.GetExporter(i).GetSettings()->mExpSpaceWarps				= m_ExpWarp!=0;

	// IK settings
	gPlugManager.GetExporter(i).GetSettings()->mExportIK					= m_ExportIK!=0;

#endif
	return *this;
}


void IceOptions::OnClose() 
{
#ifndef DISCARD_PLUGINS
	// Save options for current exporter
	DownloadExporter(gPlugManager.GetCurrentExporter());
#endif	
	CDialog::OnClose();
}

// Called when user clicks on "Load settings"
void IceOptions::OnLoadsettings() 
{
#ifndef DISCARD_PLUGINS
	udword i = gPlugManager.GetCurrentExporter();
	udword Value=0;

	// Load settings from registry
	bool Status = gPlugManager.GetExporter(i).LoadSettings();

	// Set current settings
	UploadExporter(i);

	// Messages
	m_Message = "Settings loaded.";
	char Buffer[256];
//	sprintf(Buffer, "%d", gUserSettings[i]->mSamplingRate);
	sprintf(Buffer, "%d", gPlugManager.GetExporter(i).GetSettings()->mSamplingRate);
	strcat(Buffer, " frames/sample");
	m_SRate = Buffer;

	UPLOAD_DATA;
#endif

	SetupFeatures(gPlugManager.GetCurrentExporter());
}

// Called when user clicks on "Save settings"
void IceOptions::OnSavesettings() 
{
#ifndef DISCARD_PLUGINS
	udword i = gPlugManager.GetCurrentExporter();

	// Get current settings
	DownloadExporter(i);

	// Save settings to registry
	bool Status = gPlugManager.GetExporter(i).SaveSettings();

	// Message
	m_Message = "Settings saved.";
	UPLOAD_DATA;
#endif	
}

void IceOptions::OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	char SlideText[256];
	UpdateData(TRUE);

	long DefValue = m_SamplingRate;
	if(!DefValue) DefValue++;
//	gUserSettings[gPlugManager.GetCurrentExporter()]->mSamplingRate	= DefValue;
	gPlugManager.GetSelectedExporter().GetSettings()->mSamplingRate	= DefValue;
	sprintf(SlideText, "%d", DefValue);
	strcat(SlideText, " frames/sample");
	m_SRate = SlideText;

	UpdateData(FALSE);
	*pResult = 0;
}

void IceOptions::OnCustomdrawQuality(NMHDR* pNMHDR, LRESULT* pResult) 
{
	char SlideText[256];
	UpdateData(TRUE);

	long DefValue = m_Quality;
	// DefValue between 0 and 100
	DefValue*=14;
	DefValue/=100;
	DefValue++;

//	gUserSettings[gPlugManager.GetCurrentExporter()]->NbBits = DefValue;

	sprintf(SlideText, "%d", DefValue);
	strcat(SlideText, " bits/c");
	m_NbBits = SlideText;

	UpdateData(FALSE);
	*pResult = 0;
}

// Enable/disable features according to settings
void IceOptions::SetupFeatures(udword i)
{
	DOWNLOAD_DATA;

//#define SETUP_PERMISSION(member, flag)	if(!gEnabledSettings[i]->member)	DlgDisable(flag);

#define SETUP_PERMISSION(member, flag)					\
/*	if(gEnabledSettings[i]->member)	DlgEnable(flag);*/	\
	if(gPlugManager.GetExporter(i).GetEnabledSettings()->member)	DlgEnable(flag);	\
	else										DlgDisable(flag);	\

	// Enable/disable according to user
//	if(gEnabledSettings[i])
	if(gPlugManager.GetExporter(i).GetEnabledSettings())
	{
//		SETUP_PERMISSION(mCompression,					IDC_COMPRESSION)
		SETUP_PERMISSION(mExportWholeScene,				IDC_EXPCOMPLETE)
		SETUP_PERMISSION(mExportVisTrack,				IDC_VISTRACK)
		SETUP_PERMISSION(mExportHiddenNodes,			IDC_EXPHIDDEN)
		SETUP_PERMISSION(mAutoUnload,					IDC_AUTOUNLOAD)
		SETUP_PERMISSION(mAutoLoad,						IDC_AUTOLOAD)
		SETUP_PERMISSION(mGlobalScale,					IDC_GLOBAL_SCALE)
		SETUP_PERMISSION(mLocale,						IDC_LOCALE)
		SETUP_PERMISSION(mExportPHYSIQUE,				IDC_EXPPHYSIQUE)
		SETUP_PERMISSION(mExportSKIN,					IDC_EXPSKIN)
		SETUP_PERMISSION(mExportBIPED,					IDC_EXPBIPED)
		SETUP_PERMISSION(mAlwaysExportMotion,			IDC_ALWAYSEXPORTMOTION)

		SETUP_PERMISSION(mExpUVW,						IDC_EXPUVW)
		SETUP_PERMISSION(mDiscardW,						IDC_DISCARDW)
		SETUP_PERMISSION(mExpVTXColor,					IDC_EXPVTXCOL)
		SETUP_PERMISSION(mUseSmgrp,						IDC_USESMG)
		SETUP_PERMISSION(mRemoveScaling,				IDC_REMOVESCALING)
		SETUP_PERMISSION(mConvertToD3D,					IDC_CVTD3D)
		SETUP_PERMISSION(mAbsolutePRS,					IDC_ABSOLUTEPRS)
		SETUP_PERMISSION(mConvexHull,					IDC_CONVEXHULL)
		SETUP_PERMISSION(mBoundingSphere,				IDC_BSPHERE)
		SETUP_PERMISSION(mInertiaTensor,				IDC_INERTIA)
		SETUP_PERMISSION(mEdgeVis,						IDC_EDGEVIS)
		SETUP_PERMISSION(mMakeManifold,					IDC_MANIFOLD)
		SETUP_PERMISSION(mExpTargetNodes,				IDC_TARGET)
		SETUP_PERMISSION(mDiscardInstances,				IDC_DISCARD_INSTANCES)

		SETUP_PERMISSION(mConsolidateMesh,				IDC_CONSOLIDATE)
		SETUP_PERMISSION(mComputeFaceNormals,			IDC_FACENORMALS)
		SETUP_PERMISSION(mComputeVertexNormals,			IDC_VTXNORMALS)
		SETUP_PERMISSION(mExportNormalInfo,				IDC_EXPNORMINFO)
		SETUP_PERMISSION(mWeightNormalsByArea,			IDC_WEIGHTBYAREA)
		SETUP_PERMISSION(mCacheOptimize,				IDC_CACHEOPTIMIZE)

		SETUP_PERMISSION(mForceAmbient,					IDC_FORCEAMB)
		SETUP_PERMISSION(mForceDiffuse,					IDC_FORCEDIF)
		SETUP_PERMISSION(mForceSpecular,				IDC_FORCESPE)
		SETUP_PERMISSION(mForceFilter,					IDC_FORCEFIL)
		SETUP_PERMISSION(mExpMtlCtrl,					IDC_MTLANIM)
		SETUP_PERMISSION(mMaterialSharing,				IDC_MATERIAL_SHARING)

		SETUP_PERMISSION(mOpacityInAlpha,				IDC_OPAINALPHA)
		SETUP_PERMISSION(mTexnameOnly,					IDC_TEXNAMEONLY)
		SETUP_PERMISSION(mTexMaxSize,					IDC_MAXTEXSIZE)
		SETUP_PERMISSION(mKeepAmbientTexture,			IDC_KEEPTEXAMB)
		SETUP_PERMISSION(mKeepDiffuseTexture,			DC_KEEPTEXDIF)
		SETUP_PERMISSION(mKeepSpecularTexture,			DC_KEEPTEXSPE)
		SETUP_PERMISSION(mKeepShininessTexture,			DC_KEEPTEXSHI)
		SETUP_PERMISSION(mKeepShiningStrengthTexture,	DC_KEEPTEXSTR)
		SETUP_PERMISSION(mKeepSelfIllumTexture,			DC_KEEPTEXSLF)
		SETUP_PERMISSION(mKeepOpacityTexture,			DC_KEEPTEXOPA)
		SETUP_PERMISSION(mKeepFilterTexture,			DC_KEEPTEXFIL)
		SETUP_PERMISSION(mKeepBumpTexture,				DC_KEEPTEXBUM)
		SETUP_PERMISSION(mKeepReflexionTexture,			DC_KEEPTEXRFL)
		SETUP_PERMISSION(mKeepRefractionTexture,		DC_KEEPTEXRFR)
		SETUP_PERMISSION(mKeepDisplacementTexture,		DC_KEEPTEXDSP)
		SETUP_PERMISSION(mFlipHorizontal,				IDC_FLIPH)
		SETUP_PERMISSION(mFlipVertical,					IDC_FLIPV)
		SETUP_PERMISSION(mQuantizeTextures,				IDC_QUANTIZE)
		SETUP_PERMISSION(mDiscardTextureAlpha,			IDC_DISCARD_ALPHA)
		SETUP_PERMISSION(mAutoCopyTextures,				IDC_AUTO_COPY_TEXTURES)

		SETUP_PERMISSION(mExpCameras,					IDC_EXPCAMERAS)
		SETUP_PERMISSION(mExpFOVCtrl,					IDC_FOVTRACK)
		SETUP_PERMISSION(mExpCamCtrl,					IDC_EXPCAMCTRL)

		SETUP_PERMISSION(mExpLights,					IDC_EXPLIGHTS)
		SETUP_PERMISSION(mExpLitCtrl,					IDC_EXPLITCTRL)
		SETUP_PERMISSION(mExpProjMaps,					IDC_PROJECTOR_MAPS)
//		SETUP_PERMISSION(mComputeVtxColors,				IDC_COMPUTEVCOL)
//		SETUP_PERMISSION(mComputeShadows,				IDC_SHADOW)
//		SETUP_PERMISSION(mColorSmoothing,				IDC_COLORSMOOTH)

		SETUP_PERMISSION(mSingleFrame,					IDC_SINGLEFRAME)
		SETUP_PERMISSION(mSampling,						IDC_SAMPLING)
		SETUP_PERMISSION(mSamplingRate,					IDC_SRATE)

		SETUP_PERMISSION(mExpShapes,					IDC_EXPSHAPES)
		SETUP_PERMISSION(mShapeToMesh,					IDC_SHAPETOMESH)
		SETUP_PERMISSION(mExpHelpers,					IDC_EXPHELPERS)
		SETUP_PERMISSION(mExpSpaceWarps,				IDC_EXPWARP)

		SETUP_PERMISSION(mExportIK,						IDC_EXPORT_IK)
	}


	// Enable/disable according to logic
//	if(m_Compression==FLX_COMPRESSION_NONE)	DlgDisable(IDC_QUALITY);
//	else									DlgEnable(IDC_QUALITY);

	if(m_Consolidate)
	{
//		DlgEnable(IDC_FACENORMALS);
//		DlgEnable(IDC_VTXNORMALS);
//		DlgEnable(IDC_EXPNORMINFO);
//		DlgEnable(IDC_WEIGHTBYAREA);
	}
	else
	{
		DlgDisable(IDC_FACENORMALS);
		DlgDisable(IDC_VTXNORMALS);
		DlgDisable(IDC_EXPNORMINFO);
		DlgDisable(IDC_WEIGHTBYAREA);
		DlgDisable(IDC_CACHEOPTIMIZE);
	}

	if(!m_ExpUVW)	DlgDisable(IDC_DISCARDW);
//	else			DlgEnable(IDC_DISCARDW);

	if(m_TexnameOnly)
	{
//		DlgDisable(IDC_MAXTEXSIZE);
		DlgDisable(IDC_OPAINALPHA);
		DlgDisable(IDC_FLIPH);
		DlgDisable(IDC_FLIPV);
		DlgDisable(IDC_QUANTIZE);
	}
	else
	{
//		DlgEnable(IDC_MAXTEXSIZE);
//		DlgEnable(IDC_OPAINALPHA);
//		DlgEnable(IDC_FLIPH);
//		DlgEnable(IDC_FLIPV);
//		DlgEnable(IDC_QUANTIZE);
	}

/*
	if(m_ComputeVCol)
	{
//		DlgEnable(IDC_SHADOW);
//		DlgEnable(IDC_COLORSMOOTH);
//		DlgEnable(IDC_LIGHTMAP);
	}
	else
	{
		DlgDisable(IDC_SHADOW);
		DlgDisable(IDC_COLORSMOOTH);
		DlgDisable(IDC_LIGHTMAP);
	}
*/
	if(m_Sampling)
	{
//		DlgEnable(IDC_SLIDER1);
	}
	else
	{
		DlgDisable(IDC_SLIDER1);
	}

	if(m_SingleFrame)
	{
		DlgDisable(IDC_VISTRACK);
		DlgDisable(IDC_FOVTRACK);
		DlgDisable(IDC_MTLANIM);
		DlgDisable(IDC_EXPCAMCTRL);
		DlgDisable(IDC_EXPLITCTRL);
	}
	else
	{
//		DlgEnable(IDC_VISTRACK);
//		DlgEnable(IDC_FOVTRACK);
//		DlgEnable(IDC_MTLANIM);
//		DlgEnable(IDC_EXPCAMCTRL);
//		DlgEnable(IDC_EXPLITCTRL);
	}

	if(!m_ExpShapes)
	{
		DlgDisable(IDC_SHAPETOMESH);
	}

	if(!m_ExpCameras)
	{
		DlgDisable(IDC_EXPCAMCTRL);
	}

	if(!m_ExpLights)
	{
		DlgDisable(IDC_EXPLITCTRL);
		DlgDisable(IDC_PROJECTOR_MAPS);
	}

	// Temp overrides
//	DlgDisable(IDC_MTLANIM);
//	DlgDisable(IDC_LIGHTMAP);

	// Force redraw
	Invalidate();
}

// Here we must "setup features" again each time the user clicks on one of the following items.
// ex: the user deselects "Consolidate" => we must gray all consolidation-related settings
void IceOptions::OnSelchangeCompression()	{ SetupFeatures(gPlugManager.GetCurrentExporter()); }
void IceOptions::OnConsolidate()			{ SetupFeatures(gPlugManager.GetCurrentExporter()); }
void IceOptions::OnExpuvw()					{ SetupFeatures(gPlugManager.GetCurrentExporter()); }
void IceOptions::OnTexnameonly()			{ SetupFeatures(gPlugManager.GetCurrentExporter()); }
//void IceOptions::OnComputevcol()			{ SetupFeatures(gPlugManager.GetCurrentExporter()); }
void IceOptions::OnSampling()				{ SetupFeatures(gPlugManager.GetCurrentExporter()); }
void IceOptions::OnSingleframe()			{ SetupFeatures(gPlugManager.GetCurrentExporter()); }
void IceOptions::OnSingleFrame()			{ SetupFeatures(gPlugManager.GetCurrentExporter()); }
void IceOptions::OnExpshapes() 				{ SetupFeatures(gPlugManager.GetCurrentExporter()); }

void IceOptions::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

/*
	HWND Handle;
	GetDlgItem(IDC_AMBIENTCOLOR, &Handle);

	{
		HDC myDC = ::GetDC(Handle);
		//
		RECT Rect;
		::GetClientRect(Handle, &Rect);
		HBRUSH myBrush = ::CreateSolidBrush(0);
		HBRUSH oldBrush = (HBRUSH) ::SelectObject(myDC, myBrush);
		::FillRect(myDC, &Rect, myBrush);
		::SelectObject(myDC, oldBrush);
		::ReleaseDC(Handle, myDC);
	}
*/

	// Do not call CDialog::OnPaint() for painting messages
}

BOOL IceOptions::PreTranslateMessage(MSG* pMsg)
{
/*
	HWND Handle;
	GetDlgItem(IDC_AMBIENTCOLOR, &Handle);

	if(pMsg->hwnd == Handle)
	{
		//TRACE1("Message %d\n", pMsg->message);

	}

	if(pMsg->hwnd == Handle)
//	if(pMsg->hwnd == Handle && pMsg->message == WM_PAINT)
//	if(pMsg->message == WM_PAINT)
	{
		HDC myDC = ::GetDC(Handle);
		//
		RECT Rect;
		::GetClientRect(Handle, &Rect);
		HBRUSH myBrush = ::CreateSolidBrush(0);
		HBRUSH oldBrush = (HBRUSH) ::SelectObject(myDC, myBrush);
		::FillRect(myDC, &Rect, myBrush);
		::SelectObject(myDC, oldBrush);
		::ReleaseDC(Handle, myDC);
		return TRUE;
	}
*/
	return CDialog::PreTranslateMessage(pMsg);
}

// Called when user clicks on "About plug-in"
void IceOptions::OnAboutPlug()
{
	ExportFormat* Format = gPlugManager.GetSelectedExporter().GetPlug();
	if(Format)	Format->About();
}

// Called when user clicks on "OK"
void IceOptions::OnOK()
{
#ifndef DISCARD_PLUGINS
	// Save options for current exporter
	DownloadExporter(gPlugManager.GetCurrentExporter());
#endif	
	CDialog::OnOK();
}

// Called when user clicks on "CANCEL"
void IceOptions::OnCancel()
{
	CDialog::OnCancel();
}

void IceOptions::OnDeltaposScaleSpinner(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	DOWNLOAD_DATA
	m_GlobalScale -= float(pNMUpDown->iDelta)*0.1f;
	if(m_GlobalScale<0.0f)	m_GlobalScale = 0.0f;
	UPLOAD_DATA;
}
