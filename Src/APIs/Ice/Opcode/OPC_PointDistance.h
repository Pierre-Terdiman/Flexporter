///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for point distance queries.
 *	\file		OPC_PointDistance.h
 *	\author		Pierre Terdiman
 *	\date		December, 12, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef OPC_POINTDISTANCE_H
#define OPC_POINTDISTANCE_H

		class OPCODE_API PointDistanceQuery
		{
			public:
			// Constructor / Destructor
											PointDistanceQuery();
			virtual							~PointDistanceQuery();

							bool			ComputeDistance(const Point& world_pt, const Model* model,					const Matrix4x4* world=null, udword* cache=null);

							bool			ComputeDistance(const Point& world_pt, const AABBCollisionTree* tree,		const Matrix4x4* world=null, udword* cache=null);
							bool			ComputeDistance(const Point& world_pt, const AABBNoLeafTree* tree,			const Matrix4x4* world=null, udword* cache=null);
							bool			ComputeDistance(const Point& world_pt, const AABBQuantizedTree* tree,		const Matrix4x4* world=null, udword* cache=null);
							bool			ComputeDistance(const Point& world_pt, const AABBQuantizedNoLeafTree* tree,	const Matrix4x4* world=null, udword* cache=null);

			inline_			float			GetDistance()			const	{ return mMinDist;	}
			inline_			udword			GetClosestTriangle()	const	{ return mPrim;		}
			inline_			float			GetU()					const	{ return mU;		}
			inline_			float			GetV()					const	{ return mV;		}

			private:
		// User mesh interface
					const	MeshInterface*	mIMesh;				//!< User-defined mesh interface
			// Dequantization coeffs
							Point			mCenterCoeff;
							Point			mExtentsCoeff;
			// Distance data
							Point			mLocalPt;
							float			mMinDist;
							float			mU, mV;
							udword			mPrim;
			// Internal methods
		inline_				void			DIST_PRIM(udword id);
							void			_Distance(const AABBCollisionNode* node);
							void			_Distance(const AABBNoLeafNode* node);
							void			_Distance(const AABBQuantizedNode* node);
							void			_Distance(const AABBQuantizedNoLeafNode* node);
							void			InitQuery(const Point& world_pt, const Matrix4x4* world=null, udword* face_id=null);

		inline_				BOOL			Setup(const MeshInterface* mi)
											{
												mIMesh = mi;
												return mIMesh!=null;
											}
		};

#endif // OPC_POINTDISTANCE_H
