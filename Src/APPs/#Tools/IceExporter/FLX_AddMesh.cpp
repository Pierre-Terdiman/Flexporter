///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a mesh. (GeomObjects & DerivedObjects)
 *	\file		FLX_AddMesh.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

CHull::~CHull()
{
	DELETEARRAY(mFaces);
	DELETEARRAY(mVerts);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Converts a geom-object.
 *	\param		node			[in] the MAX node
 *	\param		derived_object	[in] a possible collapsed derived object
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddGeomObject(INode* node, Object* derived_object)
{
	// 1) Preliminaries
	// 1-1) Get time info back
	TimeValue CurrentTime = mTimeInfo.mCurrentTime;

	// 1-3) Update log text
	mArrayLog.StoreASCII("Exporting mesh %s\n", node->GetName());

	// 1-2) Get skeleton status
	char Skeleton = IsSkeleton(node);
	if(Skeleton && !mSettings->mExportBIPED)
	{
		mArrayLog.StoreASCII("Skeleton part (discarded because of export options)\n");
		return true;
	}

	// 1-4) Get right object: it can be the real node or a collapsed derived object
	Object* NodeObject = derived_object ? derived_object : node->GetObjectRef();

	// 1-5) Get instance state (if needed). We have to do this before CreateDescriptor since we now use that
	// descriptor to decide between instance or not........
	sdword MasterID;
	bool IsInstance = mSettings->mDiscardInstances ? false : mGlobalInfo.IsInstance(NodeObject, MasterID);

	// 2) Create info structure
	MeshDescriptor* MeshInf = (MeshDescriptor*)mGlobalInfo.CreateDescriptor(node, FLX_OBJ_MESH);
	// 2-1) The header
	AddHeader(node, MeshInf);

	// Give special access to the node's object
	MeshInf->mInternalData3 = NodeObject;

	if(IsMirrored(node, CurrentTime))
		mArrayLog.StoreASCII("WARNING: mirrored object, PRS might be invalid. Use Reset X-Form !\n");

	// 2-2) The basic mesh settings
	MeshInf->mIsCollapsed		= derived_object ? true : false;
	MeshInf->mIsSkeleton		= Skeleton!=0;
	MeshInf->mIsTarget			= false;
	MeshInf->mIsConvertible		= true;
	MeshInf->mIsSkin			= false;
	MeshInf->mCastShadows		= node->CastShadows()!=0;
	MeshInf->mReceiveShadows	= node->RcvShadows()!=0;
	MeshInf->mMotionBlur		= node->MotBlur()!=0;
	MeshInf->mIsInstance		= IsInstance;

	// 2-3) Is this a BIPED part? i.e. part of a symbolic skeleton
	if(MeshInf->mIsSkeleton)
	{
		// Yep! Get the Character ID and the bone's ID
		udword CharID		= mGlobalInfo.GetCharacterID(node);
		udword CSID			= GetCSID(node);
		MeshInf->mCharID	= CharID;
		MeshInf->mCSID		= CSID;
	}

	// 2-4) Detect primitives
	if(!MeshInf->mIsInstance)	AddPrimitive(NodeObject, *MeshInf);
//	if(!MeshInf->mIsInstance)	AddPrimitive(node->GetObjectRef(), *MeshInf);

	// 3) Check instance state
	if(MeshInf->mIsInstance)
	{
//		MeshInf->mMasterMesh	= (MeshDescriptor*)mGlobalInfo.mNodeInfo[MasterID].mExportedNode;
		MeshInf->mMasterMesh	= (MeshDescriptor*)mGlobalInfo.GetNodeInfo()[MasterID].mExportedNode;
		MeshInf->mMasterID		= MeshInf->mMasterMesh->mObjectID;

		// Update log text
//		mArrayLog.StoreASCII("Instance node from: ").StoreASCII(mGlobalInfo.mNodeInfo[MasterID].mMAXNode->GetName()).StoreASCII("\n");
		mArrayLog.StoreASCII("Instance node from: ").StoreASCII(mGlobalInfo.GetNodeInfo()[MasterID].mMAXNode->GetName()).StoreASCII("\n");

		// Check master & instance have same material
		{
			Mtl* Material0 = mGlobalInfo.GetNodeInfo()[MasterID].mMAXNode->GetMtl();
			Mtl* Material1 = node->GetMtl();
			if(Material0!=Material1)
			{
				mArrayLog.StoreASCII("WARNING: master mesh doesn't have same material! Current material will be lost.\n");
			}
		}

		if(MeshInf->mMasterMesh->mResetPivot)
//		if(MeshInf->mResetPivot)
		{
			mArrayLog.StoreASCII("(Reseting instance pivot)\n");
			// Here we're an instance. The master has already been reset.

			// 1) Compute master's translate matrix again
			Point LocalCenter;
			LocalCenter.x = MeshInf->mMasterMesh->mOriginalMesh->mInternal.x;
			LocalCenter.y = MeshInf->mMasterMesh->mOriginalMesh->mInternal.y;
			LocalCenter.z = MeshInf->mMasterMesh->mOriginalMesh->mInternal.z;
//			MeshInf->mMasterMesh->mOriginalMesh.ComputeGeometricCenter(LocalCenter);

			Matrix4x4 Translate;
			Translate.Identity();
			Translate.SetTrans(-LocalCenter);

			// Invert translate matrix
			Translate.Invert();

			// Update the position so that the transformed vertices stay in place
			Matrix4x4 LocalToWorld;
			MAXToICE(LocalToWorld, MeshInf->mPrs);	// Instance matrix

			// Fix instance

			// Previous pattern:
			// - Master = vertices + world matrix, world point = vertices * world matrix
			// - Instance = instance matrix + master, vertices = vertices * instance matrix
			//
			// New pattern:
			// - Master = vertices + new world matrix, world point = (vertices * translate matrix) * new world matrix
			// - Instance = new instance matrix + master, vertices = (vertices * translate matrix) * new instance matrix
			//
			// We want the instance to stay in place, i.e.:
			//     old instance matrix = translate matrix * new instance matrix
			// <=> Inverse(translate matrix) * old instance matrix = new instance matrix
			//
			Matrix4x4 NewInstanceMatrix = Translate * LocalToWorld;

			// So here's the new instance PR.
			PR NewInstancePR = NewInstanceMatrix;
//			CurrentInstance->mPrs = NewInstancePR;	// Bug with scaled instances!
			MeshInf->mPrs.mPosition.x = NewInstancePR.mPos.x;
			MeshInf->mPrs.mPosition.y = NewInstancePR.mPos.y;
			MeshInf->mPrs.mPosition.z = NewInstancePR.mPos.z;
			MeshInf->mPrs.mRotation.x = NewInstancePR.mRot.p.x;
			MeshInf->mPrs.mRotation.y = NewInstancePR.mRot.p.y;
			MeshInf->mPrs.mRotation.z = NewInstancePR.mRot.p.z;
			MeshInf->mPrs.mRotation.w = NewInstancePR.mRot.w;
		}
	}

	// 4) Check target state
	// GeomObjects are:
	// - all basic primitives
	// - camera target
	// - spot target
	// - directional target
	else if(NodeObject->ClassID() == Class_ID(TARGET_CLASS_ID, 0))
	{
		// Here we have a target: camera, spot or directional.
		// Hence we do not have to convert it to a tri-mesh.
		MeshInf->mIsTarget = true;

		// Discard targets if needed. Note: stats aren't updated.
		if(!mSettings->mExpTargetNodes && !mProps.mForceExport)
		{
			mArrayLog.StoreASCII("Target node. (discarded because of export options)\n");
			return true;
		}
		else mArrayLog.StoreASCII("Target node. (kept)\n");
	}

	// 5) Convert geometric object
	else if(NodeObject->CanConvertToType(triObjectClassID))
	{
		// 5-2) Material stuff
		if(!AddMetaMaterial(node))					IceCore::MessageBox(null, _F("Object %s has an invalid material!\n", node->GetName()), "Oops", MB_OK);

		// 5-3) Mesh stuff
		if(!BuildMesh(*MeshInf, NodeObject, node))	IceCore::MessageBox(null, _F("Object %s failed!\n", node->GetName()), "Oops", MB_OK);

		// 5-4) Convex hull
		if(!BuildConvexHull(*MeshInf))				IceCore::MessageBox(null, _F("Object %s: convex hull computation failed!\n", node->GetName()), "Oops", MB_OK);

		// 5-5) Bounding sphere
		if(!BuildBoundingSphere(*MeshInf))			IceCore::MessageBox(null, _F("Object %s: bounding sphere computation failed!\n", node->GetName()), "Oops", MB_OK);

		// 5-6) Inertia tensor
		if(!ComputeInertiaTensor(*MeshInf))			IceCore::MessageBox(null, _F("Object %s: inertia tensor computation failed!\n", node->GetName()), "Oops", MB_OK);

		// 5-7) Consolidation
		if(!ConsolidateMesh(*MeshInf))				IceCore::MessageBox(null, _F("Object %s: mesh consolidation failed!\n", node->GetName()), "Oops", MB_OK);
	}
	else
	{
		// Here we have a non-convertible GeomObject.
		MeshInf->mIsConvertible	= false;

		// Update log text
		mArrayLog.StoreASCII("WARNING! Non convertible node.\n");
	}

	// Update members
	mStats.mNbGeomObjects++;

	// 6) Call the exporter
	SetExtStatus("calling export plug-in...");
	if(mExporter)	mExporter->ExportMesh(*MeshInf);
	DumpLogString();

	// 7) Animation
	// 7-1) PRS track
	SetExtStatus("exporting PRS data...");
	bool Status = AddPRSController(node, MeshInf);

	// 7-2) Visibility track
	SetExtStatus("exporting visibility data...");
	Status = AddVisibilityController(node, MeshInf);

	// 7-3) Morphing
	SetExtStatus("exporting morphing data...");
	Status = AddMorphController(node, MeshInf);

	// Test
/*	IIKControl* IK;

	IK = (IIKControl*)node->Animatable::GetInterface(I_IKCONTROL);
	if(IK)	mArrayLog.StoreASCII("Found IK1\n");

	IK = (IIKControl*)node->GetTMController()->GetInterface(I_IKCONTROL);
	if(IK)	mArrayLog.StoreASCII("Found IK2\n");

	IK = (IIKControl*)node->Animatable::GetInterface(I_IKCHAINCONTROL);
	if(IK)	mArrayLog.StoreASCII("Found IK3\n");

	IK = (IIKControl*)node->GetTMController()->GetInterface(I_IKCHAINCONTROL);
	if(IK)	mArrayLog.StoreASCII("Found IK4\n");

/*	int nb0 = node->NumSubs();
	mArrayLog.StoreASCII("Node: %d sub-anims\n", nb0);

	int nb1 = node->GetTMController()->NumSubs();
	mArrayLog.StoreASCII("Control: %d sub-anims\n", nb1);
*/
//	DumpSubAnimsNames(node);
/*	DumpSubAnimsNames(node->GetTMController());
	DumpSubAnimsNames(node->GetTMController()->SubAnim(IMAX_POSITION));
	DumpSubAnimsNames(node->GetTMController()->SubAnim(IMAX_ROTATION));
	DumpSubAnimsNames(node->GetTMController()->SubAnim(IMAX_SCALE));
*/
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Builds a MAX native mesh.
 *	\param		mesh_desc		[in] the mesh info structure
 *	\param		node_object		[in] the current mesh object
 *	\param		node			[in] the MAX node
 *	\return		Self-reference
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::BuildMesh(MeshDescriptor& mesh_desc, Object* node_object, INode* node)
{
	// Checkings
	if(!node_object || !node_object->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))	return false;

	// ### instance
#ifdef SUPPORT_OLD_LIGHTMAPPER
	LM_Mesh* LM=mMapper ? mMapper->GetMesh(mesh_desc.mObjectID) : null;

	if(LM && LM->mColors2 && LM->mNbColors)
	{
		mesh_desc.mNbColors	= LM->mNbColors;
		mesh_desc.mColors	= new NakedPoint[LM->mNbColors];
		CHECKALLOC(mesh_desc.mColors);

		CopyMemory(mesh_desc.mColors, LM->mColors2, LM->mNbColors*sizeof(NakedPoint));
	}

LM=null; // ### temp discard
#endif

	// Get time info back
	TimeValue CurrentTime = mTimeInfo.mCurrentTime;

	// Update dialog
	SetExtStatus("creating triangle lists...");

	MAXMeshPointer MAXMesh(node_object, CurrentTime);

	// Fill native struct
	mesh_desc.CreateOriginalMesh();

	MAXNativeMesh* M = mesh_desc.mOriginalMesh;
	if(!M)	return false;

	// Get vanilla data
	udword NbFaces = MAXMesh->getNumFaces();
	udword NbVerts = MAXMesh->getNumVerts();
	// And keep track of it
	M->mNbFaces	= NbFaces;
#ifdef OLD_FLEX_FACES
	NakedPoint*& Verts = M->mVerts;
#else
	NakedPoint*& Verts = M->mPointSurface.mVerts;
#endif

//	M->mNbVerts	= NbVerts;
//	Verts		= NbVerts ? new NakedPoint[NbVerts]	: null;
//	if(Verts)	CopyMemory(Verts, MAXMesh->verts, NbVerts * sizeof(NakedPoint));
	M->SetVerts(NbVerts, (const NakedPoint*)MAXMesh->verts);

	// Setup extra vertex data
	SetupVertexData(mesh_desc, node_object);

/*
	if(ExportVtxCol && LM)
	{
		mesh_desc.mOriginalMesh.mNbCVerts = mesh_desc.mOriginalMesh.mNbVerts;
	}
*/

/*
	if(ExportVtxCol && LM)
	{
		CopyMemory(mesh_desc.mOriginalMesh.mCVerts,	LM->mColors,	mesh_desc.mOriginalMesh.mNbVerts * sizeof(Point));
	}
	else
*/

	Point Scaling = GetScaling();

	// User-defined frame conversion
//	if(mExporter)	mExporter->AdjustScale(Scaling.x, Scaling.y, Scaling.z);
	Scaling *= mSettings->mGlobalScale;

	// Remove possible non-uniform scaling and convert to D3D
	for(udword i=0;i<NbVerts;i++)
	{
		Verts[i].x *= Scaling.x;
		Verts[i].y *= Scaling.y;
		Verts[i].z *= Scaling.z;

		// Possible extra XForm
		if(HasXForm())
		{
			Point p = Point(Verts[i].x, Verts[i].y, Verts[i].z) * GetXForm();
			Verts[i].x = p.x;
			Verts[i].y = p.y;
			Verts[i].z = p.z;
		}

		// D3D transform
		if(mSettings->mConvertToD3D)	Swap(IR(Verts[i].y), IR(Verts[i].z));
/*		{
			IR(mesh_desc.mOriginalMesh.mVerts[i].y) ^= IR(mesh_desc.mOriginalMesh.mVerts[i].z);
			IR(mesh_desc.mOriginalMesh.mVerts[i].z) ^= IR(mesh_desc.mOriginalMesh.mVerts[i].y);
			IR(mesh_desc.mOriginalMesh.mVerts[i].y) ^= IR(mesh_desc.mOriginalMesh.mVerts[i].z);
		}*/

		// User-defined frame conversion
		if(mExporter)	mExporter->ConvertPoint(Verts[i].x, Verts[i].y, Verts[i].z);
	}

	// Set topologies
	SetupTopologies(node, mesh_desc, MAXMesh);

SetupExtraMap(mesh_desc, MAXMesh);

	// Test interface Meshmerizer
	if((mSettings->mMakeManifold || mProps.mForceManifold) && !mProps.mDiscardManifold)
	{
		// Local surface
		IndexedSurface IS;

		// Initialize surface
		bool Status = IS.Init(NbFaces, NbVerts, (Point*)Verts);
//		IS->SetParity(mesh_desc.mOriginalMesh.mParity);

		// Build surface
		for(udword i=0;i<NbFaces;i++)
		{
			Status = IS.SetFace(M->mFaces[i].VRef[0],
								M->mFaces[i].VRef[1+M->mParity],
								M->mFaces[i].VRef[2-M->mParity],
								i);
		}

		// Make manifold mesh
		IS.RemoveSingularEdges();

		// Get surface back
//		DELETEARRAY(M->mVerts);
		NbVerts = IS.GetNbVerts();
//		M->mVerts = new NakedPoint[NbVerts];
//		CopyMemory(M->mVerts, IS.GetVerts(), NbVerts * sizeof(NakedPoint));
//		M->mNbVerts = NbVerts;
		M->SetVerts(NbVerts, (const NakedPoint*)IS.GetVerts());

		const IndexedTriangle* Faces = IS.GetFaces();
		for(udword i=0;i<NbFaces;i++)
		{
			M->mFaces[i].VRef[0]			= Faces[i].mRef[0];
			M->mFaces[i].VRef[1+M->mParity]	= Faces[i].mRef[1];
			M->mFaces[i].VRef[2-M->mParity]	= Faces[i].mRef[2];
		}
	}

	// Reset pivot if needed - after the transform so that PRS & vertices and in the same space
	if(mesh_desc.mResetPivot)
	{
		// Here we're always a master. We want the pivot point to be at the geometric center.
		mArrayLog.StoreASCII("(Reseting master pivot)\n");

		// 1) Apply transform

		// Compute geometric center in local space, using native vertices
		Point LocalCenter;
		M->ComputeGeometricCenter(LocalCenter);

		// Move vertices. This invalidates the geometry, i.e. the already existing bounding volumes.
		// Fortunately at this point none of them should have been computed yet.
		Matrix4x4 Translate;
		Translate.Identity();
		Translate.SetTrans(-LocalCenter);
		M->Transform(Translate);

		// Update the position so that the transformed vertices stay in place
		Matrix4x4 LocalToWorld;
		MAXToICE(LocalToWorld, mesh_desc.mPrs);
		Point NewPos = LocalCenter * LocalToWorld;
		mesh_desc.mPrs.mPosition.x = NewPos.x;
		mesh_desc.mPrs.mPosition.y = NewPos.y;
		mesh_desc.mPrs.mPosition.z = NewPos.z;

		// Save local center to fix possible instances
		M->mInternal.x = LocalCenter.x;
		M->mInternal.y = LocalCenter.y;
		M->mInternal.z = LocalCenter.z;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Setups vertex data.
 *	\param		mesh_desc		[in/out] the mesh info structure
 *	\param		node_object		[in] the current mesh object
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::SetupVertexData(MeshDescriptor& mesh_desc, Object* node_object)
{
	MAXNativeMesh* M = mesh_desc.mOriginalMesh;
	if(!M)	return false;

	// Get time info back
	TimeValue CurrentTime = mTimeInfo.mCurrentTime;

	// Catch the mesh
	MAXMeshPointer MAXMesh(node_object, CurrentTime);

	// Fill native struct
	M->mParity		= mParity;
//	M->mNbTVerts	= mExportUVW	? MAXMesh->getNumTVerts()	: 0;
//	M->mNbCVerts	= mExportColors	? MAXMesh->getNumVertCol()	: 0;

//	if(mExportUVW)		M->mTVerts	= M->mNbTVerts	? new NakedPoint[M->mNbTVerts]	: null;
//	if(mExportColors)	M->mCVerts	= M->mNbCVerts	? new NakedPoint[M->mNbCVerts]	: null;

//	if(M->mTVerts)	CopyMemory(M->mTVerts,	MAXMesh->tVerts,	M->mNbTVerts * sizeof(NakedPoint));
//	if(M->mCVerts)	CopyMemory(M->mCVerts,	MAXMesh->vertCol,	M->mNbCVerts * sizeof(NakedPoint));

	if(mExportUVW)		M->SetTVerts(MAXMesh->getNumTVerts(), (const NakedPoint*)MAXMesh->tVerts);

	if(mExportColors)	M->SetCVerts(MAXMesh->getNumVertCol(), (const NakedPoint*)MAXMesh->vertCol);

#ifndef FLEXPORTER_MAX3
	// Handle vertex alpha - thanks to Jed Adams for the hint
	if(mExportColors)
	{
		// Check alpha-values are defined
		if(MAXMesh->mapSupport(MAP_ALPHA))
		{
			UVVert* AlphaArray = MAXMesh->mapVerts(MAP_ALPHA);
			if(AlphaArray)
			{
				udword NbVerts = MAXMesh->getNumVerts();	// i.e. one alpha value for each base vertex
				M->mVertexAlpha = new float[NbVerts];
				if(M->mVertexAlpha)
				{
					for(udword i=0;i<NbVerts;i++)	M->mVertexAlpha[i] = AlphaArray[i].x;
				}
			}
		}
	}
#endif
	return true;
}

// A method to handle extra mapping channels
bool MAXConverter::SetupExtraMap(MeshDescriptor& mesh_desc, const MAXMeshPointer& mesh)
{
//return true;
	MAXNativeMesh* M = mesh_desc.mOriginalMesh;
	if(!M)	return false;

	// "For mp in following: 0=vert colors, 1=original TVerts, 2&up = new map channels"
	udword NbMaps = mesh->getNumMaps();

	for(udword i=2;i<NbMaps;i++)
	{
		BOOL Support = mesh->mapSupport(i);
		int NbVerts = mesh->getNumMapVerts(i);

		UVVert* V = mesh->mapVerts(i);
		TVFace* F = mesh->mapFaces(i);

		// Replace TVerts with first valid channel
		if(!M->mNbTVerts && NbVerts && Support)
		{
			M->SetTVerts(NbVerts, (const NakedPoint*)V);

			udword NbFaces = M->GetNbFaces();
			
			for(udword j=0;j<NbFaces;j++)
			{
				M->mFaces[j].TRef[0] = F[j].t[0];
				M->mFaces[j].TRef[1] = F[j].t[1];
				M->mFaces[j].TRef[2] = F[j].t[2];
			}
			M->mFlags|=MESH_TFACE;
			M->mFlags|=MESH_UVW;
			return true;
		}
	}

	return true;
}

bool MAXConverter::SetupTopologies(INode* node, MeshDescriptor& mesh_desc, const MAXMeshPointer& mesh)
{
	MAXNativeMesh* M = mesh_desc.mOriginalMesh;
	if(!M)	return false;

	// Set topologies
	M->mFaces = new MAXFace[M->mNbFaces];

	// Get MAX pointers
	TVFace*	MAXtvFaces	= mesh->tvFace;
	TVFace*	MAXvcFaces	= mesh->vcFace;
	Face*	MAXFaces	= mesh->faces;
/*
	if(ExportVtxCol && LM)
	{
		mesh_desc.mOriginalMesh.mFlags|=MESH_CFACE|MESH_VERTEXCOLORS;
	}
*/
	// Compute mesh flags:
	// - doing it here even for non-topology stuff allows skins to benefit from this
	// - some flags may seem redundant with pointers but it's actually needed for binary formats (to import them)
	if(mSettings->mEdgeVis)			M->mFlags|=MESH_EDGEVIS;
	if(MAXFaces)					M->mFlags|=MESH_VFACE;
	if(MAXtvFaces)					M->mFlags|=MESH_TFACE;
	if(MAXvcFaces)					M->mFlags|=MESH_CFACE;
	if(mExportUVW && MAXtvFaces)	M->mFlags|=MESH_UVW;
	if(mSettings->mDiscardW)		M->mFlags|=MESH_WDISCARDED;
	if(mExportColors && MAXvcFaces)	M->mFlags|=MESH_VERTEXCOLORS;
	if(M->mVertexAlpha)				M->mFlags|=MESH_VERTEX_ALPHA;

	// Get final SMG settings
	bool UseSmg = mSettings->mUseSmgrp;			// Global settings
		if(mProps.mExportSMG)	UseSmg = true;	// Override
		if(mProps.mDiscardSMG)	UseSmg = false;	// Override

	// Loop through faces
udword MinRef = MAX_UDWORD;
udword MaxRef = MIN_UDWORD;
	for(udword i=0;i<M->mNbFaces;i++)
	{
		udword	MatID = ComputeMaterialID(node, mesh.GetMesh(), i, false);	// Absolute ID
		MatID = RemapMaterialID(MatID);
		udword Smg = UseSmg ? MAXFaces[i].getSmGroup() : CVT_DEFAULT_SMOOTHING_GROUP;

		udword*	VRefs = (udword*)MAXFaces[i].getAllVerts();
		udword* TRefs = (mExportUVW && MAXtvFaces)		?	(udword*)MAXtvFaces[i].getAllTVerts()	: null;
		udword* CRefs = (mExportColors && MAXvcFaces)	?	(udword*)MAXvcFaces[i].getAllTVerts()	: null;
/*
		if(ExportVtxCol && LM)	CRefs = VRefs;
*/
		// Compute edges visibility code
		ubyte EdgeVis=0;
		if(mSettings->mEdgeVis)
		{
			if(MAXFaces[i].getEdgeVis(0))	EdgeVis|=FLX_EDGE_0;
			if(MAXFaces[i].getEdgeVis(1))	EdgeVis|=FLX_EDGE_1;
			if(MAXFaces[i].getEdgeVis(2))	EdgeVis|=FLX_EDGE_2;
		}

		// Fill struct
		M->mFaces[i].VRef[0]	= VRefs ? VRefs[0] : -1;
		M->mFaces[i].VRef[1]	= VRefs ? VRefs[1] : -1;
		M->mFaces[i].VRef[2]	= VRefs ? VRefs[2] : -1;
		M->mFaces[i].TRef[0]	= TRefs ? TRefs[0] : -1;
		M->mFaces[i].TRef[1]	= TRefs ? TRefs[1] : -1;
		M->mFaces[i].TRef[2]	= TRefs ? TRefs[2] : -1;
		M->mFaces[i].CRef[0]	= CRefs ? CRefs[0] : -1;
		M->mFaces[i].CRef[1]	= CRefs ? CRefs[1] : -1;
		M->mFaces[i].CRef[2]	= CRefs ? CRefs[2] : -1;
		M->mFaces[i].MatID		= MatID;
		M->mFaces[i].Smg		= Smg;
		M->mFaces[i].EdgeVis	= EdgeVis;
//DEBUG
if(VRefs)
{
	if(VRefs[0]<MinRef)	MinRef = VRefs[0];
	if(VRefs[0]>MaxRef)	MaxRef = VRefs[0];
	if(VRefs[1]<MinRef)	MinRef = VRefs[1];
	if(VRefs[1]>MaxRef)	MaxRef = VRefs[1];
	if(VRefs[2]<MinRef)	MinRef = VRefs[2];
	if(VRefs[2]>MaxRef)	MaxRef = VRefs[2];
}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Builds a convex hull.
 *	\param		mesh_desc		[in] the mesh info structure
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::BuildConvexHull(MeshDescriptor& mesh_desc)
{
	// Checkings
	if(!mSettings->mConvexHull) return true;

	MAXNativeMesh* M = mesh_desc.mOriginalMesh;
	if(!M)	return false;

	// Update dialog
	SetExtStatus("computing convex hull...");

	ConvexHull Hull;

	// Create hull
	sdword NbVerts	= M->mNbVerts;
	const Point* Verts	= (const Point*)M->mVerts;

	CONVEXHULLCREATE CHC;
	CHC.NbVerts			= NbVerts;
	CHC.Vertices		= Verts;
	CHC.UnifyNormals	= true;
	CHC.ReduceVertices	= true;
	CHC.WordFaces		= false;
	bool Status = Hull.Compute(CHC);

	// Fill mesh structure
	M->mConvexHull = new CHull;
	CHECKALLOC(M->mConvexHull);

	M->mConvexHull->mNbVerts	= Hull.GetNbVerts();
	M->mConvexHull->mNbFaces	= Hull.GetNbFaces();

	M->mConvexHull->mVerts	= new NakedPoint[Hull.GetNbVerts()];
	CopyMemory(M->mConvexHull->mVerts, Hull.GetVerts(), Hull.GetNbVerts()*sizeof(NakedPoint));

	M->mConvexHull->mFaces	= new udword[Hull.GetNbFaces()*3];
	CopyMemory(M->mConvexHull->mFaces, Hull.GetFaces(), Hull.GetNbFaces()*sizeof(udword)*3);

	M->mFlags|=MESH_CONVEXHULL;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Builds a bounding sphere.
 *	\param		mesh_desc		[in] the mesh info structure
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::BuildBoundingSphere(MeshDescriptor& mesh_desc)
{
	// Checkings
	if(!mSettings->mBoundingSphere) return true;

	MAXNativeMesh* M = mesh_desc.mOriginalMesh;
	if(!M)	return false;

	// Update dialog
	SetExtStatus("computing bounding sphere...");

	// Create sphere
	udword NbVerts	= M->mNbVerts;
	const Point* Verts	= (const Point*)M->mVerts;

	Sphere BSphere;
	BSphereMethod Method = BSphere.Compute(NbVerts, Verts);

	M->mBSCenter.x	= BSphere.mCenter.x;
	M->mBSCenter.y	= BSphere.mCenter.y;
	M->mBSCenter.z	= BSphere.mCenter.z;
	M->mBSRadius	= BSphere.mRadius;

	M->mFlags|=MESH_BOUNDINGSPHERE;

	mArrayLog.StoreASCII(_F("Bounding sphere computed with method %d\n", Method));
	if(BSphere.mRadius<0.0f)	mArrayLog.StoreASCII("WARNING: negative bounding sphere radius!\n");

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Computes the inertia tensor.
 *	\param		mesh_desc		[in] the mesh info structure
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::ComputeInertiaTensor(MeshDescriptor& mesh_desc)
{
	// Checkings
	if(!mSettings->mInertiaTensor) return true;

	MAXNativeMesh* M = mesh_desc.mOriginalMesh;
	if(!M)	return false;

	MAXFace* f = M->mFaces;
	if(!f)	return false;

	// Update dialog
	SetExtStatus("computing inertia tensor...");

	VolumeIntegrator vi;

	// Creation structure
	udword* Faces = new udword[M->mNbFaces*3];
	CHECKALLOC(Faces);

	ubyte c = (ubyte)M->mParity;
	for(udword i=0;i<M->mNbFaces;i++)
	{
		Faces[i*3+0] = f[i].VRef[0];
		Faces[i*3+1] = f[i].VRef[1+c];
		Faces[i*3+2] = f[i].VRef[2-c];
	}

	TENSORCREATE create;
	create.mDensity	= 1.0;
	create.mNbVerts	= M->mNbVerts;
	create.mNbFaces	= M->mNbFaces;
	create.mVerts	= (Point*)M->mVerts;
	create.mFaces	= Faces;

	vi.Init(create);

	// Compute inertia tensor
	INTEGRALSRESULT Result;
	vi.ComputeVolumeIntegrals(Result);

	// Free local ram
	DELETEARRAY(Faces);

	// Fill mesh structure
	M->mTensor = new ITensor;
	CHECKALLOC(M->mTensor);

	M->mTensor->COM.x	= Result.mCOM.x;
	M->mTensor->COM.y	= Result.mCOM.y;
	M->mTensor->COM.z	= Result.mCOM.z;

	M->mTensor->Mass	= (float)Result.mMass;

	M->mTensor->InertiaTensor[0][0]	=	(float)Result.mInertiaTensor[0][0];
	M->mTensor->InertiaTensor[0][1]	=	(float)Result.mInertiaTensor[0][1];
	M->mTensor->InertiaTensor[0][2]	=	(float)Result.mInertiaTensor[0][2];
	M->mTensor->InertiaTensor[1][0]	=	(float)Result.mInertiaTensor[1][0];
	M->mTensor->InertiaTensor[1][1]	=	(float)Result.mInertiaTensor[1][1];
	M->mTensor->InertiaTensor[1][2]	=	(float)Result.mInertiaTensor[1][2];
	M->mTensor->InertiaTensor[2][0]	=	(float)Result.mInertiaTensor[2][0];
	M->mTensor->InertiaTensor[2][1]	=	(float)Result.mInertiaTensor[2][1];
	M->mTensor->InertiaTensor[2][2]	=	(float)Result.mInertiaTensor[2][2];

	M->mTensor->COMInertiaTensor[0][0]	=	(float)Result.mCOMInertiaTensor[0][0];
	M->mTensor->COMInertiaTensor[0][1]	=	(float)Result.mCOMInertiaTensor[0][1];
	M->mTensor->COMInertiaTensor[0][2]	=	(float)Result.mCOMInertiaTensor[0][2];
	M->mTensor->COMInertiaTensor[1][0]	=	(float)Result.mCOMInertiaTensor[1][0];
	M->mTensor->COMInertiaTensor[1][1]	=	(float)Result.mCOMInertiaTensor[1][1];
	M->mTensor->COMInertiaTensor[1][2]	=	(float)Result.mCOMInertiaTensor[1][2];
	M->mTensor->COMInertiaTensor[2][0]	=	(float)Result.mCOMInertiaTensor[2][0];
	M->mTensor->COMInertiaTensor[2][1]	=	(float)Result.mCOMInertiaTensor[2][1];
	M->mTensor->COMInertiaTensor[2][2]	=	(float)Result.mCOMInertiaTensor[2][2];

	M->mFlags|=MESH_INERTIATENSOR;

	// Check result
//	if(IsNAN(Result.COM.x))	return false;
//	if(IsNAN(Result.COM.y))	return false;
//	if(IsNAN(Result.COM.z))	return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Consolidates the mesh.
 *	\param		mesh_desc		[in] the mesh info structure
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::ConsolidateMesh(MeshDescriptor& mesh_desc)
{
	// Checkings
	if(!mSettings->mConsolidateMesh && !mProps.mForceConsolidation)	return true;
	if(mProps.mDiscardConsolidation)								return true;

	// Update dialog
	SetExtStatus("mesh consolidation...");

	// Consolidation
	return mesh_desc.Consolidate(mSettings, mProps.mForceNormals, mProps.mDiscardNormals, mProps.mMorph);
}
