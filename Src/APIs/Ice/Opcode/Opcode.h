///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Main file for Opcode.dll.
 *	\file		Opcode.h
 *	\author		Pierre Terdiman
 *	\date		March, 20, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef OPCODE_H
#define OPCODE_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compilation messages
#if defined(OPCODE_EXPORTS)
	#pragma message("Compiling OPCODE")
#elif !defined(OPCODE_EXPORTS)
	#pragma message("Using OPCODE")
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Automatic linking
	#ifndef BAN_OPCODE_AUTOLINK
		#ifdef _DEBUG
			#pragma comment(lib, "Opcode_D.lib")
		#else
			#pragma comment(lib, "Opcode.lib")
		#endif
	#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Preprocessor
#ifndef ICE_NO_DLL
	#ifdef OPCODE_EXPORTS
		#define OPCODE_API __declspec(dllexport)
	#else
		#define OPCODE_API __declspec(dllimport)
	#endif
#else
		#define OPCODE_API
#endif

#ifndef ICECORE_H
	// ### is that really necessary ?
	#ifdef WIN32
	#include <windows.h>
	#include <windowsx.h>
	#endif // WIN32

	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>
	#include <float.h>

	#ifndef ASSERT
		#define	ASSERT	assert
	#endif

	#define	Log
	#define	SetIceError		false
	#define	EC_OUTOFMEMORY	"Out of memory"
	#define	Alignment

	#include <IceCore\IcePreprocessor.h>
	#include <IceCore\IceTypes.h>
	#include <IceCore\IceFPU.h>
	#include <IceCore\IceMemoryMacros.h>
	namespace IceCore
	{
		#include <IceCore\IceContainer.h>
	}
	using namespace IceCore;
#endif

#ifndef ICEMATHS_H
	#include <Math.h>
	#define ICEMATHS_API	OPCODE_API
	namespace IceMaths
	{
		#include <IceMaths\IcePoint.h>
		#include <IceMaths\IceHPoint.h>
		#include <IceMaths\IceMatrix3x3.h>
		#include <IceMaths\IceMatrix4x4.h>
		#include <IceMaths\IcePlane.h>
		#include <IceMaths\IceRay.h>
	}
	using namespace IceMaths;
#endif

#ifndef MESHMERIZER_H
	#define MESHMERIZER_API	OPCODE_API
	namespace Meshmerizer
	{
		#include <Meshmerizer\IceTriangle.h>
		#include <Meshmerizer\IceAABB.h>
		#include <Meshmerizer\IceOBB.h>
		#include <Meshmerizer\IceBoundingSphere.h>
	}
	using namespace Meshmerizer;
#endif

#undef new
#undef delete

	namespace Opcode
	{
		// Bulk-of-the-work
		#include "OPC_Settings.h"
		#include "OPC_Allocator.h"
		#include "OPC_Common.h"
		#include "OPC_MeshInterface.h"
		#include "OPC_TreeCoherence.h"
		// Builders
		#include "OPC_TreeBuilders.h"
		// Trees
		#include "OPC_AABBTree.h"
		#include "OPC_OptimizedTree.h"
		// Models
		#include "OPC_BaseModel.h"
		#include "OPC_Model.h"
		#include "OPC_HybridModel.h"
		// Colliders
		#include "OPC_Collider.h"
		#include "OPC_VolumeCollider.h"
		#include "OPC_TreeCollider.h"
		#include "OPC_RayCollider.h"
		#include "OPC_SphereCollider.h"
		#include "OPC_OBBCollider.h"
		#include "OPC_AABBCollider.h"
		#include "OPC_LSSCollider.h"
		#include "OPC_PlanesCollider.h"
		// Distance queries
		#include "OPC_Distance.h"
		#include "OPC_PointDistance.h"
		// Usages
		#include "OPC_Picking.h"
		#include "OPC_InOutTest.h"
		#include "OPC_BoxCamera.h"
		#include "OPC_SphereCamera.h"
		// Sweep-and-prune
		#include "OPC_BoxPruning.h"
		#include "OPC_SweepAndPrune.h"

		FUNCTION OPCODE_API bool InitOpcode();
		FUNCTION OPCODE_API bool CloseOpcode();
	}

#endif // OPCODE_H
