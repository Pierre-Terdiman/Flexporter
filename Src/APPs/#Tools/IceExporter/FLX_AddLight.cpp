///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a light.
 *	\file		FLX_AddLight.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ### TO DO: instances for cams & lights

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

static float gGlobalScale = 1.0f;

// Sampling callbacks
static Point3 SampleColor			(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetRGBColor(time_value, Inter); }
static float SampleIntensity		(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetIntensity(time_value, Inter); }
static float SampleContrast			(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetContrast(time_value, Inter); }
static float SampleDiffuseSoft		(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetDiffuseSoft(time_value, Inter); }
static float SampleNearAttenStart	(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetAtten(time_value, ATTEN1_START, Inter) * gGlobalScale; }
static float SampleNearAttenEnd		(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetAtten(time_value, ATTEN1_END, Inter) * gGlobalScale; }
static float SampleFarAttenStart	(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetAtten(time_value, ATTEN_START, Inter) * gGlobalScale; }
static float SampleFarAttenEnd		(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetAtten(time_value, ATTEN_END, Inter) * gGlobalScale; }
static float SampleHotSpot			(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetHotspot(time_value, Inter); }
static float SampleFallsize			(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetFallsize(time_value, Inter); }
static float SampleAspect			(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetAspect(time_value, Inter); }
//static float SampleTDist			(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetTDist(time_value, Inter); }
static float SampleRayBias			(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetRayBias(time_value, Inter); }
static float SampleMapBias			(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetMapBias(time_value, Inter); }
static float SampleMapRange			(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetMapRange(time_value, Inter); }
static float SampleMapSize			(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetMapSize(time_value, Inter); }
static Point3 SampleShadowColor		(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetShadColor(time_value, Inter); }
static float SampleShadowDensity	(udword time_value, udword user_data)	{ Interval Inter = FOREVER;	return ((GenLight*)user_data)->GetShadMult(time_value, Inter); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Converts a light.
 *	\param		node			[in] the MAX node
 *	\param		derived_object	[in] a possible collapsed derived object
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddLight(INode* node, Object* derived_object)
{
	// Checkings
	if(!node)	return false;
	if(!mSettings->mExpLights && !mProps.mForceExport)
	{
		// Update log text
		mArrayLog.StoreASCII("(discarded because of export options)\n");
		return true;
	}

	// Get time info back
	long		StartTime		= mTimeInfo.mStartTime;
	long		EndTime			= mTimeInfo.mEndTime;
	long		DeltaTime		= mTimeInfo.mDeltaTime;
	long		FrameRate		= mTimeInfo.mFrameRate;
	TimeValue	CurrentTime		= mTimeInfo.mCurrentTime;

	// Get right object: it can be the real node or a collapsed derived object
	Object* NodeObject = derived_object ? derived_object : node->GetObjectRef();

	// 1) Get useful information from MAX
	Interval	Inter				=			FOREVER;
	char*		LightName			=			node	->GetName();									// Light's name
	bool		IsGroupMember		=			node	->IsGroupMember()!=0;							// Belonging to a group
	INode*		Target				=			node	->GetTarget();									// Get possible light target

	GenLight*	Gl					= (GenLight*)NodeObject;											// Assign node-object as a generic MAX light

	int			LightType			=			Gl		->Type();										// Get light's type
	bool		IsSpot				=			Gl		->IsSpot()!=0;									// Is the light a spotlight?
	bool		IsDir				=			Gl		->IsDir()!=0;									// Is the light a directional?
	Point3		Color				=			Gl		->GetRGBColor(CurrentTime, Inter);				// Get light's color
	float		Intensity			=			Gl		->GetIntensity(CurrentTime, Inter);				// Get light's intensity
	float		Contrast			=			Gl		->GetContrast(CurrentTime, Inter);				// Get light's contrast
	float		DiffuseSoft			=			Gl		->GetDiffuseSoft(CurrentTime, Inter);			// Get light's diffuse soft
	bool		LightUsed			=			Gl		->GetUseLight()!=0;								// Is the light used?
	bool		AffectDiffuse		=			Gl		->GetAffectDiffuse()!=0;						// Does the light affect diffuse?
	bool		AffectSpecular		=			Gl		->GetAffectSpecular()!=0;						// Does the light affect specular?
	bool		UseAttenNear		=			Gl		->GetUseAttenNear()!=0;
	bool		AttenNearDisplay	=			Gl		->GetAttenNearDisplay()!=0;
	bool		UseAtten			=			Gl		->GetUseAtten()!=0;								// Is attenuation used?
	bool		ShowAtten			=			Gl		->GetAttenDisplay()!=0;
	float		NearAttenStart		=			Gl		->GetAtten(CurrentTime, ATTEN1_START, Inter);	// Near atten start
	float		NearAttenEnd		=			Gl		->GetAtten(CurrentTime, ATTEN1_END, Inter);		// Near atten end
	float		AttenStart			=			Gl		->GetAtten(CurrentTime, ATTEN_START, Inter);	// Atten start
	float		AttenEnd			=			Gl		->GetAtten(CurrentTime, ATTEN_END, Inter);		// Atten end
	char		DecayType			= (char)	Gl		->GetDecayType();								// Get light's decay type
	float		HotSpot				=			Gl		->GetHotspot(CurrentTime, Inter);				// Get light's hotspot
	float		Fallsize			=			Gl		->GetFallsize(CurrentTime, Inter);				// Get light's falloff
	float		Aspect				=			Gl		->GetAspect(CurrentTime, Inter);				// Get light's aspect
	long		SpotShape			= (long)	Gl		->GetSpotShape();								// Get light's spot shape (RECT_LIGHT, CIRCLE_LIGHT)
	long		Overshoot			= (long)	Gl		->GetOvershoot();								// Get light's overshoot
	bool		ConeDisplay			=			Gl		->GetConeDisplay()!=0;
	float		TDist				=			Gl		->GetTDist(CurrentTime, Inter);					// Distance to target
	long		ShadowType			= (long)	Gl		->GetShadowType();								// Get light's shadow type
	long		AbsMapBias			= (long)	Gl		->GetAbsMapBias();								// Get light's absolute map bias
	float		RayBias				=			Gl		->GetRayBias(CurrentTime, Inter);				// Raytrace bias
	float		MapBias				=			Gl		->GetMapBias(CurrentTime, Inter);				// Map bias
	float		MapRange			=			Gl		->GetMapRange(CurrentTime, Inter);				// Map range
	long		MapSize				= (long)	Gl		->GetMapSize(CurrentTime, Inter);				// Map size
	bool		CastShadows			=			Gl		->GetShadow()!=0;								// Cast shadows or not

	Point3		ShadowColor			=			Gl		->GetShadColor(CurrentTime, Inter);				// Shadow color
	bool		LightAffectsShadow	=			Gl		->GetLightAffectsShadow()!=0;					// Light affects shadow
	float		ShadowDensity		=			Gl		->GetShadMult(CurrentTime, Inter);				// Shadow multiplier

	// Override TDist for target nodes
/*
	if(TDist==0.0f)
	{
		// Get possible target
		INode* Target = node->GetTarget();
		if(Target)
		{
			MAXPRS prs;
			ConvertPRS(Target, mTimeInfo.CurrentTime, prs);
			TDist = prs.Position.
		}
	}
*/

	// 2) Update log text
	mArrayLog.StoreASCII("Exporting light %s\n", LightName);

	// 3) Create info structure
	LightDescriptor* LightInf = (LightDescriptor*)mGlobalInfo.CreateDescriptor(node, FLX_OBJ_LIGHT);
	AddHeader(node, LightInf);

	// Get instance state (if needed)
	sdword MasterID;
	LightInf->mIsInstance = mSettings->mDiscardInstances ? false : mGlobalInfo.IsInstance(NodeObject, MasterID);

	// Check instance state (### to refactor)
	if(LightInf->mIsInstance)
	{
		LightInf->mMasterLight	= (LightDescriptor*)mGlobalInfo.GetNodeInfo()[MasterID].mExportedNode;
		LightInf->mMasterID		= LightInf->mMasterLight->mObjectID;

		// Update log text
		mArrayLog.StoreASCII("Instance node from: ").StoreASCII(mGlobalInfo.GetNodeInfo()[MasterID].mMAXNode->GetName()).StoreASCII("\n");
	}

	LightInf->mLightType			= (LType)LightType;
	LightInf->mIsSpot				= IsSpot;
	LightInf->mIsDir				= IsDir;
	LightInf->mColor				= NakedPoint(Color.x, Color.y, Color.z);
	LightInf->mIntensity			= Intensity;
	LightInf->mContrast				= Contrast;
	LightInf->mDiffuseSoft			= DiffuseSoft;
	LightInf->mLightUsed			= LightUsed;
	LightInf->mAffectDiffuse		= AffectDiffuse;
	LightInf->mAffectSpecular		= AffectSpecular;
	LightInf->mUseAttenNear			= UseAttenNear;
	LightInf->mAttenNearDisplay		= AttenNearDisplay;
	LightInf->mUseAtten				= UseAtten;
	LightInf->mShowAtten			= ShowAtten;
	LightInf->mNearAttenStart		= NearAttenStart * mSettings->mGlobalScale;
	LightInf->mNearAttenEnd			= NearAttenEnd * mSettings->mGlobalScale;
	LightInf->mAttenStart			= AttenStart * mSettings->mGlobalScale;
	LightInf->mAttenEnd				= AttenEnd * mSettings->mGlobalScale;
	LightInf->mDecayType			= DecayType;
	LightInf->mHotSpot				= HotSpot;
	LightInf->mFallsize				= Fallsize;
	LightInf->mAspect				= Aspect;
	LightInf->mSpotShape			= (SpotShp)SpotShape;
	LightInf->mOvershoot			= Overshoot;
	LightInf->mConeDisplay			= ConeDisplay;
	LightInf->mTDist				= TDist * mSettings->mGlobalScale;
	LightInf->mShadowType			= ShadowType;
	LightInf->mAbsMapBias			= AbsMapBias;
	LightInf->mRayBias				= RayBias;
	LightInf->mMapBias				= MapBias;
	LightInf->mMapRange				= MapRange;
	LightInf->mMapSize				= MapSize;
	LightInf->mCastShadows			= CastShadows;
	LightInf->mShadowColor			= NakedPoint(ShadowColor.x, ShadowColor.y, ShadowColor.z);
	LightInf->mLightAffectsShadow	= LightAffectsShadow;
	LightInf->mShadowDensity		= ShadowDensity;

	// 1.15: export projector textures
	if(mSettings->mExpProjMaps)
	{
		Texmap* ProjMap = Gl->GetProjMap();
		if(ProjMap)
		{
			TextureBlend TB;
			TB.mRGBMap					= ProjMap;
			TB.mRGBMapCoeff				= 1.0f;
			LightInf->mProjMapID		= AddTexture(TB);
		}

		Texmap* ShadowProjMap = Gl->GetShadowProjMap();
		if(ShadowProjMap)
		{
			TextureBlend TB;
			TB.mRGBMap					= ShadowProjMap;
			TB.mRGBMapCoeff				= 1.0f;
			LightInf->mShadowProjMapID	= AddTexture(TB);
		}
	}

	// 4) Update members
	mStats.mNbLights++;

	// 5) Perform format-dependent export
	SetExtStatus("calling export plug-in...");
	if(mExporter)	mExporter->ExportLight(*LightInf);
	DumpLogString();

	// 6) Animation
	// 6-1) PRS track
	SetExtStatus("exporting PRS data...");
	bool Status = AddPRSController(node, LightInf);

	// 6-2) Visibility track
	SetExtStatus("exporting visibility data...");
	Status = AddVisibilityController(node, LightInf);

	// 6-3) Attributes
	if(!mProps.mDiscardLitCtrl && (mSettings->mExpLitCtrl || mProps.mExportLitCtrl))
	{
		SetExtStatus("exporting light attributes controllers...");
		Animatable* Attributes = GetSubAnim(Gl, "Parameters");
		if(Attributes)
		{
			// ...give callbacks access to global scale...
			gGlobalScale = mSettings->mGlobalScale;

			// Enable anim export temporarily
			bool Saved = mSettings->mSingleFrame;
			if(mProps.mExportLitCtrl)	mSettings->mSingleFrame = false;

			Status = AddPointController(GetSubAnim(Attributes, "Color"),					LightInf, "Color",				SampleColor,			udword(Gl));
			Status = AddFloatController(GetSubAnim(Attributes, "Multiplier"),				LightInf, "Intensity",			SampleIntensity,		udword(Gl));
			Status = AddFloatController(GetSubAnim(Attributes, "Contrast"),					LightInf, "Contrast",			SampleContrast,			udword(Gl));
			Status = AddFloatController(GetSubAnim(Attributes, "Diffuse Soften"),			LightInf, "Diffuse Soften",		SampleDiffuseSoft,		udword(Gl));
			Status = AddFloatController(GetSubAnim(Attributes, "Attenuation Near Start"),	LightInf, "Near Atten Start",	SampleNearAttenStart,	udword(Gl));
			Status = AddFloatController(GetSubAnim(Attributes, "Attenuation Near End"),		LightInf, "Near Atten End",		SampleNearAttenEnd,		udword(Gl));
			Status = AddFloatController(GetSubAnim(Attributes, "Attenuation Far Start"),	LightInf, "Far Atten Start",	SampleFarAttenStart,	udword(Gl));
			Status = AddFloatController(GetSubAnim(Attributes, "Attenuation Far End"),		LightInf, "Far Atten End",		SampleFarAttenEnd,		udword(Gl));
	//		Status = AddController(GetSubAnim(Attributes, "Decay Falloff"),					LightInf, "Color",				Sample,					udword(Gl));
			Status = AddPointController(GetSubAnim(Attributes, "Shadow Color"),				LightInf, "Shadow Color",		SampleShadowColor,		udword(Gl));
	//		Status = AddController(GetSubAnim(Attributes, "Atmosphere Opacity"),			LightInf, "Color",				Sample,					udword(Gl));
	//		Status = AddController(GetSubAnim(Attributes, "Atmosphere Color Amount")	,	LightInf, "Color",				Sample,					udword(Gl));
			Status = AddFloatController(GetSubAnim(Attributes, "Shadow Density"),			LightInf, "Shadow Multiplier",	SampleShadowDensity,	udword(Gl));

			// Restore anim export status
			mSettings->mSingleFrame = Saved;
		}
	}
	return true;
}
