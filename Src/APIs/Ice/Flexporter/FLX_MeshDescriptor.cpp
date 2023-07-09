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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MeshDescriptor::MeshDescriptor()
{
	mIsCollapsed	= false;
	mIsSkeleton		= false;
	mIsTarget		= false;
	mIsConvertible	= false;
	mIsSkin			= false;
	mCastShadows	= false;
	mReceiveShadows	= false;
	mMotionBlur		= false;

	mMasterMesh		= null;
	mOriginalMesh	= null;

	mCharID			= -1;
	mCSID			= -1;

	mBuilder		= null;
	mCleanMesh		= null;

#ifdef SUPPORT_OLD_LIGHTMAPPER
	mNbColors		= 0;
	mColors			= null;
#endif

	mPrimParams		= null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MeshDescriptor::~MeshDescriptor()
{
	DELETESINGLE(mOriginalMesh);
	DELETESINGLE(mPrimParams);
#ifdef SUPPORT_OLD_LIGHTMAPPER
	DELETEARRAY(mColors);
#endif
	DELETESINGLE(mCleanMesh);
	DELETESINGLE(mBuilder);
}

bool MeshDescriptor::CreateOriginalMesh()
{
	DELETESINGLE(mOriginalMesh);
	mOriginalMesh = new MAXNativeMesh;
	CHECKALLOC(mOriginalMesh);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Consolidates the mesh.
 *	\param		settings	[in] export settings
 *	\param		.......
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MeshDescriptor::Consolidate(const ExportSettings* settings, bool force_normals, bool discard_normals, bool morph)
{
	// Checkings
	if(!settings || !mOriginalMesh)	return false;

	// Allocation cycle
	DELETESINGLE(mBuilder);		mBuilder	= new MeshBuilder2;	CHECKALLOC(mBuilder);
	DELETESINGLE(mCleanMesh);	mCleanMesh	= new MBResult;		CHECKALLOC(mCleanMesh);

	// Setup creation structure
	MBCreate Create;
	Create.UseW					= settings->mDiscardW ? false : true;	// Keep or discard W
	Create.ComputeVNorm			= settings->mComputeVertexNormals;		// Compute vertex-normals
	Create.ComputeFNorm			= settings->mComputeFaceNormals;		// Compute face-normals
	if(force_normals)	Create.ComputeVNorm = Create.ComputeFNorm = true;
	if(discard_normals)	Create.ComputeVNorm = Create.ComputeFNorm = false;
	Create.ComputeNormInfo		= settings->mExportNormalInfo;			// Compute normal-info
	Create.RelativeIndices		= false;								// Ask for absolute indices
	Create.IndexedGeo			= true;									// Ask for indexed geometry
	Create.IndexedUVW			= true;									// Ask for indexed UVs
//	Create.IndexedColors		= true;									// Ask for indexed colors [1.15]
	Create.NbFaces				= mOriginalMesh->GetNbFaces();
#ifdef OLD_FLEX_FACES
	Create.NbVerts				= mOriginalMesh->GetNbVerts();
	Create.NbTVerts				= mOriginalMesh->GetNbTVerts();
	Create.NbCVerts				= mOriginalMesh->GetNbCVerts();
	Create.Verts				= (const Point*)mOriginalMesh->GetVerts();
	Create.TVerts				= (const Point*)mOriginalMesh->GetTVerts();
	Create.CVerts				= (const Point*)mOriginalMesh->GetCVerts();
#else
	Create.NbVerts				= mOriginalMesh->mPointSurface.mNbVerts;
	Create.NbTVerts				= mOriginalMesh->mUVWSurface.mNbVerts;
	Create.NbCVerts				= mOriginalMesh->mColorSurface.mNbVerts;
	Create.Verts				= (const Point*)mOriginalMesh->mPointSurface.mVerts;
	Create.TVerts				= (const Point*)mOriginalMesh->mUVWSurface.mVerts;
	Create.CVerts				= (const Point*)mOriginalMesh->mColorSurface.mVerts;
#endif
	Create.IsSkin				= mIsSkin;
	if(morph) Create.IsSkin = true;
//	Create.WeightNormalWithArea	= settings->mWeightNormalsByArea;
	Create.WeightNormalWithAngles	= settings->mWeightNormalsByArea;
	Create.OptimizeVertexList	= settings->mCacheOptimize;

	// Handle vertex alpha
	const udword Flags = mOriginalMesh->GetFlags();
	Point* RGBA = null;
	udword* DefaultTopo = null;
	if(mOriginalMesh->GetVertexAlpha() && mOriginalMesh->GetNbFaces())
	{
		// We have mNbVerts alpha values for mNbVerts vertices. So they would be "indexed" by a default topology.

		// We also have some vertex colors and a color topology.
		//
		// So here we need to blend vertex alpha and vertex colors.
		// To do that we:
		// - create a non-indexed pool of RGBA colors
		// - override original vertex colors with it

		// The final list has mNbFaces*3 colors...
		RGBA = new Point[mOriginalMesh->GetNbFaces()*3];
		if(RGBA)
		{
			// ...and it's indexed by a default topology
			DefaultTopo = new udword[mOriginalMesh->GetNbFaces()*3];
			if(DefaultTopo)
			{
				// Create default topology
				for(udword i=0;i<mOriginalMesh->GetNbFaces()*3;i++)	DefaultTopo[i] = i;

#ifndef OLD_FLEX_FACES
				// Get original MAX faces
				const udword* VRefs = (const udword*)mOriginalMesh->mUVWSurface.mFaces;
				const udword* CRefs = (const udword*)((Flags&MESH_CFACE) ? mOriginalMesh->mColorSurface.mFaces : null);
#endif
				// Create colors
				for(udword i=0;i<mOriginalMesh->GetNbFaces();i++)
				{
#ifdef OLD_FLEX_FACES
					// Get original MAX face
					const MAXFace* MF = &mOriginalMesh->GetFaces()[i];
					const udword* VRefs = (const udword*)MF->VRef;
					const udword* CRefs = (const udword*)((Flags&MESH_CFACE) ? MF->CRef : null);
#endif
					// 3 colors
					Point c0;
					Point c1;
					Point c2;
#ifdef OLD_FLEX_FACES
					if(mOriginalMesh->GetNbCVerts() && CRefs && Create.CVerts)
#else
					if(mOriginalMesh->mColorSurface.mNbVerts && CRefs && Create.CVerts)
#endif
					{
						// Mesh had vertex colors
						c0 = Create.CVerts[*CRefs++];
						c1 = Create.CVerts[*CRefs++];
						c2 = Create.CVerts[*CRefs++];
					}
					else
					{
						// No vertex colors -> black !
						c0.Zero();
						c1.Zero();
						c2.Zero();
					}

					// 3 alpha
					float a0 = mOriginalMesh->GetVertexAlpha()[*VRefs++];
					float a1 = mOriginalMesh->GetVertexAlpha()[*VRefs++];
					float a2 = mOriginalMesh->GetVertexAlpha()[*VRefs++];

					// 3 mixed colors
					RGBAColor mc0(c0.x, c0.y, c0.z, a0);
					RGBAColor mc1(c1.x, c1.y, c1.z, a1);
					RGBAColor mc2(c2.x, c2.y, c2.z, a2);

					// Gives dword colors
					udword FinalColor0 = mc0;
					udword FinalColor1 = mc1;
					udword FinalColor2 = mc2;

					// Stuffed in the red channel
					RGBA[i*3+0] = Point(FR(FinalColor0), 0.0f, 0.0f);
					RGBA[i*3+1] = Point(FR(FinalColor1), 0.0f, 0.0f);
					RGBA[i*3+2] = Point(FR(FinalColor2), 0.0f, 0.0f);
				}

				// Now we need to replace the vertex colors (or setup new ones only made of alpha values)
				Create.NbCVerts = mOriginalMesh->GetNbFaces()*3;
				Create.CVerts = RGBA;
			}
		}
	}

	if(!mBuilder->Init(Create))	return SetIceError("MeshBuilder init failed!", null);

	MBFaceData FD;
	FD.CCW = mOriginalMesh->GetParity();
	// Loop through faces
	udword* DF = DefaultTopo;
	for(udword i=0;i<mOriginalMesh->GetNbFaces();i++)
	{
#ifdef OLD_FLEX_FACES
		// Get original MAX face
		const MAXFace* MF = &mOriginalMesh->GetFaces()[i];

		// Add face in MeshBuilder
		FD.VRefs			= (udword*)((Flags&MESH_VFACE) ? MF->VRef : null);
		FD.TRefs			= (udword*)((Flags&MESH_TFACE) ? MF->TRef : null);
#else
		// Add face in MeshBuilder
		FD.VRefs			= (udword*)((Flags&MESH_VFACE) ? &mOriginalMesh->mPointSurface.mFaces[i] : null);
		FD.TRefs			= (udword*)((Flags&MESH_TFACE) ? &mOriginalMesh->mUVWSurface.mFaces[i] : null);
#endif
		if(DF)				FD.CRefs = DF;
#ifdef OLD_FLEX_FACES
		else				FD.CRefs = (udword*)((Flags&MESH_CFACE) ? MF->CRef : null);
		FD.SmoothingGroups	= MF->Smg;
		FD.MaterialID		= MF->MatID;
//		ubyte EdgeVis		= MF->EdgeVis;
#else
		else				FD.CRefs = (udword*)((Flags&MESH_CFACE) ? &mOriginalMesh->mColorSurface.mFaces[i] : null);
		FD.SmoothingGroups	= mOriginalMesh->mSmg[i];
		FD.MaterialID		= mOriginalMesh->mMatID[i];
//		ubyte EdgeVis		= mOriginalMesh->mEdgeVis[i];
#endif
		if(!mBuilder->AddFace(FD))	return SetIceError("MeshBuilder AddFace failed!", null);
		if(DF)				DF+=3;
	}

	// Perform mesh conversion
	if(!mBuilder->Build(*mCleanMesh))	return SetIceError("Mesh building failed!", null);

	// Mark as consolidated
	mOriginalMesh->mFlags|=MESH_CONSOLIDATION;

	// Mark vertex alpha
	if(RGBA)	mOriginalMesh->mFlags|=MESH_DWORD_COLORS;

	// Normals flags
	if(settings->mComputeFaceNormals)	mOriginalMesh->mFlags|=MESH_FACENORMALS;
	if(settings->mComputeVertexNormals)	mOriginalMesh->mFlags|=MESH_VERTEXNORMALS;

	// NormalInfo flags. Only valid if vertex-normals are computed.
	if(settings->mComputeVertexNormals && settings->mExportNormalInfo)		mOriginalMesh->mFlags|=MESH_NORMALINFO;

	// Release local ram
	DELETEARRAY(DefaultTopo);
	DELETEARRAY(RGBA);

	return true;
}
