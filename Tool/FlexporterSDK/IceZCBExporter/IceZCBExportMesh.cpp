///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	A method to dump a list of points in a CustomArray.
 *	\param		points			[in] the list of points
 *	\param		nb_points		[in] the number of points in the list
 *	\param		array			[out] the destination array
 *	\param		nb_bits			[in] the number of bits for quantization (0 to export plain floats)
 *	\param		keep_last		[in] true to keep the last component, false to discard it (used to discard W for example)
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBFormat::ExportFloats(const NakedPoint* points, udword nb_points, CustomArray& array, udword nb_bits, bool keep_last)
{
	if(!nb_bits)
	{
		// Here we export plain floating-point values, in the same way as Flexporter did before version 1.05
		for(udword i=0;i<nb_points;i++)
		{
			array.Store(points[i].x).Store(points[i].y);
			if(keep_last)	array.Store(points[i].z);
		}
	}
	else
	{
		// Here we quantize the vertex cloud before exporting it. The purpose is to reduce the size of the file without using advanced
		// mesh compression (which would be painful to read back for most users)

		// 1) Compute the quantization coeffs
		float MaxX = MIN_FLOAT, MaxY = MIN_FLOAT, MaxZ = MIN_FLOAT;
		for(udword i=0;i<nb_points;i++)
		{
			if(fabsf(points[i].x)>MaxX)	MaxX = fabsf(points[i].x);
			if(fabsf(points[i].y)>MaxY)	MaxY = fabsf(points[i].y);
			if(fabsf(points[i].z)>MaxZ)	MaxZ = fabsf(points[i].z);
		}
		float QuantCoeffX = float((1<<nb_bits)-1)/MaxX;
		float QuantCoeffY = float((1<<nb_bits)-1)/MaxY;
		float QuantCoeffZ = float((1<<nb_bits)-1)/MaxZ;

		float DequantCoeffX = MaxX / float((1<<nb_bits)-1);
		float DequantCoeffY = MaxY / float((1<<nb_bits)-1);
		float DequantCoeffZ = MaxZ / float((1<<nb_bits)-1);

		// 2) Export quantization coeffs
		array.Store(DequantCoeffX).Store(DequantCoeffY);
		if(keep_last)	array.Store(DequantCoeffZ);

		// 3) Quantize vertex cloud and export
		for(udword i=0;i<nb_points;i++)
		{
			sword x = sword(points[i].x * QuantCoeffX);
			sword y = sword(points[i].y * QuantCoeffY);
			array.Store(x).Store(y);
			if(keep_last)
			{
				sword z = sword(points[i].z * QuantCoeffZ);
				array.Store(z);
			}
		}

		// To dequantize, perform the following operations:
		//float x = float(QuantizedX) * DequantCoeffX;
		//float y = float(QuantizedY) * DequantCoeffY;
		//float z = float(QuantizedZ) * DequantCoeffZ;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Mesh export method.
 *	This method is called once for each exported mesh.
 *	\param		mesh		[in] a structure filled with current mesh information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBFormat::ExportMesh(const MeshDescriptor& mesh)
{
	// Export common information
	ExportBasicInfo(&mesh, &mGeomObjects);

	// Export mesh parameters
	mGeomObjects
		.Store(mesh.mIsCollapsed)
		.Store(mesh.mIsSkeleton)
		.Store(mesh.mIsInstance)
		.Store(mesh.mIsTarget)
		.Store(mesh.mIsConvertible)
		.Store(mesh.mIsSkin)
		.Store(mesh.mCastShadows)
		// CHUNK_MESH_VER >= 9
		.Store(mesh.mReceiveShadows)
		.Store(mesh.mMotionBlur);

	// Export character ID for BIPED parts and skins
	if(mesh.mIsSkeleton || mesh.mIsSkin)
	{
		mGeomObjects.Store(mesh.mCharID);
	}

	// Export the bone's CSID for BIPED parts
	if(mesh.mIsSkeleton)
	{
		mGeomObjects.Store(mesh.mCSID);
	}

	if(!mesh.mIsInstance)	// If that's an instance, use mLinkID
	{
		// Export primitive parameters
		if(!mesh.mPrimParams)
		{
			mGeomObjects.Store(udword(FLX_PRIM_UNDEFINED));
		}
		else
		{
			mGeomObjects.Store(mesh.mPrimParams->mType);
			switch(mesh.mPrimParams->mType)
			{
				case FLX_PRIM_BOX:
				{
					BoxParams* BP = (BoxParams*)mesh.mPrimParams;
					mGeomObjects
						.Store(BP->mLength)
						.Store(BP->mWidth)
						.Store(BP->mHeight)
						.Store(BP->mWSegs)
						.Store(BP->mLSegs)
						.Store(BP->mHSegs)
						.Store(BP->mGenUVS);
				}
				break;

				case FLX_PRIM_SPHERE:
				{
					SphereParams* SP = (SphereParams*)mesh.mPrimParams;
					mGeomObjects
						.Store(SP->mRadius)
						.Store(SP->mSegments)
						.Store(SP->mSmooth)
						.Store(SP->mHemisphere)
						.Store(SP->mSquash)
						.Store(SP->mSliceFrom)
						.Store(SP->mSliceTo)
						.Store(SP->mSliceOn)
						.Store(SP->mRecenter)
						.Store(SP->mGenUVS);
				}
				break;

				case FLX_PRIM_GEOSPHERE:
				{
					GeosphereParams* GP = (GeosphereParams*)mesh.mPrimParams;
					mGeomObjects
						.Store(GP->mRadius)
						.Store(GP->mSegments)
						.Store(GP->mGenType)
						.Store(GP->mHemisphere)
						.Store(GP->mSmooth)
						.Store(GP->mRecenter)
						.Store(GP->mGenUVS);
				}
				break;

				case FLX_PRIM_CYLINDER:
				{
					CylinderParams* CP = (CylinderParams*)mesh.mPrimParams;
					mGeomObjects
						.Store(CP->mRadius)
						.Store(CP->mHeight)
						.Store(CP->mHSegs)
						.Store(CP->mCapSegs)
						.Store(CP->mSides)
						.Store(CP->mSmooth)
						.Store(CP->mSliceOn)
						.Store(CP->mSliceFrom)
						.Store(CP->mSliceTo)
						.Store(CP->mGenUVS);
				}
				break;

				case FLX_PRIM_CONE:
				{
					ConeParams* CP = (ConeParams*)mesh.mPrimParams;
					mGeomObjects
						.Store(CP->mRadius1)
						.Store(CP->mRadius2)
						.Store(CP->mHeight)
						.Store(CP->mHSegs)
						.Store(CP->mCapSegs)
						.Store(CP->mSides)
						.Store(CP->mSmooth)
						.Store(CP->mSliceOn)
						.Store(CP->mSliceFrom)
						.Store(CP->mSliceTo)
						.Store(CP->mGenUVS);
				}
				break;

				case FLX_PRIM_TORUS:
				{
					TorusParams* TP = (TorusParams*)mesh.mPrimParams;
					mGeomObjects
						.Store(TP->mRadius1)
						.Store(TP->mRadius2)
						.Store(TP->mRotation)
						.Store(TP->mTwist)
						.Store(TP->mSegments)
						.Store(TP->mSides)
						.Store(TP->mSmooth)
						.Store(TP->mSliceOn)
						.Store(TP->mSliceFrom)
						.Store(TP->mSliceTo)
						.Store(TP->mGenUVS);
				}
				break;

				case FLX_PRIM_TUBE:
				{
					TubeParams* TP = (TubeParams*)mesh.mPrimParams;
					mGeomObjects
						.Store(TP->mRadius1)
						.Store(TP->mRadius2)
						.Store(TP->mHeight)
						.Store(TP->mSegments)
						.Store(TP->mCapSegs)
						.Store(TP->mSides)
						.Store(TP->mSmooth)
						.Store(TP->mSliceOn)
						.Store(TP->mSliceFrom)
						.Store(TP->mSliceTo)
						.Store(TP->mGenUVS);
				}
				break;

				case FLX_PRIM_TEAPOT:
				{
				}
				break;

				case FLX_PRIM_PLANE:
				{
					PlaneParams* PP = (PlaneParams*)mesh.mPrimParams;
					mGeomObjects
						.Store(PP->mLength)
						.Store(PP->mWidth)
						.Store(PP->mWidthSegs)
						.Store(PP->mLengthSegs)
						.Store(PP->mDensity)
						.Store(PP->mScale)
						.Store(PP->mGenUVS);
				}
				break;

				case FLX_PRIM_HEDRA:
				{
					HedraParams* HP = (HedraParams*)mesh.mPrimParams;
					mGeomObjects
						.Store(HP->mRadius)
						.Store(HP->mFamily)
						.Store(HP->mP)
						.Store(HP->mQ)
						.Store(HP->mScaleP)
						.Store(HP->mScaleQ)
						.Store(HP->mScaleR)
						.Store(HP->mVertices)
						.Store(HP->mGenUVS);
				}
				break;

				case FLX_PRIM_CAPSULE:
				{
					CapsuleParams* CP = (CapsuleParams*)mesh.mPrimParams;
					mGeomObjects
						.Store(CP->mRadius)
						.Store(CP->mHeight)
						.Store(CP->mCenters)
						.Store(CP->mSides)
						.Store(CP->mHSegs)
						.Store(CP->mSmooth)
						.Store(CP->mSliceOn)
						.Store(CP->mSliceFrom)
						.Store(CP->mSliceTo)
						.Store(CP->mGenUVS);
				}
				break;
			}
		}

		// Export MAX native mesh
		MAXNativeMesh* Mesh = mesh.mOriginalMesh;

		// Adjust flags
		if(mCompression!=ZCB_COMPRESSION_NONE)
		{
			// If compression is on, two things happen:
			// 1) Vertices are quantized. A "vertex" here is a geometric point, a UVW triplet or a vertex-color.
			// 2) Faces are delta-encoded to please further compressors (e.g. ZLib)
			// NB: the MAX native mesh only is modified by the compression settings, not the consolidated mesh!
			// ..simply because if you really want to save space, don't export the consolidation results, do it at runtime...
			Mesh->mFlags|=MESH_QUANTIZEDVERTICES;
			Mesh->mFlags|=MESH_COMPRESSED;
		}

		// Check vertex alpha flags
		if(!Mesh->mVertexAlpha)	Mesh->mFlags&=~MESH_VERTEX_ALPHA;

		// Now, faces can still be saved as words or dwords, regardless of the compression setting.
		if(Mesh->mNbVerts < 65536 && Mesh->mNbTVerts < 65536 && Mesh->mNbCVerts < 65536)	Mesh->mFlags|=MESH_WORDFACES;

		// Save useful figures
		mGeomObjects.Store(Mesh->mNbFaces).Store(Mesh->mNbVerts).Store(Mesh->mNbTVerts).Store(Mesh->mNbCVerts).Store(Mesh->mFlags).Store(Mesh->mParity);

		// Save vertices (null for skins)
		if(Mesh->mVerts)
		{
			// Save plain floats or quantized vertices
			if(mCompression!=ZCB_COMPRESSION_NONE)	ExportFloats(Mesh->mVerts, Mesh->mNbVerts, mGeomObjects, mNbBits, true);
			else									ExportFloats(Mesh->mVerts, Mesh->mNbVerts, mGeomObjects, 0, true);
		}

		// Save skin-related data
		if(mesh.mIsSkin)
		{
			// Export links to the driving skeleton
			if(!Mesh->mBonesNb)
			{
				// A simple skin with one bone/vertex.

				// Save offset vectors
				const NakedPoint* v = Mesh->mOffsetVectors;
				for(udword i=0;i<Mesh->mNbVerts;i++)	mGeomObjects.Store(v[i].x).Store(v[i].y).Store(v[i].z);

				// Save bones CSIDs
				for(udword i=0;i<Mesh->mNbVerts;i++)	mGeomObjects.Store(Mesh->mBonesCSID[i]);
				// NB: local IDs and bones names not supported in ZCB files!
			}
			else
			{
				// A skin with multiple bones/vertex.

				// Save bones Nb
				udword Sum = 0;
				for(udword i=0;i<Mesh->mNbVerts;i++)
				{
					mGeomObjects.Store(Mesh->mBonesNb[i]);
					Sum+=Mesh->mBonesNb[i];
				}

				// Save bones CSIDs
				for(udword i=0;i<Sum;i++)				mGeomObjects.Store(Mesh->mBonesCSID[i]);
				// NB: local IDs and bones names not supported in ZCB files!

				// Save weights
				for(udword i=0;i<Sum;i++)				mGeomObjects.Store(Mesh->mWeights[i]);

				// Save offset vectors
				const NakedPoint* v = Mesh->mOffsetVectors;
				for(udword i=0;i<Sum;i++)				mGeomObjects.Store(v[i].x).Store(v[i].y).Store(v[i].z);
			}

			// Export compatible skeletal information
			const MAXSkeleton* Skel = Mesh->mSkeleton;
			if(Skel)
			{
				mGeomObjects.Store(Skel->mNbBones);
				udword* ID = Skel->mID;
				for(udword i=0;i<Skel->mNbBones;i++)
				{
					udword CSID = *ID++;
					udword pCSID = *ID++;
					mGeomObjects.Store(CSID).Store(pCSID);
				}
			}
			else mGeomObjects.Store(udword(0));
		}

		// Save mapping coordinates
		if(Mesh->mTVerts)
		{
			// Save plain floats or quantized vertices
			if(mCompression!=ZCB_COMPRESSION_NONE)	ExportFloats(Mesh->mTVerts, Mesh->mNbTVerts, mGeomObjects, mNbBits, !(Mesh->mFlags&MESH_WDISCARDED));
			else									ExportFloats(Mesh->mTVerts, Mesh->mNbTVerts, mGeomObjects, 0, !(Mesh->mFlags&MESH_WDISCARDED));
		}

		// Save vertex-colors
		if(Mesh->mCVerts)
		{
			// Save plain floats or quantized vertices
			if(mCompression!=ZCB_COMPRESSION_NONE)	ExportFloats(Mesh->mCVerts, Mesh->mNbCVerts, mGeomObjects, mNbBits, true);
			else									ExportFloats(Mesh->mCVerts, Mesh->mNbCVerts, mGeomObjects, 0, true);
		}

		// Export vertex-alpha - since it's just one extra flag, no need to increase version number
		if(Mesh->mFlags&MESH_VERTEX_ALPHA)
		{
			float* v = Mesh->mVertexAlpha;			// We already checked the array was allocated (above)
			for(udword i=0;i<Mesh->mNbVerts;i++)	// Beware: mNbVerts !
			{
				mGeomObjects.Store(v[i]);
			}
		}

		// Save faces
		// The way faces are stored within the ZCB file has changed! Use the version number to detect old ZCB files.
		// CHUNK_MESH_VER = 2   => old format
		// CHUNK_MESH_VER > 2	=> new format, as below
		MAXFace* f = Mesh->mFaces;

/*
		Just for the records, here's the old way:

		for(udword i=0;i<Mesh->mNbFaces;i++)
		{
			if(Mesh->mFlags&MESH_WORDFACES)
			{
				if(Mesh->mFlags&MESH_VFACE)										mGeomObjects.Store(uword(f[i].VRef[0]))	.Store(uword(f[i].VRef[1]))	.Store(uword(f[i].VRef[2]));
				if((Mesh->mFlags&MESH_TFACE)&&(Mesh->mFlags&MESH_UVW))			mGeomObjects.Store(uword(f[i].TRef[0]))	.Store(uword(f[i].TRef[1]))	.Store(uword(f[i].TRef[2]));
				if((Mesh->mFlags&MESH_CFACE)&&(Mesh->mFlags&MESH_VERTEXCOLORS))	mGeomObjects.Store(uword(f[i].CRef[0]))	.Store(uword(f[i].CRef[1]))	.Store(uword(f[i].CRef[2]));
			}
			else
			{
				if(Mesh->mFlags&MESH_VFACE)										mGeomObjects.Store(udword(f[i].VRef[0])).Store(udword(f[i].VRef[1])).Store(udword(f[i].VRef[2]));
				if((Mesh->mFlags&MESH_TFACE)&&(Mesh->mFlags&MESH_UVW))			mGeomObjects.Store(udword(f[i].TRef[0])).Store(udword(f[i].TRef[1])).Store(udword(f[i].TRef[2]));
				if((Mesh->mFlags&MESH_CFACE)&&(Mesh->mFlags&MESH_VERTEXCOLORS))	mGeomObjects.Store(udword(f[i].CRef[0])).Store(udword(f[i].CRef[1])).Store(udword(f[i].CRef[2]));
			}
			mGeomObjects.Store(f[i].MatID).Store(f[i].Smg).Store(f[i].EdgeVis);
		}
*/
		udword i;
		if(Mesh->mFlags&MESH_WORDFACES)
		{
			sword* Buffer = new sword[Mesh->mNbFaces*3];	CHECKALLOC(Buffer);

			// Save faces
			if(Mesh->mFlags&MESH_VFACE)
			{
				for(i=0;i<Mesh->mNbFaces;i++)
				{
					Buffer[i*3+0] = uword(f[i].VRef[0]);
					Buffer[i*3+1] = uword(f[i].VRef[1]);
					Buffer[i*3+2] = uword(f[i].VRef[2]);
				}
				if(Mesh->mFlags&MESH_COMPRESSED)	Delta(Buffer, Mesh->mNbFaces*3, 2);
				for(i=0;i<Mesh->mNbFaces*3;i++)	mGeomObjects.Store(Buffer[i]);
			}

			// Save texture faces
			if((Mesh->mFlags&MESH_TFACE)&&(Mesh->mFlags&MESH_UVW))
			{
				for(i=0;i<Mesh->mNbFaces;i++)
				{
					Buffer[i*3+0] = uword(f[i].TRef[0]);
					Buffer[i*3+1] = uword(f[i].TRef[1]);
					Buffer[i*3+2] = uword(f[i].TRef[2]);
				}
				if(Mesh->mFlags&MESH_COMPRESSED)	Delta(Buffer, Mesh->mNbFaces*3, 2);
				for(i=0;i<Mesh->mNbFaces*3;i++)	mGeomObjects.Store(Buffer[i]);
			}

			// Save color faces
			if((Mesh->mFlags&MESH_CFACE)&&(Mesh->mFlags&MESH_VERTEXCOLORS))
			{
				for(i=0;i<Mesh->mNbFaces;i++)
				{
					Buffer[i*3+0] = uword(f[i].CRef[0]);
					Buffer[i*3+1] = uword(f[i].CRef[1]);
					Buffer[i*3+2] = uword(f[i].CRef[2]);
				}
				if(Mesh->mFlags&MESH_COMPRESSED)	Delta(Buffer, Mesh->mNbFaces*3, 2);
				for(i=0;i<Mesh->mNbFaces*3;i++)	mGeomObjects.Store(Buffer[i]);
			}

			DELETEARRAY(Buffer);
		}
		else
		{
			sdword* Buffer = new sdword[Mesh->mNbFaces*3];	CHECKALLOC(Buffer);

			// Save faces
			if(Mesh->mFlags&MESH_VFACE)
			{
				for(i=0;i<Mesh->mNbFaces;i++)
				{
					Buffer[i*3+0] = udword(f[i].VRef[0]);
					Buffer[i*3+1] = udword(f[i].VRef[1]);
					Buffer[i*3+2] = udword(f[i].VRef[2]);
				}
				if(Mesh->mFlags&MESH_COMPRESSED)	Delta(Buffer, Mesh->mNbFaces*3, 4);
				for(i=0;i<Mesh->mNbFaces*3;i++)	mGeomObjects.Store(Buffer[i]);
			}

			// Save texture faces
			if((Mesh->mFlags&MESH_TFACE)&&(Mesh->mFlags&MESH_UVW))
			{
				for(i=0;i<Mesh->mNbFaces;i++)
				{
					Buffer[i*3+0] = udword(f[i].TRef[0]);
					Buffer[i*3+1] = udword(f[i].TRef[1]);
					Buffer[i*3+2] = udword(f[i].TRef[2]);
				}
				if(Mesh->mFlags&MESH_COMPRESSED)	Delta(Buffer, Mesh->mNbFaces*3, 4);
				for(i=0;i<Mesh->mNbFaces*3;i++)	mGeomObjects.Store(Buffer[i]);
			}

			// Save color faces
			if((Mesh->mFlags&MESH_CFACE)&&(Mesh->mFlags&MESH_VERTEXCOLORS))
			{
				for(i=0;i<Mesh->mNbFaces;i++)
				{
					Buffer[i*3+0] = udword(f[i].CRef[0]);
					Buffer[i*3+1] = udword(f[i].CRef[1]);
					Buffer[i*3+2] = udword(f[i].CRef[2]);
				}
				if(Mesh->mFlags&MESH_COMPRESSED)	Delta(Buffer, Mesh->mNbFaces*3, 4);
				for(i=0;i<Mesh->mNbFaces*3;i++)	mGeomObjects.Store(Buffer[i]);
			}

			DELETEARRAY(Buffer);
		}

		// Save face parameters
		for(i=0;i<Mesh->mNbFaces;i++)	mGeomObjects.Store(f[i].MatID);
		for(i=0;i<Mesh->mNbFaces;i++)	mGeomObjects.Store(f[i].Smg);

		if(mSettings.mEdgeVis)
		{
			for(i=0;i<Mesh->mNbFaces;i++)	mGeomObjects.Store(f[i].EdgeVis);
		}

		// Export the possible convex hull
		if(Mesh->mFlags&MESH_CONVEXHULL)
		{
			mGeomObjects.Store(Mesh->mConvexHull->mNbVerts).Store(Mesh->mConvexHull->mNbFaces);
			if(Mesh->mConvexHull->mVerts)
			{
				const NakedPoint* v = Mesh->mConvexHull->mVerts;
				for(udword i=0;i<Mesh->mConvexHull->mNbVerts;i++)	mGeomObjects.Store(v[i].x).Store(v[i].y).Store(v[i].z);
			}

			if(Mesh->mConvexHull->mFaces)
			{
				const udword* f = Mesh->mConvexHull->mFaces;
				for(udword i=0;i<Mesh->mConvexHull->mNbFaces;i++)	mGeomObjects.Store(f[i*3+0]).Store(f[i*3+1]).Store(f[i*3+2]);
			}
		}

		// Export the possible bounding sphere
		if(Mesh->mFlags&MESH_BOUNDINGSPHERE)
		{
			mGeomObjects.Store(Mesh->mBSCenter.x).Store(Mesh->mBSCenter.y).Store(Mesh->mBSCenter.z).Store(Mesh->mBSRadius);
		}

		// Export the inertia tensor
		if(Mesh->mFlags&MESH_INERTIATENSOR)
		{
			const ITensor* T = Mesh->mTensor;
			mGeomObjects
				.Store(T->COM.x).Store(T->COM.y).Store(T->COM.z)
				.Store(T->Mass)
				.Store(T->InertiaTensor[0][0]).Store(T->InertiaTensor[0][1]).Store(T->InertiaTensor[0][2])
				.Store(T->InertiaTensor[1][0]).Store(T->InertiaTensor[1][1]).Store(T->InertiaTensor[1][2])
				.Store(T->InertiaTensor[2][0]).Store(T->InertiaTensor[2][1]).Store(T->InertiaTensor[2][2])
				.Store(T->COMInertiaTensor[0][0]).Store(T->COMInertiaTensor[0][1]).Store(T->COMInertiaTensor[0][2])
				.Store(T->COMInertiaTensor[1][0]).Store(T->COMInertiaTensor[1][1]).Store(T->COMInertiaTensor[1][2])
				.Store(T->COMInertiaTensor[2][0]).Store(T->COMInertiaTensor[2][1]).Store(T->COMInertiaTensor[2][2]);
		}

		// Export consolidated mesh
		if(Mesh->mFlags&MESH_CONSOLIDATION)
		{
			// Topology
			{
				const MBTopology& Topo = mesh.mCleanMesh->Topology;

				// udwords since Flexporter 1.14
				mGeomObjects.Store(Topo.NbFaces).Store(Topo.NbSubmeshes);

				// Submeshes
				for(udword i=0;i<Topo.NbSubmeshes;i++)
				{
					const MBSubmesh* CurSM = &Topo.SubmeshProperties[i];
					mGeomObjects.Store(CurSM->MatID).Store(CurSM->SmGrp).Store(CurSM->NbFaces).Store(CurSM->NbVerts).Store(CurSM->NbSubstrips);
				}

				// Connectivity
				const udword* VRefs = Topo.VRefs;	// udword* since Flexporter 1.14 (CHUNK_MESH_VER == 7)
				for(i=0;i<Topo.NbSubmeshes;i++)
				{
					// udword since Flexporter 1.14
					udword NbFaces = Topo.FacesInSubmesh[i];

					mGeomObjects.Store(NbFaces);

					// Save faces (Ref0, Ref1, Ref2)
					for(udword j=0;j<NbFaces;j++)
					{
						udword Ref0 = *VRefs++;
						udword Ref1 = *VRefs++;
						udword Ref2 = *VRefs++;
						mGeomObjects.Store(Ref0).Store(Ref1).Store(Ref2);
					}
				}

				// Face normals
				if(Mesh->mFlags&MESH_FACENORMALS)
				{
					const float* Normals = Topo.Normals;
					for(i=0;i<Topo.NbFaces;i++)
					{
						float nx = Normals ? Normals[i*3+0] : 0.0f;
						float ny = Normals ? Normals[i*3+1] : 0.0f;
						float nz = Normals ? Normals[i*3+2] : 0.0f;
						mGeomObjects.Store(nx).Store(ny).Store(nz);
					}
				}
			}

			// Geometry
			{
				const MBGeometry& Geo = mesh.mCleanMesh->Geometry;

				// udwords since Flexporter 1.14
				mGeomObjects.Store(Geo.NbGeomPts).Store(Geo.NbVerts).Store(Geo.NbTVerts);

				// Indexed geometry
				udword MaxRef=0;
				if(Geo.VertsRefs)
				{
					for(udword i=0;i<Geo.NbVerts;i++)
					{
						udword Ref = Geo.VertsRefs[i];
						if(Ref>MaxRef)	MaxRef=Ref;
						mGeomObjects.Store(Ref);
					}
				}

				// Vertices
				udword NbVerts = Geo.VertsRefs ? (MaxRef+1) : Geo.NbVerts;
				mGeomObjects.Store(NbVerts);
				for(udword i=0;i<NbVerts;i++)
				{
					float x = Geo.Verts ? Geo.Verts[i*3+0] : 0.0f;
					float y = Geo.Verts ? Geo.Verts[i*3+1] : 0.0f;
					float z = Geo.Verts ? Geo.Verts[i*3+2] : 0.0f;
					mGeomObjects.Store(x).Store(y).Store(z);
				}

				// Indexed UVWs
				MaxRef = 0;
				if((Mesh->mFlags&MESH_UVW) && Geo.TVertsRefs)
				{
					for(udword i=0;i<Geo.NbVerts;i++)
					{
						udword Ref = Geo.TVertsRefs[i];
						if(Ref>MaxRef)	MaxRef=Ref;
						mGeomObjects.Store(Ref);
					}
				}

				// Texture-vertices
				if((Mesh->mFlags&MESH_UVW) && Geo.TVerts)
				{
					udword NbTVerts = Geo.TVertsRefs ? (MaxRef+1) : Geo.NbVerts;
					mGeomObjects.Store(NbTVerts);
					float* p = Geo.TVerts;
					for(udword i=0;i<NbTVerts;i++)
					{
						float u = *p++;	mGeomObjects.Store(u);
						float v = *p++;	mGeomObjects.Store(v);
						if(!(Mesh->mFlags&MESH_WDISCARDED))	
						{
							float w = *p++;	mGeomObjects.Store(w);
						}
					}
				}

				// Normals
				if(Mesh->mFlags&MESH_VERTEXNORMALS)
				{
					udword NbNormals = Geo.NbVerts;
					mGeomObjects.Store(NbNormals);
					for(udword i=0;i<NbNormals;i++)
					{
						float x = Geo.Normals[i*3+0];
						float y = Geo.Normals[i*3+1];
						float z = Geo.Normals[i*3+2];
						mGeomObjects.Store(x).Store(y).Store(z);
					}
				}

				// Vertex colors
				if((Mesh->mFlags&MESH_VERTEXCOLORS) && Geo.CVerts)
				{
					udword NbVtxColors = Geo.NbVerts;	// Because non-indexed
					mGeomObjects.Store(NbVtxColors);
					if(Mesh->mFlags&MESH_DWORD_COLORS)
					{
						// Since the internal code only handled "points", vertex colors/alpha have been packed to dwords
						for(udword i=0;i<NbVtxColors;i++)
						{
							float Data = Geo.CVerts[i*3+0];	// We only need the red channel
							udword Color = IR(Data);
							mGeomObjects.Store(Color);
						}
					}
					else
					{
						for(udword i=0;i<NbVtxColors;i++)
						{
							float r = Geo.CVerts[i*3+0];
							float g = Geo.CVerts[i*3+1];
							float b = Geo.CVerts[i*3+2];
							mGeomObjects.Store(r).Store(g).Store(b);
						}
					}
				}

				// Normal info
				if((Mesh->mFlags&MESH_NORMALINFO) && Geo.NormalInfo)
				{
					udword NormalInfoSize = Geo.NormalInfoSize;
					mGeomObjects.Store(NormalInfoSize);
					for(udword i=0;i<NormalInfoSize;i++)
					{
						udword d = Geo.NormalInfo[i];
						mGeomObjects.Store(d);
					}
				}
			}

			// Materials
			{
				const MBMaterials& Mtl = mesh.mCleanMesh->Materials;

				mGeomObjects.Store(Mtl.NbMtls);

				for(udword i=0;i<Mtl.NbMtls;i++)
				{
					MBMatInfo* CurMtl = &Mtl.MaterialInfo[i];

					mGeomObjects.Store(CurMtl->MatID).Store(CurMtl->NbFaces).Store(CurMtl->NbVerts).Store(CurMtl->NbSubmeshes);
				}
			}
		}
	}

#ifdef SUPPORT_OLD_LIGHTMAPPER
	// Lightmapper's vertex colors. Since the lighting depends on the world space position, instances have their own lighting data.
	// That's why those colors are not directly included in MAX's vertex colors channel !
	mGeomObjects.Store(mesh.mNbColors);	// Can be 0 => use that as a flag
	if(mesh.mColors)
	{
		for(udword i=0;i<mesh.mNbColors;i++)	mGeomObjects.Store(mesh.mColors[i].x).Store(mesh.mColors[i].y).Store(mesh.mColors[i].z);
	}
#else
	mGeomObjects.Store(udword(0));	// Can be 0 => use that as a flag
#endif
	return true;
}
