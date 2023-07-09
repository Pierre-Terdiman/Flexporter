///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the Void Format exporter plug-in for Flexporter.
 *	\file		VoidExporter.cpp
 *	\author		Pierre Terdiman
 *	\date		May, 6, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

static VoidFormat		gVoidExporter;		// Gloabl exporter instance
static ExportSettings	gSettings;			// Global export settings

// FLEXPORTER Identification Callbacks

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Gives a brief exporter description. That string is displayed inside MAX, in the Options Panel.
 *	\relates	VoidFormat
 *	\fn			ExporterDescription()
 *	\return		a description string
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* ExporterDescription()
{ 
	return "Void Exporter";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the format's extension. That string is displayed inside MAX, in the Options Panel.
 *	\relates	VoidFormat
 *	\fn			FormatExtension()
 *	\return		an extension string
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* FormatExtension()
{
	return "null";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the one and only exporter instance.
 *	\relates	VoidFormat
 *	\fn			GetExporter()
 *	\return		pointer to the global exporter instance.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExportFormat* GetExporter()
{
	return &gVoidExporter;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the default exportation settings for this format. This is the right place to initialize the default settings for your format.
 *	\relates	VoidFormat
 *	\fn			GetDefaultSettings()
 *	\return		pointer to the global ExportSettings instance.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExportSettings* GetDefaultSettings()
{
	return &gSettings;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the enabled/disabled status for all settings. This is the right place to disable options your own format doesn't support.
 *	\relates	VoidFormat
 *	\fn			GetEnabledSettings()
 *	\return		pointer to a global Enabled instance.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Enabled* GetEnabledSettings()
{
	static Enabled Settings;
	return &Settings;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Flexporter callback.
 *	Returns the FLEXPORTER SDK Version.
 *	\relates	VoidFormat
 *	\fn			Version()
 *	\return		FLEXPORTER_VERSION
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Version()
{
	return FLEXPORTER_VERSION;
}









///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	A void exporter plug-in for Flexporter..
 *	\class		VoidFormat
 *	\author		Pierre Terdiman
 *	\version	1.0
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VoidFormat::VoidFormat()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VoidFormat::~VoidFormat()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Plug-in initialization method.
 *	This method is called once before each export. When this method is called, the mSettings and mFilename members of the base format are valid.
 *	\param		motion		[in] true for pure motion files.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VoidFormat::Init(bool motion)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Scene export method.
 *	This method is called once to export the scene global information.
 *	\param		maininfo	[in] main info structure
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VoidFormat::SetSceneInfo(const MainDescriptor& maininfo)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Camera export method.
 *	This method is called once for each exported camera.
 *	\param		camera		[in] a structure filled with current camera information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VoidFormat::ExportCamera(const CameraDescriptor& camera)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Controller export method.
 *	This method is called once for each exported controller.
 *	\param		controller		[in] a structure filled with current controller information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VoidFormat::ExportController(const ControllerDescriptor& controller)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Helper export method.
 *	This method is called once for each exported helper.
 *	\param		helper		[in] a structure filled with current helper information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VoidFormat::ExportHelper(const HelperDescriptor& helper)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Light export method.
 *	This method is called once for each exported light.
 *	\param		light		[in] a structure filled with current light information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VoidFormat::ExportLight(const LightDescriptor& light)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Material export method.
 *	This method is called once for each exported material.
 *	\param		material		[in] a structure filled with current material information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VoidFormat::ExportMaterial(const MaterialDescriptor& material)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Mesh export method.
 *	This method is called once for each exported mesh.
 *	\param		mesh		[in] a structure filled with current mesh information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VoidFormat::ExportMesh(const MeshDescriptor& mesh)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Motion export method.
 *	This method is called once for each exported motion.
 *	\param		motion		[in] a structure filled with current motion information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VoidFormat::ExportMotion(const MotionDescriptor& motion)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Shape export method.
 *	This method is called once for each exported shape.
 *	\param		shape		[in] a structure filled with current shape information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VoidFormat::ExportShape(const ShapeDescriptor& shape)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Texture export method.
 *	This method is called once for each exported texture.
 *	\param		texture		[in] a structure filled with current texture information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VoidFormat::ExportTexture(const TextureDescriptor& texture)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Space warp export method.
 *	This method is called once for each exported space warp.
 *	\param		space_warp	[in] a structure filled with current space_warp information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VoidFormat::ExportSpaceWarp(const SpaceWarpDescriptor& space_warp)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	End of export notification.
 *	This method is called once all nodes have been exported. This is a convenient place to free all used ram, etc.
 *	\param		stats		[in] a structure filled with some export statistics.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VoidFormat::EndExport(const StatsDescriptor& stats)
{
	return true;
}

