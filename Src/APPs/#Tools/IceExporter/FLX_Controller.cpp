///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains various controller-related functions.
 *	\file		FLX_Controller.cpp
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
 *	Checks if the given controller is known.
 *	\param		cont	[in] a MAX animatable
 *	\return		MAXCtrlType enumeration code
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXCtrlType IceExporter::IsKnownController(Animatable* cont)
{
	// Checkings
	if(!cont) return IMAX_CTRL_INTER_UNDEFINED;

	// Get controller ID
	udword PartA = cont->ClassID().PartA();
	udword PartB = cont->ClassID().PartB();
	if(PartB) return IMAX_CTRL_INTER_INVALID;

	switch(PartA)
	{
		case LININTERP_FLOAT_CLASS_ID:			return IMAX_CTRL_INTER_LINEAR_FLOAT;	break;
		case LININTERP_POSITION_CLASS_ID:		return IMAX_CTRL_INTER_LINEAR_POSITION;	break;
		case LININTERP_ROTATION_CLASS_ID:		return IMAX_CTRL_INTER_LINEAR_ROTATION;	break;
		case LININTERP_SCALE_CLASS_ID:			return IMAX_CTRL_INTER_LINEAR_SCALE;	break;
		case PRS_CONTROL_CLASS_ID:				return IMAX_CTRL_INTER_PRS;				break;
		case LOOKAT_CONTROL_CLASS_ID:			return IMAX_CTRL_INTER_LOOKAT;			break;

		case HYBRIDINTERP_FLOAT_CLASS_ID:		return IMAX_CTRL_INTER_HYBRID_FLOAT;	break;
		case HYBRIDINTERP_POSITION_CLASS_ID:	return IMAX_CTRL_INTER_HYBRID_POSITION;	break;
		case HYBRIDINTERP_ROTATION_CLASS_ID:	return IMAX_CTRL_INTER_HYBRID_ROTATION;	break;
		case HYBRIDINTERP_POINT3_CLASS_ID:		return IMAX_CTRL_INTER_HYBRID_POINT3;	break;
		case HYBRIDINTERP_SCALE_CLASS_ID:		return IMAX_CTRL_INTER_HYBRID_SCALE;	break;
		case HYBRIDINTERP_COLOR_CLASS_ID:		return IMAX_CTRL_INTER_HYBRID_COLOR;	break;

		case TCBINTERP_FLOAT_CLASS_ID:			return IMAX_CTRL_INTER_TCB_FLOAT;		break;
		case TCBINTERP_POSITION_CLASS_ID:		return IMAX_CTRL_INTER_TCB_POSITION;	break;
		case TCBINTERP_ROTATION_CLASS_ID:		return IMAX_CTRL_INTER_TCB_ROTATION;	break;
		case TCBINTERP_POINT3_CLASS_ID:			return IMAX_CTRL_INTER_TCB_POINT3;		break;
		case TCBINTERP_SCALE_CLASS_ID:			return IMAX_CTRL_INTER_TCB_SCALE;		break;
	}
	return IMAX_CTRL_INTER_UNKNOW;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks if the given controller is a float controller.
 *	\param		cont	[in] a MAX animatable
 *	\return		true for float controllers
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::IsFloatController(Animatable* cont)
{
	// Checkings
	if(!cont) return false;

	// Get controller ID
	udword PartA = cont->ClassID().PartA();
	udword PartB = cont->ClassID().PartB();
	if(PartB) return false;

	// Check float types
	if(PartA==LININTERP_FLOAT_CLASS_ID)		return true;
	if(PartA==HYBRIDINTERP_FLOAT_CLASS_ID)	return true;
	if(PartA==TCBINTERP_FLOAT_CLASS_ID)		return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks if the given controller is a Point controller.
 *	\param		cont	[in] a MAX animatable
 *	\return		true for Point controllers
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::IsPointController(Animatable* cont)
{
	// Checkings
	if(!cont) return false;

	// Get controller ID
	udword PartA = cont->ClassID().PartA();
	udword PartB = cont->ClassID().PartB();
	if(PartB) return false;

	// Check float types
	if(PartA==LININTERP_POSITION_CLASS_ID)		return true;
	if(PartA==HYBRIDINTERP_POSITION_CLASS_ID)	return true;
	if(PartA==HYBRIDINTERP_POINT3_CLASS_ID)		return true;
	if(PartA==HYBRIDINTERP_COLOR_CLASS_ID)		return true;
	if(PartA==TCBINTERP_POSITION_CLASS_ID)		return true;
	if(PartA==TCBINTERP_POINT3_CLASS_ID)		return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks if a given node has a visibility track.
 *	\param		node	[in] a MAX node
 *	\return		true if the node has a vistrack
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::HasVisibilityTrack(INode* node)
{
	Control* VisCtrl = node->GetVisController();
	if(VisCtrl)	return true;
	return false;
}

// NEW:

// 0: Visibility
// 1: Space Warps
// 2: Transform
// 3: Object
// 4: Material
// 5: Image Motion Blur Multiplier
// 6: Object Motion Blur On Off
//
// It's safer to use MAX accessors instead of hardcoded values.

static Control* GetVisibilityController(INode* node)
{
	if(!node)	return null;
	// This is the same as:
	// Animatable* anim = node->SubAnim(0);
	// (0 == "Visibility")
	return node->GetVisController();
}

static Control* GetTransformController(INode* node)
{
	if(!node)	return null;
	// This is the same as:
	// Animatable* anim = node->SubAnim(2);
	// (2 == "Transform")
	return node->GetTMController();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets the number of keys used by a given controller.
 *	\param		anim	[in] a MAX animatable
 *	\return		number of keys or 0 if the controller is invalid (or has no keys defined)
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword IceExporter::GetNbKeys(Animatable* anim)
{
/*
	MAXCtrlType Status = IsKnownController(anim);
	if(Status==IMAX_CTRL_INTER_UNDEFINED || Status==IMAX_CTRL_INTER_INVALID || Status==IMAX_CTRL_INTER_UNKNOW) return 0;
//	if(Status==IMAX_CTRL_INTER_UNDEFINED || Status==IMAX_CTRL_INTER_INVALID) return 0;

	// Now we're sure Anim!=null, so we can access the IKC.
	IKeyControl* ikc = GetKeyControlInterface(anim);
	if(!ikc) return 0;
*/
	if(!anim)	return 0;

	// Check the animatable is keyframable (get number of keyframes)
	int NbKeys = anim->NumKeys();
	if(NbKeys==NOT_KEYFRAMEABLE)	return 0;
	return NbKeys;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks if a given node is keyframed.
 *	\param		node		[in] a MAX node
 *	\param		is_animated	[in] true if we only want to check it's "animated"
 *	\return		true if the node is keyframed
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::IsKeyframed(INode* node, bool is_animated)
{
	// Checkings
	if(!node)	return false;

	// Get TM Controller
	Control* TMC = node->GetTMController();
	if(!TMC)	return false;

	// We only check the PRS track here. Checking pointers validity is not enough
	// since a newly created mesh *has* those tracks, even if it's not animated.
	Animatable* PosCtrl = TMC->SubAnim(IMAX_POSITION);
	Animatable* RotCtrl = TMC->SubAnim(IMAX_ROTATION);
	Animatable* SclCtrl = TMC->SubAnim(IMAX_SCALE);

	// We DON'T want to known if the controller is "known" or not, since we're going to -sample- them!
//	if(IsKnownController(PosCtrl) && GetNbKeys(PosCtrl))	return true;
//	if(IsKnownController(RotCtrl) && GetNbKeys(RotCtrl))	return true;
//	if(IsKnownController(SclCtrl) && GetNbKeys(SclCtrl))	return true;

//	if(PosCtrl && (is_animated || GetNbKeys(PosCtrl)))	return true;
//	if(RotCtrl && (is_animated || GetNbKeys(RotCtrl)))	return true;
//	if(SclCtrl && (is_animated || GetNbKeys(SclCtrl)))	return true;
	// ### Changed January, 1, 2003 : was creating PR tracks for non-animated nodes !
	// ### BUT IT WAS THE IK-WALKER FIX !!!!!!!!!
	// => now handled in a different way
	if(PosCtrl && GetNbKeys(PosCtrl))	return true;
	if(RotCtrl && GetNbKeys(RotCtrl))	return true;
	if(SclCtrl && GetNbKeys(SclCtrl))	return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks if a given node is keyframed OR HAS A KEYFRAMED PARENT.
 *	\param		node		[in] a MAX node
 *	\param		is_animated	[in] true if we only want to check it's "animated"
 *	\return		true if the node is keyframed
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::HasKeyframedAncestor(INode* node, bool is_animated)
{
	// Checkings
	if(!node)	return false;

	// Test current node
	if(IsKeyframed(node, is_animated))	return true;

	// Recurse through parent
	return HasKeyframedAncestor(node->GetParentNode(), is_animated);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets a sub-anim from a name.
 *	\param		anim	[in] a MAX animatable
 *	\param		name	[in] name of desired sub-anim
 *	\return		desired MAX sub-anim, or null if not found
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Animatable*	IceExporter::GetSubAnim(Animatable* anim, const char* name)
{
	// Checkings
	if(!anim || !name)	return null;

	// Get number of sub-anims
	udword NbAnims = anim->NumSubs();

	// Loop through sub-anims
	for(udword i=0;i<NbAnims;i++)
	{
		// Catch current sub-anim's name
		TSTR AnimName = anim->SubAnimName(i);

		// [dump test]
//		Log("%s\n", AnimName);

		// Check it's the one we're looking for
		if(strcmp(name, AnimName)==0)	return anim->SubAnim(i);
	}
	return null;
}

ControllerType IceExporter::GetControllerType(MAXCtrlType ctrlid)
{
	switch(ctrlid)
	{
		case IMAX_CTRL_INTER_LINEAR_FLOAT:		return FLX_CTRL_FLOAT;	break;
		case IMAX_CTRL_INTER_LINEAR_POSITION:	return FLX_CTRL_VECTOR;	break;
		case IMAX_CTRL_INTER_LINEAR_ROTATION:	return FLX_CTRL_QUAT;	break;
		case IMAX_CTRL_INTER_LINEAR_SCALE:		return FLX_CTRL_SCALE;	break;
		//
		case IMAX_CTRL_INTER_TCB_FLOAT:			return FLX_CTRL_FLOAT;	break;
		case IMAX_CTRL_INTER_TCB_POSITION:		return FLX_CTRL_VECTOR;	break;
		case IMAX_CTRL_INTER_TCB_ROTATION:		return FLX_CTRL_QUAT;	break;
		case IMAX_CTRL_INTER_TCB_POINT3:		return FLX_CTRL_VECTOR;	break;
		case IMAX_CTRL_INTER_TCB_SCALE:			return FLX_CTRL_SCALE;	break;
		//
		case IMAX_CTRL_INTER_HYBRID_FLOAT:		return FLX_CTRL_FLOAT;	break;
		case IMAX_CTRL_INTER_HYBRID_POSITION:	return FLX_CTRL_VECTOR;	break;
		case IMAX_CTRL_INTER_HYBRID_ROTATION:	return FLX_CTRL_QUAT;	break;
		case IMAX_CTRL_INTER_HYBRID_POINT3:		return FLX_CTRL_VECTOR;	break;
		case IMAX_CTRL_INTER_HYBRID_SCALE:		return FLX_CTRL_SCALE;	break;
		case IMAX_CTRL_INTER_HYBRID_COLOR:		return FLX_CTRL_VECTOR;	break;
		//
		case IMAX_CTRL_INTER_PRS:				return FLX_CTRL_PRS;	break;
		case IMAX_CTRL_INTER_LOOKAT:			return FLX_CTRL_VECTOR;	break;
	}
	return FLX_CTRL_UNKNOWN;
}

InterpScheme IceExporter::GetControllerScheme(MAXCtrlType ctrlid)
{
	switch(ctrlid)
	{
		case IMAX_CTRL_INTER_LINEAR_FLOAT:		return FLX_SCHEME_LINEAR;	break;
		case IMAX_CTRL_INTER_LINEAR_POSITION:	return FLX_SCHEME_LINEAR;	break;
		case IMAX_CTRL_INTER_LINEAR_ROTATION:	return FLX_SCHEME_LINEAR;	break;
		case IMAX_CTRL_INTER_LINEAR_SCALE:		return FLX_SCHEME_LINEAR;	break;
		//
		case IMAX_CTRL_INTER_TCB_FLOAT:			return FLX_SCHEME_TCB;		break;
		case IMAX_CTRL_INTER_TCB_POSITION:		return FLX_SCHEME_TCB;		break;
		case IMAX_CTRL_INTER_TCB_ROTATION:		return FLX_SCHEME_TCB;		break;
		case IMAX_CTRL_INTER_TCB_POINT3:		return FLX_SCHEME_TCB;		break;
		case IMAX_CTRL_INTER_TCB_SCALE:			return FLX_SCHEME_TCB;		break;
		//
		case IMAX_CTRL_INTER_HYBRID_FLOAT:		return FLX_SCHEME_BEZIER;	break;
		case IMAX_CTRL_INTER_HYBRID_POSITION:	return FLX_SCHEME_BEZIER;	break;
		case IMAX_CTRL_INTER_HYBRID_ROTATION:	return FLX_SCHEME_BEZIER;	break;
		case IMAX_CTRL_INTER_HYBRID_POINT3:		return FLX_SCHEME_BEZIER;	break;
		case IMAX_CTRL_INTER_HYBRID_SCALE:		return FLX_SCHEME_BEZIER;	break;
		case IMAX_CTRL_INTER_HYBRID_COLOR:		return FLX_SCHEME_BEZIER;	break;
		//
		case IMAX_CTRL_INTER_PRS:				return FLX_SCHEME_UNKNOWN;	break;
		case IMAX_CTRL_INTER_LOOKAT:			return FLX_SCHEME_UNKNOWN;	break;
	}
	return FLX_SCHEME_UNKNOWN;
}

TimeValue IceExporter::GetLastKeyframeTime(Animatable* anim)
{
	// Checkings
	if(!anim)	return 0;

	// Get number of keyframes
	udword NbKeys = GetNbKeys(anim);
	if(!NbKeys) return 0;

	udword LastKey = NbKeys-1;

	// Get control interface
	IKeyControl* ikc = GetKeyControlInterface(anim);
	if(!ikc)	return 0;

	// Get controller type
	switch(IsKnownController(anim))
	{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 1) Linear controllers
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// 1-1) Linear float
		case IMAX_CTRL_INTER_LINEAR_FLOAT:
		{
			ILinFloatKey Key;
			ikc->GetKey(LastKey, &Key);
			return Key.time;
		}
		break;

		// 1-2) Linear position
		case IMAX_CTRL_INTER_LINEAR_POSITION:
		{
			ILinPoint3Key Key;
			ikc->GetKey(LastKey, &Key);
			return Key.time;
		}
		break;

		// 1-3) Linear rotation
		case IMAX_CTRL_INTER_LINEAR_ROTATION:
		{
			ILinRotKey Key;
			ikc->GetKey(LastKey, &Key);
			return Key.time;
		}
		break;

		// 1-4) Linear scale
		case IMAX_CTRL_INTER_LINEAR_SCALE:
		{
			ILinScaleKey Key;
			ikc->GetKey(LastKey, &Key);
			return Key.time;
		}
		break;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2) TCB controllers
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// 2-1) TCB float
		case IMAX_CTRL_INTER_TCB_FLOAT:
		{
			ITCBFloatKey Key;
			ikc->GetKey(LastKey, &Key);
			return Key.time;
		}
		break;

		// 2-2) TCB position
		case IMAX_CTRL_INTER_TCB_POSITION:
		{
			ITCBPoint3Key Key;
			ikc->GetKey(LastKey, &Key);
			return Key.time;
		}
		break;

		// 2-3) TCB rotation
		case IMAX_CTRL_INTER_TCB_ROTATION:
		{
			ITCBRotKey Key;
			ikc->GetKey(LastKey, &Key);
			return Key.time;
		}
		break;

		// 2-4) TCB scale
		case IMAX_CTRL_INTER_TCB_SCALE:
		{
			ITCBScaleKey Key;
			ikc->GetKey(LastKey, &Key);
			return Key.time;
		}
		break;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 3) Bezier controllers
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// 1-1) Bezier float
		case IMAX_CTRL_INTER_HYBRID_FLOAT:
		{
			IBezFloatKey Key;
			ikc->GetKey(LastKey, &Key);
			return Key.time;
		}
		break;

		// 1-2) Bezier position
		case IMAX_CTRL_INTER_HYBRID_POSITION:
		{
			IBezPoint3Key Key;
			ikc->GetKey(LastKey, &Key);
			return Key.time;
		}
		break;

		// 1-3) Bezier rotation
		case IMAX_CTRL_INTER_HYBRID_ROTATION:
		{
			IBezQuatKey Key;
			ikc->GetKey(LastKey, &Key);
			return Key.time;
		}
		break;

		// 1-4) Bezier scale
		case IMAX_CTRL_INTER_HYBRID_SCALE:
		{
			IBezScaleKey Key;
			ikc->GetKey(LastKey, &Key);
			return Key.time;
		}
		break;

		default:
			// Unknown controller. In doubt, return max number
			return TIME_PosInfinity;
	}
	return 0;
}
