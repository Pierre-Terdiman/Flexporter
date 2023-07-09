///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2003 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter SDK v1.2
 *	\file		FlexporterSDK.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __FLEXPORTER_SDK__
#define __FLEXPORTER_SDK__

	#ifndef __ICEMESHBUILDERRESULTS_H__
		#define MESHMERIZER_API
		#include "IceMeshBuilderResults.h"
	#endif

	#ifndef __ICEMESHBUILDER2_H__
		class MeshBuilder2;
	#endif

	#ifndef __ICEPOINT_H__
		class Point;
	#endif

	#ifndef __ICEPRS_H__
		class PRS;
	#endif

	#ifndef __ICEMATRIX4X4_H__
		class Matrix4x4;
	#endif

//! Include Flexporter headers
	#include "FlexporterSDKVersion.h"
	#include "FLX_AddPRS.h"
	#include "FLX_Keys.h"
	#include "FLX_TimeInfo.h"
	#include "FLX_Units.h"
	#include "FLX_ExportSettings.h"
	#include "FLX_AddJoint.h"
	#include "FLX_BaseInfo.h"
	#include "FLX_CropValues.h"
	#include "FLX_AddCamera.h"
	#include "FLX_AddLight.h"
	#include "FLX_AddMaterial.h"
	#include "FLX_AddTexture.h"
	#include "FLX_AddPrimitive.h"
	#include "FLX_AddMesh.h"
	#include "FLX_AddController.h"
	#include "FLX_AddHelper.h"
	#include "FLX_AddMotion.h"
	#include "FLX_AddShape.h"
	#include "FLX_AddSpaceWarp.h"
	#include "FLX_BaseFormat.h"

	#include "IceCharacterStudioNodes.h"

	#define DLLEXPORT __declspec(dllexport)

// Exported functions

	//! A character string describing the exporter. Appears in Flexporter's Options panel.
	FUNCTION DLLEXPORT const char*		ExporterDescription();

	//! The extension used for the plug-in's format. (e.g. "txt" for default ascii exporter)
	FUNCTION DLLEXPORT const char*		FormatExtension();

	//! Returns a pointer to the unique instance of an exporter.
	FUNCTION DLLEXPORT ExportFormat*	GetExporter();

	//! Returns the default settings for the exporter.
	FUNCTION DLLEXPORT ExportSettings*	GetDefaultSettings();

	//! Returns the enabled/disabled status for all settings
	FUNCTION DLLEXPORT Enabled*			GetEnabledSettings();

	//! Returns FLEXPORTER_VERSION. (i.e. the SDK version, not Flexporter's version)
	FUNCTION DLLEXPORT int				Version();

#endif // __FLEXPORTER_SDK__
