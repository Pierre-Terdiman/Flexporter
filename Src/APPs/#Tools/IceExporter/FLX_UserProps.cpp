///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file deals with user-defined properties.
 *	\file		FLX_UserProps.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

/*
- (plus dur surement) la limitation de la taille maxi des textures par 
objets :)
- Le calcul lightmap/shadows et cie

	Mass <float>
		Defines the object's mass.

	Friction <float>
		Defines the object's friction.

	StaticFriction <float>
		Defines the object's static friction.

	Elasticity <float>
		Defines the object's elasticity

	Sphere
		Tells the engine to use a computer-generated bounding sphere, as the object's bounding volume.

	AABB
		Tells the engine to use a computer-generated AABB, as the object's bounding volume.

	OBB
		Tells the engine to use a computer-generated OBB, as the object's bounding volume.


 You can also define some global parameters at scene-level. To do that, use the Scene Description in MAX:
	- in the menu, select "File->Summary Info..."
	- use the Description box as the scene's User Properties.

 Available scene parameters :

	PhysStart <decimal>
		Setup start of simulation (number of seconds to wait before physics gets activated)

	PhysDuration <decimal>
		Setup duration of simulation (number of seconds to wait before going on to next scene)

	DragForce <decimal>
		Setup mouse drag force (applied on objects)

	CollisionDetection <decimal>
		0 => disable collision detection, else enable it

	GridRes <decimal>
		Setup resolution of grid

	Gravity <float> <float> <float>
		Setup gravity vector.

	GroundPlane <decimal>
		0 => disable default ground plane, else enable it

	StepSize <float>
		Setup simulation step size (most of the time you should rely on default value)


*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UserProps::UserProps()
{
	Reset();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UserProps::~UserProps()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Resets the user-props to default values.
 *	\return		Self-reference
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UserProps& UserProps::Reset()
{
	mDensity				= 0.0f;
	mMass					= 0.0f;
	mSamplingDensity		= 0;
	mResetPivot				= false;
	mCollisionDetection		= true;
	mLockPivot				= false;
	mLinearVelocity.Zero();
	mAngularVelocity.Zero();

	mFractal				= false;
	mMirror					= false;
	mShadow					= false;
	mRoom					= false;
	mBone					= false;
	mMainBone				= false;

	mMorph					= false;

	mForceExport			= false;
	mForceConsolidation		= false;
	mForceManifold			= false;
	mForceNormals			= false;
	mExportPRS				= false;
	mExportVisibility		= false;
	mExportFOVTrack			= false;
	mExportCamCtrl			= false;
	mExportLitCtrl			= false;
	mExportUVW				= false;
	mExportVtxColors		= false;
	mExportSMG				= false;
	mForceShapeToMesh		= false;
	mForceIK				= false;

	mDiscardExport			= false;
	mDiscardConsolidation	= false;
	mDiscardManifold		= false;
	mDiscardNormals			= false;
	mDiscardPRS				= false;
	mDiscardVisibility		= false;
	mDiscardFOVTrack		= false;
	mDiscardCamCtrl			= false;
	mDiscardLitCtrl			= false;
	mDiscardUVW				= false;
	mDiscardVtxColors		= false;
	mDiscardSMG				= false;
	mDiscardShapeToMesh		= false;
	mDiscardIK				= false;

	mDebugDumpSubanims		= false;

	return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	A global function to catch user-defined properties.
 *	\param		node	[in] a MAX node
 *	\param		props	[out] result structure
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void Flexporter::GetUserProperties(INode* node, UserProps& props)
{
	struct Local
	{
		static void AnalyzeLine(char* line, UserProps& props)
		{
			char Command[256];
			// We need to cut this line to pieces in order to get the command and parameters as well.
			udword i=0;
			// Get command
			while(line[i] && line[i]!=' ')
			{
				Command[i]=toupper(line[i]);
				i++;
			}
			Command[i]=0;

					if(strcmp(Command, "FRACTAL")==0)				props.mFractal				= true;
			else	if(strcmp(Command, "MIRROR")==0)				props.mMirror				= true;
			else	if(strcmp(Command, "MORPH")==0)					props.mMorph				= true;
			else	if(strcmp(Command, "SHADOW")==0)				props.mShadow				= true;
			else	if(strcmp(Command, "ROOM")==0)					props.mRoom					= true;
			else	if(strcmp(Command, "BIPED")==0)					props.mBone					= true;
			else	if(strcmp(Command, "BIPEDMAIN")==0)				props.mMainBone				= true;

			else	if(strcmp(Command, "FORCEEXPORT")==0)			props.mForceExport			= true;
			else	if(strcmp(Command, "FORCECONSOLIDATION")==0)	props.mForceConsolidation	= true;
			else	if(strcmp(Command, "FORCEMANIFOLD")==0)			props.mForceManifold		= true;
			else	if(strcmp(Command, "FORCENORMALS")==0)			props.mForceNormals			= true;
			else	if(strcmp(Command, "EXPORTPRS")==0)				props.mExportPRS			= true;
			else	if(strcmp(Command, "EXPORTVIS")==0)				props.mExportVisibility		= true;
			else	if(strcmp(Command, "EXPORTFOVCTRL")==0)			props.mExportFOVTrack		= true;
			else	if(strcmp(Command, "EXPORTCAMCTRL")==0)			props.mExportCamCtrl		= true;
			else	if(strcmp(Command, "EXPORTLITCTRL")==0)			props.mExportLitCtrl		= true;
			else	if(strcmp(Command, "EXPORTUVW")==0)				props.mExportUVW			= true;
			else	if(strcmp(Command, "EXPORTVTXCOL")==0)			props.mExportVtxColors		= true;
			else	if(strcmp(Command, "EXPORTSMG")==0)				props.mExportSMG			= true;
			else	if(strcmp(Command, "FORCESHAPETOMESH")==0)		props.mForceShapeToMesh		= true;

			else	if(strcmp(Command, "DISCARDEXPORT")==0)			props.mDiscardExport		= true;
			else	if(strcmp(Command, "DISCARDCONSOLIDATION")==0)	props.mDiscardConsolidation	= true;
			else	if(strcmp(Command, "DISCARDMANIFOLD")==0)		props.mDiscardManifold		= true;
			else	if(strcmp(Command, "DISCARDNORMALS")==0)		props.mDiscardNormals		= true;
			else	if(strcmp(Command, "DISCARDPRS")==0)			props.mDiscardPRS			= true;
			else	if(strcmp(Command, "DISCARDVIS")==0)			props.mDiscardVisibility	= true;
			else	if(strcmp(Command, "DISCARDFOVCTRL")==0)		props.mDiscardFOVTrack		= true;
			else	if(strcmp(Command, "DISCARDCAMCTRL")==0)		props.mDiscardCamCtrl		= true;
			else	if(strcmp(Command, "DISCARDLITCTRL")==0)		props.mDiscardLitCtrl		= true;
			else	if(strcmp(Command, "DISCARDUVW")==0)			props.mDiscardUVW			= true;
			else	if(strcmp(Command, "DISCARDVTXCOL")==0)			props.mDiscardVtxColors		= true;
			else	if(strcmp(Command, "DISCARDSMG")==0)			props.mDiscardSMG			= true;
			else	if(strcmp(Command, "DISCARDSHAPETOMESH")==0)	props.mDiscardShapeToMesh	= true;
		}
	};

	char Line[1024];

	// Reset all user-controlled values 
	props.Reset();

	//
	if(node)
	{
		CStr Buffer;
		node->GetUserPropBuffer(Buffer);
		char* Source = Buffer.data();

		udword i=0;
		while(*Source)
		{
			if(*Source!=0xd && *Source!=0xa)	Line[i++]=*Source++;
			else Source++;
			if(*Source==0xd && *(Source+1)==0xa)
			{
				Source+=2;
				Line[i]=0;
				Local::AnalyzeLine(Line, props);
				i=0;
			}
		}
		Line[i]=0;
		Local::AnalyzeLine(Line, props);
	}
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	User-callback, called by the parser each time a new line has been found.
 *	\param		line	[in] the new line
 *	\param		pb		[in] a parameter-block made from current line
 *	\return		true to end parsing
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::Parameter(const char* line, const ParameterBlock& pb)
{
	// Look for known keywords
	String StringToParse(line);
	StringToParse.Replace('=', ' ');	// We want to treat "Item = value" like "Item value"
	StringToParse.ToLower();
	// Make a parameter block from the transformed version
	ParameterBlock PB(StringToParse);

	if(mParseSceneInfos)
	{
		// The input string has already been converted to lower-case, and '=' characters have been removed
		// ### changed => we must use the original string for help text
		if(mHelpText)
		{
			// The only possible keyword here is "~FLX_HelpText" to end help text.
			if(PB[0]=="~flx_helptext" && PB.GetNbParams()==1)
			{
				mHelpText = false;
				mArrayLog.StoreASCII(_F("Scene helper text: %s\n", mSceneHelpText));
			}
			else
			{
				// Add unmodified string to help text
				mSceneHelpText += line;
//				mSceneHelpText += "\n";
				mSceneHelpText.NewLine();
			}
		}
		else
		{
			// FLX_GlobalScale = <float>
			if(PB[0]=="flx_globalscale" && PB.GetNbParams()==2)
			{
				mSettings->mGlobalScale = ToFloat(PB[1]);
				mArrayLog.StoreASCII(_F("Scene settings override: global scale = %f\n", mSettings->mGlobalScale));
			}
			// FLX_Gravity = <float> <float> <float>
			else if(PB[0]=="flx_gravity" && PB.GetNbParams()==4)
			{
				mGravity.x = ToFloat(PB[1]);
				mGravity.y = ToFloat(PB[2]);
				mGravity.z = ToFloat(PB[3]);
				mArrayLog.StoreASCII(_F("Gravity: %f %f %f\n", mGravity.x, mGravity.y, mGravity.z));
			}
			// FLX_Restitution = <float>
			else if(PB[0]=="flx_restitution" && PB.GetNbParams()==2)
			{
				mRestitution = ToFloat(PB[1]);
				mArrayLog.StoreASCII(_F("Restitution = %f\n", mRestitution));
			}
			// FLX_StaticFriction = <float>
			else if(PB[0]=="flx_staticfriction" && PB.GetNbParams()==2)
			{
				mStaticFriction = ToFloat(PB[1]);
				mArrayLog.StoreASCII(_F("StaticFriction = %f\n", mStaticFriction));
			}
			// FLX_Friction = <float>
			else if(PB[0]=="flx_friction" && PB.GetNbParams()==2)
			{
				mFriction = ToFloat(PB[1]);
				mArrayLog.StoreASCII(_F("Friction = %f\n", mFriction));
			}
			// FLX_GroundPlane
			else if(PB[0]=="flx_groundplane" && PB.GetNbParams()==1)
			{
				mGroundPlane = true;
				mArrayLog.StoreASCII("Ground-plane enabled.\n");
			}
			// FLX_CollisionDetection = <decimal>
			else if(PB[0]=="flx_collisiondetection" && PB.GetNbParams()==2)
			{
				mCollisionDetection = ToDecimal(PB[1])!=0;
//				mArrayLog.StoreASCII("Ground-plane enabled.\n");
			}
			// FLX_Tesselation = <decimal>
			else if(PB[0]=="flx_tesselation" && PB.GetNbParams()==2)
			{
				mTesselation = ToDecimal(PB[1]);
				mArrayLog.StoreASCII(_F("Tesselation = %d\n", mTesselation));
			}
			// FLX_LightingMode = <decimal>
			else if(PB[0]=="flx_lightingmode" && PB.GetNbParams()==2)
			{
				mLightingMode = ToDecimal(PB[1]);
				mArrayLog.StoreASCII(_F("Lighting mode = %d\n", mLightingMode));
			}
			// FLX_HelpText
			else if(PB[0]=="flx_helptext" && PB.GetNbParams()==1)
			{
				mHelpText = true;
			}
			else
			{
				// Add all other lines to scene info
				mSceneInfo += line;
//				mSceneInfo += "\n";
				mSceneInfo.NewLine();
			}
		}
	}
	else
	{
		// Here we parse data for a single object

		// ### known keywords should be removed from user props

		// FLX_Density = <float>
		if(PB[0]=="flx_density" && PB.GetNbParams()==2)
		{
			mProps.mDensity = ToFloat(PB[1]);
		}
		// FLX_Mass = <float>
		else if(PB[0]=="flx_mass" && PB.GetNbParams()==2)
		{
			mProps.mMass = ToFloat(PB[1]);
		}
		// FLX_SamplingDensity = <sdword>
		else if(PB[0]=="flx_samplingdensity" && PB.GetNbParams()==2)
		{
			mProps.mSamplingDensity = ToDecimal(PB[1]);
		}
		// FLX_CollisionDetection = <decimal>
		else if(PB[0]=="flx_collisiondetection" && PB.GetNbParams()==2)
		{
			mProps.mCollisionDetection = ToDecimal(PB[1])!=0;
		}
		// FLX_ResetPivot
		else if(PB[0]=="flx_resetpivot" && PB.GetNbParams()==1)
		{
			mProps.mResetPivot = true;
		}
		// FLX_LockPivot
		else if(PB[0]=="flx_lockpivot" && PB.GetNbParams()==1)
		{
			mProps.mLockPivot = true;
		}
		// FLX_LinearVelocity = <float> <float> <float>
		else if(PB[0]=="flx_linearvelocity" && PB.GetNbParams()==4)
		{
			mProps.mLinearVelocity.x = ToFloat(PB[1]);
			mProps.mLinearVelocity.y = ToFloat(PB[2]);
			mProps.mLinearVelocity.z = ToFloat(PB[3]);
			mArrayLog.StoreASCII(_F("Linear velocity: %f %f %f\n", mProps.mLinearVelocity.x, mProps.mLinearVelocity.y, mProps.mLinearVelocity.z));
		}
		// FLX_AngularVelocity = <float> <float> <float>
		else if(PB[0]=="flx_angularvelocity" && PB.GetNbParams()==4)
		{
			mProps.mAngularVelocity.x = ToFloat(PB[1]);
			mProps.mAngularVelocity.y = ToFloat(PB[2]);
			mProps.mAngularVelocity.z = ToFloat(PB[3]);
			mArrayLog.StoreASCII(_F("Angular velocity: %f %f %f\n", mProps.mAngularVelocity.x, mProps.mAngularVelocity.y, mProps.mAngularVelocity.z));
		}
		else
		{
			// Very old code kept for compatibility
			// ### but they should be unified in "Consolidation = 1", etc
			String Command = pb[0];

			// Those ones are especially obsolete
					if(Command=="flx_fractal")				mProps.mFractal				= true;
			else	if(Command=="flx_mirror")				mProps.mMirror				= true;
			else	if(Command=="flx_morph")				mProps.mMorph				= true;
			else	if(Command=="flx_shadow")				mProps.mShadow				= true;
			else	if(Command=="flx_room")					mProps.mRoom				= true;
			else	if(Command=="flx_biped")				mProps.mBone				= true;
			else	if(Command=="flx_bipedmain")			mProps.mMainBone			= true;

			else	if(Command=="flx_forceexport")			mProps.mForceExport			= true;
			else	if(Command=="flx_forceconsolidation")	mProps.mForceConsolidation	= true;
			else	if(Command=="flx_forcemanifold")		mProps.mForceManifold		= true;
			else	if(Command=="flx_forcenormals")			mProps.mForceNormals		= true;
			else	if(Command=="flx_exportprs")			mProps.mExportPRS			= true;
			else	if(Command=="flx_exportvis")			mProps.mExportVisibility	= true;
			else	if(Command=="flx_exportfovctrl")		mProps.mExportFOVTrack		= true;
			else	if(Command=="flx_exportcamctrl")		mProps.mExportCamCtrl		= true;
			else	if(Command=="flx_exportlitctrl")		mProps.mExportLitCtrl		= true;
			else	if(Command=="flx_exportuvw")			mProps.mExportUVW			= true;
			else	if(Command=="flx_exportvtxcol")			mProps.mExportVtxColors		= true;
			else	if(Command=="flx_exportsmg")			mProps.mExportSMG			= true;
			else	if(Command=="flx_forceshapetomesh")		mProps.mForceShapeToMesh	= true;
			else	if(Command=="flx_forceik")				mProps.mForceIK				= true;

			else	if(Command=="flx_discardexport")		mProps.mDiscardExport		= true;
			else	if(Command=="flx_discardconsolidation")	mProps.mDiscardConsolidation= true;
			else	if(Command=="flx_discardmanifold")		mProps.mDiscardManifold		= true;
			else	if(Command=="flx_discardnormals")		mProps.mDiscardNormals		= true;
			else	if(Command=="flx_discardprs")			mProps.mDiscardPRS			= true;
			else	if(Command=="flx_discardvis")			mProps.mDiscardVisibility	= true;
			else	if(Command=="flx_discardfovctrl")		mProps.mDiscardFOVTrack		= true;
			else	if(Command=="flx_discardcamctrl")		mProps.mDiscardCamCtrl		= true;
			else	if(Command=="flx_discardlitctrl")		mProps.mDiscardLitCtrl		= true;
			else	if(Command=="flx_discarduvw")			mProps.mDiscardUVW			= true;
			else	if(Command=="flx_discardvtxcol")		mProps.mDiscardVtxColors	= true;
			else	if(Command=="flx_discardsmg")			mProps.mDiscardSMG			= true;
			else	if(Command=="flx_discardshapetomesh")	mProps.mDiscardShapeToMesh	= true;
			else	if(Command=="flx_discardik")			mProps.mDiscardIK			= true;

			else	if(Command=="DEBUG_DUMP_SUBANIMS")		mProps.mDebugDumpSubanims	= true;
		}
	}
	return false;
}

void UserProps::SetupObject(ObjectDescriptor& object)
{
	object.mDensity				= mDensity;
	object.mMass				= mMass;
	object.mSamplingDensity		= mSamplingDensity;
	object.mResetPivot			= mResetPivot;
	object.mIsCollidable		= mCollisionDetection;
	object.mLockPivot			= mLockPivot;
	object.mLinearVelocity.x	= mLinearVelocity.x;
	object.mLinearVelocity.y	= mLinearVelocity.y;
	object.mLinearVelocity.z	= mLinearVelocity.z;
	object.mAngularVelocity.x	= mAngularVelocity.x;
	object.mAngularVelocity.y	= mAngularVelocity.y;
	object.mAngularVelocity.z	= mAngularVelocity.z;
}
