///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Keyframe definitions.
 *	\file		FLX_Keys.h
 *	\author		Pierre Terdiman
 *	\date		October, 5, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_KEYS_H
#define FLX_KEYS_H

// Base key

	class FLEXPORTER_API Key
	{
		public:
		// Constructor/Destructor
		inline_				Key() : mTime(0), mFlags(0)	{}
		inline_				~Key()						{}

				sdword		mTime;
				udword		mFlags;
	};

// Linear keys

	class FLEXPORTER_API LinearFloatKey : public Key
	{
		public:
		// Constructor/Destructor
		inline_					LinearFloatKey()	{}
		inline_					~LinearFloatKey()	{}

				float			mVal;
	};

	class FLEXPORTER_API LinearPointKey : public Key
	{
		public:
		// Constructor/Destructor
		inline_					LinearPointKey()	{}
		inline_					~LinearPointKey()	{}

				NakedPoint		mVal;
	};

	class FLEXPORTER_API LinearRotKey : public Key
	{
		public:
		// Constructor/Destructor
		inline_					LinearRotKey()	{}
		inline_					~LinearRotKey()	{}

				NakedQuat		mVal;
	};

	class FLEXPORTER_API LinearScaleKey : public Key
	{
		public:
		// Constructor/Destructor
		inline_					LinearScaleKey()	{}
		inline_					~LinearScaleKey()	{}

				NakedPoint		s;
				NakedQuat		q;
	};

// TCB keys

	class FLEXPORTER_API TCBKey : public Key
	{
		public:
		// Constructor/Destructor
		inline_					TCBKey()	{}
		inline_					~TCBKey()	{}

				float			mTens;
				float			mCont;
				float			mBias;
				float			mEaseIn;
				float			mEaseOut;
	};

	class FLEXPORTER_API TCBFloatKey : public TCBKey
	{
		public:
		// Constructor/Destructor
		inline_					TCBFloatKey()	{}
		inline_					~TCBFloatKey()	{}

				float			mVal;
	};

	class FLEXPORTER_API TCBPointKey : public TCBKey
	{
		public:
		// Constructor/Destructor
		inline_					TCBPointKey()	{}
		inline_					~TCBPointKey()	{}

				NakedPoint		mVal;
	};

	class FLEXPORTER_API TCBRotKey : public TCBKey
	{
		public:
		// Constructor/Destructor
		inline_					TCBRotKey()		{}
		inline_					~TCBRotKey()	{}

				NakedQuat		mVal;
	};

	class FLEXPORTER_API TCBScaleKey : public TCBKey
	{
		public:
		// Constructor/Destructor
		inline_					TCBScaleKey()	{}
		inline_					~TCBScaleKey()	{}

				NakedPoint		s;
				NakedQuat		q;
	};

// Bezier keys

	class FLEXPORTER_API BezierFloatKey : public Key
	{
		public:
		// Constructor/Destructor
		inline_					BezierFloatKey()	{}
		inline_					~BezierFloatKey()	{}

				float			mInTan, mOutTan;
				float			mVal;
	};

	class FLEXPORTER_API BezierPointKey : public Key
	{
		public:
		// Constructor/Destructor
		inline_					BezierPointKey()	{}
		inline_					~BezierPointKey()	{}

				NakedPoint		mInTan, mOutTan;
				NakedPoint		mVal;
	};

	class FLEXPORTER_API BezierRotKey : public Key
	{
		public:
		// Constructor/Destructor
		inline_					BezierRotKey()	{}
		inline_					~BezierRotKey()	{}

				NakedQuat		mVal;
	};

	class FLEXPORTER_API BezierScaleKey : public Key
	{
		public:
		// Constructor/Destructor
		inline_					BezierScaleKey()	{}
		inline_					~BezierScaleKey()	{}

				NakedPoint		mInTan, mOutTan;
				NakedPoint		s;
				NakedQuat		q;
	};

#endif // FLX_KEYS_H
