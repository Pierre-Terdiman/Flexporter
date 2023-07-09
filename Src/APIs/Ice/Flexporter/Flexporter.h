///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLEXPORTER_H
#define FLEXPORTER_H

	#include "FlexporterSettings.h"

	namespace Flexporter
	{
		// Flexporter SDK
		#include <Flexporter\Sdk\IceCharacterStudioNodes.h>
		#include <Flexporter\Sdk\FlexporterSDKVersion.h>
		#include <Flexporter\Sdk\FLX_ExportSettings.h>
		#include <Flexporter\Sdk\FLX_Units.h>
		#include <Flexporter\Sdk\FLX_CropValues.h>
		#include <Flexporter\Sdk\FLX_AddPRS.h>
		#include <Flexporter\Sdk\FLX_TimeInfo.h>
		#include <Flexporter\Sdk\FLX_Keys.h>
		#include <Flexporter\Sdk\FLX_AddJoint.h>
		#include <Flexporter\Sdk\FLX_BaseInfo.h>
		#include <Flexporter\Sdk\FLX_AddCamera.h>
		#include <Flexporter\Sdk\FLX_AddController.h>
		#include <Flexporter\Sdk\FLX_AddHelper.h>
		#include <Flexporter\Sdk\FLX_AddLight.h>
		#include <Flexporter\Sdk\FLX_AddMaterial.h>
		#include <Flexporter\Sdk\FLX_AddPrimitive.h>
		#include <Flexporter\Sdk\FLX_AddMesh.h>
		#include <Flexporter\Sdk\FLX_AddMotion.h>
		#include <Flexporter\Sdk\FLX_AddShape.h>
		#include <Flexporter\Sdk\FLX_AddTexture.h>
		#include <Flexporter\Sdk\FLX_AddSpaceWarp.h>
		#include <Flexporter\Sdk\FLX_BaseFormat.h>

		#include "FLX_Plug.h"
		#include "FLX_PlugManager.h"
		#include "FLX_Converter.h"
	}

#endif // FLEXPORTER_H
