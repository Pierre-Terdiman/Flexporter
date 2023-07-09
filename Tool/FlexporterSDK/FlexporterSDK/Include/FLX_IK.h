///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2002 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains all IK parameters.
 *	\file		FLX_IK.h
 *	\author		Pierre Terdiman
 *	\date		August, 7, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __FLX_IK_H__
#define __FLX_IK_H__

	// Forward declarations
	class ExportSettings;

	// Validates DOF members
	// Introduced since IK data can come from various sources,
	// and all sources don't give access to all data!
	enum ValidDOFMember
	{
		VDOF_ACTIVE				= (1<<0),	//!< mActive has been setup
		VDOF_LIMIT				= (1<<1),	//!< mLimit has been setup
		VDOF_EASE				= (1<<2),	//!< mEase has been setup
		VDOF_SPRING				= (1<<3),	//!< mSpring has been setup
		VDOF_AXIS				= (1<<4),	//!< mAxis has been setup
		VDOF_POS				= (1<<5),	//!< mPos has been setup
		VDOF_MIN_MAX			= (1<<6),	//!< mMin and mMax have been setup
		VDOF_CURRENT			= (1<<7),	//!< mCurrentValue has been setup
		VDOF_DAMPING			= (1<<8),	//!< mDamping has been setup
		VDOF_SPRING_VALUE		= (1<<9),	//!< mSpringValue has been setup
		VDOF_SPRING_TENSION		= (1<<10),	//!< mSpringTension has been setup

		VDOF_FORCE_DWORD		= 0x7fffffff
	};

	// Captures a single DOF (degree of freedom)
	class DOF
	{
		public:
		// Constructor/Destructor
							DOF();
							~DOF();

				bool		mActive;		//!< DOF is active or not
				bool		mLimit;			//!< DOF is limited or not
				bool		mEase;			//!< 
				bool		mSpring;		//!< 
				MAXPoint	mAxis;			//!< DOF axis
				MAXPoint	mPos;			//!< Base of axis
				float		mMin;			//!< DOF min limit
				float		mMax;			//!< DOF max limit
				float		mCurrentValue;	//!< Current value of the parameter
				float		mDamping;		//!< Damping
				float		mSpringValue;	//!< Spring
				float		mSpringTension;	//!< Spring tension

				udword		mValidFlags;	//!< Combination of ValidDOFMember
		// Notes:
		// * mMin and mMax:
		//   - for position DOFs, they're distances
		//   - for rotation DOFs, they're angles in radians
		//	- MAX ensures that Min < Max (both are swapped in the UI when it's not the case)
	};

	// One JointData structure / node. Contains a collection of DOFs.
	class JointData
	{
		public:
		// Constructor/Destructor
							JointData();
							~JointData();

		// Derived data
				udword		mPosDOF;		//!< Number of active DOFs, between 0 and 3
				udword		mRotDOF;		//!< Number of active DOFs, between 0 and 3
		// MAX data
				DOF			mPos[3];		//!< Position DOFs, X, Y, Z axis
				DOF			mRot[3];		//!< Rotation DOFs, X, Y, Z axis
		private:
				void		FixData(const ExportSettings* settings);
				void		CountDOFs();

		friend	class		Converter;
	};

#endif // __FLX_IK_H__
