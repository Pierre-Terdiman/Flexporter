///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a BPatch.
 *	\file		FLX_AddBPatch.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

/*
	This file has never been converted from my old exporter.
*/

#ifdef OLDIES

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A method to export a BPatch
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input	:	-
// Output	:	-
// Return	:	-
// Remark	:	-
MAX_Exporter& MAX_Exporter::AddBPatch(INode* Node)
{
	// Get time info back
	long		StartTime		= mTimeInfo.StartTime;
	long		EndTime			= mTimeInfo.EndTime;
	long		DeltaTime		= mTimeInfo.DeltaTime;
	long		FrameRate		= mTimeInfo.FrameRate;
	TimeValue	CurrentTime		= mTimeInfo.CurrentTime;

	// Evaluate object at current frame
	Object* NodeObject = Node->EvalWorldState(CurrentTime).obj;

	// Check if node is a PatchMesh
	if (!NodeObject->CanConvertToType(patchObjectClassID))
		return *this;

	// Cast it to a PatchObject and grab pointer to the PatchMesh
	PatchMesh* patchmesh =  &((PatchObject*)NodeObject->ConvertToType(CurrentTime, patchObjectClassID))->patch;

	// Get global PatchMesh parameters
	udword NbVerts		= patchmesh->numVerts;
	udword NbVectors	= patchmesh->numVecs;
	udword NbPatches	= patchmesh->numPatches;

	// Get PatchMesh's vertices
	for(udword i=0;i<NbVerts;i++)
	{
		float x = patchmesh->verts[i].p.x;
		float y = patchmesh->verts[i].p.y;
		float z = patchmesh->verts[i].p.z;
	}

	// Get PatchMesh's vectors
	for(i=0;i<NbVectors;i++)
	{
		float x = patchmesh->vecs[i].p.x;
		float y = patchmesh->vecs[i].p.y;
		float z = patchmesh->vecs[i].p.z;
	}

	// Get PatchMesh's patches
	for(udword pm=0;pm<NbPatches;pm++)
	{
		Patch* patch = &patchmesh->patches[pm];

		udword PatchType = patch->type;
//		udword i;
		switch(PatchType)
		{
			case PATCH_UNDEF:
				// Undefined patch
				break;

/*			case PATCH_TRI:
				// Store patch's vertices 
				for (i=0; i<3; i++)
					Array->Store((Dword)patch->v[i]);
				// Store patch's vectors
				for (i=0; i<6; i++)
					Array->Store((Dword)patch->vec[i]);
				// Store patch's interior points
				for (i=0; i<3; i++)
					Array->Store((Dword)patch->interior[i]);
				break;

		
			case PATCH_QUAD:
				// Store patch's vertices 
				for (i=0; i<4; i++)
					Array->Store((Dword)patch->v[i]);
				// Store patch's vectors
				for (i=0; i<8; i++)
					Array->Store((Dword)patch->vec[i]);
				// Store patch's interior points
				for (i=0; i<4; i++)
					Array->Store((Dword)patch->interior[i]);
				break;*/
		}
	}

	// Update members
	mStats.NbBPatchObjects++;

	return *this;
}

#endif