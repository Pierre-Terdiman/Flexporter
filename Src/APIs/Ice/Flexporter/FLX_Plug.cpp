///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains code for Flexporter plug-ins.
 *	\file		FLX_Plug.cpp
 *	\author		Pierre Terdiman
 *	\date		September, 24, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace Flexporter;

	// Exposed functions in Flexporter's plug-ins
	typedef const char*		(__stdcall* EXPORTERDESC)	();
	typedef const char*		(__stdcall* FORMAT)			();
	typedef ExportFormat*	(__stdcall* GETEXPORTER)	();
	typedef ExportSettings*	(__stdcall* GETSETTINGS)	();
	typedef Enabled*		(__stdcall* GETENABLED)		();
	typedef int				(__stdcall* EXPVERSION)		();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FormatDescriptor::FormatDescriptor()
{
	mLibHandle			= 0;
	mName				= null;
	mExtension			= null;
	mPlug				= null;
	mSettings			= null;
	mEnabledSettings	= null;
	mVersion			= 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FormatDescriptor::~FormatDescriptor()
{
	UnloadPlug();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Loads a format plug-in.
 *	\param		filename	[in] format's dll name
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool FormatDescriptor::LoadPlug(const char* filename)
{
	// Checkings
	if(!filename)	return false;

	// Plug explicit loading
	LIBRARY hLib;
	bool Status = LoadLibrary(filename, hLib);

	// Check lib handle
	if(!hLib)	return false;

	// Catch all relevant functions
	EXPORTERDESC	Proc1 = (EXPORTERDESC)	BindSymbol(hLib, "ExporterDescription");
	GETEXPORTER		Proc2 = (GETEXPORTER)	BindSymbol(hLib, "GetExporter");
	GETSETTINGS		Proc4 = (GETSETTINGS)	BindSymbol(hLib, "GetDefaultSettings");
	EXPVERSION		Proc3 = (EXPVERSION)	BindSymbol(hLib, "Version");
	FORMAT			Proc5 = (FORMAT)		BindSymbol(hLib, "FormatExtension");
	GETENABLED		Proc6 = (GETENABLED)	BindSymbol(hLib, "GetEnabledSettings");

	if(!Proc1 || !Proc2 || !Proc3 || !Proc4 || !Proc5 || !Proc6)
	{
		// Invalid plug
		BOOL Status = FreeLibrary(hLib);
		// Notification
		::MessageBox(null, "Found invalid Flexporter plug.", "Flexporter message", MB_OK);
		return false;
	}

	// Plug is valid
	mName				= (*Proc1)();
	mPlug				= (*Proc2)();
	mSettings			= (*Proc4)();
	mVersion			= (*Proc3)();
	mExtension			= (*Proc5)();
	mEnabledSettings	= (*Proc6)();

	// Check the plug
	if(mVersion!=FLEXPORTER_VERSION)
	{
		CustomArray Text;
		if(mName)	Text.Store(mName);
		else		Text.Store("A unnamed plug");
		Text.Store(" must be recompiled with the new Flexporter SDK.").Store(ubyte(0));

		// Invalid plug
		BOOL Status = FreeLibrary(hLib);
		// Notification
		::MessageBox(null, (const char*)Text.Collapse(), "Flexporter message", MB_OK);

		return false;
	}

	// Keep the plug
	mLibHandle = hLib;

	// Auto-load settings if we're allowed to do so
	if(CanAutoLoad())	LoadSettings();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Unloads a format plug-in.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FormatDescriptor::UnloadPlug()
{
	if(mLibHandle)
	{
		bool Status = UnloadLibrary(mLibHandle);
		mLibHandle = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets the last used filename.
 *	\param		filename	[out] last used filename
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FormatDescriptor::GetLastSavedFilename(String& filename)
{
	// Get from registry
	if(mName)	Load("Flexporter", mName, "Filename", filename);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Saves the last used filename.
 *	\param		filename	[in] last used filename
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FormatDescriptor::SaveLastFilename(const String& filename)
{
	// Save to registry
	if(mName)	Save("Flexporter", mName, "Filename", filename);

	// Save name of most recently used exporter (i.e. our name)
	if(mName)	Save("Flexporter", "LastExporter", "ExporterName", mName);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks we can auto-load the settings for this format.
 *	\return		true if we can auto-load
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool FormatDescriptor::CanAutoLoad()	const
{
	// Checkings
	if(!mName)	return false;

	udword Value=0;

	// Get from registry
	const char* Folder = mName;

	// Yes, the 'autoload' bool will be read 2 times from the registry. Big deal!
	Load("Flexporter", Folder, "AutoLoad", Value);
	return Value!=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Loads settings from the registry.
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool FormatDescriptor::LoadSettings() 
{
	// Checkings
	if(!mName || !mSettings)	return false;

	udword Value=0;

	// Get from registry
	const char* Folder = mName;

	// 1.15: If the value has not been saved to registry before, use default value.

	// General settings
//	if(Load("Flexporter", Folder, "Compressed",					Value))	mSettings->mCompressed					= Value!=0;
//	if(Load("Flexporter", Folder, "Compression",				Value))	mSettings->mCompression					= Value;
	if(Load("Flexporter", Folder, "ExportWholeScene",			Value))	mSettings->mExportWholeScene			= Value!=0;
	if(Load("Flexporter", Folder, "ExportVisTrack",				Value))	mSettings->mExportVisTrack				= Value!=0;
	if(Load("Flexporter", Folder, "ExportHiddenNodes",			Value))	mSettings->mExportHiddenNodes			= Value!=0;
	if(Load("Flexporter", Folder, "AutoUnload",					Value))	mSettings->mAutoUnload					= Value!=0;
	if(Load("Flexporter", Folder, "AutoLoad",					Value))	mSettings->mAutoLoad					= Value!=0;
	if(Load("Flexporter", Folder, "GlobalScale",				Value))	mSettings->mGlobalScale					= FR(Value);	// Handled as binary data
	if(Load("Flexporter", Folder, "Locale",						Value))	mSettings->mLocale						= Value;
	if(Load("Flexporter", Folder, "ExportPHYSIQUE",				Value))	mSettings->mExportPHYSIQUE				= Value!=0;
	if(Load("Flexporter", Folder, "ExportSKIN",					Value))	mSettings->mExportSKIN					= Value!=0;
	if(Load("Flexporter", Folder, "ExportBIPED",				Value))	mSettings->mExportBIPED					= Value!=0;
	if(Load("Flexporter", Folder, "AlwaysExportMotion",			Value))	mSettings->mAlwaysExportMotion			= Value!=0;
//	if(Load("Flexporter", Folder, "NbBits",						Value))	mSettings->mNbBits						= Value;

	// Meshes settings
	if(Load("Flexporter", Folder, "ExpUVW",						Value))	mSettings->mExpUVW						= Value!=0;
	if(Load("Flexporter", Folder, "DiscardW",					Value))	mSettings->mDiscardW					= Value!=0;
	if(Load("Flexporter", Folder, "ExpVTXColor",				Value))	mSettings->mExpVTXColor					= Value!=0;
	if(Load("Flexporter", Folder, "UseSmgrp",					Value))	mSettings->mUseSmgrp					= Value!=0;
	if(Load("Flexporter", Folder, "RemoveScaling",				Value))	mSettings->mRemoveScaling				= Value!=0;
	if(Load("Flexporter", Folder, "ConvertToD3D",				Value))	mSettings->mConvertToD3D				= Value!=0;
	if(Load("Flexporter", Folder, "AbsolutePRS",				Value))	mSettings->mAbsolutePRS					= Value!=0;
	if(Load("Flexporter", Folder, "ConvexHull",					Value))	mSettings->mConvexHull					= Value!=0;
	if(Load("Flexporter", Folder, "BoundingSphere",				Value))	mSettings->mBoundingSphere				= Value!=0;
	if(Load("Flexporter", Folder, "InertiaTensor",				Value))	mSettings->mInertiaTensor				= Value!=0;
	if(Load("Flexporter", Folder, "EdgeVisibility",				Value))	mSettings->mEdgeVis						= Value!=0;
	if(Load("Flexporter", Folder, "MakeManifold",				Value))	mSettings->mMakeManifold				= Value!=0;
	if(Load("Flexporter", Folder, "ExpTargetNodes",				Value))	mSettings->mExpTargetNodes				= Value!=0;
	if(Load("Flexporter", Folder, "DiscardInstances",			Value))	mSettings->mDiscardInstances			= Value!=0;

	// Consolidation settings
	if(Load("Flexporter", Folder, "ConsolidateMesh",			Value))	mSettings->mConsolidateMesh				= Value!=0;
	if(Load("Flexporter", Folder, "ComputeFaceNormals",			Value))	mSettings->mComputeFaceNormals			= Value!=0;
	if(Load("Flexporter", Folder, "ComputeVertexNormals",		Value))	mSettings->mComputeVertexNormals		= Value!=0;
	if(Load("Flexporter", Folder, "ExportNormalInfo",			Value))	mSettings->mExportNormalInfo			= Value!=0;
	if(Load("Flexporter", Folder, "WeightByArea",				Value))	mSettings->mWeightNormalsByArea			= Value!=0;
	if(Load("Flexporter", Folder, "CacheOptimize",				Value))	mSettings->mCacheOptimize				= Value!=0;

	// Material settings
	if(Load("Flexporter", Folder, "ForceAmbient",				Value))	mSettings->mForceAmbient				= Value!=0;
	if(Load("Flexporter", Folder, "ForceDiffuse",				Value))	mSettings->mForceDiffuse				= Value!=0;
	if(Load("Flexporter", Folder, "ForceSpecular",				Value))	mSettings->mForceSpecular				= Value!=0;
	if(Load("Flexporter", Folder, "ForceFilter",				Value))	mSettings->mForceFilter					= Value!=0;
	if(Load("Flexporter", Folder, "ExpMtlCtrl",					Value))	mSettings->mExpMtlCtrl					= Value!=0;
	if(Load("Flexporter", Folder, "MaterialSharing",			Value))	mSettings->mMaterialSharing				= Value!=0;

	// Texture settings
	if(Load("Flexporter", Folder, "OpacityInAlpha",				Value))	mSettings->mOpacityInAlpha				= Value!=0;
	if(Load("Flexporter", Folder, "TexnameOnly",				Value))	mSettings->mTexnameOnly					= Value!=0;
	if(Load("Flexporter", Folder, "KeepAmbientTexture",			Value))	mSettings->mKeepAmbientTexture			= Value!=0;
	if(Load("Flexporter", Folder, "KeepDiffuseTexture",			Value))	mSettings->mKeepDiffuseTexture			= Value!=0;
	if(Load("Flexporter", Folder, "KeepSpecularTexture",		Value))	mSettings->mKeepSpecularTexture			= Value!=0;
	if(Load("Flexporter", Folder, "KeepShininessTexture",		Value))	mSettings->mKeepShininessTexture		= Value!=0;
	if(Load("Flexporter", Folder, "KeepShiningStrengthTexture",	Value))	mSettings->mKeepShiningStrengthTexture	= Value!=0;
	if(Load("Flexporter", Folder, "KeepSelfIllumTexture",		Value))	mSettings->mKeepSelfIllumTexture		= Value!=0;
	if(Load("Flexporter", Folder, "KeepOpacityTexture",			Value))	mSettings->mKeepOpacityTexture			= Value!=0;
	if(Load("Flexporter", Folder, "KeepFilterTexture",			Value))	mSettings->mKeepFilterTexture			= Value!=0;
	if(Load("Flexporter", Folder, "KeepBumpTexture",			Value))	mSettings->mKeepBumpTexture				= Value!=0;
	if(Load("Flexporter", Folder, "KeepReflexionTexture",		Value))	mSettings->mKeepReflexionTexture		= Value!=0;
	if(Load("Flexporter", Folder, "KeepRefractionTexture",		Value))	mSettings->mKeepRefractionTexture		= Value!=0;
	if(Load("Flexporter", Folder, "KeepDisplacementTexture",	Value))	mSettings->mKeepDisplacementTexture		= Value!=0;
	if(Load("Flexporter", Folder, "TexMaxSize",					Value))	mSettings->mTexMaxSize					= Value;
	if(Load("Flexporter", Folder, "FlipHorizontal",				Value))	mSettings->mFlipHorizontal				= Value!=0;
	if(Load("Flexporter", Folder, "FlipVertical",				Value))	mSettings->mFlipVertical				= Value!=0;
	if(Load("Flexporter", Folder, "QuantizeTextures",			Value))	mSettings->mQuantizeTextures			= Value!=0;
	if(Load("Flexporter", Folder, "DiscardTextureAlpha",		Value))	mSettings->mDiscardTextureAlpha			= Value!=0;
	if(Load("Flexporter", Folder, "AutoCopyTextures",			Value))	mSettings->mAutoCopyTextures			= Value!=0;

	// Camera settings
	if(Load("Flexporter", Folder, "ExpCameras",					Value))	mSettings->mExpCameras					= Value!=0;
	if(Load("Flexporter", Folder, "ExpFOVControl",				Value))	mSettings->mExpFOVCtrl					= Value!=0;
	if(Load("Flexporter", Folder, "ExpCamCtrl",					Value))	mSettings->mExpCamCtrl					= Value!=0;

	// Light settings
	if(Load("Flexporter", Folder, "ExpLights",					Value))	mSettings->mExpLights					= Value!=0;
	if(Load("Flexporter", Folder, "ExpLitCtrl",					Value))	mSettings->mExpLitCtrl					= Value!=0;
	if(Load("Flexporter", Folder, "ExpProjMaps",				Value))	mSettings->mExpProjMaps					= Value!=0;
	if(Load("Flexporter", Folder, "ComputeVCol",				Value))	mSettings->mComputeVtxColors			= Value!=0;
	if(Load("Flexporter", Folder, "ComputeShadows",				Value))	mSettings->mComputeShadows				= Value!=0;
	if(Load("Flexporter", Folder, "ColorSmoothing",				Value))	mSettings->mColorSmoothing				= Value!=0;

	// Animation settings
	if(Load("Flexporter", Folder, "SingleFrame",				Value))	mSettings->mSingleFrame					= Value!=0;
	if(Load("Flexporter", Folder, "Sampling",					Value))	mSettings->mSampling					= Value!=0;
//	if(Load("Flexporter", Folder, "Keyframes",					Value))	mSettings->mSampling					= Value!=0;
	if(Load("Flexporter", Folder, "SamplingRate",				Value))	mSettings->mSamplingRate				= Value;

	// Shape settings
	if(Load("Flexporter", Folder, "ExpShapes",					Value))	mSettings->mExpShapes					= Value!=0;
	if(Load("Flexporter", Folder, "ShapeToMesh",				Value))	mSettings->mShapeToMesh					= Value!=0;

	// Helper settings
	if(Load("Flexporter", Folder, "ExpHelpers",					Value))	mSettings->mExpHelpers					= Value!=0;

	// Space warps settings
	if(Load("Flexporter", Folder, "ExpSpaceWarps",				Value))	mSettings->mExpSpaceWarps				= Value!=0;

	// IK settings
	if(Load("Flexporter", Folder, "ExportIK",					Value))	mSettings->mExportIK					= Value!=0;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Saves settings to the registry.
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool FormatDescriptor::SaveSettings()	const
{
	// Checkings
	if(!mName || !mSettings)	return false;

	// Dump to registry
	const char* Folder = mName;

	// General settings
//	Save("Flexporter", Folder, "Compressed",					(udword)mSettings->mCompressed);
//	Save("Flexporter", Folder, "Compression",					(udword)mSettings->mCompression);
	Save("Flexporter", Folder, "ExportWholeScene",				(udword)mSettings->mExportWholeScene);
	Save("Flexporter", Folder, "ExportVisTrack",				(udword)mSettings->mExportVisTrack);
	Save("Flexporter", Folder, "ExportHiddenNodes",				(udword)mSettings->mExportHiddenNodes);
	Save("Flexporter", Folder, "AutoUnload",					(udword)mSettings->mAutoUnload);
	Save("Flexporter", Folder, "AutoLoad",						(udword)mSettings->mAutoLoad);
	Save("Flexporter", Folder, "GlobalScale",					(udword)IR(mSettings->mGlobalScale));	// Handled as binary data
	Save("Flexporter", Folder, "Locale",						(udword)mSettings->mLocale);
	Save("Flexporter", Folder, "ExportPHYSIQUE",				(udword)mSettings->mExportPHYSIQUE);
	Save("Flexporter", Folder, "ExportSKIN",					(udword)mSettings->mExportSKIN);
	Save("Flexporter", Folder, "ExportBIPED",					(udword)mSettings->mExportBIPED);
	Save("Flexporter", Folder, "AlwaysExportMotion",			(udword)mSettings->mAlwaysExportMotion);
//	Save("Flexporter", Folder, "NbBits",						(udword)mSettings->mNbBits);

	// Meshes settings
	Save("Flexporter", Folder, "ExpUVW",						(udword)mSettings->mExpUVW);
	Save("Flexporter", Folder, "DiscardW",						(udword)mSettings->mDiscardW);
	Save("Flexporter", Folder, "ExpVTXColor",					(udword)mSettings->mExpVTXColor);
	Save("Flexporter", Folder, "UseSmgrp",						(udword)mSettings->mUseSmgrp);
	Save("Flexporter", Folder, "RemoveScaling",					(udword)mSettings->mRemoveScaling);
	Save("Flexporter", Folder, "ConvertToD3D",					(udword)mSettings->mConvertToD3D);
	Save("Flexporter", Folder, "AbsolutePRS",					(udword)mSettings->mAbsolutePRS);
	Save("Flexporter", Folder, "ConvexHull",					(udword)mSettings->mConvexHull);
	Save("Flexporter", Folder, "BoundingSphere",				(udword)mSettings->mBoundingSphere);
	Save("Flexporter", Folder, "InertiaTensor",					(udword)mSettings->mInertiaTensor);
	Save("Flexporter", Folder, "EdgeVisibility",				(udword)mSettings->mEdgeVis);
	Save("Flexporter", Folder, "MakeManifold",					(udword)mSettings->mMakeManifold);
	Save("Flexporter", Folder, "ExpTargetNodes",				(udword)mSettings->mExpTargetNodes);
	Save("Flexporter", Folder, "DiscardInstances",				(udword)mSettings->mDiscardInstances);

	// Consolidation settings
	Save("Flexporter", Folder, "ConsolidateMesh",				(udword)mSettings->mConsolidateMesh);
	Save("Flexporter", Folder, "ComputeFaceNormals",			(udword)mSettings->mComputeFaceNormals);
	Save("Flexporter", Folder, "ComputeVertexNormals",			(udword)mSettings->mComputeVertexNormals);
	Save("Flexporter", Folder, "ExportNormalInfo",				(udword)mSettings->mExportNormalInfo);
	Save("Flexporter", Folder, "WeightByArea",					(udword)mSettings->mWeightNormalsByArea);
	Save("Flexporter", Folder, "CacheOptimize",					(udword)mSettings->mCacheOptimize);

	// Material settings
	Save("Flexporter", Folder, "ForceAmbient",					(udword)mSettings->mForceAmbient);
	Save("Flexporter", Folder, "ForceDiffuse",					(udword)mSettings->mForceDiffuse);
	Save("Flexporter", Folder, "ForceSpecular",					(udword)mSettings->mForceSpecular);
	Save("Flexporter", Folder, "ForceFilter",					(udword)mSettings->mForceFilter);
	Save("Flexporter", Folder, "ExpMtlCtrl",					(udword)mSettings->mExpMtlCtrl);
	Save("Flexporter", Folder, "MaterialSharing",				(udword)mSettings->mMaterialSharing);

	// Texture settings
	Save("Flexporter", Folder, "OpacityInAlpha",				(udword)mSettings->mOpacityInAlpha);
	Save("Flexporter", Folder, "TexnameOnly",					(udword)mSettings->mTexnameOnly);
	Save("Flexporter", Folder, "KeepAmbientTexture",			(udword)mSettings->mKeepAmbientTexture);
	Save("Flexporter", Folder, "KeepDiffuseTexture",			(udword)mSettings->mKeepDiffuseTexture);
	Save("Flexporter", Folder, "KeepSpecularTexture",			(udword)mSettings->mKeepSpecularTexture);
	Save("Flexporter", Folder, "KeepShininessTexture",			(udword)mSettings->mKeepShininessTexture);
	Save("Flexporter", Folder, "KeepShiningStrengthTexture",	(udword)mSettings->mKeepShiningStrengthTexture);
	Save("Flexporter", Folder, "KeepSelfIllumTexture",			(udword)mSettings->mKeepSelfIllumTexture);
	Save("Flexporter", Folder, "KeepOpacityTexture",			(udword)mSettings->mKeepOpacityTexture);
	Save("Flexporter", Folder, "KeepFilterTexture",				(udword)mSettings->mKeepFilterTexture);
	Save("Flexporter", Folder, "KeepBumpTexture",				(udword)mSettings->mKeepBumpTexture);
	Save("Flexporter", Folder, "KeepReflexionTexture",			(udword)mSettings->mKeepReflexionTexture);
	Save("Flexporter", Folder, "KeepRefractionTexture",			(udword)mSettings->mKeepRefractionTexture);
	Save("Flexporter", Folder, "KeepDisplacementTexture",		(udword)mSettings->mKeepDisplacementTexture);
	Save("Flexporter", Folder, "TexMaxSize",					(udword)mSettings->mTexMaxSize);
	Save("Flexporter", Folder, "FlipHorizontal",				(udword)mSettings->mFlipHorizontal);
	Save("Flexporter", Folder, "FlipVertical",					(udword)mSettings->mFlipVertical);
	Save("Flexporter", Folder, "QuantizeTextures",				(udword)mSettings->mQuantizeTextures);
	Save("Flexporter", Folder, "DiscardTextureAlpha",			(udword)mSettings->mDiscardTextureAlpha);
	Save("Flexporter", Folder, "AutoCopyTextures",				(udword)mSettings->mAutoCopyTextures);

	// Camera settings
	Save("Flexporter", Folder, "ExpCameras",					(udword)mSettings->mExpCameras);
	Save("Flexporter", Folder, "ExpFOVControl",					(udword)mSettings->mExpFOVCtrl);
	Save("Flexporter", Folder, "ExpCamCtrl",					(udword)mSettings->mExpCamCtrl);

	// Light settings
	Save("Flexporter", Folder, "ExpLights",						(udword)mSettings->mExpLights);
	Save("Flexporter", Folder, "ExpLitCtrl",					(udword)mSettings->mExpLitCtrl);
	Save("Flexporter", Folder, "ExpProjMaps",					(udword)mSettings->mExpProjMaps);
	Save("Flexporter", Folder, "ComputeVCol",					(udword)mSettings->mComputeVtxColors);
	Save("Flexporter", Folder, "ComputeShadows",				(udword)mSettings->mComputeShadows);
	Save("Flexporter", Folder, "ColorSmoothing",				(udword)mSettings->mColorSmoothing);

	// Animation settings
	Save("Flexporter", Folder, "SingleFrame",					(udword)mSettings->mSingleFrame);
	Save("Flexporter", Folder, "Sampling",						(udword)mSettings->mSampling);
//	Save("Flexporter", Folder, "Keyframes",						(udword)mSettings->mSampling);
	Save("Flexporter", Folder, "SamplingRate",					(udword)mSettings->mSamplingRate);

	// Shape settings
	Save("Flexporter", Folder, "ExpShapes",						(udword)mSettings->mExpShapes);
	Save("Flexporter", Folder, "ShapeToMesh",					(udword)mSettings->mShapeToMesh);

	// Helper settings
	Save("Flexporter", Folder, "ExpHelpers",					(udword)mSettings->mExpHelpers);

	// Space warps settings
	Save("Flexporter", Folder, "ExpSpaceWarps",					(udword)mSettings->mExpSpaceWarps);

	// IK settings
	Save("Flexporter", Folder, "ExportIK",						(udword)mSettings->mExportIK);

	return true;
}
