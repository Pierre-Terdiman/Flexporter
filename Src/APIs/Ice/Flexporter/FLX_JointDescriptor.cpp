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

using namespace Flexporter;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DOFDescriptor::DOFDescriptor() :
	mActive(false),
	mLimit(false),
	mEase(false),
	mSpring(false),
	mMin(0.0f),
	mMax(0.0f),
	mCurrentValue(0.0f),
	mDamping(0.0f),
	mSpringValue(0.0f),
	mSpringTension(0.0f),
	mValidFlags(0)
{
	mAxis.x = mAxis.y = mAxis.z = 0.0f;
	mPos.x = mPos.y = mPos.z = 0.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DOFDescriptor::~DOFDescriptor()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
JointDescriptor::JointDescriptor() :
	mPosDOF(0),
	mRotDOF(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
JointDescriptor::~JointDescriptor()
{
}

void JointDescriptor::CountDOFs()
{
	mPosDOF = mRotDOF = 0;
	if(mPos[0].mActive)	mPosDOF++;
	if(mPos[1].mActive)	mPosDOF++;
	if(mPos[2].mActive)	mPosDOF++;
	if(mRot[0].mActive)	mRotDOF++;
	if(mRot[1].mActive)	mRotDOF++;
	if(mRot[2].mActive)	mRotDOF++;
}

void JointDescriptor::FixData(const ExportSettings* settings)
{
	// Fix IK data before leaving
	if(settings)
	{
		// Setup number of DOFs
		CountDOFs();

		// Convert to D3D if needed
		// ### NO, THIS IS WRONG! "X", "Y" and "Z" here map pivot or local axes, which have already
		// been converted! This would be a double conversion...
	/*		if(mSettings->mConvertToD3D)
		{
			TSwap(mPos[1], mPos[2]);
			TSwap(mRot[1], mRot[2]);
		}
		// ### user-defined frame conversion missing
	*/

		// Rotation DOFs: leave as radians
		// Position DOFs: fix units
		for(udword i=0;i<3;i++)
		{
			mPos[i].mMin *= settings->mGlobalScale;
			mPos[i].mMax *= settings->mGlobalScale;
		}
	}
}
