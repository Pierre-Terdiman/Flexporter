///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code to handle tree coherence.
 *	\file		OPC_TreeCoherence.h
 *	\author		Pierre Terdiman
 *	\date		May, 12, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef OPC_TREECOHERENCE_H
#define OPC_TREECOHERENCE_H

	#define MASK_XMAX	(1<<0)
	#define MASK_XMIN	(1<<1)
	#define MASK_YMAX	(1<<2)
	#define MASK_YMIN	(1<<3)
	#define MASK_ZMAX	(1<<4)
	#define MASK_ZMIN	(1<<5)

/*	class OPCODE_API AABBCoherentTree : public AABBOptimizedTree
	{
		public:
		// Constructor / Destructor
											AABBCoherentTree();
		virtual								~AABBCoherentTree();
		// Build from a standard tree
		virtual			bool				Build(AABBTree* tree);
		// Data access
//		inline_			const volume*		GetNodes()		const	{ return mNodes;					}
		// Stats
//		virtual			udword				GetUsedBytes()	const	{ return mNbNodes*sizeof(volume);	}
		private:
						ubyte*				mNodes;
	public:
						Point				mCenterCoeff;
						Point				mExtentsCoeff;
	};
*/

#ifdef OLD_DEFORM

	class OPCODE_API Deformable
	{
		public:
		// Constructor / Destructor
											Deformable();
		virtual								~Deformable();
		// 
		virtual			bool				Build(const AABB& aabb, udword depth, const Point* v, const Triangle* tr, udword nbfaces);
		// Data access
//		inline_			const volume*		GetNodes()		const	{ return mNodes;					}
		// Stats
//		virtual			udword				GetUsedBytes()	const	{ return mNbNodes*sizeof(volume);	}
		private:
//						ubyte*				mNodes;
		public:
						udword				mDepth;
						udword				mNbNodes;
						udword				mNbLeaves;
						AABB*				mBoxes;
						float*				mMinPosList;
						float*				mMaxPosList;
						AABB*				mBoxList;
						RadixSort			mSorter;
	};
#endif

#endif // OPC_TREECOHERENCE_H