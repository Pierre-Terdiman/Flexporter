///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains various functions asking basic questions to MAX.
 *	I didn't bother putting them in a class, since it seemed quite totally pointless!
 *
 *	\file		FLX_Ask2MAX.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

namespace IM = IceMaths;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define DO_SOME_TESTS

void IceExporter::DoSomeTest()
{
	// ICE TESTS !

#ifdef DO_SOME_TESTS
	// From MAX to ICE to MAX :
	{
		// 1) Setup
		// This is our reference quat
		::Quat	MAXQuat;
		MAXQuat.x = 0.302286f;
		MAXQuat.y = 0.0264466f;
		MAXQuat.z = -0.586633f;
		MAXQuat.w = 0.750856f;
		// This is our reference matrix
		// m[0]
		//		[0]	= 0.310324
		//		[1] = 0.896943
		//		[2] = -0.314947
		// m[1]
		//		[0]	= -0.864965
		//		[1] = 0.128969
		//		[2] = -0.484976
		// m[2]
		//		[0]	= -0.394378
		//		[1] = 0.422918
		//		[2] = 0.815847
		Matrix3	Child;
		MAXQuat.MakeMatrix(Child);
		// Set a translation
		Child.SetTrans(Point3(10.0f, 20.0f, 30.0f));

		// This is our second reference quat
		::Quat	MAXQuat2;
		MAXQuat2.x = 0.811666f;
		MAXQuat2.y = -0.164010f;
		MAXQuat2.z = 0.541339f;
		MAXQuat2.w = 0.145774f;
		// This is our second reference matrix
		// m[0]
		//		[0]	= 0.360105
		//		[1] = -0.424069
		//		[2] = 0.830957
		// m[1]
		//		[0]	= -0.108416
		//		[1] = -0.903701
		//		[2] = -0.414210
		// m[2]
		//		[0]	= 0.926591
		//		[1] = 0.0590697
		//		[2] = -0.371404
		Matrix3	Parent;
		MAXQuat2.MakeMatrix(Parent);
		// Set a translation
		Parent.SetTrans(Point3(40.0f, 50.0f, 60.0f));

		// 2) Simulate the MAX to ICE transform
		// 2-1) in absolute mode
		AffineParts	ChildGlobal;
		decomp_affine(Child, &ChildGlobal);
		// 2-2) in relative mode
		AffineParts	ChildLocal;
		Matrix3 Rel = Child*Inverse(Parent);
		decomp_affine(Rel, &ChildLocal);
Matrix3 Check = Rel * Parent;
		// 2-3) the parent
		AffineParts	ParentLocalGlobal;
		decomp_affine(Parent, &ParentLocalGlobal);

		MAXPRS globalprs, localprs;
		// Fill the result structure
		globalprs.Position.x	= ChildGlobal.t.x;
		globalprs.Position.y	= ChildGlobal.t.y;
		globalprs.Position.z	= ChildGlobal.t.z;

		globalprs.Rotation.x	= ChildGlobal.q.x;
		globalprs.Rotation.y	= ChildGlobal.q.y;
		globalprs.Rotation.z	= ChildGlobal.q.z;
		globalprs.Rotation.w	= ChildGlobal.q.w;

		// Fill the result structure
		localprs.Position.x		= ChildLocal.t.x;
		localprs.Position.y		= ChildLocal.t.y;
		localprs.Position.z		= ChildLocal.t.z;

		localprs.Rotation.x		= ChildLocal.q.x;
		localprs.Rotation.y		= ChildLocal.q.y;
		localprs.Rotation.z		= ChildLocal.q.z;
		localprs.Rotation.w		= ChildLocal.q.w;

		// 3) Get the original matrices back
		// 3-1) in absolute mode
		PR globalpr;
		globalpr.mRot = IM::Quat(globalprs.Rotation.w, Point(globalprs.Rotation.x, globalprs.Rotation.y, globalprs.Rotation.z));
		globalpr.mPos = Point(globalprs.Position.x, globalprs.Position.y, globalprs.Position.z);

		Matrix4x4 World = globalpr.mRot;
		World.Transpose();
		World.SetTrans(globalpr.mPos);
		// Here you must have World == Child matrix above.

		// 3-2) in relative mode
		PR localpr;
		localpr.mRot = IM::Quat(localprs.Rotation.w, Point(localprs.Rotation.x, localprs.Rotation.y, localprs.Rotation.z));
		localpr.mPos = Point(localprs.Position.x, localprs.Position.y, localprs.Position.z);

		Matrix4x4 World2 = localpr.mRot;
		World2.Transpose();
		World2.SetTrans(localpr.mPos);
		// Here we have the local MAX matrix back, so we should just need this:
		Matrix4x4 ICEParent = IM::Quat(ParentLocalGlobal.q.w, ParentLocalGlobal.q.x, ParentLocalGlobal.q.y, ParentLocalGlobal.q.z);
		ICEParent.Transpose();
		ICEParent.SetTrans(Point(ParentLocalGlobal.t.x, ParentLocalGlobal.t.y, ParentLocalGlobal.t.z));

		Matrix4x4 Final = World2 * ICEParent;
		// ...and it should also give the original Child matrix
	}
	//

/*	char* FakeImage = new char[256*256*3];
	char* FakeAlpha = new char[256*256];

	float ColourWeights[3] = { 1.0f, 1.0f, 1.0f };
	float FreqWeights[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	float Error;

	int RamNeeded = VqCalc(FakeImage,
			FakeAlpha,
			NULL, 
			256,
			false,
			false,
			true,
			256,
			FORMAT_4444,
			10,
			false,
			ColourWeights,
			FreqWeights,
			(enum ColourSpaceModeType)RGBspace,
			false,
			0.1f,
			&Error
);


	delete[]FakeAlpha;
	delete[]FakeImage;
*/
	// This is our reference quat
	::Quat	MAXQuat;
	MAXQuat.x = 0.302286f;
	MAXQuat.y = 0.0264466f;
	MAXQuat.z = -0.586633f;
	MAXQuat.w = 0.750856f;
	// This is our reference matrix
	// m[0]
	//		[0]	= 0.310324
	//		[1] = 0.896943
	//		[2] = -0.314947
	// m[1]
	//		[0]	= -0.864965
	//		[1] = 0.128969
	//		[2] = -0.484976
	// m[2]
	//		[0]	= -0.394378
	//		[1] = 0.422918
	//		[2] = 0.815847
	Matrix3	MAXMat;
	MAXQuat.MakeMatrix(MAXMat);

	// Let's compute the D3D matrix thanks to Loïc's code:
	// m[0]
	//		[0]	= 0.310324
	//		[1] = -0.314947
	//		[2] = 0.896943
	// m[1]
	//		[0]	= -0.394377
	//		[1] = 0.815847
	//		[2] = 0.422918
	// m[2]
	//		[0]	= -0.864966
	//		[1] = -0.484976
	//		[2] = 0.128969
	Matrix3	D3DMat = GetD3DMatrix(MAXMat);
	// Let's compute the D3D quat thanks to MAX.
	// Result to check against the reference quat:
	//	qx = -0.302286
	//	qy = 0.586633
	//	qz = -0.0264466
	//	qw = 0.750856
	AffineParts	PRS;
	decomp_affine(MAXMat, &PRS);	// Here you can check you get the original quat back
	decomp_affine(D3DMat, &PRS);
	::Quat	D3DQuat = PRS.q;
	// Hence the transform from MAX Quat to D3D Quat: -x, -z, -y, w

	// END OF PART ONE

	// Ok, we need a second reference quat as well as a second reference matrix.
	// Let's copy-paste the code (indented)

		// This is our second reference quat
		::Quat	MAXQuat2;
		MAXQuat2.x = 0.811666f;
		MAXQuat2.y = -0.164010f;
		MAXQuat2.z = 0.541339f;
		MAXQuat2.w = 0.145774f;
		// This is our second reference matrix
		// m[0]
		//		[0]	= 0.360105
		//		[1] = -0.424069
		//		[2] = 0.830957
		// m[1]
		//		[0]	= -0.108416
		//		[1] = -0.903701
		//		[2] = -0.414210
		// m[2]
		//		[0]	= 0.926591
		//		[1] = 0.0590697
		//		[2] = -0.371404
		Matrix3	MAXMat2;
		MAXQuat2.MakeMatrix(MAXMat2);

		// Let's compute the D3D matrix thanks to Loïc's code:
		// m[0]
		//		[0]	= 0.360105
		//		[1] = 0.830957
		//		[2] = -0.424069
		// m[1]
		//		[0]	= 0.926591
		//		[1] = -0.371403
		//		[2] = 0.0590697
		// m[2]
		//		[0]	= -0.108416
		//		[1] = -0.414210
		//		[2] = -0.903701
		Matrix3	D3DMat2 = GetD3DMatrix(MAXMat2);
		// Let's compute the D3D quat thanks to MAX.
		// Result to check against the second reference quat:
		//	qx = 0.811666
		//	qy = 0.541339
		//	qz = -0.164010
		//	qw = -0.145774
		decomp_affine(MAXMat2, &PRS);	// Here you can check you get the original quat back
		decomp_affine(D3DMat2, &PRS);
		::Quat	D3DQuat2 = PRS.q;
		// Hence the transform from MAX Quat to D3D Quat: x, z, y, -w   (...which is the same as -x, -z, -y, w if you don't know it)

	// Let's have a reference point:
	Point3	MAXPt;
	MAXPt.x = 0.12345f;
	MAXPt.y = 0.6789f;
	MAXPt.z = -0.42f;
	// The transformation matrix is the product of our 2 ref matrices:
	Matrix3	MAXTransMat = MAXMat * MAXMat2;
	// Let's compute the transformed ref point with our transform matrix.
	// Result:
	// x = -0.798868
	// y = 0.101880
	// z = -0.0630556
	Point3	MAXTransformedPt = MAXTransMat * MAXPt;

	// We must now recover the same transformed pt with our transformed data
	Point3	D3DPt;
	Matrix3	D3DTransMat = D3DMat * D3DMat2;
	// Source pt must be transformed (y & z swapped => assumed by Loïc's code)
	// Note: leaving X unchanged and swapping Y and Z has the following consequences:
	//	- the up vector is Z in MAX, Y in D3D.
	//	- MAX right-handed frame becomes a D3D left-handed one.
	D3DPt.x = MAXPt.x;
	D3DPt.y = MAXPt.z;
	D3DPt.z = MAXPt.y;
	Point3	D3DTransformedPt = D3DTransMat * D3DPt;
	// Result:
	// x = -0.798868
	// y = -0.0630556
	// z = 0.101880
	// Hence the result is correct and also swapped.

	// END OF PART TWO

	// Now, we'd like to obtain the same results, working with quaternions.
	// We need to compute a new quaternion issued from the hierarchy. This quaternion will further be transformed into a matrix, then the matrix will be used
	// to transform the point. We already know the final matrix: that's the one we computed thanks to the former D3D matrix product. We just need to check the
	// quaternion we're about to compute casts itself into that same matrix.
	::Quat D3DTransQuat = D3DQuat * D3DQuat2;
	Matrix3 D3DCastedMatrix;
	D3DTransQuat.MakeMatrix(D3DCastedMatrix);
	// To check whether that's the correct result or not, we transform the D3DPt:
	Point3	D3DTransformedPtFromCast = D3DCastedMatrix * D3DPt;
	// Result:
	// x = -0.798868
	// y = -0.0630556
	// z = 0.101880
	// => this is the same result as before, then we can safely use our D3D quats to go down the hierarchy.

	// Conclusion:
	// The pipeline works provided:
	// - our Quat-To-Mat routine performs the same operations as MAX'MakeMatrix
	// - we make our matrix products in the same way as MAX does (vector*matrix or matrix*vector). Else we may transpose the matrices.
	// - we make our quat products in the same way as MAX does (should be)
	// - our global frame match D3D's one: left-handed, Y is up, ground plane is XZ.

	// The transform from right- to left-handed frame is handled by the exporter, when feeding Irion with MAX data:
	// - quats are transformed (x,z,y,-w)
	// - vectors are transformed (x,z,y)
	// Inside Irion, the whole thing must be transparent (no defines, aso.)

	// EXTRA STUFF:
	// what is the effect of a matrix inversion on related quats? Let's check it on our reference matrix.
	Matrix3 MAXInvMat = Inverse(MAXMat);
	decomp_affine(MAXInvMat, &PRS);
	::Quat	MAXInvQuat = PRS.q;
	// Result to check against the reference quat:
	// MAXInvQuat.x = -0.302286f;
	// MAXInvQuat.y = -0.0264466f;
	// MAXInvQuat.z = 0.586633f;
	// MAXInvQuat.w = 0.750856f;
	// => to invert a quat, negate x, y, z and leave w unchanged. (ok for transpose too with rotation matrices)

	// END OF PART THREE

	// Is the Quat-To-Mat routine in Irion the same as in MAX?
	// Let's cast our ref quat and check the result.
	IM::Quat		IQuat(MAXQuat.w, MAXQuat.x, MAXQuat.y, MAXQuat.z);
	IM::Matrix4x4	IMat = (IM::Matrix4x4)IQuat;
	// Result to be checked against the reference matrix:
	// m[0]
	//		[0]	= 0.310324
	//		[1] = -0.864966
	//		[2] = -0.394377
	// m[1]
	//		[0]	= 0.896943
	//		[1] = 0.128969
	//		[2] = 0.422918
	// m[2]
	//		[0]	= 0.314947
	//		[1] = 0.484976
	//		[2] = -0.815847
	// => not ok

	// Is our quat product the same as in MAX?
	IM::Quat	IQuat2(MAXQuat2.w, MAXQuat2.x, MAXQuat2.y, MAXQuat2.z);
	::Quat		MAXTransQuat = MAXQuat * MAXQuat2;
	IM::Quat	ITransQuat = IQuat * IQuat2;
	// MAXTransQuat and ITransQuat must be the same.
	// => ok

	// END OF PART FOUR

	// Now we're gonna test some more exotic stuff. The victim is our reference quat, as always.
	// First, let's test the LogN of a quaternion.
	IM::Quat	ILogNQuat(MAXQuat.w, MAXQuat.x, MAXQuat.y, MAXQuat.z);
	ILogNQuat.LogN();
	// Previous quat must be equal to following one.
	::Quat MAXLogN = LogN(MAXQuat);

	// Now, test the LnDif method with our two reference quats
	IM::Quat LnDifQuat1(MAXQuat.w, MAXQuat.x, MAXQuat.y, MAXQuat.z);
	IM::Quat LnDifQuat2(MAXQuat2.w, MAXQuat2.x, MAXQuat2.y, MAXQuat2.z);
	LnDifQuat1.LnDif(LnDifQuat2);
	//
	::Quat MAXLnDifQuat = LnDif(MAXQuat, MAXQuat2);

	// Now, test the Exp of a quat...
	IM::Quat ExpQuat(MAXQuat.w, MAXQuat.x, MAXQuat.y, MAXQuat.z);
	ExpQuat.Exp();
	//
	::Quat MAXExpQuat = Exp(MAXQuat);

	// ...the division by a quaternion...
	IM::Quat OneOverQuat(1.0f, 0.0f, 0.0f, 0.0f);
	OneOverQuat/=IQuat;
	//
	::Quat MAXOneOverQuat(0.0f, 0.0f, 0.0f, 1.0f);
	MAXOneOverQuat=MAXOneOverQuat/MAXQuat;

	// ...and the squadrev.
	float angle = 0.5678f;				// angle of rotation 
	Point3 axis(0.2f, 0.4f, 0.8f);		// the axis of rotation 
	::Quat p = MAXQuat;					// start quaternion 
	::Quat a(0.1f, 0.2f, 0.3f, 0.4f);	// start tangent quaternion 
	::Quat b(0.4f, 0.3f, 0.2f, 0.1f);	// end tangent quaternion 
	::Quat q = MAXQuat2;				// end quaternion 
	float t=0.42f;						// parameter, in range [0.0,1.0] 
	::Quat MAXSquadRevQuat = squadrev(angle, axis, p, a, b, q, t);
	//
	AngleAxis	aa(0.5678f, 0.2f, 0.4f, 0.8f);
	IM::Quat	q0 = IQuat;
	IM::Quat	q1 = IQuat2;
	IM::Quat	qa(0.4f, 0.1f, 0.2f, 0.3f);
	IM::Quat	qb(0.1f, 0.4f, 0.3f, 0.2f);
	IM::Quat	SquadRevQuat;
	SquadRevQuat.MakeSquadRev(aa, q0, q1, qa, qb, t);

	// END OF PART FIVE
#endif
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks if a given node is a root one.
 *	There's an INode method called IsRootNode(), but this is not what we expect..
 *	\param		node	[in] a MAX node
 *	\return		true for root node
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::IsRootNode(INode* node)
{
	INode* Parent	= node->GetParentNode();		// MAX root node
	INode* pParent	= Parent->GetParentNode();
	return pParent ? false : true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks if a given texmap is exportable.
 *	We only export bitmap-texmaps for the moment.
 *	\param		tmap	[in] a MAX texmap
 *	\return		error code
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ubyte IceExporter::IsKnownTexmap(Texmap* tmap)
{
	if(!tmap) return TEXMAP_UNDEFINED;
	if(tmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))	return TEXMAP_BITMAP;
	return TEXMAP_UNKNOWN;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks a MAX material is a valid one.
 *	\param		material			[in] a MAX material
 *	\param		invalid_materials	[out] a possible container to store invalid material pointers (or null)
 *	\return		true if the material is valid
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::_IsValidMaterial(Mtl* material, Container* invalid_materials)
{
	if(material)
	{
#ifdef FLEXPORTER_MAX5
		if(material->ClassID()==Class_ID(BAKE_SHELL_CLASS_ID, 0))
		{
			// Material is a baked material => valid
		}
		else
#endif
#ifdef FLEXPORTER_MAX6
		#error Adapt me!
#endif
		if(material->ClassID()==Class_ID(DMTL_CLASS_ID, 0))
		{
			// Material is a standard material => valid
		}
		else if(material->ClassID()==Class_ID(MULTI_CLASS_ID, 0))
		{
			// Get number of submaterials
			sdword NbSubMtls = (sdword)material->NumSubMtls();
			// Loop through submaterials
			for(sdword i=0;i<NbSubMtls;i++)
			{
				// Get current submaterial
				Mtl* SubMat = material->GetSubMtl(i);
				// Recurse through it
				if(!_IsValidMaterial(SubMat, invalid_materials))	return false;
			}
		}
		// ClassID is unknown
		else
		{
			// Keep track of wrong materials if needed
			if(invalid_materials)	invalid_materials->AddUnique(udword(material));
			return false;
		}
	}
	// No material defined
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks a node is mirrored or not.
 *	\param		node		[in] a MAX node
 *	\param		time_value	[in] a time value
 *	\return		true if mirrored
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::IsMirrored(INode* node, long time_value)
{
	// Checkings
	if(!node)	return false;

	Matrix3 SegTransform = node->GetNodeTM(time_value);
	INode* parent = node->GetParentNode();
	if(parent && !parent->IsRootNode()) 
		SegTransform = SegTransform * Inverse(parent->GetNodeTM(time_value));
	Matrix3 LocalToWorld = Inverse(SegTransform);

	Point3 a = SegTransform.GetRow(0);
	Point3 b = SegTransform.GetRow(1);
	Point3 c = SegTransform.GetRow(2);

	Point3 cs = CrossProd(a,b);
	if(DotProd(cs, c)<0.0f)	return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets the total number of nodes in a MAX scene, including root node.
 *	\param		current_node	[in] MAX root node
 *	\return		total number of nodes
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword IceExporter::GetNbNodes(INode* current_node)
{
	struct Local
	{
		static void CountNbNodes(INode* current_node, udword& count)
		{
			// Update count...
			count++;
			// ...and recurse through children
			for(udword i=0;i<(udword)current_node->NumberOfChildren();i++) CountNbNodes(current_node->GetChildNode(i), count);
		}
	};

	udword Count=0;
	Local::CountNbNodes(current_node, Count);
	return(Count);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Finds a node given its name.
 *	\param		current_node	[in] MAX root node
 *	\param		name			[in] name of the node we're looking for
 *	\return		node of given name, or null if not found
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
INode* IceExporter::_FindNode(INode* current_node, const char* name)
{
	if(name && current_node && current_node->GetName())
	{
		if(strcmp(current_node->GetName(), name)==0)	return current_node;

		for(udword i=0;i<current_node->NumberOfChildren();i++)
		{
			INode* found = _FindNode(current_node->GetChildNode(i), name);
			if(found)	return found;
		}
	}
	return null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets a tri-mesh from a MAX object.
 *	\param		nodeobject		[in] a MAX object
 *	\param		time			[in] current time value
 *	\return		the trimesh
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef OLDIES
::Mesh* IceExporter::GetMeshFromObject(Object* nodeobject, TimeValue time)
{
	TriObject* To = nodeobject->ClassID()==triObjectClassID ? (TriObject*)nodeobject : (TriObject*)nodeobject->ConvertToType(time, triObjectClassID);
	return To ? &To->mesh : null;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Catches some time-related infos
 *	\param		ip			[in] MAX interface
 *	\param		result		[out] the result structure
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IceExporter::GetTimeInfo(Interface* ip, MAXTimeInfo& result)
{
	// Get valid interval
	Interval Range = ip->GetAnimRange();

	// Get time related values. Ex: say your MAX anim begins to frame 0 and ends to frame 100. You'll get those typical values:
	// StartTime	= 0
	// EndTime		= 16000
	// DeltaTime	= 160
	// FrameRate	= 30	=> mDeltaTM in ICE is 1.0f / mFrameRate
	result.mStartTime	= (long)Range.Start();
	result.mEndTime		= (long)Range.End();
	result.mDeltaTime	= (long)GetTicksPerFrame();
	result.mFrameRate	= (long)GetFrameRate();
	result.mCurrentTime	= ip->GetTime();		// Get current frame number used for static export
	result.mNbFrames	= 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks whether a node has counter-clockwise faces or not.
 *	Looks like that's the same as checking for negative scales...
 *	\param		node			[in] a MAX node pointer
 *	\param		current_time	[in] the current frame
 *	\return		false for standard meshes, true for mirrored ones.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::GetParity(INode* node, TimeValue current_time)
{
	Interval Inter;			// The whole time interval is valid
	Inter.SetInfinite();

	Matrix3 RelTM;
	Matrix3 CurMat = node->GetObjectTM(current_time, &Inter);
	Matrix3 ParMat = node->GetParentNode()->GetObjectTM(current_time, &Inter);

	RelTM = CurMat * Inverse(ParMat);

	return RelTM.Parity()!=0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets the crop values for a given texmap.
 *	\param		tmap			[in] current texmap
 *	\param		crop			[out] a result structure filled with crop values
 *	\param		uvtrans			[out] the texture matrix
 *	\param		current_time	[in] current MAX time
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::GetCropValues(Texmap* tmap, TextureCrop& crop, Matrix3& uvtrans, TimeValue current_time)
{
	// 1) Init result structure with default values
	crop.Reset();

	// 2) Check Texmap type
	char Status	= IsKnownTexmap(tmap);
	if(Status!=TEXMAP_BITMAP)	return false;

	// 3) Get crop values
	BitmapTex*	BTex	= (BitmapTex*)tmap;
#ifdef FLEXPORTER_MAX2
	IParamBlock* pUVWCropParam = (IParamBlock*)(BTex->GetReference(1));			// for MAX 2
#else
	IParamBlock2* pUVWCropParam = (IParamBlock2*)(BTex->GetReference(1));
#endif
	if(pUVWCropParam)
	{
		pUVWCropParam->GetValue(0, current_time, crop.mOffsetU, FOREVER);
		pUVWCropParam->GetValue(1, current_time, crop.mOffsetV, FOREVER);
		pUVWCropParam->GetValue(2, current_time, crop.mScaleU,  FOREVER);
		pUVWCropParam->GetValue(3, current_time, crop.mScaleV,  FOREVER);
	}

	// 4) While we are at it, get the texture matrix as well
	BTex->GetUVTransform(uvtrans);

	return true;
}

bool IceExporter::GetTextureTiling(Texmap* tmap, udword& tiling_u, udword& tiling_v)
{
	// 1) Check Texmap type
	char Status	= IsKnownTexmap(tmap);
	if(Status!=TEXMAP_BITMAP)	return false;

	// 2) Get tiling
	BitmapTex*	BTex	= (BitmapTex*)tmap;

	tiling_u = 0;
	tiling_v = 0;

	int Tiling = BTex->GetTextureTiling();
	if(Tiling & U_WRAP)		tiling_u |= TILING_WRAP;
	if(Tiling & U_MIRROR)	tiling_u |= TILING_MIRROR;
	if(Tiling & V_WRAP)		tiling_v |= TILING_WRAP;
	if(Tiling & V_MIRROR)	tiling_v |= TILING_MIRROR;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets the offset matrix from a node.
 *	\param		node	[in] MAX node
 *	\return		MAX offset matrix
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Matrix3 IceExporter::GetOffsetMatrix(INode* node)
{
	// From the SDK
	// Constructing an offset TM for a node is done with the following code:

	Matrix3 tm(1);
	Point3 pos = node->GetObjOffsetPos();
	tm.PreTranslate(pos);
	::Quat quat = node->GetObjOffsetRot();
	PreRotateMatrix(tm, quat);
	ScaleValue scaleValue = node->GetObjOffsetScale();
	ApplyScaling(tm, scaleValue);

	return tm;
}

bool IceExporter::AreEqual(void* text0, void* text1)
{
	if(!text0 && !text1)	return true;
	if(text0 && !text1)		return false;
	if(!text0 && text1)		return false;

	udword Length0 = strlen((const char*)text0);
	udword Length1 = strlen((const char*)text1);
	if(Length0!=Length1)	return false;

	ubyte* t0 = (ubyte*)text0;
	ubyte* t1 = (ubyte*)text1;

	for(udword i=0;i<Length0;i++)
	{
		if(t0[i]!=t1[i])	return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets units data.
 *	\param		units	[out] current units
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::GetUnits(MAXUnits& units)
{
	// From the MAX SDK:
	//
	// 3ds max allows users to work with units that they are most comfortable with. Users may specify that they wish to
	// work in US Standard units (Inches, Feet, Miles) or Metric units (Millimeters, Centimeters, Meters or Kilometers).
	// Or they may specify a custom setting (for example perhaps they want to work in fathoms). Users may also work with
	// generic units (these are the same as the default 3ds max master units).
	//
	// Internally 3ds max keeps track of all measurements in its own internal unit called the master unit. The program
	// converts measurements from user-defined units into the master defined unit for storage and computation. The master
	// unit is defined as 1.0 inch. The user is able to change this master unit using Customize/Preferences, selecting
	// the General page, and adjusting the System Unit Scale controls.
	//
	// To retrieve the current unit display information one can use GetUnitDisplayInfo(). This returns the information
	// describing MAX's current system of measurement. This includes the type of units used, how they are displayed and
	// custom unit name and scale. A developer may also use SetUnitDisplayInfo(). This allows the developer to control
	// the unit settings.
	//
	// Two functions are available for converting units in and out of ASCII form. These are FormatUniverseValue() and
	// DecodeUniverseValue(). FormatUniverseValue() converts the specified value to its ASCII representation using the
	// current unit display settings.  DecodeUniverseValue() parses the specified measurement string and converts it to
	// a floating point value.
	//
	// Developers can use the method GetMasterScale(int type) to retrieve the master scale in terms of the specified unit
	// type. For example: GetMasterScale(UNITS_INCHES) returns the number of inches per unit (unless altered by the user
	// this would be 1.0).

	struct Local
	{
		// Better safe than...
		static void MAXtoFLX(int sourcetype, DisplayType& desttype)
		{
					if(sourcetype==UNITDISP_GENERIC)	desttype = FLX_UNITDISPLAY_GENERIC;
			else	if(sourcetype==UNITDISP_METRIC)		desttype = FLX_UNITDISPLAY_METRIC;
			else	if(sourcetype==UNITDISP_US)			desttype = FLX_UNITDISPLAY_US;
			else	if(sourcetype==UNITDISP_CUSTOM)		desttype = FLX_UNITDISPLAY_CUSTOM;
			else										desttype = FLX_UNITDISPLAY_FORCE_DWORD;
		}
		static void MAXtoFLX(int sourcetype, UnitType& desttype)
		{
					if(sourcetype==UNITS_INCHES)		desttype = FLX_UNIT_INCHES;
			else	if(sourcetype==UNITS_FEET)			desttype = FLX_UNIT_FEET;
			else	if(sourcetype==UNITS_MILES)			desttype = FLX_UNIT_MILES;
			else	if(sourcetype==UNITS_MILLIMETERS)	desttype = FLX_UNIT_MILLIMETERS;
			else	if(sourcetype==UNITS_CENTIMETERS)	desttype = FLX_UNIT_CENTIMETERS;
			else	if(sourcetype==UNITS_METERS)		desttype = FLX_UNIT_METERS;
			else	if(sourcetype==UNITS_KILOMETERS)	desttype = FLX_UNIT_KILOMETERS;
			else										desttype = FLX_UNIT_FORCE_DWORD;
		}
	};

	// Get the master scale in terms of the specified unit type
	// e.g. GetMasterScale(UNITS_INCHES) gives number of inches per unit
	// Returns -1.0 if invalid unit type supplied
	// Supported unit types:
	// UNITS_INCHES
	// UNITS_FEET
	// UNITS_MILES
	// UNITS_MILLIMETERS
	// UNITS_CENTIMETERS
	// UNITS_METERS
	// UNITS_KILOMETERS
	// Return Value:
	// The master scale in terms of the specified unit type.
	units.mMasterScaleInches		= GetMasterScale(UNITS_INCHES);
	units.mMasterScaleFeet			= GetMasterScale(UNITS_FEET);
	units.mMasterScaleMiles			= GetMasterScale(UNITS_MILES);
	units.mMasterScaleMillimeters	= GetMasterScale(UNITS_MILLIMETERS);
	units.mMasterScaleCentimeters	= GetMasterScale(UNITS_CENTIMETERS);
	units.mMasterScaleMeters		= GetMasterScale(UNITS_METERS);
	units.mMasterScaleKilometers	= GetMasterScale(UNITS_KILOMETERS);

	// Retrieves the master unit settings in effect.  These are the unit type (Inches, Feet, Meters, etc.) and the master scale setting.
	// 	int *type
	// 	Retrieves the unit type in effect.  This may be one of the following values:
	// 	Supported unit types:
	// 	UNITS_INCHES
	// 	UNITS_FEET
	// 	UNITS_MILES
	// 	UNITS_MILLIMETERS
	// 	UNITS_CENTIMETERS
	// 	UNITS_METERS
	// 	UNITS_KILOMETERS
	// 	float *scale
	// 	The master scale setting.  This is the value the user entered in the
	// 	1 Unit = XXX field of the General Page in the Preference Settings dialog box.
	int Type;
	GetMasterUnitInfo(&Type, &units.mScale);
		Local::MAXtoFLX(Type, units.mType);

	// Retrieves the current unit display information.
	// DispInfo *info
	// Points to storage for the display information. See Structure DispInfo.
	DispInfo di;
	GetUnitDisplayInfo(&di);
		// Structure translation (safer than the direct copy..)
		Local::MAXtoFLX(di.dispType, units.mDispInfo.mDispType);
		units.mDispInfo.mMetricDisp		= di.metricDisp;
		units.mDispInfo.mUSDisp			= di.usDisp;
		units.mDispInfo.mUSFrac			= di.usFrac;
		units.mDispInfo.mCustomName		= di.customName;
		units.mDispInfo.mCustomValue	= di.customValue;
		units.mDispInfo.mCustomUnit		= di.customUnit;

	//Returns the current unit display type.
	// Return Value:
	// One of the following values:
	// UNITDISP_GENERIC
	// UNITDISP_METRIC
	// UNITDISP_US
	// UNITDISP_CUSTOM
	Local::MAXtoFLX(GetUnitDisplayType(), units.mDType);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Dumps MAX selection in a dedicated array.
 *	\param		ip		[in] MAX interface
 *	\param		nodes	[out] array of selected nodes
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::GetSelectedNodes(Interface* ip, MAXNodes& nodes)
{
	if(!ip)	return false;
	for(udword i=0;i<ip->GetSelNodeCount();i++)	nodes.AddNode(ip->GetSelNode(i));
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets the bitmap array from a MAX node.
 *	\param		node			[in] MAX node
 *	\param		texture_id		[in] id of desired texture (diffuse, etc)
 *	\param		current_time	[in] current time for animated textures
 *	\return		bitmap storage pointer, or null
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BitmapStorage* IceExporter::GetBitmapStorage(INode* node, MAXTextureID texture_id, TimeValue current_time)
{
	// Checkings
	if(!node)	return null;

	Mtl* material = node->GetMtl();
	if(!material)	return null;

	if(material->ClassID()==Class_ID(DMTL_CLASS_ID, 0))
	{
		StdMat* StandardMaterial = (StdMat*)material;

		Texmap* TMap = material->GetSubTexmap(texture_id);

		if(TMap && TMap->ClassID()==Class_ID(BMTEX_CLASS_ID, 0x00))
		{
			BitmapTex* RGBBitmapTex = (BitmapTex*)TMap;

			Bitmap* RGBTexData = RGBBitmapTex->GetBitmap(current_time);

			if(RGBTexData)	return RGBTexData->Storage();
		}
	}

	return null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Fills a MAX bitmap array with a given color.
 *	\param		bitmap_storage	[out] MAX bitmap storage pointer
 *	\param		red				[in] 16-bits red component
 *	\param		green			[in] 16-bits green component
 *	\param		blue			[in] 16-bits blue component
 *	\param		alpha			[in] 16-bits alpha component
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::FillTexture(BitmapStorage* bitmap_storage, uword red, uword green, uword blue, uword alpha)
{
	if(!bitmap_storage)	return false;

	udword Width = bitmap_storage->Width();
	udword Height = bitmap_storage->Height();

	if(!Width || !Height)	return false;

	BMM_Color_64* Line = new BMM_Color_64[Width];
	CHECKALLOC(Line);

	for(udword i=0;i<Width;i++)
	{
		Line[i].r = red;
		Line[i].g = green;
		Line[i].b = blue;
		Line[i].a = alpha;
	}

	for(udword i=0;i<Height;i++)
	{
		int Status = bitmap_storage->PutPixels(0, i, Width, Line);
	}

	DELETEARRAY(Line);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Fills a MAX bitmap array with a given array of colors.
 *	\param		bitmap_storage	[out] MAX bitmap storage pointer
 *	\param		pixels			[in] source picture
 *	\param		alpha			[in] alpha component for all pixels
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::CopyToTexture(BitmapStorage* bitmap_storage, const Point* pixels, uword alpha)
{
	if(!bitmap_storage || !pixels)	return false;

	udword Width = bitmap_storage->Width();
	udword Height = bitmap_storage->Height();

	if(!Width || !Height)	return false;

	BMM_Color_64* Line = new BMM_Color_64[Width];
	CHECKALLOC(Line);

	for(udword y=0;y<Height;y++)
	{
		for(udword x=0;x<Width;x++)
		{
			VectorToColor(*pixels++, Line[x]);
			Line[x].a = alpha;
		}
		int Status = bitmap_storage->PutPixels(0, y, Width, Line);
	}

	DELETEARRAY(Line);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Copies a MAX native prs to our own prs.
 *	\param		prs				[out] destination structure
 *	\param		affine_parts	[in] source MAX structure
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IceExporter::CopyPRS(PRS& prs, const AffineParts& affine_parts)
{
	prs.mPos.x		= affine_parts.t.x;
	prs.mPos.y		= affine_parts.t.y;
	prs.mPos.z		= affine_parts.t.z;

	prs.mRot.p.x	= affine_parts.q.x;
	prs.mRot.p.y	= affine_parts.q.y;
	prs.mRot.p.z	= affine_parts.q.z;
	prs.mRot.w		= affine_parts.q.w;

	prs.mScale.x	= affine_parts.f * affine_parts.k.x;
	prs.mScale.y	= affine_parts.f * affine_parts.k.y;
	prs.mScale.z	= affine_parts.f * affine_parts.k.z;
}

void IceExporter::GetScaling(Point& scaling, INode* node, TimeValue frame, bool from_node)
{
	Interval Inter=FOREVER;		// Validity interval

	Matrix3 ObjTM;
	if(from_node)	ObjTM = node->GetNodeTM(frame, &Inter);	// And not GetObjectTM, else Damien's Battoo is whacked. (and the CS Guy as well)
	else			ObjTM = node->GetObjectTM(frame, &Inter);
/*
//	ObjTM = node->GetNodeTM(frame, &Inter);	// And not GetObjectTM, else Damien's Battoo is whacked. (and the CS Guy as well)

	if(BT==BONE_BIPED_MAIN || BT==BONE_BIPED_SLAVE || BT==BONE_BIPED_FOOT || BT==BONE_BIPED_DUMMY)
	{
//			ObjTM = node->GetNodeTM(frame, &Inter);	// And not GetObjectTM, else Damien's Battoo is whacked. (and the CS Guy as well)
		ObjTM = node->GetObjectTM(frame, &Inter);
	}
	else
	{
		ObjTM = node->GetObjectTM(Frame, &Inter);
	}
*/
	AffineParts	StaticPRS;
	decomp_affine(ObjTM, &StaticPRS);
	scaling.x = StaticPRS.k.x * StaticPRS.f;
	scaling.y = StaticPRS.k.y * StaticPRS.f;
	scaling.z = StaticPRS.k.z * StaticPRS.f;
}

bool IceExporter::HasCustomMaterial(INode* node, const char* class_name, int* index)
{
	// Checkings
	if(!node || !class_name)	return false;

	// Catch the node's material pointer
	Mtl* Material = node->GetMtl();
	if(!Material)	return false;

	// Catch the container of custom attributes
	ICustAttribContainer* cc = Material->GetCustAttribContainer();
	if(!cc)	return false;

	// Walk through custom attributes and look for desired one
	int Nb = cc->GetNumCustAttribs();
	for(int i=0;i<Nb;i++)
	{
		// Catch current custom attribute
		CustAttrib* ca = cc->GetCustAttrib(i);
		if(!ca)	continue;

		if(ca->GetParamBlock(0))
		{
			ParamBlockDesc2* Desc = ca->GetParamBlock(0)->GetDesc();
			if(Desc)
			{
				if(strcmp(Desc->cd->ClassName(), class_name)==0)
				{
					if(index)	*index = i;
					return true;
				}
			}
		}
	}
	return false;
}

Modifier* IceExporter::FindModifier(INode* node, Class_ID class_id)
{
	// Checkings
	if(!node)	return null;

	// Get object from node. Abort if no object.
	Object* ObjectPtr = node->GetObjectRef();
	if(!ObjectPtr) return null;

	// Is derived object?
	while(ObjectPtr->SuperClassID()==GEN_DERIVOB_CLASS_ID)
	{
		// Yes => cast
		IDerivedObject* DOP = (IDerivedObject*)ObjectPtr;

		// Iterate over all entries of the modifier stack
		int ModStackIndex = 0;
		while(ModStackIndex<DOP->NumModifiers())
		{
			// Get current modifier
			Modifier* Mody = DOP->GetModifier(ModStackIndex++);

			// Look for desired modifier
			if(Mody->ClassID() == class_id)
			{
				// Yes => exit
				return Mody;
			}
		}
		ObjectPtr = DOP->GetObjRef();
	}
	// Not found
	return null;
}

bool IceExporter::DeleteModifier(INode* node, Class_ID class_id)
{
	// Checkings
	if(!node)	return false;

	// Get object from node. Abort if no object.
	Object* ObjectPtr = node->GetObjectRef();
	if(!ObjectPtr) return false;

	// Is derived object?
	while(ObjectPtr->SuperClassID()==GEN_DERIVOB_CLASS_ID)
	{
		// Yes => cast
		IDerivedObject* DOP = (IDerivedObject*)ObjectPtr;

		// Iterate over all entries of the modifier stack
		int ModStackIndex = 0;
		while(ModStackIndex<DOP->NumModifiers())
		{
			// Get current modifier
			Modifier* Mody = DOP->GetModifier(ModStackIndex);

			// Look for desired modifier
			if(Mody->ClassID() == class_id)
			{
				// Yes => delete it
				DOP->DeleteModifier(ModStackIndex);
				return true;
			}

			ModStackIndex++;
		}
		ObjectPtr = DOP->GetObjRef();
	}
	// Not found
	return false;
}
