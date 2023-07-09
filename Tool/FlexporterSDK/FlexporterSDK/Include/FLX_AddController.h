///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2003 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the controller-related exported structures.
 *	\file		FLX_AddController.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __FLX_ADDCONTROLLER_H__
#define __FLX_ADDCONTROLLER_H__

	enum ControllerType
	{
		FLX_CTRL_UNKNOWN		= 0,		//!< Unknown type
		FLX_CTRL_FLOAT			= 1,		//!< A single float value
		FLX_CTRL_VECTOR			= 2,		//!< A point/vector (x,y,z, 3 floats)
		FLX_CTRL_QUAT			= 3,		//!< A quaternion (x,y,z,w, 4 floats)
		FLX_CTRL_SCALE			= 4,		//!< A MAX ScaleValue (Point + Quat, 7 floats)
		FLX_CTRL_PR				= 5,		//!< A PR structure (Position, Rotation = vector, quat = 7 floats)
		FLX_CTRL_PRS			= 6,		//!< A PRS structure (Position, Rotation, Scale = vector, quat, vector = 10 floats)
		FLX_CTRL_VERTEXCLOUD	= 7,		//!< A list of vectors

		FLX_CTRL_DWORD			= 0x7fffffff
	};

	FLEXPORTER_API	udword	GetSizeOf(ControllerType type);

	// Handy helper structures.
	struct FLEXPORTER_API CtrlFloat		{ float Value;									};
	struct FLEXPORTER_API CtrlVector	{ float x, y, z;								};
	struct FLEXPORTER_API CtrlQuat		{ float x, y, z, w;								};
	struct FLEXPORTER_API CtrlScale		{ float sx, sy, sz, qx, qy, qz, qw;				};
	struct FLEXPORTER_API CtrlPR		{ float tx, ty, tz, qx, qy, qz, qw;				};
	struct FLEXPORTER_API CtrlPRS		{ float tx, ty, tz, qx, qy, qz, qw, sx, sy, sz;	};

/*
	enum ControllerMode{
		CTRL_SAMPLES		= 1,			//!< Sampling mode
		CTRL_KEYFRAMES		= 2,			//!< Keyframing mode
		CTRL_DWORD			= 0x7fffffff
	};
*/

	//! Interpolation scheme
	enum InterpScheme
	{
		FLX_SCHEME_UNKNOWN		= 0,			//!< Unknown interpolation scheme
		FLX_SCHEME_LINEAR		= 1,			//!< Linear interpolation
		FLX_SCHEME_TCB			= 2,			//!< TCB interpolation
		FLX_SCHEME_BEZIER		= 3,			//!< Bezier interpolation
		FLX_SCHEME_FORCE_DWORD	= 0x7fffffff
	};

	// ControllerData: this is the base class for all controlled values.
	// The base information is:
	// - the ControllerType, which gives the type of data beeing controlled (from a single float to a vertex cloud)
	// - the ControllerMode, which gives the way the type is beeing controlled (by keyframes? Samples?)

	struct FLEXPORTER_API ControllerDataCreate
	{
		ControllerType	Type;
		CtrlMode		Mode;
	};

	// Base class
	class FLEXPORTER_API ControllerData
	{
		public:
		// Constructor/Destructor
								ControllerData();
		virtual					~ControllerData();

		// Initialize
		virtual	bool			Init(ControllerDataCreate* create)	= 0;

		public:
				ControllerType	mType;					//!< Type of data handled by the controller
				CtrlMode		mMode;					//!< Data are saved as samples, keyframes, etc
				udword			mFlags;					//!< Possible flags [not used yet]
	};

	// SampleData: this is the class to handle sampled data.
	// The extra information is:
	// - NbSamples, which gives the number of samples (one sample beeing one 'type' value. Ex: a Vector is 3 floats, but still one sample)
	// - the sampling rate

	struct FLEXPORTER_API SampleDataCreate : ControllerDataCreate
	{
		udword	NbSamples;
		udword	SamplingRate;
	};

	// A sample container.
	class FLEXPORTER_API SampleData : public ControllerData
	{
		public:
		// Constructor/Destructor

								SampleData();
		virtual					~SampleData();

		virtual	bool			Init(ControllerDataCreate* create);

		public:
				udword			mNbSamples;				//!< Number of samples
				udword			mSamplingRate;			//!< Sampling rate
				void*			mSamples;				//!< List of mNbSamples samples, whose type is mType
	};

	// KeyframeData: this is the class to handle keyframed data
	// The extra information is:
	// - the number of keyframes
	// - the interpolation scheme

	struct FLEXPORTER_API KeyframeDataCreate : ControllerDataCreate
	{
		udword			NbKeyframes;
		InterpScheme	Scheme;
	};

	// A keyframe container.
	class FLEXPORTER_API KeyframeData : public ControllerData
	{
		public:
		// Constructor/Destructor

								KeyframeData();
		virtual					~KeyframeData();

		virtual	bool			Init(ControllerDataCreate* create);

		public:
				udword			mNbKeyframes;			//!< Number of keyframes
				InterpScheme	mScheme;				//!< Interpolation scheme
				void*			mKeyframes;				//!< List of mNbKeyframes keyframes, whose type is mType
	};

	// MorphData: this is the class to handle morphing data.
	// Morphing is handled by sampling only, since individual vertex transformations may not be trivial, and different for each vertex.
	// The extra information is just the number of controlled vertices. One sample here is a list of NbVertices vectors.

	struct FLEXPORTER_API MorphDataCreate : SampleDataCreate
	{
		udword NbVertices;
	};

	// Morphing data
	class FLEXPORTER_API MorphData : public SampleData
	{
		public:
		// Constructor/Destructor

								MorphData();
		virtual					~MorphData();

		virtual	bool			Init(ControllerDataCreate* create);

		public:
				udword			mNbVertices;			//!< Number of morphed vertices
	};

	//! The controller information class.
	class FLEXPORTER_API ControllerDescriptor
	{
		public:
		// Constructor/Destructor
								ControllerDescriptor();
		virtual					~ControllerDescriptor();

		// Database management
				String			mField;					//!< Controlled field
				sdword			mObjectID;				//!< Controller's ID
				sdword			mOwnerID;				//!< Owner object's ID
				ObjType			mOwnerType;				//!< Owner object's type
		// NB: that way I added the animation controllers without breaking the old format.

		// Animation data
				ControllerData*	mData;					//!< Actual animation data
	};

#endif // __FLX_ADDCONTROLLER_H__
