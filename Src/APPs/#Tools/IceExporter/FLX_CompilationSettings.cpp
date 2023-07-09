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
 *	\file		FLX_CompilationSettings.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 10, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

#ifdef FLEXPORTER_MAX3
	#pragma comment (lib, "x:\\MAX\\3D STUDIO MAX 3 SDK\\LIB\\MESH.LIB")
	#pragma comment (lib, "x:\\MAX\\3D STUDIO MAX 3 SDK\\LIB\\CORE.LIB")
	#pragma comment (lib, "x:\\MAX\\3D STUDIO MAX 3 SDK\\LIB\\GEOM.LIB")
	#pragma comment (lib, "x:\\MAX\\3D STUDIO MAX 3 SDK\\LIB\\MAXUTIL.LIB")
	#pragma comment (lib, "x:\\MAX\\3D STUDIO MAX 3 SDK\\LIB\\BMM.LIB")
#endif

#ifdef FLEXPORTER_MAX4
	#pragma comment (lib, "x:\\MAX\\MAX4SDK\\LIB\\MESH.LIB")
	#pragma comment (lib, "x:\\MAX\\MAX4SDK\\LIB\\CORE.LIB")
	#pragma comment (lib, "x:\\MAX\\MAX4SDK\\LIB\\GEOM.LIB")
	#pragma comment (lib, "x:\\MAX\\MAX4SDK\\LIB\\MAXUTIL.LIB")
	#pragma comment (lib, "x:\\MAX\\MAX4SDK\\LIB\\BMM.LIB")

	#pragma comment (lib, "X:\\MAX\\MAX4SDK\\lib\\paramblk2.lib")
#endif

#ifdef FLEXPORTER_MAX5
	#pragma comment (lib, "x:\\MAX\\MAX5SDK\\LIB\\MESH.LIB")
	#pragma comment (lib, "x:\\MAX\\MAX5SDK\\LIB\\CORE.LIB")
	#pragma comment (lib, "x:\\MAX\\MAX5SDK\\LIB\\GEOM.LIB")
	#pragma comment (lib, "x:\\MAX\\MAX5SDK\\LIB\\MAXUTIL.LIB")
	#pragma comment (lib, "x:\\MAX\\MAX5SDK\\LIB\\BMM.LIB")

	#pragma comment (lib, "X:\\MAX\\MAX5SDK\\lib\\paramblk2.lib")
#endif

#ifdef FLEXPORTER_MAX6
	#pragma comment (lib, "x:\\MAX\\MAX6SDK\\LIB\\MESH.LIB")
	#pragma comment (lib, "x:\\MAX\\MAX6SDK\\LIB\\CORE.LIB")
	#pragma comment (lib, "x:\\MAX\\MAX6SDK\\LIB\\GEOM.LIB")
	#pragma comment (lib, "x:\\MAX\\MAX6SDK\\LIB\\MAXUTIL.LIB")
	#pragma comment (lib, "x:\\MAX\\MAX6SDK\\LIB\\BMM.LIB")

	#pragma comment (lib, "X:\\MAX\\MAX6SDK\\lib\\paramblk2.lib")
#endif
