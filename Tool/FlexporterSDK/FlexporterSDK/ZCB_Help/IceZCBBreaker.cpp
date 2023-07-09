///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code to import ZCB files.
 *	\file		IceZCBBreaker.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains an importer for ZCB files.
 *	- "ZCB" means "Zappy's Custom Binary"
 *	- at first it was only a test format, to check exported MAX data was ok
 *	- then people started to use it as-is !
 *	- so I kept it alive, but you may want to replace it with your own format in the end
 *
 *	\class		ZCBBreaker
 *	\author		Pierre Terdiman
 *	\version	1.0
 *	\date		2000-2002
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceRenderManager;

// Define this to make IDs unique among 3D objects/materials/textures
#define ZCB_MAKE_UNIQUE_IDS

// Lame implementation of a progress bar.
#define ZCB_PROGRESS_BAR

#define ZCB_CHECKALLOC(x)	{ if(!x) { ZCBImportError("Out of memory.", ZCB_ERROR_OUT_OF_MEMORY); return false;} }
#define ZCB_CHECKVERSION(file_version, current_version, chunk_name)																														\
		{		if(file_version>current_version)	{ ZCBImportError("Obsolete code! Get the latest ZCB reader.", ZCB_ERROR_UNKNOWN_VERSION);	return false;	}	\
		else	if(file_version<current_version)	ZCBImportError(_F("Found obsolete %s chunk. Please resave the file.", chunk_name), ZCB_ERROR_OBSOLETE_FILE);	}

// Ugly but...
static bool gHasPivot = false;
static udword gZCBVersion = 0;

#ifdef ZCB_PROGRESS_BAR
static ProgressBarParams* gParams = null;
static void InitProgressBar()
{
	gParams = new ProgressBarParams;
}

static void ReleaseProgressBar()
{
	DELETESINGLE(gParams);
}
#endif

static bool Cleanup()
{
#ifdef ZCB_PROGRESS_BAR
	gParams->mRequestCode = RQ_DELETE_PROGRESS_BAR;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);
	ReleaseProgressBar();
#endif
	return false;
}

static void UpdateProgressBar0()
{
#ifdef ZCB_PROGRESS_BAR
	gParams->mRequestCode = RQ_UPDATE_PROGRESS_BAR0;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);
#endif
}

static void UpdateProgressBar1()
{
#ifdef ZCB_PROGRESS_BAR
	gParams->mRequestCode = RQ_UPDATE_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);
#endif
}

static void PatchTextureID(sdword& id)
{
#ifdef ZCB_MAKE_UNIQUE_IDS
	if(id!=-1)	id+=0x10000000;
#endif
}

static void PatchMaterialID(sdword& id)
{
#ifdef ZCB_MAKE_UNIQUE_IDS
	if(id!=-1)	id+=0x20000000;
#endif
}

static void PatchControllerID(sdword& id)
{
#ifdef ZCB_MAKE_UNIQUE_IDS
	if(id!=-1)	id+=0x40000000;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBComponents::ZCBComponents()
{
	mNbMeshes		= 0;
	mNbDerived		= 0;
	mNbCameras		= 0;
	mNbLights		= 0;
	mNbShapes		= 0;
	mNbHelpers		= 0;
	mNbControllers	= 0;
	mNbMaterials	= 0;
	mNbTextures		= 0;
	mNbUnknowns		= 0;
	mNbInvalidNodes	= 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBComponents::~ZCBComponents()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBSceneInfo::ZCBSceneInfo() : mFirstFrame(0), mLastFrame(0), mFrameRate(0), mDeltaTime(0), mGlobalScale(1.0f)
{
	mBackColor		.Zero();
	mAmbientColor	.Zero();
	mGravity.Zero();
	mRestitution		= 0.0f;
	mStaticFriction		= 0.0f;
	mFriction			= 0.0f;
	mGroundPlane		= false;
	mCollisionDetection	= true;
	mTesselation		= INVALID_ID;
	mLightingMode		= INVALID_ID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBSceneInfo::~ZCBSceneInfo()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBBaseInfo::ZCBBaseInfo() : mID(INVALID_ID), mParentID(INVALID_ID), mTargetID(INVALID_ID), mMasterID(INVALID_ID)
{
	mPrs.Reset();
	mWireColor			= 0x7fffffff;
	mLocalPRS			= false;
	mD3DCompliant		= false;
	mGroup				= false;
	mIsHidden			= false;

	mDensity			= 0.0f;
	mMass				= 0.0f;
	mSamplingDensity	= 0;
	mResetPivot			= false;
	mIsCollidable		= true;
	mLockPivot			= false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBBaseInfo::~ZCBBaseInfo()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBCameraInfo::ZCBCameraInfo()
{
	mType			= ZCB_CAMERA_TARGET;
	mOrthoCam		= false;
	mFOV			= 0.0f;
	mFOVType		= ZCB_FOV_HORIZONTAL;
	mNearClip		= 0.0f;
	mFarClip		= 0.0f;
	mTDist			= 0.0f;
	mHLineDisplay	= 0;
	mEnvNearRange	= 0.0f;
	mEnvFarRange	= 0.0f;
	mEnvDisplay		= 0;
	mManualClip		= 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBCameraInfo::~ZCBCameraInfo()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBLightInfo::ZCBLightInfo()
{
	// ...
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBLightInfo::~ZCBLightInfo()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBMaterialInfo::ZCBMaterialInfo()
{
	mID						= INVALID_ID;

	mAmbientMapID			= INVALID_ID;
	mDiffuseMapID			= INVALID_ID;
	mSpecularMapID			= INVALID_ID;
	mShininessMapID			= INVALID_ID;
	mShiningStrengthMapID	= INVALID_ID;
	mSelfIllumMapID			= INVALID_ID;
	mOpacityMapID			= INVALID_ID;
	mFilterMapID			= INVALID_ID;
	mBumpMapID				= INVALID_ID;
	mReflexionMapID			= INVALID_ID;
	mRefractionMapID		= INVALID_ID;
	mDisplacementMapID		= INVALID_ID;
	mDecalMapID				= INVALID_ID;
	mDetailMapID			= INVALID_ID;
	mBillboardMapID			= INVALID_ID;

	mAmbientCoeff			= 1.0f;
	mDiffuseCoeff			= 1.0f;
	mSpecularCoeff			= 1.0f;
	mShininessCoeff			= 1.0f;
	mShiningStrengthCoeff	= 1.0f;
	mSelfIllumCoeff			= 1.0f;
	mOpacityCoeff			= 1.0f;
	mFilterCoeff			= 1.0f;
	mBumpCoeff				= 1.0f;
	mReflexionCoeff			= 1.0f;
	mRefractionCoeff		= 1.0f;
	mDisplacementCoeff		= 1.0f;

	mMtlAmbientColor		.Zero();
	mMtlDiffuseColor		.Zero();
	mMtlSpecularColor		.Zero();
	mMtlFilterColor			.Zero();

	mShading				= ZCB_SHADING_FORCE_DWORD;
	mSoften					= false;
	mFaceMap				= false;
	mTwoSided				= false;
	mWire					= false;
	mWireUnits				= false;
	mFalloffOut				= false;
	mTransparencyType		= ZCB_TRANSPA_FORCE_DWORD;

	mShininess				= 0.0f;
	mShiningStrength		= 0.0f;
	mSelfIllum				= 0.0f;
	mOpacity				= 1.0f;
	mOpaFalloff				= 0.0f;
	mWireSize				= 0.0f;
	mIOR					= 0.0f;

	mBounce					= 0.0f;
	mStaticFriction			= 0.0f;
	mSlidingFriction		= 0.0f;

	mSelfIllumOn			= false;
	mSelfIllumValue			= 0.0f;
	mSelfIllumColor			.Zero();

	mWrapU					= true;
	mMirrorU				= false;
	mWrapV					= true;
	mMirrorV				= false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBMaterialInfo::~ZCBMaterialInfo()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBTextureInfo::ZCBTextureInfo()
{
	mID					= INVALID_ID;
	mWidth				= 0;
	mHeight				= 0;
	mBitmap				= null;
	mHasAlpha			= false;
	mIsBitmapIncluded	= false;

	mWrapU					= true;
	mMirrorU				= false;
	mWrapV					= true;
	mMirrorV				= false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBTextureInfo::~ZCBTextureInfo()
{
	DELETEARRAY(mBitmap);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBNativeMeshInfo::ZCBNativeMeshInfo()
{
	mNbFaces		= 0;
	mNbVerts		= 0;
	mNbTVerts		= 0;
	mNbCVerts		= 0;
	mFlags			= 0;
	mFaces			= null;
	mTFaces			= null;
	mCFaces			= null;
	mFaceProperties	= null;
	mVerts			= null;
	mTVerts			= null;
	mCVerts			= null;
	mVertexAlpha	= null;
	mParity			= false;

	mBonesNb		= null;
	mBonesID		= null;
	mOffsetVectors	= null;
	mWeights		= null;
	mSkeleton		= null;
	mNbBones		= 0;

	mNbHullVerts	= 0;
	mNbHullFaces	= 0;
	mHullVerts		= null;
	mHullFaces		= null;

	mBSCenter.Zero();
	mBSRadius		= 0.0f;

	mCOM.Zero();
	mComputedMass	= 0.0f;
	ZeroMemory(mInertiaTensor, 9*sizeof(float));
	ZeroMemory(mCOMInertiaTensor, 9*sizeof(float));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBNativeMeshInfo::~ZCBNativeMeshInfo()
{
	DELETEARRAY(mHullFaces);
	DELETEARRAY(mHullVerts);

	DELETEARRAY(mSkeleton);
	DELETEARRAY(mWeights);
	DELETEARRAY(mOffsetVectors);
	DELETEARRAY(mBonesID);
	DELETEARRAY(mBonesNb);

	DELETEARRAY(mFaces);
	DELETEARRAY(mTFaces);
	DELETEARRAY(mCFaces);
	DELETEARRAY(mFaceProperties);
	DELETEARRAY(mVerts);
	DELETEARRAY(mTVerts);
	DELETEARRAY(mCVerts);
	DELETEARRAY(mVertexAlpha);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBMeshInfo::ZCBMeshInfo()
{
	mIsCollapsed	= false;
	mIsSkeleton		= false;
	mIsInstance		= false;
	mIsTarget		= false;
	mIsConvertible	= false;
	mIsSkin			= false;
	mCastShadows	= false;
	mReceiveShadows	= false;
	mMotionBlur		= false;

	mCharID			= INVALID_ID;
	mCSID			= INVALID_ID;

	mCleanMesh		= null;

	mNbColors		= 0;
	mColors			= null;

	mPrimParams		= null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBMeshInfo::~ZCBMeshInfo()
{
	DELETESINGLE(mPrimParams);
	DELETESINGLE(mCleanMesh);
	DELETEARRAY(mColors);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBShapeInfo::ZCBShapeInfo() : mType(ZCB_SHAP_UNDEFINED), mNbLines(0), mNbVerts(null), mClosed(null), mVerts(null), mTotalNbVerts(0), mMatID(INVALID_ID)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBShapeInfo::~ZCBShapeInfo()
{
	DELETEARRAY(mVerts);
	DELETEARRAY(mClosed);
	DELETEARRAY(mNbVerts);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBHelperInfo::ZCBHelperInfo() : mIsGroupHead(false)
{
	mHelperType		= ZCB_HELPER_DUMMY;
	mIsGroupHead	= false;
	mLength			= 0.0f;
	mWidth			= 0.0f;
	mHeight			= 0.0f;
	mRadius			= 0.0f;
	mHemi			= false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBHelperInfo::~ZCBHelperInfo()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBControllerInfo::ZCBControllerInfo()
{
	mObjectID		= INVALID_ID;
	mOwnerID		= INVALID_ID;
	mOwnerType		= ZCB_OBJ_UNDEFINED;

	mCtrlType		= ZCB_CTRL_NONE;
	mCtrlMode		= ZCB_CTRL_SAMPLES;

	mNbSamples		= 0;
	mSamplingRate	= 0;
	mSamples		= null;
	mNbVertices		= 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBControllerInfo::~ZCBControllerInfo()
{
	DELETEARRAY(mSamples);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBBreaker::ZCBBreaker() : mFileType(ZCB_FILE_FORCE_DWORD), mImportArray(null), mZCBGlobalVersion(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ZCBBreaker::~ZCBBreaker()
{
	ReleaseRam();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Frees used memory.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZCBBreaker::ReleaseRam()
{
	DELETESINGLE(mImportArray);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports a ZCB scene.
 *	\param		filename	[in] the scene's filename
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::Import(const String& filename)
{
	// Find the file somewhere in registered paths
	String FoundFile;

	// Check the file exists
	if(!FindFile(filename, &FoundFile))
	{
		ZCBImportError("File not found.", ZCB_ERROR_FILE_NOT_FOUND);
		return false;
	}

	// Release possibly already existing array
	DELETESINGLE(mImportArray);

	// Create a new array
	mImportArray = new CustomArray(FoundFile);
	ZCB_CHECKALLOC(mImportArray);

	// Parse the array
	return Import();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports a ZCB scene.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::Import()
{
	// Check format signature
	ubyte b1 = mImportArray->GetByte();
	ubyte b2 = mImportArray->GetByte();
	ubyte b3 = mImportArray->GetByte();

	// The three first bytes must be "ZCB"
	if(b1!='Z' || b2!='C' || b3!='B')
	{
		ZCBImportError("Invalid ZCB file.", ZCB_ERROR_INVALID_FILE);
		return false;
	}

	// The fourth byte can be '!' for normal files and 'P' for packed files.
	ubyte b4 = mImportArray->GetByte();
	if(b4=='P')
	{
		// This is a packed ZCB file. ZCB files are packed with ZLib or BZip2 only, so that anyone can read them back.
		udword Compression	= mImportArray->GetDword();	// Compression scheme
		udword OriginalSize	= mImportArray->GetDword();	// Size of the forthcoming depacked buffer
		udword PackedSize	= mImportArray->GetDword();	// Size of the packed buffer

		// Get some bytes for the depacked buffer
		ubyte* Depacked = new ubyte[OriginalSize];
		ZCB_CHECKALLOC(Depacked);

		if(Compression==ZCB_COMPRESSION_ZLIB)
		{
#ifdef __FLEXINESDK_H__
			if(!Decompress(Depacked, OriginalSize, mImportArray->GetAddress(), PackedSize, "ZLIB"))	return false;
#else
	#ifdef _ZLIB_H
			// Use ZLib to depack the file
			int ErrorCode = uncompress((Bytef*)Depacked, (uLongf*)&OriginalSize, (Bytef*)mImportArray->GetAddress(), PackedSize);
			if(ErrorCode!=Z_OK)
			{
				ZCBImportError("Depacking failed.", ZCB_ERROR_CANT_DEPACK);
				return false;
			}
	#else
			ZCBImportError("ZLIB depacking not available.", ZCB_ERROR_CANT_DEPACK);
			return false;
	#endif
#endif
		}
		else if(Compression==ZCB_COMPRESSION_BZIP2)
		{
#ifdef __FLEXINESDK_H__
			if(!Decompress(Depacked, OriginalSize, mImportArray->GetAddress(), PackedSize, "BZIP2"))	return false;
#else
	#ifdef _BZLIB_H
			// Use BZip2 to depack the file
			int ErrorCode = BZ2_bzBuffToBuffDecompress((char*)Depacked, &OriginalSize, (char*)mImportArray->GetAddress(), PackedSize, 0, 0);
			if(ErrorCode!=BZ_OK)
			{
				ZCBImportError("Depacking failed.", ZCB_ERROR_CANT_DEPACK);
				return false;
			}
	#else
			ZCBImportError("BZIP2 depacking not available.", ZCB_ERROR_CANT_DEPACK);
			return false;
	#endif
#endif
		}
		else
		{
			ZCBImportError("Depacking failed.", ZCB_ERROR_CANT_DEPACK);
			return false;
		}

		// Release packed buffer
		DELETESINGLE(mImportArray);

		// Create a new array filled with depacked data
		mImportArray = new CustomArray(OriginalSize, Depacked);
		ZCB_CHECKALLOC(mImportArray);
		mImportArray->Reset();

		// Release depacked buffer
		DELETEARRAY(Depacked);

		// And wrap it up
		return Import();
	}
	else if(b4=='!')
	{
		// This is a valid ZCB file. Get the type.
		udword Data = mImportArray->GetDword();
		// Since 1.14 we need to mask out reserved bits & get back the global version
		mZCBGlobalVersion = Data>>16;
		mFileType = ZCBFile(Data & ZCB_FILE_MASK);
		gZCBVersion = mZCBGlobalVersion;
	}
	else
	{
		// Uh... unknown header... corrupted file ?
		ZCBImportError("Invalid ZCB file.", ZCB_ERROR_INVALID_FILE);
		return false;
	}

	// Import the file
	if(mFileType==ZCB_FILE_SCENE)
	{
#ifdef ZCB_PROGRESS_BAR
		InitProgressBar();

		gParams->mRequestCode = RQ_CREATE_PROGRESS_BAR;
		GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);

		gParams->mMin = 0;
		gParams->mMax = 9;
		gParams->mRequestCode = RQ_SET_RANGE_PROGRESS_BAR0;
		GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);

		gParams->mText = "Importing file...";
		gParams->mRequestCode = RQ_SET_TEXT_PROGRESS_BAR0;
		GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);
#endif
		// Find MAIN chunk
		if(mImportArray->GetChunk(mZCBGlobalVersion ? "MAINCHUNK" : "MAIN"))
		{
			// Get chunk version
			mMAINVersion	= mImportArray->GetDword();
			ZCB_CHECKVERSION(mMAINVersion, CHUNK_MAIN_VER, "MAIN");

			// Ugly but....
			if(mMAINVersion>=2)	gHasPivot = true;
			else				gHasPivot = false;

			// Fill a scene structure
			ZCBSceneInfo	SceneInfo;
			// Time-related info
			SceneInfo.mFirstFrame		= mImportArray->GetDword();
			SceneInfo.mLastFrame		= mImportArray->GetDword();
			SceneInfo.mFrameRate		= mImportArray->GetDword();
			SceneInfo.mDeltaTime		= mImportArray->GetDword();
			// Background color
			SceneInfo.mBackColor.x		= mImportArray->GetFloat();
			SceneInfo.mBackColor.y		= mImportArray->GetFloat();
			SceneInfo.mBackColor.z		= mImportArray->GetFloat();
			// Global ambient color
			SceneInfo.mAmbientColor.x	= mImportArray->GetFloat();
			SceneInfo.mAmbientColor.y	= mImportArray->GetFloat();
			SceneInfo.mAmbientColor.z	= mImportArray->GetFloat();

			// Scene info
			if(mMAINVersion>=2)	SceneInfo.mSceneInfo.Set((const char*)mImportArray->GetString());
			if(mMAINVersion>=4)
			{
				SceneInfo.mSceneHelpText.Set((const char*)mImportArray->GetString());

				if(mMAINVersion>=5)
				{
					SceneInfo.mTesselation = mImportArray->GetDword();
				}
				if(mMAINVersion>=6)
				{
					SceneInfo.mLightingMode = mImportArray->GetDword();
				}

				// Scene physics
				SceneInfo.mGravity.x = mImportArray->GetFloat();
				SceneInfo.mGravity.y = mImportArray->GetFloat();
				SceneInfo.mGravity.z = mImportArray->GetFloat();
				SceneInfo.mRestitution = mImportArray->GetFloat();
				SceneInfo.mStaticFriction = mImportArray->GetFloat();
				SceneInfo.mFriction = mImportArray->GetFloat();
				SceneInfo.mGroundPlane = mImportArray->GetByte()!=0;
				SceneInfo.mCollisionDetection = mImportArray->GetByte()!=0;
			}

			if(mMAINVersion>=3)	SceneInfo.mGlobalScale = mImportArray->GetFloat();

			// Get number of expected elements
			mNbMeshes		= SceneInfo.mNbMeshes		= mImportArray->GetDword();
			mNbDerived		= SceneInfo.mNbDerived		= mImportArray->GetDword();
			mNbCameras		= SceneInfo.mNbCameras		= mImportArray->GetDword();
			mNbLights		= SceneInfo.mNbLights		= mImportArray->GetDword();
			mNbShapes		= SceneInfo.mNbShapes		= mImportArray->GetDword();
			mNbHelpers		= SceneInfo.mNbHelpers		= mImportArray->GetDword();
			mNbControllers	= SceneInfo.mNbControllers	= mImportArray->GetDword();
			mNbMaterials	= SceneInfo.mNbMaterials	= mImportArray->GetDword();
			mNbTextures		= SceneInfo.mNbTextures		= mImportArray->GetDword();
			mNbUnknowns		= SceneInfo.mNbUnknowns		= mImportArray->GetDword();
			mNbInvalidNodes	= SceneInfo.mNbInvalidNodes	= mImportArray->GetDword();

			// Call the app
			NewScene(SceneInfo);

			UpdateProgressBar0();
		}
		else
		{
			ZCBImportError("Chunk MAIN not found!", ZCB_ERROR_CHUNK_NOT_FOUND);
			return Cleanup();
		}

		// Import everything
		ImportMotion			(*mImportArray);
		if(!ImportCameras		(*mImportArray))	return Cleanup();	UpdateProgressBar0();
		if(!ImportLights		(*mImportArray))	return Cleanup();	UpdateProgressBar0();
		if(!ImportTextures		(*mImportArray))	return Cleanup();	UpdateProgressBar0();
		if(!ImportMaterials		(*mImportArray))	return Cleanup();	UpdateProgressBar0();
		if(!ImportMeshes		(*mImportArray))	return Cleanup();	UpdateProgressBar0();
		if(!ImportShapes		(*mImportArray))	return Cleanup();	UpdateProgressBar0();
		if(!ImportHelpers		(*mImportArray))	return Cleanup();	UpdateProgressBar0();
		if(!ImportControllers	(*mImportArray))	return Cleanup();	UpdateProgressBar0();

		Cleanup();
	}
	else if(mFileType==ZCB_FILE_MOTION)
	{
		if(!ImportMotion		(*mImportArray))	return false;
	}
	else
	{
		ZCBImportError("Invalid ZCB file.", ZCB_ERROR_INVALID_FILE);
		return false;
	}

	// Release array
	DELETESINGLE(mImportArray);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports a BIPED motion
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportMotion(CustomArray& importer)
{
	// Find MOVE chunk
	if(importer.GetChunk(mZCBGlobalVersion ? "MOVECHUNK" : "MOVE"))
	{
		// Get version number back
		mMOVEVersion	= importer.GetDword();
		ZCB_CHECKVERSION(mMOVEVersion, CHUNK_MOVE_VER, "MOVE");

		// Log message
		ZCBLog("Importing BIPED motion...\n");

		// Fill a motion structure
		ZCBMotionInfo Mot;

		Mot.mCharID		= importer.GetDword();						// LinkID
		Mot.mNbBones	= importer.GetDword();						// Nb bones
		Mot.mNbVBones	= importer.GetDword();						// Nb virtual bones
		// Since version 2
		if(mMOVEVersion>=2)	Mot.mLocalPRS = importer.GetByte()!=0;		// Local/global PRS
		else				Mot.mLocalPRS = true;						// Arbitrary
		// Since version 4
		if(mMOVEVersion>=4)	Mot.mD3DCompliant = importer.GetByte()!=0;	// D3D compatible
		else				Mot.mD3DCompliant = false;
		// Since version 3
		if(mMOVEVersion>=3)	Mot.mName.Set((const char*)importer.GetString());		// The motion's name
		else				Mot.mName.Set((const char*)"MotionName");
		if(mMOVEVersion>=3)	Mot.mType.Set((const char*)importer.GetString());		// The motion's type
		else				Mot.mType.Set((const char*)"MotionType");
		Mot.mData		= importer.GetAddress();				// Motion data

		// Call the app
		NewMotion(Mot);
	}
	else
	{
		// MOVE chunk is mandatory for motion files only
		if(mFileType==ZCB_FILE_MOTION)
		{
			ZCBImportError("Chunk MOVE not found!", ZCB_ERROR_CHUNK_NOT_FOUND);
			return false;
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports basic object information.
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBaseInfo::Import(CustomArray& importer)
{
	// Database information
	mName.Set((const char*)importer.GetString());		// The object's name
	mID					= importer.GetDword();			// The object's ID
	mParentID			= importer.GetDword();			// The parent's ID
	if(gZCBVersion>=0x2)
	{
		mMasterID		= importer.GetDword();			// ID of master object (ex: we're an instance)
		mTargetID		= importer.GetDword();			// ID of target object (ex: we're a camera)
	}
	else
	{
		// In old versions we only had a "link ID" whose role depended on context
		mTargetID		= importer.GetDword();			// The link ID (target nodes, master objects, etc)
		mMasterID		= mTargetID;
	}
	mGroup				= importer.GetByte()!=0;		// true if the object belongs to a group
	if(gZCBVersion>=0x3)
	{
		mIsHidden		= importer.GetByte()!=0;
	}
	// PRS
	mPrs.mPos.x			= importer.GetFloat();			// Position x
	mPrs.mPos.y			= importer.GetFloat();			// Position y
	mPrs.mPos.z			= importer.GetFloat();			// Position z

	mPrs.mRot.p.x		= importer.GetFloat();			// Rotation x
	mPrs.mRot.p.y		= importer.GetFloat();			// Rotation y
	mPrs.mRot.p.z		= importer.GetFloat();			// Rotation z
	mPrs.mRot.w			= importer.GetFloat();			// Rotation w

	mPrs.mScale.x		= importer.GetFloat();			// Scale x
	mPrs.mScale.y		= importer.GetFloat();			// Scale y
	mPrs.mScale.z		= importer.GetFloat();			// Scale z

	// Rendering information
	mWireColor			= importer.GetDword();			// The wireframe color
	mLocalPRS			= importer.GetByte()!=0;		// true for local PRS
	mD3DCompliant		= importer.GetByte()!=0;		// true if converted to D3D frame

	if(gZCBVersion>=0x3)
	{
		mDensity			= importer.GetFloat();
		mMass				= importer.GetFloat();
		mSamplingDensity	= importer.GetDword();
		mResetPivot			= importer.GetByte()!=0;
		mIsCollidable		= importer.GetByte()!=0;
		mLockPivot			= importer.GetByte()!=0;
	}

	// User-properties
	mUserProps.Set((const char*)importer.GetString());	// The user-defined properties

	// Get pivot
	if(gHasPivot)
	{
		mPivotPos.x		= importer.GetFloat();			// Position x
		mPivotPos.y		= importer.GetFloat();			// Position y
		mPivotPos.z		= importer.GetFloat();			// Position z

		mPivotRot.p.x	= importer.GetFloat();			// Rotation x
		mPivotRot.p.y	= importer.GetFloat();			// Rotation y
		mPivotRot.p.z	= importer.GetFloat();			// Rotation z
		mPivotRot.w		= importer.GetFloat();			// Rotation w
	}
	else
	{
		mPivotPos.Zero();
		mPivotRot.Identity();
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the cameras.
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportCameras(CustomArray& importer)
{
#ifdef ZCB_PROGRESS_BAR
	gParams->mText = "Importing cameras...";
	gParams->mRequestCode = RQ_SET_TEXT_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);

	gParams->mMin = 0;
	gParams->mMax = mNbCameras;
	gParams->mRequestCode = RQ_SET_RANGE_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);
#endif
	// Are there any cameras to import?
	if(mNbCameras)
	{
		// Find CAMS chunk
		if(importer.GetChunk(mZCBGlobalVersion ? "CAMSCHUNK" : "CAMS"))
		{
			// Get version number back
			mCAMSVersion	= importer.GetDword();
			ZCB_CHECKVERSION(mCAMSVersion, CHUNK_CAMS_VER, "CAMS");

			// Log message
			ZCBLog("Importing %d cameras...\n", mNbCameras);

			// Import all cameras
			for(udword n=0;n<mNbCameras;n++)
			{
				// Fill a camera structure
				ZCBCameraInfo CurCam;

				// Base info
				CurCam.Import(importer);

				// Get camera information back
				CurCam.mOrthoCam		= importer.GetByte()!=0;	// Camera type: ortographic (true) or perspective (false)
				CurCam.mFOV				= importer.GetFloat();		// Field-Of-View (degrees) or Width for ortho cams
				CurCam.mNearClip		= importer.GetFloat();		// Near/hither clip
				CurCam.mFarClip			= importer.GetFloat();		// Far/yon clip
				CurCam.mTDist			= importer.GetFloat();		// Distance to target
				CurCam.mHLineDisplay	= importer.GetDword();		// Horizon Line Display
				CurCam.mEnvNearRange	= importer.GetFloat();		// Environment near range
				CurCam.mEnvFarRange		= importer.GetFloat();		// Environment far range
				CurCam.mEnvDisplay		= importer.GetDword();		// Environment display
				CurCam.mManualClip		= importer.GetDword();		// Manual clip

				if(mCAMSVersion>=2)
				{
					CurCam.mFOVType		= (ZCBFOVType)importer.GetDword();		// FOV type
					CurCam.mType		= (ZCBCameraType)importer.GetDword();	// Camera type
				}

				// Call the app
				NewCamera(CurCam);

				UpdateProgressBar1();
			}
		}
		else
		{
			ZCBImportError("Chunk CAMS not found!", ZCB_ERROR_CHUNK_NOT_FOUND);
			return false;
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the lights.
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportLights(CustomArray& importer)
{
#ifdef ZCB_PROGRESS_BAR
	gParams->mText = "Importing lights...";
	gParams->mRequestCode = RQ_SET_TEXT_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);

	gParams->mMin = 0;
	gParams->mMax = mNbLights;
	gParams->mRequestCode = RQ_SET_RANGE_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);
#endif
	// Are there any lights to import?
	if(mNbLights)
	{
		// Find LITE chunk
		if(importer.GetChunk(mZCBGlobalVersion ? "LITECHUNK" : "LITE"))
		{
			// Get version number back
			mLITEVersion	= importer.GetDword();
			ZCB_CHECKVERSION(mLITEVersion, CHUNK_LITE_VER, "LITE");

			// Log message
			ZCBLog("Importing %d lights...\n", mNbLights);

			for(udword n=0;n<mNbLights;n++)
			{
				// Fill a light structure
				ZCBLightInfo CurLight;

				// Base info
				CurLight.Import(importer);

				// Get light information back
				CurLight.mLightType			= (ZCBLightType)importer.GetDword();	// Light's type
				CurLight.mIsSpot			= importer.GetByte()!=0;				// Is the light a spotlight?
				CurLight.mIsDir				= importer.GetByte()!=0;				// Is the light a directional?
				CurLight.mColor.x			= importer.GetFloat();					// Light's color
				CurLight.mColor.y			= importer.GetFloat();					//
				CurLight.mColor.z			= importer.GetFloat();					//
				CurLight.mIntensity			= importer.GetFloat();					// Light's intensity
				CurLight.mContrast			= importer.GetFloat();					// Light's contrast
				CurLight.mDiffuseSoft		= importer.GetFloat();					// Light's diffuse soft
				CurLight.mLightUsed			= importer.GetByte()!=0;				// Is the light used?
				CurLight.mAffectDiffuse		= importer.GetByte()!=0;				// Does the light affect diffuse?
				CurLight.mAffectSpecular	= importer.GetByte()!=0;				// Does the light affect specular?
				CurLight.mUseAttenNear		= importer.GetByte()!=0;				//
				CurLight.mAttenNearDisplay	= importer.GetByte()!=0;				//
				CurLight.mUseAtten			= importer.GetByte()!=0;				// Is attenuation used?
				CurLight.mShowAtten			= importer.GetByte()!=0;				//
				CurLight.mNearAttenStart	= importer.GetFloat();					// Near atten start
				CurLight.mNearAttenEnd		= importer.GetFloat();					// Near atten end
				CurLight.mAttenStart		= importer.GetFloat();					// Atten start
				CurLight.mAttenEnd			= importer.GetFloat();					// Atten end (use that as a range for non-dir lights)
				CurLight.mDecayType			= importer.GetByte();					// Light's decay type
				CurLight.mHotSpot			= importer.GetFloat();					// Light's hotspot
				CurLight.mFallsize			= importer.GetFloat();					// Light's falloff
				CurLight.mAspect			= importer.GetFloat();					// Light's aspect
				CurLight.mSpotShape			= (ZCBSpotShape)importer.GetDword();	// Light's spot shape
				CurLight.mOvershoot			= importer.GetDword();					// Light's overshoot
				CurLight.mConeDisplay		= importer.GetByte()!=0;				//
				CurLight.mTDist				= importer.GetFloat();					// Distance to target
				CurLight.mShadowType		= importer.GetDword();					// Light's shadow type
				CurLight.mAbsMapBias		= importer.GetDword();					// Light's absolute map bias
				CurLight.mRayBias			= importer.GetFloat();					// Raytrace bias
				CurLight.mMapBias			= importer.GetFloat();					// Map bias
				CurLight.mMapRange			= importer.GetFloat();					// Map range
				CurLight.mMapSize			= importer.GetDword();					// Map size

				if(mLITEVersion>=2)	CurLight.mCastShadows	= importer.GetByte()!=0;	// Cast shadows
				else				CurLight.mCastShadows	= false;

				if(mLITEVersion>=3)
				{
					CurLight.mShadowDensity			= importer.GetFloat();		// Shadow density
					CurLight.mShadowColor.x			= importer.GetFloat();		// Shadow color
					CurLight.mShadowColor.y			= importer.GetFloat();		// Shadow color
					CurLight.mShadowColor.z			= importer.GetFloat();		// Shadow color
					CurLight.mLightAffectsShadow	= importer.GetByte()!=0;	// Light affects shadow or not
				}
				else
				{
					CurLight.mShadowDensity			= 0.0f;
					CurLight.mShadowColor.x			= 0.0f;
					CurLight.mShadowColor.y			= 0.0f;
					CurLight.mShadowColor.z			= 0.0f;
					CurLight.mLightAffectsShadow	= false;
				}

				if(mLITEVersion>=4)
				{
					CurLight.mProjMapID			= importer.GetDword();
					CurLight.mShadowProjMapID	= importer.GetDword();
					PatchTextureID(CurLight.mProjMapID);
					PatchTextureID(CurLight.mShadowProjMapID);
				}
				else
				{
					CurLight.mProjMapID			= -1;
					CurLight.mShadowProjMapID	= -1;
				}

				// Call the app
				NewLight(CurLight);

				UpdateProgressBar1();
			}
		}
		else
		{
			ZCBImportError("Chunk LITE not found!", ZCB_ERROR_CHUNK_NOT_FOUND);
			return false;
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the cropping values & texture matrix.
 *	\param		cvalues		[out] a place to store the cropping values
 *	\param		tmtx		[out] a place to store the texture matrix
 *	\param		importer	[in] the imported array.
 *	\param		wrap_u		[out] 
 *	\param		mirror_u	[out] 
 *	\param		wrap_v		[out] 
 *	\param		mirror_v	[out] 
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportCroppingValues(TextureCrop& cvalues, TextureMatrix& tmtx, CustomArray& importer, bool& wrap_u, bool& mirror_u, bool& wrap_v, bool& mirror_v)
{
	// Get cropping values back
	cvalues.mOffsetU	= importer.GetFloat();
	cvalues.mOffsetV	= importer.GetFloat();
	cvalues.mScaleU		= importer.GetFloat();
	cvalues.mScaleV		= importer.GetFloat();

	// Get texture matrix back
	tmtx.m[0][0]		= importer.GetFloat();
	tmtx.m[0][1]		= importer.GetFloat();
	tmtx.m[0][2]		= importer.GetFloat();

	tmtx.m[1][0]		= importer.GetFloat();
	tmtx.m[1][1]		= importer.GetFloat();
	tmtx.m[1][2]		= importer.GetFloat();

	tmtx.m[2][0]		= importer.GetFloat();
	tmtx.m[2][1]		= importer.GetFloat();
	tmtx.m[2][2]		= importer.GetFloat();

	tmtx.m[3][0]		= importer.GetFloat();
	tmtx.m[3][1]		= importer.GetFloat();
	tmtx.m[3][2]		= importer.GetFloat();

	// Export texture tiling
	// CHUNK_TEXM_VER >= 3
	// CHUNK_MATL_VER >= 4
	if(mTEXMVersion >= 3 || mMATLVersion >= 4)
	{
		wrap_u		= importer.GetByte()!=0;
		mirror_u	= importer.GetByte()!=0;
		wrap_v		= importer.GetByte()!=0;
		mirror_v	= importer.GetByte()!=0;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the materials.
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportMaterials(CustomArray& importer)
{
#ifdef ZCB_PROGRESS_BAR
	gParams->mText = "Importing materials...";
	gParams->mRequestCode = RQ_SET_TEXT_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);

	gParams->mMin = 0;
	gParams->mMax = mNbMaterials;
	gParams->mRequestCode = RQ_SET_RANGE_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);
#endif
	// Are there any materials to import?
	if(mNbMaterials)
	{
		// Find MATL chunk
		if(importer.GetChunk(mZCBGlobalVersion ? "MATLCHUNK" : "MATL"))
		{
			// Get version number back
			mMATLVersion	= importer.GetDword();
			ZCB_CHECKVERSION(mMATLVersion, CHUNK_MATL_VER, "MATL");
//IceCore::MessageBox(null, _F("MATL version %d", mMATLVersion), "Report", MB_OK);			
			// Log message
			ZCBLog("Importing %d materials...\n", mNbMaterials);

			// Import all materials
			for(udword n=0;n<mNbMaterials;n++)
			{
				// Fill a material structure
				ZCBMaterialInfo CurMaterial;

				// Database information
				CurMaterial.mName.Set((const char*)importer.GetString());	// Material name
				CurMaterial.mID	= importer.GetDword();						// Material ID
//Log("Imported material: %s - %d\n", CurMaterial.mName, CurMaterial.mID);
				PatchMaterialID(CurMaterial.mID);

				// Texture IDs
				CurMaterial.mAmbientMapID			= importer.GetDword();	PatchTextureID(CurMaterial.mAmbientMapID);
				CurMaterial.mDiffuseMapID			= importer.GetDword();	PatchTextureID(CurMaterial.mDiffuseMapID);
				CurMaterial.mSpecularMapID			= importer.GetDword();	PatchTextureID(CurMaterial.mSpecularMapID);
				CurMaterial.mShininessMapID			= importer.GetDword();	PatchTextureID(CurMaterial.mShininessMapID);
				CurMaterial.mShiningStrengthMapID	= importer.GetDword();	PatchTextureID(CurMaterial.mShiningStrengthMapID);
				CurMaterial.mSelfIllumMapID			= importer.GetDword();	PatchTextureID(CurMaterial.mSelfIllumMapID);
				CurMaterial.mOpacityMapID			= importer.GetDword();	PatchTextureID(CurMaterial.mOpacityMapID);
				CurMaterial.mFilterMapID			= importer.GetDword();	PatchTextureID(CurMaterial.mFilterMapID);
				CurMaterial.mBumpMapID				= importer.GetDword();	PatchTextureID(CurMaterial.mBumpMapID);
				CurMaterial.mReflexionMapID			= importer.GetDword();	PatchTextureID(CurMaterial.mReflexionMapID);
				CurMaterial.mRefractionMapID		= importer.GetDword();	PatchTextureID(CurMaterial.mRefractionMapID);
				CurMaterial.mDisplacementMapID		= importer.GetDword();	PatchTextureID(CurMaterial.mDisplacementMapID);

				// Amounts
				CurMaterial.mAmbientCoeff			= importer.GetFloat();
				CurMaterial.mDiffuseCoeff			= importer.GetFloat();
				CurMaterial.mSpecularCoeff			= importer.GetFloat();
				CurMaterial.mShininessCoeff			= importer.GetFloat();
				CurMaterial.mShiningStrengthCoeff	= importer.GetFloat();
				CurMaterial.mSelfIllumCoeff			= importer.GetFloat();
				CurMaterial.mOpacityCoeff			= importer.GetFloat();
				CurMaterial.mFilterCoeff			= importer.GetFloat();
				CurMaterial.mBumpCoeff				= importer.GetFloat();
				CurMaterial.mReflexionCoeff			= importer.GetFloat();
				CurMaterial.mRefractionCoeff		= importer.GetFloat();
				CurMaterial.mDisplacementCoeff		= importer.GetFloat();

				// Colors
				CurMaterial.mMtlAmbientColor.x		= importer.GetFloat();
				CurMaterial.mMtlAmbientColor.y		= importer.GetFloat();
				CurMaterial.mMtlAmbientColor.z		= importer.GetFloat();

				CurMaterial.mMtlDiffuseColor.x		= importer.GetFloat();
				CurMaterial.mMtlDiffuseColor.y		= importer.GetFloat();
				CurMaterial.mMtlDiffuseColor.z		= importer.GetFloat();

				CurMaterial.mMtlSpecularColor.x		= importer.GetFloat();
				CurMaterial.mMtlSpecularColor.y		= importer.GetFloat();
				CurMaterial.mMtlSpecularColor.z		= importer.GetFloat();

				CurMaterial.mMtlFilterColor.x		= importer.GetFloat();
				CurMaterial.mMtlFilterColor.y		= importer.GetFloat();
				CurMaterial.mMtlFilterColor.z		= importer.GetFloat();

				// Static properties
				CurMaterial.mShading				= (ZCBShadingMode)importer.GetDword();
				CurMaterial.mSoften					= importer.GetByte()!=0;
				CurMaterial.mFaceMap				= importer.GetByte()!=0;
				CurMaterial.mTwoSided				= importer.GetByte()!=0;
				CurMaterial.mWire					= importer.GetByte()!=0;
				CurMaterial.mWireUnits				= importer.GetByte()!=0;
				CurMaterial.mFalloffOut				= importer.GetByte()!=0;
				CurMaterial.mTransparencyType		= (ZCBTranspaType)importer.GetDword();

				// Dynamic properties
				CurMaterial.mShininess				= importer.GetFloat();
				CurMaterial.mShiningStrength		= importer.GetFloat();
				CurMaterial.mSelfIllum				= importer.GetFloat();
				CurMaterial.mOpacity				= importer.GetFloat();
				CurMaterial.mOpaFalloff				= importer.GetFloat();
				CurMaterial.mWireSize				= importer.GetFloat();
				CurMaterial.mIOR					= importer.GetFloat();

				CurMaterial.mBounce					= importer.GetFloat();
				CurMaterial.mStaticFriction			= importer.GetFloat();
				CurMaterial.mSlidingFriction		= importer.GetFloat();

				// Cropping values & texture matrix
				if(mMATLVersion>=2)	ImportCroppingValues(CurMaterial.mCValues, CurMaterial.mTMtx, importer, CurMaterial.mWrapU, CurMaterial.mMirrorU, CurMaterial.mWrapV, CurMaterial.mMirrorV);

				// Extended self-illum
				if(mMATLVersion>=3)
				{
					CurMaterial.mSelfIllumOn		= importer.GetByte()!=0;
					CurMaterial.mSelfIllumValue		= importer.GetFloat();
					CurMaterial.mSelfIllumColor.x	= importer.GetFloat();
					CurMaterial.mSelfIllumColor.y	= importer.GetFloat();
					CurMaterial.mSelfIllumColor.z	= importer.GetFloat();
				}

				// Flexporter specific
				if(mMATLVersion>=5)
				{
//Log("Shader address: %d\n", importer.GetAddress());
					const char* ShaderFile = (const char*)importer.GetString();
//Log("UserProps address: %d\n", importer.GetAddress());
					const char* UserProps = (const char*)importer.GetString();
/*Log("Next address: %d\n", importer.GetAddress());
char Buf[21];
ubyte* Ad = (ubyte*)importer.GetAddress();
for(int ppp=0;ppp<20;ppp++)	Buf[ppp] = Ad[ppp];
Buf[20]=0;
Log("Next bytes: %s\n", Buf);
*/
					CurMaterial.mShaderFile.Set(ShaderFile);
					CurMaterial.mUserProps.Set(UserProps);
//Log("%s - %s\n", CurMaterial.mShaderFile, CurMaterial.mUserProps);
				}

				if(mMATLVersion>=6)
				{
					CurMaterial.mDecalMapID = importer.GetDword();
					CurMaterial.mDetailMapID = importer.GetDword();
				}
				if(mMATLVersion>=7)
				{
					CurMaterial.mBillboardMapID = importer.GetDword();
				}

				// Call the app
				NewMaterial(CurMaterial);

				UpdateProgressBar1();
			}
		}
		else
		{
			ZCBImportError("Chunk MATL not found!", ZCB_ERROR_CHUNK_NOT_FOUND);
			return false;
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the textures.
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportTextures(CustomArray& importer)
{
#ifdef ZCB_PROGRESS_BAR
	gParams->mText = "Importing textures...";
	gParams->mRequestCode = RQ_SET_TEXT_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);

	gParams->mMin = 0;
	gParams->mMax = mNbTextures;
	gParams->mRequestCode = RQ_SET_RANGE_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);
#endif
	// Are there any textures to import?
	if(mNbTextures)
	{
		// Find TEXM chunk
		if(importer.GetChunk(mZCBGlobalVersion ? "TEXMCHUNK" : "TEXM"))
		{
			// Get version number back
			mTEXMVersion	= importer.GetDword();
			ZCB_CHECKVERSION(mTEXMVersion, CHUNK_TEXM_VER, "TEXM");

			// Log message
			ZCBLog("Importing %d textures...\n", mNbTextures);

			// Import all textures
			for(udword n=0;n<mNbTextures;n++)
			{
				// Fill a texture structure
				ZCBTextureInfo CurTexture;

				// Database information
				CurTexture.mName.Set((const char*)importer.GetString());	// Texture path
				CurTexture.mID = importer.GetDword();						// Texture ID
				PatchTextureID(CurTexture.mID);

				// Get bitmap data
				ubyte Code = 1;	// Default for version <=1
				if(mTEXMVersion>1)	Code = importer.GetByte();
//				CurTexture.mIsBitmapIncluded = Version>1 ? (importer.GetByte()!=0) : true;
				CurTexture.mIsBitmapIncluded = Code!=0;

				if(Code)
				{
					// Get texture information back
					CurTexture.mWidth		= importer.GetDword();
					CurTexture.mHeight		= importer.GetDword();
					CurTexture.mHasAlpha	= importer.GetByte()!=0;

					// Get bytes for a RGBA texture
					CurTexture.mBitmap		= new ubyte[CurTexture.mWidth*CurTexture.mHeight*4];
					ZCB_CHECKALLOC(CurTexture.mBitmap);

					if(Code==1)
					{
						// => RGBA texture
						for(udword i=0;i<CurTexture.mWidth*CurTexture.mHeight;i++)
						{
							CurTexture.mBitmap[i*4+0] = importer.GetByte();	// Red
							CurTexture.mBitmap[i*4+1] = importer.GetByte();	// Green
							CurTexture.mBitmap[i*4+2] = importer.GetByte();	// Blue
							CurTexture.mBitmap[i*4+3] = CurTexture.mHasAlpha ? importer.GetByte() : PIXEL_OPAQUE;
						}
					}
					else
					{
						// => Quantized RGB texture
						ubyte Palette[768];
						for(udword i=0;i<768;i++)	Palette[i] = importer.GetByte();
						//
						for(udword i=0;i<CurTexture.mWidth*CurTexture.mHeight;i++)
						{
							ubyte ColorIndex = importer.GetByte();
							CurTexture.mBitmap[i*4+0] = Palette[ColorIndex*3+0];
							CurTexture.mBitmap[i*4+1] = Palette[ColorIndex*3+1];
							CurTexture.mBitmap[i*4+2] = Palette[ColorIndex*3+2];
							CurTexture.mBitmap[i*4+3] = PIXEL_OPAQUE;
						}
					}
				}

				// Cropping values & texture matrix
				ImportCroppingValues(CurTexture.mCValues, CurTexture.mTMtx, importer, CurTexture.mWrapU, CurTexture.mMirrorU, CurTexture.mWrapV, CurTexture.mMirrorV);

				// Call the app
				NewTexture(CurTexture);

				UpdateProgressBar1();
			}
		}
		else
		{
			ZCBImportError("Chunk TEXM not found!", ZCB_ERROR_CHUNK_NOT_FOUND);
			return false;
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the meshes.
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportMeshes(CustomArray& importer)
{
	// Are there any meshes to import?
	udword Total = mNbMeshes + mNbDerived;

#ifdef ZCB_PROGRESS_BAR
	gParams->mText = "Importing meshes...";
	gParams->mRequestCode = RQ_SET_TEXT_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);

	gParams->mMin = 0;
	gParams->mMax = Total;
	gParams->mRequestCode = RQ_SET_RANGE_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);
#endif
	if(Total)
	{
		// Find MESH chunk
		if(importer.GetChunk(mZCBGlobalVersion ? "MESHCHUNK" : "MESH"))
		{
			// Get version number back
			mMESHVersion	= importer.GetDword();
			ZCB_CHECKVERSION(mMESHVersion, CHUNK_MESH_VER, "MESH");

			// Log message
			ZCBLog("Importing %d meshes...\n", Total);

			// Import all meshes
			for(udword n=0;n<Total;n++)
			{
				// Fill a mesh structure
				ZCBMeshInfo CurMesh;

				// Base info
				CurMesh.Import(importer);

				// Get mesh information back
				CurMesh.mIsCollapsed	= importer.GetByte()!=0;	// true if the object has been collapsed
				CurMesh.mIsSkeleton		= importer.GetByte()!=0;	// true for BIPED parts
				CurMesh.mIsInstance		= importer.GetByte()!=0;	// true for instances
				CurMesh.mIsTarget		= importer.GetByte()!=0;	// true for target objects
				CurMesh.mIsConvertible	= importer.GetByte()!=0;	// true for valid objects
				CurMesh.mIsSkin			= importer.GetByte()!=0;	// true for PHYSIQUE skins
				if(mMESHVersion>=4)
					CurMesh.mCastShadows= importer.GetByte()!=0;	// true if the mesh can cast its shadow

				if(mMESHVersion>=9)
				{
					CurMesh.mReceiveShadows	= importer.GetByte()!=0;
					CurMesh.mMotionBlur		= importer.GetByte()!=0;
				}

				// Get skin's character ID
				if(mMESHVersion>=5 && CurMesh.mIsSkin)
				{
					CurMesh.mCharID		= importer.GetDword();		// the owner's character ID
				}

				// Get BIPED parts information if needed
				if(CurMesh.mIsSkeleton)
				{
					CurMesh.mCharID		= importer.GetDword();		// the owner's character ID
					CurMesh.mCSID		= importer.GetDword();		// the CSID
				}

				// Get data back for non-instance meshes
				if(!CurMesh.mIsInstance)
				{
					// Get primitive parameters
					if(mMESHVersion>=8)
					{
						udword PrimitiveType = importer.GetDword();
						switch(PrimitiveType)
						{
							case ZCB_PRIM_BOX:
							{
								ZCBBoxParams* BP = new ZCBBoxParams;
								CurMesh.mPrimParams = BP;
								BP->mLength	= importer.GetFloat();
								BP->mWidth	= importer.GetFloat();
								BP->mHeight	= importer.GetFloat();
								BP->mWSegs	= importer.GetDword();
								BP->mLSegs	= importer.GetDword();
								BP->mHSegs	= importer.GetDword();
								BP->mGenUVS	= importer.GetDword();
							}
							break;

							case ZCB_PRIM_SPHERE:
							{
								ZCBSphereParams* SP = new ZCBSphereParams;
								CurMesh.mPrimParams = SP;
								SP->mRadius		= importer.GetFloat();
								SP->mSegments	= importer.GetDword();
								SP->mSmooth		= importer.GetDword();
								SP->mHemisphere	= importer.GetFloat();
								SP->mSquash		= importer.GetDword();
								SP->mSliceFrom	= importer.GetFloat();
								SP->mSliceTo	= importer.GetFloat();
								SP->mSliceOn	= importer.GetDword();
								SP->mRecenter	= importer.GetDword();
								SP->mGenUVS		= importer.GetDword();
							}
							break;

							case ZCB_PRIM_GEOSPHERE:
							{
								ZCBGeosphereParams* GP = new ZCBGeosphereParams;
								CurMesh.mPrimParams = GP;
								GP->mRadius		= importer.GetFloat();
								GP->mSegments	= importer.GetDword();
								GP->mGenType	= importer.GetDword();
								GP->mHemisphere	= importer.GetDword();
								GP->mSmooth		= importer.GetDword();
								GP->mRecenter	= importer.GetDword();
								GP->mGenUVS		= importer.GetDword();
							}
							break;

							case ZCB_PRIM_CYLINDER:
							{
								ZCBCylinderParams* CP = new ZCBCylinderParams;
								CurMesh.mPrimParams = CP;
								CP->mRadius		= importer.GetFloat();
								CP->mHeight		= importer.GetFloat();
								CP->mHSegs		= importer.GetDword();
								CP->mCapSegs	= importer.GetDword();
								CP->mSides		= importer.GetDword();
								CP->mSmooth		= importer.GetDword();
								CP->mSliceOn	= importer.GetDword();
								CP->mSliceFrom	= importer.GetFloat();
								CP->mSliceTo	= importer.GetFloat();
								CP->mGenUVS		= importer.GetDword();
							}
							break;

							case ZCB_PRIM_CONE:
							{
								ZCBConeParams* CP = new ZCBConeParams;
								CurMesh.mPrimParams = CP;
								CP->mRadius1	= importer.GetFloat();
								CP->mRadius2	= importer.GetFloat();
								CP->mHeight		= importer.GetFloat();
								CP->mHSegs		= importer.GetDword();
								CP->mCapSegs	= importer.GetDword();
								CP->mSides		= importer.GetDword();
								CP->mSmooth		= importer.GetDword();
								CP->mSliceOn	= importer.GetDword();
								CP->mSliceFrom	= importer.GetFloat();
								CP->mSliceTo	= importer.GetFloat();
								CP->mGenUVS		= importer.GetDword();
							}
							break;

							case ZCB_PRIM_TORUS:
							{
								ZCBTorusParams* TP = new ZCBTorusParams;
								CurMesh.mPrimParams = TP;
								TP->mRadius1	= importer.GetFloat();
								TP->mRadius2	= importer.GetFloat();
								TP->mRotation	= importer.GetFloat();
								TP->mTwist		= importer.GetFloat();
								TP->mSegments	= importer.GetDword();
								TP->mSides		= importer.GetDword();
								TP->mSmooth		= importer.GetDword();
								TP->mSliceOn	= importer.GetDword();
								TP->mSliceFrom	= importer.GetFloat();
								TP->mSliceTo	= importer.GetFloat();
								TP->mGenUVS		= importer.GetDword();
							}
							break;

							case ZCB_PRIM_TUBE:
							{
								ZCBTubeParams* TP = new ZCBTubeParams;
								CurMesh.mPrimParams = TP;
								TP->mRadius1	= importer.GetFloat();
								TP->mRadius2	= importer.GetFloat();
								TP->mHeight		= importer.GetFloat();
								TP->mSegments	= importer.GetDword();
								TP->mCapSegs	= importer.GetDword();
								TP->mSides		= importer.GetDword();
								TP->mSmooth		= importer.GetDword();
								TP->mSliceOn	= importer.GetDword();
								TP->mSliceFrom	= importer.GetFloat();
								TP->mSliceTo	= importer.GetFloat();
								TP->mGenUVS		= importer.GetDword();
							}
							break;

							case ZCB_PRIM_TEAPOT:
							{
							}
							break;

							case ZCB_PRIM_PLANE:
							{
								ZCBPlaneParams* PP = new ZCBPlaneParams;
								CurMesh.mPrimParams = PP;
								PP->mLength		= importer.GetFloat();
								PP->mWidth		= importer.GetFloat();
								PP->mWidthSegs	= importer.GetDword();
								PP->mLengthSegs	= importer.GetDword();
								PP->mDensity	= importer.GetFloat();
								PP->mScale		= importer.GetFloat();
								PP->mGenUVS		= importer.GetDword();
							}
							break;

							case ZCB_PRIM_HEDRA:
							{
								ZCBHedraParams* HP = new ZCBHedraParams;
								CurMesh.mPrimParams = HP;
								HP->mRadius		= importer.GetFloat();
								HP->mFamily		= importer.GetDword();
								HP->mP			= importer.GetFloat();
								HP->mQ			= importer.GetFloat();
								HP->mScaleP		= importer.GetFloat();
								HP->mScaleQ		= importer.GetFloat();
								HP->mScaleR		= importer.GetFloat();
								HP->mVertices	= importer.GetDword();
								HP->mGenUVS		= importer.GetDword();
							}
							break;

							case ZCB_PRIM_CAPSULE:
							{
								ZCBCapsuleParams* CP = new ZCBCapsuleParams;
								CurMesh.mPrimParams = CP;
								CP->mRadius		= importer.GetFloat();
								CP->mHeight		= importer.GetFloat();
								CP->mCenters	= importer.GetDword();
								CP->mSides		= importer.GetDword();
								CP->mHSegs		= importer.GetDword();
								CP->mSmooth		= importer.GetDword();
								CP->mSliceOn	= importer.GetDword();
								CP->mSliceFrom	= importer.GetFloat();
								CP->mSliceTo	= importer.GetFloat();
								CP->mGenUVS		= importer.GetDword();
							}
							break;
						}
					}

					CurMesh.mNbFaces	= importer.GetDword();		// Number of faces
					CurMesh.mNbVerts	= importer.GetDword();		// Number of vertices
					CurMesh.mNbTVerts	= importer.GetDword();		// Number of texture-vertices
					CurMesh.mNbCVerts	= importer.GetDword();		// Number of vertex-colors
					CurMesh.mFlags		= importer.GetDword();		// Flags
					CurMesh.mParity		= importer.GetByte()!=0;	// Mesh parity

					// Get data for skins / non-skins
					if(!CurMesh.mIsSkin)	ImportVertices			(CurMesh, importer);
					else					ImportSkinData			(CurMesh, importer);

					// Native texture vertices
											ImportTextureVertices	(CurMesh, importer);

					// Native vertex-colors
											ImportVertexColors		(CurMesh, importer);

					// Native faces
											ImportFaces				(CurMesh, importer);

					// Extra stuff
											ImportExtraStuff		(CurMesh, importer);

					// Consolidated mesh
											ImportConsolidated		(CurMesh, importer);
				}

				// Lightmapper data
				ImportLightingData(CurMesh, importer);

				// Call the app
				NewMesh(CurMesh);

				// Free consolidation ram
				if(CurMesh.mCleanMesh)
				{
					MBMaterials& Mtl = CurMesh.mCleanMesh->Materials;
					DELETEARRAY(Mtl.MaterialInfo);
					MBGeometry& Geo = CurMesh.mCleanMesh->Geometry;
					DELETEARRAY(Geo.NormalInfo);
					DELETEARRAY(Geo.CVerts);
					DELETEARRAY(Geo.Normals);
					DELETEARRAY(Geo.TVerts);
					DELETEARRAY(Geo.TVertsRefs);
					DELETEARRAY(Geo.Verts);
					DELETEARRAY(Geo.VertsRefs);
					MBTopology& Topo = CurMesh.mCleanMesh->Topology;
					DELETEARRAY(Topo.Normals);
					DELETEARRAY(Topo.VRefs);
					DELETEARRAY(Topo.FacesInSubmesh);
					DELETEARRAY(Topo.SubmeshProperties);

					DELETESINGLE(CurMesh.mCleanMesh);
				}

				UpdateProgressBar1();
			}
		}
		else
		{
			ZCBImportError("Chunk MESH not found!", ZCB_ERROR_CHUNK_NOT_FOUND);
			return false;
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the vertices.
 *	\param		curmesh			[in] current mesh structure
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportVertices(ZCBMeshInfo& curmesh, CustomArray& importer)
{
	// Checkings
	if(!curmesh.mNbVerts)	return true;

	// Get some bytes for vertices
	curmesh.mVerts = new Point[curmesh.mNbVerts];
	ZCB_CHECKALLOC(curmesh.mVerts);

	// Get vertices back
	if(curmesh.mFlags&ZCB_QUANTIZED_VERTICES)
	{
		// Get dequantization coeffs
		float DequantCoeffX = importer.GetFloat();
		float DequantCoeffY = importer.GetFloat();
		float DequantCoeffZ = importer.GetFloat();
		// Get quantized vertices
		for(udword i=0;i<curmesh.mNbVerts;i++)
		{
			sword x = importer.GetWord();
			sword y = importer.GetWord();
			sword z = importer.GetWord();
			// Dequantize
			curmesh.mVerts[i].x = float(x) * DequantCoeffX;
			curmesh.mVerts[i].y = float(y) * DequantCoeffY;
			curmesh.mVerts[i].z = float(z) * DequantCoeffZ;
		}
	}
	else
	{
		// Get vertices
		for(udword i=0;i<curmesh.mNbVerts;i++)
		{
			float x = importer.GetFloat();
			float y = importer.GetFloat();
			float z = importer.GetFloat();
			curmesh.mVerts[i].x = x;
			curmesh.mVerts[i].y = y;
			curmesh.mVerts[i].z = z;
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the skin data.
 *	\param		curmesh			[in] current mesh structure
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportSkinData(ZCBMeshInfo& curmesh, CustomArray& importer)
{
	// A skin can be a simple or a complex skin. (err, that's *my* wordlist...)
	// - simple: each vertex is linked to a single bone
	// - complex: each vertex is linked to many bones
	if(curmesh.mFlags & ZCB_ONE_BONE_PER_VERTEX)
	{
		// The skin has one bone/vertex. Hence:
		// - We have N vertices and N bones
		// - N offset vectors
		// - N bones ID
		// - mBonesNb remains null
		// - mWeights remains null

		// Get offset vectors back
		curmesh.mOffsetVectors	= new Point[curmesh.mNbVerts];
		ZCB_CHECKALLOC(curmesh.mOffsetVectors);

		for(udword i=0;i<curmesh.mNbVerts;i++)
		{
			curmesh.mOffsetVectors[i].x = importer.GetFloat();
			curmesh.mOffsetVectors[i].y = importer.GetFloat();
			curmesh.mOffsetVectors[i].z = importer.GetFloat();

			// Fix what was missing in Flexporter before v1.13
			if(curmesh.mD3DCompliant && mMESHVersion<6)	TSwap(curmesh.mOffsetVectors[i].y, curmesh.mOffsetVectors[i].z);
		}

		// Get bones ID back
		curmesh.mBonesID		= new udword[curmesh.mNbVerts];
		ZCB_CHECKALLOC(curmesh.mBonesID);

		for(udword i=0;i<curmesh.mNbVerts;i++)
		{
			curmesh.mBonesID[i] = importer.GetDword();
		}
	}
	else
	{
		// The skin has many bones/vertex. Hence:
		// - We have N vertices and M bones
		// - We have N numbers of bones, stored in mBonesNb
		// - M is the sum of all those number of bones
		// - We have M offset vectors
		// - We have M bones ID
		// - We have M weights stored in mWeights

		// Get number of bones / vertex, compute total number of bones
		curmesh.mBonesNb		= new udword[curmesh.mNbVerts];
		ZCB_CHECKALLOC(curmesh.mBonesNb);

		udword Sum=0;
		for(udword i=0;i<curmesh.mNbVerts;i++)
		{
			curmesh.mBonesNb[i] = importer.GetDword();
			Sum+=curmesh.mBonesNb[i];
		}

		// Get bones ID back
		curmesh.mBonesID		= new udword[Sum];
		ZCB_CHECKALLOC(curmesh.mBonesID);

		for(udword i=0;i<Sum;i++)
		{
			curmesh.mBonesID[i] = importer.GetDword();
		}

		// Get weights back
		curmesh.mWeights		= new float[Sum];
		ZCB_CHECKALLOC(curmesh.mWeights);

		for(udword i=0;i<Sum;i++)
		{
			curmesh.mWeights[i] = importer.GetFloat();
		}

		// Get offset vectors back
		curmesh.mOffsetVectors	= new Point[Sum];
		ZCB_CHECKALLOC(curmesh.mOffsetVectors);

		for(udword i=0;i<Sum;i++)
		{
			curmesh.mOffsetVectors[i].x = importer.GetFloat();
			curmesh.mOffsetVectors[i].y = importer.GetFloat();
			curmesh.mOffsetVectors[i].z = importer.GetFloat();

			// Fix what was missing in Flexporter before v1.13
			if(curmesh.mD3DCompliant && mMESHVersion<6)	TSwap(curmesh.mOffsetVectors[i].y, curmesh.mOffsetVectors[i].z);
		}
	}

	// Get skeletal information back. This just gives the skeleton structure in a simple way, so that you
	// can discard the actual BIPED parts and still use the skin (=derived object) alone.
	curmesh.mNbBones = importer.GetDword();

	curmesh.mSkeleton = new BasicBone[curmesh.mNbBones];
	ZCB_CHECKALLOC(curmesh.mSkeleton);

	for(udword i=0;i<curmesh.mNbBones;i++)
	{
		curmesh.mSkeleton[i].CSID	= importer.GetDword();	// CSID
		curmesh.mSkeleton[i].pCSID	= importer.GetDword();	// parent's CSID
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the texture-vertices.
 *	\param		curmesh			[in] current mesh structure
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportTextureVertices(ZCBMeshInfo& curmesh, CustomArray& importer)
{
	// Checkings
	if(!curmesh.mNbTVerts)	return true;

	if(curmesh.mFlags & ZCB_UVW)
	{
		// Get some bytes for texture vertices
		curmesh.mTVerts = new Point[curmesh.mNbTVerts];
		ZCB_CHECKALLOC(curmesh.mTVerts);

		// Get texture-vertices back
		if(curmesh.mFlags&ZCB_QUANTIZED_VERTICES)
		{
			// Get dequantization coeffs
			float DequantCoeffX = importer.GetFloat();
			float DequantCoeffY = importer.GetFloat();
			float DequantCoeffZ = (curmesh.mFlags & ZCB_W_DISCARDED) ? 0.0f : importer.GetFloat();

			// Get quantized vertices
			for(udword i=0;i<curmesh.mNbTVerts;i++)
			{
				sword x = importer.GetWord();
				sword y = importer.GetWord();
				sword z = (curmesh.mFlags & ZCB_W_DISCARDED) ? 0 : importer.GetWord();
				// Dequantize
				curmesh.mTVerts[i].x = float(x) * DequantCoeffX;
				curmesh.mTVerts[i].y = float(y) * DequantCoeffY;
				curmesh.mTVerts[i].z = float(z) * DequantCoeffZ;
			}
		}
		else
		{
			// Get texture-vertices
			for(udword i=0;i<curmesh.mNbTVerts;i++)
			{
				curmesh.mTVerts[i].x = importer.GetFloat();
				curmesh.mTVerts[i].y = importer.GetFloat();
				curmesh.mTVerts[i].z = (curmesh.mFlags & ZCB_W_DISCARDED) ? 0.0f : importer.GetFloat();
			}
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the vertex-colors.
 *	\param		curmesh			[in] current mesh structure
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportVertexColors(ZCBMeshInfo& curmesh, CustomArray& importer)
{
	// Checkings
	if(!curmesh.mNbCVerts)	return true;

	if(curmesh.mFlags & ZCB_VERTEX_COLORS)
	{
		// Get some bytes for vertex-colors
		curmesh.mCVerts = new Point[curmesh.mNbCVerts];
		ZCB_CHECKALLOC(curmesh.mCVerts);

		// Get vertex-colors back
		if(curmesh.mFlags&ZCB_QUANTIZED_VERTICES)
		{
			// Get dequantization coeffs
			float DequantCoeffX = importer.GetFloat();
			float DequantCoeffY = importer.GetFloat();
			float DequantCoeffZ = importer.GetFloat();

			// Get quantized vertices
			for(udword i=0;i<curmesh.mNbCVerts;i++)
			{
				sword x = importer.GetWord();
				sword y = importer.GetWord();
				sword z = importer.GetWord();
				// Dequantize
				curmesh.mCVerts[i].x = float(x) * DequantCoeffX;
				curmesh.mCVerts[i].y = float(y) * DequantCoeffY;
				curmesh.mCVerts[i].z = float(z) * DequantCoeffZ;
			}
		}
		else
		{
			// Get vertex-colors
			for(udword i=0;i<curmesh.mNbCVerts;i++)
			{
				curmesh.mCVerts[i].x = importer.GetFloat();
				curmesh.mCVerts[i].y = importer.GetFloat();
				curmesh.mCVerts[i].z = importer.GetFloat();
			}
		}

		// Vertex alpha
		if(curmesh.mFlags&ZCB_VERTEX_ALPHA)
		{
			curmesh.mVertexAlpha = new float[curmesh.mNbVerts];
			for(udword i=0;i<curmesh.mNbVerts;i++)
			{
				curmesh.mVertexAlpha[i] = importer.GetFloat();
			}
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the topologies.
 *	\param		curmesh			[in] current mesh structure
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportFaces(ZCBMeshInfo& curmesh, CustomArray& importer)
{
	// Get number of faces
	udword NbFaces = curmesh.mNbFaces;

	// Get some bytes for faces
	if(NbFaces)
	{
		if(curmesh.mNbVerts)	{ curmesh.mFaces = new udword[NbFaces*3];	ZCB_CHECKALLOC(curmesh.mFaces); }
		if(curmesh.mNbTVerts)	{ curmesh.mTFaces = new udword[NbFaces*3];	ZCB_CHECKALLOC(curmesh.mTFaces); }
		if(curmesh.mNbCVerts)	{ curmesh.mCFaces = new udword[NbFaces*3];	ZCB_CHECKALLOC(curmesh.mCFaces); }
	}

	// Get faces
	if(curmesh.mFaces)
		for(udword j=0;j<NbFaces*3;j++)
			curmesh.mFaces[j] = (curmesh.mFlags&ZCB_WORD_FACES) ? (sword)importer.GetWord() : importer.GetDword();

	// Get texture faces
	if(curmesh.mTFaces)
		for(udword j=0;j<NbFaces*3;j++)
			curmesh.mTFaces[j] = (curmesh.mFlags&ZCB_WORD_FACES) ? (sword)importer.GetWord() : importer.GetDword();

	// Get color faces
	if(curmesh.mCFaces)
		for(udword j=0;j<NbFaces*3;j++)
			curmesh.mCFaces[j] = (curmesh.mFlags&ZCB_WORD_FACES) ? (sword)importer.GetWord() : importer.GetDword();

	// Get face properties
	if(NbFaces)
	{
		curmesh.mFaceProperties = new FaceProperties[NbFaces];
		ZCB_CHECKALLOC(curmesh.mFaceProperties);
	}

	udword j;
	for(j=0;j<NbFaces;j++)	{ curmesh.mFaceProperties[j].MatID	= importer.GetDword();	PatchMaterialID(curmesh.mFaceProperties[j].MatID);	}
	for(j=0;j<NbFaces;j++)	curmesh.mFaceProperties[j].Smg		= importer.GetDword();
	for(j=0;j<NbFaces;j++)	curmesh.mFaceProperties[j].EdgeVis	= (curmesh.mFlags&ZCB_EDGE_VIS) ? importer.GetByte() : 0;

	// Undo delta compression
	if(curmesh.mFlags&ZCB_COMPRESSED)
	{
		if(curmesh.mFaces)	UnDelta(curmesh.mFaces, NbFaces*3, 4);
		if(curmesh.mTFaces)	UnDelta(curmesh.mTFaces, NbFaces*3, 4);
		if(curmesh.mCFaces)	UnDelta(curmesh.mCFaces, NbFaces*3, 4);
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the volume integrals and other extra stuff.
 *	\param		curmesh			[in] current mesh structure
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportExtraStuff(ZCBMeshInfo& curmesh, CustomArray& importer)
{
	// Get convex hull back
	if(curmesh.mFlags & ZCB_CONVEX_HULL)
	{
		curmesh.mNbHullVerts = importer.GetDword();
		curmesh.mNbHullFaces = importer.GetDword();

		// Get hull vertices
		curmesh.mHullVerts	= new Point[curmesh.mNbHullVerts];
		ZCB_CHECKALLOC(curmesh.mHullVerts);

		for(udword i=0;i<curmesh.mNbHullVerts;i++)
		{
			curmesh.mHullVerts[i].x = importer.GetFloat();
			curmesh.mHullVerts[i].y = importer.GetFloat();
			curmesh.mHullVerts[i].z = importer.GetFloat();
		}

		// Get hull faces
		curmesh.mHullFaces	= new udword[curmesh.mNbHullFaces*3];
		ZCB_CHECKALLOC(curmesh.mHullFaces);

		for(udword i=0;i<curmesh.mNbHullFaces;i++)
		{
			curmesh.mHullFaces[i*3+0] = importer.GetDword();
			curmesh.mHullFaces[i*3+1] = importer.GetDword();
			curmesh.mHullFaces[i*3+2] = importer.GetDword();
		}
	}

	// Get bounding sphere back
	if(curmesh.mFlags & ZCB_BOUNDING_SPHERE)
	{
		curmesh.mBSCenter.x = importer.GetFloat();
		curmesh.mBSCenter.y = importer.GetFloat();
		curmesh.mBSCenter.z = importer.GetFloat();
		curmesh.mBSRadius	= importer.GetFloat();
	}

	// Get volume integrals back
	if(curmesh.mFlags & ZCB_INERTIA_TENSOR)
	{
		// Center of mass
		curmesh.mCOM.x = importer.GetFloat();
		curmesh.mCOM.y = importer.GetFloat();
		curmesh.mCOM.z = importer.GetFloat();

		// Mass
		curmesh.mComputedMass = importer.GetFloat();

		// Inertia tensor
		curmesh.mInertiaTensor[0][0] = importer.GetFloat();
		curmesh.mInertiaTensor[0][1] = importer.GetFloat();
		curmesh.mInertiaTensor[0][2] = importer.GetFloat();
		curmesh.mInertiaTensor[1][0] = importer.GetFloat();
		curmesh.mInertiaTensor[1][1] = importer.GetFloat();
		curmesh.mInertiaTensor[1][2] = importer.GetFloat();
		curmesh.mInertiaTensor[2][0] = importer.GetFloat();
		curmesh.mInertiaTensor[2][1] = importer.GetFloat();
		curmesh.mInertiaTensor[2][2] = importer.GetFloat();

		// COM inertia tensor
		curmesh.mCOMInertiaTensor[0][0] = importer.GetFloat();
		curmesh.mCOMInertiaTensor[0][1] = importer.GetFloat();
		curmesh.mCOMInertiaTensor[0][2] = importer.GetFloat();
		curmesh.mCOMInertiaTensor[1][0] = importer.GetFloat();
		curmesh.mCOMInertiaTensor[1][1] = importer.GetFloat();
		curmesh.mCOMInertiaTensor[1][2] = importer.GetFloat();
		curmesh.mCOMInertiaTensor[2][0] = importer.GetFloat();
		curmesh.mCOMInertiaTensor[2][1] = importer.GetFloat();
		curmesh.mCOMInertiaTensor[2][2] = importer.GetFloat();
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the consolidation results.
 *	\param		curmesh			[in] current mesh structure
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportConsolidated(ZCBMeshInfo& curmesh, CustomArray& importer)
{
	// Get consolidated mesh back
	if(curmesh.mFlags & ZCB_CONSOLIDATION)
	{
		curmesh.mCleanMesh = new MBResult;
		ZCB_CHECKALLOC(curmesh.mCleanMesh);

		// Get topology back
		{
			MBTopology& Topo = curmesh.mCleanMesh->Topology;

			if(mMESHVersion<7)
			{
				Topo.NbFaces		= importer.GetWord();
				Topo.NbSubmeshes	= importer.GetWord();
			}
			else
			{
				Topo.NbFaces		= importer.GetDword();
				Topo.NbSubmeshes	= importer.GetDword();
			}

			// Submeshes
			Topo.SubmeshProperties = new MBSubmesh[Topo.NbSubmeshes];
			ZCB_CHECKALLOC(Topo.SubmeshProperties);

			for(udword i=0;i<Topo.NbSubmeshes;i++)
			{
				MBSubmesh* CurSM = &Topo.SubmeshProperties[i];

				CurSM->MatID		= (sdword)importer.GetDword();
				PatchMaterialID(CurSM->MatID);
				CurSM->SmGrp		= importer.GetDword();
				CurSM->NbFaces		= importer.GetDword();
				CurSM->NbVerts		= importer.GetDword();
				CurSM->NbSubstrips	= importer.GetDword();
			}

			// Connectivity
			Topo.FacesInSubmesh = new udword[Topo.NbSubmeshes];
			ZCB_CHECKALLOC(Topo.FacesInSubmesh);

			Topo.VRefs = new udword[Topo.NbFaces*3];
			ZCB_CHECKALLOC(Topo.VRefs);

			udword* VRefs = Topo.VRefs;
			for(udword i=0;i<Topo.NbSubmeshes;i++)
			{
				if(mMESHVersion<7)	Topo.FacesInSubmesh[i] = importer.GetWord();
				else				Topo.FacesInSubmesh[i] = importer.GetDword();

				for(udword j=0;j<Topo.FacesInSubmesh[i];j++)
				{
					if(mMESHVersion<7)
					{
						*VRefs++ = importer.GetWord();
						*VRefs++ = importer.GetWord();
						*VRefs++ = importer.GetWord();
					}
					else
					{
						*VRefs++ = importer.GetDword();
						*VRefs++ = importer.GetDword();
						*VRefs++ = importer.GetDword();
					}
				}
			}

			// Face normals
			if(curmesh.mFlags & ZCB_FACE_NORMALS)
			{
				Topo.Normals = new float[Topo.NbFaces*3];
				ZCB_CHECKALLOC(Topo.Normals);

				for(udword i=0;i<Topo.NbFaces;i++)
				{
					Topo.Normals[i*3+0] = importer.GetFloat();
					Topo.Normals[i*3+1] = importer.GetFloat();
					Topo.Normals[i*3+2] = importer.GetFloat();
				}
			}
		}

		// Get geometry back
		{
			MBGeometry& Geo = curmesh.mCleanMesh->Geometry;

			if(mMESHVersion<7)
			{
				Geo.NbGeomPts	= importer.GetWord();
				Geo.NbVerts		= importer.GetWord();
				Geo.NbTVerts	= importer.GetWord();
			}
			else
			{
				Geo.NbGeomPts	= importer.GetDword();
				Geo.NbVerts		= importer.GetDword();
				Geo.NbTVerts	= importer.GetDword();
			}

			// Indexed geometry
			Geo.VertsRefs = new udword[Geo.NbVerts];
			ZCB_CHECKALLOC(Geo.VertsRefs);

			for(udword i=0;i<Geo.NbVerts;i++)
			{
				Geo.VertsRefs[i] = importer.GetDword();
			}

			// Vertices
			udword _NbVerts = importer.GetDword();
			Geo.Verts = new float[_NbVerts*3];
			ZCB_CHECKALLOC(Geo.Verts);

			for(udword i=0;i<_NbVerts;i++)
			{
				Geo.Verts[i*3+0] = importer.GetFloat();
				Geo.Verts[i*3+1] = importer.GetFloat();
				Geo.Verts[i*3+2] = importer.GetFloat();
			}

			// Indexed UVWs
			if(curmesh.mFlags & ZCB_UVW)
			{
				Geo.TVertsRefs = new udword[Geo.NbVerts];
				ZCB_CHECKALLOC(Geo.TVertsRefs);

				for(udword i=0;i<Geo.NbVerts;i++)
				{
					Geo.TVertsRefs[i] = importer.GetDword();
				}

				// UVWs
				udword _NbTVerts = importer.GetDword();
				Geo.TVerts = new float[_NbTVerts*3];
				ZCB_CHECKALLOC(Geo.TVerts);

				float* p = Geo.TVerts;
				for(udword i=0;i<_NbTVerts;i++)
				{
					*p++ = importer.GetFloat();
					*p++ = importer.GetFloat();
					if(!(curmesh.mFlags & ZCB_W_DISCARDED))	
					{
						*p++ = importer.GetFloat();
					}
				}
			}

			// Normals
			if(curmesh.mFlags & ZCB_VERTEX_NORMALS)
			{
				udword NbNormals = importer.GetDword();
				Geo.Normals = new float[NbNormals*3];
				ZCB_CHECKALLOC(Geo.Normals);

				for(udword i=0;i<NbNormals;i++)
				{
					Geo.Normals[i*3+0] = importer.GetFloat();
					Geo.Normals[i*3+1] = importer.GetFloat();
					Geo.Normals[i*3+2] = importer.GetFloat();
				}
			}

			// Vertex colors
			if(curmesh.mFlags & ZCB_VERTEX_COLORS)
			{
				udword NbVtxColors = importer.GetDword();

				if(curmesh.mFlags & ZCB_DWORD_COLORS)
				{
					Geo.CVerts = new float[NbVtxColors*3];
					ZCB_CHECKALLOC(Geo.CVerts);

					for(udword i=0;i<NbVtxColors;i++)
					{
						Geo.CVerts[i*3+0] = importer.GetFloat();	// actually a dword RGBA color
						Geo.CVerts[i*3+1] = 0.0f;
						Geo.CVerts[i*3+2] = 0.0f;
					}
				}
				else
				{
					Geo.CVerts = new float[NbVtxColors*3];
					ZCB_CHECKALLOC(Geo.CVerts);

					for(udword i=0;i<NbVtxColors;i++)
					{
						Geo.CVerts[i*3+0] = importer.GetFloat();
						Geo.CVerts[i*3+1] = importer.GetFloat();
						Geo.CVerts[i*3+2] = importer.GetFloat();
					}
				}
			}

			// NormalInfo
			if(curmesh.mFlags & ZCB_NORMAL_INFO)
			{
				Geo.NormalInfoSize = importer.GetDword();
				Geo.NormalInfo = new udword[Geo.NormalInfoSize];
				ZCB_CHECKALLOC(Geo.NormalInfo);

				for(udword i=0;i<Geo.NormalInfoSize;i++)
				{
					Geo.NormalInfo[i] = importer.GetDword();
				}
			}
		}

		// Materials
		{
			MBMaterials& Mtl = curmesh.mCleanMesh->Materials;

			Mtl.NbMtls = importer.GetDword();
			Mtl.MaterialInfo	= new MBMatInfo[Mtl.NbMtls];
			ZCB_CHECKALLOC(Mtl.MaterialInfo);

			for(udword i=0;i<Mtl.NbMtls;i++)
			{
				MBMatInfo* CurMtl = &Mtl.MaterialInfo[i];

				CurMtl->MatID		= (sdword)importer.GetDword();
				PatchMaterialID(CurMtl->MatID);
				CurMtl->NbFaces		= importer.GetDword();
				CurMtl->NbVerts		= importer.GetDword();
				CurMtl->NbSubmeshes	= importer.GetDword();
			}
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the lighting data.
 *	\param		curmesh			[in] current mesh structure
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportLightingData(ZCBMeshInfo& curmesh, CustomArray& importer)
{
	// Get number of precomputed colors
	curmesh.mNbColors = importer.GetDword();

	// Get them back if needed
	if(curmesh.mNbColors)
	{
		curmesh.mColors = new Point[curmesh.mNbColors];
		ZCB_CHECKALLOC(curmesh.mColors);

		for(udword i=0;i<curmesh.mNbColors;i++)
		{
			curmesh.mColors[i].x = importer.GetFloat();
			curmesh.mColors[i].y = importer.GetFloat();
			curmesh.mColors[i].z = importer.GetFloat();
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the controllers.
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportControllers(CustomArray& importer)
{
#ifdef ZCB_PROGRESS_BAR
	gParams->mText = "Importing controllers...";
	gParams->mRequestCode = RQ_SET_TEXT_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);

	gParams->mMin = 0;
	gParams->mMax = mNbControllers;
	gParams->mRequestCode = RQ_SET_RANGE_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);
#endif
	// Are there any controllers to import?
	if(mNbControllers)
	{
		// Find CTRL chunk
		if(importer.GetChunk(mZCBGlobalVersion ? "CTRLCHUNK" : "CTRL"))
		{
			// Get version number back
			mCTRLVersion		= importer.GetDword();
			ZCB_CHECKVERSION(mCTRLVersion, CHUNK_CTRL_VER, "CTRL");

			// Log message
			ZCBLog("Importing %d controllers...\n", mNbControllers);

			// Import all controllers
			for(udword n=0;n<mNbControllers;n++)
			{
				// Fill a controller structure
				ZCBControllerInfo CurController;

				CurController.mField.Set((const char*)importer.GetString());	// Get the field back
				CurController.mObjectID	= importer.GetDword();					// Controller's ID
				PatchControllerID(CurController.mObjectID);
				CurController.mOwnerID	= importer.GetDword();					// Owner's ID
				if(mCTRLVersion>=2)
				{
					// Needed since now we export controllers for camera/material parameters, etc
					CurController.mOwnerType = (ZCBObjType)importer.GetDword();	// Owner's type
					if(CurController.mOwnerType==ZCB_OBJ_MATERIAL)	PatchMaterialID(CurController.mOwnerID);
					if(CurController.mOwnerType==ZCB_OBJ_TEXTURE)	PatchTextureID(CurController.mOwnerID);
				}
				else
				{
					// Old versions only exported mesh controllers
					CurController.mOwnerType = ZCB_OBJ_MESH;
				}

				CurController.mCtrlType	= (ZCBCtrlType)importer.GetDword();		// Controller type
				CurController.mCtrlMode	= (ZCBCtrlMode)importer.GetDword();		// Controller mode

				if(CurController.mCtrlMode==ZCB_CTRL_SAMPLES)
				{
					if(CurController.mCtrlType==ZCB_CTRL_VERTEXCLOUD)	CurController.mNbVertices = importer.GetDword();

					CurController.mNbSamples	= importer.GetDword();
					CurController.mSamplingRate	= importer.GetDword();

					// Use built-in types only
					switch(CurController.mCtrlType)
					{
						case ZCB_CTRL_FLOAT:
						{
							float* Samples = new float[CurController.mNbSamples];
							ZCB_CHECKALLOC(Samples);
							for(udword i=0;i<CurController.mNbSamples;i++)
							{
								Samples[i]	= importer.GetFloat();
							}
							CurController.mSamples = Samples;
						}
						break;

						case ZCB_CTRL_VECTOR:
						{
							Point* Samples = (Point*)(new float[3*CurController.mNbSamples]);
							ZCB_CHECKALLOC(Samples);
							for(udword i=0;i<CurController.mNbSamples;i++)
							{
								Samples[i].x	= importer.GetFloat();
								Samples[i].y	= importer.GetFloat();
								Samples[i].z	= importer.GetFloat();
							}
							CurController.mSamples = Samples;
						}
						break;

						case ZCB_CTRL_QUAT:
						{
							Quat* Samples = (Quat*)(new float[4*CurController.mNbSamples]);
							ZCB_CHECKALLOC(Samples);
							for(udword i=0;i<CurController.mNbSamples;i++)
							{
								Samples[i].p.x	= importer.GetFloat();
								Samples[i].p.y	= importer.GetFloat();
								Samples[i].p.z	= importer.GetFloat();
								Samples[i].w	= importer.GetFloat();
							}
							CurController.mSamples = Samples;
						}
						break;

						case ZCB_CTRL_PR:
						{
							PR* Samples = (PR*)(new float[7*CurController.mNbSamples]);
							ZCB_CHECKALLOC(Samples);
							for(udword i=0;i<CurController.mNbSamples;i++)
							{
								Samples[i].mPos.x	= importer.GetFloat();
								Samples[i].mPos.y	= importer.GetFloat();
								Samples[i].mPos.z	= importer.GetFloat();
								Samples[i].mRot.p.x	= importer.GetFloat();
								Samples[i].mRot.p.y	= importer.GetFloat();
								Samples[i].mRot.p.z	= importer.GetFloat();
								Samples[i].mRot.w	= importer.GetFloat();
							}
							CurController.mSamples = Samples;
						}
						break;

						case ZCB_CTRL_PRS:
						{
							PRS* Samples = (PRS*)(new float[10*CurController.mNbSamples]);
							ZCB_CHECKALLOC(Samples);
							for(udword i=0;i<CurController.mNbSamples;i++)
							{
								Samples[i].mPos.x	= importer.GetFloat();
								Samples[i].mPos.y	= importer.GetFloat();
								Samples[i].mPos.z	= importer.GetFloat();
								Samples[i].mRot.p.x	= importer.GetFloat();
								Samples[i].mRot.p.y	= importer.GetFloat();
								Samples[i].mRot.p.z	= importer.GetFloat();
								Samples[i].mRot.w	= importer.GetFloat();
								Samples[i].mScale.x	= importer.GetFloat();
								Samples[i].mScale.y	= importer.GetFloat();
								Samples[i].mScale.z	= importer.GetFloat();
							}
							CurController.mSamples = Samples;
						}
						break;

						case ZCB_CTRL_VERTEXCLOUD:
						{
							float* Samples = new float[3*CurController.mNbSamples*CurController.mNbVertices];
							ZCB_CHECKALLOC(Samples);
							float* Pool = (float*)Samples;
							for(udword i=0;i<CurController.mNbSamples;i++)
							{
								for(udword j=0;j<CurController.mNbVertices;j++)
								{
									*Pool++	= importer.GetFloat();
									*Pool++	= importer.GetFloat();
									*Pool++	= importer.GetFloat();
								}
							}
							CurController.mSamples = Samples;
						}
						break;
					}
				}
				else return false;	// only sampling for now.....

				// Call the app
				NewController(CurController);

				UpdateProgressBar1();
			}
		}
		else
		{
			ZCBImportError("Chunk CTRL not found!", ZCB_ERROR_CHUNK_NOT_FOUND);
			return false;
		}
	}
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the shapes.
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportShapes(CustomArray& importer)
{
#ifdef ZCB_PROGRESS_BAR
	gParams->mText = "Importing shapes...";
	gParams->mRequestCode = RQ_SET_TEXT_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);

	gParams->mMin = 0;
	gParams->mMax = mNbShapes;
	gParams->mRequestCode = RQ_SET_RANGE_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);
#endif
	// Are there any shapes to import?
	if(mNbShapes)
	{
		// Find SHAP chunk
		if(importer.GetChunk(mZCBGlobalVersion ? "SHAPCHUNK" : "SHAP"))
		{
			// Get version number back
			mSHAPVersion	= importer.GetDword();
			ZCB_CHECKVERSION(mSHAPVersion, CHUNK_SHAP_VER, "SHAP");

			// Log message
			ZCBLog("Importing %d shapes...\n", mNbShapes);

			// Import all shapes
			for(udword n=0;n<mNbShapes;n++)
			{
				// Fill a shape structure
				ZCBShapeInfo CurShape;

				// Base info
				CurShape.Import(importer);

				// Get type
				if(mSHAPVersion>=3)	CurShape.mType = (ZCBShapeType)importer.GetDword();
				else			CurShape.mType = ZCB_SHAP_UNDEFINED;

				// Get shape information back
				CurShape.mNbLines	= importer.GetDword();	// Number of lines
				if(CHUNK_SHAP_VER>=2)
				{
					CurShape.mMatID = importer.GetDword();	// Material ID
					PatchMaterialID(CurShape.mMatID);
				}
				if(CurShape.mNbLines)
				{
					CurShape.mNbVerts = new udword[CurShape.mNbLines];	ZCB_CHECKALLOC(CurShape.mNbVerts);
					CurShape.mClosed = new bool[CurShape.mNbLines];	ZCB_CHECKALLOC(CurShape.mClosed);

					// Get all polylines
					CustomArray	Vertices;
					CurShape.mTotalNbVerts = 0;
					for(udword i=0;i<CurShape.mNbLines;i++)
					{
						udword NbVerts = importer.GetDword();	// Number of vertices in current line
						bool Closed = importer.GetByte()!=0;	// Closed/open status
						for(udword j=0;j<NbVerts;j++)
						{
							float x = importer.GetFloat();
							float y = importer.GetFloat();
							float z = importer.GetFloat();
							Vertices.Store(x).Store(y).Store(z);
						}
						CurShape.mNbVerts[i] = NbVerts;
						CurShape.mClosed[i] = Closed;
						CurShape.mTotalNbVerts+=NbVerts;
					}
					CurShape.mVerts = new Point[CurShape.mTotalNbVerts];
					ZCB_CHECKALLOC(CurShape.mVerts);
					CopyMemory(CurShape.mVerts, Vertices.Collapse(), CurShape.mTotalNbVerts*sizeof(Point));
				}

				// Call the app
				NewShape(CurShape);

				UpdateProgressBar1();
			}
		}
		else
		{
			ZCBImportError("Chunk SHAP not found!", ZCB_ERROR_CHUNK_NOT_FOUND);
			return false;
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Imports the helpers.
 *	\param		importer		[in] the imported array.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBBreaker::ImportHelpers(CustomArray& importer)
{
#ifdef ZCB_PROGRESS_BAR
	gParams->mText = "Importing helpers...";
	gParams->mRequestCode = RQ_SET_TEXT_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);

	gParams->mMin = 0;
	gParams->mMax = mNbHelpers;
	gParams->mRequestCode = RQ_SET_RANGE_PROGRESS_BAR1;
	GetCallbacksManager()->ExecCallbacks(ICCB_REQUEST, ICCB_REQUEST, gParams);
#endif
	// Are there any helpers to import?
	if(mNbHelpers)
	{
		// Find HELP chunk
		if(importer.GetChunk(mZCBGlobalVersion ? "HELPCHUNK" : "HELP"))
		{
			// Get version number back
			mHELPVersion	= importer.GetDword();
			ZCB_CHECKVERSION(mHELPVersion, CHUNK_HELP_VER, "HELP");

			// Log message
			ZCBLog("Importing %d helpers...\n", mNbHelpers);

			// Import all helpers
			for(udword n=0;n<mNbHelpers;n++)
			{
				// Fill a helper structure
				ZCBHelperInfo CurHelper;

				// Base info
				CurHelper.Import(importer);

				// Get helper information back
				if(mHELPVersion<2)	CurHelper.mIsGroupHead	= importer.GetByte()!=0;
				else
				{
					CurHelper.mHelperType	= (ZCBHelperType)importer.GetDword();
					CurHelper.mIsGroupHead	= importer.GetByte()!=0;

					if(CurHelper.mIsGroupHead && mHELPVersion>=3)
					{
						// Get the list of grouped objects
						udword NbGroupedObjects = importer.GetDword();
						CurHelper.mGroupedObjects.SetSize(NbGroupedObjects);
						for(udword i=0;i<NbGroupedObjects;i++)
						{
							udword ID = importer.GetDword();
							CurHelper.mGroupedObjects.Add(ID);
						}
					}

					// Get gizmo data
					switch(CurHelper.mHelperType)
					{
						case ZCB_HELPER_GIZMO_BOX:
						{
							CurHelper.mLength	= importer.GetFloat();
							CurHelper.mWidth	= importer.GetFloat();
							CurHelper.mHeight	= importer.GetFloat();
						}
						break;

						case ZCB_HELPER_GIZMO_SPHERE:
						{
							CurHelper.mRadius	= importer.GetFloat();
							CurHelper.mHemi		= importer.GetByte()!=0;
						}
						break;

						case ZCB_HELPER_GIZMO_CYLINDER:
						{
							CurHelper.mRadius	= importer.GetFloat();
							CurHelper.mHeight	= importer.GetFloat();
						}
						break;

						case ZCB_HELPER_BILLBOARD:
						{
							float Size		= importer.GetFloat();
							float Length	= importer.GetFloat();
							bool ScreenAlign = importer.GetByte()!=0;
						}
						break;
					}
				}

				// Call the app
				NewHelper(CurHelper);

				UpdateProgressBar1();
			}
		}
		else
		{
			ZCBImportError("Chunk HELP not found!", ZCB_ERROR_CHUNK_NOT_FOUND);
			return false;
		}
	}
	return true;
}
