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

bool MAXConverter::AddPrimitive(Object* node_object, MeshDescriptor& mesh_desc)
{
	if(!node_object)	return false;

	// Get time info back
	TimeValue CurrentTime = mTimeInfo.mCurrentTime;

	Interval Inter = FOREVER;
	Class_ID Type = node_object->ClassID();

	// From gsphere.cpp
	#define GSPHERE_CLASS_ID 0
	// From scuba.cpp
	static Class_ID SCUBA_CLASS_ID(0x6d3d77ac, 0x79c939a9);
	// From gridobj.cpp
	#define GRIDID 0x81f1dfc, 0x77566f65

	if(Type==Class_ID(BOXOBJ_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Box)\n");

		// Box
		SimpleObject* SObj = (SimpleObject*)node_object;
		IParamBlock* PB = SObj->pblock;
		if(PB)
		{
			// Create new primitive params
			BoxParams* PP = new BoxParams;
			mesh_desc.mPrimParams = PP;

			// From boxobj.cpp:
			// Parameter block indices
			#define BOX_PB_LENGTH	0
			#define BOX_PB_WIDTH	1
			#define BOX_PB_HEIGHT	2
			#define BOX_PB_WSEGS	3
			#define BOX_PB_LSEGS	4
			#define BOX_PB_HSEGS	5
			#define BOX_PB_GENUVS	6

			PB->GetValue(BOX_PB_LENGTH,	CurrentTime, PP->mLength,	Inter);
			PB->GetValue(BOX_PB_WIDTH,	CurrentTime, PP->mWidth,	Inter);
			PB->GetValue(BOX_PB_HEIGHT,	CurrentTime, PP->mHeight,	Inter);
			PB->GetValue(BOX_PB_LSEGS,	CurrentTime, PP->mLSegs,	Inter);
			PB->GetValue(BOX_PB_WSEGS,	CurrentTime, PP->mWSegs,	Inter);
			PB->GetValue(BOX_PB_HSEGS,	CurrentTime, PP->mHSegs,	Inter);
			PB->GetValue(BOX_PB_GENUVS,	CurrentTime, PP->mGenUVS,	Inter);

			// Take global scale into account
			PP->mLength *= mSettings->mGlobalScale;
			PP->mWidth *= mSettings->mGlobalScale;
			PP->mHeight *= mSettings->mGlobalScale;
		}
	}
	else if(Type==Class_ID(SPHERE_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Sphere)\n");

		// Sphere
		SimpleObject* SObj = (SimpleObject*)node_object;
		IParamBlock* PB = SObj->pblock;
		if(PB)
		{
			// Create new primitive params
			SphereParams* PP = new SphereParams;
			mesh_desc.mPrimParams = PP;

			// From sphere.cpp:
			// Parameter block indices
			#define SPHERE_PB_RADIUS	0
			#define SPHERE_PB_SEGS		1
			#define SPHERE_PB_SMOOTH	2
			#define SPHERE_PB_HEMI		3
			#define SPHERE_PB_SQUASH	4
			#define SPHERE_PB_RECENTER	5
			#define SPHERE_PB_GENUVS	6
			#define SPHERE_PB_SLICEON	7
			#define SPHERE_PB_SLICEFROM	8
			#define SPHERE_PB_SLICETO	9

			PB->GetValue(SPHERE_PB_RADIUS,		CurrentTime, PP->mRadius,		Inter);
			PB->GetValue(SPHERE_PB_SEGS,		CurrentTime, PP->mSegments,		Inter);
			PB->GetValue(SPHERE_PB_SMOOTH,		CurrentTime, PP->mSmooth,		Inter);
			PB->GetValue(SPHERE_PB_HEMI,		CurrentTime, PP->mHemisphere,	Inter);
			PB->GetValue(SPHERE_PB_SQUASH,		CurrentTime, PP->mSquash,		Inter);
			PB->GetValue(SPHERE_PB_RECENTER,	CurrentTime, PP->mRecenter,		Inter);
			PB->GetValue(SPHERE_PB_GENUVS,		CurrentTime, PP->mGenUVS,		Inter);
			PB->GetValue(SPHERE_PB_SLICEON,		CurrentTime, PP->mSliceOn,		Inter);
			PB->GetValue(SPHERE_PB_SLICEFROM,	CurrentTime, PP->mSliceFrom,	Inter);
			PB->GetValue(SPHERE_PB_SLICETO,		CurrentTime, PP->mSliceTo,		Inter);

			// Take global scale into account
			PP->mRadius *= mSettings->mGlobalScale;
			// ### Finish this
		}
	}
	else if(Type==Class_ID(GSPHERE_CLASS_ID, 32670))
	{
		mArrayLog.StoreASCII("(GeoSphere)\n");

		// GeoSphere
		SimpleObject2* SObj2 = (SimpleObject2*)node_object;
		IParamBlock2* PB = SObj2->pblock2;
		if(PB)
		{
			// Create new primitive params
			GeosphereParams* PP = new GeosphereParams;
			mesh_desc.mPrimParams = PP;

			PB->GetValue(0,	CurrentTime, PP->mRadius,		Inter);
			PB->GetValue(1,	CurrentTime, PP->mSegments,		Inter);
			PB->GetValue(2,	CurrentTime, PP->mGenType,		Inter);
			PB->GetValue(3,	CurrentTime, PP->mHemisphere,	Inter);
			PB->GetValue(4,	CurrentTime, PP->mSmooth,		Inter);
			PB->GetValue(5,	CurrentTime, PP->mRecenter,		Inter);
			PB->GetValue(6,	CurrentTime, PP->mGenUVS,		Inter);

			// Take global scale into account
			PP->mRadius *= mSettings->mGlobalScale;
			// ### Finish this
		}
	}
	else if(Type==Class_ID(CYLINDER_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Cylinder)\n");

		// Cylinder
		SimpleObject* SObj = (SimpleObject*)node_object;
		IParamBlock* PB = SObj->pblock;
		if(PB)
		{
			// Create new primitive params
			CylinderParams* PP = new CylinderParams;
			mesh_desc.mPrimParams = PP;

			// From cyl.cpp:
			// Parameter map indices
			#define CYLINDER_PB_RADIUS		0
			#define CYLINDER_PB_HEIGHT		1
			#define CYLINDER_PB_SEGMENTS	2
			#define CYLINDER_PB_CAPSEGMENTS	3
			#define CYLINDER_PB_SIDES		4
			#define CYLINDER_PB_SMOOTH		5
			#define CYLINDER_PB_SLICEON		6
			#define CYLINDER_PB_PIESLICE1	7
			#define CYLINDER_PB_PIESLICE2	8
			#define CYLINDER_PB_GENUVS		9

			PB->GetValue(CYLINDER_PB_RADIUS,		CurrentTime, PP->mRadius,		Inter);
			PB->GetValue(CYLINDER_PB_HEIGHT,		CurrentTime, PP->mHeight,		Inter);
			PB->GetValue(CYLINDER_PB_SEGMENTS,		CurrentTime, PP->mHSegs,		Inter);
			PB->GetValue(CYLINDER_PB_CAPSEGMENTS,	CurrentTime, PP->mCapSegs,		Inter);
			PB->GetValue(CYLINDER_PB_SIDES,			CurrentTime, PP->mSides,		Inter);
			PB->GetValue(CYLINDER_PB_SMOOTH,		CurrentTime, PP->mSmooth,		Inter);
			PB->GetValue(CYLINDER_PB_SLICEON,		CurrentTime, PP->mSliceOn,		Inter);
			PB->GetValue(CYLINDER_PB_PIESLICE1,		CurrentTime, PP->mSliceFrom,	Inter);
			PB->GetValue(CYLINDER_PB_PIESLICE2,		CurrentTime, PP->mSliceTo,		Inter);
			PB->GetValue(CYLINDER_PB_GENUVS,		CurrentTime, PP->mGenUVS,		Inter);

			// Take global scale into account
			PP->mRadius *= mSettings->mGlobalScale;
			PP->mHeight *= mSettings->mGlobalScale;
			// ### Finish this
		}
	}
	else if(Type==Class_ID(CONE_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Cone)\n");

		// Cone
		SimpleObject* SObj = (SimpleObject*)node_object;
		IParamBlock* PB = SObj->pblock;
		if(PB)
		{
			// Create new primitive params
			ConeParams* PP = new ConeParams;
			mesh_desc.mPrimParams = PP;

			// From cone.cpp:
			// Parameter map indices
			#define CONE_PB_RADIUS1		0
			#define CONE_PB_RADIUS2		1
			#define CONE_PB_HEIGHT		2
			#define CONE_PB_SEGMENTS	3
			#define CONE_PB_CAPSEGMENTS	4
			#define CONE_PB_SIDES		5
			#define CONE_PB_SMOOTH		6
			#define CONE_PB_SLICEON		7
			#define CONE_PB_PIESLICE1	8
			#define CONE_PB_PIESLICE2	9
			#define CONE_PB_GENUVS		10

			PB->GetValue(CONE_PB_RADIUS1,		CurrentTime, PP->mRadius1,		Inter);
			PB->GetValue(CONE_PB_RADIUS2,		CurrentTime, PP->mRadius2,		Inter);
			PB->GetValue(CONE_PB_HEIGHT,		CurrentTime, PP->mHeight,		Inter);
			PB->GetValue(CONE_PB_SEGMENTS,		CurrentTime, PP->mHSegs,		Inter);
			PB->GetValue(CONE_PB_CAPSEGMENTS,	CurrentTime, PP->mCapSegs,		Inter);
			PB->GetValue(CONE_PB_SIDES,			CurrentTime, PP->mSides,		Inter);
			PB->GetValue(CONE_PB_SMOOTH,		CurrentTime, PP->mSmooth,		Inter);
			PB->GetValue(CONE_PB_SLICEON,		CurrentTime, PP->mSliceOn,		Inter);
			PB->GetValue(CONE_PB_PIESLICE1,		CurrentTime, PP->mSliceFrom,	Inter);
			PB->GetValue(CONE_PB_PIESLICE2,		CurrentTime, PP->mSliceTo,		Inter);
			PB->GetValue(CONE_PB_GENUVS,		CurrentTime, PP->mGenUVS,		Inter);

			// Take global scale into account
			PP->mRadius1 *= mSettings->mGlobalScale;
			PP->mRadius2 *= mSettings->mGlobalScale;
			PP->mHeight *= mSettings->mGlobalScale;
			// ### Finish this
		}
	}
	else if(Type==Class_ID(TORUS_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Torus)\n");

		// Torus
		SimpleObject* SObj = (SimpleObject*)node_object;
		IParamBlock* PB = SObj->pblock;
		if(PB)
		{
			// Create new primitive params
			TorusParams* PP = new TorusParams;
			mesh_desc.mPrimParams = PP;

			// From torus.cpp:
			// Parameter map indices
			#define TORUS_PB_RADIUS		0
			#define TORUS_PB_RADIUS2	1
			#define TORUS_PB_ROTATION	2
			#define TORUS_PB_TWIST		3
			#define TORUS_PB_SEGMENTS	4
			#define TORUS_PB_SIDES		5
			#define TORUS_PB_SMOOTH		6
			#define TORUS_PB_SLICEON	7
			#define TORUS_PB_PIESLICE1	8
			#define TORUS_PB_PIESLICE2	9
			#define TORUS_PB_GENUVS		10

			PB->GetValue(TORUS_PB_RADIUS,		CurrentTime, PP->mRadius1,		Inter);
			PB->GetValue(TORUS_PB_RADIUS2,		CurrentTime, PP->mRadius2,		Inter);
			PB->GetValue(TORUS_PB_ROTATION,		CurrentTime, PP->mRotation,		Inter);
			PB->GetValue(TORUS_PB_TWIST,		CurrentTime, PP->mTwist,		Inter);
			PB->GetValue(TORUS_PB_SEGMENTS,		CurrentTime, PP->mSegments,		Inter);
			PB->GetValue(TORUS_PB_SIDES,		CurrentTime, PP->mSides,		Inter);
			PB->GetValue(TORUS_PB_SMOOTH,		CurrentTime, PP->mSmooth,		Inter);
			PB->GetValue(TORUS_PB_SLICEON,		CurrentTime, PP->mSliceOn,		Inter);
			PB->GetValue(TORUS_PB_PIESLICE1,	CurrentTime, PP->mSliceFrom,	Inter);
			PB->GetValue(TORUS_PB_PIESLICE2,	CurrentTime, PP->mSliceTo,		Inter);
			PB->GetValue(TORUS_PB_GENUVS,		CurrentTime, PP->mGenUVS,		Inter);

			// Take global scale into account
			PP->mRadius1 *= mSettings->mGlobalScale;
			PP->mRadius2 *= mSettings->mGlobalScale;
			// ### Finish this
		}
	}
	else if(Type==Class_ID(TUBE_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Tube)\n");

		// Tube
		SimpleObject* SObj = (SimpleObject*)node_object;
		IParamBlock* PB = SObj->pblock;
		if(PB)
		{
			// Create new primitive params
			TubeParams* PP = new TubeParams;
			mesh_desc.mPrimParams = PP;

			// From tube.cpp:
			// Parameter map indices
			#define TUBE_PB_RADIUS		0
			#define TUBE_PB_RADIUS2		1
			#define TUBE_PB_HEIGHT		2
			#define TUBE_PB_SEGMENTS	3
			#define TUBE_PB_CAPSEGMENTS	4
			#define TUBE_PB_SIDES		5
			#define TUBE_PB_SMOOTH		6
			#define TUBE_PB_SLICEON		7
			#define TUBE_PB_PIESLICE1	8
			#define TUBE_PB_PIESLICE2	9
			#define TUBE_PB_GENUVS		10

			PB->GetValue(TUBE_PB_RADIUS,		CurrentTime, PP->mRadius1,		Inter);
			PB->GetValue(TUBE_PB_RADIUS2,		CurrentTime, PP->mRadius2,		Inter);
			PB->GetValue(TUBE_PB_HEIGHT,		CurrentTime, PP->mHeight,		Inter);
			PB->GetValue(TUBE_PB_SEGMENTS,		CurrentTime, PP->mSegments,		Inter);
			PB->GetValue(TUBE_PB_CAPSEGMENTS,	CurrentTime, PP->mCapSegs,		Inter);
			PB->GetValue(TUBE_PB_SIDES,			CurrentTime, PP->mSides,		Inter);
			PB->GetValue(TUBE_PB_SMOOTH,		CurrentTime, PP->mSmooth,		Inter);
			PB->GetValue(TUBE_PB_SLICEON,		CurrentTime, PP->mSliceOn,		Inter);
			PB->GetValue(TUBE_PB_PIESLICE1,		CurrentTime, PP->mSliceFrom,	Inter);
			PB->GetValue(TUBE_PB_PIESLICE2,		CurrentTime, PP->mSliceTo,		Inter);
			PB->GetValue(TUBE_PB_GENUVS,		CurrentTime, PP->mGenUVS,		Inter);

			// Take global scale into account
			PP->mRadius1 *= mSettings->mGlobalScale;
			PP->mRadius2 *= mSettings->mGlobalScale;
			PP->mHeight *= mSettings->mGlobalScale;
			// ### Finish this
		}
	}
	else if(Type==Class_ID(HEDRA_CLASS_ID, 0))
	{
		mArrayLog.StoreASCII("(Hedra)\n");

		// HedraObject
		SimpleObject* SObj = (SimpleObject*)node_object;
		IParamBlock* PB = SObj->pblock;
		if(PB)
		{
			// Create new primitive params
			HedraParams* PP = new HedraParams;
			mesh_desc.mPrimParams = PP;

			// From hedraobj.cpp:
			// Parameter block indices
			#define HEDRA_PB_RADIUS	0
			#define HEDRA_PB_FAMILY	1
			#define HEDRA_PB_P		2
			#define HEDRA_PB_Q		3
			#define HEDRA_PB_SCALEP	4
			#define HEDRA_PB_SCALEQ	5
			#define HEDRA_PB_SCALER	6
			#define HEDRA_PB_VERTS	7
			#define HEDRA_PB_GENUVS	8

			PB->GetValue(HEDRA_PB_RADIUS,	CurrentTime, PP->mRadius,	Inter);
			PB->GetValue(HEDRA_PB_FAMILY,	CurrentTime, PP->mFamily,	Inter);
			PB->GetValue(HEDRA_PB_P,		CurrentTime, PP->mP,		Inter);
			PB->GetValue(HEDRA_PB_Q,		CurrentTime, PP->mQ,		Inter);
			PB->GetValue(HEDRA_PB_SCALEP,	CurrentTime, PP->mScaleP,	Inter);
			PB->GetValue(HEDRA_PB_SCALEQ,	CurrentTime, PP->mScaleQ,	Inter);
			PB->GetValue(HEDRA_PB_SCALER,	CurrentTime, PP->mScaleR,	Inter);
			PB->GetValue(HEDRA_PB_VERTS,	CurrentTime, PP->mVertices,	Inter);
			PB->GetValue(HEDRA_PB_GENUVS,	CurrentTime, PP->mGenUVS,	Inter);

			// Take global scale into account
			PP->mRadius *= mSettings->mGlobalScale;
			// ### Finish this
		}
	}
	else if(Type==SCUBA_CLASS_ID)
	{
		mArrayLog.StoreASCII("(Capsule)\n");

		// Capsule
		SimpleObject* SObj = (SimpleObject*)node_object;
		IParamBlock* PB = SObj->pblock;
		if(PB)
		{
			// Create new primitive params
			CapsuleParams* PP = new CapsuleParams;
			mesh_desc.mPrimParams = PP;

			// From scuba.cpp:
			// Parameter block indices
			#define CAPSULE_PB_RADIUS		0
			#define CAPSULE_PB_HEIGHT		1
			#define CAPSULE_PB_CENTERS		2
			#define CAPSULE_PB_SIDES		3
			#define CAPSULE_PB_HSEGS		4
			#define CAPSULE_PB_SMOOTHON		5
			#define CAPSULE_PB_SLICEON		6
			#define CAPSULE_PB_SLICEFROM	7
			#define CAPSULE_PB_SLICETO		8
			#define CAPSULE_PB_GENUVS		9

			PB->GetValue(CAPSULE_PB_RADIUS,		CurrentTime, PP->mRadius,		Inter);
			PB->GetValue(CAPSULE_PB_HEIGHT,		CurrentTime, PP->mHeight,		Inter);
			PB->GetValue(CAPSULE_PB_CENTERS,	CurrentTime, PP->mCenters,		Inter);
			PB->GetValue(CAPSULE_PB_SIDES,		CurrentTime, PP->mSides,		Inter);
			PB->GetValue(CAPSULE_PB_HSEGS,		CurrentTime, PP->mHSegs,		Inter);
			PB->GetValue(CAPSULE_PB_SMOOTHON,	CurrentTime, PP->mSmooth,		Inter);
			PB->GetValue(CAPSULE_PB_SLICEON,	CurrentTime, PP->mSliceOn,		Inter);
			PB->GetValue(CAPSULE_PB_SLICEFROM,	CurrentTime, PP->mSliceFrom,	Inter);
			PB->GetValue(CAPSULE_PB_SLICETO,	CurrentTime, PP->mSliceTo,		Inter);
			PB->GetValue(CAPSULE_PB_GENUVS,		CurrentTime, PP->mGenUVS,		Inter);

			// Take global scale into account
			PP->mRadius *= mSettings->mGlobalScale;
			PP->mHeight *= mSettings->mGlobalScale;
			// ### Finish this
		}
	}
	else if(Type==Class_ID(TEAPOT_CLASS_ID1,TEAPOT_CLASS_ID2))
	{
		mArrayLog.StoreASCII("(Teapot)\n");

		// Create new primitive params
		TeapotParams* PP = new TeapotParams;
		mesh_desc.mPrimParams = PP;
	}
	else if(Type==Class_ID(GRIDID))
	{
		mArrayLog.StoreASCII("(Plane)\n");

		SimpleObject2* SObj2 = (SimpleObject2*)node_object;
		IParamBlock2* PB = SObj2->pblock2;
		if(PB)
		{
			// Create new primitive params
			PlaneParams* PP = new PlaneParams;
			mesh_desc.mPrimParams = PP;

			int Nb = PB->NumParams();
			PB->GetValue(0,	CurrentTime, PP->mLength,		Inter);
			PB->GetValue(1,	CurrentTime, PP->mWidth,		Inter);
			PB->GetValue(2,	CurrentTime, PP->mWidthSegs,	Inter);
			PB->GetValue(3,	CurrentTime, PP->mLengthSegs,	Inter);
			PB->GetValue(4,	CurrentTime, PP->mDensity,		Inter);
			PB->GetValue(5,	CurrentTime, PP->mScale,		Inter);
			PB->GetValue(6,	CurrentTime, PP->mGenUVS,		Inter);

			// Take global scale into account
			PP->mLength *= mSettings->mGlobalScale;
			PP->mWidth *= mSettings->mGlobalScale;
		}
	}
//	else mArrayLog.StoreASCII("(Unknown ClassID(%d %d))\n", Type.PartA(), Type.PartB());

	return true;
}
