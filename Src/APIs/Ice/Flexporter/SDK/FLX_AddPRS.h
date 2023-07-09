///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the PRS-related exported structures.
 *	\file		FLX_AddPRS.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_ADDPRS_H
#define FLX_ADDPRS_H

	//! A point. Redefined so that the plug-ins can be compiled without MAX or ICE.
	class FLEXPORTER_API NakedPoint
	{
		public:

		inline_		NakedPoint()													{}
		inline_		NakedPoint(float _x, float _y, float _z) : x(_x), y(_y), z(_z)	{}
		inline_		~NakedPoint()													{}

		union
		{
			struct { float x, y, z; };
			struct { float u, v, w; };
			struct { float r, g, b; };
			struct { udword i, j, k; };
		};
	};

	//! A quaternion. Redefined so that the plug-ins can be compiled without MAX or ICE.
	class FLEXPORTER_API NakedQuat
	{
		public:

		inline_		NakedQuat()														{}
		inline_		~NakedQuat()													{}

					float	x, y, z, w;
	};

	//! PRS = Position/Rotation/Scale. PRS data can be recovered from MAX thanks to the decomp_affine function in MAX SDK.
	class FLEXPORTER_API NakedPRS
	{
		public:

					NakedPoint	mPosition;		//!< Translation part
					NakedQuat	mRotation;		//!< Rotation part
					NakedPoint	mScale;			//!< Scale part
	};

	class FLEXPORTER_API Pivot
	{
		public:
					NakedPRS	mLocalPivot;			//!< Pivot PRS in local space
					NakedPRS	mWorldPivot;			//!< Pivot PRS in world space
					NakedPRS	mParentWorldPivot;		//!< Parent pivot PRS in world space
	};

#endif // FLX_ADDPRS_H
