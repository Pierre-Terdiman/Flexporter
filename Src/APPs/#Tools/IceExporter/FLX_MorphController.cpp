///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a morph controller.
 *	\file		FLX_MorphController.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Creates a morph controller.
 *	\param		node			[in] the MAX node
 *	\param		exported_node	[in] the exported node's info structure
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddMorphController(INode* node, ResourceDescriptor* exported_node)
{
	// Checkings
	if(!node || !exported_node)	return false;

	// Check settings
	if(mSettings->mSingleFrame)	return true;

	// Export morph controller if needed
	if(mProps.mMorph)
	{
		// Dump
		CustomArray VertsData;
		udword OldCount;
		udword NbFrames = 0;

		// Catch sampling rate
		long SamplingRate = mSettings->GetSamplingRate();

		INIT_PERCENTS
		for(long i=mTimeInfo.mStartTime;i<=mTimeInfo.mEndTime;i+=mTimeInfo.mDeltaTime * SamplingRate)
		{
			UPDATE_PERCENTS(i, mTimeInfo.mEndTime, "exporting morph data... ")

			// Convert to tri-mesh for current frame
			ObjectState os = node->EvalWorldState(i);			// Collapse the object...
			Object* NodeObject = os.obj;						// ...and get it back

			{
				MAXMeshPointer TriMesh(NodeObject, i);

				// Get the #vertices, supposed to be constant
				udword NbVerts = TriMesh->getNumVerts();
				if(i)
				{
					if(NbVerts!=OldCount)
					{
						mArrayLog.StoreASCII("\nError in morph controller: found dynamic number of vertices.\n");
						return false;
					}
				}

//### Fixed 1.16
//				Point Scaling = GetScaling();
				Point Scaling;
				IceExporter::GetScaling(Scaling, node, i);

				// User-defined frame conversion
//				if(mExporter)	mExporter->AdjustScale(Scaling.x, Scaling.y, Scaling.z);
				Scaling *= mSettings->mGlobalScale;

				for(udword j=0;j<NbVerts;j++)
				{
					float x = TriMesh->verts[j].x * Scaling.x;
					float y = TriMesh->verts[j].y * Scaling.y;
					float z = TriMesh->verts[j].z * Scaling.z;

					// Possible extra XForm
					if(HasXForm())
					{
						Point p = Point(x, y, z) * GetXForm();
						x = p.x;
						y = p.y;
						z = p.z;
					}

					// MAX-to-D3D
					if(mSettings->mConvertToD3D)	Swap(IR(y), IR(z));

					// User-defined frame conversion
					if(mExporter)	mExporter->ConvertPoint(x, y, z);

					VertsData.Store(x).Store(y).Store(z);
				}
				OldCount = NbVerts;
				NbFrames++;
			}
		}

		// Create a new ControllerInfo structure. Not shared => create it, export it, destroy it
		ControllerDescriptor Ctrl;
		Ctrl.mField			= String("VERTICES");
		Ctrl.mObjectID		= mStats.mNbControllers++;
		Ctrl.mOwnerID		= exported_node->mObjectID;
		Ctrl.mOwnerType		= exported_node->mType;
		// Create ControllerData
		MorphDataCreate mdc;
		mdc.Mode			= CTRL_SAMPLES;
		mdc.Type			= FLX_CTRL_VERTEXCLOUD;
		mdc.NbSamples		= NbFrames;
		mdc.SamplingRate	= SamplingRate;
		mdc.NbVertices		= OldCount;

		Ctrl.mData	= new MorphData;	CHECKALLOC(Ctrl.mData);
		Ctrl.mData->Init(&mdc);

		CopyMemory(((MorphData*)Ctrl.mData)->mSamples, VertsData.Collapse(), VertsData.GetSize());
		_DUMP_CONTROLLER("sampled morph")
	}

	return true;
}
