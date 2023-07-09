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

using namespace IceExporter;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Converts a so-called "derived object". Includes non-collapsed objects as well as Character Studio's objects.
 *	We force GeomObjects to collapse and call the right export method, unless the derived object is something we know how to handle.
 *
 *	\param		node	[in] a MAX node
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddDerivedObject(INode* node)
{
	// Get time info back
	TimeValue CurrentTime = mTimeInfo.mCurrentTime;

	// Update log text
	mArrayLog.StoreASCII("Exporting derived object %s\n", node->GetName());

	// We allow users to discard it so that they can export a morphed mesh instead (in case their engine doesn't support skinning)
	if(mSettings->mExportPHYSIQUE)
	{
		// Look for Physique modifier
		Modifier* phyMod = FindPhysiqueModifier(node);
		if(phyMod)
		{
			// Here we're only supposed to deal with Character Studio's deformed skin bodies.
			SetExtStatus("exporting Character Studio's Physique...");
			mArrayLog.StoreASCII("(Found Physique modifier)\n");

			// Create a Physique Export Interface for the given Physique Modifier
			IPhysiqueExport* phyExport = (IPhysiqueExport*)phyMod->GetInterface(I_PHYINTERFACE);	// I_PHYEXPORT in CS3
			if(phyExport)
			{
				// Get the physique version number.  
				// If the version number is > 30 you may have floating bones
				int CSVersion = phyExport->Version();	// Should be at least 22
				Log(_F("Found Character Studio version %d\n", CSVersion));

#ifdef FLEXPORTER_MAX3
				if(CSVersion>=30)	{ mArrayLog.StoreASCII("Unsupported Character Studio version!\n");	return false;	}
#endif

#ifdef FLEXPORTER_MAX4
				if(CSVersion<30)	{ mArrayLog.StoreASCII("Unsupported Character Studio version!\n");	return false;	}
#endif

				// Create info structure
				MeshDescriptor* MeshInf = (MeshDescriptor*)mGlobalInfo.CreateDescriptor(node, FLX_OBJ_MESH);
				AddHeader(node, MeshInf);

				MeshInf->mIsCollapsed	= false;
				MeshInf->mIsSkeleton	= false;
				MeshInf->mIsInstance	= false;	// Not supported for skins
				MeshInf->mIsTarget		= false;
				MeshInf->mIsConvertible	= true;
				MeshInf->mIsSkin		= true;

				bool HasBeenConverted = false;
				// Create a ModContext Export Interface for the specific node of the Physique Modifier
				IPhyContextExport* mcExport = (IPhyContextExport*)phyExport->GetContextInterface(node);
				if(mcExport)
				{
					IDerivedObject* M = (IDerivedObject*)node->GetObjectRef();
					Object* M2 = M->GetObjRef();	// ......what the..?!
					if(M2->CanConvertToType(triObjectClassID))
					{
						// Export material
						if(!AddMetaMaterial(node))	IceCore::MessageBox(null, _F("Object %s has an invalid material!\n", node->GetName()), "Oops", MB_OK);

						// We convert all vertices to Rigid. (### Is it possible to do something else?)
						mcExport->ConvertToRigid(TRUE);

						// Allow blending to export multi-link assignments (### safe ?)
						mcExport->AllowBlending(TRUE);

						// Build the skin
						if(!BuildSkin(*MeshInf, M2, node, mcExport))	IceCore::MessageBox(null, _F("Object %s failed!\n", node->GetName()), "Oops", MB_OK);

						// Convex hull / BSphere / Inertia tensors not for skins!

						// Consolidation
						if(!ConsolidateMesh(*MeshInf))	IceCore::MessageBox(null, _F("Object %s: mesh consolidation failed!\n", node->GetName()), "Oops", MB_OK);

						HasBeenConverted = true;
					}
					phyExport->ReleaseContextInterface(mcExport);
				}
				phyMod->ReleaseInterface(I_PHYINTERFACE, phyExport);

				// Update members
				mStats.mNbDerivedObjects++;

				// Call the exporter
				if(HasBeenConverted)
				{
					SetExtStatus("calling export plug-in...");
					if(mExporter)	mExporter->ExportMesh(*MeshInf);
					DumpLogString();
				}
			}
			else mArrayLog.StoreASCII("Can't find PHYSIQUE interface!\n");
			return true;
		}
	}

	if(mSettings->mExportSKIN)
	{
		// Look for Skin modifier
		Modifier* SkinMod = FindSkinModifier(node);
		if(SkinMod)
		{
			// Found a Skin modifier - here we're only supposed to deal with MAX skins.
			SetExtStatus("exporting MAX skin...");
			mArrayLog.StoreASCII("(Found Skin modifier)\n");

			// Catch skin interface
			ISkin* pSkin = (ISkin*)SkinMod->GetInterface(I_SKIN);
			if(pSkin)
			{
				// Create info structure
				MeshDescriptor* MeshInf = (MeshDescriptor*)mGlobalInfo.CreateDescriptor(node, FLX_OBJ_MESH);
				AddHeader(node, MeshInf);

				MeshInf->mIsCollapsed	= false;
				MeshInf->mIsSkeleton	= false;
				MeshInf->mIsInstance	= false;	// Not supported for skins
				MeshInf->mIsTarget		= false;
				MeshInf->mIsConvertible	= true;
				MeshInf->mIsSkin		= true;

				// Catch context interface
				ISkinContextData* pSkinContext = pSkin->GetContextInterface(node);
				if(pSkinContext)
				{
					IDerivedObject* M = (IDerivedObject*)node->GetObjectRef();
					Object* M2 = M->GetObjRef();	// ......what the..?!
					if(M2->CanConvertToType(triObjectClassID))
					{
						// Export material
						if(!AddMetaMaterial(node))	IceCore::MessageBox(null, _F("Object %s has an invalid material!\n", node->GetName()), "Oops", MB_OK);

						// Build the skin
						if(!BuildSkin(*MeshInf, M2, node, pSkin, pSkinContext))	IceCore::MessageBox(null, _F("Object %s failed!\n", node->GetName()), "Oops", MB_OK);

						// Convex hull / BSphere / Inertia tensors not for skins!

						// Consolidation
						if(!ConsolidateMesh(*MeshInf))	IceCore::MessageBox(null, _F("Object %s: mesh consolidation failed!\n", node->GetName()), "Oops", MB_OK);
					}

	/*				int numPoints = pSkinContext->GetNumPoints();
					for(int i = 0; i < numPoints;  i++)
					{
						int numBones = pSkinContext->GetNumAssignedBones(i);
						for(int j = 0; j < numBones; j++)
						{
							int assignedBone = pSkinContext->GetAssignedBone(i, j);
							if(assignedBone < 0)
								continue;

							INode *pBone = pSkin->GetBone(assignedBone);
			//					int boneIdx  = GetBoneIndex(ip->GetRootNode(), pBone);
			//
			//					IndexWeight *idxWeight = new IndexWeight;
			//					idxWeight->index  = i;
			//					idxWeight->weight = pSkinContext->GetBoneWeight(i, j);
			//					boneWeights[boneIdx].weights.Append(1, &idxWeight);
						}
					}
	*/
				}
				SkinMod->ReleaseInterface(I_SKIN, pSkin);

				// Update members
				mStats.mNbDerivedObjects++;

				// Call the exporter
				SetExtStatus("calling export plug-in...");
				if(mExporter)	mExporter->ExportMesh(*MeshInf);
				DumpLogString();
			}
			else mArrayLog.StoreASCII("Can't find skin interface!\n");
			return true;
		}
	}

	{
		// Here, Physique/Skin modifier hasn't been found (or is not wanted). The object is probably a non-collapsed GeomObject.
		// We collapse it ourselves by calling EvalWorldState().

		ObjectState os = node->EvalWorldState(CurrentTime);	// Collapse the object...
		Object* NodeObject = os.obj;						// ...and get it back

		// Hmmm, actually all references end up here (even for cameras, helpers, etc)
		// So in the end, here's the difference between instance and reference:
		// - instances are detected as direct objects
		// - references are detected as derived objects
		switch(NodeObject->SuperClassID())
		{
			case GEN_DERIVOB_CLASS_ID:
				mArrayLog.StoreASCII("Derived object in derived object! Can't export this node.\n");
				break;

			case GEOMOBJECT_CLASS_ID :
				// Object is now a true GeomObject, we can try to export it.
				if(NodeObject->CanConvertToType(triObjectClassID))
				{
					return AddGeomObject(node, NodeObject);
				}
				else mArrayLog.StoreASCII("Can't convert derived mesh to TriObject! Can't export this node.\n");
				break;

			case CAMERA_CLASS_ID :
				// Probably a camera reference
				return AddCamera(node, NodeObject);
				break;

			case LIGHT_CLASS_ID:
				// Probably a light reference
				return AddLight(node, NodeObject);
				break;

			case SHAPE_CLASS_ID:
				// Probably a shape reference
				return AddShape(node, NodeObject);
				break;

			case HELPER_CLASS_ID:
				// Probably a helper reference
				return AddHelper(node, NodeObject);
				break;

			default:
				mArrayLog.StoreASCII("Unknown ClassID in derived object! Can't export this node.\n");
				break;
		}

		// Update members
		mStats.mNbUnknowns++;
	}
	return true;
}

#define DUMP_BONE																		\
	/* Catch the character ID */														\
	if(mesh_desc.mCharID==INVALID_ID)	mesh_desc.mCharID = mGlobalInfo.GetCharacterID(Bone);	\
																						\
	Point Scaling = GetScalingFrom(Bone, true);											\
																						\
	/* User-defined frame conversion */													\
	/*if(mExporter)	mExporter->AdjustScale(Scaling.x, Scaling.y, Scaling.z);	*/		\
																						\
	Offset.x *= Scaling.x;																\
	Offset.y *= Scaling.y;																\
	Offset.z *= Scaling.z;																\
																						\
	Offset.x *= mSettings->mGlobalScale;												\
	Offset.y *= mSettings->mGlobalScale;												\
	Offset.z *= mSettings->mGlobalScale;												\
																						\
	/* (1.13) Convert offset vectors to D3D */											\
	if(mSettings->mConvertToD3D)														\
	{																					\
		TSwap(Offset.y, Offset.z);														\
	}																					\
																						\
	/* (1.15) User-defined frame conversion */											\
	if(mExporter)	mExporter->ConvertPoint(Offset.x, Offset.y, Offset.z);				\
																						\
	OffsetVectors.AddVertex(Offset.x, Offset.y, Offset.z);								\
	BonesCSID.Add(GetCSID(Bone));														\
	BonesLocalID.Add(mGlobalInfo.GetNodeID(Bone));										\
	BonesNames.Add(udword(Bone->GetName()));

bool MAXConverter::BuildSkeleton(MeshDescriptor& mesh_desc)
{
	// Build skeletal information
	if(mesh_desc.mCharID==INVALID_ID)
	{
		mesh_desc.mCharID = 0;
		mArrayLog.StoreASCII("ERROR: Invalid Character ID!\n");
	}

	MAXNativeMesh* M = mesh_desc.mOriginalMesh;
	if(!M)	return false;

	// Catch character info
	const CharInfo* CI = mGlobalInfo.GetCharacterInfo();
	if(!CI)	return false;

	udword NbBones			= CI[mesh_desc.mCharID].mNbNodes;
	udword NbVirtualBones	= CI[mesh_desc.mCharID].mNbVirtualBones;
	udword Total			= GetCSTotalNbNodes();
	if(NbBones+NbVirtualBones)
	{
		M->mSkeleton = new MAXSkeleton(NbBones+NbVirtualBones);
		if(M->mSkeleton)
		{
			udword LocalID = 0;
			for(udword i=0;i<Total;i++)
			{
				BoneInfo* CurBone = &CI[mesh_desc.mCharID].mSkeleton[i];
				if(CurBone->mBone)
				{
					M->mSkeleton->mID[LocalID*2+0] = CurBone->mCSID;
					M->mSkeleton->mID[LocalID*2+1] = CurBone->mpCSID;
					LocalID++;
				}
			}
		}
	}
	return true;
}

// Fill the structure with skin info
static void DumpSkinData(
MeshDescriptor& mesh_desc,
const Vertices& OffsetVectors,
 const Container& BonesCSID,
 const Container& BonesLocalID,
 const Container& BonesNames,
 const Container& BonesNb,
 const Container& Weights,
 bool UseMultipleBones
 )
{
	MAXNativeMesh* M = mesh_desc.mOriginalMesh;
	if(!M)	return;

	//if(!BonesNb.GetOffset())
	if(!UseMultipleBones)
	{
		// The skin has one bone/vertex only. Weights & BonesNb arrays are actually useless.
		M->mFlags|=MESH_ONEBONEPERVERTEX;

		// Vectors
		M->mOffsetVectors = new NakedPoint[M->mNbVerts];
		CopyMemory(M->mOffsetVectors, OffsetVectors.GetVertices(), M->mNbVerts*sizeof(NakedPoint));

		// Bones CSIDs
		M->mBonesCSID = new udword[M->mNbVerts];
		CopyMemory(M->mBonesCSID, BonesCSID.GetEntries(), M->mNbVerts*sizeof(udword));

		// Bones local IDs [version 1.14]
		M->mBonesLocalID = new udword[M->mNbVerts];
		CopyMemory(M->mBonesLocalID, BonesLocalID.GetEntries(), M->mNbVerts*sizeof(udword));

		// Bones names [version 1.14]
		M->mBonesName = new const char*[M->mNbVerts];
		CopyMemory(M->mBonesName, BonesNames.GetEntries(), M->mNbVerts*sizeof(udword));
	}
	else
	{
		// The skin has multiple bones/vertex.

		// Bones Nb: there's always one #bones/vertex, so we know the size needed.
		M->mBonesNb = new udword[M->mNbVerts];
		udword NbBonesNb = BonesNb.GetNbEntries();
		ASSERT(NbBonesNb==M->mNbVerts);
		CopyMemory(M->mBonesNb, BonesNb.GetEntries(), M->mNbVerts*sizeof(udword));

		// Check: compute the sum of all #bones
		udword CheckSum=0;
		for(udword i=0;i<M->mNbVerts;i++)	CheckSum+=M->mBonesNb[i];

		// The CheckSum must be equal to the sizes of the remaining arrays.
		udword BonesIDSize = BonesCSID.GetNbEntries();
		udword WeightsSize = Weights.GetNbEntries();
		udword VectorsSize = OffsetVectors.GetNbVertices();
		ASSERT(BonesIDSize==CheckSum);
		ASSERT(WeightsSize==CheckSum);
		ASSERT(VectorsSize==CheckSum);

		// Bones CSIDs
		M->mBonesCSID = new udword[BonesIDSize];
		CopyMemory(M->mBonesCSID, BonesCSID.GetEntries(), BonesIDSize*sizeof(udword));

		// Bones local IDs [version 1.14]
		M->mBonesLocalID = new udword[BonesIDSize];
		CopyMemory(M->mBonesLocalID, BonesLocalID.GetEntries(), BonesIDSize*sizeof(udword));

		// Bones names [version 1.14]
		M->mBonesName = new const char*[BonesIDSize];
		CopyMemory(M->mBonesName, BonesNames.GetEntries(), BonesIDSize*sizeof(udword));

		// Weights
		M->mWeights = new float[WeightsSize];
		CopyMemory(M->mWeights, Weights.GetEntries(), WeightsSize*sizeof(float));

		// Vectors
		M->mOffsetVectors = new NakedPoint[VectorsSize];
		CopyMemory(M->mOffsetVectors, OffsetVectors.GetVertices(), VectorsSize*sizeof(NakedPoint));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	A method to build a skin.
 *	\param		
 *	\return		
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::BuildSkin(MeshDescriptor& mesh_desc, Object* node_object, INode* node, IPhyContextExport* pc_export)
{
	// Get time info back
	TimeValue CurrentTime = mTimeInfo.mCurrentTime;

	udword NbVerts = pc_export->GetNumberVertices();

	MAXMeshPointer MAXMesh(node_object, CurrentTime);

// Check
udword NbVerts2 = MAXMesh->getNumVerts();
ASSERT(NbVerts2==NbVerts);
Point3* pp = MAXMesh->verts;

	// Reset character ID
	mesh_desc.mCharID = INVALID_ID;

	// Fill native struct
	mesh_desc.CreateOriginalMesh();

	MAXNativeMesh* M = mesh_desc.mOriginalMesh;
	if(!M)	return false;

	M->mNbFaces	= MAXMesh->getNumFaces();
	M->mNbVerts	= NbVerts;
	SetupVertexData(mesh_desc, node_object);

	// Physique stuff
	Vertices	OffsetVectors;
	Container	BonesCSID, BonesLocalID, BonesNames;
	Container	BonesNb;
	Container	Weights;

	INIT_PERCENTS
	bool UseMultipleBones = false;
	// Loop through vertices
	for(udword i=0;i<M->mNbVerts;i++)
	{
		UPDATE_PERCENTS(i, NbVerts, "building skin... ")

		// Get vertex
		IPhyVertexExport* vtxExport = pc_export->GetVertexInterface(i);
		if(vtxExport)
		{
			// Need to check if vertex has blending
			if(vtxExport->GetVertexType() & BLENDED_TYPE)
			{
				// Here, each vertex is linked to multiple bones.
				IPhyBlendedRigidVertex* vtxBlend = (IPhyBlendedRigidVertex*)vtxExport;

				udword NbBones = vtxBlend->GetNumberNodes();	// Number of driving bones
				BonesNb.Add(NbBones);
				for(long n=0;n<NbBones;n++)
				{
					INode* Bone		= vtxBlend->GetNode(n);
					Point3 Offset	= vtxBlend->GetOffsetVector(n);
					float Weight	= vtxBlend->GetWeight(n);

					DUMP_BONE
					Weights.Add(Weight);
				}
				pc_export->ReleaseVertexInterface(vtxExport);
				vtxExport=null;
				UseMultipleBones = true;
			}
			else
			{
				// Here, each vertex is linked to a single bone.
				IPhyRigidVertex* vtxNoBlend = (IPhyRigidVertex*)vtxExport;

				INode* Bone		= vtxNoBlend->GetNode();			// The bone which controls the vertex
				Point3 Offset	= vtxNoBlend->GetOffsetVector();	// The offset vector

//The offset vectors you get from Physique are the vertex positions in space of the given bone the offset vector is related to. 
/*
// Ok in Figure Mode
Matrix3 NodeMat = node->GetObjectTM(CurrentTime);
Matrix3 BoneMat = Bone->GetObjectTM(CurrentTime);
//Matrix3 NodeMat = node->GetObjTMAfterWSM(CurrentTime);
//Matrix3 BoneMat = Bone->GetObjTMAfterWSM(CurrentTime);
Point3 p = (pp[i] * NodeMat) * Inverse(BoneMat);
*/
				DUMP_BONE

				pc_export->ReleaseVertexInterface(vtxExport);
				vtxExport = null;

				// Update arrays anyway, so that it works for skins mixing BLENDED_TYPE & non blended ones. (à la Amaury)
				Weights.Add(1.0f);
				BonesNb.Add((udword)1);
			}
		}
	}

	// Fill the structure with skin info
	DumpSkinData(mesh_desc, OffsetVectors, BonesCSID, BonesLocalID, BonesNames, BonesNb, Weights, UseMultipleBones);

	// Set topologies
	SetupTopologies(node, mesh_desc, MAXMesh);

	// Build skeletal information
	BuildSkeleton(mesh_desc);

	return true;
}











/*
DEJAVIEW:

	// write the mesh vertices
	meshHdr.vert_cnt = pMesh->getNumVerts();
	meshHdr.vert_ofs = ftell(stream);
	for(int i = 0; i < meshHdr.vert_cnt; i++)
	{
		Point3 pnt = pMesh->getVert(i) * tm;
		FixCoordSys(pnt);
		CheckBBox(pnt, bboxMin, bboxMax);
		fwrite(&pnt.x, sizeof(float), 3, stream);
	}

bool MeshExp::GetPhysiqueWeights(INode *pNode, Modifier *pMod, BoneWeights *boneWeights)
{
	// create a Physique Export Interface for the given Physique Modifier
	IPhysiqueExport *phyExport = (IPhysiqueExport*)pMod->GetInterface(I_PHYINTERFACE);
	if(phyExport)
	{
		// create a ModContext Export Interface for the specific node of the Physique Modifier
		IPhyContextExport *mcExport = (IPhyContextExport*)phyExport->GetContextInterface(pNode);
		mcExport->ConvertToRigid(TRUE);
		mcExport->AllowBlending(TRUE);

		if(mcExport)
		{
			int numverts = mcExport->GetNumberVertices();
			for(int i = 0; i < numverts; i++)
			{
				IPhyVertexExport *vtxExport = (IPhyVertexExport*)mcExport->GetVertexInterface(i);
				if(vtxExport)
				{
					INode *bone = NULL;

					int vtxType = vtxExport->GetVertexType();
					if(vtxType == RIGID_TYPE)
					{
						bone = ((IPhyRigidVertex*)vtxExport)->GetNode();
						int boneIdx = GetBoneIndex(ip->GetRootNode(), bone);

						IndexWeight *idxWeight = new IndexWeight;
						idxWeight->index = i;
						idxWeight->weight = 1.f;
						boneWeights[boneIdx].weights.Append(1, &idxWeight);
					}
					else if(vtxType == RIGID_BLENDED_TYPE)
					{
						IPhyBlendedRigidVertex *vtx = (IPhyBlendedRigidVertex*)vtxExport;
						for(int j = 0; j < vtx->GetNumberNodes(); j++)
						{
							bone = vtx->GetNode(j);
							int boneIdx = GetBoneIndex(ip->GetRootNode(), bone);

							IndexWeight *idxWeight = new IndexWeight;
							idxWeight->index = i;
							idxWeight->weight = vtx->GetWeight(j);
							boneWeights[boneIdx].weights.Append(1, &idxWeight);
						}
					}
				}
			}

			phyExport->ReleaseContextInterface(mcExport);
		}

		pMod->ReleaseInterface(I_PHYINTERFACE, phyExport);
	}
	return true;
}

// ============================================================================
bool MeshExp::GetSkinWeights(INode *pNode, Modifier *pMod, BoneWeights *boneWeights)
{
	ISkin *pSkin = (ISkin*)pMod->GetInterface(I_SKIN);
	if(pSkin == NULL)
		return false;

	ISkinContextData *pSkinContext = pSkin->GetContextInterface(pNode);
	if(pSkinContext == NULL)
		return false;

	int numPoints = pSkinContext->GetNumPoints();
	for(int i = 0; i < numPoints;  i++)
	{
		int numBones = pSkinContext->GetNumAssignedBones(i);
		for(int j = 0; j < numBones; j++)
		{
			int assignedBone = pSkinContext->GetAssignedBone(i, j);
			if(assignedBone < 0)
				continue;

			INode *pBone = pSkin->GetBone(assignedBone);
			int boneIdx  = GetBoneIndex(ip->GetRootNode(), pBone);

			IndexWeight *idxWeight = new IndexWeight;
			idxWeight->index  = i;
			idxWeight->weight = pSkinContext->GetBoneWeight(i, j);
			boneWeights[boneIdx].weights.Append(1, &idxWeight);
		}
	}

	pMod->ReleaseInterface(I_SKIN, pSkin);

	return true;
}

// ============================================================================
void CMesh::Deform(CSkeleton *skel)
{
	if(!skel->IsLoaded() || m_numBones != skel->NumBones())
	{
		memcpy(m_vertCache, m_verts, sizeof(vertex_t)*m_numVerts);
		return;
	}

	vector_t vec, baseVtx;
	CIndexWeight *iw = NULL;
	float *cacheVtx = NULL;

	memset(m_vertCache, 0, sizeof(vertex_t)*m_numVerts);
	for(int i = 0; i < m_numBones; i++)
	{
		CBone *bone = &skel->m_bones[i];

		for(int j = 0; j < m_boneWeights[i].numWeights; j++)
		{
			iw = &m_boneWeights[i].weights[j];

			VectorMultiply(baseVtx, bone->m_baseTM, m_verts[iw->index]);
			VectorMultiply(vec, bone->m_cacheTM, baseVtx);

			cacheVtx = m_vertCache[iw->index];
			VectorScale(vec, vec, iw->weight);
			VectorAdd(cacheVtx, cacheVtx, vec);
		}
	}
}



*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	A method to build a skin.
 *	\param		
 *	\return		
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::BuildSkin(MeshDescriptor& mesh_desc, Object* node_object, INode* node, ISkin* skin, ISkinContextData* skin_context)
{
	// Get time info back
	TimeValue CurrentTime = mTimeInfo.mCurrentTime;

	udword NbVerts = skin_context->GetNumPoints();

	MAXMeshPointer MAXMesh(node_object, CurrentTime);

// Check
udword NbVerts2 = MAXMesh->getNumVerts();
//NbVerts = NbVerts2;	// topology seems to index this
Point3* pp = MAXMesh->verts;

Interval	Inter	= FOREVER;	// MAX validity interval
Matrix3 CurMat	= node->GetObjectTM(CurrentTime, &Inter);

	// Reset character ID
	mesh_desc.mCharID = INVALID_ID;

	mesh_desc.CreateOriginalMesh();

	MAXNativeMesh* M = mesh_desc.mOriginalMesh;
	if(!M)	return false;

	// Fill native struct
	M->mNbFaces	= MAXMesh->getNumFaces();
	M->mNbVerts	= NbVerts2;
	SetupVertexData(mesh_desc, node_object);

	// Skin stuff
	Vertices	OffsetVectors;
	Container	BonesCSID, BonesLocalID, BonesNames;
	Container	BonesNb;
	Container	Weights;

	INIT_PERCENTS
int MaxVertexIndex=0;
int j=0;
	// Loop through vertices
//	for(udword i=0;i<mesh_desc.mOriginalMesh.mNbVerts;i++)	// ###
	for(udword i=0;i<NbVerts;i++)
	{
		UPDATE_PERCENTS(i, NbVerts, "building skin... ")

		// For the Skin modifier, we always have "blended" skins
		udword Nb = skin_context->GetNumAssignedBones(i);	// Number of driving bones
		udword NbBones = 0;	// Number of valid bones
		for(long n=0;n<Nb;n++)
		{
			int AssignedBoneIndex = skin_context->GetAssignedBone(i, n);
			if(AssignedBoneIndex<0)	continue;
//			if(AssignedBoneIndex<0)	AssignedBoneIndex = 0;	// from a Casey Muratori slide
if(AssignedBoneIndex>MaxVertexIndex)	MaxVertexIndex=AssignedBoneIndex;
			INode* Bone = skin->GetBone(AssignedBoneIndex);

//			Point3 Offset	= vtxBlend->GetOffsetVector(n);
			Point3 Offset(0,0,0);
Offset = pp[i] * CurMat;
Offset = pp[i];
//Matrix3 BoneMat = Bone->GetObjectTM(CurrentTime);
//Offset = (pp[i] * CurMat) * Inverse(BoneMat);
//			float Weight	= vtxBlend->GetWeight(n);
			float Weight = skin_context->GetBoneWeight(i, n);
//			if(!IR(Weight))	continue;
//Offset = *pp++;

			DUMP_BONE
			Weights.Add(Weight);

			NbBones++;
			j++;
		}
//		if(!NbBones)	commented, else the MAX4 rabbit fails (!)
			BonesNb.Add(NbBones);
	}

	// Fill the structure with skin info
	DumpSkinData(mesh_desc, OffsetVectors, BonesCSID, BonesLocalID, BonesNames, BonesNb, Weights, true);

	// Set topologies
	SetupTopologies(node, mesh_desc, MAXMesh);

	// Build skeletal information
	BuildSkeleton(mesh_desc);

	return true;
}




#ifdef TOSEE

void AsciiExp::ExportSkinData(INode* node, Modifier* mod, int indentLevel)
{
	
	int i = 0;
	TSTR indent = GetIndent(indentLevel);

	ISkin *skin = (ISkin *) mod->GetInterface(I_SKIN);
	if (!skin) return;
	
	ISkinContextData *skinMC = skin->GetContextInterface(node);
	if(!skinMC) return;

	// Begin Skin section
	fprintf(pStream,"%s%s {\n", indent.data(), ID_SKIN_DATA);
	
	// Node name
	fprintf(pStream,"%s\t%s \"%s\"\n", indent.data(), ID_NODE_NAME, FixupName(node->GetName()));
	
	//print out the list of bones
	fprintf(pStream,"%s\t%s\t{\n", indent.data(), ID_SKIN_BONE_LIST);
	for (i=0;i<skin->GetNumBones();i++)
	{
		fprintf(pStream,"%s\t\t\"%s\"\n", indent.data(), FixupName(skin->GetBone(i)->GetName()));
	}
	fprintf(pStream,"%s\t}\n", indent.data());

	// vertex assignment informtion
	fprintf(pStream,"%s\t%s\t{\n", indent.data(), ID_SKIN_VERTICES);
	for(i = 0 ; i < skinMC->GetNumPoints() ; i++)
	{
		//print the vertex number
		fprintf(pStream,"%s\t\t%s\t%s\t{\n", indent.data(), ID_SKIN_VERTEX_NUM, Format(i));
		for(int j = 0 ; j < skinMC->GetNumAssignedBones(i) ; j++)
		{
			//get the bone and its name
			TSTR nodename;
			INode* bone = skin->GetBone(skinMC->GetAssignedBone(i,j));
			
			//If the bone is a biped bone, scale needs to be
			//restored before exporting skin data
			ScaleBiped(bone, 0);

			nodename = bone->GetName();
			//get the offset vector
			Point3 offset = Point3(0.0f,0.0f,0.0f); 
			//get the weight
			float weight = skinMC->GetBoneWeight(i,j);

			//write the data to the file stream
			fprintf(pStream, "%s\t\t\t%s\t\"%s\"\n", indent.data(), ID_SKIN_BONE_NAME, nodename.data());
			fprintf(pStream, "%s\t\t\t%s\t%s\n", indent.data(), ID_SKIN_WEIGHT, Format(weight));
			
			//there is on offset from Skin
			//fprintf(pStream, "%s\t\t\t%s\t%s\n", indent.data(), ID_SKIN_OFFSET, Format(offset));
		}
		//close the vertex section
		fprintf(pStream,"%s\t\t}\n", indent.data());
	}
	//close the other sections
	fprintf(pStream,"%s\t}\n", indent.data());
	fprintf(pStream,"%s}\n", indent.data());
}

#endif