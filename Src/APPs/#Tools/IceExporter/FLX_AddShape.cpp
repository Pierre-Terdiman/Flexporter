///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a shape.
 *	\file		FLX_AddShape.cpp
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
 *	Converts a shape.
 *	\param		node			[in] the MAX node
 *	\param		derived_object	[in] a possible collapsed derived object
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddShape(INode* node, Object* derived_object)
{
	// Checkings
	if(!node)	return false;
	if(!mSettings->mExpShapes && !mProps.mForceExport)
	{
		// Update log text
		mArrayLog.StoreASCII("(discarded because of export options)\n");
		return true;
	}

	// Get time info back
	long		StartTime	= mTimeInfo.mStartTime;
	long		EndTime		= mTimeInfo.mEndTime;
	long		DeltaTime	= mTimeInfo.mDeltaTime;
	long		FrameRate	= mTimeInfo.mFrameRate;
	TimeValue	CurrentTime	= mTimeInfo.mCurrentTime;

	// Get right object: it can be the real node or a collapsed derived object
	Object* NodeObject = derived_object ? derived_object : node->GetObjectRef();

	// 1) Update log text
	char*		ShapeName	= node->GetName();				// Shape's name
	mArrayLog.StoreASCII("Exporting shape %s\n", ShapeName);

	// Convert to mesh if needed
	if((mSettings->mShapeToMesh || mProps.mForceShapeToMesh) && !mProps.mDiscardShapeToMesh)
	{
		ObjectState os = node->EvalWorldState(CurrentTime);
		Object* NodeObject = os.obj;

		if(NodeObject && NodeObject->CanConvertToType(triObjectClassID))
		{
			mArrayLog.StoreASCII("Converting shape to mesh...");	// Update log text
			return AddGeomObject(node, NodeObject);
		}
		else mArrayLog.StoreASCII("Can't convert shape to mesh...");	// Update log text
	}

	// Material stuff
	if(!AddMetaMaterial(node))	IceCore::MessageBox(null, _F("Object %s has an invalid material!\n", node->GetName()), "Oops", MB_OK);

	// Get the material ID for the shape
	sdword MatID = ComputeMaterialID(node, null, 0, false);	// Absolute ID
	MatID = RemapMaterialID(MatID);

	// 2) Get useful information from MAX
	Interval	Inter			= FOREVER;												// Validity interval
	bool		IsGroupMember	=					node	->IsGroupMember()!=0;		// Belonging to a group

	ShapeObject* Shape			= (ShapeObject*)	NodeObject;							// Assign node-object as a generic MAX shape

	// Get type
	ShapeType SType = FLX_SHAP_UNDEFINED;
	{
		Class_ID	Type			= Shape->ClassID();
		if(Type==Class_ID(SPLINE3D_CLASS_ID, 0))
		{
			mArrayLog.StoreASCII("(Spline)\n");
			SType = FLX_SHAP_SPLINE;

//			SimpleObject* SObj = (SimpleObject*)NodeObject;

		}
		else if(Type==Class_ID(NGON_CLASS_ID, 0))
		{
			mArrayLog.StoreASCII("(N-gon)\n");
			SType = FLX_SHAP_NGON;
		}
		else if(Type==Class_ID(DONUT_CLASS_ID, 0))
		{
			mArrayLog.StoreASCII("(Donut)\n");
			SType = FLX_SHAP_DONUT;
		}
		else if(Type==Class_ID(STAR_CLASS_ID, 0))
		{
			mArrayLog.StoreASCII("(Star)\n");
			SType = FLX_SHAP_STAR;
		}
		else if(Type==Class_ID(RECTANGLE_CLASS_ID, 0))
		{
			mArrayLog.StoreASCII("(Rectangle)\n");
			SType = FLX_SHAP_RECTANGLE;
		}
		else if(Type==Class_ID(HELIX_CLASS_ID, 0))
		{
			mArrayLog.StoreASCII("(Helix)\n");
			SType = FLX_SHAP_HELIX;
		}
		else if(Type==Class_ID(ELLIPSE_CLASS_ID, 0))
		{
			mArrayLog.StoreASCII("(Ellipse)\n");
			SType = FLX_SHAP_ELLIPSE;
		}
		else if(Type==Class_ID(CIRCLE_CLASS_ID, 0))
		{
			mArrayLog.StoreASCII("(Circle)\n");
			SType = FLX_SHAP_CIRCLE;
		}
		else if(Type==Class_ID(TEXT_CLASS_ID, 0))
		{
			mArrayLog.StoreASCII("(Text)\n");
			SType = FLX_SHAP_TEXT;
		}
		else if(Type==Class_ID(ARC_CLASS_ID, 0))
		{
			mArrayLog.StoreASCII("(Arc)\n");
			SType = FLX_SHAP_ARC;
		}
	}
//	int	NumVertices = Shape->NumberOfVertices(CurrentTime);
//	int NumCurves	= Shape->NumberOfCurves();

//	mArrayLog.StoreASCII("\n").StoreASCII(NumVertices).StoreASCII(" ").StoreASCII(NumCurves).StoreASCII("\n");
//	mArrayLog.StoreASCII("\n").StoreASCII(udword(Shape)).StoreASCII("\n");

	// We will output shapes as a collection of polylines. Each polyline contains collection of line segments.
	PolyShape	pShape;
	Shape->MakePolyShape(CurrentTime, pShape);

	udword NbLines = pShape.numLines;

	CustomArray VertsNumbers;
	CustomArray ClosedStatus;
//	CustomArray Verts;
	Vertices Verts;

	// Loop through lines
//	udword TotalNbVerts = 0;
	for(udword j=0;j<NbLines;j++)
	{
		// Get current line
		PolyLine* Line = &pShape.lines[j];

		// Get number of vertices for current line
		udword NbVerts = Line->numPts;
		bool Closed = Shape->CurveClosed(CurrentTime, j)==TRUE;

		VertsNumbers.Store(NbVerts);
		ClosedStatus.Store(Closed);

		// Loop through vertices
		for(udword i=0;i<NbVerts;i++)
		{
			// Get current vertex
			PolyPt* Pt = &Line->pts[i];

			float x = Pt->p.x;
			float y = Pt->p.y;
			float z = Pt->p.z;

			// Global scaling
			{
				Point Scaling(1.0f, 1.0f, 1.0f);
		//		mExporter->AdjustScale(Scaling.x, Scaling.y, Scaling.z);
				Scaling *= mSettings->mGlobalScale;

				if(IR(Scaling.x)!=IEEE_1_0)	x *= Scaling.x;
				if(IR(Scaling.y)!=IEEE_1_0)	y *= Scaling.y;
				if(IR(Scaling.z)!=IEEE_1_0)	z *= Scaling.z;
			}

			// Convert to D3D if needed
			if(mSettings->mConvertToD3D)	Swap(IR(y), IR(z));

			// User-defined frame conversion
			if(mExporter)	mExporter->ConvertPoint(x, y, z);

//			Verts.Store(x).Store(y).Store(z);
			Verts.AddVertex(x, y, z);
//			TotalNbVerts++;
		}
	}

	// 3) Create info structure
	ShapeDescriptor* ShapeInf = (ShapeDescriptor*)mGlobalInfo.CreateDescriptor(node, FLX_OBJ_SHAPE);
	AddHeader(node, ShapeInf);

	// Get instance state (if needed)
	sdword MasterID;
	ShapeInf->mIsInstance = mSettings->mDiscardInstances ? false : mGlobalInfo.IsInstance(NodeObject, MasterID);

	// Check instance state (### to refactor)
	if(ShapeInf->mIsInstance)
	{
		ShapeInf->mMasterShape	= (ShapeDescriptor*)mGlobalInfo.GetNodeInfo()[MasterID].mExportedNode;
		ShapeInf->mMasterID		= ShapeInf->mMasterShape->mObjectID;

		// Update log text
		mArrayLog.StoreASCII("Instance node from: ").StoreASCII(mGlobalInfo.GetNodeInfo()[MasterID].mMAXNode->GetName()).StoreASCII("\n");
	}

	ShapeInf->mType			= SType;
	ShapeInf->mNbLines		= NbLines;
	ShapeInf->mMatID		= MatID;
	if(NbLines)
	{
		ShapeInf->mNbVerts	= new udword[NbLines];					CopyMemory(ShapeInf->mNbVerts,	VertsNumbers.Collapse(),	NbLines*sizeof(udword));
		ShapeInf->mClosed	= new bool[NbLines];					CopyMemory(ShapeInf->mClosed,	ClosedStatus.Collapse(),	NbLines*sizeof(bool));
//		ShapeInf->mVerts	= new NakedPoint[TotalNbVerts];			CopyMemory(ShapeInf->mVerts,	Verts.Collapse(),			TotalNbVerts*sizeof(NakedPoint));
		ShapeInf->mVerts	= new NakedPoint[Verts.GetNbVertices()];	CopyMemory(ShapeInf->mVerts,	Verts.GetVertices(),		Verts.GetNbVertices()*sizeof(NakedPoint));
	}
	else
	{
		// A shape with 0 lines is probably a shape whose modifier stack is not empty.
		// EvalWorldState seems not to collapse those shapes... Don't know why.
		mArrayLog.StoreASCII("\nThat shape has 0 lines... Try to manually collapse it.\n");
	}

	// 4) Update members
	mStats.mNbShapes++;

	// 5) Perform format-dependent export
	SetExtStatus("calling export plug-in...");
	if(mExporter)	mExporter->ExportShape(*ShapeInf);
	DumpLogString();

	// 6) Animation
	// 6-1) PRS track
	SetExtStatus("exporting PRS data...");
	bool Status = AddPRSController(node, ShapeInf);

	// 6-2) Visibility track
	SetExtStatus("exporting visibility data...");
	Status = AddVisibilityController(node, ShapeInf);

	return true;
}

#ifdef OLD
	BezierShape		BZShape;
	long			numLines;

	switch(mFormat)
	{
		case ES_FORMAT_ZCB:
		{
			// Export Bézier shapes
			if(mFormat==ES_FORMAT_ZCB)
			{
				long NbCurves = Shape->NumberOfCurves();
				mShapes.Store(NbCurves);
				for(long Curve=0;Curve<NbCurves;Curve++)
				{
					if(Shape->CanMakeBezier())
					{
						mShapes.Store((char)1);
						Shape->MakeBezier(0, BZShape);

						BOOL	Closed		= Shape->CurveClosed(0, Curve);
						int		NbPieces	= Shape->NumberOfPieces(0, Curve);

						long	NbSplines	= BZShape.splineCount;
						mShapes.Store(NbSplines);
						for(long Spline=0;Spline<NbSplines;Spline++)
						{
							Spline3D* CurSpline = BZShape.splines[Spline];
							long NbKnots = CurSpline->KnotCount();
							mShapes.Store(NbKnots);
							for(long BezPt=0;BezPt<NbKnots;BezPt++)
							{
								Point3	InVec		= CurSpline->GetInVec(BezPt);
								Point3	KnotPoint	= CurSpline->GetKnotPoint(BezPt);
								Point3	OutVec		= CurSpline->GetOutVec(BezPt);
								mShapes.Store(InVec.x);
								mShapes.Store(InVec.y);
								mShapes.Store(InVec.z);
								mShapes.Store(KnotPoint.x);
								mShapes.Store(KnotPoint.y);
								mShapes.Store(KnotPoint.z);
								mShapes.Store(OutVec.x);
								mShapes.Store(OutVec.y);
								mShapes.Store(OutVec.z);
							}
						}

					}
					else mShapes.Store((char)0);
				}
			}
		////////
#endif

