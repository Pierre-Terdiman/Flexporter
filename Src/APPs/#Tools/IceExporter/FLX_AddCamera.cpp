///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a camera.
 *	\file		FLX_AddCamera.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

static float gGlobalScale = 1.0f;

// Sampling callbacks
static float SampleNearClip		(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenCamera*)user_data)->GetClipDist(time_value, CAM_HITHER_CLIP, Inter) * gGlobalScale;	}
static float SampleFarClip		(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenCamera*)user_data)->GetClipDist(time_value, CAM_YON_CLIP, Inter) * gGlobalScale;	}
static float SampleTDist		(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenCamera*)user_data)->GetTDist(time_value, Inter) * gGlobalScale;						}
static float SampleNearRange	(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenCamera*)user_data)->GetEnvRange(time_value, ENV_NEAR_RANGE, Inter) * gGlobalScale;	}
static float SampleFarRange		(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenCamera*)user_data)->GetEnvRange(time_value, ENV_FAR_RANGE, Inter) * gGlobalScale;	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Converts a camera.
 *	\param		node			[in] the MAX node
 *	\param		derived_object	[in] a possible collapsed derived object
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddCamera(INode* node, Object* derived_object)
{
	// Checkings
	if(!node)	return false;
	if(!mSettings->mExpCameras && !mProps.mForceExport)
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
	Interval	Inter			= FOREVER;																	// Validity interval
	char*		CameraName		=			node	->GetName();											// Camera's name
	bool		IsGroupMember	=			node	->IsGroupMember()!=0;									// Belonging to a group
	INode*		Target			=			node	->GetTarget();											// Get possible camera target

	GenCamera*	Gc				= (GenCamera*)NodeObject;													// Assign node-object as a generic MAX camera

	bool		OrthoCam		=			Gc		->IsOrtho()!=0;											// Get camera type: ortographic or perspective
	float		FOV				=			Gc		->GetFOV(CurrentTime, Inter);							// Get Field-Of-View in radians or Width for ortho cams
	float		NearClip		=			Gc		->GetClipDist(CurrentTime, CAM_HITHER_CLIP, Inter);		// Get near clip
	float		FarClip			=			Gc		->GetClipDist(CurrentTime, CAM_YON_CLIP, Inter);		// Get far clip
	float		TDist			=			Gc		->GetTDist(CurrentTime, Inter);							// Get distance to target
	long		HLineDisplay	= (long)	Gc		->GetHorzLineState();									// Horizon Line Display
	float		EnvNearRange	=			Gc		->GetEnvRange(CurrentTime, ENV_NEAR_RANGE, Inter);		// Environment near range
	float		EnvFarRange		=			Gc		->GetEnvRange(CurrentTime, ENV_FAR_RANGE, Inter);		// Environment far range
	long		EnvDisplay		= (long)	Gc		->GetEnvDisplay();										// Environment display
	long		ManualClip		= (long)	Gc		->GetManualClip();										// Manual clip
	// Flexporter 1.12
	int			CamFOVType		=			Gc		->GetFOVType();											// FOV type
	int			CamType			=			Gc		->Type();												// Camera type

	// 2) Update log text
	mArrayLog.StoreASCII("Exporting camera %s\n", CameraName);

	// 3) Create info structure
	CameraDescriptor* CamInf = (CameraDescriptor*)mGlobalInfo.CreateDescriptor(node, FLX_OBJ_CAMERA);
	AddHeader(node, CamInf);

	// Get instance state (if needed)
	sdword MasterID;
	CamInf->mIsInstance = mSettings->mDiscardInstances ? false : mGlobalInfo.IsInstance(NodeObject, MasterID);

	// Check instance state (### to refactor)
	if(CamInf->mIsInstance)
	{
		CamInf->mMasterCamera	= (CameraDescriptor*)mGlobalInfo.GetNodeInfo()[MasterID].mExportedNode;
		CamInf->mMasterID		= CamInf->mMasterCamera->mObjectID;

		// Update log text
		mArrayLog.StoreASCII("Instance node from: ").StoreASCII(mGlobalInfo.GetNodeInfo()[MasterID].mMAXNode->GetName()).StoreASCII("\n");
	}

	CamInf->mOrthoCam			= OrthoCam;
	CamInf->mFOV				= FOV * RADTODEG;															// Convert FOV to degrees
	CamInf->mNearClip			= NearClip * mSettings->mGlobalScale;
	CamInf->mFarClip			= FarClip * mSettings->mGlobalScale;
	CamInf->mTDist				= TDist * mSettings->mGlobalScale;
	CamInf->mHLineDisplay		= HLineDisplay;
	CamInf->mEnvNearRange		= EnvNearRange * mSettings->mGlobalScale;
	CamInf->mEnvFarRange		= EnvFarRange * mSettings->mGlobalScale;
	CamInf->mEnvDisplay			= EnvDisplay;
	CamInf->mManualClip			= ManualClip;
	// Flexporter 1.12
	CamInf->mFOVType			= (FOVType)CamFOVType;
	CamInf->mCamType			= (CType)CamType;

	// 4) Extra transforms
	if(OrthoCam)	CamInf->mFOV = 1.0f / (FOV * RADTODEG);

	// 5) Update members
	mStats.mNbCameras++;

	// 6) Perform format-dependent export
	SetExtStatus("calling export plug-in...");
	if(mExporter)	mExporter->ExportCamera(*CamInf);
	DumpLogString();

	// 7) Animation
	// 7-1) PRS track
	SetExtStatus("exporting PRS data...");
	bool Status = AddPRSController(node, CamInf);

	// 7-2) Visibility track
	SetExtStatus("exporting visibility data...");
	Status = AddVisibilityController(node, CamInf);

	// 7-3) FOV track
	if(!mProps.mDiscardFOVTrack && (mSettings->mExpFOVCtrl || mProps.mExportFOVTrack))
	{
		SetExtStatus("exporting FOV data...");
		Status = AddFOVController(Gc, CamInf);
	}

	// 7-4) Attributes
	if(!mProps.mDiscardCamCtrl && (mSettings->mExpCamCtrl || mProps.mExportCamCtrl))
	{
		SetExtStatus("exporting camera attributes controllers...");
		Animatable* Attributes = GetSubAnim(Gc, "Parameters");
		if(Attributes)
		{
			// ...give callbacks access to global scale...
			gGlobalScale = mSettings->mGlobalScale;

			// Enable anim export temporarily
			bool Saved = mSettings->mSingleFrame;
			if(mProps.mExportCamCtrl)	mSettings->mSingleFrame = false;

			Status = AddFloatController(GetSubAnim(Attributes, "Near Clip"),		CamInf, "NearClip",		SampleNearClip,		udword(Gc));
			// 1.17 : the extra space is needed below !
			Status = AddFloatController(GetSubAnim(Attributes, "Far Clip "),		CamInf, "FarClip",		SampleFarClip,		udword(Gc));
			Status = AddFloatController(GetSubAnim(Attributes, "Target Distance"),	CamInf, "TDist",		SampleTDist,		udword(Gc));
			Status = AddFloatController(GetSubAnim(Attributes, "Near Env Range"),	CamInf, "NearEnvRange",	SampleNearRange,	udword(Gc));
			Status = AddFloatController(GetSubAnim(Attributes, "Far Env Range"),	CamInf, "FarEnvRange",	SampleFarRange,		udword(Gc));

			// Restore anim export status
			mSettings->mSingleFrame = Saved;
		}
	}
	return true;
}

