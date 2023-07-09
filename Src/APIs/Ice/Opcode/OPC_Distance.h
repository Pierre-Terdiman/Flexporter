///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for distance queries.
 *	\file		OPC_Distance.h
 *	\author		Pierre Terdiman
 *	\date		June, 2, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef OPC_DISTANCE_H
#define OPC_DISTANCE_H

	class OPCODE_API AABBTreeDistance : public AABBTreeCollider
	{
		public:
		// Constructor / Destructor
								AABBTreeDistance();
		virtual					~AABBTreeDistance();

				float			Compute(BVTCache& cache, const Matrix4x4* world0=null, const Matrix4x4* world1=null);

		inline_	float			GetDistance()			const	{ return mDist;	}
		inline_	const Point&	GetClosestPoint0()		const	{ return mCP0;	}
		inline_	const Point&	GetClosestPoint1()		const	{ return mCP1;	}
		inline_	udword			GetClosestTriangle0()	const	{ return mID0;	}
		inline_	udword			GetClosestTriangle1()	const	{ return mID1;	}

		protected:
				float			mDist;		//!< Separating distance
				Point			mCP0;		//!< Closest point on model 0, in model0 space
				Point			mCP1;		//!< Closest point on model 1, in model0 space (this is not a typo)
				udword			mID0;		//!< Index of closest triangle from model 0
				udword			mID1;		//!< Index of closest triangle from model 1
		// Internal methods
				void			_Distance(const AABBCollisionNode* b0, const AABBCollisionNode* b1);
		inline_	float			TriTriDist(udword id0, udword id1, Point& cp, Point& cq);
	};

	class OPCODE_API AABBTreeDistances : public AABBTreeCollider
	{
		public:
		// Constructor / Destructor
								AABBTreeDistances();
		virtual					~AABBTreeDistances();

				void			Compute(float threshold, BVTCache& cache, const Matrix4x4* world0=null, const Matrix4x4* world1=null);
/*
		inline_	float			GetDistance()			const	{ return mDist;	}
		inline_	const Point&	GetClosestPoint0()		const	{ return mCP0;	}
		inline_	const Point&	GetClosestPoint1()		const	{ return mCP1;	}
		inline_	udword			GetClosestTriangle0()	const	{ return mID0;	}
		inline_	udword			GetClosestTriangle1()	const	{ return mID1;	}
*/
		inline_	const Container&	GetData()			const	{ return mData;	}
		protected:
/*				float			mDist;		//!< Separating distance
				Point			mCP0;		//!< Closest point on model 0, in model0 space
				Point			mCP1;		//!< Closest point on model 1, in model0 space (this is not a typo)
				udword			mID0;		//!< Index of closest triangle from model 0
				udword			mID1;		//!< Index of closest triangle from model 1*/
				float			mThreshold;
				Container		mData;
		// Internal methods
				void			_Distance(const AABBCollisionNode* b0, const AABBCollisionNode* b1);
		inline_	float			TriTriDist(udword id0, udword id1, Point& cp, Point& cq);
	};

#endif // OPC_DISTANCE_H
