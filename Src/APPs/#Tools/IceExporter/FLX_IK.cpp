///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains all IK parameters.
 *	\file		FLX_IK.cpp
 *	\author		Pierre Terdiman
 *	\date		August, 7, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;







/*
class IIKControl {
public:
  typedef IKSys::DofAxis DofAxis;
  typedef IKSys::JointType JointType;
  typedef IKSys::DofSet DofSet;

  //Queries
  virtual bool		DofActive(DofAxis) const =0;
  virtual DofSet	ActiveTrans() const =0;
  virtual DofSet	ActiveRot() const =0;
  virtual DofSet	ActiveDofs() const =0;
  virtual INodeTab	IKChains(JointType) const =0;
  virtual bool		DofLowerLimited(DofAxis) const =0;
  virtual bool		DofUpperLimited(DofAxis) const =0;
  virtual Point2	DofLimits(DofAxis) const =0;
  virtual Point3	TransLowerLimits() const =0;
  virtual Point3	TransUpperLimits() const =0;
  virtual Point3	RotLowerLimits() const =0;
  virtual Point3	RotUpperLimits() const =0;
  virtual bool		IKBound(TimeValue t, JointType jt)=0;
  virtual Control*	FKSubController() const =0;
  virtual INode*	GetNode() const =0;
  virtual Point3	PrefPosition(TimeValue t, Interval& validityInterval) =0;
  virtual Point3	PrefRotation(TimeValue t, Interval& validityInterval) =0;

  // DOF values
  virtual Point3	TransValues(TimeValue, Interval* =0)=0;
  virtual Point3	RotValues(TimeValue, Interval* =0)=0;
  virtual void		AssignTrans(const Point3&, const Interval&)=0;
  virtual void		AssignRot(const Point3&, const Interval&)=0;
  virtual void		AssignActiveTrans(const Point3&, const Interval&)=0;
  virtual void		AssignActiveRot(const Point3&, const Interval&)=0;
  virtual void		AssignActiveTrans(const DofSet&, const float[],
									  const Interval&)=0;
  virtual void		AssignActiveRot(const DofSet&, const float[],
									const Interval&)=0;
  virtual void		SetTransValid(const Interval& valid) =0;
  virtual void		SetRotValid(const Interval& valid) =0;
  virtual void		SetTRValid(const Interval& valid) =0;
  virtual void		SetPrefTrans(const Point3& val, TimeValue t =0) =0;
  virtual void		SetPrefRot(const Point3& val, TimeValue t =0) =0;
  virtual void		SetPrefTR(const Point3& trans, const Point3& rot,
							  TimeValue t =0) =0;
};

  enum DofAxis
  {
	TransX = 0,	TransY,	TransZ,
	RotX, RotY,	RotZ,
	DofX = 0, DofY,	DofZ
  };

  enum JointType
  {
	SlidingJoint,
	RotationalJoint
  };

  struct DofSet
  {
	DofSet() : bits(0) {}
	DofSet(const DofSet& src) : bits(src.bits) {}
	void Add(DofAxis dn) { bits |= (1 << dn); }
	void Clear(DofAxis dn) { bits &= ~(unsigned(1 << dn)); }
	int Include(DofAxis dn) const { return bits & (1 << dn); }
	int Count() const;
	unsigned bits;
  };

*/

#ifndef FLEXPORTER_MAX3
using namespace IKSys;

// old test for MAX4 interfaces
bool MAXConverter::DumpIKData(IIKControl* ik)
{
	if(!ik)	return false;

	bool ActiveTransX	= ik->DofActive(TransX);
	bool ActiveTransY	= ik->DofActive(TransY);
	bool ActiveTransZ	= ik->DofActive(TransZ);
	bool ActiveRotX		= ik->DofActive(RotX);
	bool ActiveRotY		= ik->DofActive(RotY);
	bool ActiveRotZ		= ik->DofActive(RotZ);

	DofSet DS0 = ik->ActiveTrans();
	DofSet DS1 = ik->ActiveRot();
	DofSet DS2 = ik->ActiveDofs();

//	INodeTab NT0 = ik->IKChains(SlidingJoint);
//	INodeTab NT1 = ik->IKChains(RotationalJoint);

	bool Low0 = ik->DofLowerLimited(TransX);
	bool Low1 = ik->DofLowerLimited(TransY);
	bool Low2 = ik->DofLowerLimited(TransZ);
	bool Low3 = ik->DofLowerLimited(RotX);
	bool Low4 = ik->DofLowerLimited(RotY);
	bool Low5 = ik->DofLowerLimited(RotZ);

	bool Up0 = ik->DofUpperLimited(TransX);
	bool Up1 = ik->DofUpperLimited(TransY);
	bool Up2 = ik->DofUpperLimited(TransZ);
	bool Up3 = ik->DofUpperLimited(RotX);
	bool Up4 = ik->DofUpperLimited(RotY);
	bool Up5 = ik->DofUpperLimited(RotZ);
return true;
	Point2 Limits0 = ik->DofLimits(TransX);
	Point2 Limits1 = ik->DofLimits(TransY);
	Point2 Limits2 = ik->DofLimits(TransZ);
	Point2 Limits3 = ik->DofLimits(RotX);
	Point2 Limits4 = ik->DofLimits(RotY);
	Point2 Limits5 = ik->DofLimits(RotZ);

	Point3 p1 =	ik->TransLowerLimits();
	Point3 p2 =	ik->TransUpperLimits();
	Point3 p3 =	ik->RotLowerLimits();
	Point3 p4 =	ik->RotUpperLimits();

//	bool b2 = ik->IKBound(TimeValue t, JointType jt);
	Control* c = ik->FKSubController();
	INode* i = ik->GetNode();
//	Point3 p5 =	ik->PrefPosition(TimeValue t, Interval& validityInterval);
//	Point3 p6 = ik->PrefRotation(TimeValue t, Interval& validityInterval);

/*	// DOF values
	virtual Point3	ik->TransValues(TimeValue, Interval* =0)=0;
	virtual Point3	ik->RotValues(TimeValue, Interval* =0)=0;
	virtual void		ik->AssignTrans(const Point3&, const Interval&)=0;
	virtual void		ik->AssignRot(const Point3&, const Interval&)=0;
	virtual void		ik->AssignActiveTrans(const Point3&, const Interval&)=0;
	virtual void		ik->AssignActiveRot(const Point3&, const Interval&)=0;
	virtual void		ik->AssignActiveTrans(const DofSet&, const float[],
									  const Interval&)=0;
	virtual void		ik->AssignActiveRot(const DofSet&, const float[],
									const Interval&)=0;
	virtual void		ik->SetTransValid(const Interval& valid) =0;
	virtual void		ik->SetRotValid(const Interval& valid) =0;
	virtual void		ik->SetTRValid(const Interval& valid) =0;
	virtual void		ik->SetPrefTrans(const Point3& val, TimeValue t =0) =0;
	virtual void		ik->SetPrefRot(const Point3& val, TimeValue t =0) =0;
	virtual void		ik->SetPrefTR(const Point3& trans, const Point3& rot,
							  TimeValue t =0) =0;
*/

	return true;
}
#endif

// Fill from DOFParams
// data		[out] 
// params	[in] MAX source
static bool GetJointData(JointDescriptor& data, DOFParams& params)
{
	// "The first 3 DOFs are assumed to be position
	// and the next 3 are assumed to be rotation"

	udword Flags = FLX_VDOF_ACTIVE|FLX_VDOF_LIMIT|FLX_VDOF_MIN_MAX|FLX_VDOF_AXIS|FLX_VDOF_POS|FLX_VDOF_CURRENT;

	// Read position DOFs
	for(udword i=0;i<3;i++)
	{
		// Setup flags
		data.mPos[i].mValidFlags = Flags;

		// If display is true, read the other params. Else don't bother, as they contain junk values.
		if(params.display[i])
		{
			data.mPos[i].mActive		= false;
			data.mPos[i].mAxis.x		= params.axis[i].x;
			data.mPos[i].mAxis.y		= params.axis[i].y;
			data.mPos[i].mAxis.z		= params.axis[i].z;
			data.mPos[i].mPos.x			= params.pos[i].x;
			data.mPos[i].mPos.y			= params.pos[i].y;
			data.mPos[i].mPos.z			= params.pos[i].z;
			data.mPos[i].mLimit			= params.limit[i]!=0;
			data.mPos[i].mMin			= params.min[i];
			data.mPos[i].mMax			= params.max[i];
			data.mPos[i].mCurrentValue	= params.curval[i];
			// Other parameters not available
		}
	}

	// Read rotation DOFs
	for(udword i=0;i<3;i++)
	{
		// Setup flags
		data.mRot[i].mValidFlags = Flags;

		// If display is true, read the other params. Else don't bother, as they contain junk values.
		if(params.display[i+3])
		{
			data.mRot[i].mActive		= false;
			data.mRot[i].mAxis.x		= params.axis[i+3].x;
			data.mRot[i].mAxis.y		= params.axis[i+3].y;
			data.mRot[i].mAxis.z		= params.axis[i+3].z;
			data.mRot[i].mPos.x			= params.pos[i+3].x;
			data.mRot[i].mPos.y			= params.pos[i+3].y;
			data.mRot[i].mPos.z			= params.pos[i+3].z;
			data.mRot[i].mLimit			= params.limit[i+3]!=0;
			data.mRot[i].mMin			= params.min[i+3];
			data.mRot[i].mMax			= params.max[i+3];
			data.mRot[i].mCurrentValue	= params.curval[i+3];
			// Other parameters not available
		}
	}
	return true;
}

// Fill from JointParams - the most frequent case
// data		[out] room for 3 DOFs
// joint	[in] MAX source
static bool GetJointData(DOFDescriptor* data, JointParams* joint)
{
	// 1) Checkings
	if(!data || !joint)	return false;

	// 2-0) Catch basic joint data from MAX
	int	DOFS		= joint->dofs;
	float* Min		= joint->min;
	float* Max		= joint->max;
	float* Damping	= joint->damping;
	float* Spring	= joint->spring;
	float* Stens	= joint->stens;
	float Scale		= joint->scale;
	DWORD Flags		= joint->flags;
	DWORD Type		= joint->Type();	// JNT_POS or JNT_ROT

	// 2-1) Catch extended joint data from MAX
#ifndef FLEXPORTER_MAX3
	JointParams2* J2 = joint->GetJointParams2Interface();
	float* PreferredAngle = J2 ? J2->preferredAngle : null;
#endif
	// 3) Keep track of relevant stuff
	if(DOFS<=3)	// Security
	{
		for(udword i=0;i<DOFS;i++)
		{
			data[i].mMin			= Min[i];
			data[i].mMax			= Max[i];
			data[i].mDamping		= Damping[i];
			data[i].mSpringValue	= Spring[i];
			data[i].mSpringTension	= Stens[i];
		}
	}
	//
//	data.mPos			= (Type & JNT_POS)!=0;
//	data.mRot			= (Type & JNT_ROT)!=0;
//	data.mLimitExact	= (Flags & JNT_LIMITEXACT)!=0;

	data[0].mActive		= (Flags & JNT_XACTIVE)!=0;
	data[1].mActive		= (Flags & JNT_YACTIVE)!=0;
	data[2].mActive		= (Flags & JNT_ZACTIVE)!=0;

	data[0].mLimit		= (Flags & JNT_XLIMITED)!=0;
	data[1].mLimit		= (Flags & JNT_YLIMITED)!=0;
	data[2].mLimit		= (Flags & JNT_ZLIMITED)!=0;

	data[0].mEase		= (Flags & JNT_XEASE)!=0;
	data[1].mEase		= (Flags & JNT_YEASE)!=0;
	data[2].mEase		= (Flags & JNT_ZEASE)!=0;

	data[0].mSpring		= (Flags & JNT_XSPRING)!=0;
	data[1].mSpring		= (Flags & JNT_YSPRING)!=0;
	data[2].mSpring		= (Flags & JNT_ZSPRING)!=0;

	// Setup flags
	udword VFlags = FLX_VDOF_ACTIVE|FLX_VDOF_LIMIT|FLX_VDOF_EASE|FLX_VDOF_SPRING|FLX_VDOF_MIN_MAX|FLX_VDOF_DAMPING|FLX_VDOF_SPRING_VALUE|FLX_VDOF_SPRING_TENSION;
	data[0].mValidFlags	= VFlags;
	data[1].mValidFlags	= VFlags;
	data[2].mValidFlags	= VFlags;

	return true;
}

/*
class IKEnumCallback {
	public:
		virtual void proc(Control *c, int index)=0;
	};
*/
/*
class IKCallback : public IKEnumCallback
{
	public:
	virtual void proc(Control* cont, int index)
	{
		JointParams* jp = (JointParams*)cont->GetProperty(PROPID_JOINTPARAMS);
//		for (int i=0; i<3; i++)
//		{
//			if (!jp || jp->Active(i))
//			{
//				callback.proc(cont,i);
//			}
//		}
		long p=0;
	}
};
*/



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets IK data from a node.
 *	\param		node	[in] a MAX node
 *	\param		obj		[out] filled with IK data
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MAXConverter::GetIK(INode* node, ObjectDescriptor& obj)
{
	// Checkings
	if(!node)	return;

	// Check user-properties overrides
	if(mProps.mDiscardIK)		return;

	if(!mSettings->mExportIK && !mProps.mForceIK)
	{
		return;
	}

	// Catch TM controller
	Control* Transform = node->GetTMController();
	if(!Transform)	return;

	// Following seems to be the most frequent IK params:
	// - in my test scenes
	// - in the NovodeX scenes
	// - in the MAX3 scenes
	//
	// It's also what you can find in MAX's AsciiExp exporter
	//
	// Now, I think there was a bug in AsciiExp: "IsDefault()" probably shouldn't be used....

	// Position joint
	Control* PosJoint = Transform->GetPositionController();
	if(PosJoint)
	{
		JointParams* Params = (JointParams*)PosJoint->GetProperty(PROPID_JOINTPARAMS);
		if(Params && !Params->IsDefault())	// ###
		{
			mArrayLog.StoreASCII("(Found IK data - PROPID_JOINTPARAMS position joint)\n");
			if(!obj.mIKData)	obj.mIKData = new JointDescriptor;
			bool Status = GetJointData(obj.mIKData->mPos, Params);
		}
	}

	// Rotational joint
	Control* RotJoint = Transform->GetRotationController();
	if(RotJoint)
	{
		JointParams* Params = (JointParams*)RotJoint->GetProperty(PROPID_JOINTPARAMS);
		if(Params && !Params->IsDefault())	// ###
		{
			mArrayLog.StoreASCII("(Found IK data - PROPID_JOINTPARAMS rotation joint)\n");
			if(!obj.mIKData)	obj.mIKData = new JointDescriptor;
			bool Status = GetJointData(obj.mIKData->mRot, Params);
		}
	}

/*
	// Test
	if(node->TestAFlag(A_INODE_IK_TERMINATOR))	mArrayLog.StoreASCII("Node has IK terminator flag\n");
	if(node->TestAFlag(A_INODE_IK_POS_PINNED))	mArrayLog.StoreASCII("Node has IK pos-pinned flag\n");
	if(node->TestAFlag(A_INODE_IK_ROT_PINNED))	mArrayLog.StoreASCII("Node has IK rot-pinned flag\n");
*/

	// IK from DOFparams
	Matrix3 ptm;
	DOFParams dofs;
	if(Transform->GetDOFParams(mTimeInfo.mCurrentTime, ptm, dofs, FALSE))	// ### FALSE ?
	{
		mArrayLog.StoreASCII("(Found IK data - DOFParams)\n");
		if(!obj.mIKData)	obj.mIKData = new JointDescriptor;
		bool Status = GetJointData(*obj.mIKData, dofs);
	}

//	return;


#ifndef FLEXPORTER_MAX3
	IOrientConstRotation* IOCR = GetIOrientConstInterface(node);
		//Transform->GetInterface(ORIENT_CONSTRAINT_INTERFACE);
	if(IOCR)
	{
		mArrayLog.StoreASCII("Found orientation constraint\n");
		udword NbTargets = IOCR->GetNumTargets();
		mArrayLog.StoreASCII("  %d targets:\n", NbTargets);
		for(udword i=0;i<NbTargets;i++)
		{
			INode* Target = IOCR->GetNode(i);
			if(Target)
			{
				mArrayLog.StoreASCII("  - %s\n", Target->GetName());
			}
		}
	}

//#ifdef OLDIES


/////////
IIKControl* IK;

IK = (IIKControl*)node->Animatable::GetInterface(I_IKCONTROL);
if(IK)	mArrayLog.StoreASCII("(Found unsupported IK data - I_IKCONTROL interface A)\n");

IK = (IIKControl*)node->Animatable::GetInterface(I_IKCHAINCONTROL);
if(IK)	mArrayLog.StoreASCII("(Found unsupported IK data - I_IKCHAINCONTROL interface A)\n");

if(node->GetTMController())
{
	IK = (IIKControl*)node->GetTMController()->GetInterface(I_IKCONTROL);
	if(IK)
	{
		mArrayLog.StoreASCII("(Found unsupported IK data - I_IKCONTROL interface B)\n");
//		DumpIKData(IK);
	}

	IK = (IIKControl*)node->GetTMController()->GetInterface(I_IKCHAINCONTROL);
	if(IK)
	{
		mArrayLog.StoreASCII("(Found unsupported IK data - I_IKCHAINCONTROL interface B)\n");
//		DumpIKData(IK);
	}
}
#endif

#ifdef OLDIES
	Matrix3 ptm;
	DOFParams dofs;
	if(Transform->GetDOFParams(0, ptm, dofs, FALSE))
	{
		mArrayLog.StoreASCII("Found DOF params\n");

/*
		class DOFParams {
			public:
				BOOL display[6];		// Should this DOF be displayed?
				Point3 axis[6];			// DOF axis
				Point3 pos[6];			// Base of axis
				BOOL limit[6];          // is joint limited?
				float min[6];			// min limit
				float max[6];           // max limit
				float curval[6];		// Current value of the parameter
				BOOL sel[6];			// should DOF be highlighted
				BOOL endEffector;		// is there an end effector for this controller
				Matrix3 eeTM;			// world TM of the end effector if present
			};
*/

		// If display is true, read the other params. Else don't bother, as they contain junk values.
		mArrayLog.StoreASCII("Display: %d %d %d %d %d %d\n",
			dofs.display[0],
			dofs.display[1],
			dofs.display[2],
			dofs.display[3],
			dofs.display[4],
			dofs.display[5]
			);

		mArrayLog.StoreASCII("Limit: %d %d %d %d %d %d\n",
			dofs.limit[0],
			dofs.limit[1],
			dofs.limit[2],
			dofs.limit[3],
			dofs.limit[4],
			dofs.limit[5]
			);

		mArrayLog.StoreASCII("Min %f %f %f %f %f %f\n",
			dofs.min[0],//*RADTODEG,
			dofs.min[1],//*RADTODEG,
			dofs.min[2],//*RADTODEG,
			dofs.min[3],//*RADTODEG,
			dofs.min[4],//*RADTODEG,
			dofs.min[5]//*RADTODEG
			);

		mArrayLog.StoreASCII("Max %f %f %f %f %f %f\n",
			dofs.max[0],//*RADTODEG,
			dofs.max[1],//*RADTODEG,
			dofs.max[2],//*RADTODEG,
			dofs.max[3],//*RADTODEG,
			dofs.max[4],//*RADTODEG,
			dofs.max[5]//*RADTODEG
			);
	}

	// Below: seems useless, all nodes pass that test

	InitJointData Pos, Rot;
	BOOL HasPos = Transform->CanCopyIKParams(COPYPASTE_IKPOS);
	BOOL HasRot = Transform->CanCopyIKParams(COPYPASTE_IKROT);
	// Warning: the booleans above are not always reliable

	if(Transform->GetIKJoints(&Pos, &Rot))
	{
		mArrayLog.StoreASCII("Found IK joints: Pos: %d Rot: %d\n", HasPos, HasRot);

		// Seems like the minimal IK info we can find.
		// Still, it's also there for non-IK objects (!). Create a sphere, export it, find IK joints (!!).
/*
		class InitJointData {
			public:
				BOOL active[3];
				BOOL limit[3];
				BOOL ease[3];
				Point3 min, max, damping;
			};
*/
		mArrayLog.StoreASCII("Active: %d %d %d %d %d %d\n",
			Pos.active[0],
			Pos.active[1],
			Pos.active[2],
			Rot.active[0],
			Rot.active[1],
			Rot.active[2]
			);

		mArrayLog.StoreASCII("Limit: %d %d %d %d %d %d\n",
			Pos.limit[0],
			Pos.limit[1],
			Pos.limit[2],
			Rot.limit[0],
			Rot.limit[1],
			Rot.limit[2]
			);

		mArrayLog.StoreASCII("Min %f %f %f %f %f %f\n",
			Pos.min.x,//*RADTODEG,
			Pos.min.y,//*RADTODEG,
			Pos.min.z,//*RADTODEG,
			Rot.min.x,//*RADTODEG,
			Rot.min.y,//*RADTODEG,
			Rot.min.z//*RADTODEG
			);

		mArrayLog.StoreASCII("Max %f %f %f %f %f %f\n",
			Pos.max.x,//*RADTODEG,
			Pos.max.y,//*RADTODEG,
			Pos.max.z,//*RADTODEG,
			Rot.max.x,//*RADTODEG,
			Rot.max.y,//*RADTODEG,
			Rot.max.z//*RADTODEG
			);
	}
#endif


//#endif


/*	struct Local
	{
		static void QuatEnumIKParams(Control *cont,IKEnumCallback &callback)
		{
			JointParams *jp = (JointParams*)cont->GetProperty(PROPID_JOINTPARAMS);
			for (int i=0; i<3; i++)
			{
				if (!jp || jp->Active(i))
				{
					callback.proc(cont,i);
				}
			}
		}
	};
*/
//	IKCallback ikcb;
//	Transform->EnumIKParams(ikcb);

	// Fix IK data before leaving
	if(obj.mIKData)	obj.mIKData->FixData(mSettings);
}



#ifdef PIPO
void AsciiExp::ExportIKJoints(INode* node, int indentLevel)
{
	Control* cont;
	TSTR indent = GetIndent(indentLevel);

	if (node->TestAFlag(A_INODE_IK_TERMINATOR)) 
		fprintf(pStream,"%s\t%s\n", indent.data(), ID_IKTERMINATOR);

	if(node->TestAFlag(A_INODE_IK_POS_PINNED))
		fprintf(pStream,"%s\t%s\n", indent.data(), ID_IKPOS_PINNED);

	if(node->TestAFlag(A_INODE_IK_ROT_PINNED))
		fprintf(pStream,"%s\t%s\n", indent.data(), ID_IKROT_PINNED);

	// Position joint
	cont = node->GetTMController()->GetPositionController();
	if (cont) {
		JointParams* joint = (JointParams*)cont->GetProperty(PROPID_JOINTPARAMS);
		if (joint && !joint->IsDefault()) {
			// Has IK Joints!!!
			fprintf(pStream,"%s\t%s {\n", indent.data(), ID_IKJOINT);
			DumpJointParams(joint, indentLevel+1);
			fprintf(pStream,"%s\t}\n", indent.data());
		}
	}

	// Rotational joint
	cont = node->GetTMController()->GetRotationController();
	if (cont) {
		JointParams* joint = (JointParams*)cont->GetProperty(PROPID_JOINTPARAMS);
		if (joint && !joint->IsDefault()) {
			// Has IK Joints!!!
			fprintf(pStream,"%s\t%s {\n", indent.data(), ID_IKJOINT);
			DumpJointParams(joint, indentLevel+1);
			fprintf(pStream,"%s\t}\n", indent.data());
		}
	}
}

#endif