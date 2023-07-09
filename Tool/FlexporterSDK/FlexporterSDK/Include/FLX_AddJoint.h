///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2003 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains all IK parameters.
 *	\file		FLX_AddJoint.h
 *	\author		Pierre Terdiman
 *	\date		August, 7, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __FLX_IK_H__
#define __FLX_IK_H__

	// Validates DOF members
	// Introduced since IK data can come from various sources,
	// and all sources don't give access to all data!
	enum ValidDOFMember
	{
		FLX_VDOF_ACTIVE				= (1<<0),	//!< mActive has been setup
		FLX_VDOF_LIMIT				= (1<<1),	//!< mLimit has been setup
		FLX_VDOF_EASE				= (1<<2),	//!< mEase has been setup
		FLX_VDOF_SPRING				= (1<<3),	//!< mSpring has been setup
		FLX_VDOF_AXIS				= (1<<4),	//!< mAxis has been setup
		FLX_VDOF_POS				= (1<<5),	//!< mPos has been setup
		FLX_VDOF_MIN_MAX			= (1<<6),	//!< mMin and mMax have been setup
		FLX_VDOF_CURRENT			= (1<<7),	//!< mCurrentValue has been setup
		FLX_VDOF_DAMPING			= (1<<8),	//!< mDamping has been setup
		FLX_VDOF_SPRING_VALUE		= (1<<9),	//!< mSpringValue has been setup
		FLX_VDOF_SPRING_TENSION		= (1<<10),	//!< mSpringTension has been setup

		FLX_VDOF_FORCE_DWORD		= 0x7fffffff
	};

	// Captures a single DOF (degree of freedom)
	class FLEXPORTER_API DOFDescriptor
	{
		public:
		// Constructor/Destructor
								DOFDescriptor();
								~DOFDescriptor();

				bool			mActive;		//!< DOF is active or not
				bool			mLimit;			//!< DOF is limited or not
				bool			mEase;			//!< 
				bool			mSpring;		//!< 
				NakedPoint		mAxis;			//!< DOF axis
				NakedPoint		mPos;			//!< Base of axis
				float			mMin;			//!< DOF min limit [see notes below]
				float			mMax;			//!< DOF max limit [see notes below]
				float			mCurrentValue;	//!< Current value of the parameter
				float			mDamping;		//!< Damping
				float			mSpringValue;	//!< Spring
				float			mSpringTension;	//!< Spring tension

				udword			mValidFlags;	//!< Combination of ValidDOFMember
		// Notes:
		// * mMin and mMax:
		//   - for position DOFs, they're distances
		//   - for rotation DOFs, they're angles in radians
		//	- MAX ensures that Min < Max (both are swapped in the UI when it's not the case)
	};

	// One JointData structure / node. Contains a collection of DOFs.
	class FLEXPORTER_API JointDescriptor
	{
		public:
		// Constructor/Destructor
								JointDescriptor();
								~JointDescriptor();

		// Derived data
				udword			mPosDOF;		//!< Number of active DOFs, between 0 and 3
				udword			mRotDOF;		//!< Number of active DOFs, between 0 and 3
		// MAX data
				DOFDescriptor	mPos[3];		//!< Position DOFs, X, Y, Z axis
				DOFDescriptor	mRot[3];		//!< Rotation DOFs, X, Y, Z axis
//		private:
				void			FixData(const ExportSettings* settings);
				void			CountDOFs();

//		friend	class			Converter;
	};

#endif // __FLX_IK_H__
