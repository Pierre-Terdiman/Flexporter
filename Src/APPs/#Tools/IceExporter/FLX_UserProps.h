///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file deals with user-defined properties.
 *	\file		FLX_UserProps.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_USERPROPS_H
#define FLX_USERPROPS_H

	// In MAX, user can define as many "user properties" as he wants. We use this facility to override some global settings, and to define some
	// settings which wouldn't exist in MAX otherwise. This structure is reset for each mesh, then updated thanks to the mesh's user properties.
	class UserProps
	{
		public:
			// Constructor/Destructor
							UserProps();
							~UserProps();

			UserProps&		Reset();				//!< Reset the flags, set default values
			void			SetupObject(ObjectDescriptor& object);

			// Physical parameters
			float			mDensity;				//!< Object's density
			float			mMass;					//!< Object's mass
			sdword			mSamplingDensity;		//!< Sampling rate
			bool			mResetPivot;			//!<
			bool			mCollisionDetection;	//!<
			bool			mLockPivot;				//!<
			Point			mLinearVelocity;		//!< Initial linear velocity
			Point			mAngularVelocity;		//!< Initial angular velocity

			bool			mFractal;				//!< Node has a fractal part	[OBSOLETE]
			bool			mMirror;				//!< Node is a mirror			[OBSOLETE]
			bool			mShadow;				//!< Node receives shadows		[OBSOLETE]
			bool			mRoom;					//!< Node is a room				[OBSOLETE]
			bool			mBone;					//!< Node is a virtual bone		[OBSOLETE]
			bool			mMainBone;				//!< Node is a virtual bone		[OBSOLETE]

			bool			mMorph;					//!< Node is a morphing mesh

			bool			mForceExport;			//!< Force node export.
			bool			mForceConsolidation;	//!< Force node consolidation.
			bool			mForceManifold;			//!< Force manifold mesh.
			bool			mForceNormals;			//!< Force normals export.
			bool			mExportPRS;				//!< Force PRS export.
			bool			mExportVisibility;		//!< Force visibility export.
			bool			mExportFOVTrack;		//!< Force FOV controller export.
			bool			mExportCamCtrl;			//!< Force camera attributes controllers export.
			bool			mExportLitCtrl;			//!< Force light attributes controllers export.
			bool			mExportUVW;				//!< Force UVWs export.
			bool			mExportVtxColors;		//!< Force vertex-colors export.
			bool			mExportSMG;				//!< Force smoothing groups usage.
			bool			mForceShapeToMesh;		//!< Force shape-to-mesh conversion.
			bool			mForceIK;				//!< Force IK export

			bool			mDiscardExport;			//!< Disable node export.
			bool			mDiscardConsolidation;	//!< Disable node consolidation.
			bool			mDiscardManifold;		//!< Disable manifold mesh.
			bool			mDiscardNormals;		//!< Disable normals export.
			bool			mDiscardPRS;			//!< Disable PRS export.
			bool			mDiscardVisibility;		//!< Disable visibility export.
			bool			mDiscardFOVTrack;		//!< Disable FOV controller export.
			bool			mDiscardCamCtrl;		//!< Disable camera attributes controllers export.
			bool			mDiscardLitCtrl;		//!< Disable light attributes controllers export.
			bool			mDiscardUVW;			//!< Disable UVWs export.
			bool			mDiscardVtxColors;		//!< Disable vertex-colors export.
			bool			mDiscardSMG;			//!< Disable smoothing groups.
			bool			mDiscardShapeToMesh;	//!< Disable shape-to-mesh conversion.
			bool			mDiscardIK;				//!< Disable IK export

			bool			mDebugDumpSubanims;
	};

	// User props
//	void GetUserProperties(INode* node, UserProps& props);

#endif // FLX_USERPROPS_H
