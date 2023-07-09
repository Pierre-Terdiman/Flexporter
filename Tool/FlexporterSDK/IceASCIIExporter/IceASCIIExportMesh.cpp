///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Mesh export method.
 *	This method is called once for each exported mesh.
 *	\param		mesh		[in] a structure filled with current mesh information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ASCIIFormat::ExportMesh(const MeshDescriptor& mesh)
{
	// Export common information
	mGeomObjects.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("GeomObject:   ");
	ExportBasicInfo(&mesh, &mGeomObjects);

	// Export mesh parameters
	if(mesh.mIsCollapsed)		mGeomObjects.StoreASCII("(Collapsed object)\n");
	if(mesh.mIsSkeleton)		mGeomObjects.StoreASCII("(Skeleton object)\n");
	if(mesh.mIsInstance)		mGeomObjects.StoreASCII("(Instance object)\n");
	if(mesh.mIsTarget)			mGeomObjects.StoreASCII("(Target object)\n");
	if(!mesh.mIsConvertible)	mGeomObjects.StoreASCII("(Non convertible object)\n");
	if(mesh.mIsSkin)			mGeomObjects.StoreASCII("(Skin)\n");
	if(mesh.mCastShadows)		mGeomObjects.StoreASCII("(Can cast shadows)\n");

	// Export character ID for BIPED parts and skins
	if(mesh.mIsSkeleton || mesh.mIsSkin)
	{
		mGeomObjects.StoreASCII("CharID: ").StoreASCII(mesh.mCharID).StoreASCII("\n");
	}

	// Export the bone's CSID for BIPED parts
	if(mesh.mIsSkeleton)
	{
		mGeomObjects.StoreASCII("CSID:   ").StoreASCII(mesh.mCSID).StoreASCII("\n");
	}

	if(mesh.mIsInstance)
	{
		mGeomObjects.StoreASCII("Instance from: ").StoreASCII((const char*)mesh.mMasterMesh->mName).StoreASCII("\n");
	}
	else
	{
		const MAXNativeMesh* Mesh = mesh.mOriginalMesh;
		if(!Mesh)	return true;	// Target meshes don't have data

		// Export primitive parameters
		if(mesh.mPrimParams)
		{
			switch(mesh.mPrimParams->mType)
			{
				case FLX_PRIM_BOX:
				{
					BoxParams* BP = (BoxParams*)mesh.mPrimParams;
					mGeomObjects
						.StoreASCII("\nBox primitive:\n")
						.StoreASCII("  Length: %f\n", BP->mLength)
						.StoreASCII("  Width: %f\n", BP->mWidth)
						.StoreASCII("  Height: %f\n", BP->mHeight)
						.StoreASCII("  WSegs: %d\n", BP->mWSegs)
						.StoreASCII("  LSegs: %d\n", BP->mLSegs)
						.StoreASCII("  HSegs: %d\n", BP->mHSegs)
						.StoreASCII("  GenUVS: %d\n", BP->mGenUVS);
				}
				break;

				case FLX_PRIM_SPHERE:
				{
					SphereParams* SP = (SphereParams*)mesh.mPrimParams;
					mGeomObjects
						.StoreASCII("\nSphere primitive:\n")
						.StoreASCII("  Radius: %f\n", SP->mRadius)
						.StoreASCII("  Segments: %d\n", SP->mSegments)
						.StoreASCII("  Smooth: %d\n", SP->mSmooth)
						.StoreASCII("  Hemisphere: %f\n", SP->mHemisphere)
						.StoreASCII("  Squash: %d\n", SP->mSquash)
						.StoreASCII("  SliceFrom: %f\n", SP->mSliceFrom)
						.StoreASCII("  SliceTo: %f\n", SP->mSliceTo)
						.StoreASCII("  SliceOn: %d\n", SP->mSliceOn)
						.StoreASCII("  Recenter: %d\n", SP->mRecenter)
						.StoreASCII("  GenUVS: %d\n", SP->mGenUVS);
				}
				break;

				case FLX_PRIM_GEOSPHERE:
				{
					GeosphereParams* GP = (GeosphereParams*)mesh.mPrimParams;
					mGeomObjects
						.StoreASCII("\nGeosphere primitive:\n")
						.StoreASCII("  Radius: %f\n", GP->mRadius)
						.StoreASCII("  Segments: %d\n", GP->mSegments)
						.StoreASCII("  GenType: %d\n", GP->mGenType)
						.StoreASCII("  Hemisphere: %d\n", GP->mHemisphere)
						.StoreASCII("  Smooth: %d\n", GP->mSmooth)
						.StoreASCII("  Recenter: %d\n", GP->mRecenter)
						.StoreASCII("  GenUVS: %d\n", GP->mGenUVS);
				}
				break;

				case FLX_PRIM_CYLINDER:
				{
					CylinderParams* CP = (CylinderParams*)mesh.mPrimParams;
					mGeomObjects
						.StoreASCII("\nCylinder primitive:\n")
						.StoreASCII("  Radius: %f\n", CP->mRadius)
						.StoreASCII("  Height: %f\n", CP->mHeight)
						.StoreASCII("  HSegs: %d\n", CP->mHSegs)
						.StoreASCII("  CapSegs: %d\n", CP->mCapSegs)
						.StoreASCII("  Sides: %d\n", CP->mSides)
						.StoreASCII("  Smooth: %d\n", CP->mSmooth)
						.StoreASCII("  SliceOn: %d\n", CP->mSliceOn)
						.StoreASCII("  SliceFrom: %f\n", CP->mSliceFrom)
						.StoreASCII("  SliceTo: %f\n", CP->mSliceTo)
						.StoreASCII("  GenUVS: %d\n", CP->mGenUVS);
				}
				break;

				case FLX_PRIM_CONE:
				{
					ConeParams* CP = (ConeParams*)mesh.mPrimParams;
					mGeomObjects
						.StoreASCII("\nCone primitive:\n")
						.StoreASCII("  Radius1: %f\n", CP->mRadius1)
						.StoreASCII("  Radius2: %f\n", CP->mRadius2)
						.StoreASCII("  Height: %f\n", CP->mHeight)
						.StoreASCII("  HSegs: %d\n", CP->mHSegs)
						.StoreASCII("  CapSegs: %d\n", CP->mCapSegs)
						.StoreASCII("  Sides: %d\n", CP->mSides)
						.StoreASCII("  Smooth: %d\n", CP->mSmooth)
						.StoreASCII("  SliceOn: %d\n", CP->mSliceOn)
						.StoreASCII("  SliceFrom: %f\n", CP->mSliceFrom)
						.StoreASCII("  SliceTo: %f\n", CP->mSliceTo)
						.StoreASCII("  GenUVS: %d\n", CP->mGenUVS);
				}
				break;

				case FLX_PRIM_TORUS:
				{
					TorusParams* TP = (TorusParams*)mesh.mPrimParams;
					mGeomObjects
						.StoreASCII("\nTorus primitive:\n")
						.StoreASCII("  Radius1: %f\n", TP->mRadius1)
						.StoreASCII("  Radius2: %f\n", TP->mRadius2)
						.StoreASCII("  Rotation: %f\n", TP->mRotation)
						.StoreASCII("  Twist: %f\n", TP->mTwist)
						.StoreASCII("  Segments: %d\n", TP->mSegments)
						.StoreASCII("  Sides: %d\n", TP->mSides)
						.StoreASCII("  Smooth: %d\n", TP->mSmooth)
						.StoreASCII("  SliceOn: %d\n", TP->mSliceOn)
						.StoreASCII("  SliceFrom: %f\n", TP->mSliceFrom)
						.StoreASCII("  SliceTo: %f\n", TP->mSliceTo)
						.StoreASCII("  GenUVS: %d\n", TP->mGenUVS);
				}
				break;

				case FLX_PRIM_TUBE:
				{
					TubeParams* TP = (TubeParams*)mesh.mPrimParams;
					mGeomObjects
						.StoreASCII("\nTube primitive:\n")
						.StoreASCII("  Radius1: %f\n", TP->mRadius1)
						.StoreASCII("  Radius2: %f\n", TP->mRadius2)
						.StoreASCII("  Height: %f\n", TP->mHeight)
						.StoreASCII("  Segments: %d\n", TP->mSegments)
						.StoreASCII("  CapSegs: %d\n", TP->mCapSegs)
						.StoreASCII("  Sides: %d\n", TP->mSides)
						.StoreASCII("  Smooth: %d\n", TP->mSmooth)
						.StoreASCII("  SliceOn: %d\n", TP->mSliceOn)
						.StoreASCII("  SliceFrom: %f\n", TP->mSliceFrom)
						.StoreASCII("  SliceTo: %f\n", TP->mSliceTo)
						.StoreASCII("  GenUVS: %d\n", TP->mGenUVS);
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
						.StoreASCII("\nPlane primitive:\n")
						.StoreASCII("  Length: %f\n", PP->mLength)
						.StoreASCII("  Width: %f\n", PP->mWidth)
						.StoreASCII("  WidthSegs: %f\n", PP->mWidthSegs)
						.StoreASCII("  LengthSegs: %f\n", PP->mLengthSegs)
						.StoreASCII("  Density: %f\n", PP->mDensity)
						.StoreASCII("  Scale: %f\n", PP->mScale)
						.StoreASCII("  GenUVS: %d\n", PP->mGenUVS);
				}
				break;

				case FLX_PRIM_HEDRA:
				{
					HedraParams* HP = (HedraParams*)mesh.mPrimParams;
					mGeomObjects
						.StoreASCII("\nHedra primitive:\n")
						.StoreASCII("  Radius: %f\n", HP->mRadius)
						.StoreASCII("  Family: %d\n", HP->mFamily)
						.StoreASCII("  P: %f\n", HP->mP)
						.StoreASCII("  Q: %f\n", HP->mQ)
						.StoreASCII("  ScaleP: %f\n", HP->mScaleP)
						.StoreASCII("  ScaleQ: %f\n", HP->mScaleQ)
						.StoreASCII("  ScaleR: %f\n", HP->mScaleR)
						.StoreASCII("  Vertices: %d\n", HP->mVertices)
						.StoreASCII("  GenUVS: %d\n", HP->mGenUVS);
				}
				break;

				case FLX_PRIM_CAPSULE:
				{
					CapsuleParams* CP = (CapsuleParams*)mesh.mPrimParams;
					mGeomObjects
						.StoreASCII("\nCapsule primitive:\n")
						.StoreASCII("  Radius: %f\n", CP->mRadius)
						.StoreASCII("  Height: %f\n", CP->mHeight)
						.StoreASCII("  Centers: %d\n", CP->mCenters)
						.StoreASCII("  Sides: %d\n", CP->mSides)
						.StoreASCII("  HSegs: %d\n", CP->mHSegs)
						.StoreASCII("  Smooth: %d\n", CP->mSmooth)
						.StoreASCII("  SliceOn: %d\n", CP->mSliceOn)
						.StoreASCII("  SliceFrom: %f\n", CP->mSliceFrom)
						.StoreASCII("  SliceTo: %f\n", CP->mSliceTo)
						.StoreASCII("  GenUVS: %d\n", CP->mGenUVS);
				}
				break;
			}
		}

		// Export MAX native mesh
		mGeomObjects.StoreASCII("\nMAX native mesh data:\n");

		mGeomObjects.StoreASCII(Mesh->mNbFaces).StoreASCII(" faces\n");
		mGeomObjects.StoreASCII(Mesh->mNbVerts).StoreASCII(" vertices\n");
		mGeomObjects.StoreASCII(Mesh->mNbTVerts).StoreASCII(" mapping coordinates\n");
		mGeomObjects.StoreASCII(Mesh->mNbCVerts).StoreASCII(" vertex colors\n");

		if(Mesh->mParity)	mGeomObjects.StoreASCII("Parity is true.\n");
		else				mGeomObjects.StoreASCII("Parity is false.\n");

		// Export vertices
		if(Mesh->mVerts)
		{
			mGeomObjects.StoreASCII("\nVertices:\n");
			const NakedPoint* v = Mesh->mVerts;
			for(udword i=0;i<Mesh->mNbVerts;i++)
			{
				mGeomObjects.StoreASCII("%f %f %f\n", v[i].x, v[i].y, v[i].z);
			}
		}

		// If the mesh is a skin, export skin-related data
		if(mesh.mIsSkin)
		{
			// Export links to the driving skeleton
			if(!Mesh->mBonesNb)
			{
				// A simple skin with one bone/vertex.
				const NakedPoint* v = Mesh->mOffsetVectors;
				if(v)
				{
					mGeomObjects.StoreASCII("\nOffset vectors:\n");
					for(udword i=0;i<Mesh->mNbVerts;i++)
					{
						mGeomObjects.StoreASCII("%f %f %f\n", v[i].x, v[i].y, v[i].z);
					}
				}

				// CSIDs
				if(Mesh->mBonesCSID)	// formerly known as "mBonesID"
				{
					mGeomObjects.StoreASCII("\nBones ID:\n");
					for(udword i=0;i<Mesh->mNbVerts;i++)
					{
						mGeomObjects.StoreASCII(Mesh->mBonesCSID[i]).StoreASCII(" ");
						if((i&63)==63)	mGeomObjects.StoreASCII("\n");
					}
					mGeomObjects.StoreASCII("\n");
				}

				// Local IDs [since version 1.14]
				if(Mesh->mBonesLocalID)
				{
					mGeomObjects.StoreASCII("\nBones local ID:\n");
					for(udword i=0;i<Mesh->mNbVerts;i++)
					{
						mGeomObjects.StoreASCII(Mesh->mBonesLocalID[i]).StoreASCII(" ");
						if((i&63)==63)	mGeomObjects.StoreASCII("\n");
					}
					mGeomObjects.StoreASCII("\n");
				}

				// Bone names [since version 1.14]
				if(Mesh->mBonesName)
				{
					mGeomObjects.StoreASCII("\nBones names:\n");
					for(udword i=0;i<Mesh->mNbVerts;i++)
					{
						mGeomObjects.StoreASCII(Mesh->mBonesName[i]).StoreASCII("\n");
					}
					mGeomObjects.StoreASCII("\n");
				}
			}
			else
			{
				// A skin with multiple bones/vertex.
				mGeomObjects.StoreASCII("\nBones Nb:\n");
				udword Sum = 0;
				for(udword i=0;i<Mesh->mNbVerts;i++)
				{
					mGeomObjects.StoreASCII(Mesh->mBonesNb[i]).StoreASCII(" ");
					if((i&63)==63)	mGeomObjects.StoreASCII("\n");
					Sum+=Mesh->mBonesNb[i];
				}
				mGeomObjects.StoreASCII("\n");

				// CSIDs
				if(Mesh->mBonesCSID)
				{
					mGeomObjects.StoreASCII("\nBones ID:\n");
					for(udword i=0;i<Sum;i++)
					{
						mGeomObjects.StoreASCII(Mesh->mBonesCSID[i]).StoreASCII(" ");
						if((i&63)==63)	mGeomObjects.StoreASCII("\n");
					}
					mGeomObjects.StoreASCII("\n");
				}

				// Local IDs [since version 1.14]
				if(Mesh->mBonesLocalID)
				{
					mGeomObjects.StoreASCII("\nBones local ID:\n");
					for(udword i=0;i<Sum;i++)
					{
						mGeomObjects.StoreASCII(Mesh->mBonesLocalID[i]).StoreASCII(" ");
						if((i&63)==63)	mGeomObjects.StoreASCII("\n");
					}
					mGeomObjects.StoreASCII("\n");
				}

				// Bone names [since version 1.14]
				if(Mesh->mBonesName)
				{
					mGeomObjects.StoreASCII("\nBones names:\n");
					for(udword i=0;i<Sum;i++)
					{
						mGeomObjects.StoreASCII(Mesh->mBonesName[i]).StoreASCII("\n");
					}
					mGeomObjects.StoreASCII("\n");
				}

				// Weights
				if(Mesh->mWeights)
				{
					mGeomObjects.StoreASCII("\nWeights:\n");
					for(udword i=0;i<Sum;i++)
					{
						mGeomObjects.StoreASCII(Mesh->mWeights[i]).StoreASCII(" ");
						if((i&63)==63)	mGeomObjects.StoreASCII("\n");
					}
					mGeomObjects.StoreASCII("\n");
				}

				// Offset vectors
				if(Mesh->mOffsetVectors)
				{
					const NakedPoint* v = Mesh->mOffsetVectors;
					mGeomObjects.StoreASCII("\nOffset vectors:\n");
					for(udword i=0;i<Sum;i++)
					{
						mGeomObjects.StoreASCII("%f %f %f\n", v[i].x, v[i].y, v[i].z);
					}
					mGeomObjects.StoreASCII("\n");
				}
			}

			// Export compatible skeletal information
			const MAXSkeleton* Skel = Mesh->mSkeleton;
			if(Skel)
			{
				mGeomObjects.StoreASCII("\nSkeletal information: ").StoreASCII(Skel->mNbBones).StoreASCII(" bones:\n");
				mGeomObjects.StoreASCII("(CSID, parent CSID)\n");
				udword* ID = Skel->mID;
				for(udword i=0;i<Skel->mNbBones;i++)
				{
					udword CSID = *ID++;
					udword pCSID = *ID++;
					mGeomObjects.StoreASCII(CSID).StoreASCII(" ").StoreASCII(pCSID).StoreASCII("\n");
				}
			}
		}

		// Export UVW mappings
		if(Mesh->mTVerts)
		{
			mGeomObjects.StoreASCII("\nMapping coordinates:\n");
			const NakedPoint* v = Mesh->mTVerts;
			for(udword i=0;i<Mesh->mNbTVerts;i++)
			{
				mGeomObjects.StoreASCII("%f %f %f\n", v[i].x, v[i].y, v[i].z);
			}
		}

		// Export vertex-colors
		if(Mesh->mCVerts)
		{
			mGeomObjects.StoreASCII("\nVertex colors:\n");
			const NakedPoint* v = Mesh->mCVerts;
			for(udword i=0;i<Mesh->mNbCVerts;i++)
			{
				mGeomObjects.StoreASCII("%f %f %f\n", v[i].x, v[i].y, v[i].z);
			}
		}

		// Export vertex-alpha
		if(Mesh->mVertexAlpha)
		{
			mGeomObjects.StoreASCII("\nVertex alpha:\n");
			const float* v = Mesh->mVertexAlpha;
			for(udword i=0;i<Mesh->mNbVerts;i++)	// Beware: mNbVerts !
			{
				mGeomObjects.StoreASCII("%f\n", v[i]);
			}
		}

		// Export faces
		if(Mesh->mFaces)
		{
			mGeomObjects.StoreASCII("\nFaces:\n(vref0 vref1 vref2  tref0 tref1 tref2  cref0 cref1 cref2  MatID Smg EdgeVisCode)\n");
			const MAXFace* f = Mesh->mFaces;
			for(udword i=0;i<Mesh->mNbFaces;i++)
			{
				mGeomObjects
					.StoreASCII(f[i].VRef[0]).StoreASCII(" ").StoreASCII(f[i].VRef[1]).StoreASCII(" ").StoreASCII(f[i].VRef[2]).StoreASCII("  ")
					.StoreASCII(f[i].TRef[0]).StoreASCII(" ").StoreASCII(f[i].TRef[1]).StoreASCII(" ").StoreASCII(f[i].TRef[2]).StoreASCII("  ")
					.StoreASCII(f[i].CRef[0]).StoreASCII(" ").StoreASCII(f[i].CRef[1]).StoreASCII(" ").StoreASCII(f[i].CRef[2]).StoreASCII("  ")
					.StoreASCII(f[i].MatID).StoreASCII(" ").StoreASCII(f[i].Smg).StoreASCII(" ").StoreASCII(f[i].EdgeVis).StoreASCII("\n");
			}
		}

		// Export the convex hull
		if(Mesh->mFlags&MESH_CONVEXHULL)
		{
			mGeomObjects.StoreASCII("\nConvex hull:");
			mGeomObjects.StoreASCII("\n").StoreASCII(Mesh->mConvexHull->mNbVerts).StoreASCII(" vertices");
			mGeomObjects.StoreASCII("\n").StoreASCII(Mesh->mConvexHull->mNbFaces).StoreASCII(" faces\n");

			if(Mesh->mConvexHull->mVerts)
			{
				mGeomObjects.StoreASCII("\nConvex hull vertices:\n");
				const NakedPoint* v = Mesh->mConvexHull->mVerts;
				for(udword i=0;i<Mesh->mConvexHull->mNbVerts;i++)
				{
					mGeomObjects.StoreASCII(v[i].x).StoreASCII(" ").StoreASCII(v[i].y).StoreASCII(" ").StoreASCII(v[i].z).StoreASCII("\n");
				}
			}

			if(Mesh->mConvexHull->mFaces)
			{
				mGeomObjects.StoreASCII("\nConvex hull faces:\n(vref0 vref1 vref2)\n");
				const udword* f = Mesh->mConvexHull->mFaces;
				for(udword i=0;i<Mesh->mConvexHull->mNbFaces;i++)
				{
					mGeomObjects.StoreASCII(f[i*3+0]).StoreASCII(" ").StoreASCII(f[i*3+1]).StoreASCII(" ").StoreASCII(f[i*3+2]).StoreASCII("\n");
				}
			}
		}

		// Export the bounding sphere
		if(Mesh->mFlags&MESH_BOUNDINGSPHERE)
		{
			mGeomObjects
				.StoreASCII("\nBounding sphere:")
				.StoreASCII("\nCenter: ").StoreASCII(Mesh->mBSCenter.x).StoreASCII(" ").StoreASCII(Mesh->mBSCenter.y).StoreASCII(" ").StoreASCII(Mesh->mBSCenter.z)
				.StoreASCII("\nRadius: ").StoreASCII(Mesh->mBSRadius).StoreASCII("\n");
		}

		// Export the inertia tensor
		if(Mesh->mFlags&MESH_INERTIATENSOR)
		{
			ITensor* T = Mesh->mTensor;
			mGeomObjects
				.StoreASCII("\nVolume integrals:")

				.StoreASCII("\nCOM:  ").StoreASCII(T->COM.x).StoreASCII(" ").StoreASCII(T->COM.y).StoreASCII(" ").StoreASCII(T->COM.z)

				.StoreASCII("\nMass: ").StoreASCII(T->Mass).StoreASCII("\n")

				.StoreASCII("\nInertia tensor:\n")
				.StoreASCII(T->InertiaTensor[0][0]).StoreASCII(" ").StoreASCII(T->InertiaTensor[0][1]).StoreASCII(" ").StoreASCII(T->InertiaTensor[0][2]).StoreASCII("\n")
				.StoreASCII(T->InertiaTensor[1][0]).StoreASCII(" ").StoreASCII(T->InertiaTensor[1][1]).StoreASCII(" ").StoreASCII(T->InertiaTensor[1][2]).StoreASCII("\n")
				.StoreASCII(T->InertiaTensor[2][0]).StoreASCII(" ").StoreASCII(T->InertiaTensor[2][1]).StoreASCII(" ").StoreASCII(T->InertiaTensor[2][2]).StoreASCII("\n")

				.StoreASCII("\nCOM Inertia tensor:\n")
				.StoreASCII(T->COMInertiaTensor[0][0]).StoreASCII(" ").StoreASCII(T->COMInertiaTensor[0][1]).StoreASCII(" ").StoreASCII(T->COMInertiaTensor[0][2]).StoreASCII("\n")
				.StoreASCII(T->COMInertiaTensor[1][0]).StoreASCII(" ").StoreASCII(T->COMInertiaTensor[1][1]).StoreASCII(" ").StoreASCII(T->COMInertiaTensor[1][2]).StoreASCII("\n")
				.StoreASCII(T->COMInertiaTensor[2][0]).StoreASCII(" ").StoreASCII(T->COMInertiaTensor[2][1]).StoreASCII(" ").StoreASCII(T->COMInertiaTensor[2][2]).StoreASCII("\n");
		}

		// Export consolidated mesh
		if(Mesh->mFlags&MESH_CONSOLIDATION)
		{
			mGeomObjects.StoreASCII("\nCONSOLIDATION\n");

			// Topology
			{
				MBTopology& Topo = mesh.mCleanMesh->Topology;

				mGeomObjects
				.StoreASCII("\nTOPOLOGY\n")
				.StoreASCII("\nNb faces:      ").StoreASCII(Topo.NbFaces)
				.StoreASCII("\nNb submeshes:  ").StoreASCII(Topo.NbSubmeshes)
				.StoreASCII("\n");

				// Submeshes
				for(udword i=0;i<Topo.NbSubmeshes;i++)
				{
					MBSubmesh* CurSM = &Topo.SubmeshProperties[i];
					mGeomObjects
					.StoreASCII("\nSubmesh #").StoreASCII(i)
					.StoreASCII("\nMaterial ID:   ").StoreASCII(CurSM->MatID)
					.StoreASCII("\nSmGrps:        ").StoreASCII(CurSM->SmGrp)
					.StoreASCII("\nNbFaces:       ").StoreASCII(CurSM->NbFaces)
					.StoreASCII("\nNbVerts:       ").StoreASCII(CurSM->NbVerts)
					.StoreASCII("\nNbSubstrips:   ").StoreASCII(CurSM->NbSubstrips)
					.StoreASCII("\n");
				}

				// Connectivity
				udword* VRefs = Topo.VRefs;
				for(udword i=0;i<Topo.NbSubmeshes;i++)
				{
					uword NbFaces = Topo.FacesInSubmesh[i];

					mGeomObjects.StoreASCII("\nSubmesh #").StoreASCII(i).StoreASCII(" (").StoreASCII(NbFaces).StoreASCII(" faces)\n");

					// Save faces (Ref0, Ref1, Ref2)
					for(uword j=0;j<NbFaces;j++)
					{
						udword Ref0 = *VRefs++;
						udword Ref1 = *VRefs++;
						udword Ref2 = *VRefs++;
						mGeomObjects.StoreASCII(Ref0).StoreASCII(" ").StoreASCII(Ref1).StoreASCII(" ").StoreASCII(Ref2).StoreASCII("\n");
					}
				}

				// Face normals
				mGeomObjects.StoreASCII("\nFace normals:\n");
				float* Normals = Topo.Normals;
				if(Normals)
				{
					for(udword i=0;i<Topo.NbFaces;i++)
					{
						float nx = Normals[i*3+0];
						float ny = Normals[i*3+1];
						float nz = Normals[i*3+2];
						mGeomObjects.StoreASCII(nx).StoreASCII(" ").StoreASCII(ny).StoreASCII(" ").StoreASCII(nz).StoreASCII("\n");
					}
				}
				else mGeomObjects.StoreASCII("(not available)\n");
			}

			// Geometry
			{
				MBGeometry& Geo = mesh.mCleanMesh->Geometry;

				mGeomObjects
				.StoreASCII("\nGEOMETRY\n")
				.StoreASCII("\nNb GeomPts:    ").StoreASCII(Geo.NbGeomPts)
				.StoreASCII("\nNb Verts:      ").StoreASCII(Geo.NbVerts)
				.StoreASCII("\nNb TVerts:     ").StoreASCII(Geo.NbTVerts)
				// No number of CVerts since they're not indexed!
				.StoreASCII("\n");

				// Indexed geometry
				udword MaxRef=0;
				if(Geo.VertsRefs)
				{
					mGeomObjects.StoreASCII("\nIndexed geometry:\n");
					for(udword i=0;i<Geo.NbVerts;i++)
					{
						udword Ref = Geo.VertsRefs[i];
						if(Ref>MaxRef)	MaxRef=Ref;
						mGeomObjects.StoreASCII(Ref).StoreASCII(" ");
						if((i&63)==63)	mGeomObjects.StoreASCII("\n");
					}
					mGeomObjects.StoreASCII("\n");
				}

				// Vertices
				udword NbVerts = Geo.VertsRefs ? (MaxRef+1) : Geo.NbVerts;
				mGeomObjects.StoreASCII("\nVertices: (").StoreASCII(NbVerts).StoreASCII(")\n");
				for(udword i=0;i<NbVerts;i++)
				{
					float x = Geo.Verts[i*3+0];
					float y = Geo.Verts[i*3+1];
					float z = Geo.Verts[i*3+2];
					mGeomObjects.StoreASCII(x).StoreASCII(" ").StoreASCII(y).StoreASCII(" ").StoreASCII(z).StoreASCII(" ").StoreASCII("\n");
				}
				mGeomObjects.StoreASCII("\n");

				// Indexed UVWs
				MaxRef = 0;
				if(Geo.TVertsRefs)
				{
					mGeomObjects.StoreASCII("\nIndexed UVWs:\n");
					for(udword i=0;i<Geo.NbVerts;i++)
					{
						udword Ref = Geo.TVertsRefs[i];
						if(Ref>MaxRef)	MaxRef=Ref;
						mGeomObjects.StoreASCII(Ref).StoreASCII(" ");
						if((i&63)==63)	mGeomObjects.StoreASCII("\n");
					}
					mGeomObjects.StoreASCII("\n");
				}

				// Vertices
				if(mSettings.mExpUVW && Geo.TVerts)
				{
					udword NbTVerts = Geo.TVertsRefs ? (MaxRef+1) : Geo.NbVerts;
					mGeomObjects.StoreASCII("\nUVW mappings: (").StoreASCII(NbTVerts).StoreASCII(")\n");
					float* p = Geo.TVerts;
					bool UseW = !mSettings.mDiscardW;
					for(udword i=0;i<NbTVerts;i++)
					{
						float u = *p++;	mGeomObjects.StoreASCII(u).StoreASCII(" ");
						float v = *p++;	mGeomObjects.StoreASCII(v).StoreASCII(" ");
						if(UseW)
						{
							float w = *p++;	mGeomObjects.StoreASCII(w).StoreASCII(" ");
						}
						mGeomObjects.StoreASCII("\n");
					}
				}

				// Normals
				if(Geo.Normals)
				{
					udword NbNormals = Geo.NbVerts;
					mGeomObjects.StoreASCII("\nVertex normals: (").StoreASCII(NbNormals).StoreASCII(")\n");
					for(udword i=0;i<NbNormals;i++)
					{
						float x = Geo.Normals[i*3+0];
						float y = Geo.Normals[i*3+1];
						float z = Geo.Normals[i*3+2];
						mGeomObjects.StoreASCII(x).StoreASCII(" ").StoreASCII(y).StoreASCII(" ").StoreASCII(z).StoreASCII(" ").StoreASCII("\n");
					}
				}
				else mGeomObjects.StoreASCII("\nVertex normals: (not available)\n");

				// Vertex colors
				if(mSettings.mExpVTXColor && Geo.CVerts)
				{
					udword NbVtxColors = Geo.NbVerts;	// Because non-indexed
					mGeomObjects.StoreASCII("\nVertex colors: (").StoreASCII(NbVtxColors).StoreASCII(")\n");
					if(Mesh->mFlags&MESH_DWORD_COLORS)
					{
						// Since the internal code only handled "points", vertex colors/alpha have been packed to dwords
						for(udword i=0;i<NbVtxColors;i++)
						{
							float Data = Geo.CVerts[i*3+0];	// We only need the red channel
							udword Color = IR(Data);
							// ### should be dumped in Hexa one day...
							mGeomObjects.StoreASCII(Color).StoreASCII("\n");
						}
					}
					else
					{
						for(udword i=0;i<NbVtxColors;i++)
						{
							// Bugfix here since Flexporter 1.12, thanks to Jere Sanisalo.
							float r = Geo.CVerts[i*3+0];
							float g = Geo.CVerts[i*3+1];
							float b = Geo.CVerts[i*3+2];
							mGeomObjects.StoreASCII(r).StoreASCII(" ").StoreASCII(g).StoreASCII(" ").StoreASCII(b).StoreASCII(" ").StoreASCII("\n");
						}
					}
				}

				// Normal info
				if(mSettings.mExportNormalInfo && Geo.NormalInfo)
				{
					udword NormalInfoSize = Geo.NormalInfoSize;
					mGeomObjects.StoreASCII("\nNormalInfo: (").StoreASCII(NormalInfoSize).StoreASCII(")\n");
					for(udword i=0;i<NormalInfoSize;i++)
					{
						udword d = Geo.NormalInfo[i];
						mGeomObjects.StoreASCII(d).StoreASCII(" ");
						if((i&63)==63)	mGeomObjects.StoreASCII("\n");
					}
				}
			}

			// Materials
			{
				MBMaterials& Mtl = mesh.mCleanMesh->Materials;

				mGeomObjects
				.StoreASCII("\nMATERIALS\n")
				.StoreASCII("\nNb materials:  ").StoreASCII(Mtl.NbMtls)
				.StoreASCII("\n");

				for(udword i=0;i<Mtl.NbMtls;i++)
				{
					MBMatInfo* CurMtl = &Mtl.MaterialInfo[i];

					mGeomObjects
					.StoreASCII("\nMaterial #").StoreASCII(i).StoreASCII(":")
					.StoreASCII("\nID:          ").StoreASCII(CurMtl->MatID)
					.StoreASCII("\nNbFaces:     ").StoreASCII(CurMtl->NbFaces)
					.StoreASCII("\nNbVerts:     ").StoreASCII(CurMtl->NbVerts)
					.StoreASCII("\nNbSubmeshes: ").StoreASCII(CurMtl->NbSubmeshes)
					.StoreASCII("\n");
				}
			}
		}
	}

	return true;
}
