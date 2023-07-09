///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the ZCB Format exporter plug-in for Flexporter.
 *	\file		IceZCBExporter.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

static ZCBFormat		gZCBExporter;		// Gloabl exporter instance
static ExportSettings	gSettings;			// Global export settings

// FLEXPORTER Identification Callbacks

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Gives a brief exporter description. That string is displayed inside MAX, in the Options Panel.
 *	\relates	ZCBFormat
 *	\fn			ExporterDescription()
 *	\return		a description string
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* ExporterDescription()
{ 
	return "ICE ZCB Exporter";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the format's extension. That string is displayed inside MAX, in the Options Panel.
 *	\relates	ZCBFormat
 *	\fn			FormatExtension()
 *	\return		an extension string
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* FormatExtension()
{
	return "zcb";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the one and only exporter instance.
 *	\relates	ZCBFormat
 *	\fn			GetExporter()
 *	\return		pointer to the global exporter instance.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExportFormat* GetExporter()
{
	return &gZCBExporter;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the default exportation settings for this format. This is the right place to initialize the default settings for your format.
 *	\relates	ZCBFormat
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
	gSettings.mOpacityInAlpha				= true;
	gSettings.mTexnameOnly					= false;
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
	gSettings.mTexMaxSize					= 256;
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
	gSettings.mExpShapes					= false;
	gSettings.mShapeToMesh					= false;
	// Helper settings
	gSettings.mExpHelpers					= false;
	// Space warps settings
	gSettings.mExpSpaceWarps				= false;
	// IK settings
	gSettings.mExportIK						= false;

	return &gSettings;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the enabled/disabled status for all settings. This is the right place to disable options your own format doesn't support.
 *	\relates	ZCBFormat
 *	\fn			GetEnabledSettings()
 *	\return		pointer to a global Enabled instance.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Enabled* GetEnabledSettings()
{
	static Enabled Settings;
	return &Settings;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the FLEXPORTER SDK Version.
 *	\relates	ZCBFormat
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
 *	A ZCB exporter plug-in for Flexporter..
 *	\class		ZCBFormat
 *	\author		Pierre Terdiman
 *	\version	1.0
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBFormat::ZCBFormat() : mCompression(ZCB_COMPRESSION_NONE), mNbBits(8)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBFormat::~ZCBFormat()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Plug-in initialization method.
 *	This method is called once before each export. When this method is called, the mSettings and mFilename members of the base format are valid.
 *	\param		motion		[in] true for pure motion files.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBFormat::Init(bool motion)
{
	// Header
	udword FileType = (udword)motion ? ZCB_FILE_MOTION : ZCB_FILE_SCENE;
	FileType|=CHUNK_ZCB_VER;
	mGeneral.Store((char)'Z').Store((char)'C').Store((char)'B').Store((char)'!').Store(FileType);

	// Chunk & version
	if(!motion)
	{
		mGeneral		.Store("MAINCHUNK").Store((udword)CHUNK_MAIN_VER);
		mGeomObjects	.Store("MESHCHUNK").Store((udword)CHUNK_MESH_VER);
		mCameras		.Store("CAMSCHUNK").Store((udword)CHUNK_CAMS_VER);
		mLights			.Store("LITECHUNK").Store((udword)CHUNK_LITE_VER);
		mShapes			.Store("SHAPCHUNK").Store((udword)CHUNK_SHAP_VER);
		mHelpers		.Store("HELPCHUNK").Store((udword)CHUNK_HELP_VER);
		mTexmaps		.Store("TEXMCHUNK").Store((udword)CHUNK_TEXM_VER);
		mMaterials		.Store("MATLCHUNK").Store((udword)CHUNK_MATL_VER);
		mControllers	.Store("CTRLCHUNK").Store((udword)CHUNK_CTRL_VER);
		mMotion			.Store("MOVECHUNK").Store((udword)CHUNK_MOVE_VER);
	}
	else
	{
		mMotion			.Store("MOVECHUNK").Store((udword)CHUNK_MOVE_VER);
	}

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
bool ZCBFormat::SetSceneInfo(const MainDescriptor& maininfo)
{
	// Save time info for later
	CopyMemory(&mTimeInfo, &maininfo.mTime, sizeof(MAXTimeInfo));

	// Export time info
	mGeneral
		.Store((long)(mTimeInfo.mStartTime / mTimeInfo.mDeltaTime))
		.Store((long)(mTimeInfo.mEndTime / mTimeInfo.mDeltaTime))
		.Store(mTimeInfo.mFrameRate).Store((long)mTimeInfo.mDeltaTime);

	// Export background color
	mGeneral.Store(maininfo.mBackColor.r).Store(maininfo.mBackColor.g).Store(maininfo.mBackColor.b);

	// Export ambient color
	mGeneral.Store(maininfo.mAmbColor.r).Store(maininfo.mAmbColor.g).Store(maininfo.mAmbColor.b);

	// Export scene info
	if(maininfo.mSceneInfos)	mGeneral.Store((const char*)maininfo.mSceneInfos);
	mGeneral.Store(ubyte(0));

	// Export help text [CHUNK_MAIN_VER >= 4]
	if(maininfo.mSceneHelpText)	mGeneral.Store((const char*)maininfo.mSceneHelpText);
	mGeneral.Store(ubyte(0));

	// Export various scene params [CHUNK_MAIN_VER >= 5]
	mGeneral
		.Store(maininfo.mTesselation);
	// Export various scene params [CHUNK_MAIN_VER >= 6]
	mGeneral
		.Store(maininfo.mLightingMode);

	// Export scene physics [CHUNK_MAIN_VER >= 4]
	mGeneral
		.Store(maininfo.mGravity.x).Store(maininfo.mGravity.y).Store(maininfo.mGravity.z)
		.Store(maininfo.mRestitution)
		.Store(maininfo.mStaticFriction)
		.Store(maininfo.mFriction)
		.Store(maininfo.mGroundPlane)
		.Store(maininfo.mCollisionDetection);

	// Export global scale [CHUNK_MAIN_VER >= 3]
	mGeneral.Store(maininfo.mGlobalScale);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Common method used to export basic information for a mesh, a camera, a light, etc.
 *	\param		obj			[in] current object
 *	\param		array		[out] destination export array
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBFormat::ExportBasicInfo(const ObjectDescriptor* obj, CustomArray* array)
{
	// Checkings
	if(!array || !obj) return false;

	// Export name
	array	->Store((const char*)obj->mName).Store((ubyte)0);

	// Export database management information
	array	->Store(obj->mObjectID)
			.Store(obj->mParentID)
			.Store(obj->mMasterID)
			.Store(obj->mTargetID)
			.Store(obj->mIsGroupMember)
	// CHUNK_ZCB_VER >= 3
			.Store(obj->mIsHidden);

	// Export common properties
	array	->Store(obj->mPrs.mPosition.x)
			.Store(obj->mPrs.mPosition.y)
			.Store(obj->mPrs.mPosition.z)

			.Store(obj->mPrs.mRotation.x)
			.Store(obj->mPrs.mRotation.y)
			.Store(obj->mPrs.mRotation.z)
			.Store(obj->mPrs.mRotation.w)

			.Store(obj->mPrs.mScale.x)
			.Store(obj->mPrs.mScale.y)
			.Store(obj->mPrs.mScale.z)

			.Store(obj->mWireColor)

			.Store(obj->mLocalPRS)
			.Store(obj->mD3DCompliant)
	// CHUNK_ZCB_VER >= 3
			.Store(obj->mDensity)
			.Store(obj->mMass)
			.Store(obj->mSamplingDensity)
			.Store(obj->mResetPivot)
			.Store(obj->mIsCollidable)
			.Store(obj->mLockPivot)
	// CHUNK_ZCB_VER >= 4
			.Store(obj->mLinearVelocity.x).Store(obj->mLinearVelocity.y).Store(obj->mLinearVelocity.z)
			.Store(obj->mAngularVelocity.x).Store(obj->mAngularVelocity.y).Store(obj->mAngularVelocity.z);

	// Export user-defined properties
	if(obj->mUserProps)	array->Store((const char*)obj->mUserProps);
	array->Store((ubyte)0);

	// Export pivot
	array	->Store(obj->mPivot.mWorldPivot.mPosition.x)
			.Store(obj->mPivot.mWorldPivot.mPosition.y)
			.Store(obj->mPivot.mWorldPivot.mPosition.z)

			.Store(obj->mPivot.mWorldPivot.mRotation.x)
			.Store(obj->mPivot.mWorldPivot.mRotation.y)
			.Store(obj->mPivot.mWorldPivot.mRotation.z)
			.Store(obj->mPivot.mWorldPivot.mRotation.w);

	// ### missing pivot scale

	return true;
}

void ZCBFormat::ExportTextureTransform(const TextureTransform& tt, CustomArray& array)
{
	// Export cropping values
	const TextureCrop& TC = tt.mCValues;
	array
		.Store(TC.mOffsetU).Store(TC.mOffsetV).Store(TC.mScaleU).Store(TC.mScaleV);

	// Export texture matrix
	const TextureMatrix& TM = tt.mTMtx;
	array
		.Store(TM.m[0][0]).Store(TM.m[0][1]).Store(TM.m[0][2])
		.Store(TM.m[1][0]).Store(TM.m[1][1]).Store(TM.m[1][2])
		.Store(TM.m[2][0]).Store(TM.m[2][1]).Store(TM.m[2][2])
		.Store(TM.m[3][0]).Store(TM.m[3][1]).Store(TM.m[3][2]);

	// Export texture tiling
	// CHUNK_TEXM_VER >= 3
	// CHUNK_MATL_VER >= 4
	array.Store((tt.mTextureTilingU & TILING_WRAP)!=0);
	array.Store((tt.mTextureTilingU & TILING_MIRROR)!=0);
	array.Store((tt.mTextureTilingV & TILING_WRAP)!=0);
	array.Store((tt.mTextureTilingV & TILING_MIRROR)!=0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	End of export notification.
 *	This method is called once all nodes have been exported. This is a convenient place to free all used ram, etc.
 *	\param		stats		[in] a structure filled with some export statistics.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBFormat::EndExport(const StatsDescriptor& stats)
{
	// Complete general chunk
	if(!mIsMotionFile)
		mGeneral
			.Store(stats.mNbGeomObjects)
			.Store(stats.mNbDerivedObjects)
			.Store(stats.mNbCameras)
			.Store(stats.mNbLights)
			.Store(stats.mNbShapes)
			.Store(stats.mNbHelpers)
			.Store(stats.mNbControllers)
			.Store(stats.mNbMaterials)
			.Store(stats.mNbTexmaps)
			.Store(stats.mNbUnknowns)
			.Store(stats.mNbInvalidNodes);

	// Export to disk
	{
		// Open output file
		udword fp = CreateFile(mFilename, FILE_WRITE, FILE_CREATE_ALWAYS);
		if(fp!=-1)
		{
			// Write data to disk
			void* Data;
				// The "General" array must be exported first, since it may include the format's signature.
											{ Data = mGeneral			.Collapse();	if(Data)	WriteFile(fp, Data, mGeneral		.GetSize());}
			if(!mIsMotionFile)
			{
				udword TotalNb = stats.mNbGeomObjects + stats.mNbDerivedObjects;
				if(TotalNb)					{ Data = mGeomObjects		.Collapse();	if(Data)	WriteFile(fp, Data, mGeomObjects	.GetSize());}
				if(stats.mNbCameras)		{ Data = mCameras			.Collapse();	if(Data)	WriteFile(fp, Data, mCameras		.GetSize());}
				if(stats.mNbLights)			{ Data = mLights			.Collapse();	if(Data)	WriteFile(fp, Data, mLights			.GetSize());}
				if(stats.mNbShapes)			{ Data = mShapes			.Collapse();	if(Data)	WriteFile(fp, Data, mShapes			.GetSize());}
				if(stats.mNbHelpers)		{ Data = mHelpers			.Collapse();	if(Data)	WriteFile(fp, Data, mHelpers		.GetSize());}
				if(stats.mNbTexmaps)		{ Data = mTexmaps			.Collapse();	if(Data)	WriteFile(fp, Data, mTexmaps		.GetSize());}
				if(stats.mNbMaterials)		{ Data = mMaterials			.Collapse();	if(Data)	WriteFile(fp, Data, mMaterials		.GetSize());}
				if(stats.mNbControllers)	{ Data = mControllers		.Collapse();	if(Data)	WriteFile(fp, Data, mControllers	.GetSize());}
				if(stats.mNbDerivedObjects)	{ Data = mMotion			.Collapse();	if(Data)	WriteFile(fp, Data, mMotion			.GetSize());}
			}
			else							{ Data = mMotion			.Collapse();	if(Data)	WriteFile(fp, Data, mMotion			.GetSize());}

			// Close output file
			CloseFile(fp);

			// ...and while we're at it.....
			if(mCompression!=ZCB_COMPRESSION_NONE)
			{
				// Compress file
				// - Load the file from disk
				udword Size = GetFileSize(mFilename);
				CustomArray Importer(mFilename);
				void* SrcBuffer = Importer.GetAddress();

				// - Create a destination (packed) buffer
				ubyte* DstBuffer = new ubyte[Size*2];
				CHECKALLOC(DstBuffer);

				// - Compress the buffer
				udword DestLen=Size*2;
				bool Success = true;
				if(mCompression==ZCB_COMPRESSION_ZLIB)
				{
					int ErrorCode = compress2((Bytef*)DstBuffer, (uLongf*)&DestLen, (Bytef*)SrcBuffer, Size, Z_BEST_COMPRESSION);
					if(ErrorCode!=Z_OK)	Success = false;
				}
				else if(mCompression==ZCB_COMPRESSION_BZIP2)
				{
					int ErrorCode = BZ2_bzBuffToBuffCompress((char*)DstBuffer, &DestLen, (char*)SrcBuffer, Size, 9, 0, 0);
					if(ErrorCode!=BZ_OK)	Success = false;
				}

				// - Create a new ZCB packed file in memory
				if(Success)
				{
					CustomArray PackedBuffer;
					PackedBuffer
						.Store((char)'Z').Store((char)'C').Store((char)'B').Store((char)'P')
						.Store(mCompression)
						.Store(Size).Store(DestLen)
						.Store(DstBuffer, DestLen);

					// - Save the new ZCB file to disk
					PackedBuffer.ExportToDisk(mFilename);

					SetLogString("Compression successful!");
				}
				else SetLogString("Compression failed!");

				// Free used bytes
				DELETEARRAY(DstBuffer);
			}
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
	}

	return true;
}
