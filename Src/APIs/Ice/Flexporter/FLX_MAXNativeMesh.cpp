///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace Flexporter;

#ifndef OLD_FLEX_FACES
Surface::Surface() : mNbVerts(0), mFaces(null), mVerts(null)
{
}

Surface::~Surface()
{
	DELETEARRAY(mVerts);
	DELETEARRAY(mFaces);
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXNativeMesh::MAXNativeMesh()
{
	mFlags			= 0;
	mNbFaces		= 0;
#ifdef OLD_FLEX_FACES
	mFaces			= null;
	mNbVerts		= 0;
	mNbTVerts		= 0;
	mNbCVerts		= 0;
	mVerts			= null;
	mTVerts			= null;
	mCVerts			= null;
#else
	mMatID			= null;
	mSmg			= null;
	mEdgeVis		= null;
#endif
	mVertexAlpha	= null;
	mBonesNb		= null;
	mBonesCSID		= null;
	mBonesLocalID	= null;
	mBonesName		= null;
	mOffsetVectors	= null;
	mWeights		= null;
	mSkeleton		= null;
	mConvexHull		= null;
	mBSCenter		= NakedPoint(0.0f, 0.0f, 0.0f);
	mBSRadius		= 0.0f;
	mTensor			= null;
	mParity			= false;
	mInternal		= NakedPoint(0.0f, 0.0f, 0.0f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXNativeMesh::~MAXNativeMesh()
{
#ifdef OLD_FLEX_FACES
	DELETEARRAY(mFaces);
	DELETEARRAY(mVerts);
	DELETEARRAY(mTVerts);
	DELETEARRAY(mCVerts);
#else
	DELETEARRAY(mEdgeVis);
	DELETEARRAY(mSmg);
	DELETEARRAY(mMatID);
#endif
	DELETEARRAY(mVertexAlpha);
	DELETEARRAY(mBonesNb);
	DELETEARRAY(mBonesName);
	DELETEARRAY(mBonesLocalID);
	DELETEARRAY(mBonesCSID);
	DELETEARRAY(mOffsetVectors);
	DELETEARRAY(mWeights);
	DELETESINGLE(mSkeleton);
	DELETESINGLE(mConvexHull);
	DELETESINGLE(mTensor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Transforms the cloud by an arbitrary 4x4 matrix.
 *	\param		mat		[in] the 4x4 matrix
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXNativeMesh::Transform(const Matrix4x4& mat)
{
#ifdef OLD_FLEX_FACES
	Point* V = (Point*)mVerts;
	for(udword i=0;i<mNbVerts;i++)
#else
	Point* V = (Point*)mPointSurface.mVerts;
	for(udword i=0;i<mPointSurface.mNbVerts;i++)
#endif
	{
		V[i] *= mat;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Computes the cloud's geometric center. Here we compute the average of all vertices of the cloud.
 *	\param		center	[out] destination point
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXNativeMesh::ComputeGeometricCenter(Point& center)
{
#ifdef OLD_FLEX_FACES
	if(!mNbVerts || !mVerts)	return false;
	const Point* V = (const Point*)mVerts;
	udword Nb = mNbVerts;
#else
	if(!mPointSurface.mNbVerts || !mPointSurface.mVerts)	return false;
	const Point* V = (const Point*)mPointSurface.mVerts;
	udword Nb = mPointSurface.mNbVerts;
#endif

	// ### do that better using faces

	// Init to zero
	center.Zero();
	// Loop through vertices
	for(udword i=0;i<Nb;i++)	center += V[i];
	// Average
	center/=float(Nb);

	return true;
}

static bool AllocVerts(udword nb_verts, const NakedPoint* verts, udword& dest_nb, NakedPoint*& dest_verts)
{
	dest_nb = 0;
	DELETEARRAY(dest_verts);

	if(nb_verts)
	{
		dest_verts = new NakedPoint[nb_verts];
		CHECKALLOC(dest_verts);
		dest_nb = nb_verts;
	}

	if(dest_verts && verts)	CopyMemory(dest_verts, verts, nb_verts * sizeof(NakedPoint));

	return true;
}

bool MAXNativeMesh::SetVerts(udword nb_verts, const NakedPoint* verts)
{
#ifdef OLD_FLEX_FACES
	AllocVerts(nb_verts, verts, mNbVerts, mVerts);
#else

#endif
	return true;
}

bool MAXNativeMesh::SetTVerts(udword nb_verts, const NakedPoint* verts)
{
#ifdef OLD_FLEX_FACES
	AllocVerts(nb_verts, verts, mNbTVerts, mTVerts);
#else
#endif
	return true;
}

bool MAXNativeMesh::SetCVerts(udword nb_verts, const NakedPoint* verts)
{
#ifdef OLD_FLEX_FACES
	AllocVerts(nb_verts, verts, mNbCVerts, mCVerts);
#else
#endif
	return true;
}
