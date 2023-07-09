///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a visibility controller.
 *	\file		FLX_VisibilityController.cpp
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
 *	Creates a visibility controller. (export the visibility track)
 *	\param		node			[in] the MAX node
 *	\param		exported_node	[in] the exported node's info structure
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddVisibilityController(INode* node, ResourceDescriptor* exported_node)
{
	// Checkings
	if(!node || !exported_node)		return false;

	 // Discard bones...
	if(IsSkeleton(node))			return true;

	// Check user-properties overrides
	if(mProps.mDiscardVisibility)	return true;

	// Is the input node animated...? Is the animation needed...?
	if(mSettings->mExportVisTrack && HasVisibilityTrack(node) && (!mSettings->mSingleFrame || mProps.mExportVisibility))
	{
		if(mSettings->mSampling)
		{
			// Dump the visibility data
			CustomArray VisData;

			long Nb=0;
			// Catch sampling rate
			long SamplingRate = mSettings->GetSamplingRate();

			for(long i=mTimeInfo.mStartTime;i<=mTimeInfo.mEndTime;i+=mTimeInfo.mDeltaTime * SamplingRate)
			{
				float Value = node->GetVisibility(i);
				// >=0 -> visible
				// <0  -> not visible
				VisData.Store(Value);
				Nb++;
			}

			// Create a new ControllerInfo structure. Not shared => create it, export it, destroy it
			ControllerDescriptor Ctrl;
			Ctrl.mField			= String("VISTRACK");
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

			CopyMemory(((SampleData*)Ctrl.mData)->mSamples, VisData.Collapse(), VisData.GetSize());
			_DUMP_CONTROLLER("sampled visibility")
		}
	}
	return true;
}
