///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a float controller.
 *	\file		FLX_FloatController.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

/*		// Get the number of keys and the time of the ith key.
		virtual int NumKeys() {return NOT_KEYFRAMEABLE;}
		virtual TimeValue GetKeyTime(int index) {return 0;}
		virtual int GetKeyIndex(TimeValue t) {return -1;}
		virtual BOOL GetNextKeyTime(TimeValue t,DWORD flags,TimeValue &nt) {return FALSE;}
		virtual void CopyKeysFromTime(TimeValue src,TimeValue dst,DWORD flags) {} 
		virtual void DeleteKeyAtTime(TimeValue t) {}
		virtual BOOL IsKeyAtTime(TimeValue t,DWORD flags) {return FALSE;}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Creates a float controller.
 *	\param		anim			[in] a MAX animatable
 *	\param		exported_node	[in] the exported node's info structure
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddFloatController(Animatable* anim, ResourceDescriptor* exported_node, const char* name, SAMPLE_FLOAT_CALLBACK cb, udword user_data)
{
	// Checkings
	if(!anim || !exported_node)	return false;

	// Check the animatable has keyframes (or is worth sampling)
	udword NbKeys = GetNbKeys(anim);
	if(!NbKeys)	return true;	// Nothing to do

	// Check export settings
	if(mSettings->mSingleFrame)		return true;

	// Check float status
	if(!IsFloatController(anim))	return false;

	// Export it
	if(mSettings->mSampling)
	{
		// We need a sampling callback
		if(!cb)	return false;

		// Dump the float data
		CustomArray FloatData;

		// Catch sampling rate
		long SamplingRate = mSettings->GetSamplingRate();

		long Nb=0;
		for(long i=mTimeInfo.mStartTime;i<=mTimeInfo.mEndTime;i+=mTimeInfo.mDeltaTime * SamplingRate)
		{
			// Catch the sample
			float Value = (cb)(i, user_data);
			// Store it
			FloatData.Store(Value);
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
		cdc.Type			= FLX_CTRL_FLOAT;
		cdc.NbSamples		= Nb;
		cdc.SamplingRate	= SamplingRate;

		Ctrl.mData	= new SampleData;	CHECKALLOC(Ctrl.mData);
		Ctrl.mData->Init(&cdc);

		CopyMemory(((SampleData*)Ctrl.mData)->mSamples, FloatData.Collapse(), FloatData.GetSize());
		_DUMP_CONTROLLER("sampled float")
	}
	else
	{
		// ### TO CHECK

		// Dump the float data
		CustomArray FloatData;

		MAXCtrlType Type;
		udword Nb = CreateKeyframedData(anim, FloatData, Type);
		if(Nb)
		{
			// Create a new ControllerInfo structure. Not shared => create it, export it, destroy it
			ControllerDescriptor Ctrl;
			Ctrl.mField		= String(name);
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
				CopyMemory(((KeyframeData*)Ctrl.mData)->mKeyframes, FloatData.Collapse(), FloatData.GetSize());
				_DUMP_CONTROLLER("keyframed float")
			}
		}
	}

	return true;
}
