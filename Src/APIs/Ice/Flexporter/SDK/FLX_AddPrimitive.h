///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains code for supported primitives (standard or extended). We don't support all of them.
 *	You probably won't need all exposed parameters. Standard usage (as far as I'm concerned) is to feed a
 *	physics engine.
 *
 *	\file		FLX_AddPrimitive.h
 *	\author		Pierre Terdiman
 *	\date		August, 30, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_ADDPRIMITIVE_H
#define FLX_ADDPRIMITIVE_H

	enum PrimitiveType
	{
		FLX_PRIM_UNDEFINED		= 0,
		// Standard primitives
		FLX_PRIM_BOX			= 1,
		FLX_PRIM_SPHERE			= 2,
		FLX_PRIM_GEOSPHERE		= 3,
		FLX_PRIM_CYLINDER		= 4,
		FLX_PRIM_CONE			= 5,
		FLX_PRIM_TORUS			= 6,
		FLX_PRIM_TUBE			= 7,
		FLX_PRIM_TEAPOT			= 8,
		FLX_PRIM_PLANE			= 11,
		// Extended primitives
		FLX_PRIM_HEDRA			= 9,
		FLX_PRIM_CAPSULE		= 10,

		FLX_PRIM_FORCE_DWORD	= 0x7fffffff
	};

	class FLEXPORTER_API PrimitiveParams
	{
		public:
		// Constructor / Destructor
									PrimitiveParams() : mType(FLX_PRIM_UNDEFINED)	{}
		virtual						~PrimitiveParams()								{}

				PrimitiveType		mType;
	};

	class FLEXPORTER_API BoxParams : public PrimitiveParams
	{
		public:
		// Constructor / Destructor
									BoxParams()		{ mType = FLX_PRIM_BOX;	}
		virtual						~BoxParams()	{}

				float				mLength;
				float				mWidth;
				float				mHeight;
				int					mWSegs;
				int					mLSegs;
				int					mHSegs;
				BOOL				mGenUVS;
	};

	class FLEXPORTER_API SphereParams : public PrimitiveParams
	{
		public:
		// Constructor / Destructor
									SphereParams()	{ mType = FLX_PRIM_SPHERE;	}
		virtual						~SphereParams()	{}

				float				mRadius;
				int					mSegments;
				BOOL				mSmooth;
				float				mHemisphere;
				BOOL				mSquash;	// else Chop
				float				mSliceFrom;
				float				mSliceTo;
				BOOL				mSliceOn;
				BOOL				mRecenter;	// Base to pivot
				BOOL				mGenUVS;
	};

	class FLEXPORTER_API GeosphereParams : public PrimitiveParams
	{
		public:
		// Constructor / Destructor
									GeosphereParams()	{ mType = FLX_PRIM_GEOSPHERE;	}
		virtual						~GeosphereParams()	{}

				float				mRadius;
				int					mSegments;
				int					mGenType;	// Between 0 & 2
				BOOL				mHemisphere;
				BOOL				mSmooth;
				BOOL				mRecenter;
				BOOL				mGenUVS;
	};

	class FLEXPORTER_API CylinderParams : public PrimitiveParams
	{
		public:
		// Constructor / Destructor
									CylinderParams()	{ mType = FLX_PRIM_CYLINDER;	}
		virtual						~CylinderParams()	{}

				float				mRadius;
				float				mHeight;
				int					mHSegs;
				int					mCapSegs;
				int					mSides;
				BOOL				mSmooth;
				BOOL				mSliceOn;
				float				mSliceFrom;
				float				mSliceTo;
				BOOL				mGenUVS;
	};

	class FLEXPORTER_API ConeParams : public PrimitiveParams
	{
		public:
		// Constructor / Destructor
									ConeParams()	{ mType = FLX_PRIM_CONE;	}
		virtual						~ConeParams()	{}

				float				mRadius1;
				float				mRadius2;
				float				mHeight;
				int					mHSegs;
				int					mCapSegs;
				int					mSides;
				BOOL				mSmooth;
				BOOL				mSliceOn;
				float				mSliceFrom;
				float				mSliceTo;
				BOOL				mGenUVS;
	};

	class FLEXPORTER_API TorusParams : public PrimitiveParams
	{
		public:
		// Constructor / Destructor
									TorusParams()	{ mType = FLX_PRIM_TORUS;	}
		virtual						~TorusParams()	{}

				float				mRadius1;
				float				mRadius2;
				float				mRotation;
				float				mTwist;
				int					mSegments;
				int					mSides;
				BOOL				mSmooth;
				BOOL				mSliceOn;
				float				mSliceFrom;
				float				mSliceTo;
				BOOL				mGenUVS;
	};

	class FLEXPORTER_API TubeParams : public PrimitiveParams
	{
		public:
		// Constructor / Destructor
									TubeParams()	{ mType = FLX_PRIM_TUBE;	}
		virtual						~TubeParams()	{}

				float				mRadius1;
				float				mRadius2;
				float				mHeight;
				int					mSegments;
				int					mCapSegs;
				int					mSides;
				BOOL				mSmooth;
				BOOL				mSliceOn;
				float				mSliceFrom;
				float				mSliceTo;
				BOOL				mGenUVS;
	};

	class FLEXPORTER_API TeapotParams : public PrimitiveParams
	{
		public:
		// Constructor / Destructor
									TeapotParams()	{ mType = FLX_PRIM_TEAPOT;	}
		virtual						~TeapotParams()	{}
		// Not implemented
	};

	class FLEXPORTER_API PlaneParams : public PrimitiveParams
	{
		public:
		// Constructor / Destructor
									PlaneParams()	{ mType = FLX_PRIM_PLANE;	}
		virtual						~PlaneParams()	{}

				float				mLength;
				float				mWidth;
				int					mWidthSegs;
				int					mLengthSegs;
				float				mDensity;
				float				mScale;
				BOOL				mGenUVS;
	};

	class FLEXPORTER_API HedraParams : public PrimitiveParams
	{
		public:
		// Constructor / Destructor
									HedraParams()	{ mType = FLX_PRIM_HEDRA;	}
		virtual						~HedraParams()	{}

				float				mRadius;
				int					mFamily;
				float				mP;
				float				mQ;
				float				mScaleP;
				float				mScaleQ;
				float				mScaleR;
				int					mVertices;
				BOOL				mGenUVS;
	};

	class FLEXPORTER_API CapsuleParams : public PrimitiveParams
	{
		public:
		// Constructor / Destructor
									CapsuleParams()		{ mType = FLX_PRIM_CAPSULE;	}
		virtual						~CapsuleParams()	{}

				float				mRadius;
				float				mHeight;
				int					mCenters;
				int					mSides;
				int					mHSegs;
				BOOL				mSmooth;
				BOOL				mSliceOn;
				float				mSliceFrom;
				float				mSliceTo;
				BOOL				mGenUVS;
	};

#endif // FLX_ADDPRIMITIVE_H
