///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export space warp objects.
 *	\file		FLX_AddSpaceWarp.cpp
 *	\author		Pierre Terdiman
 *	\date		March, 24, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Converts a space warp object.
 *	\param		node			[in] the MAX node
 *	\param		derived_object	[in] a possible collapsed derived object
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddSpaceWarp(INode* node, Object* derived_object)
{
	// Checkings
	if(!node)	return false;
	if(!mSettings->mExpSpaceWarps && !mProps.mForceExport)
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

	// 1) Get useful information from MAX
	char*		SpaceWarpName	= node->GetName();

	// Get right object: it can be the real node or a collapsed derived object
	Object* NodeObject = derived_object ? derived_object : node->GetObjectRef();

	// Get space warp's type
	Class_ID Type = NodeObject->ClassID();

	SpaceWarpDescriptor* SWD = null;

	// 2) Update log text
	mArrayLog.StoreASCII("Exporting space warp %s\n", SpaceWarpName);

	// Handle gravity object
	if(Type==Class_ID(GRAVITYOBJECT_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Gravity force)\n");

		// 3) Create info structure
		GravityObject* Gravity = (GravityObject*)mGlobalInfo.CreateDescriptor(node, FLX_OBJ_GRAVITY);
		AddHeader(node, Gravity);

		SimpleWSMObject2* Obj = (SimpleWSMObject2*)NodeObject;
		IParamBlock2* PB = Obj->pblock2;
		if(PB)
		{
			Interval Inter = FOREVER;

//			float mStrength;
//			float mDecay;
			int mType;
//			float mDispLength;

			PB->GetValue(GRAVITY_STRENGTH,		CurrentTime, Gravity->mStrength,	Inter);
			PB->GetValue(GRAVITY_DECAY,			CurrentTime, Gravity->mDecay,		Inter);
			PB->GetValue(GRAVITY_TYPE,			CurrentTime, mType,					Inter);
			PB->GetValue(GRAVITY_DISPLENGTH,	CurrentTime, Gravity->mDispLength,	Inter);

			// Take global scale into account
			Gravity->mStrength		*= mSettings->mGlobalScale;
			Gravity->mDispLength	*= mSettings->mGlobalScale;
		}

		SWD = Gravity;
	}
	else if(Type==Class_ID(WINDOBJECT_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Wind)\n");

		// 3) Create info structure
		WindObject* Wind = (WindObject*)mGlobalInfo.CreateDescriptor(node, FLX_OBJ_WIND);
		AddHeader(node, Wind);

		SimpleWSMObject2* Obj = (SimpleWSMObject2*)NodeObject;
		IParamBlock2* PB = Obj->pblock2;
		if(PB)
		{
			Interval Inter = FOREVER;

			int mType;

			PB->GetValue(WIND_STRENGTH,		CurrentTime, Wind->mStrength,	Inter);
			PB->GetValue(WIND_DECAY,		CurrentTime, Wind->mDecay,		Inter);
			PB->GetValue(WIND_TYPE,			CurrentTime, mType,				Inter);
			PB->GetValue(WIND_DISPLENGTH,	CurrentTime, Wind->mDispLength,	Inter);
			PB->GetValue(WIND_TURBULENCE,	CurrentTime, Wind->mTurbulence,	Inter);
			PB->GetValue(WIND_FREQUENCY,	CurrentTime, Wind->mFrequency,	Inter);
			PB->GetValue(WIND_SCALE,		CurrentTime, Wind->mScale,		Inter);

			// Take global scale into account
			Wind->mStrength		*= mSettings->mGlobalScale;
			Wind->mDispLength	*= mSettings->mGlobalScale;
			Wind->mScale		*= mSettings->mGlobalScale;
		}

		SWD = Wind;
	}
	else
	{
		mArrayLog.StoreASCII("(Unsupported space warp object)\n");
	}

	if(SWD)
	{
		// 4) Update members
		mStats.mNbSpaceWarps++;

		// 5) Perform format-dependent export
		SetExtStatus("calling export plug-in...");
		if(mExporter)	mExporter->ExportSpaceWarp(*SWD);
		DumpLogString();

		// 6) Animation
		// 6-1) PRS track
		SetExtStatus("exporting PRS data...");
		bool Status = AddPRSController(node, SWD);

		// 6-2) Visibility track
		SetExtStatus("exporting visibility data...");
		Status = AddVisibilityController(node, SWD);
	}
	return true;
}
