///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the camera-related exported structures.
 *	\file		FLX_AddCamera.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_ADDCAMERA_H
#define FLX_ADDCAMERA_H

	//! Camera types from MAX.
	enum CType
	{
		FLX_CTYPE_FREE			= 0,			//!< Free camera
		FLX_CTYPE_TARGET		= 1,			//!< Target camera
		FLX_CTYPE_PARALLEL		= 2,			//!< Parallel camera

		FLX_CTYPE_FORCE_DWORD	= 0x7fffffff
	};

	//! FOV types from MAX.
	enum FOVType
	{
		FLX_FOV_HORIZONTAL		= 0,
		FLX_FOV_VERTICAL		= 1,
		FLX_FOV_DIAGONAL		= 2,

		FLX_FOV_FORCE_DWORD		= 0x7fffffff
	};

	//! The camera information class.
	class FLEXPORTER_API CameraDescriptor : public ObjectDescriptor
	{
		public:
		// Constructor / Destructor
									CameraDescriptor();
		virtual						~CameraDescriptor();

		// Parameters
				bool				mOrthoCam;			//!< Camera type: ortographic (true) or perspective (false)
				float				mFOV;				//!< Field-Of-View (degrees) or Width for ortho cams		[Animatable]
				float				mNearClip;			//!< Near/hither clip										[Animatable]
				float				mFarClip;			//!< Far/yon clip											[Animatable]
				float				mTDist;				//!< Distance to target										[Animatable]
				long				mHLineDisplay;		//!< Horizon Line Display
				float				mEnvNearRange;		//!< Environment near range									[Animatable]
				float				mEnvFarRange;		//!< Environment far range									[Animatable]
				long				mEnvDisplay;		//!< Environment display
				long				mManualClip;		//!< Manual clip
				FOVType				mFOVType;			//!< FOV type [Flexporter 1.12]
				CType				mCamType;			//!< Camera type [Flexporter 1.12]
		// Instance information
				CameraDescriptor*	mMasterCamera;		//!< Master object [Flexporter 1.15]

		// Notes:
		//	- if mManualClip is on, you should take mNearClip and mFarClip into account. Else there's no near/far clipping.
		//	- if mManualClip is off and you build your projection matrix with mNearClip and mFarClip nonetheless, you may
		//	obtain a dark screen (nothing rendered), because your objects have been clipped in your renderer, but they weren't
		//	in MAX. To avoid this, always check "Manual Clip" in MAX, to make sure your objects will be visible once replayed.
	};

#endif // FLX_ADDCAMERA_H
