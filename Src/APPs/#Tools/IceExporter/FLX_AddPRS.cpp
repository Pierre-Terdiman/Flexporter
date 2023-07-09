///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a PRS.
 *	\file		FLX_AddPRS.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

/*
// Get/Set node's transform ( without object-offset or WSM affect)
virtual Matrix3	GetNodeTM(TimeValue t, Interval* valid=NULL)=0;
		
// Get object's transform (including object-offset)
// and also the WSM affect when appropriate )
// This is used inside object Display and HitTest routines
virtual Matrix3 GetObjectTM(TimeValue time, Interval* valid=NULL)=0;

// Get object's transform including object-offset but not WSM affect
virtual Matrix3 GetObjTMBeforeWSM(TimeValue time, Interval* valid=NULL)=0;

// Get object's transform including object-offset and WSM affect
virtual Matrix3 GetObjTMAfterWSM(TimeValue time, Interval* valid=NULL)=0;

// evaluate the State the object after offset and WSM's applied		
// if evalHidden is FALSE and the node is hidden the pipeline will not
// actually be evaluated (however the TM will).
virtual	const ObjectState& EvalWorldState(TimeValue time,BOOL evalHidden=TRUE)=0;	

// Object offset from node:
virtual	Point3 	GetObjOffsetPos()=0;
virtual	Quat 	GetObjOffsetRot()=0;
virtual	ScaleValue GetObjOffsetScale()=0;
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets a PRS
 *	\param		node						[in] the MAX node
 *	\param		time_value					[in] the frame number
 *	\param		remove_non_uniform_scaling	[in] true to remove non-uniform scaling
 *	\param		static_prs					[out] the result structure
 *	\return		Self-Reference
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXConverter& MAXConverter::GetStaticPRS(INode* node, long time_value, bool remove_non_uniform_scaling, AffineParts& static_prs)
{
	Interval	Inter	= FOREVER;	// MAX validity interval

	Matrix3		RelTM;
	Matrix3		CurMat	= node->GetObjectTM(time_value, &Inter);
	Matrix3		ParMat	= node->GetParentNode()->GetObjectTM(time_value, &Inter);
//	Matrix3		CurMat	= node->GetNodeTM(time_value, &Inter);
//	Matrix3		ParMat	= node->GetParentNode()->GetNodeTM(time_value, &Inter);

	if(mSettings->mAbsolutePRS)
	{
		// Here we need a global matrix
		if(remove_non_uniform_scaling)	RelTM = GetUniformMatrix(CurMat);
		else							RelTM = CurMat;
	}
	else
	{
		// Here we need a local matrix
		if(remove_non_uniform_scaling)	RelTM = GetUniformMatrix(CurMat) * Inverse(GetUniformMatrix(ParMat));
		else							RelTM = CurMat * Inverse(ParMat);
	}

#ifdef TEST_PRS
	if(0)
	{
		if(IsMirrored(node, time_value))
		{
		//	prs.Rotation.w = -prs.Rotation.w;
			Point3 R0 = RelTM.GetRow(0);
			Point3 R1 = RelTM.GetRow(1);
			Point3 R2 = RelTM.GetRow(2);

			Matrix3x3 Diag;
			Diag.m[0][0] = R0.x;
			Diag.m[0][1] = R0.y;
			Diag.m[0][2] = R0.z;

			Diag.m[1][0] = R1.x;
			Diag.m[1][1] = R1.y;
			Diag.m[1][2] = R1.z;

			Diag.m[2][0] = R2.x;
			Diag.m[2][1] = R2.y;
			Diag.m[2][2] = R2.z;
	Diag.Transpose();
			Matrix3x3 vout;
			Point dout;
			int np = Eigen(vout, dout, Diag);

			if(1)
			{
				if(dout.x>0.0f)	mScaling.x*=-1.0f;
				if(dout.y>0.0f)	mScaling.y*=-1.0f;
				if(dout.z>0.0f)	mScaling.z*=-1.0f;
		//		mScaling = -dout;
		//mScaling.Normalize();
			}

			mArrayLog
				.StoreASCII("Mat: ")
				.StoreASCII(Diag.m[0][0]).StoreASCII("  ").StoreASCII(Diag.m[0][1]).StoreASCII("  ").StoreASCII(Diag.m[0][2]).StoreASCII("\n")
				.StoreASCII(Diag.m[1][0]).StoreASCII("  ").StoreASCII(Diag.m[1][1]).StoreASCII("  ").StoreASCII(Diag.m[1][2]).StoreASCII("\n")
				.StoreASCII(Diag.m[2][0]).StoreASCII("  ").StoreASCII(Diag.m[2][1]).StoreASCII("  ").StoreASCII(Diag.m[2][2]).StoreASCII("\n")
				.StoreASCII("\n");

			mArrayLog
				.StoreASCII("Det: ")
				.StoreASCII(Diag.Determinant())
				.StoreASCII("\n\n");

			mArrayLog
				.StoreASCII("Eigenvalues: ")
				.StoreASCII(dout.x)
				.StoreASCII("  ")
				.StoreASCII(dout.y)
				.StoreASCII("  ")
				.StoreASCII(dout.z)
				.StoreASCII("\n\n");

			mArrayLog
				.StoreASCII("Eigenvectors: ")
				.StoreASCII(vout.m[0][0]).StoreASCII("  ").StoreASCII(vout.m[0][1]).StoreASCII("  ").StoreASCII(vout.m[0][2]).StoreASCII("\n")
				.StoreASCII(vout.m[1][0]).StoreASCII("  ").StoreASCII(vout.m[1][1]).StoreASCII("  ").StoreASCII(vout.m[1][2]).StoreASCII("\n")
				.StoreASCII(vout.m[2][0]).StoreASCII("  ").StoreASCII(vout.m[2][1]).StoreASCII("  ").StoreASCII(vout.m[2][2]).StoreASCII("\n")
				.StoreASCII("\n");
	if(1)
	{
	//vout.Transpose();
			Point Row0, Row1, Row2;
			vout.GetRow(0, Row0);
			vout.GetRow(1, Row1);
			vout.GetRow(2, Row2);
	//		Row0*=dout.x;
	//		Row1*=dout.y;
	//		Row2*=dout.z;
			Matrix3 Permut;
			Permut.IdentityMatrix();
//			Permut.SetRow(0, Point3(Row0.x, Row0.y, Row0.z));
//			Permut.SetRow(1, Point3(Row1.x, Row1.y, Row1.z));
//			Permut.SetRow(2, Point3(Row2.x, Row2.y, Row2.z));
			Permut.SetRow(0, Point3(Row0.x, Row1.x, Row2.x));
			Permut.SetRow(1, Point3(Row0.y, Row1.y, Row2.y));
			Permut.SetRow(2, Point3(Row0.z, Row1.z, Row2.z));

	//		Matrix3 Pipo = ScaleMatrix(Point3(dout.x, dout.y, dout.z));
			Point3 Trans = RelTM.GetTrans();
	//		RelTM.SetTrans(Point3(0.0f,0.0f,0.0f));
	//		Trans.x*=dout.x;
	//		Trans.y*=dout.y;
	//		Trans.z*=dout.z;
	//		RelTM=Pipo*RelTM;
	//		RelTM=Permut*RelTM;
			RelTM = Permut;
			RelTM.SetTrans(Trans);
	}
		}
	}

	if(0)
	{
		if(IsMirrored(node, timevalue))
		{
			Point3 R0 = RelTM.GetRow(0);
			Point3 R1 = RelTM.GetRow(1);
			Point3 R2 = RelTM.GetRow(2);

			mXForm.m[0][0] = R0.x;
			mXForm.m[0][1] = R0.y;
			mXForm.m[0][2] = R0.z;

			mXForm.m[1][0] = R1.x;
			mXForm.m[1][1] = R1.y;
			mXForm.m[1][2] = R1.z;

			mXForm.m[2][0] = R2.x;
			mXForm.m[2][1] = R2.y;
			mXForm.m[2][2] = R2.z;
mXForm.Neg();
			mResetXForm = TRUE;
			Point3 Trans = RelTM.GetTrans();
			RelTM.IdentityMatrix();
			RelTM.SetTrans(Trans);
		}
	}
#endif

	// Decompose the matrix into affine parts
	decomp_affine(RelTM, &static_prs);

	// Discard scaling if needed
	if(remove_non_uniform_scaling)
	{
		static_prs.k.x = static_prs.k.y = static_prs.k.z = 1.0f;
	}
	return *this;
}

void MAXConverter::ConvertPRS(PRS& prs)
{
	// Global scaling
//	if(mExporter)
	{
		Point Scaling(1.0f, 1.0f, 1.0f);
//		mExporter->AdjustScale(Scaling.x, Scaling.y, Scaling.z);
		Scaling *= mSettings->mGlobalScale;

		if(IR(Scaling.x)!=IEEE_1_0)	prs.mPos.x *= Scaling.x;
		if(IR(Scaling.y)!=IEEE_1_0)	prs.mPos.y *= Scaling.y;
		if(IR(Scaling.z)!=IEEE_1_0)	prs.mPos.z *= Scaling.z;
	}

	// Convert to D3D if needed
	if(mSettings->mConvertToD3D)
	{
		Swap(IR(prs.mPos.y), IR(prs.mPos.z));
		Swap(IR(prs.mRot.p.y), IR(prs.mRot.p.z));
		Swap(IR(prs.mScale.y), IR(prs.mScale.z));
	}

	// User-defined frame conversion
	if(mExporter)
	{
		mExporter->ConvertPoint(prs.mPos.x, prs.mPos.y, prs.mPos.z);
		mExporter->ConvertQuat(prs.mRot.p.x, prs.mRot.p.y, prs.mRot.p.z, prs.mRot.w);
		mExporter->ConvertPoint(prs.mScale.x, prs.mScale.y, prs.mScale.z);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Converts the PRS of a given node, at a given time
 *	\param		node			[in] the MAX node
 *	\param		time_value		[in] the frame number
 *	\param		prs				[out] a PRS structure to fill with converted data
 *	\return		Self-Reference
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXConverter& MAXConverter::ConvertPRS(INode* node, long time_value, PRS& prs)
{
	AffineParts	StaticPRS;		// MAX will cut a matrix to pieces into this structure

	// Should the non-uniform scaling be removed? We remove it for skeleton parts, and for all nodes when user asked it.
	bool RemoveScaling = mSettings->mRemoveScaling ? true : IsSkeleton(node) ? true : false;

	// Compute the affine parts
	GetStaticPRS(node, time_value, RemoveScaling, StaticPRS);

	// Fill the result structure
	CopyPRS(prs, StaticPRS);

	// Output a warning on negative scales
	if(StaticPRS.f<0.0f)	mArrayLog.StoreASCII("WARNING: negative scale\n");

	ConvertPRS(prs);
	return *this;
}
