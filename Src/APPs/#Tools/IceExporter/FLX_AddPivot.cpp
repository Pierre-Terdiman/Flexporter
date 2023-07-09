///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export the pivot.
 *	\file		FLX_AddPivot.cpp
 *	\author		Pierre Terdiman
 *	\date		February, 13, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Converts the pivot.
 *	\param		node	[in] the MAX node
 *	\param		pivot	[out] pivot information
 *	\return		Self-Reference
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXConverter& MAXConverter::GetPivot(INode* node, Pivot& pivot)
{
	// Checkings
	if(!node)	return *this;

#ifdef OLDIES
	if(0)
	{
		{
			Matrix3 OffsetTransform = GetOffsetMatrix(node);
			AffineParts AP;
			decomp_affine(OffsetTransform, &AP);

			if(AP.f<0.0f)	mArrayLog.StoreASCII("WARNING: negative pivot scale\n");
		}

		// Catch pivot PRS
		Point3 PPos = node->GetObjOffsetPos();
		::Quat PRot = node->GetObjOffsetRot();
		ScaleValue PScl = node->GetObjOffsetScale();

		// Dump useful data
		local_pivot.Position.x = PPos.x;
		local_pivot.Position.y = PPos.y;
		local_pivot.Position.z = PPos.z;

		local_pivot.Rotation.x = PRot.x;
		local_pivot.Rotation.y = PRot.y;
		local_pivot.Rotation.z = PRot.z;
		local_pivot.Rotation.w = PRot.w;

		local_pivot.Scale.x = PScl.s.x;
		local_pivot.Scale.y = PScl.s.y;
		local_pivot.Scale.z = PScl.s.z;
	}
#endif

	// 1.15
	if(1)
	{
		AffineParts StaticPRS;
		Interval	Inter	= FOREVER;	// MAX validity interval

		Matrix3		CurMat	= node->GetNodeTM(0, &Inter);
		Matrix3		ParMat	= node->GetParentNode()->GetNodeTM(0, &Inter);
		Matrix3		RelTM	= CurMat * Inverse(ParMat);

		// Global data
		{
			// Here we need a global matrix
//			if(remove_non_uniform_scaling)	RelTM = GetUniformMatrix(CurMat);
//			else

			decomp_affine(CurMat, &StaticPRS);
			CopyPRS((PRS&)pivot.mWorldPivot, StaticPRS);

			decomp_affine(ParMat, &StaticPRS);
			CopyPRS((PRS&)pivot.mParentWorldPivot, StaticPRS);
		}

		// Local data
		{
			// Here we need a local matrix
//			if(remove_non_uniform_scaling)	RelTM = GetUniformMatrix(CurMat) * Inverse(GetUniformMatrix(ParMat));
//			else

			decomp_affine(RelTM, &StaticPRS);
			CopyPRS((PRS&)pivot.mLocalPivot, StaticPRS);
		}

		// Test
//		float CurAngles[3];	MatrixToEuler(CurMat, CurAngles, EULERTYPE_XYZ|EULERTYPE_RF);
//		float ParAngles[3];	MatrixToEuler(ParMat, ParAngles, EULERTYPE_XYZ|EULERTYPE_RF);
//		float RelAngles[3];	MatrixToEuler(RelTM, RelAngles, EULERTYPE_XYZ|EULERTYPE_RF);
	}





/*	// Convert to D3D if needed
	if(mSettings->mConvertToD3D)
	{
		Swap(IR(pivot.Position.y), IR(pivot.Position.z));
		Swap(IR(pivot.Rotation.y), IR(pivot.Rotation.z));
		Swap(IR(pivot.Scale.y), IR(pivot.Scale.z));
	}

	// User-defined frame conversion
	if(mExporter)
	{
		mExporter->ConvertPoint(pivot.Position.x, pivot.Position.y, pivot.Position.z);
		mExporter->ConvertQuat(pivot.Rotation.x, pivot.Rotation.y, pivot.Rotation.z, pivot.Rotation.w);
		mExporter->ConvertPoint(pivot.Scale.x, pivot.Scale.y, pivot.Scale.z);
	}
*/
	ConvertPRS((PRS&)pivot.mLocalPivot);
	ConvertPRS((PRS&)pivot.mWorldPivot);
	ConvertPRS((PRS&)pivot.mParentWorldPivot);

/*
	Point3 P_Pos = GetPivotOffset(node);
	if(!node->GetParentNode()->IsRootNode())
	{
		if(!(node->GetParentNode()->GetObjOffsetRot() == IdentQuat()))
		{
			Quat q;
			AddParentsOffset(node, P_Pos, q);
		}
	}

	if ( PRS ) {						// Pequeña duda en el pivote
		PRS->pivot.x = P_Pos.x;
		PRS->pivot.y = P_Pos.z;
		PRS->pivot.z = P_Pos.y;
//		if ( bLogFile ) fprintf(LogFile,"\n\tFuckPivot[ %f %f %f ]\n",PRS->pivot.x,PRS->pivot.y,PRS->pivot.z );
	}
*/
	return *this;
}

/*

// gdf remove the rotational and scale comonents from the max pivot
// to get the pivot offset for 3ds
Point3 ProExp::GetPivotOffset(INode* node)
{
 // get the pivot offset and remove the rotational/scale component
    Matrix3 mat(TRUE);
    Quat qRot = node->GetObjOffsetRot();
    qRot.MakeMatrix(mat);
 // max scales the object if the pivot is scaled so skip.
 // m.SetScale( ((SceneEntry *)data)->node->GetObjOffsetScale().s );
    Point3 pivot = -node->GetObjOffsetPos();
    mat = Inverse(mat);
    Point3 pOff = VectorTransform(mat, pivot);
    return pOff;
}

// gdf used for reducing keys
inline BOOL
ApproxEqual(float a, float b)
{
    float eps = float(1.0e-5);
    float d = (float) fabs(a-b);
    return d < eps;
}

// gdf see if we can write TCB keys instead of generating keys by
// sampling frames.
static BOOL
IsTCBControl(Control *cont)
{
	return (cont->ClassID()==Class_ID(TCBINTERP_POSITION_CLASS_ID,0) ||
		    cont->ClassID()==Class_ID(TCBINTERP_ROTATION_CLASS_ID,0) ||
    		cont->ClassID()==Class_ID(TCBINTERP_SCALE_CLASS_ID,0));
}

// gdf Get the tranform matrix that take a point from its local coordinate
// system to it's parent's coordinate system
static Matrix3
GetLocalNodeTM(INode* node, TimeValue t)
{
    Matrix3 tm(TRUE);
    if (node) {
        tm = node->GetNodeTM(t);
        if (!node->GetParentNode()->IsRootNode()) {
            Matrix3 ip = Inverse(node->GetParentNode()->GetNodeTM(t));
            tm = tm * ip;
        }
    }
    return tm;
}

// gdf remove the offset effects of the parent from the child.
static BOOL UndoParentsOffset(INode* node, Point3& pt, Quat& rOff)
{
    INode* parent;
    Matrix3 mat(TRUE);
    Quat pOff;
    if (node && pt && rOff) {
        if (!node->GetParentNode()->IsRootNode()) {
            parent = node->GetParentNode();
            pOff   = parent->GetObjOffsetRot();
            if (pOff == IdentQuat()) return FALSE;
            pOff.MakeMatrix(mat);
            mat   = Inverse(mat);
            pt    = VectorTransform(mat, pt);
            rOff  = rOff/pOff;
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL AddParentsOffset(INode* node, Point3& pt, Quat& rOff)
{
    INode* parent;
    Matrix3 mat(TRUE);
    Quat pOff;
    if (node && pt && rOff) {
        if (!node->GetParentNode()->IsRootNode()) {
            parent = node->GetParentNode();
            pOff   = parent->GetObjOffsetRot();
            if (pOff == IdentQuat()) return FALSE;
            pOff.MakeMatrix(mat);
            mat   = Inverse(mat);
            pt    = VectorTransform(mat, pt);
            rOff  = rOff/ Inverse(pOff);
            return TRUE;
        }
    }
    return FALSE;
}

*/
