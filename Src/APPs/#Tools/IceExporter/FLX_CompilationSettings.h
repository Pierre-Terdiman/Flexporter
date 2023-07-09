///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains Flexporter's compilation settings.
 *	\file		FLX_CompilationSettings.h
 *	\author		Pierre Terdiman
 *	\date		April, 10, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define DISCARD_PLUGINS		//!< Debug purpose, do not touch
#define DYNAMIC_MFC			//!< Debug purpose, do not touch

//#define FLEXPORTER_MAX3		//!< Compiles for MAX 3
//#define FLEXPORTER_MAX4		//!< Compiles for MAX 4
	#define CS31
//	#define CS32
#define FLEXPORTER_MAX5		//!< Compiles for MAX 5
//#define FLEXPORTER_MAX6		//!< Compiles for MAX 6 (experimental)

// Should create these:
// C:\3dsmax3_1\Plugins\IceExporter.dlu
// C:\3dsmax4\Plugins\IceExporter.dlu
// C:\3dsmax5\Plugins\IceExporter.dlu

#ifdef STRICT
#undef STRICT
#endif

#ifdef FLEXPORTER_MAX3
	// MAX 3 includes
	#include "x:\MAX\3D STUDIO MAX 3 SDK\INCLUDE\Max.h"			// 3D Studio MAX generic include
	#include "x:\MAX\3D STUDIO MAX 3 SDK\INCLUDE\utilapi.h"		// For the UtilityObj base class
	#include "x:\MAX\3D STUDIO MAX 3 SDK\INCLUDE\Decomp.h"		// Needed for decomp_affine
	#include "x:\MAX\3D STUDIO MAX 3 SDK\INCLUDE\StdMat.h"		// Needed for materials, BitmapTex, etc
	#include "x:\MAX\3D STUDIO MAX 3 SDK\INCLUDE\IParamb2.h"	// For cropping values in Shiva
	#include "x:\MAX\3D STUDIO MAX 3 SDK\INCLUDE\ModStack.h"	// For Physique
	// For Gizmo helpers
	#include "x:\MAX\3D STUDIO MAX 3 SDK\INCLUDE\Gizmo.h"
	#include "x:\MAX\3D STUDIO MAX 3 SDK\INCLUDE\GizmoImp.h"
	// For primitives
	#include "x:\MAX\3D STUDIO MAX 3 SDK\INCLUDE\SimpObj.h"

	// Character Studio 2 includes
	#include "BipExp2.h"
	#include "PhyExp2.h"

	// Skin modifier
	#include "ISkin3.h"

	// IK
	#include "x:\MAX\3D STUDIO MAX 3 SDK\INCLUDE\interpik.h"
#endif

#ifdef FLEXPORTER_MAX4
	// MAX 4 includes
	#include "x:\MAX\MAX4SDK\INCLUDE\Max.h"			// 3D Studio MAX generic include
	#include "x:\MAX\MAX4SDK\INCLUDE\utilapi.h"		// For the UtilityObj base class
	#include "x:\MAX\MAX4SDK\INCLUDE\Decomp.h"		// Needed for decomp_affine
	#include "x:\MAX\MAX4SDK\INCLUDE\StdMat.h"		// Needed for materials, BitmapTex, etc
	#include "x:\MAX\MAX4SDK\INCLUDE\IParamb2.h"	// For cropping values in Shiva
	#include "x:\MAX\MAX4SDK\INCLUDE\bmmlib.h"		// For BMMExport used in bitmap.h
	#include "x:\MAX\MAX4SDK\INCLUDE\bitmap.h"		// For the bitmap class
	#include "x:\MAX\MAX4SDK\INCLUDE\ModStack.h"	// For Physique
	// For Gizmo helpers
	#include "x:\MAX\MAX4SDK\INCLUDE\Gizmo.h"
	#include "x:\MAX\MAX4SDK\INCLUDE\GizmoImp.h"
	// For primitives
	#include "x:\MAX\MAX4SDK\INCLUDE\SimpObj.h"

	// Character Studio 3 includes
#ifdef CS31
	#include "BipExp3.h"
	#include "PhyExp3.h"
#endif
	// Character Studio 3.2 includes
#ifdef CS32
	#include "BipExp4.h"
	#include "PhyExp4.h"
#endif

	// Skin modifier
	#include "ISkin4.h"

	// IK
	#include "x:\MAX\MAX4SDK\INCLUDE\IIKSys.h"
	#include "x:\MAX\MAX4SDK\INCLUDE\interpik.h"

	// Custom material
//	#include "X:\MAX\MAX4SDK\Include\Max.h"
	#include "X:\MAX\MAX4SDK\Include\istdplug.h"
//	#include "X:\MAX\MAX4SDK\Include\iparamb2.h"
	#include "X:\MAX\MAX4SDK\Include\iparamm2.h"
//	#include "X:\MAX\MAX4SDK\Include\utilapi.h"
	#include "X:\MAX\MAX4SDK\Include\CustAttrib.h"
	#include "X:\MAX\MAX4SDK\Include\ICustAttribContainer.h"
	#include "X:\MAX\MAX4SDK\Include\IMtlEdit.h"

#endif

#ifdef FLEXPORTER_MAX5
	// MAX 5 includes
	#include "x:\MAX\MAX5SDK\INCLUDE\Max.h"			// 3D Studio MAX generic include
	#include "x:\MAX\MAX5SDK\INCLUDE\utilapi.h"		// For the UtilityObj base class
	#include "x:\MAX\MAX5SDK\INCLUDE\Decomp.h"		// Needed for decomp_affine
	#include "x:\MAX\MAX5SDK\INCLUDE\StdMat.h"		// Needed for materials, BitmapTex, etc
	#include "x:\MAX\MAX5SDK\INCLUDE\IParamb2.h"	// For cropping values in Shiva
	#include "x:\MAX\MAX5SDK\INCLUDE\bmmlib.h"		// For BMMExport used in bitmap.h
	#include "x:\MAX\MAX5SDK\INCLUDE\bitmap.h"		// For the bitmap class
	#include "x:\MAX\MAX5SDK\INCLUDE\ModStack.h"	// For Physique
	// For Gizmo helpers
	#include "x:\MAX\MAX5SDK\INCLUDE\Gizmo.h"
	#include "x:\MAX\MAX5SDK\INCLUDE\GizmoImp.h"
	// For primitives
	#include "x:\MAX\MAX5SDK\INCLUDE\SimpObj.h"

	// Character Studio 3 includes
//	#include "BipExp3.h"
//	#include "PhyExp3.h"
	// Character Studio 3.2 includes
	#include "BipExp4.h"
	#include "PhyExp4.h"

	// Skin modifier
	#include "ISkin4.h"

	// IK
	#include "x:\MAX\MAX5SDK\INCLUDE\IIKSys.h"
	#include "x:\MAX\MAX5SDK\INCLUDE\interpik.h"

	// Custom material
	#include "X:\MAX\MAX5SDK\Include\istdplug.h"
//	#include "X:\MAX\MAX5SDK\Include\iparamb2.h"
	#include "X:\MAX\MAX5SDK\Include\iparamm2.h"
//	#include "X:\MAX\MAX5SDK\Include\utilapi.h"
	#include "X:\MAX\MAX5SDK\Include\CustAttrib.h"
	#include "X:\MAX\MAX5SDK\Include\ICustAttribContainer.h"
	#include "X:\MAX\MAX5SDK\Include\IMtlEdit.h"
#endif

#ifdef FLEXPORTER_MAX6
	// MAX 6 includes
	#include "x:\MAX\MAX6SDK\INCLUDE\Max.h"			// 3D Studio MAX generic include
	#include "x:\MAX\MAX6SDK\INCLUDE\utilapi.h"		// For the UtilityObj base class
	#include "x:\MAX\MAX6SDK\INCLUDE\Decomp.h"		// Needed for decomp_affine
	#include "x:\MAX\MAX6SDK\INCLUDE\StdMat.h"		// Needed for materials, BitmapTex, etc
	#include "x:\MAX\MAX6SDK\INCLUDE\IParamb2.h"	// For cropping values in Shiva
	#include "x:\MAX\MAX6SDK\INCLUDE\bmmlib.h"		// For BMMExport used in bitmap.h
	#include "x:\MAX\MAX6SDK\INCLUDE\bitmap.h"		// For the bitmap class
	#include "x:\MAX\MAX6SDK\INCLUDE\ModStack.h"	// For Physique
	// For Gizmo helpers
	#include "x:\MAX\MAX6SDK\INCLUDE\Gizmo.h"
	#include "x:\MAX\MAX6SDK\INCLUDE\GizmoImp.h"
	// For primitives
	#include "x:\MAX\MAX6SDK\INCLUDE\SimpObj.h"

	// Character Studio 3 includes
//	#include "BipExp3.h"
//	#include "PhyExp3.h"
	// Character Studio 3.2 includes
	#include "BipExp6.h"
	#include "PhyExp6.h"

	// Skin modifier
	#include "ISkin6.h"

	// IK
	#include "x:\MAX\MAX6SDK\INCLUDE\IIKSys.h"
	#include "x:\MAX\MAX6SDK\INCLUDE\interpik.h"

	// Custom material
	#include "X:\MAX\MAX6SDK\Include\istdplug.h"
//	#include "X:\MAX\MAX6SDK\Include\iparamb2.h"
	#include "X:\MAX\MAX6SDK\Include\iparamm2.h"
//	#include "X:\MAX\MAX6SDK\Include\utilapi.h"
	#include "X:\MAX\MAX6SDK\Include\CustAttrib.h"
	#include "X:\MAX\MAX6SDK\Include\ICustAttribContainer.h"
	#include "X:\MAX\MAX6SDK\Include\IMtlEdit.h"

#endif
