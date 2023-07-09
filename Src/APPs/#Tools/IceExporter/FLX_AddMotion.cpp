///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a motion.
 *	\file		FLX_AddMotion.cpp
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
 *	Converts a motion.
 *	\param		charid		[in] character ID
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddMotion(udword charid)
{
	CustomArray Motion;

	// Update dialog
	SetStatus("Exporting motion...");
	SetExtStatus("exporting PRS...");

	// Get info structure for the first character
//	const CharInfo& Char = mGlobalInfo.mCharInfo[charid];
	const CharInfo& Char = mGlobalInfo.GetCharacterInfo()[charid];

	// Loop through all possible bones
	udword NbUsedBones = 0;
	for(udword i=0;i<GetCSTotalNbNodes();i++)
	{
		// Get current bone
		INode* CurNode = Char.mSkeleton[i].mBone;
		// Export bone's PR data
		if(CurNode)
		{
			ExportBonePRS(CurNode, Motion);
			NbUsedBones++;
		}

		// Update dialog

		int Pos = (i * 100)/GetCSTotalNbNodes();
		bool DefaultGUI = true;
		if(mExporter)	DefaultGUI = mExporter->Progress(Pos);
		if(mDialog)	mDialog->m_Progress.SetPos(Pos);
		// End of dialog-related code
	}

	if(NbUsedBones)
	{
		// Request motion name and motion type
		IceMotionDlg Dlg;
		Dlg.CenterWindow();
		Dlg.m_MotionName = "MotionName";
		Dlg.m_MotionType = "Undefined";
		Dlg.DoModal();

		// Create a MotionInfo structure
		MotionDescriptor MD;

		MD.mName			= (ubyte*)(const char*)Dlg.m_MotionName;
		MD.mType			= (ubyte*)(const char*)Dlg.m_MotionType;
		MD.mLocalPRS		= !mSettings->mAbsolutePRS;
		MD.mD3DCompliant	= mSettings->mConvertToD3D;
		MD.mMasterID		= charid;
		MD.mNbBones			= NbUsedBones;
		MD.mNbVirtualBones	= Char.mNbVirtualBones;
		MD.mData			= Motion.Collapse();

		// Perform format-dependent export
		SetExtStatus("calling export plug-in...");
		if(mExporter)	mExporter->ExportMotion(MD);
		DumpLogString();
	}
	return true;
}

MAXConverter& MAXConverter::ExportBonePRS(INode* node, CustomArray& array)
{
	AffineParts	StaticPRS;

	array.Store(GetCSID(node));

	if(mSettings->mSingleFrame)
	{
		// Here we must save the current frame only.
		array.Store((long)1);	// 1 == one frame == the static PRS

		GetStaticPRS(node, mTimeInfo.mCurrentTime, true, StaticPRS);

		// Global scaling
		{
			Point Scaling(1.0f, 1.0f, 1.0f);
			Scaling *= mSettings->mGlobalScale;

			if(IR(Scaling.x)!=IEEE_1_0)	StaticPRS.t.x *= Scaling.x;
			if(IR(Scaling.y)!=IEEE_1_0)	StaticPRS.t.y *= Scaling.y;
			if(IR(Scaling.z)!=IEEE_1_0)	StaticPRS.t.z *= Scaling.z;
		}

		// Version 1.13 - do the conversion for motion files
		if(mSettings->mConvertToD3D)
		{
			TSwap(StaticPRS.t.y, StaticPRS.t.z);
			TSwap(StaticPRS.q.y, StaticPRS.q.z);
		}
		//

		// User-defined frame conversion
		if(mExporter)
		{
			mExporter->ConvertPoint(StaticPRS.t.x, StaticPRS.t.y, StaticPRS.t.z);
			mExporter->ConvertQuat(StaticPRS.q.x, StaticPRS.q.y, StaticPRS.q.z, StaticPRS.q.w);
		}

		array.Store(StaticPRS.t.x).Store(StaticPRS.t.y).Store(StaticPRS.t.z);
		array.Store(StaticPRS.q.x).Store(StaticPRS.q.y).Store(StaticPRS.q.z).Store(StaticPRS.q.w);
	}
	else
	{
		// Here we must save information for all frames.
		array.Store((long)0).PushAddress();		// Leave room for #frames

		long Nb=0;
		long SamplingRate = mSettings->mSamplingRate;

		if(SamplingRate<=0) SamplingRate=1;	// Prevent bugs

		for(long i=mTimeInfo.mStartTime;i<=mTimeInfo.mEndTime;i+=mTimeInfo.mDeltaTime * SamplingRate)
		{
			GetStaticPRS(node, i, true, StaticPRS);

			// Global scaling
			{
				Point Scaling(1.0f, 1.0f, 1.0f);
				Scaling *= mSettings->mGlobalScale;

				if(IR(Scaling.x)!=IEEE_1_0)	StaticPRS.t.x *= Scaling.x;
				if(IR(Scaling.y)!=IEEE_1_0)	StaticPRS.t.y *= Scaling.y;
				if(IR(Scaling.z)!=IEEE_1_0)	StaticPRS.t.z *= Scaling.z;
			}

			// Version 1.13 - do the conversion for motion files
			if(mSettings->mConvertToD3D)
			{
				TSwap(StaticPRS.t.y, StaticPRS.t.z);
				TSwap(StaticPRS.q.y, StaticPRS.q.z);
			}
			//

			// User-defined frame conversion
			if(mExporter)
			{
				mExporter->ConvertPoint(StaticPRS.t.x, StaticPRS.t.y, StaticPRS.t.z);
				mExporter->ConvertQuat(StaticPRS.q.x, StaticPRS.q.y, StaticPRS.q.z, StaticPRS.q.w);
			}

			array.Store(StaticPRS.t.x).Store(StaticPRS.t.y).Store(StaticPRS.t.z);
			array.Store(StaticPRS.q.x).Store(StaticPRS.q.y).Store(StaticPRS.q.z).Store(StaticPRS.q.w);
			Nb++;
		}
		array.PopAddressAndStore(Nb);	// Store it back
	}
	return *this;
}