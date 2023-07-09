///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a helper.
 *	\file		FLX_AddHelper.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

	// For VRML Objects
	TCHAR *GetString(int id);
#undef IDD_ABOUT
#undef IDC_STATIC

#ifdef FLEXPORTER_MAX4
//	#include "x:\MAX\MAX4SDK\SAMPLES\ImpExp\Vrmlexp\vrml.h"
	#include "x:\MAX\MAX4SDK\SAMPLES\ImpExp\Vrmlexp\resource.h"
	#include "x:\MAX\MAX4SDK\SAMPLES\ImpExp\Vrmlexp\bboard.h"
#endif
#ifdef FLEXPORTER_MAX5
//	#include "x:\MAX\MAX5SDK\SAMPLES\ImpExp\Vrmlexp\vrml.h"
	#include "x:\MAX\MAX5SDK\SAMPLES\ImpExp\Vrmlexp\resource.h"
	#include "x:\MAX\MAX5SDK\SAMPLES\ImpExp\Vrmlexp\bboard.h"
#endif
#ifdef FLEXPORTER_MAX6
//	#include "x:\MAX\MAX6SDK\SAMPLES\ImpExp\Vrmlexp\vrml.h"
	#include "x:\MAX\MAX6SDK\SAMPLES\ImpExp\Vrmlexp\resource.h"
	#include "x:\MAX\MAX6SDK\SAMPLES\ImpExp\Vrmlexp\bboard.h"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Converts a helper.
 *	\param		node			[in] the MAX node
 *	\param		derived_object	[in] a possible collapsed derived object
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddHelper(INode* node, Object* derived_object)
{
	// Checkings
	if(!node)	return false;
	if(!mSettings->mExpHelpers && !mProps.mForceExport)
	{
		// Update log text
		mArrayLog.StoreASCII("(discarded because of export options)\n");
		return true;
	}

	// Get time info back
	long		StartTime		= mTimeInfo.mStartTime;
	long		EndTime			= mTimeInfo.mEndTime;
	long		DeltaTime		= mTimeInfo.mDeltaTime;
	long		FrameRate		= mTimeInfo.mFrameRate;
	TimeValue	CurrentTime		= mTimeInfo.mCurrentTime;

	// Get right object: it can be the real node or a collapsed derived object
	Object* NodeObject = derived_object ? derived_object : node->GetObjectRef();

	// 1) Get useful information from MAX
	Interval	Inter			= FOREVER;													// Validity interval
	char*		HelperName		= node->GetName();											// Helper's name
	bool		IsGroupMember	= node->IsGroupMember()!=0;									// Belonging to a group
	bool		IsGroupHead		= node->IsGroupHead()!=0;									// Group head ?

	HelperObject* Hlp			= (HelperObject*)NodeObject;								// Assign node-object as a generic MAX helper
	Class_ID	Type			= Hlp->ClassID();											// Catch helper type

	// 2) Update log text
	mArrayLog.StoreASCII("Exporting helper %s\n", HelperName);

	HType ht;
	ObjType RequestedType = FLX_OBJ_HELPER;
	if(Type==SPHEREGIZMO_CLASSID)
	{
		mArrayLog.StoreASCII("(Sphere gizmo)\n");
		ht = FLX_HTYPE_GIZMO_SPHERE;
		RequestedType = FLX_OBJ_SPHEREGIZMO;
	}
	else if(Type==CYLGIZMO_CLASSID)
	{
		mArrayLog.StoreASCII("(Cylinder gizmo)\n");
		ht = FLX_HTYPE_GIZMO_CYLINDER;
		RequestedType = FLX_OBJ_CYLINDERGIZMO;
	}
	else if(Type==BOXGIZMO_CLASSID)
	{
		mArrayLog.StoreASCII("(Box gizmo)\n");
		ht = FLX_HTYPE_GIZMO_BOX;
		RequestedType = FLX_OBJ_BOXGIZMO;
	}
	// 1.15
	else if(Type==Class_ID(DUMMY_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Dummy object)\n");
		ht = FLX_HTYPE_DUMMY;
	}
	else if(Type==Class_ID(TAPEHELP_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Tape helper)\n");
		ht = FLX_HTYPE_TAPE;
	}
	else if(Type==Class_ID(GRIDHELP_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Grid helper)\n");
		ht = FLX_HTYPE_GRID;
	}
	else if(Type==Class_ID(POINTHELP_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Point helper)\n");
		ht = FLX_HTYPE_POINT;
	}
	else if(Type==Class_ID(PROTHELP_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Protractor helper)\n");
		ht = FLX_HTYPE_PROTRACTOR;
	}
	else if(Type==Class_ID(0x60144302, 0x43455584))
	{
		mArrayLog.StoreASCII("(Crowd helper)\n");
		ht = FLX_HTYPE_CROWD;
	}
	else if(Type==Class_ID(0x69011e82, 0x5622b0d))
	{
		mArrayLog.StoreASCII("(Compass helper)\n");
		ht = FLX_HTYPE_COMPASS;
	}
	else if(Type==Class_ID(0x40c07baa, 0x245c7fe6))
	{
		mArrayLog.StoreASCII("(Delegate helper)\n");
		ht = FLX_HTYPE_DELEGATE;
	}
	// 1.17
	else if(Type==BillboardClassID)
	{
		mArrayLog.StoreASCII("(Billboard)\n");
		ht = FLX_HTYPE_BILLBOARD;
		RequestedType = FLX_OBJ_BILLBOARD;
	}
	else
	{
		mArrayLog.StoreASCII("(Undefined helper)\n");
		ht = FLX_HTYPE_UNDEFINED;
	}

	if(IsGroupHead)	mArrayLog.StoreASCII("(Group)\n");

	// 3) Create info structure
	HelperDescriptor* HelpInf = (HelperDescriptor*)mGlobalInfo.CreateDescriptor(node, RequestedType);
	AddHeader(node, HelpInf);

	// Get instance state (if needed)
	sdword MasterID;
	HelpInf->mIsInstance = mSettings->mDiscardInstances ? false : mGlobalInfo.IsInstance(NodeObject, MasterID);

	// Check instance state (### to refactor)
	if(HelpInf->mIsInstance)
	{
		HelpInf->mMasterHelper	= (HelperDescriptor*)mGlobalInfo.GetNodeInfo()[MasterID].mExportedNode;
		HelpInf->mMasterID		= HelpInf->mMasterHelper->mObjectID;

		// Update log text
		mArrayLog.StoreASCII("Instance node from: ").StoreASCII(mGlobalInfo.GetNodeInfo()[MasterID].mMAXNode->GetName()).StoreASCII("\n");
	}

	HelpInf->mIsGroupHead	= IsGroupHead;
	HelpInf->mHelperType	= ht;

	// Get gizmo data
	switch(ht)
	{
		case FLX_HTYPE_GIZMO_BOX:
		{
			BoxGizmoDescriptor* BoxGizmoInf = (BoxGizmoDescriptor*)HelpInf;
			BoxGizmoObject* BoxGizmo = (BoxGizmoObject*)Hlp;

			IParamBlock* PB = BoxGizmo->pblock;
			PB->GetValue(PB_BOXGIZMO_LENGTH,	CurrentTime, BoxGizmoInf->mLength, Inter);
			PB->GetValue(PB_BOXGIZMO_WIDTH,		CurrentTime, BoxGizmoInf->mWidth, Inter);
			PB->GetValue(PB_BOXGIZMO_HEIGHT,	CurrentTime, BoxGizmoInf->mHeight, Inter);

			// Fix values according to global scale
			BoxGizmoInf->mLength *= mSettings->mGlobalScale;
			BoxGizmoInf->mWidth *= mSettings->mGlobalScale;
			BoxGizmoInf->mHeight *= mSettings->mGlobalScale;
		}
		break;

		case FLX_HTYPE_GIZMO_CYLINDER:
		{
			CylinderGizmoDescriptor* CylinderGizmoInf = (CylinderGizmoDescriptor*)HelpInf;
			CylGizmoObject* CylGizmo = (CylGizmoObject*)Hlp;

			IParamBlock* PB = CylGizmo->pblock;
			PB->GetValue(PB_CYLGIZMO_RADIUS,	CurrentTime, CylinderGizmoInf->mRadius, Inter);
			PB->GetValue(PB_CYLGIZMO_HEIGHT,	CurrentTime, CylinderGizmoInf->mHeight, Inter);

			// Fix values according to global scale
			CylinderGizmoInf->mRadius *= mSettings->mGlobalScale;
			CylinderGizmoInf->mHeight *= mSettings->mGlobalScale;
		}
		break;

		case FLX_HTYPE_GIZMO_SPHERE:
		{
			SphereGizmoDescriptor* SphereGizmoInf = (SphereGizmoDescriptor*)HelpInf;
			SphereGizmoObject* SphereGizmo = (SphereGizmoObject*)Hlp;

			IParamBlock* PB = SphereGizmo->pblock;
			PB->GetValue(PB_GIZMO_RADIUS,	CurrentTime, SphereGizmoInf->mRadius, Inter);
			int Hemi;
			PB->GetValue(PB_GIZMO_HEMI,		CurrentTime, Hemi, Inter);
			SphereGizmoInf->mHemi = Hemi!=0;

			// Fix values according to global scale
			SphereGizmoInf->mRadius *= mSettings->mGlobalScale;
		}
		break;

		case FLX_HTYPE_BILLBOARD:
		{
			BillboardDescriptor* BillboardInf = (BillboardDescriptor*)HelpInf;
			BillboardObject* Billboard = (BillboardObject*)Hlp;

			IParamBlock* PB = Billboard->pblock;

			int ScreenAlign;
			PB->GetValue(PB_BB_SCREEN_ALIGN,	CurrentTime, ScreenAlign, Inter);
			BillboardInf->mScreenAlign = ScreenAlign!=0;

			PB->GetValue(PB_BB_SIZE,			CurrentTime, BillboardInf->mSize, Inter);
			PB->GetValue(PB_BB_LENGTH,			CurrentTime, BillboardInf->mLength, Inter);

			// Fix values according to global scale
			BillboardInf->mSize *= mSettings->mGlobalScale;
			BillboardInf->mLength *= mSettings->mGlobalScale;
		}
		break;
	};

	// 4) Update members
	mStats.mNbHelpers++;

	// 5) Perform format-dependent export
	SetExtStatus("calling export plug-in...");
	if(mExporter)	mExporter->ExportHelper(*HelpInf);
	DumpLogString();

	// 6) Animation
	// 6-1) PRS track
	SetExtStatus("exporting PRS data...");
	bool Status = AddPRSController(node, HelpInf);

	// 6-2) Visibility track
	SetExtStatus("exporting visibility data...");
	Status = AddVisibilityController(node, HelpInf);

	return true;
}

/*// subclasses of HELPER_CLASS_ID
#define DUMMY_CLASS_ID 			0x876234
#define BONE_CLASS_ID 			0x8a63c0
#define TAPEHELP_CLASS_ID 		0x02011
#define GRIDHELP_CLASS_ID		0x02010
#define POINTHELP_CLASS_ID		0x02013
#define PROTHELP_CLASS_ID		0x02014

class  HelperObject : public Object {
	public:
	SClass_ID SuperClassID() { return HELPER_CLASS_ID; }
	int IsRenderable() { return(0); }
	virtual void InitNodeName(TSTR& s) { s = _T("Helper"); }
	virtual int UsesWireColor() { return FALSE; }   // TRUE if the object color is used for display
	virtual BOOL NormalAlignVector(TimeValue t,Point3 &pt, Point3 &norm) {pt=Point3(0,0,0);norm=Point3(0,0,-1);return TRUE;}
	};

void AsciiExp::ExportHelperObject(INode* node, int indentLevel)
{
	TSTR indent = GetIndent(indentLevel);
	ExportNodeHeader(node, ID_HELPER, indentLevel);

	// We don't really know what kind of helper this is, but by exporting
	// the Classname of the helper object, the importer has a chance to
	// identify it.
	Object* helperObj = node->EvalWorldState(0).obj;
	if (helperObj) {
		TSTR className;
		helperObj->GetClassName(className);
		fprintf(pStream,"%s\t%s \"%s\"\n", indent.data(), ID_HELPER_CLASS, className);
	}

	ExportNodeTM(node, indentLevel);

	if (helperObj) {
		TimeValue	t = GetStaticFrame();
		Matrix3		oTM = node->GetObjectTM(t);
		Box3		bbox;

		helperObj->GetDeformBBox(t, bbox, &oTM);

		fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_BOUNDINGBOX_MIN, Format(bbox.pmin));
		fprintf(pStream,"%s\t%s %s\n", indent.data(), ID_BOUNDINGBOX_MAX, Format(bbox.pmax));
	}

	if (GetIncludeAnim()) {
		ExportAnimKeys(node, indentLevel);
	}
	
	fprintf(pStream,"%s}\n", indent.data());
}

*/
