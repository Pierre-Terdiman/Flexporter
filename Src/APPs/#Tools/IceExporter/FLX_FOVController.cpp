///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a FOV controller.
 *	\file		FLX_FOVController.cpp
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
 *	Creates a FOV controller.
 *	\param		camera			[in] the MAX camera
 *	\param		exported_node	[in] the exported node's info structure
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddFOVController(GenCamera* camera, ResourceDescriptor* exported_node)
{
	// Checkings
	if(!camera || !exported_node)	return false;

	// Get and check FOV controller
	Control* FOVCtrl = camera->GetFOVControl();
	if(!FOVCtrl)	return true;

	// Check settings
	if(mSettings->mSingleFrame && !mProps.mExportFOVTrack)	return true;

	// Export it
	if(mSettings->mSampling)
	{
		// Dump the FOV data
		CustomArray FOVData;

		// Catch sampling rate
		long SamplingRate = mSettings->GetSamplingRate();

		Interval Inter=FOREVER; // Validity interval
		long Nb=0;
		for(long i=mTimeInfo.mStartTime;i<=mTimeInfo.mEndTime;i+=mTimeInfo.mDeltaTime * SamplingRate)
		{
			float Value;
			FOVCtrl->GetValue(i, &Value, Inter);
			FOVData.Store(Value * RADTODEG);
			Nb++;
		}

		// Create a new ControllerInfo structure. Not shared => create it, export it, destroy it
		ControllerDescriptor Ctrl;
		Ctrl.mField			= String("FOVTRACK");
		Ctrl.mObjectID		= mStats.mNbControllers++;
		Ctrl.mOwnerID		= exported_node->mObjectID;
		Ctrl.mOwnerType		= exported_node->mType;
		// Create ControllerData
		SampleDataCreate cdc;
		cdc.Mode			= CTRL_SAMPLES;
		cdc.Type			= FLX_CTRL_FLOAT;
		cdc.NbSamples		= Nb;
		cdc.SamplingRate	= SamplingRate;

		Ctrl.mData	= new SampleData;	CHECKALLOC(Ctrl.mData);
		Ctrl.mData->Init(&cdc);

		CopyMemory(((SampleData*)Ctrl.mData)->mSamples, FOVData.Collapse(), FOVData.GetSize());
		_DUMP_CONTROLLER("sampled FOV")
	}
	else
	{
		// ### TO CHECK

		// Dump the FOV data
		CustomArray FOVData;

		MAXCtrlType Type;
		udword Nb = CreateKeyframedData(FOVCtrl, FOVData, Type);
		if(Nb)
		{
			// Create a new ControllerInfo structure. Not shared => create it, export it, destroy it
			ControllerDescriptor Ctrl;
			Ctrl.mField		= String("FOVTRACK");
			Ctrl.mObjectID	= mStats.mNbControllers++;
			Ctrl.mOwnerID	= exported_node->mObjectID;
			Ctrl.mOwnerType	= exported_node->mType;
			// Create ControllerData
			KeyframeDataCreate kdc;
			kdc.Mode			= CTRL_KEYFRAMES;
			kdc.Type			= GetControllerType(Type);
			kdc.NbKeyframes		= Nb;
			kdc.Scheme			= GetControllerScheme(Type);

			Ctrl.mData	= new KeyframeData;	CHECKALLOC(Ctrl.mData);
			if(Ctrl.mData->Init(&kdc))
			{
				CopyMemory(((KeyframeData*)Ctrl.mData)->mKeyframes, FOVData.Collapse(), FOVData.GetSize());
				_DUMP_CONTROLLER("keyframed FOV")
			}
		}
	}

	return true;
}
