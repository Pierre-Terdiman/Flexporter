///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Instances ?

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

#ifdef COMPILE_VERTEX_LIGHTER

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXMeshHelper::MAXMeshHelper()
{
	mVertexCountTable	= null;
	mFaceCountTable		= null;
	mParityTable		= null;

	mNbVerts			= 0;
	mNbFaces			= 0;
	mVerts				= null;
	mFaces				= null;

	mModel				= null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXMeshHelper::~MAXMeshHelper()
{
	DELETEARRAY(mFaces);
	DELETEARRAY(mVerts);
	DELETESINGLE(mModel);
	DELETEARRAY(mParityTable);
	DELETEARRAY(mFaceCountTable);
	DELETEARRAY(mVertexCountTable);
}

bool MAXMeshHelper::AddMesh(Object* node_object, INode* node)
{
	// Extra checkings
	if(!node || !node_object || !node_object->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))	return false;

	// Keep track of objects
	mMergedNodeTable.Add(udword(node));
	mMergedObjectTable.Add(udword(node_object));
	return true;
}

bool MAXMeshHelper::Merge(TimeValue current_time)
{
	udword node_count = mMergedNodeTable.GetNbEntries();
	if(!node_count)	return false;

	Container merged_triobject_table;	// ### use an array
	mNbVerts = 0;
	mNbFaces = 0;
	{
		mVertexCountTable = new udword[node_count];
		mFaceCountTable = new udword[node_count];
		mParityTable = new BOOL[node_count];

		for(udword i=0;i<node_count;i++)
		{
			// Catch current nodes
			INode* current_node = (INode*)mMergedNodeTable.GetEntry(i);
			Object* current_object = (Object*)mMergedObjectTable.GetEntry(i);

			MAXMeshPointer* NewMesh = new MAXMeshPointer(current_object, current_time);
			if(!NewMesh->GetMesh())
			{
				DELETESINGLE(NewMesh);
				continue;
			}

			// Merge callback
			MergeCallback(NewMesh);

			// Catch relevant figures
			udword current_face_count = NewMesh->GetMesh()->getNumFaces();
			udword current_vertex_count = NewMesh->GetMesh()->getNumVerts();

			// Keep track of some data
			merged_triobject_table.Add(udword(NewMesh));

			mVertexCountTable[i] = current_vertex_count;
			mFaceCountTable[i] = current_face_count;
			mParityTable[i] = (BOOL)GetParity(current_node, current_time);

			// Update counters
			mNbVerts += current_vertex_count;
			mNbFaces += current_face_count;
		}
	}

	udword vertex_offset = 0;
	DELETEARRAY(mFaces);
	DELETEARRAY(mVerts);
	mVerts = new Point[mNbVerts];
	mFaces = new IndexedTriangle[mNbFaces];
	{
		Point* vertex_array = mVerts;
		IndexedTriangle* triangle_array = mFaces;

		for(udword i=0;i<node_count;i++)
		{
			// Catch current node
			INode* current_node = (INode*)mMergedNodeTable.GetEntry(i);
			::Mesh* current_mesh = ((MAXMeshPointer*)merged_triobject_table.GetEntry(i))->GetMesh();
			udword current_vertex_count = mVertexCountTable[i];
			udword current_face_count = mFaceCountTable[i];
			udword current_parity = mParityTable[i]!=0;

			// Get vertices in world space
//			Matrix3 world_matrix;
//			GetWorldMatrix(world_matrix, current_node, current_time);
			Interval inter = FOREVER;    // MAX validity interval
			Matrix3 world_matrix = current_node->GetObjectTM(current_time, &inter);

			for(udword j=0;j<current_vertex_count;j++)
			{
				Point3 world_point = world_matrix * current_mesh->verts[j];

				vertex_array->x = world_point.x;
				vertex_array->y = world_point.y;
				vertex_array->z = world_point.z;

				vertex_array++;
			}

			// Get faces
			for(j=0;j<current_face_count;j++)
			{
				triangle_array->mVRef[0] = vertex_offset + current_mesh->faces[j].v[0];
				triangle_array->mVRef[1] = vertex_offset + current_mesh->faces[j].v[1+current_parity];
				triangle_array->mVRef[2] = vertex_offset + current_mesh->faces[j].v[2-current_parity];
			}

			vertex_offset += current_vertex_count;
		}

		{
			// Ok so here we have two local arrays filled with merged faces & vertices
			DELETESINGLE(mModel);

			OPCODECREATE OC;
			OC.NbTris		= mNbFaces;
			OC.NbVerts		= mNbVerts;
			OC.Tris			= (const udword*)mFaces;
			OC.Verts		= mVerts;
			OC.Rules		= SPLIT_COMPLETE|SPLIT_SPLATTERPOINTS|SPLIT_GEOMCENTER;
			OC.NoLeaf		= true;
			OC.Quantized	= true;
			mModel = new OPCODE_Model;
			bool Status = mModel->Build(OC);
		}
	}

	// Final cleanup
	{
		for(udword i=0;i<node_count;i++)
		{
			MAXMeshPointer* CurrentMesh = (MAXMeshPointer*)merged_triobject_table.GetEntry(i);
			DELETESINGLE(CurrentMesh);
		}
	}

	// Invalidate normals
//	ReleaseNormals();

	return true;
}

udword MAXMeshHelper::Stab(const Ray& world_ray, float max_dist)
{
	if(mModel)
	{
		RayCollider RC;
#ifdef OPC_USE_CALLBACKS
		RC.SetCallback(RMMesh::OPCODE_Callback, udword(Master));
#else
		RC.SetPointers(mFaces, mVerts);
#endif
		RC.SetFirstContact(true);	// we only want a boolean answer
		RC.SetTemporalCoherence(true);
//		RC.SetFirstContact(true);
//		RC.SetTemporalCoherence(true);

		CollisionFaces CFS;
		RC.SetDestination(&CFS);

		RC.SetClosestHit(true);

		RC.SetMaxDist(max_dist);
//			RC.SetMinDist(stabinfo.mMinDist);
		RC.SetCulling(true);

		RC.Collide(world_ray, mModel);
		if(RC.GetContactStatus())
		{
			const CollisionFace* CF = &CFS.GetFaces()[0];
/*			StabbedFace SF;
			SF.mDistance	= CF->mDistance;
			SF.mFaceID		= CF->mFaceID;
			SF.mU			= CF->mU;
			SF.mV			= CF->mV;
			SF.mObjectID	= GetKernelID();
			stabbed.AddStabbedFace(SF);*/
			return CF->mFaceID;
		}
	}
	return INVALID_ID;
}






VertexLighterMesh::VertexLighterMesh()
{
}

VertexLighterMesh::~VertexLighterMesh()
{
}

bool VertexLighterMesh::BuildMesh(Object* node_object, INode* node)
{
	// Call the base methods
	if(!VertexLighterBase::Build(node))	return false;

	if(!MAXMeshHelper::AddMesh(node_object, node))	return false;

	return true;
}

void VertexLighterMesh::MergeCallback(MAXMeshPointer* merged_mesh)
{
	// Enable per-vertex user channels right here, as it seems to make MAX crash if done in the modifiers
/*	merged_mesh->setVDataSupport( LOCAL_VDATA_SPECULAR_X_CHANNEL );
	merged_mesh->setVDataSupport( LOCAL_VDATA_SPECULAR_Y_CHANNEL );
	merged_mesh->setVDataSupport( LOCAL_VDATA_SPECULAR_Z_CHANNEL );
	merged_mesh->setVDataSupport( LOCAL_VDATA_LIGHT_DIRECTION_X_CHANNEL );
	merged_mesh->setVDataSupport( LOCAL_VDATA_LIGHT_DIRECTION_Y_CHANNEL );
	merged_mesh->setVDataSupport( LOCAL_VDATA_LIGHT_DIRECTION_Z_CHANNEL );*/
}

inline_ BOOL* VertexLighterMesh::PrepareValidLightArray()
{
	BOOL* valid_light_array = mCurrentLighter->GetValidLightArray();

	// Validate all lights
	memset(valid_light_array, 0xFF, mCurrentLighter->GetLightCount());

	// So we're a mesh, we can be excluded by some lights.
	// Goal is to find which ones are lighting us.
	// First, if we're a merged mesh, don't bother even trying.
	if(mMergedNodeTable.GetNbEntries()==1 && mCurrentLighter->UsesExclusionLists())
	{
		// Catch the unique node
		INode* MAX_node = (INode*)mMergedNodeTable.GetEntry(0);

		// Parse the lights. This is what really needs optimizing.
		for(udword i=0;i<mCurrentLighter->GetLightCount();i++)
		{
			if(mCurrentLighter->GetLight(i)->Exclude(MAX_node))
			{
				valid_light_array[i] = FALSE;
			}
		}
	}
	return valid_light_array;
}

// :TODO: two paths + specular support + handling cancel made it quite unpretty in the end

BOOL VertexLighterMesh::Light()
{
#ifdef TOFIX
#endif
	return TRUE;
}

BOOL VertexLighterMesh::Light2()
{
#ifdef TOFIX
#endif
	return TRUE;
}

BOOL VertexLighterMesh::ApplyModifiers()
{
#ifdef TOFIX
#endif
	return TRUE;
}

#endif
