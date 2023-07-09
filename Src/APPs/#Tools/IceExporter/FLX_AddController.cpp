///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a controller.
 *	\file		FLX_AddController.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

//if( node->GetTMController()->GetRollController() &&
//    node->GetTMController()->GetRollController()->NumKeys() )

/*

  y'a meme pas besoin d'accéder aux controllers, y'a qu'à sampler 




			if(Gl->GetHotSpotControl() && Gl->GetHotSpotControl()->SubAnim(0))								// You can use GetFalloffControl() or GetColorControl() instead
			{
				mLights
					.Store((char)1)
					.Store(AddController(Gl->GetHotSpotControl()->SubAnim(0)->SubAnim(IMAX_COLOR)))
					.Store(AddController(Gl->GetHotSpotControl()->SubAnim(0)->SubAnim(IMAX_MULTIPLIER)))
					.Store(AddController(Gl->GetHotSpotControl()->SubAnim(0)->SubAnim(IMAX_CONTRAST)))
					.Store(AddController(Gl->GetHotSpotControl()->SubAnim(0)->SubAnim(IMAX_DIFFUSE_SOFTEN)))
					.Store(AddController(Gl->GetHotSpotControl()->SubAnim(0)->SubAnim(IMAX_MAP_SIZE)))
					.Store(AddController(Gl->GetHotSpotControl()->SubAnim(0)->SubAnim(IMAX_MAP_BIAS)))
					.Store(AddController(Gl->GetHotSpotControl()->SubAnim(0)->SubAnim(IMAX_MAP_RANGE)))
					.Store(AddController(Gl->GetHotSpotControl()->SubAnim(0)->SubAnim(IMAX_RAY_BIAS)))
					.Store(AddController(Gl->GetHotSpotControl()->SubAnim(0)->SubAnim(IMAX_ATTEN_NEAR_START)))
					.Store(AddController(Gl->GetHotSpotControl()->SubAnim(0)->SubAnim(IMAX_ATTEN_NEAR_END)))
					.Store(AddController(Gl->GetHotSpotControl()->SubAnim(0)->SubAnim(IMAX_ATTEN_FAR_START)))
					.Store(AddController(Gl->GetHotSpotControl()->SubAnim(0)->SubAnim(IMAX_ATTEN_FAR_END)));
			}

*/

using namespace IceExporter;








///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Creates sampled PRS data.
 *	\param		node	[in] the MAX node
 *	\param		ctype	[in] the controller's type (PR or PRS)
 *	\param		data	[out] an array to store the sampled data
 *	\return		number of samples
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword MAXConverter::CreateSampledPRSData(INode* node, ControllerType ctype, CustomArray& data)
{
	// Checkings
	if(!node)	return 0;

	// Number of samples
	udword Nb = 0;

	// Catch sampling rate
	long SamplingRate = mSettings->GetSamplingRate();

	INIT_PERCENTS

	// Compute last keyframe time
	TimeValue EndTime = mTimeInfo.mEndTime;

	if(0)	// Won't sample IK or path-constraint, same bug as before with the IK walker!
	{
		TimeValue LastTime0 = GetLastKeyframeTime(node->GetTMController()->SubAnim(IMAX_POSITION));
		TimeValue LastTime1 = GetLastKeyframeTime(node->GetTMController()->SubAnim(IMAX_ROTATION));
		TimeValue LastTime2 = GetLastKeyframeTime(node->GetTMController()->SubAnim(IMAX_SCALE));
		if(LastTime0==TIME_PosInfinity)	LastTime0 = EndTime;
		if(LastTime1==TIME_PosInfinity)	LastTime1 = EndTime;
		if(LastTime2==TIME_PosInfinity)	LastTime2 = EndTime;
		EndTime = MAXMAX(LastTime0, LastTime1, LastTime2);
	}

	// While we're at it, we'll check sampling was worth it. If all samples are similar, it's not !
	PRS PreviousPRS;
	bool WorthIt = false;
	// Loop through frames - use long since time can be negative
	for(long i=mTimeInfo.mStartTime;i<=EndTime;i+=mTimeInfo.mDeltaTime * SamplingRate)
	{
		UPDATE_PERCENTS(i, EndTime, "exporting PRS data... ")
		// Convert PRS data for current frame
		PRS prs;
		ConvertPRS(node, i, prs);
		// Dump position
		data.Store(prs.mPos.x).Store(prs.mPos.y).Store(prs.mPos.z);
		// Dump rotation
		data.Store(prs.mRot.p.x).Store(prs.mRot.p.y).Store(prs.mRot.p.z).Store(prs.mRot.w);
		// Dump scale
		if(ctype==FLX_CTRL_PRS)	data.Store(prs.mScale.x).Store(prs.mScale.y).Store(prs.mScale.z);

		// Compare to previous sample
		if(i!=mTimeInfo.mStartTime && !WorthIt)
		{
			if(prs!=PreviousPRS)	WorthIt = true;
		}
		PreviousPRS = prs;

		// One more time
		Nb++;
	}
	// Here, WorthIt hasn't been updated if there's a single sample. But since
	// a single sample has already been captured in the static scene, it's indeed
	// not worth creating a controller. So :
	return WorthIt ? Nb : 0;
}

bool MAXConverter::ExportKeyframes(Animatable* anim, const char* name, const char* log_name, ResourceDescriptor* exported_node)
{
	if(!anim || !name || !log_name || !exported_node)	return false;

	// Dump the keyframes
	CustomArray KeyframedData;
	MAXCtrlType Type;
	udword Nb = CreateKeyframedData(anim, KeyframedData, Type);
	if(!Nb)	return false;

	// Create a new ControllerInfo structure. Not shared => create it, export it, destroy it
	ControllerDescriptor Ctrl;
	Ctrl.mField		= name;
	Ctrl.mObjectID	= mStats.mNbControllers++;
	Ctrl.mOwnerID	= exported_node->mObjectID;
	Ctrl.mOwnerType	= exported_node->mType;

	// Create ControllerData
	KeyframeDataCreate kdc;
	kdc.Mode			= CTRL_KEYFRAMES;
	kdc.Type			= GetControllerType(Type);
	kdc.NbKeyframes		= Nb;
	kdc.Scheme			= GetControllerScheme(Type);

	Ctrl.mData	= new KeyframeData;	CHECKALLOC(Ctrl.mData);
	if(Ctrl.mData->Init(&kdc))
	{
		CopyMemory(((KeyframeData*)Ctrl.mData)->mKeyframes, KeyframedData.Collapse(), KeyframedData.GetSize());
		_DUMP_CONTROLLER(log_name)
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Creates keyframed PRS data.
 *	\param		anim		[in] a MAX animatable
 *	\param		data		[out] an array to store the keyframed data
 *	\param		ctrltype	[out] controlled type
 *	\return		number of keyframes
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword MAXConverter::CreateKeyframedData(Animatable* anim, CustomArray& data, MAXCtrlType& ctrltype)
{
	// Checkings
	if(!anim)	return 0;

	// Get number of keyframes
	udword NbKeys = GetNbKeys(anim);
	if(!NbKeys) return 0;

	// Get control interface
	IKeyControl* ikc = GetKeyControlInterface(anim);
	if(!ikc)	return 0;

	// Get controller type
	ctrltype = IsKnownController(anim);
/*
Linear	|	Flex ID							|	MAX ID							|	MAX Class
====================================================================================================================
		|	IMAX_CTRL_INTER_LINEAR_FLOAT	|	LININTERP_FLOAT_CLASS_ID		|	ILinFloatKey
		|	IMAX_CTRL_INTER_LINEAR_POSITION	|	LININTERP_POSITION_CLASS_ID		|	ILinPoint3Key
		|	IMAX_CTRL_INTER_LINEAR_ROTATION	|	LININTERP_ROTATION_CLASS_ID		|	ILinRotKey
		|	IMAX_CTRL_INTER_LINEAR_SCALE	|	LININTERP_SCALE_CLASS_ID		|	ILinScaleKey
		|									|									|
TCB		|	Flex ID							|	MAX ID							|	MAX Class
====================================================================================================================
		|	IMAX_CTRL_INTER_TCB_FLOAT		|	TCBINTERP_FLOAT_CLASS_ID		|	ITCBFloatKey
		|	IMAX_CTRL_INTER_TCB_POSITION	|	TCBINTERP_POSITION_CLASS_ID		|	ITCBPoint3Key
		|	IMAX_CTRL_INTER_TCB_ROTATION	|	TCBINTERP_ROTATION_CLASS_ID		|	ITCBRotKey
		|	IMAX_CTRL_INTER_TCB_SCALE		|	TCBINTERP_SCALE_CLASS_ID		|	ITCBScaleKey
		|									|									|
Bezier	|	Flex ID							|	MAX ID							|	MAX Class
====================================================================================================================
		|	IMAX_CTRL_INTER_HYBRID_FLOAT	|	HYBRIDINTERP_FLOAT_CLASS_ID		|	IBezFloatKey
		|	IMAX_CTRL_INTER_HYBRID_POSITION	|	HYBRIDINTERP_POSITION_CLASS_ID	|	IBezPoint3Key
		|	IMAX_CTRL_INTER_HYBRID_ROTATION	|	HYBRIDINTERP_ROTATION_CLASS_ID	|	IBezQuatKey
		|	IMAX_CTRL_INTER_HYBRID_SCALE	|	HYBRIDINTERP_SCALE_CLASS_ID		|	IBezScaleKey
*/
	switch(ctrltype)
	{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 1) Linear controllers
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// 1-1) Linear float
		case IMAX_CTRL_INTER_LINEAR_FLOAT:
		{
			// Loop through keys
			for(udword i=0;i<NbKeys;i++)
			{
				// Get MAX key
				ILinFloatKey Key;
				ikc->GetKey(i, &Key);

				// Dump key
				data.Store((udword)Key.time)
					.Store((udword)Key.flags)
					//
					.Store(Key.val);
			}
			return NbKeys;
		}
		break;

		// 1-2) Linear position
		case IMAX_CTRL_INTER_LINEAR_POSITION:
		{
			// Loop through keys
			for(udword i=0;i<NbKeys;i++)
			{
				// Get MAX key
				ILinPoint3Key Key;
				ikc->GetKey(i, &Key);

				// Transform key [Flexporter 1.16]
				if(mSettings->mConvertToD3D)
				{
					TSwap(Key.val.y, Key.val.z);
				}

				// Dump key
				data.Store((udword)Key.time)
					.Store((udword)Key.flags)
					//
					.Store(Key.val.x).Store(Key.val.y).Store(Key.val.z);
			}
			return NbKeys;
		}
		break;

		// 1-3) Linear rotation
		case IMAX_CTRL_INTER_LINEAR_ROTATION:
		{
			// Loop through keys
			for(udword i=0;i<NbKeys;i++)
			{
				// Get MAX key
				ILinRotKey Key;
				ikc->GetKey(i, &Key);

				// AA to Quat
				::Quat Q = Key.val;

				// Transform key [Flexporter 1.16]
				if(mSettings->mConvertToD3D)
				{
					TSwap(Q.y, Q.z);
				}

				// Dump key
				data.Store((udword)Key.time)
					.Store((udword)Key.flags)
					//
					.Store(Q.x).Store(Q.y).Store(Q.z).Store(Q.w);
			}
			return NbKeys;
		}
		break;

		// 1-4) Linear scale
		case IMAX_CTRL_INTER_LINEAR_SCALE:
		{
			// Loop through keys
			for(udword i=0;i<NbKeys;i++)
			{
				// Get MAX key
				ILinScaleKey Key;
				ikc->GetKey(i, &Key);

				// AA to Quat
				::Quat Q = Key.val.q;

				// Transform key [Flexporter 1.16]
				if(mSettings->mConvertToD3D)
				{
					TSwap(Key.val.s.y, Key.val.s.z);
					TSwap(Q.y, Q.z);
				}

				// Dump key
				data.Store((udword)Key.time)
					.Store((udword)Key.flags)
					//
					.Store(Key.val.s.x).Store(Key.val.s.y).Store(Key.val.s.z)
					.Store(Q.x).Store(Q.y).Store(Q.z).Store(Q.w);
			}
			return NbKeys;
		}
		break;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2) TCB controllers
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// 2-1) TCB float
		case IMAX_CTRL_INTER_TCB_FLOAT:
		{
			// Loop through keys
			for(udword i=0;i<NbKeys;i++)
			{
				// Get MAX key
				ITCBFloatKey Key;
				ikc->GetKey(i, &Key);

				// Dump key
				data.Store((udword)Key.time)
					.Store((udword)Key.flags)
					//
					.Store(Key.tens)
					.Store(Key.cont)
					.Store(Key.bias)
					.Store(Key.easeIn)
					.Store(Key.easeOut)
					//
					.Store(Key.val);
			}
			return NbKeys;
		}
		break;

		// 2-2) TCB position
		case IMAX_CTRL_INTER_TCB_POSITION:
		{
			// Loop through keys
			for(udword i=0;i<NbKeys;i++)
			{
				// Get MAX key
				ITCBPoint3Key Key;
				ikc->GetKey(i, &Key);

				// Transform key [Flexporter 1.16]
				if(mSettings->mConvertToD3D)
				{
					TSwap(Key.val.y, Key.val.z);
				}

				// Dump key
				data.Store((udword)Key.time)
					.Store((udword)Key.flags)
					//
					.Store(Key.tens)
					.Store(Key.cont)
					.Store(Key.bias)
					.Store(Key.easeIn)
					.Store(Key.easeOut)
					//
					.Store(Key.val.x).Store(Key.val.y).Store(Key.val.z);
			}
			return NbKeys;
		}
		break;

		// 2-3) TCB rotation
		case IMAX_CTRL_INTER_TCB_ROTATION:
		{
			// Loop through keys
			for(udword i=0;i<NbKeys;i++)
			{
				// Get MAX key
				ITCBRotKey Key;
				ikc->GetKey(i, &Key);

				// AA to Quat
				::Quat Q = Key.val;

				// Transform key [Flexporter 1.16]
				if(mSettings->mConvertToD3D)
				{
					TSwap(Q.y, Q.z);
				}

				// Dump key
				data.Store((udword)Key.time)
					.Store((udword)Key.flags)
					//
					.Store(Key.tens)
					.Store(Key.cont)
					.Store(Key.bias)
					.Store(Key.easeIn)
					.Store(Key.easeOut)
					//
					.Store(Q.x).Store(Q.y).Store(Q.z).Store(Q.w);
			}
			return NbKeys;
		}
		break;

		// 2-4) TCB scale
		case IMAX_CTRL_INTER_TCB_SCALE:
		{
			// Loop through keys
			for(udword i=0;i<NbKeys;i++)
			{
				// Get MAX key
				ITCBScaleKey Key;
				ikc->GetKey(i, &Key);

				// AA to Quat
				::Quat Q = Key.val.q;

				// Transform key [Flexporter 1.16]
				if(mSettings->mConvertToD3D)
				{
					TSwap(Key.val.s.y, Key.val.s.z);
					TSwap(Q.y, Q.z);
				}

				// Dump key
				data.Store((udword)Key.time)
					.Store((udword)Key.flags)
					//
					.Store(Key.tens)
					.Store(Key.cont)
					.Store(Key.bias)
					.Store(Key.easeIn)
					.Store(Key.easeOut)
					//
					.Store(Key.val.s.x).Store(Key.val.s.y).Store(Key.val.s.z)
					.Store(Q.x).Store(Q.y).Store(Q.z).Store(Q.w);
			}
			return NbKeys;
		}
		break;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 3) Bezier controllers
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// 1-1) Bezier float
		case IMAX_CTRL_INTER_HYBRID_FLOAT:
		{
			// Loop through keys
			for(udword i=0;i<NbKeys;i++)
			{
				// Get MAX key
				IBezFloatKey Key;
				ikc->GetKey(i, &Key);

				// Dump key
				data.Store((udword)Key.time)
					.Store((udword)Key.flags)
					//
					.Store(Key.intan).Store(Key.outtan)
					.Store(Key.val);
			}
			return NbKeys;
		}
		break;

		// 1-2) Bezier position
		case IMAX_CTRL_INTER_HYBRID_POSITION:
		{
			// Loop through keys
			for(udword i=0;i<NbKeys;i++)
			{
				// Get MAX key
				IBezPoint3Key Key;
				ikc->GetKey(i, &Key);

				// Transform key [Flexporter 1.16]
				if(mSettings->mConvertToD3D)
				{
					TSwap(Key.intan.y, Key.intan.z);
					TSwap(Key.outtan.y, Key.outtan.z);
					TSwap(Key.val.y, Key.val.z);
				}

				// Dump key
				data.Store((udword)Key.time)
					.Store((udword)Key.flags)
					//
					.Store(Key.intan.x).Store(Key.intan.y).Store(Key.intan.z)
					.Store(Key.outtan.x).Store(Key.outtan.y).Store(Key.outtan.z)
					.Store(Key.val.x).Store(Key.val.y).Store(Key.val.z);
			}
			return NbKeys;
		}
		break;

		// 1-3) Bezier rotation
		case IMAX_CTRL_INTER_HYBRID_ROTATION:
		{
			// Loop through keys
			for(udword i=0;i<NbKeys;i++)
			{
				// Get MAX key
				IBezQuatKey Key;
				ikc->GetKey(i, &Key);

				// AA to Quat
				::Quat Q = Key.val;

				// Transform key [Flexporter 1.16]
				if(mSettings->mConvertToD3D)
				{
					TSwap(Q.y, Q.z);
				}

				// Dump key
				data.Store((udword)Key.time)
					.Store((udword)Key.flags)
					//
					.Store(Q.x).Store(Q.y).Store(Q.z).Store(Q.w);
			}
			return NbKeys;
		}
		break;

		// 1-4) Bezier scale
		case IMAX_CTRL_INTER_HYBRID_SCALE:
		{
			// Loop through keys
			for(udword i=0;i<NbKeys;i++)
			{
				// Get MAX key
				IBezScaleKey Key;
				ikc->GetKey(i, &Key);

				// AA to Quat
				::Quat Q = Key.val.q;

				// Transform key [Flexporter 1.16]
				if(mSettings->mConvertToD3D)
				{
					TSwap(Key.intan.y, Key.intan.z);
					TSwap(Key.outtan.y, Key.outtan.z);
					TSwap(Key.val.s.y, Key.val.s.z);
					TSwap(Q.y, Q.z);
				}

				// Dump key
				data.Store((udword)Key.time)
					.Store((udword)Key.flags)
					//
					.Store(Key.intan.x).Store(Key.intan.y).Store(Key.intan.z)
					.Store(Key.outtan.x).Store(Key.outtan.y).Store(Key.outtan.z)
					.Store(Key.val.s.x).Store(Key.val.s.y).Store(Key.val.s.z)
					.Store(Q.x).Store(Q.y).Store(Q.z).Store(Q.w);
			}
			return NbKeys;
		}
		break;

		default:
			mArrayLog.StoreASCII("WARNING: Found unsupported controller.\n");
			break;
	}
	return 0;
}
