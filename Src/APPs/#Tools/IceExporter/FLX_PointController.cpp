///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a Point controller.
 *	\file		FLX_PointController.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Creates a Point controller.
 *	\param		anim			[in] a MAX animatable
 *	\param		exported_node	[in] the exported node's info structure
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddPointController(Animatable* anim, ResourceDescriptor* exported_node, const char* name, SAMPLE_POINT_CALLBACK cb, udword user_data)
{
	// Checkings
	if(!anim || !exported_node)	return false;

	// Check the animatable has keyframes (or is worth sampling)
	udword NbKeys = GetNbKeys(anim);
	if(!NbKeys)	return true;	// Nothing to do

	// Check export settings
	if(mSettings->mSingleFrame)	return true;

	// Check float status
	if(!IsPointController(anim))	return false;

	// Export it
	if(mSettings->mSampling)
	{
		// We need a sampling callback
		if(!cb)	return false;

		// Dump the Point data
		CustomArray PointData;

		// Catch sampling rate
		long SamplingRate = mSettings->GetSamplingRate();

		long Nb=0;
		for(long i=mTimeInfo.mStartTime;i<=mTimeInfo.mEndTime;i+=mTimeInfo.mDeltaTime * SamplingRate)
		{
			// Catch the sample
			Point3 Value = (cb)(i, user_data);
			// Store it
			PointData.Store(Value.x).Store(Value.y).Store(Value.z);
			// Once more
			Nb++;
		}

		// Create a new ControllerInfo structure. Not shared => create it, export it, destroy it
		ControllerDescriptor Ctrl;
		Ctrl.mField			= String(name);
		Ctrl.mObjectID		= mStats.mNbControllers++;
		Ctrl.mOwnerID		= exported_node->mObjectID;
		Ctrl.mOwnerType		= exported_node->mType;
		// Create ControllerData
		SampleDataCreate cdc;
		cdc.Mode			= CTRL_SAMPLES;
		cdc.Type			= FLX_CTRL_VECTOR;
		cdc.NbSamples		= Nb;
		cdc.SamplingRate	= SamplingRate;

		Ctrl.mData	= new SampleData;	CHECKALLOC(Ctrl.mData);
		Ctrl.mData->Init(&cdc);

		CopyMemory(((SampleData*)Ctrl.mData)->mSamples, PointData.Collapse(), PointData.GetSize());
		_DUMP_CONTROLLER("sampled point")
	}

	return true;
}
