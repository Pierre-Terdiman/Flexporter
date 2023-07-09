///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the ASCII Format exporter plug-in for Flexporter.
 *	\file		IceASCIIExporter.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

static ASCIIFormat		gASCIIExporter;		// Gloabl exporter instance
static ExportSettings	gSettings;			// Global export settings

// FLEXPORTER Identification Callbacks

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Gives a brief exporter description. That string is displayed inside MAX, in the Options Panel.
 *	\relates	ASCIIFormat
 *	\fn			ExporterDescription()
 *	\return		a description string
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* ExporterDescription()
{ 
	return "ICE ASCII Exporter";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the format's extension. That string is displayed inside MAX, in the Options Panel.
 *	\relates	ASCIIFormat
 *	\fn			FormatExtension()
 *	\return		an extension string
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* FormatExtension()
{
	return "txt";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the one and only exporter instance.
 *	\relates	ASCIIFormat
 *	\fn			GetExporter()
 *	\return		pointer to the global exporter instance.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExportFormat* GetExporter()
{
	return &gASCIIExporter;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the default exportation settings for this format. This is the right place to initialize the default settings for your format.
 *	\relates	ASCIIFormat
 *	\fn			GetDefaultSettings()
 *	\return		pointer to the global ExportSettings instance.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExportSettings* GetDefaultSettings()
{
	// General settings
//	gSettings.mCompression					= FLX_COMPRESSION_NONE;
//	gSettings.mNbBits						= 8;
	gSettings.mExportWholeScene				= true;
	gSettings.mExportVisTrack				= false;
	gSettings.mExportHiddenNodes			= false;
	gSettings.mAutoUnload					= false;
	gSettings.mAutoLoad						= false;
	gSettings.mGlobalScale					= 1.0f;
	gSettings.mLocale						= FLX_LOCALE_ENGLISH;
	gSettings.mExportPHYSIQUE				= true;
	gSettings.mExportSKIN					= false;
	gSettings.mExportBIPED					= false;
	gSettings.mAlwaysExportMotion			= false;
	// Mesh settings
	gSettings.mExpUVW						= true;
	gSettings.mDiscardW						= true;
	gSettings.mExpVTXColor					= true;
	gSettings.mUseSmgrp						= true;
	gSettings.mRemoveScaling				= true;
	gSettings.mConvertToD3D					= true;
	gSettings.mAbsolutePRS					= false;
	gSettings.mConvexHull					= false;
	gSettings.mBoundingSphere				= false;
	gSettings.mInertiaTensor				= false;
	gSettings.mEdgeVis						= false;
	gSettings.mMakeManifold					= false;
	gSettings.mExpTargetNodes				= true;
	gSettings.mDiscardInstances				= false;
	// Consolidation settings
	gSettings.mConsolidateMesh				= false;
	gSettings.mComputeFaceNormals			= false;
	gSettings.mComputeVertexNormals			= true;
	gSettings.mExportNormalInfo				= false;
	gSettings.mWeightNormalsByArea			= false;
	gSettings.mCacheOptimize				= false;
	// Material settings
	gSettings.mForceAmbient					= false;
	gSettings.mForceDiffuse					= false;
	gSettings.mForceSpecular				= false;
	gSettings.mForceFilter					= false;
	gSettings.mExpMtlCtrl					= false;
	gSettings.mMaterialSharing				= false;
	// Texture settings
	gSettings.mOpacityInAlpha				= false;
	gSettings.mTexnameOnly					= true;
	gSettings.mKeepAmbientTexture			= false;
	gSettings.mKeepDiffuseTexture			= true;
	gSettings.mKeepSpecularTexture			= false;
	gSettings.mKeepShininessTexture			= false;
	gSettings.mKeepShiningStrengthTexture	= false;
	gSettings.mKeepSelfIllumTexture			= false;
	gSettings.mKeepOpacityTexture			= true;
	gSettings.mKeepFilterTexture			= false;
	gSettings.mKeepBumpTexture				= false;
	gSettings.mKeepReflexionTexture			= false;
	gSettings.mKeepRefractionTexture		= false;
	gSettings.mKeepDisplacementTexture		= false;
	gSettings.mTexMaxSize					= 0;
	gSettings.mFlipHorizontal				= false;
	gSettings.mFlipVertical					= false;
	gSettings.mQuantizeTextures				= false;
	gSettings.mDiscardTextureAlpha			= false;
	gSettings.mAutoCopyTextures				= false;
	// Camera settings
	gSettings.mExpCameras					= true;
	gSettings.mExpFOVCtrl					= false;
	gSettings.mExpCamCtrl					= false;
	// Light settings
	gSettings.mExpLights					= true;
	gSettings.mExpLitCtrl					= false;
	gSettings.mExpProjMaps					= false;
	gSettings.mComputeVtxColors				= false;
	gSettings.mComputeShadows				= false;
	gSettings.mColorSmoothing				= false;
	// Animation settings
	gSettings.mSingleFrame					= true;
	gSettings.mSampling						= true;
	gSettings.mSamplingRate					= 1;
	// Shape settings
	gSettings.mExpShapes					= true;
	gSettings.mShapeToMesh					= false;
	// Helper settings
	gSettings.mExpHelpers					= true;
	// Space warps settings
	gSettings.mExpSpaceWarps				= true;
	// IK settings
	gSettings.mExportIK						= false;

	return &gSettings;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the enabled/disabled status for all settings. This is the right place to disable options your own format doesn't support.
 *	\relates	ASCIIFormat
 *	\fn			GetEnabledSettings()
 *	\return		pointer to a global Enabled instance.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Enabled* GetEnabledSettings()
{
	// Let's disable some features for the ASCII format
	static Enabled Settings;
//	Settings.mCompression		= false;
	Settings.mOpacityInAlpha	= false;
	Settings.mTexnameOnly		= false;
//	Settings.mTexMaxSize		= false;	// since version 1.15 it makes sense
	Settings.mFlipHorizontal	= false;
	Settings.mFlipVertical		= false;
	Settings.mQuantizeTextures	= false;
	return &Settings;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the FLEXPORTER SDK Version.
 *	\relates	ASCIIFormat
 *	\fn			Version()
 *	\return		FLEXPORTER_VERSION
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Version()
{
	return FLEXPORTER_VERSION;
}










///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	An ASCII exporter plug-in for Flexporter..
 *	\class		ASCIIFormat
 *	\author		Pierre Terdiman
 *	\version	1.0
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASCIIFormat::ASCIIFormat()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ASCIIFormat::~ASCIIFormat()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Plug-in initialization method.
 *	This method is called once before each export. When this method is called, the mSettings and mFilename members of the base format are valid.
 *	\param		motion			[in] true for pure motion files.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ASCIIFormat::Init(bool motion)
{
	mGeneral
		.StoreASCII("// File created by FLEXPORTER\n")
		.StoreASCII("// Copyright (c) 2000-2004 Pierre Terdiman\n")
		.StoreASCII("//\n// (Quat format: x y z w)\n");

	if(!motion)
	{
		mGeneral		.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("GENERAL INFORMATION\n");
		mGeomObjects	.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("MESHES\n");
		mCameras		.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("CAMERAS\n");
		mLights			.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("LIGHTS\n");
		mShapes			.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("SHAPES\n");
		mHelpers		.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("HELPERS\n");
		mTexmaps		.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("TEXTURES\n");
		mMaterials		.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("MATERIALS\n");
		mControllers	.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("CONTROLLERS\n");
		mSpaceWarps		.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("SPACE WARPS\n");
	}
	mMotion			.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("MOTIONS\n");

	mIsMotionFile	= motion;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Scene export method.
 *	This method is called once to export the scene global information.
 *	\param		maininfo		[in] main info structure
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ASCIIFormat::SetSceneInfo(const MainDescriptor& maininfo)
{
	// Save time info for later
	CopyMemory(&mTimeInfo, &maininfo.mTime, sizeof(MAXTimeInfo));

	// Export time info
	mGeneral
		.StoreASCII("First frame: ")		.StoreASCII((long)(mTimeInfo.mStartTime / mTimeInfo.mDeltaTime))
		.StoreASCII("\nLast frame: ")		.StoreASCII((long)(mTimeInfo.mEndTime / mTimeInfo.mDeltaTime))
		.StoreASCII("\nFrame rate: ")		.StoreASCII(mTimeInfo.mFrameRate)
		.StoreASCII("\nTicks per frame: ")	.StoreASCII((long)mTimeInfo.mDeltaTime)
		.StoreASCII("\n");

	// Export background color
	mGeneral
		.StoreASCII("Background color: ")
		.StoreASCII(maininfo.mBackColor.r).StoreASCII(" ")
		.StoreASCII(maininfo.mBackColor.g).StoreASCII(" ")
		.StoreASCII(maininfo.mBackColor.b).StoreASCII("\n");

	// Export ambient color
	mGeneral
		.StoreASCII("Global ambient color: ")
		.StoreASCII(maininfo.mAmbColor.r).StoreASCII(" ")
		.StoreASCII(maininfo.mAmbColor.g).StoreASCII(" ")
		.StoreASCII(maininfo.mAmbColor.b).StoreASCII("\n");

	// Export scene info
	if(maininfo.mSceneInfos)	mGeneral.StoreASCII("\nScene info:   %s\n", maininfo.mSceneInfos);

	// Export help text
	if(maininfo.mSceneHelpText)	mGeneral.StoreASCII("\nHelp text:   %s\n", maininfo.mSceneHelpText);

	// Export various scene params [CHUNK_MAIN_VER >= 5]
	if(maininfo.mTesselation!=INVALID_ID)
	{
		mGeneral
			.StoreASCII("\nTesselation: %d\n", maininfo.mTesselation);
	}

	if(maininfo.mLightingMode!=INVALID_ID)
	{
		mGeneral
			.StoreASCII("\nLighting mode: %d\n", maininfo.mLightingMode);
	}

	// Export scene physics
	if(maininfo.mGravity.i!=INVALID_ID)
	{
		mGeneral
			.StoreASCII("\nGravity vector: %f %f %f\n", maininfo.mGravity.x, maininfo.mGravity.y, maininfo.mGravity.z);
	}

	if(maininfo.mRestitution>=0.0f)
	{
		mGeneral
		.StoreASCII("Restitution: %f\n", maininfo.mRestitution);
	}

	if(maininfo.mStaticFriction>=0.0f)
	{
		mGeneral
		.StoreASCII("StaticFriction: %f\n", maininfo.mStaticFriction);
	}

	if(maininfo.mFriction>=0.0f)
	{
		mGeneral
		.StoreASCII("Friction: %f\n", maininfo.mFriction);
	}

	if(maininfo.mGroundPlane)			mGeneral.StoreASCII("Ground plane enabled.\n");
	else								mGeneral.StoreASCII("Ground plane disabled.\n");
	if(maininfo.mCollisionDetection)	mGeneral.StoreASCII("Collision detection allowed.\n");
	else								mGeneral.StoreASCII("Collision detection disabled.\n");

	// Export global scale
	mGeneral.StoreASCII("\nGlobal scale: %f\n", maininfo.mGlobalScale);

	return true;
}

static void DumpDOF(const DOFDescriptor& dof, CustomArray& array, const char* name)
{
//	if(dof.mActive)
	{
		array.StoreASCII("%s:\n Active: %d\n Axis: %f %f %f\n Base: %f %f %f\n Limited: %d\n Min: %f\n Max: %f\n Current value: %f\n Damping: %f\n Spring: %f\n SpringTension: %f\n\n",
			name,
			dof.mActive,
			dof.mAxis.x, dof.mAxis.y, dof.mAxis.z,
			dof.mPos.x, dof.mPos.y, dof.mPos.z,
			dof.mLimit,
			dof.mMin,
			dof.mMax,
			dof.mCurrentValue,
			dof.mDamping,
			dof.mSpring,
			dof.mSpringTension
			);
	}
}

static void DumpJointData(const JointDescriptor* joint, CustomArray* array)
{
	if(!joint || !array)	return;

	array->StoreASCII("\n%d Pos dofs:\n\n", joint->mPosDOF);

	array->StoreASCII("\nPosition dofs:\n");

	DumpDOF(joint->mPos[0], *array, "X axis");
	DumpDOF(joint->mPos[1], *array, "Y axis");
	DumpDOF(joint->mPos[2], *array, "Z axis");
/*	for(udword i=0;i<3;i++)
	{
		array->StoreASCII("DOF %d:\n Active: %d\n Axis: %f %f %f\n Base: %f %f %f\n Limited: %d\n Min: %f\n Max: %f\n Current value: %f\n Damping: %f\n Spring: %f\n SpringTension: %f\n\n",
			i,
			joint->mPos[i].mActive,
			joint->mPos[i].mAxis.x, joint->mPos[i].mAxis.y, joint->mPos[i].mAxis.z,
			joint->mPos[i].mPos.x, joint->mPos[i].mPos.y, joint->mPos[i].mPos.z,
			joint->mPos[i].mLimit,
			joint->mPos[i].mMin,
			joint->mPos[i].mMax,
			joint->mPos[i].mCurrentValue,
			joint->mPos[i].mDamping,
			joint->mPos[i].mSpring,
			joint->mPos[i].mSpringTension
			);
	}
*/

	array->StoreASCII("\n%d Rot dofs:\n\n", joint->mRotDOF);

	array->StoreASCII("\nRotation dofs:\n");
/*
	for(i=0;i<3;i++)
	{
		array->StoreASCII("DOF %d:\n Active: %d\n Axis: %f %f %f\n Base: %f %f %f\n Limited: %d\n Min: %f\n Max: %f\n Current value: %f\n Damping: %f\n Spring: %f\n SpringTension: %f\n\n",
			i,
			joint->mRot[i].mActive,
			joint->mRot[i].mAxis.x, joint->mPos[i].mAxis.y, joint->mPos[i].mAxis.z,
			joint->mRot[i].mPos.x, joint->mPos[i].mPos.y, joint->mPos[i].mPos.z,
			joint->mRot[i].mLimit,
			joint->mRot[i].mMin,
			joint->mRot[i].mMax,
			joint->mRot[i].mCurrentValue,
			joint->mRot[i].mDamping,
			joint->mRot[i].mSpring,
			joint->mRot[i].mSpringTension
			);
	}
*/
	DumpDOF(joint->mRot[0], *array, "X axis");
	DumpDOF(joint->mRot[1], *array, "Y axis");
	DumpDOF(joint->mRot[2], *array, "Z axis");

#ifdef TOFIX
	array
		->StoreASCII("Scale: %f\nPreferred angle: %f\n", joint->mScale, joint->mPreferredAngle)
		.StoreASCII("PosJoint: ").StoreASCII(joint->mPos).StoreASCII("\n")
		.StoreASCII("RotJoint: ").StoreASCII(joint->mRot).StoreASCII("\n")
		.StoreASCII("LimitExact: ").StoreASCII(joint->mLimitExact).StoreASCII("\n")
		.StoreASCII("x-active: ").StoreASCII(joint->mXActive).StoreASCII("\n")
		.StoreASCII("y-active: ").StoreASCII(joint->mYActive).StoreASCII("\n")
		.StoreASCII("z-active: ").StoreASCII(joint->mZActive).StoreASCII("\n")
		.StoreASCII("x-limited: ").StoreASCII(joint->mXLimited).StoreASCII("\n")
		.StoreASCII("y-limited: ").StoreASCII(joint->mYLimited).StoreASCII("\n")
		.StoreASCII("z-limited: ").StoreASCII(joint->mZLimited).StoreASCII("\n")
		.StoreASCII("x-ease: ").StoreASCII(joint->mXEase).StoreASCII("\n")
		.StoreASCII("y-ease: ").StoreASCII(joint->mYEase).StoreASCII("\n")
		.StoreASCII("z-ease: ").StoreASCII(joint->mZEase).StoreASCII("\n")
		.StoreASCII("x-spring: ").StoreASCII(joint->mXSpring).StoreASCII("\n")
		.StoreASCII("y-spring: ").StoreASCII(joint->mYSpring).StoreASCII("\n")
		.StoreASCII("z-spring: ").StoreASCII(joint->mZSpring).StoreASCII("\n");

	array->StoreASCII("\n%d dof:\n", joint->mDOF);

	for(udword i=0;i<joint->mDOF;i++)
	{
		array->StoreASCII("DOF %d:\nMin: %f\nMax: %f\nDamping: %f\nSpring: %f\nSpringTension: %f\n\n",
			i,
			joint->mDOFs[i].mMin,
			joint->mDOFs[i].mMax,
			joint->mDOFs[i].mDamping,
			joint->mDOFs[i].mSpring,
			joint->mDOFs[i].mSpringTension
			);
	}
#endif
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Common method used to export basic information for a mesh, a camera, a light, etc.
 *	\param		obj			[in] current object
 *	\param		array		[out] destination export array
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ASCIIFormat::ExportBasicInfo(const ObjectDescriptor* obj, CustomArray* array)
{
	// Checkings
	if(!array || !obj) return false;

	// Export name
	array	->StoreASCII((const char*)obj->mName);

	// Export parent name
	if(obj->mParentName)
	array	->StoreASCII("\nParent:       %s", (const char*)obj->mParentName);

	// Export database management information
	array	->StoreASCII("\nNodeID:       ").StoreASCII(obj->mObjectID)
			.StoreASCII("\nParentNodeID: ")	.StoreASCII(obj->mParentID)
			.StoreASCII("\nMasterID:     ")	.StoreASCII(obj->mMasterID)
			.StoreASCII("\nTargetID:     ")	.StoreASCII(obj->mTargetID)
			.StoreASCII("\nIsInGroup:    ")	.StoreASCII(obj->mIsGroupMember)
			.StoreASCII("\nIsHidden:     ")	.StoreASCII(obj->mIsHidden)
			.StoreASCII("\n");

	// Export common properties
	array	->StoreASCII("\nPosition:     ").StoreASCII(obj->mPrs.mPosition.x)	.StoreASCII("  ")
											.StoreASCII(obj->mPrs.mPosition.y)	.StoreASCII("  ")
											.StoreASCII(obj->mPrs.mPosition.z)

			.StoreASCII("\nRotation:     ")	.StoreASCII(obj->mPrs.mRotation.x)	.StoreASCII("  ")
											.StoreASCII(obj->mPrs.mRotation.y)	.StoreASCII("  ")
											.StoreASCII(obj->mPrs.mRotation.z)	.StoreASCII("  ")
											.StoreASCII(obj->mPrs.mRotation.w)

			.StoreASCII("\nScale:        ")	.StoreASCII(obj->mPrs.mScale.x)		.StoreASCII("  ")
											.StoreASCII(obj->mPrs.mScale.y)		.StoreASCII("  ")
											.StoreASCII(obj->mPrs.mScale.z)

			.StoreASCII("\nWireColor:    ")	.StoreASCII(obj->mWireColor)

			.StoreASCII("\nLocal PRS:    ")	.StoreASCII(obj->mLocalPRS)

			.StoreASCII("\nD3D compliant:")	.StoreASCII(obj->mD3DCompliant);

	// ### TODO: export extra physics props

	// Export user-defined properties
	if(obj->mUserProps)	array->StoreASCII("\nUser props:   ").StoreASCII((const char*)obj->mUserProps);

	array	->StoreASCII("\n");

	// Export pivot
	array	->StoreASCII("\nPivot position:  ")	.StoreASCII(obj->mPivot.mWorldPivot.mPosition.x)	.StoreASCII("  ")
												.StoreASCII(obj->mPivot.mWorldPivot.mPosition.y)	.StoreASCII("  ")
												.StoreASCII(obj->mPivot.mWorldPivot.mPosition.z)

			.StoreASCII("\nPivot rotation:   ")	.StoreASCII(obj->mPivot.mWorldPivot.mRotation.x)	.StoreASCII("  ")
												.StoreASCII(obj->mPivot.mWorldPivot.mRotation.y)	.StoreASCII("  ")
												.StoreASCII(obj->mPivot.mWorldPivot.mRotation.z)	.StoreASCII("  ")
												.StoreASCII(obj->mPivot.mWorldPivot.mRotation.w)

			.StoreASCII("\nPivot scale:   ")	.StoreASCII(obj->mPivot.mWorldPivot.mScale.x)		.StoreASCII("  ")
												.StoreASCII(obj->mPivot.mWorldPivot.mScale.y)		.StoreASCII("  ")
												.StoreASCII(obj->mPivot.mWorldPivot.mScale.z);

	// Export IK data
	if(obj->mIKData)
	{
		array->StoreASCII("\n\n(IK node)\n");
		DumpJointData(obj->mIKData, array);
	}
/*	if(obj->mPosJoint)
	{
		array->StoreASCII("\n\nPos joint:\n");
		DumpJointData(obj->mPosJoint, array);
	}
	if(obj->mRotJoint)
	{
		array->StoreASCII("\n\nRot joint:\n");
		DumpJointData(obj->mRotJoint, array);
	}
*/
	array	->StoreASCII("\n\n");

	return true;
}

void ASCIIFormat::ExportTextureTransform(const TextureTransform& tt, CustomArray& array)
{
	// The cropping values
	const TextureCrop& TC = tt.mCValues;
	array
		.StoreASCII("\n\nCropping values:\n")
		.StoreASCII("OffsetU: ").StoreASCII(TC.mOffsetU)
		.StoreASCII("\nOffsetV: ").StoreASCII(TC.mOffsetV)
		.StoreASCII("\nScaleU: ").StoreASCII(TC.mScaleU)
		.StoreASCII("\nScaleV: ").StoreASCII(TC.mScaleV)
		.StoreASCII("\n");

	// The texture matrix
	const TextureMatrix& TM = tt.mTMtx;
	array
		.StoreASCII("\nTexture matrix:\n")
		.StoreASCII(TM.m[0][0]).StoreASCII(" ").StoreASCII(TM.m[0][1]).StoreASCII(" ").StoreASCII(TM.m[0][2]).StoreASCII("\n")
		.StoreASCII(TM.m[1][0]).StoreASCII(" ").StoreASCII(TM.m[1][1]).StoreASCII(" ").StoreASCII(TM.m[1][2]).StoreASCII("\n")
		.StoreASCII(TM.m[2][0]).StoreASCII(" ").StoreASCII(TM.m[2][1]).StoreASCII(" ").StoreASCII(TM.m[2][2]).StoreASCII("\n")
		.StoreASCII(TM.m[3][0]).StoreASCII(" ").StoreASCII(TM.m[3][1]).StoreASCII(" ").StoreASCII(TM.m[3][2]).StoreASCII("\n");

	// The texture tiling
	array.StoreASCII("\nTexture tiling U: ");
	if(tt.mTextureTilingU & TILING_WRAP)	array.StoreASCII(" WRAP");
	else									array.StoreASCII(" CLAMP");
	if(tt.mTextureTilingU & TILING_MIRROR)	array.StoreASCII(" MIRROR");

	array.StoreASCII("\nTexture tiling V: ");
	if(tt.mTextureTilingV & TILING_WRAP)	array.StoreASCII(" WRAP");
	else									array.StoreASCII(" CLAMP");
	if(tt.mTextureTilingV & TILING_MIRROR)	array.StoreASCII(" MIRROR");
	mMaterials.StoreASCII("\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	End of export notification.
 *	This method is called once all nodes have been exported. This is a convenient place to free all used ram, etc.
 *	\param		stats		[in] a structure filled with some export statistics.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ASCIIFormat::EndExport(const StatsDescriptor& stats)
{
	// Complete general chunk
	if(!mIsMotionFile)
	{
		mGeneral
			.StoreASCII("Number of geom objects:      ")	.StoreASCII(stats.mNbGeomObjects)
			.StoreASCII("\nNumber of derived objects:   ")	.StoreASCII(stats.mNbDerivedObjects)
			.StoreASCII("\nNumber of cameras:           ")	.StoreASCII(stats.mNbCameras)
			.StoreASCII("\nNumber of lights:            ")	.StoreASCII(stats.mNbLights)
			.StoreASCII("\nNumber of shapes:            ")	.StoreASCII(stats.mNbShapes)
			.StoreASCII("\nNumber of helpers:           ")	.StoreASCII(stats.mNbHelpers)
			.StoreASCII("\nNumber of controllers:       ")	.StoreASCII(stats.mNbControllers)
			.StoreASCII("\nNumber of materials:         ")	.StoreASCII(stats.mNbMaterials)
			.StoreASCII("\nNumber of texmaps:           ")	.StoreASCII(stats.mNbTexmaps)
			.StoreASCII("\nNumber of unknown nodes:     ")	.StoreASCII(stats.mNbUnknowns)
			.StoreASCII("\nNumber of object-free nodes: ")	.StoreASCII(stats.mNbInvalidNodes)
			.StoreASCII("\n");
	}

	// Export to disk
	{
		// Open output file
		udword fp = CreateFile(mFilename, FILE_WRITE, FILE_CREATE_ALWAYS);
		if(fp!=-1)
		{
			// Write data to disk
			void* Data;
			Data = mGeneral				.Collapse();	if(Data)	WriteFile(fp, Data, mGeneral		.GetSize());
			if(!mIsMotionFile)
			{
				Data = mGeomObjects		.Collapse();	if(Data)	WriteFile(fp, Data, mGeomObjects	.GetSize());
				Data = mCameras			.Collapse();	if(Data)	WriteFile(fp, Data, mCameras		.GetSize());
				Data = mLights			.Collapse();	if(Data)	WriteFile(fp, Data, mLights			.GetSize());
				Data = mShapes			.Collapse();	if(Data)	WriteFile(fp, Data, mShapes			.GetSize());
				Data = mHelpers			.Collapse();	if(Data)	WriteFile(fp, Data, mHelpers		.GetSize());
				Data = mTexmaps			.Collapse();	if(Data)	WriteFile(fp, Data, mTexmaps		.GetSize());
				Data = mMaterials		.Collapse();	if(Data)	WriteFile(fp, Data, mMaterials		.GetSize());
				Data = mControllers		.Collapse();	if(Data)	WriteFile(fp, Data, mControllers	.GetSize());
				Data = mSpaceWarps		.Collapse();	if(Data)	WriteFile(fp, Data, mSpaceWarps		.GetSize());
			}
			Data = mMotion				.Collapse();	if(Data)	WriteFile(fp, Data, mMotion			.GetSize());

			// Close output file
			CloseFile(fp);
		}

		// Free everything for next call
		mGeneral		.Clean();
		mGeomObjects	.Clean();
		mCameras		.Clean();
		mLights			.Clean();
		mShapes			.Clean();
		mHelpers		.Clean();
		mTexmaps		.Clean();
		mMaterials		.Clean();
		mControllers	.Clean();
		mMotion			.Clean();
		mSpaceWarps		.Clean();
	}

	return true;
}

// Test GUI overrides
/*
bool ASCIIFormat::InitGUI()
{
	return false;
}

bool ASCIIFormat::SetStatus(const char* text)
{
	return false;
}

bool ASCIIFormat::SetExtStatus(const char* text)
{
	return false;
}

bool ASCIIFormat::Progress(int pos)
{
	return false;
}

bool ASCIIFormat::DisplayLogText(const char* text)
{
	IceCore::MessageBox(null, text, "ASCIIFormat box", MB_OK);
	return false;
}

bool ASCIIFormat::CloseGUI()
{
	return false;
}
*/

// About box
void ASCIIFormat::About()
{
	IceCore::MessageBox(null, "Flexporter's native ASCII format", "About...", MB_OK);
}
