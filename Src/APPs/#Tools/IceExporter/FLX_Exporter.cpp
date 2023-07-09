///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	The Flexporter application.
 *	\file		FLX_Exporter.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 10, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

/////////////////////////////////////////////////////////////////////////////
// CIceExporterApp

BEGIN_MESSAGE_MAP(CIceExporterApp, CWinApp)
	//{{AFX_MSG_MAP(CIceExporterApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIceExporterApp construction

CIceExporterApp::CIceExporterApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIceExporterApp object
static	CIceExporterApp	gApp;				// The application
		ExporterPanel	gExporterPanel;		// The exporter's panel
static	ExporterDesc	gExporterDesc;		// The exporter's descriptor

BOOL CIceExporterApp::InitInstance() 
{
	// Get the DLL's HINSTANCE
	HINSTANCE hInstance = AfxGetResourceHandle();

	// Save hInstance
	gExporterPanel.hInstance = hInstance;

	// Initialize 3DS Max Custom Controls
	InitCustomControls(hInstance);

	// Initialize Win32 Controls
	IceCore::InitCommonControls();

	// Initialize ICE
	ICECORECREATE ICC;
	ICC.mSetupFPU = false;
	InitIceCore(&ICC);
	InitIceMaths();
	InitIceImageWork();
	InitMeshmerizer();

	return TRUE;
}


int CIceExporterApp::ExitInstance() 
{
	UnbindFunctions();

	CloseMeshmerizer();
	CloseIceImageWork();
	CloseIceMaths();
	CloseIceCore();

	return CWinApp::ExitInstance();
}

// Below are exported functions for MAX
#ifdef TEST_MYSPHERE
ClassDesc* GetSphereDesc();
#endif

ClassDesc2* GetMatCustAttribDesc();
ClassDesc2* GetFlexporterMaterialDesc();
//VertexDataModifierDesc* GetVertexDataModifierDesc();

	//! 3DS Max Identification Callbacks
	FUNCTION ICEEXPORTER_API const TCHAR* LibDescription()
	{ 
		// This text appears in MAX in file/Summary info/Plug-ins infos  (or when the DLL is not found -> this text is stored in .MAX files) 
		return _T("Zappy's ICE Flexporter");
	}

	//! Returns the Number of Classes present in the DLLs
	FUNCTION ICEEXPORTER_API int LibNumberClasses()
	{
		int NbPlugs = 1;	// Flexporter's DLU
		NbPlugs++;			// Custom Material
		NbPlugs++;			// ZCB importer
		NbPlugs++;			// Flexporter material
		NbPlugs++;			// Vertex Data modifier
#ifdef TEST_MYSPHERE
		NbPlugs++;			// Sphere object
#endif
		return NbPlugs;
	}

	//! Returns the i'th Class Description Object
	FUNCTION ICEEXPORTER_API ClassDesc* LibClassDesc(int i)
	{
		if(i==0)	return &gExporterDesc;		// Returns the Exporter's Descriptor
		if(i==1)	return GetZCBDesc();
		if(i==2)	return GetMatCustAttribDesc();
		if(i==3)	return GetFlexporterMaterialDesc();
		if(i==4)	return GetVertexDataModifierDesc();
#ifdef TEST_MYSPHERE
		if(i==5)	return GetSphereDesc();
#endif
					return null;
	}

	//! MAX API Version
	FUNCTION ICEEXPORTER_API ULONG LibVersion()
	{
		return VERSION_3DSMAX;
	}



// TEST
#ifdef TEST_MYSPHERE

#undef Ray

//#include "prim.h"
//#include "iparamm.h"
//#include "Simpobj.h"
//#include "surf_api.h"
#include "d:\MAX\MAX4SDK\INCLUDE\notify.h"


class SphereObject : public GenSphere, public IParamArray
{
	public:			
															SphereObject();
															~SphereObject();
		// From Object

		// Objects need to be able convert themselves 
		// to TriObjects. Most modifiers will ask for
		// Deformable Objects, and triobjects will suffice.
		override(Object)			int						CanConvertToType(Class_ID obtype);

		// Developers have to make sure, that the channels, that the BaseObject implements 
		// (e.g. ExtensionChannels) are copied over to the new object as well. They can do this by simply
		// calling CopyXTCObjects(this,false); The validity will be automatically copied with it..
		override(Object)			Object*					ConvertToType(TimeValue t, Class_ID obtype);

		// Indicate the types this object can collapse to
		override(Object)			void					GetCollapseTypes(Tab<Class_ID> &clist,Tab<TSTR*> &nlist);
		
		override(BaseObject)		CreateMouseCallBack*	GetCreateMouseCallBack();

		// 'create' is TRUE if parameters are being edited in the create branch.
		// 'removeUI' is TRUE if the object's UI should be removed.
		override(Animatable)		void					BeginEditParams( IObjParam  *ip, ULONG flags,Animatable *prev);
		override(Animatable)		void					EndEditParams( IObjParam *ip, ULONG flags,Animatable *next);

		override(ReferenceTarget)	RefTargetHandle			Clone(RemapDir& remap = NoRemap());
		// This is the name that will appear in the history browser.
		override(BaseObject)		TCHAR*					GetObjectName()		{ return "IceSphere"; }		// Default object name in "Name and color" (user can change it)
		// Find out if the Object or Modifer is is generating UVW's
		// on map channel 1.
		override(BaseObject)		BOOL					HasUVW();
		// Change the state of the object's Generate UVW boolean.
		// IFF the state changes, the object should send a REFMSG_CHANGED down the pipe.
		override(BaseObject)		void					SetGenUVW(BOOL sw);
		// Access a parametric position on the surface of the object
		override(Object)			BOOL					IsParamSurface() {return TRUE;}
		// Single-surface version (surface 0)
		override(Object)			Point3					GetSurfacePoint(TimeValue t, float u, float v,Interval &iv);

		// From GeomObject

		// This used to be in GeomObject but I realized that other types of objects may
		// want this (mainly to participate in normal align) such as grid helper objects.
		override(Object)			int						IntersectRay(TimeValue t, ::Ray& ray, float& at, Point3& norm);
		
		// Animatable methods		

		// This is how things are actually deleted. 
		// Since they are created with the ClassDesc::Create()  function, and 
		// deleted via this function, they can use a different memory allocator
		// than the core code. (theoretically)
		override(Animatable)		void					DeleteThis()	{ delete this;	}
//									Class_ID				ClassID()		{ return Class_ID(SPHERE_CLASS_ID,0); } 
		override(Animatable)		Class_ID				ClassID()		{ return Class_ID(0x42424242,0); } 
		
		// From ReferenceTarget
		override(ReferenceMaker)	IOResult				Load(ILoad *iload);
		override(ReferenceMaker)	IOResult				Save(ISave *isave);
				
		// From IParamArray
		override(IParamArray)		BOOL					SetValue(int i, TimeValue t, int v);
		override(IParamArray)		BOOL					SetValue(int i, TimeValue t, float v);
		override(IParamArray)		BOOL					SetValue(int i, TimeValue t, Point3 &v);
		override(IParamArray)		BOOL					GetValue(int i, TimeValue t, int &v, Interval &ivalid);
		override(IParamArray)		BOOL					GetValue(int i, TimeValue t, float &v, Interval &ivalid);
		override(IParamArray)		BOOL					GetValue(int i, TimeValue t, Point3 &v, Interval &ivalid);

		// From SimpleObject
		override(SimpleObject)		void					BuildMesh(TimeValue t);
		override(SimpleObject)		BOOL					OKtoDisplay(TimeValue t);
		override(SimpleObject)		void					InvalidateUI();
		override(SimpleObject)		ParamDimension*			GetParameterDim(int pbIndex);
		override(SimpleObject)		TSTR					GetParameterName(int pbIndex);		

		// From GenSphere
		override(GenSphere)			void					SetParams(float rad, int segs, BOOL smooth=TRUE, BOOL genUV=TRUE, float hemi=0.0f, BOOL squash=FALSE, BOOL recenter=FALSE);
									void					PreSaveOld(); 
									void					PostSaveOld();
		// Class vars
		static						IParamMap*				pmapCreate;
		static						IParamMap*				pmapTypeIn;
		static						IParamMap*				pmapParam;
		static						int						dlgSegments;
		static						int						dlgCreateMeth;
		static						int						dlgSmooth,dlgSlice;
		static						Point3					crtPos;		
		static						float					crtRadius,crtSliceFrom,crtSliceTo;
		static						IObjParam*				ip;
									IParamBlock*			temppb;
		// mjm - 3.19.99 - ensure accurate matIDs and smoothing groups
									int						lastSquash;
									BOOL					lastNoHemi;
	};

// Misc stuff
#define MAX_SEGMENTS	200
#define MIN_SEGMENTS	4

#define MIN_RADIUS		float(0)
#define MAX_RADIUS		float(1.0E30)

#define MIN_SMOOTH		0
#define MAX_SMOOTH		1

#define DEF_SEGMENTS	32	// 16
#define DEF_RADIUS		float(0.0)

#define SMOOTH_ON	1
#define SMOOTH_OFF	0

#define MIN_SLICE	float(-1.0E30)
#define MAX_SLICE	float( 1.0E30)


//--- ClassDescriptor and class vars ---------------------------------

// The class descriptor for sphere
class SphereClassDesc:public ClassDesc
{
	public:
	int 			IsPublic()		{ return 1; }
	void *			Create(BOOL loading = FALSE) { return new SphereObject; }
	const TCHAR *	ClassName()		{ return "ICE Sphere"; }		// "Object Type"
	SClass_ID		SuperClassID()	{ return GEOMOBJECT_CLASS_ID; }
//	Class_ID		ClassID()		{ return Class_ID(SPHERE_CLASS_ID,0); }
	Class_ID		ClassID()		{ return Class_ID(0x42424242,0); }
//	const TCHAR* 	Category()		{ return GetString(IDS_RB_PRIMITIVES); }
	const TCHAR* 	Category()		{ return "ICE objects"; }
//	const TCHAR* 	Category()		{ return "Standard Primitives"; }
	void			ResetClassParams(BOOL fileReset);
};

static SphereClassDesc sphereDesc;
ClassDesc* GetSphereDesc() { return &sphereDesc; }

// in prim.cpp  - The dll instance handle
//extern HINSTANCE hInstance;

#define hInstance	gExporterPanel.hInstance

// Static members
int			SphereObject::dlgSegments	= DEF_SEGMENTS;
int			SphereObject::dlgCreateMeth	= 1; // create_radius
int			SphereObject::dlgSmooth		= SMOOTH_ON;
IParamMap*	SphereObject::pmapCreate	= NULL;
IParamMap*	SphereObject::pmapParam		= NULL;
IParamMap*	SphereObject::pmapTypeIn	= NULL;
IObjParam*	SphereObject::ip			= NULL;
Point3		SphereObject::crtPos		= Point3(0,0,0);
float		SphereObject::crtRadius		= 0.0f;
float		SphereObject::crtSliceFrom	= 0.0f;
float		SphereObject::crtSliceTo	= 0.0f;
int			SphereObject::dlgSlice		= 0;

void SphereClassDesc::ResetClassParams(BOOL fileReset)
{
	SphereObject::dlgSegments    = DEF_SEGMENTS;
	SphereObject::dlgCreateMeth  = 1; // create_radius
	SphereObject::dlgSmooth      = SMOOTH_ON;
	SphereObject::crtPos         = Point3(0,0,0);
	SphereObject::crtRadius      = 0.0f;
	SphereObject::dlgSlice          = 0;
	SphereObject::crtSliceFrom    = 0.0f;
	SphereObject::crtSliceTo    = 0.0f;
}

//--- Parameter map/block descriptors -------------------------------

// Parameter block indices
#define PB_RADIUS	0
#define PB_SEGS		1
#define PB_SMOOTH	2
#define PB_HEMI		3
#define PB_SQUASH	4
#define PB_RECENTER	5
#define PB_GENUVS	6
#define PB_SLICEON		7
#define PB_SLICEFROM	8
#define PB_SLICETO		9

// Non-parameter block indices
#define PB_CREATEMETHOD		0
#define PB_TI_POS			1
#define PB_TI_RADIUS		2

//
//
//	Creation method

static int createMethIDs[] = {IDC_CREATEDIAMETER,IDC_CREATERADIUS};

static ParamUIDesc descCreate[] = {
	// Diameter/radius
	ParamUIDesc(PB_CREATEMETHOD,TYPE_RADIO,createMethIDs,2)
	};
#define CREATEDESC_LENGH 1

//
//
// Type in

static ParamUIDesc descTypeIn[] = {
	
	// Position
	ParamUIDesc(
		PB_TI_POS,
		EDITTYPE_UNIVERSE,
		IDC_TI_POSX,IDC_TI_POSXSPIN,
		IDC_TI_POSY,IDC_TI_POSYSPIN,
		IDC_TI_POSZ,IDC_TI_POSZSPIN,
		-99999999.0f,99999999.0f,
		SPIN_AUTOSCALE),
	
	// Radius
	ParamUIDesc(
		PB_TI_RADIUS,
		EDITTYPE_UNIVERSE,
		IDC_RADIUS,IDC_RADSPINNER,
		MIN_RADIUS,MAX_RADIUS,
		SPIN_AUTOSCALE)	
	};
#define TYPEINDESC_LENGH 2


//
//
// Parameters


static int squashIDs[] = {IDC_HEMI_CHOP,IDC_HEMI_SQUASH};

static ParamUIDesc descParam[] = {
	// Radius
	ParamUIDesc(
		PB_RADIUS,
		EDITTYPE_UNIVERSE,
		IDC_RADIUS,IDC_RADSPINNER,
		MIN_RADIUS,MAX_RADIUS,
		SPIN_AUTOSCALE),	
	
	// Segments
	ParamUIDesc(
		PB_SEGS,
		EDITTYPE_INT,
		IDC_SEGMENTS,IDC_SEGSPINNER,
		(float)MIN_SEGMENTS,(float)MAX_SEGMENTS,
		0.1f),
	
	// Smooth
	ParamUIDesc(PB_SMOOTH,TYPE_SINGLECHEKBOX,IDC_OBSMOOTH),

	// Hemisphere
	ParamUIDesc(
		PB_HEMI,
		EDITTYPE_FLOAT,
		IDC_HEMISPHERE,IDC_HEMISPHERESPINNER,
		0.0f,1.0f,
		0.005f),

	// Chop/squash
	ParamUIDesc(PB_SQUASH,TYPE_RADIO,squashIDs,2),

	// Gen Slice
	ParamUIDesc(PB_SLICEON,TYPE_SINGLECHEKBOX,IDC_SC_SLICEON),			

	// Pie slice from
	ParamUIDesc(
		PB_SLICEFROM,
		EDITTYPE_FLOAT,
		IDC_SC_SLICE1,IDC_SC_SLICE1SPIN,
		MIN_SLICE,MAX_SLICE,		
		0.5f,
		stdAngleDim),

	// Pie slice to
	ParamUIDesc(
		PB_SLICETO,
		EDITTYPE_FLOAT,
		IDC_SC_SLICE2,IDC_SC_SLICE2SPIN,
		MIN_SLICE,MAX_SLICE,		
		0.5f,
		stdAngleDim),

	// Recenter
	ParamUIDesc(PB_RECENTER,TYPE_SINGLECHEKBOX,IDC_HEMI_RECENTER),

	// Gen UVs
	ParamUIDesc(PB_GENUVS,TYPE_SINGLECHEKBOX,IDC_GENTEXTURE),
	};
#define PARAMDESC_LENGH 10


static ParamBlockDescID descVer0[] = {
	{ TYPE_FLOAT, NULL, TRUE, 0 },		
	{ TYPE_INT, NULL, TRUE, 1 },
	{ TYPE_INT, NULL, TRUE, 2 } };

static ParamBlockDescID descVer1[] = {
	{ TYPE_FLOAT, NULL, TRUE, 0 },	
	{ TYPE_INT, NULL, TRUE, 1 },
	{ TYPE_INT, NULL, TRUE, 2 },
	{ TYPE_FLOAT, NULL, TRUE, 3 },
	{ TYPE_INT, NULL, FALSE, 4 },
	{ TYPE_INT, NULL, FALSE, 5 } };

static ParamBlockDescID descVer2[] = {
	{ TYPE_FLOAT, NULL, TRUE, 0 },	
	{ TYPE_INT, NULL, TRUE, 1 },
	{ TYPE_BOOL, NULL, TRUE, 2 },
	{ TYPE_FLOAT, NULL, TRUE, 3 },
	{ TYPE_INT, NULL, FALSE, 4 },
	{ TYPE_INT, NULL, FALSE, 5 },
	{ TYPE_INT, NULL, FALSE, 6 } };

static ParamBlockDescID descVer3[] = {
	{ TYPE_FLOAT, NULL, TRUE, 0 },	
	{ TYPE_INT, NULL, TRUE, 1 },
	{ TYPE_BOOL, NULL, TRUE, 2 },
	{ TYPE_FLOAT, NULL, TRUE, 3 },
	{ TYPE_INT, NULL, FALSE, 4 },
	{ TYPE_INT, NULL, FALSE, 5 },
	{ TYPE_INT, NULL, FALSE, 6 },
	{ TYPE_INT, NULL, FALSE, 7 },
	{ TYPE_FLOAT, NULL, TRUE, 8 },
	{ TYPE_FLOAT, NULL, TRUE, 9 },
};

#define PBLOCK_LENGTH	10

// Array of old versions
static ParamVersionDesc versions[] = {
	ParamVersionDesc(descVer0,3,0),
	ParamVersionDesc(descVer1,6,1),
	ParamVersionDesc(descVer2,7,2)
	};
#define NUM_OLDVERSIONS	3

// Current version
#define CURRENT_VERSION	3
static ParamVersionDesc curVersion(descVer3,PBLOCK_LENGTH,CURRENT_VERSION);

//--- TypeInDlgProc --------------------------------

class SphereTypeInDlgProc : public ParamMapUserDlgProc
{
	public:
		SphereObject *so;

		SphereTypeInDlgProc(SphereObject *s) {so=s;}
		BOOL DlgProc(TimeValue t,IParamMap *map,HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
		void DeleteThis() {delete this;}
};

BOOL SphereTypeInDlgProc::DlgProc(TimeValue t,IParamMap *map,HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_TI_CREATE:
				{
					if (so->crtRadius==0.0) return TRUE;
					
					// We only want to set the value if the object is 
					// not in the scene.
					if (so->TestAFlag(A_OBJ_CREATING))
					{
						so->pblock->SetValue(PB_RADIUS,0,so->crtRadius);
					}

					Matrix3 tm(1);
					tm.SetTrans(so->crtPos);
					so->suspendSnap = FALSE;
					so->ip->NonMouseCreate(tm);					
					// NOTE that calling NonMouseCreate will cause this
					// object to be deleted. DO NOT DO ANYTHING BUT RETURN.
					return TRUE;	
				}
			}
			break;	
	}
	return FALSE;
}

//--- ParamDlgProc --------------------------------
void TurnSpinner(HWND hWnd,int SpinNum,BOOL ison)
{
	ISpinnerControl *spin2 = GetISpinner(::GetDlgItem(hWnd,SpinNum));
	if (ison) spin2->Enable();else spin2->Disable();
	ReleaseISpinner(spin2);

};

class SphereParamDlgProc : public ParamMapUserDlgProc
{
	public:
		SphereObject *so;
		HWND thishWnd;

		SphereParamDlgProc(SphereObject *s) {so=s;thishWnd=NULL;}
		BOOL DlgProc(TimeValue t,IParamMap *map,HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
		void Update(TimeValue t);
		void DeleteThis() {delete this;}
};

void SphereParamDlgProc::Update(TimeValue t)
{
	if (!thishWnd) return;
	int ison;
	so->pblock->GetValue(PB_SLICEON,0,ison,FOREVER);
	TurnSpinner(thishWnd,IDC_SC_SLICE1SPIN,ison);
	TurnSpinner(thishWnd,IDC_SC_SLICE2SPIN,ison);
}

BOOL SphereParamDlgProc::DlgProc(TimeValue t,IParamMap *map,HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	thishWnd=hWnd;
	switch (msg)
	{
		case WM_INITDIALOG:
			Update(t);
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_SC_SLICEON:
				{
					int ison;
					so->pblock->GetValue(PB_SLICEON,0,ison,FOREVER);
					TurnSpinner(hWnd,IDC_SC_SLICE1SPIN,ison);
					TurnSpinner(hWnd,IDC_SC_SLICE2SPIN,ison);
					return TRUE;	
				}
			}
			break;	
	}
	return FALSE;
}



static void NotifyPreSaveOld(void *param, NotifyInfo *info)
{
	SphereObject *mt = (SphereObject *)param;
	mt->PreSaveOld();
}

static void NotifyPostSaveOld(void *param, NotifyInfo *info)
{
	SphereObject *mt = (SphereObject *)param;
	mt->PostSaveOld();
}

//--- Sphere methods -------------------------------

SphereObject::SphereObject() : lastSquash(-1), lastNoHemi(FALSE)
{
	SetAFlag(A_PLUGIN1);
	MakeRefByID(FOREVER, 0, CreateParameterBlock(descVer3, PBLOCK_LENGTH, CURRENT_VERSION));
	assert(pblock);
	temppb = NULL;
	
	pblock->SetValue(PB_RADIUS,0,crtRadius);
	pblock->SetValue(PB_SMOOTH,0,dlgSmooth);
	pblock->SetValue(PB_SEGS,0,dlgSegments);	
	pblock->SetValue(PB_SQUASH,0,0);
	pblock->SetValue(PB_SLICEON,0,dlgSlice);
	pblock->SetValue(PB_SLICEFROM,0,crtSliceFrom);
	pblock->SetValue(PB_SLICETO,0,crtSliceTo);
	RegisterNotification(NotifyPreSaveOld, (void *)this, NOTIFY_FILE_PRE_SAVE_OLD);
	RegisterNotification(NotifyPostSaveOld, (void *)this, NOTIFY_FILE_POST_SAVE_OLD);
}

SphereObject::~SphereObject()
{
	UnRegisterNotification(NotifyPreSaveOld, (void *)this, NOTIFY_FILE_PRE_SAVE_OLD);
	UnRegisterNotification(NotifyPostSaveOld, (void *)this, NOTIFY_FILE_POST_SAVE_OLD);
}

void SphereObject::PreSaveOld()
{
	if(GetSavingVersion()==2000)
	{
		temppb = pblock;
		pblock =  UpdateParameterBlock(descVer3,PBLOCK_LENGTH,temppb, descVer2,7,2);
	}
}

void SphereObject::PostSaveOld()
{
	if (temppb)
	{
		pblock->DeleteThis();
		pblock = temppb;
		temppb = NULL;
	}
}


#define NEWMAP_CHUNKID	0x0100

IOResult SphereObject::Load(ILoad *iload) 
{
	ClearAFlag(A_PLUGIN1);

	IOResult res;
	while (IO_OK==(res=iload->OpenChunk()))
	{
		switch (iload->CurChunkID())
		{
			case NEWMAP_CHUNKID:
				SetAFlag(A_PLUGIN1);
				break;
		}
		iload->CloseChunk();
		if (res!=IO_OK)  return res;
	}

	iload->RegisterPostLoadCallback(new ParamBlockPLCB(versions,NUM_OLDVERSIONS,&curVersion,this,0));

	return IO_OK;
}

IOResult SphereObject::Save(ISave *isave)
{
	if (TestAFlag(A_PLUGIN1)) {
		isave->BeginChunk(NEWMAP_CHUNKID);
		isave->EndChunk();
		}
 	return IO_OK;
}

void SphereObject::BeginEditParams(IObjParam *ip,ULONG flags,Animatable *prev)
{
	SimpleObject::BeginEditParams(ip,flags,prev);
	this->ip = ip;

	if (pmapCreate && pmapParam)
	{
		// Left over from last sphere ceated
		pmapCreate->SetParamBlock(this);
		pmapTypeIn->SetParamBlock(this);
		pmapParam->SetParamBlock(pblock);
	}
	else
	{
		// Gotta make a new one.
		if (flags&BEGIN_EDIT_CREATE) {
			pmapCreate = CreateCPParamMap(
				descCreate,CREATEDESC_LENGH,
				this,
				ip,
				hInstance,
//				MAKEINTRESOURCE(IDD_SPHEREPARAM1),
				MAKEINTRESOURCE(IDD_SPHERE_DIALOG0),
//				GetString(IDS_RB_CREATIONMETHOD),
				"Creation method",
				0);

			pmapTypeIn = CreateCPParamMap(
				descTypeIn,TYPEINDESC_LENGH,
				this,
				ip,
				hInstance,
//				MAKEINTRESOURCE(IDD_SPHEREPARAM3),
				MAKEINTRESOURCE(IDD_SPHERE_DIALOG1),
//				GetString(IDS_RB_KEYBOARDENTRY),
				"Keyboard entry",
				APPENDROLL_CLOSED);
			}

		pmapParam = CreateCPParamMap(
			descParam,PARAMDESC_LENGH,
			pblock,
			ip,
			hInstance,
//			MAKEINTRESOURCE(IDD_SPHEREPARAM2),
			MAKEINTRESOURCE(IDD_SPHERE_DIALOG2),
//			GetString(IDS_RB_PARAMETERS),
			"Sphere parameters",
			0);
	}

	if(pmapTypeIn)
	{
		// A callback for the type in.
		pmapTypeIn->SetUserDlgProc(new SphereTypeInDlgProc(this));
	}
	if(pmapParam)
	{
		// A callback for the type in.
		pmapParam->SetUserDlgProc(new SphereParamDlgProc(this));
	}
}
		
void SphereObject::EndEditParams( IObjParam *ip, ULONG flags,Animatable *next )
{		
	SimpleObject::EndEditParams(ip,flags,next);
	this->ip = NULL;

	if (flags&END_EDIT_REMOVEUI )
	{
		if (pmapCreate) DestroyCPParamMap(pmapCreate);
		if (pmapTypeIn) DestroyCPParamMap(pmapTypeIn);
		DestroyCPParamMap(pmapParam);
		pmapParam  = NULL;
		pmapTypeIn = NULL;
		pmapCreate = NULL;
	}

	// Save these values in class variables so the next object created will inherit them.
	pblock->GetValue(PB_SEGS,ip->GetTime(),dlgSegments,FOREVER);
	pblock->GetValue(PB_SMOOTH,ip->GetTime(),dlgSmooth,FOREVER);	
}

Point3 SphereObject::GetSurfacePoint(TimeValue t, float u, float v,Interval &iv)
{
	float rad;
	pblock->GetValue(PB_RADIUS, t, rad, iv);
	Point3 pos;	
	v -= 0.5f;
	float ar = (float)cos(v*PI);
	pos.x = rad * float(cos(u*TWOPI)) * ar;
	pos.y = rad * float(sin(u*TWOPI)) * ar;
	pos.z = rad * float(sin(v*PI));
	return pos;
}

void SphereObject::SetParams(float rad, int segs, BOOL smooth, BOOL genUV, float hemi, BOOL squash, BOOL recenter)
{
	pblock->SetValue(PB_RADIUS,0, rad);				
	pblock->SetValue(PB_HEMI,0, hemi);				
	pblock->SetValue(PB_SEGS,0, segs);				
	pblock->SetValue(PB_SQUASH,0, squash);				
	pblock->SetValue(PB_SMOOTH,0, smooth);				
	pblock->SetValue(PB_RECENTER,0, recenter);				
	pblock->SetValue(PB_GENUVS,0, genUV);				
}			   

BOOL SphereObject::HasUVW()
{
	BOOL genUVs;
	Interval v;
	pblock->GetValue(PB_GENUVS, 0, genUVs, v);
	return genUVs; 
}

void SphereObject::SetGenUVW(BOOL sw)
{
	if (sw==HasUVW()) return;
	pblock->SetValue(PB_GENUVS,0, sw);				
}

// This method is called to build the mesh representation of the object using its parameter settings at the time passed.
// The plug-in should use the data member mesh to store the built mesh.
float uval[3]={1.0f,0.0f,1.0f};
void SphereObject::BuildMesh(TimeValue t)
{
	Point3 p;	
	int ix,na,nb,nc,nd,jx,kx;
	int nf=0,nv=0;
	float delta, delta2;
	float a,alt,secrad,secang,b,c;
	int segs, smooth;
	float radius;
	float hemi;
	BOOL noHemi = FALSE;	
	int squash;
	int recenter;
	BOOL genUVs = TRUE;
	float startAng = 0.0f;
	float pie1,pie2;int doPie;
	if (TestAFlag(A_PLUGIN1)) startAng = HALFPI;

	// Start the validity interval at forever and widdle it down.
	ivalid = FOREVER;
	pblock->GetValue(PB_RADIUS, t, radius, ivalid);
	pblock->GetValue(PB_SEGS, t, segs, ivalid);
	pblock->GetValue(PB_SMOOTH, t, smooth, ivalid);
	pblock->GetValue(PB_HEMI, t, hemi, ivalid);
	pblock->GetValue(PB_SQUASH, t, squash, ivalid);
	pblock->GetValue(PB_RECENTER, t, recenter, ivalid);
	pblock->GetValue(PB_GENUVS, t, genUVs, ivalid);
	pblock->GetValue(PB_SLICEFROM,t,pie1,ivalid);
	pblock->GetValue(PB_SLICETO,t,pie2,ivalid);	
	pblock->GetValue(PB_SLICEON,t,doPie,ivalid);	
	LimitValue(segs, MIN_SEGMENTS, MAX_SEGMENTS);
	LimitValue(smooth, MIN_SMOOTH, MAX_SMOOTH);
	LimitValue(radius, MIN_RADIUS, MAX_RADIUS);
	LimitValue(hemi, 0.0f, 1.0f);

	float totalPie(0.0f);
	if (doPie) doPie = 1;
	else doPie = 0;
	if (doPie)
	{ pie2+=startAng;pie1+=startAng;
	  while (pie1 < pie2) pie1 += TWOPI;
	  while (pie1 > pie2+TWOPI) pie1 -= TWOPI;
	  if (pie1==pie2) totalPie = TWOPI;
	  else totalPie = pie1-pie2;	
	}

	if (hemi<0.00001f) noHemi = TRUE;
	if (hemi>=1.0f) hemi = 0.9999f;
	hemi = (1.0f-hemi) * PI;
	float basedelta=2.0f*PI/(float)segs;
	delta2=(doPie?totalPie/(float)segs:basedelta);
	if (!noHemi && squash) {
		delta  = 2.0f*hemi/float(segs-2);
	} else {
		delta  = basedelta;
		}

	int rows;
	if (noHemi || squash) {
		rows = (segs/2-1);
	} else {
		rows = int(hemi/delta) + 1;
		}
	int realsegs=(doPie?segs+2:segs);
	int nverts = rows * realsegs + 2;
	int nfaces = rows * realsegs * 2;
	if (doPie) 
	{ startAng=pie2;segs+=1;
	  if (!noHemi) {nfaces-=2;nverts-=1;}
	}
	mesh.setNumVerts(nverts);
	mesh.setNumFaces(nfaces);
	mesh.setSmoothFlags(smooth != 0);
	int lastvert=nverts-1;

	// mjm - 3.19.99 - ensure accurate matIDs and smoothing groups
	if (lastSquash != squash || lastNoHemi != noHemi)
	{
		lastSquash = squash;
		lastNoHemi = noHemi;
		mesh.InvalidateStrips();
	}

	// Top vertex 
	mesh.setVert(nv, 0.0f, 0.0f, radius);
	nv++;

	// Middle vertices 
	alt=delta;
	for(ix=1; ix<=rows; ix++) {		
		if (!noHemi && ix==rows) alt = hemi;
		a = (float)cos(alt)*radius;		
		secrad = (float)sin(alt)*radius;
		secang = startAng; //0.0f
		for(jx=0; jx<segs; ++jx) {
			b = (float)cos(secang)*secrad;
			c = (float)sin(secang)*secrad;
			mesh.setVert(nv++,b,c,a);
			secang+=delta2;
			}
		if (doPie &&(noHemi ||(ix<rows))) mesh.setVert(nv++,0.0f,0.0f,a);
		alt+=delta;		
		}

	/* Bottom vertex */
	if (noHemi) {
		mesh.setVert(nv++, 0.0f, 0.0f,-radius);
		}
	else {
		a = (float)cos(hemi)*radius;
		mesh.setVert(nv++, 0.0f, 0.0f, a);
		}

	BOOL issliceface;
	// Now make faces 
	if (doPie) segs++;

	// Make top conic cap
	for(ix=1; ix<=segs; ++ix) {
		issliceface=(doPie && (ix>=segs-1));
		nc=(ix==segs)?1:ix+1;
		mesh.faces[nf].setEdgeVisFlags(1,1,1);
		if ((issliceface)&&(ix==segs-1))
		{	mesh.faces[nf].setSmGroup(smooth?4:0);
			mesh.faces[nf].setMatID(2);
		}
		else if ((issliceface)&&(ix==segs))
		{	mesh.faces[nf].setSmGroup(smooth?8:0);
			mesh.faces[nf].setMatID(3);
		}
		else
		{	mesh.faces[nf].setSmGroup(smooth?1:0);
			mesh.faces[nf].setMatID(1); // mjm - 5.5.99 - rollback change - should be fixed in later release
//			mesh.faces[nf].setMatID(0); // mjm - 3.2.99 - was set to 1
		}
		mesh.faces[nf].setVerts(0, ix, nc);
		nf++;
		}

	/* Make midsection */
	int lastrow=rows-1,lastseg=segs-1,almostlast=lastseg-1;
	BOOL weirdpt=doPie && !noHemi,weirdmid=weirdpt && (rows==2);
	for(ix=1; ix<rows; ++ix) {
		jx=(ix-1)*segs+1;
		for(kx=0; kx<segs; ++kx) {
			issliceface=(doPie && (kx>=almostlast));

			na = jx+kx;
			nb = na+segs;
			nb = (weirdmid &&(kx==lastseg)? lastvert:na+segs);
			if ((weirdmid) &&(kx==almostlast)) nc=lastvert; else
			nc = (kx==lastseg)? jx+segs: nb+1;
			nd = (kx==lastseg)? jx : na+1;
			
			mesh.faces[nf].setEdgeVisFlags(1,1,0);

			if ((issliceface)&&((kx==almostlast-2)||(kx==almostlast)))
			{	mesh.faces[nf].setSmGroup(smooth?4:0);
				mesh.faces[nf].setMatID(2);
			}
			else if((issliceface)&&((kx==almostlast-1)||(kx==almostlast+1)))
			{	mesh.faces[nf].setSmGroup(smooth?8:0);
				mesh.faces[nf].setMatID(3);
			}
			else
			{	mesh.faces[nf].setSmGroup(smooth?1:0);
				mesh.faces[nf].setMatID(1); // mjm - 5.5.99 - rollback change - should be fixed in later release
//				mesh.faces[nf].setMatID(0); // mjm - 3.2.99 - was set to 1
			}

			mesh.faces[nf].setVerts(na,nb,nc);
			nf++;

			mesh.faces[nf].setEdgeVisFlags(0,1,1);

			if ((issliceface)&&((kx==almostlast-2)||(kx==almostlast)))
			{	mesh.faces[nf].setSmGroup(smooth?4:0);
				mesh.faces[nf].setMatID(2);
			}
			else if((issliceface)&&((kx==almostlast-1)||(kx==almostlast+1)))
			{	mesh.faces[nf].setSmGroup(smooth?8:0);
				mesh.faces[nf].setMatID(3);
			}
			else
			{	mesh.faces[nf].setSmGroup(smooth?1:0);
				mesh.faces[nf].setMatID(1); // mjm - 5.5.99 - rollback change - should be fixed in later release
//				mesh.faces[nf].setMatID(0); // mjm - 3.2.99 - was set to 1
			}

			mesh.faces[nf].setVerts(na,nc,nd);
			nf++;
			}
	 	}

	// Make bottom conic cap
	na = mesh.getNumVerts()-1;
	int botsegs=(weirdpt?segs-2:segs);
	jx = (rows-1)*segs+1;lastseg=botsegs-1;
	for(ix=0; ix<botsegs; ++ix) {
		issliceface=(doPie && (ix>=botsegs-2));
		nc = ix + jx;
		nb = (!weirdpt && (ix==lastseg)?jx:nc+1);
		mesh.faces[nf].setEdgeVisFlags(1,1,1);

		if ((issliceface)&&(noHemi)&&(ix==botsegs-2))
		{	mesh.faces[nf].setSmGroup(smooth?4:0);
			mesh.faces[nf].setMatID(2);
		}
		else if ((issliceface)&&(noHemi)&&(ix==botsegs-1))
		{	mesh.faces[nf].setSmGroup(smooth?8:0);
			mesh.faces[nf].setMatID(3);
		}
		else if ((!issliceface)&&(noHemi))
		{	mesh.faces[nf].setSmGroup(smooth?1:0);
			mesh.faces[nf].setMatID(1); // mjm - 5.5.99 - rollback change - should be fixed in later release
//			mesh.faces[nf].setMatID(0); // mjm - 3.2.99 - was set to 1
		}
		else if (!noHemi)
		{	mesh.faces[nf].setSmGroup(smooth?2:0);
			mesh.faces[nf].setMatID(0); // mjm - 5.5.99 - rollback change - should be fixed in later release
//			mesh.faces[nf].setMatID(1); // mjm - 3.2.99 - was set to 0
		}
//		else
//		{	mesh.faces[nf].setSmGroup(0);
//			mesh.faces[nf].setMatID(noHemi?1:0); // mjm - 5.5.99 - rollback change - should be fixed in later release
//			mesh.faces[nf].setMatID(noHemi?0:1); // mjm - 3.2.99 - was commented out but set to 1:0
//		}

		mesh.faces[nf].setVerts(na, nb, nc);

		nf++;
		}

	// Put the flat part of the hemisphere at z=0
	if (recenter) {
		float shift = (float)cos(hemi) * radius;
		for (ix=0; ix<mesh.getNumVerts(); ix++) {
			mesh.verts[ix].z -= shift;
			}
		}

	if (genUVs) {
		int tvsegs=segs;
		int tvpts=(doPie?segs+1:segs); 
		int ntverts = (rows+2)*(tvpts+1);
//		if (doPie) {ntverts-=6; if (weirdpt) ntverts-3;}
		mesh.setNumTVerts(ntverts);
		mesh.setNumTVFaces(nfaces);
		nv = 0;
		delta  = basedelta;  // make the texture squash too
		alt = 0.0f; // = delta;
		int dsegs=(doPie?3:0),midsegs=tvpts-dsegs,m1=midsegs+1,t1=tvpts+1;
		for(ix=0; ix < rows+2; ix++) {		
		//	if (!noHemi && ix==rows) alt = hemi;		
			secang = 0.0f; //angle;
			float yang=1.0f-alt/PI;
			for(jx=0; jx <= midsegs; ++jx) {
				mesh.setTVert(nv++, secang/TWOPI, yang, 0.0f);
				secang += delta2;
				}
			for (jx=0;jx<dsegs;jx++) mesh.setTVert(nv++,uval[jx],yang,0.0f);
			alt += delta;		
			}

		nf = 0;dsegs=(doPie?2:0),midsegs=segs-dsegs;
		// Make top conic cap
		for(ix=0; ix<midsegs; ++ix) {
			mesh.tvFace[nf++].setTVerts(ix,ix+t1,ix+t1+1);
		} ix=midsegs+1;int topv=ix+1;
		for (jx=0;jx<dsegs;jx++) 
		{ mesh.tvFace[nf++].setTVerts(topv,ix+t1,ix+t1+1);ix++;
		}
		int cpt;
		/* Make midsection */
		for(ix=1; ix<rows; ++ix) {
			cpt=ix*t1;
			for(kx=0; kx<tvsegs; ++kx) {
				if (kx==midsegs) cpt++;
				na = cpt+kx;
				nb = na+t1;
				nc = nb+1;
				nd = na+1;
				assert(nc<ntverts);
				assert(nd<ntverts);
				mesh.tvFace[nf++].setTVerts(na,nb,nc);
				mesh.tvFace[nf++].setTVerts(na,nc,nd);
				}
			}
		// Make bottom conic cap
		int lastv=rows*t1,jx=lastv+t1;
		if (weirdpt) dsegs=0;
		for (int j1=lastv; j1<lastv+midsegs; j1++) {
			mesh.tvFace[nf++].setTVerts(jx,j1+1,j1);jx++;
			}j1=lastv+midsegs+1;topv=j1+t1+1;
		for (ix=0;ix<dsegs;ix++) 
		{ mesh.tvFace[nf++].setTVerts(topv,j1+1,j1);j1++;
		}
		assert(nf==nfaces);
		}
	else {
		mesh.setNumTVerts(0);
		mesh.setNumTVFaces(0);
		}

	mesh.InvalidateGeomCache();
	mesh.BuildStripsAndEdges();
}

// Indicates whether the object can be converted to the specified type.
// If the object returns nonzero to indicate it can be converted to the specified type,
// it must handle converting to and returning an object of that type from ConvertToType().
// in: The Class_ID of the type of object to convert to.
// return: Nonzero if the object can be converted to the specified type; otherwise 0.
int SphereObject::CanConvertToType(Class_ID obtype)
{
	if(obtype==defObjectClassID ||	obtype==triObjectClassID) return 1;

	// Call the base method
	return SimpleObject::CanConvertToType(obtype);
}

Object* SphereObject::ConvertToType(TimeValue t, Class_ID obtype)
{
	// Call the base method
	return SimpleObject::ConvertToType(t,obtype);
}

void SphereObject::GetCollapseTypes(Tab<Class_ID> &clist,Tab<TSTR*> &nlist)
{
	// Call the base method
    Object::GetCollapseTypes(clist, nlist);
}

	class SphereObjCreateCallBack : public CreateMouseCallBack
	{
		IPoint2			sp0;
		SphereObject*	mCreatedObject;
		Point3			p0;
		public:
		int				proc(ViewExp* vpt, int msg, int point, int flags, IPoint2 m, Matrix3& mat);
		void			SetObj(SphereObject* obj) {mCreatedObject = obj;}
	};

//	MOUSE_ABORT		When the user aborts a mouse procedure, for example when they are dragging the mouse and they right click, this message is sent.
//	MOUSE_IDLE		This message is used internally.
//	MOUSE_POINT		This message is sent when the user has clicked a point.
//	MOUSE_MOVE		This message is sent when the mouse input is captured and the user moved the mouse. When  mouse input is captured all mouse events
//					continue to go to the current window even when the mouse is move outside the limits of the window. This is when the user is 'dragging'.
//	MOUSE_DBLCLICK	This is sent when the user has double clicked the mouse.
//	MOUSE_INIT		This is sent when the mouse proc is plugged in as the current mouse proc. If a plug-in needed to perform some kind of initialization
//					when it was first became current this message could be processed.
//	MOUSE_UNINIT	This is sent when the mouse proc is un-plugged as the current mouse proc.
//	MOUSE_FREEMOVE	This message is similar to a MOUSE_MOVE message except it is not called when the mouse is in a 'drag' session. This means that mouse
//					input is not captured. If mouse input is not captured and the mouse is moved outside the current window, the current window will no
//					longer receive the mouse messages.
//	MOUSE_KEYBOARD	This is not used. Keyboard input is processed by registering an accelerator table. See Class Interface for the methods to register
//					a keyboard accelerator. Also see the section Keyboard Accelerators and Dialog Messages.
//	MOUSE_PROPCLICK	This message is sent on a right click, when nothing is selected, and the user is not over any selectable object. For example,
//					this is how the unfreeze-by-hit pick mode knows to abort if the user presses the right mouse button. Note that this is different
//					than if you right click while dragging - in that case you get a MOUSE_ABORT message.

// This is the method where the developer defines the user / mouse interaction that takes place during the creation phase of an object.
// vpt : The viewport the creation process is taking place in.
// msg: This message describes the type of event that occurred. See List of Mouse Callback Messages.
// point: The point number. this is 0 for the first click, 1 for the second, etc.
// flags: These flags describe the state of the mouse button and keyboard Shift/Ctrl/Alt keys.  See List of Mouse Callback Flags.
// m:	The 2D screen point that the user clicked on. Methods in the viewport interface allow this point to be converted into a world space ray
//		or a 3D view space point. A world space ray can be intersected with the active construction plane which results in a point on the active
//		construction plane. See Class ViewExp.
// mat:	This represents the transformation of the object relative to the construction plane. Typically the plug-in would get a point on the
//		construction plane based on the screen point that the user clicked on and set the translation component of this matrix based on that point.
//
// Return one of the following value to indicate the state of the creation process:
//	CREATE_CONTINUE		The creation process should continue.  In this case the mouse is captured.
//	CREATE_STOP			The creation process has terminated normally.  In this case the mouse is no longer captured and input is then allowed again from any viewport.
//	CREATE_ABORT		The creation process has been aborted.  The system will delete the created object and node.
int SphereObjCreateCallBack::proc(ViewExp* vpt, int msg, int point, int flags, IPoint2 m, Matrix3& mat)
{
	float r;
	Point3 p1,center;

	#ifdef _OSNAP
	if (msg == MOUSE_FREEMOVE)
	{
		// Show a preview snap in the viewport prior to the first point event.
		#ifdef _3D_CREATE
			vpt->SnapPreview(m,m,NULL, SNAP_IN_3D);
		#else
			vpt->SnapPreview(m,m,NULL, SNAP_IN_PLANE);
		#endif
	}
	#endif

	// Sample code from SnapPreview in the docs
	if(msg==MOUSE_POINT||msg==MOUSE_MOVE)
	{
		switch(point)
		{
			case 0:  // only happens with MOUSE_POINT msg
				mCreatedObject->pblock->SetValue(PB_RADIUS,0,0.0f);
				mCreatedObject->suspendSnap = TRUE;				
				sp0 = m;
				#ifdef _3D_CREATE	
					p0 = vpt->SnapPoint(m,m,NULL,SNAP_IN_3D);
				#else	
					p0 = vpt->SnapPoint(m,m,NULL,SNAP_IN_PLANE);
				#endif
				mat.SetTrans(p0);
				break;
			case 1:
				mat.IdentityMatrix();
				//mat.PreRotateZ(HALFPI);
				#ifdef _3D_CREATE	
					p1 = vpt->SnapPoint(m,m,NULL,SNAP_IN_3D);
				#else	
					p1 = vpt->SnapPoint(m,m,NULL,SNAP_IN_PLANE);
				#endif
				if(mCreatedObject->dlgCreateMeth)
				{
					r = Length(p1-p0);
					mat.SetTrans(p0);
				}
				else
				{
					center = (p0+p1)/float(2);
					mat.SetTrans(center);
					r = Length(center-p0);
				} 
				mCreatedObject->pblock->SetValue(PB_RADIUS,0,r);
				mCreatedObject->pmapParam->Invalidate();	// update UI

				if(flags&MOUSE_CTRL)
				{
					float ang = atan2f(p1.y-p0.y,p1.x-p0.x);					
					mat.PreRotateZ(mCreatedObject->ip->SnapAngle(ang));
				}

				if(msg==MOUSE_POINT)
				{
					mCreatedObject->suspendSnap = FALSE;
					return (Length(m-sp0)<3 || Length(p1-p0)<0.1f)?CREATE_ABORT:CREATE_STOP;
				}
				break;					   
		}
	}
	else if(msg==MOUSE_ABORT)
	{
		return CREATE_ABORT;
	}
	return TRUE;
}

static SphereObjCreateCallBack sphereCreateCB;

// This method allows the system to retrieve a callback object used in creating an object in the 3D viewports.
// This method returns a pointer to an instance of a class derived from CreateMouseCallBack. This class has a method proc()
// which is where the programmer defines the user/mouse interaction during the object creation phase.
CreateMouseCallBack* SphereObject::GetCreateMouseCallBack()
{
	sphereCreateCB.SetObj(this);
	return &sphereCreateCB;
}

// This method returns a BOOL to indicate if it is okay to draw the object at the time passed.
// Normally it is always OK to draw the object, so the default implementation returns TRUE.
// However for certain objects it might be a degenerate case to draw the object at a certain time
// (perhaps the size went to zero for example), so these objects could return FALSE.
BOOL SphereObject::OKtoDisplay(TimeValue t)
{
	// If returns FALSE, only the bbox is drawn in the viewport.
	float Radius;
	pblock->GetValue(PB_RADIUS, t, Radius, FOREVER);
	if(Radius==0.0f) return FALSE;
	return TRUE;
}



// From ParamArray
BOOL SphereObject::SetValue(int i, TimeValue t, int v) 
{
	switch(i)
	{
		case PB_CREATEMETHOD: dlgCreateMeth = v; break;
	}
	return TRUE;
}

BOOL SphereObject::SetValue(int i, TimeValue t, float v)
{
	switch(i)
	{
		case PB_TI_RADIUS: crtRadius = v; break;
	}	
	return TRUE;
}

BOOL SphereObject::SetValue(int i, TimeValue t, Point3 &v)
{
	switch(i)
	{
		case PB_TI_POS: crtPos = v; break;
	}		
	return TRUE;
}

BOOL SphereObject::GetValue(int i, TimeValue t, int &v, Interval &ivalid) 
{
	switch(i)
	{
		case PB_CREATEMETHOD: v = dlgCreateMeth; break;
	}
	return TRUE;
}

BOOL SphereObject::GetValue(int i, TimeValue t, float &v, Interval &ivalid) 
{	
	switch(i)
	{
		case PB_TI_RADIUS: v = crtRadius; break;
	}
	return TRUE;
}

BOOL SphereObject::GetValue(int i, TimeValue t, Point3 &v, Interval &ivalid) 
{	
	switch(i)
	{
		case PB_TI_POS: v = crtPos; break;
	}
	return TRUE;
}


// From GeomObject
int SphereObject::IntersectRay(TimeValue t, ::Ray& ray, float& at, Point3& norm)
{
return SimpleObject::IntersectRay(t,ray,at,norm);
	int smooth, recenter;
	pblock->GetValue(PB_SMOOTH,t,smooth,FOREVER);
	pblock->GetValue(PB_RECENTER,t,recenter,FOREVER);	
	float hemi;
	pblock->GetValue(PB_HEMI,t,hemi,FOREVER);
	if (!smooth || hemi!=0.0f || recenter) {
		return SimpleObject::IntersectRay(t,ray,at,norm);
		}	
	
	float r;
	float a, b, c, ac4, b2, at1, at2;
	float root;
	BOOL neg1, neg2;

	pblock->GetValue(PB_RADIUS,t,r,FOREVER);

	a = DotProd(ray.dir,ray.dir);
	b = DotProd(ray.dir,ray.p) * 2.0f;
	c = DotProd(ray.p,ray.p) - r*r;
	
	ac4 = 4.0f * a * c;
	b2 = b*b;

	if (ac4 > b2) return 0;

	// We want the smallest positive root
	root = float(sqrt(b2-ac4));
	at1 = (-b + root) / (2.0f * a);
	at2 = (-b - root) / (2.0f * a);
	neg1 = at1<0.0f;
	neg2 = at2<0.0f;
	if (neg1 && neg2) return 0;
	else
	if (neg1 && !neg2) at = at2;
	else 
	if (!neg1 && neg2) at = at1;
	else
	if (at1<at2) at = at1;
	else at = at2;
	
	norm = Normalize(ray.p + at*ray.dir);

	return 1;
}

// This is called if the user interface parameters needs to be updated because the user moved to a new time.
// The UI controls must display values for the current time.
void SphereObject::InvalidateUI()
{
	if(pmapParam) pmapParam->Invalidate();
}

// This method returns the parameter dimension of the parameter whose index is passed.
ParamDimension *SphereObject::GetParameterDim(int pbIndex) 
{
	switch (pbIndex)
	{
		case PB_RADIUS:
			return stdWorldDim;			
		case PB_HEMI:
			return stdNormalizedDim;
		case PB_SEGS:
			return stdSegmentsDim;			
		case PB_SMOOTH:
			return stdNormalizedDim;			
		case PB_SLICEFROM: return stdAngleDim;
		case PB_SLICETO: return stdAngleDim;
		default:
			return defaultDim;
	}
}

// This method returns the name of the parameter whose index is passed.
TSTR SphereObject::GetParameterName(int pbIndex) 
{
	switch (pbIndex)
	{
		case PB_RADIUS:
//			return TSTR(GetString(IDS_RB_RADIUS));			
			return "Radius";
		case PB_HEMI:
//			return GetString(IDS_RB_HEMISPHERE);
			return "Hemisphere";
		case PB_SEGS:
//			return TSTR(GetString(IDS_RB_SEGS));			
			return "Segments";
		case PB_SMOOTH:
//			return TSTR(GetString(IDS_RB_SMOOTH));			
			return "Smooth";
		case PB_SLICEON:
//			return TSTR(GetString(IDS_AP_SLICEON));
			return "SliceOn";
		case PB_SLICEFROM:
//			return TSTR(GetString(IDS_AP_SLICEFROM));
			return "SliceFrom";
		case PB_SLICETO:
//			return TSTR(GetString(IDS_AP_SLICETO));
			return "SliceTo";
		default:
			return TSTR(_T(""));
	}
}

// This method is called to have the plug-in clone itself. This method should copy both the data structure and all the data residing in the
// data structure of this reference target. The plug-in should clone all its references as well.
//
// Important Note: All plug-ins that implement a Clone() method have to call this BaseClone() method with the old and the new object as parameters.
// The ordering in regards to when is method is called is unimportant however it must, of course, be called after the cloned object is created.
// This method allows base classes to copy their data into a new object created by the clone operation. All overwrites of BaseClone() must call
// the base class implementation. The base class implementation copyies the CustAttrib objects into the newly created object.
RefTargetHandle SphereObject::Clone(RemapDir& remap) 
{
	// It seems all MAX objects include the following lines...
	SphereObject* newob = new SphereObject();	
	newob->ReplaceReference(0, pblock->Clone(remap));
	newob->ivalid.SetEmpty();	
	BaseClone(this, newob, remap);
	return newob;
}

#endif