///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains all the basic information structures.
 *	\file		FLX_BaseInfo.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_BASEINFO_H
#define FLX_BASEINFO_H

	// Forward declarations
	class Converter;

	enum ObjType
	{
		FLX_OBJ_UNDEFINED,
		FLX_OBJ_CAMERA,
		FLX_OBJ_LIGHT,
		FLX_OBJ_MESH,
		FLX_OBJ_BPATCH,
		FLX_OBJ_CONTROLLER,

		FLX_OBJ_HELPER,
		FLX_OBJ_BOXGIZMO,
		FLX_OBJ_SPHEREGIZMO,
		FLX_OBJ_CYLINDERGIZMO,
		FLX_OBJ_BILLBOARD,

		FLX_OBJ_MATERIAL,
		FLX_OBJ_TEXTURE,
		FLX_OBJ_MOTION,
		FLX_OBJ_SHAPE,
		FLX_OBJ_GRAVITY,
		FLX_OBJ_WIND,

		FLX_OBJ_FORCE_DWORD	= 0x7fffffff
	};

	//! A base class shared by everyone
	class FLEXPORTER_API BaseDescriptor
	{
		public:
		//! Constructor/Destructor
									BaseDescriptor() :
										mInternalData(null),
										mInternalData3(null)
														{}
		virtual						~BaseDescriptor()	{}

				void*				mInternalData;			//!< Reserved
				void*				mInternalData3;			//!< Reserved
		static	void*				mInternalData2;			//!< Reserved
		protected:
		static	Converter*			mConverter;				//!< Current converter
		friend	class				Converter;
	};

	//! This class holds the main information about the scene
	class FLEXPORTER_API MainDescriptor : public BaseDescriptor
	{
		public:
		//! Constructor/Destructor
									MainDescriptor();
		virtual						~MainDescriptor();

				MAXTimeInfo			mTime;					//!< Time-related information
				MAXUnits			mUnits;					//!< Units-related information [Flexporter 1.13]
				udword				mNbNodes;				//!< Number of expected nodes
				udword				mNbCharacters;			//!< Number of expected characters
				NakedPoint			mBackColor;				//!< Background color [animatable]
				NakedPoint			mAmbColor;				//!< Ambient light color [animatable]
				const char*			mSceneInfos;			//!< Scene infos (in SummaryInfo) [Flexporter 1.09]
				const char*			mSceneHelpText;			//!< [Flexporter 1.16]
				float				mGlobalScale;			//!< Global scale used when exporting the scene [Flexporter 1.15]
				udword				mTesselation;			//!< Global scene tesselation (INVALID_ID if undefined) [Flexporter 1.17]
				udword				mLightingMode;			//!< Scene lighting mode (INVALID_ID if undefined) [Flexporter 1.17]
		// Scene physics
				NakedPoint			mGravity;				//!< Gravity vector (INVALID_IDs if undefined) [Flexporter 1.16]
				float				mRestitution;			//!< Global restitution (-1.0f if undefined) [Flexporter 1.16]
				float				mStaticFriction;		//!< Global static friction (-1.0f if undefined) [Flexporter 1.16]
				float				mFriction;				//!< Global friction (-1.0f if undefined) [Flexporter 1.16]
				bool				mGroundPlane;			//!< Ground plane enabled or not [Flexporter 1.16]
				bool				mCollisionDetection;	//!< Collision detection allowed or not [Flexporter 1.16]
	};

	//! The base information class for resources
	class FLEXPORTER_API ResourceDescriptor : public BaseDescriptor
	{
		public:
		// Constructor / destructor
									ResourceDescriptor();
		virtual						~ResourceDescriptor();

				const ubyte*		mName;					//!< Object's name
				sdword				mObjectID;				//!< Object's ID
				ObjType				mType;					//!< Object's type
	};

	//! The base information class for each 3D object
	class FLEXPORTER_API ObjectDescriptor : public ResourceDescriptor
	{
		public:
		// Constructor / destructor
									ObjectDescriptor();
		virtual						~ObjectDescriptor();

		// Database management
				const ubyte*		mParentName;			//!< Parent's name
				sdword				mParentID;				//!< Parent's ID
//				sdword				mLinkID;				//!< Possible link reference
				sdword				mMasterID;				//!< Master's ID [Flexporter 1.15]
				sdword				mTargetID;				//!< Target's ID [Flexporter 1.15]
		// Common properties
				NakedPRS			mPrs;					//!< Position/Rotation/Scale
//				MAXPoint			mPivotPos;				//!< Pivot position in local-space
//				MAXQuat				mPivotRot;				//!< Pivot rotation in local-space
				Pivot				mPivot;					//!< Pivot information [Flexporter 1.15]
				bool				mLocalPRS;				//!< True for local PRS, false for absolute ones.
				bool				mD3DCompliant;			//!< Object has been converted to D3D frame
				bool				mIsGroupMember;			//!< Belonging to a group
				bool				mIsInstance;			//!< true => object is an instance from another object
				bool				mIsHidden;				//!< true => object was hidden in MAX [Flexporter 1.16]
				udword				mWireColor;				//!< Object's Wire Color in MAX
				const ubyte*		mUserProps;				//!< User-defined properties
				JointDescriptor*	mIKData;				//!< IK data [Flexporter 1.15]
		// User-defined physics properties
				float				mDensity;				//!< Object's density [Flexporter 1.16]
				float				mMass;					//!< Object's mass [Flexporter 1.16]
				sdword				mSamplingDensity;		//!< Sampling rate [Flexporter 1.16]
				bool				mResetPivot;			//!< [Flexporter 1.16]
				bool				mIsCollidable;			//!< [Flexporter 1.16]
				bool				mLockPivot;				//!< Lock pivot point or not [Flexporter 1.16]
				NakedPoint			mLinearVelocity;		//!< Initial linear velocity [Flexporter 1.18]
				NakedPoint			mAngularVelocity;		//!< Initial angular velocity [Flexporter 1.18]
	};

	//! Statistics class
	class FLEXPORTER_API StatsDescriptor : public BaseDescriptor
	{
		public:
		//! Constructor
									StatsDescriptor()
									{
										mNbGeomObjects		= 0;
										mNbDerivedObjects	= 0;
										//mNbBPatchObjects	= 0;
										mNbCameras			= 0;
										mNbLights			= 0;
										mNbShapes			= 0;
										mNbHelpers			= 0;
										mNbControllers		= 0;
										mNbMaterials		= 0;
										mNbTexmaps			= 0;
										mNbSpaceWarps		= 0;
										mNbUnknowns			= 0;
										mNbInvalidNodes		= 0;
										//mNbBipedParts		= 0;
										//mNbCharacters		= 0;
									}
		//! Destructor
		virtual						~StatsDescriptor()		{}

				udword				mNbGeomObjects;			//!< Number of geomobjects found
				udword				mNbDerivedObjects;		//!< Number of derived objects found (actually number of skins)
				//udword			mNbBPatchObjects;		//!< Number of BPatch objects found
				udword				mNbCameras;				//!< Number of cameras found
				udword				mNbLights;				//!< Number of lights found
				udword				mNbShapes;				//!< Number of shapes found
				udword				mNbHelpers;				//!< Number of helpers found
				udword				mNbControllers;			//!< Number of controllers found
				udword				mNbMaterials;			//!< Number of materials found
				udword				mNbTexmaps;				//!< Number of texmaps found
				udword				mNbSpaceWarps;			//!< Number of space-warps found
				udword				mNbUnknowns;			//!< Number of unknown nodes found
				udword				mNbInvalidNodes;		//!< Number of invalid nodes found
				//udword			mNbBipedParts;			//!< Number of biped nodes found
				//udword			mNbCharacters;			//!< Number of characters found
	};

#endif // FLX_BASEINFO_H
