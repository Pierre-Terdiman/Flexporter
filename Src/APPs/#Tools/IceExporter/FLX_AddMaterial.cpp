///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a material.
 *	\file		FLX_AddMaterial.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

// Sampling callbacks
static float SampleOpacity	(udword time_value, udword user_data)		{ StdMat* Std = (StdMat*)user_data; return Std->GetOpacity(time_value);	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Exports materials from a node.
 *	Materials from mCurrentNbMtls to mStats.NbMaterials are stored in the global info array.
 *	\param		node	[in] a MAX node
 *	\return		true if sucess.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddMetaMaterial(INode* node)
{
	// Dialog-related code
	SetExtStatus("exporting materials...");

	// Catch the node's material pointer. This is possibly a MetaMaterial.
	Mtl* Material = node->GetMtl();

	// Update members: 
	// - I save current #materials in mCurrentNbMtls. Used later in ComputeMaterialID;
	// - Reset #submaterials for this material.
	mCurrentNbMtls = mStats.mNbMaterials;
	mNbSubMtls = 0;

	// Check the meta-material is valid (else some wrong output messages will get printed)
	if(!Material)
	{
		mArrayLog.StoreASCII("WARNING: node %s has no material\n", node->GetName());
		return true;	// This is a valid behaviour, ex you create a simple object and directly export it
	}

	return _AddMaterial(Material);
}

#ifdef FLEXPORTER_MAX5
	// ######## UGLY !
	// But the material definition isn't in a SDK header ! Bad, MAX, bad !
	class BakeShell : public Mtl, public IReshading 
	{
		public:
			IParamBlock2			*mpBlock; 			// ref #0
			Mtl						*mpOrigMtl;			// ref #1
			Mtl						*mpBakedMtl;		// ref #2		
	};
#endif
#ifdef FLEXPORTER_MAX6
	#error	Adapt me!
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Exports a material and possible submaterials.
 *	\param		material	[in] a MAX material
 *	\return		true if sucess.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::_AddMaterial(Mtl* material)
{
	if(material)
	{
#ifdef FLEXPORTER_MAX5
		if(material->ClassID()==Class_ID(BAKE_SHELL_CLASS_ID, 0))
		{
			BakeShell* BS = (BakeShell*)material;
			_AddMaterial(BS->mpBakedMtl);
		}
		else
#endif
		if(material->ClassID()==Class_ID(DMTL_CLASS_ID, 0))
		{
			AddStdMaterial(material);
		}
		else if(material->ClassID()==Class_ID(MULTI_CLASS_ID, 0))
		{
			// Get number of submaterials
			sdword NbSubMtls = (sdword)material->NumSubMtls();
			// Loop through submaterials
			for(sdword i=0;i<NbSubMtls;i++)
			{
				// Get current submaterial
				Mtl* SubMat = material->GetSubMtl(i);
				// Recurse through it
				if(!_AddMaterial(SubMat))	return false;
			}
			mNbSubMtls+=NbSubMtls;
		}
		// ClassID is unknown
		else AddDefaultMaterial();
	}
	// No material defined => can make things break at this point
	else mArrayLog.StoreASCII("WARNING: found a ""None"" material - might break the material ID mapping - please convert it to ""Standard""\n");

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Exports a standard material.
 *	\param		material	[in] a MAX material
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MAXConverter::AddStdMaterial(Mtl* material)
{
	// Convert to standard material
	StdMat*	StandardMaterial = (StdMat*)material;

	// GET INFOS
	// Get debug infos
	// Get total number of possible texmaps.
	// Should be 11 in MAX 2.5:
	// -Ambient Color
	// -Diffuse Color
	// -Specular Color
	// -Filter Color
	// -Shininess
	// -Shining Strength
	// -SelfIllum
	// -Opacity
	// -Opacity Falloff
	// -WireSize
	// -IOR
	// Should be 12 in MAX 3:
	// - Displacement
	sdword NumSubTexmaps = (sdword)material->NumSubTexmaps();

	// Get total number of possible materials. Supposed to be null for a StdMaterial.
	sdword NumSubMaterials = (sdword)material->NumSubMtls();
	if(NumSubMaterials)	mArrayLog.StoreASCII("Strange! I found a MAX Standard Material which contains SubMaterials. (?)\n");

	// Keep track of the material
	mGlobalInfo.GetMaterialContainer().AddMaterial(material);

	// Important note:
	// Here I keep track of all materials, even non-referenced ones. Example: say we have a simple cube may using a single StdMaterial.
	// This StdMaterial may be part of a MetaMaterial as many other StdMaterials. Hence, we have a MAX scene with many StdMaterials, but
	// one of them only is really used. How do we know which materials are useful, and which aren't? Thanks to material IDs. I could have
	// had a look at material IDs before exporting materials, maybe. But getting the right material ID depends on mNbSubMtls, which is
	// computed here! Tricky. My solution (worse? better? about the same amount of work) is to keep track of all materials in the first
	// place. Then, according to material IDs we find, we may or may not export just the useful ones. The conversion process is delayed
	// until this final decision...

	// Update member
	mStats.mNbMaterials++;

// Now obsolete
//	if(mStats.mNbMaterials==IMAX_MAX_MATERIALS)	MessageBox(null, "IMAX_MAX_MATERIALS limit has been reached!\nHint: increase value and recompile.", "Oops", 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Exports a default material.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MAXConverter::AddDefaultMaterial()
{
	// Default material is INVALID_ID
	mGlobalInfo.GetMaterialContainer().AddMaterial((Mtl*)INVALID_ID);

	// Update member
	mStats.mNbMaterials++;

// Now obsolete
//	if(mStats.mNbMaterials==IMAX_MAX_MATERIALS)	MessageBox(null, "IMAX_MAX_MATERIALS limit has been reached!\nHint: increase value and recompile.", "Oops", 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Computes a material ID.
 *	MAX guys must've had a sordid mental hangover the day they dealt with material IDs....
 *	\param		node		[in] a MAX node
 *	\param		mesh		[in] a MAX mesh
 *	\param		i			[in] face number
 *	\param		relative	[in] true to compute relative index, else absolute index
 *	\return		valid material ID
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long MAXConverter::ComputeMaterialID(INode* node, ::Mesh* mesh, long i, bool relative)
{
	// Get node's material
	Mtl* Material = node->GetMtl();

	// No material for this node => MatID is set to -1
	if(!Material) return MATID_NO_MATERIAL;

	// Let's be sure mNbSubMtls is not zero, since we'll use it for a modulo.
	if(!mNbSubMtls) return relative ? 0 : mCurrentNbMtls;
/*
for(udword p=0;p<mNbSubMtls;p++)
{
	TSTR Test2 = Material->GetSubMtlSlotName(p);
}
*/
	// Compute correct relative material ID... [don't look at me, blame it on MAX! That's the official right way to do it]
	long MatID = mesh ? (long)(mesh->faces[i].getMatID() % mNbSubMtls) : 0;

	// ...and make it absolute. mCurrentNbMtls is the #materials before exporting current node.
	if(!relative)	MatID+=mCurrentNbMtls;

	// Return valid material ID
	return MatID;
}

udword MAXConverter::RemapMaterialID(udword matid)
{
	if(matid==MATID_NO_MATERIAL) return MATID_NO_MATERIAL;

	// Catch remapped ID
	udword RemapID = mGlobalInfo.GetMaterialContainer().GetMaterials()[matid].mRemapID;
	if(RemapID==INVALID_ID)
	{
		// Here we have a new MAX material to export, located at "matid" in the global array.
		// We must search the already exported materials for the same MAX material pointer.
		// If we found it, we can assign the previous ID to current material.

		// Search already exported materials
		udword FoundID = INVALID_ID;
		udword* um = mUsedMaterials.GetEntries();
		if(mSettings->mMaterialSharing)
		{
			for(udword i=0;i<mUsedMaterials.GetNbEntries()/3;i++)
			{
				Mtl* MaxMat = (Mtl*)*um++;
				udword PrevID = *um++;	// ### ain't that the same as i ?
				udword OrigID = *um++;
				if(SameMaterial(MaxMat, mGlobalInfo.GetMaterialContainer().GetMaterials()[matid].mMAXMaterial))
				{
					FoundID = PrevID;
					break;
				}
			}
		}
		else
		{
			// Old code path up to version 1.15
			for(udword i=0;i<mUsedMaterials.GetNbEntries()/3;i++)
			{
				Mtl* MaxMat = (Mtl*)*um++;
				udword PrevID = *um++;	// ### ain't that the same as i ?
				udword OrigID = *um++;
				if(MaxMat==mGlobalInfo.GetMaterialContainer().GetMaterials()[matid].mMAXMaterial)
				{
					FoundID = PrevID;
					break;
				}
			}
		}

		if(FoundID==INVALID_ID)
		{
			// Current MAX material is a new one :

			// 1) Store used material
			mUsedMaterials.Add((udword)mGlobalInfo.GetMaterialContainer().GetMaterials()[matid].mMAXMaterial);
			mUsedMaterials.Add(mNbUsedMaterials);
			mUsedMaterials.Add(matid);

			// 2) Assign a new ID
			mGlobalInfo.GetMaterialContainer().GetMaterials()[matid].mRemapID = mNbUsedMaterials;
			matid = mNbUsedMaterials;

			// 3) Update number of used materials
			mNbUsedMaterials++;
		}
		// Else current material has already been exported => return previously assigned ID
		else matid = FoundID;
	}
	else matid = RemapID;

	return matid;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This method is called in the end, to convert all used materials.
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::ConvertMaterials()
{
	// Status message
	SetStatus("converting materials...");

	udword NbMaterials = mUsedMaterials.GetNbEntries()/3;
	if(NbMaterials)
	{
		// Update log file
		mArrayLog.StoreASCII("\nConverting %d materials...\n", NbMaterials);

		// Loop through used materials and convert them
		udword* um = mUsedMaterials.GetEntries();
		for(udword i=0;i<NbMaterials;i++)
		{
			Mtl* MaxMat = (Mtl*)*um++;
			udword PrevID = *um++;
			udword OrigID = *um++;
			if(!ConvertStdMaterial(OrigID, PrevID))	return false;
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Converts a MAX StdMaterial to correct format.
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::ConvertStdMaterial(udword mtlnb, udword trueid)
{
	// Catch input material
	Mtl* Material = mGlobalInfo.GetMaterialContainer().GetMaterials()[mtlnb].mMAXMaterial;

	// Take care of default material
	if(udword(Material)==INVALID_ID)	return ConvertDefaultMaterial(trueid);

	// Get time info back
	long		StartTime		= mTimeInfo.mStartTime;
	long		EndTime			= mTimeInfo.mEndTime;
	long		DeltaTime		= mTimeInfo.mDeltaTime;
	long		FrameRate		= mTimeInfo.mFrameRate;
	TimeValue	CurrentTime		= mTimeInfo.mCurrentTime;

	// Convert to standard material
	StdMat*	StandardMaterial = (StdMat*)Material;

	// Get basic infos
	static const char* Noname = "(unnamed)";
	const char*	MaterialName		=			Material			->GetName();
	if(!MaterialName)	MaterialName = Noname;

	// Update log file
	mArrayLog.StoreASCII("Converting material: %s\n", MaterialName);

	// Get textures
	Texmap*	AmbientMap				=			Material			->GetSubTexmap(ID_AM);											// Ambient texture map
	Texmap*	DiffuseMap				=			Material			->GetSubTexmap(ID_DI);											// Diffuse texture map
	Texmap*	SpecularMap				=			Material			->GetSubTexmap(ID_SP);											// Specular texture map
	Texmap*	ShininessMap			=			Material			->GetSubTexmap(ID_SH);											// Shininess texture map
	Texmap*	ShiningStrengthMap		=			Material			->GetSubTexmap(ID_SS);											// Shining Strength texture map
	Texmap*	SelfIllumMap			=			Material			->GetSubTexmap(ID_SI);											// Self Illum texture map
	Texmap*	OpacityMap				=			Material			->GetSubTexmap(ID_OP);											// Opacity texture map
	Texmap*	FilterMap				=			Material			->GetSubTexmap(ID_FI);											// Filter texture map
	Texmap*	BumpMap					=			Material			->GetSubTexmap(ID_BU);											// Bump texture map
	Texmap*	ReflexionMap			=			Material			->GetSubTexmap(ID_RL);											// Reflexion texture map
	Texmap*	RefractionMap			=			Material			->GetSubTexmap(ID_RR);											// Refraction texture map
	Texmap*	DisplacementMap			=			Material			->GetSubTexmap(ID_DP);											// Displacement texture map

	// Get amounts
	float	AmbientCoeff			=			StandardMaterial	->GetTexmapAmt(ID_AM, CurrentTime);								// Ambient texture %
	float	DiffuseCoeff			=			StandardMaterial	->GetTexmapAmt(ID_DI, CurrentTime);								// Diffuse tetxure %
	float	SpecularCoeff			=			StandardMaterial	->GetTexmapAmt(ID_SP, CurrentTime);								// Specular texture %
	float	ShininessCoeff			=			StandardMaterial	->GetTexmapAmt(ID_SH, CurrentTime);								// Shininess texture %
	float	ShiningStrengthCoeff	=			StandardMaterial	->GetTexmapAmt(ID_SS, CurrentTime);								// Shining Strength texture %
	float	SelfIllumCoeff			=			StandardMaterial	->GetTexmapAmt(ID_SI, CurrentTime);								// Self Illum texture %
	float	OpacityCoeff			=			StandardMaterial	->GetTexmapAmt(ID_OP, CurrentTime);								// Opacity texture %
	float	FilterCoeff				=			StandardMaterial	->GetTexmapAmt(ID_FI, CurrentTime);								// Filter texture %
	float	BumpCoeff				=			StandardMaterial	->GetTexmapAmt(ID_BU, CurrentTime);								// Bump texture %
	float	ReflexionCoeff			=			StandardMaterial	->GetTexmapAmt(ID_RL, CurrentTime);								// Reflexion texture %
	float	RefractionCoeff			=			StandardMaterial	->GetTexmapAmt(ID_RR, CurrentTime);								// Refraction texture %
	float	DisplacementCoeff		=			StandardMaterial	->GetTexmapAmt(ID_DP, CurrentTime);								// Displacement texture %

	// Get colors
	Color	MtlAmbientColor			=			StandardMaterial	->GetAmbient	(CurrentTime);									// Material Ambient Color
	Color	MtlDiffuseColor			=			StandardMaterial	->GetDiffuse	(CurrentTime);									// Material Diffuse Color
	Color	MtlSpecularColor		=			StandardMaterial	->GetSpecular	(CurrentTime);									// Material Specular Color
	Color	MtlFilterColor			=			StandardMaterial	->GetFilter		(CurrentTime);									// Material Filter Color

	// Get material static properties
	long	Shading					= (long)	StandardMaterial	->GetShading();													// Material Shading (int)
	bool	Soften					=			StandardMaterial	->GetSoften()!=0;												// MaterialSoften (BOOL)
	bool	FaceMap					=			StandardMaterial	->GetFaceMap()!=0;												// MaterialFaceMap (BOOL)
	bool	TwoSided				=			StandardMaterial	->GetTwoSided()!=0;												// MaterialTwoSided (BOOL)
	bool	Wire					=			StandardMaterial	->GetWire()!=0;													// MaterialWire (BOOL)
	bool	WireUnits				=			StandardMaterial	->GetWireUnits()!=0;											// MaterialWireUnits (BOOL)
	bool	FalloffOut				=			StandardMaterial	->GetFalloffOut()!=0;											// MaterialFalloffOut (BOOL)
	long	TransparencyType		= (long)	StandardMaterial	->GetTransparencyType();										// MaterialTransparencyType (int)

	// Get material dynamic properties
	float	Shininess				=			StandardMaterial	->GetShininess		(CurrentTime);								// MaterialShininess (float)
	float	ShiningStrength			=			StandardMaterial	->GetShinStr		(CurrentTime);								// MaterialShiningStrength (float)
	float	SelfIllum				=			StandardMaterial	->GetSelfIllum		(CurrentTime);								// MaterialSelfIllum (float)
	float	Opacity					=			StandardMaterial	->GetOpacity		(CurrentTime);								// MaterialOpacity (float)
	float	OpaFalloff				=			StandardMaterial	->GetOpacFalloff	(CurrentTime);								// MaterialOpacityFalloff (float)
	float	WireSize				=			StandardMaterial	->GetWireSize		(CurrentTime);								// MaterialWireSize (float)
	float	IOR						=			StandardMaterial	->GetIOR			(CurrentTime);								// MaterialIOR (float)

	// Get material dynamic properties
	float	Bounce					=			Material			->GetDynamicsProperty(CurrentTime, 0, DYN_BOUNCE);				// Material bounce (float)
	float	StaticFriction			=			Material			->GetDynamicsProperty(CurrentTime, 0, DYN_STATIC_FRICTION);		// Material Static Friction (float)
	float	SlidingFriction			=			Material			->GetDynamicsProperty(CurrentTime, 0, DYN_SLIDING_FRICTION);	// Material Sliding Friction (float)

	// Some more transfos
	if(mSettings->mForceAmbient)	MtlAmbientColor		= Color(1.0f, 1.0f, 1.0f);
	if(mSettings->mForceDiffuse)	MtlDiffuseColor		= Color(1.0f, 1.0f, 1.0f);
	if(mSettings->mForceSpecular)	MtlSpecularColor	= Color(1.0f, 1.0f, 1.0f);
	if(mSettings->mForceFilter)		MtlFilterColor		= Color(0.0f, 0.0f, 0.0f);

	// 1.15: enhanced self-illum
	bool	SelfIllumOn				=			Material			->GetSelfIllumColorOn()!=0;
	float	SelfIllumValue			=			Material			->GetSelfIllum();
	Color	SelfIllumColor			=			Material			->GetSelfIllumColor();

	// Create info structure
	MaterialDescriptor MtlInf;

	// That's a new material
	MtlInf.mObjectID				=			trueid;
	MtlInf.mName					=			(ubyte*)MaterialName;

	MtlInf.mBounce					=			Bounce;
	MtlInf.mStaticFriction			=			StaticFriction;
	MtlInf.mSlidingFriction			=			SlidingFriction;

	MtlInf.mMtlAmbientColor			=			NakedPoint(MtlAmbientColor.r,		MtlAmbientColor.g,	MtlAmbientColor.b);
	MtlInf.mMtlDiffuseColor			=			NakedPoint(MtlDiffuseColor.r,		MtlDiffuseColor.g,	MtlDiffuseColor.b);
	MtlInf.mMtlSpecularColor		=			NakedPoint(MtlSpecularColor.r,	MtlSpecularColor.g,	MtlSpecularColor.b);
	MtlInf.mMtlFilterColor			=			NakedPoint(MtlFilterColor.r,		MtlFilterColor.g,	MtlFilterColor.b);

	MtlInf.mAmbientCoeff			=			AmbientCoeff;
	MtlInf.mDiffuseCoeff			=			DiffuseCoeff;
	MtlInf.mSpecularCoeff			=			SpecularCoeff;
	MtlInf.mShininessCoeff			=			ShininessCoeff;
	MtlInf.mShiningStrengthCoeff	=			ShiningStrengthCoeff;
	MtlInf.mSelfIllumCoeff			=			SelfIllumCoeff;
	MtlInf.mOpacityCoeff			=			OpacityCoeff;
	MtlInf.mFilterCoeff				=			FilterCoeff;
	MtlInf.mBumpCoeff				=			BumpCoeff;
	MtlInf.mReflexionCoeff			=			ReflexionCoeff;
	MtlInf.mRefractionCoeff			=			RefractionCoeff;
	MtlInf.mDisplacementCoeff		=			DisplacementCoeff;

	MtlInf.mShading					=			Shading;
	MtlInf.mSoften					=			Soften;
	MtlInf.mFaceMap					=			FaceMap;
	MtlInf.mTwoSided				=			TwoSided;
	MtlInf.mWire					=			Wire;
	MtlInf.mWireUnits				=			WireUnits;
	MtlInf.mFalloffOut				=			FalloffOut;
	MtlInf.mTransparencyType		=			TransparencyType;

	MtlInf.mShininess				=			Shininess;
	MtlInf.mShiningStrength			=			ShiningStrength;
	MtlInf.mSelfIllum				=			SelfIllum;
	MtlInf.mOpacity					=			Opacity;
	MtlInf.mOpaFalloff				=			OpaFalloff;
	MtlInf.mWireSize				=			WireSize;
	MtlInf.mIOR						=			IOR;

	// 1.15:
	MtlInf.mSelfIllumOn				=			SelfIllumOn;
	MtlInf.mSelfIllumValue			=			SelfIllumValue;
	MtlInf.mSelfIllumColor			=			NakedPoint(SelfIllumColor.r, SelfIllumColor.g, SelfIllumColor.b);

	// 1.17:
	GetShaderFile(Material, MtlInf.mShaderFile);
	GetUserProps(Material, MtlInf.mUserProps);

	// Maps

	if(mSettings->mKeepAmbientTexture && StandardMaterial->MapEnabled(ID_AM))
	{
		// Ambient map
		TextureBlend TB;

		TB.mRGBMap					= AmbientMap;
		TB.mRGBMapCoeff				= AmbientCoeff;
		TB.mRGBDefault				= MtlAmbientColor;

		TB.mAlphaMap				= null;
		TB.mAlphaMapCoeff			= 0.0f;
		TB.mAlphaDefault.r			= 1.0f;

		MtlInf.mAmbientMapID		= AddTexture(TB);
	}

	if(mSettings->mKeepDiffuseTexture && StandardMaterial->MapEnabled(ID_DI))
	{
		// Diffuse map
		TextureBlend TB;

		TB.mRGBMap					= DiffuseMap;
		TB.mRGBMapCoeff				= DiffuseCoeff;
		TB.mRGBDefault				= MtlDiffuseColor;

		TB.mAlphaMap				= null;
		TB.mAlphaMapCoeff			= 0.0f;
		TB.mAlphaDefault.r			= 1.0f;

		if(mSettings->mKeepOpacityTexture && mSettings->mOpacityInAlpha)
		{
			TB.mAlphaMap			= OpacityMap;
			TB.mAlphaMapCoeff		= OpacityCoeff;
			TB.mAlphaDefault.r		= 1.0f;
		}
		MtlInf.mDiffuseMapID		= AddTexture(TB);

		// Get crop values from the diffuse texmap
		Matrix3 UVTrans;
		GetCropValues(DiffuseMap, MtlInf.mTransform.mCValues, UVTrans, CurrentTime);
		CopyMemory(&MtlInf.mTransform.mTMtx, &UVTrans, sizeof(TextureMatrix));
		GetTextureTiling(DiffuseMap, MtlInf.mTransform.mTextureTilingU, MtlInf.mTransform.mTextureTilingV);
	}

	if(mSettings->mKeepSpecularTexture && StandardMaterial->MapEnabled(ID_SP))
	{
		// Specular map
		TextureBlend TB;

		TB.mRGBMap					= SpecularMap;
		TB.mRGBMapCoeff				= SpecularCoeff;
		TB.mRGBDefault				= MtlSpecularColor;

		TB.mAlphaMap				= null;
		TB.mAlphaMapCoeff			= 0.0f;
		TB.mAlphaDefault.r			= 1.0f;

		MtlInf.mSpecularMapID		= AddTexture(TB);
	}

	if(mSettings->mKeepShininessTexture && StandardMaterial->MapEnabled(ID_SH))
	{
		// Shininess map
		TextureBlend TB;

		TB.mRGBMap					= ShininessMap;
		TB.mRGBMapCoeff				= ShininessCoeff;
		TB.mRGBDefault				= Color(1.0f, 1.0f, 1.0f);

		TB.mAlphaMap				= null;
		TB.mAlphaMapCoeff			= 0.0f;
		TB.mAlphaDefault.r			= 1.0f;

		MtlInf.mShininessMapID		= AddTexture(TB);
	}

	if(mSettings->mKeepShiningStrengthTexture && StandardMaterial->MapEnabled(ID_SS))
	{
		// ShiningStrength map
		TextureBlend TB;

		TB.mRGBMap					= ShiningStrengthMap;
		TB.mRGBMapCoeff				= ShiningStrengthCoeff;
		TB.mRGBDefault				= Color(1.0f, 1.0f, 1.0f);

		TB.mAlphaMap				= null;
		TB.mAlphaMapCoeff			= 0.0f;
		TB.mAlphaDefault.r			= 1.0f;

		MtlInf.mShiningStrengthMapID= AddTexture(TB);
	}

	if(mSettings->mKeepSelfIllumTexture && StandardMaterial->MapEnabled(ID_SI))
	{
		// SelfIllum map
		TextureBlend TB;

		TB.mRGBMap					= SelfIllumMap;
		TB.mRGBMapCoeff				= SelfIllumCoeff;
		TB.mRGBDefault				= Color(1.0f, 1.0f, 1.0f);

		TB.mAlphaMap				= null;
		TB.mAlphaMapCoeff			= 0.0f;
		TB.mAlphaDefault.r			= 1.0f;

		MtlInf.mSelfIllumMapID		= AddTexture(TB);
	}

	if(!mSettings->mOpacityInAlpha && StandardMaterial->MapEnabled(ID_OP) && mSettings->mKeepOpacityTexture)
	{
		// Opacity map
		TextureBlend TB;

		TB.mRGBMap					= OpacityMap;
		TB.mRGBMapCoeff				= OpacityCoeff;
		TB.mRGBDefault				= Color(1.0f, 1.0f, 1.0f);

		TB.mAlphaMap				= null;
		TB.mAlphaMapCoeff			= 0.0f;
		TB.mAlphaDefault.r			= 1.0f;

		MtlInf.mOpacityMapID		= AddTexture(TB);
	}

	if(mSettings->mKeepFilterTexture && StandardMaterial->MapEnabled(ID_FI))
	{
		// Filter map
		TextureBlend TB;

		TB.mRGBMap					= FilterMap;
		TB.mRGBMapCoeff				= FilterCoeff;
		TB.mRGBDefault				= MtlFilterColor;

		TB.mAlphaMap				= null;
		TB.mAlphaMapCoeff			= 0.0f;
		TB.mAlphaDefault.r			= 1.0f;

		MtlInf.mFilterMapID			= AddTexture(TB);
	}

	if(mSettings->mKeepBumpTexture && StandardMaterial->MapEnabled(ID_BU))
	{
		// Bump map
		TextureBlend TB;

		TB.mRGBMap					= BumpMap;
		TB.mRGBMapCoeff				= BumpCoeff;
		TB.mRGBDefault				= Color(1.0f, 1.0f, 1.0f);

		TB.mAlphaMap				= null;
		TB.mAlphaMapCoeff			= 0.0f;
		TB.mAlphaDefault.r			= 1.0f;

		MtlInf.mBumpMapID			= AddTexture(TB);
	}

	if(mSettings->mKeepReflexionTexture && StandardMaterial->MapEnabled(ID_RL))
	{
		// Reflexion map
		TextureBlend TB;

		TB.mRGBMap					= ReflexionMap;
		TB.mRGBMapCoeff				= ReflexionCoeff;
		TB.mRGBDefault				= Color(1.0f, 1.0f, 1.0f);

		TB.mAlphaMap				= null;
		TB.mAlphaMapCoeff			= 0.0f;
		TB.mAlphaDefault.r			= 1.0f;

		MtlInf.mReflexionMapID		= AddTexture(TB);
	}

	if(mSettings->mKeepRefractionTexture && StandardMaterial->MapEnabled(ID_RR))
	{
		// Refraction map
		TextureBlend TB;

		TB.mRGBMap					= RefractionMap;
		TB.mRGBMapCoeff				= RefractionCoeff;
		TB.mRGBDefault				= Color(1.0f, 1.0f, 1.0f);

		TB.mAlphaMap				= null;
		TB.mAlphaMapCoeff			= 0.0f;
		TB.mAlphaDefault.r			= 1.0f;

		MtlInf.mRefractionMapID		= AddTexture(TB);
	}

	if(mSettings->mKeepDisplacementTexture && StandardMaterial->MapEnabled(ID_DP))
	{
		// Displacement map
		TextureBlend TB;

		TB.mRGBMap					= DisplacementMap;
		TB.mRGBMapCoeff				= DisplacementCoeff;
		TB.mRGBDefault				= Color(1.0f, 1.0f, 1.0f);

		TB.mAlphaMap				= null;
		TB.mAlphaMapCoeff			= 0.0f;
		TB.mAlphaDefault.r			= 1.0f;

		MtlInf.mDisplacementMapID	= AddTexture(TB);
	}

	// 1.17 - export custom textures
	{
		Texmap* DecalMap = GetDecalTexture(Material);
		if(DecalMap)
		{
			// Decal map
			TextureBlend TB;

			TB.mRGBMap					= DecalMap;
			TB.mRGBMapCoeff				= 1.0f;
			TB.mRGBDefault				= Color(1.0f, 1.0f, 1.0f);

			TB.mAlphaMap				= null;
			TB.mAlphaMapCoeff			= 0.0f;
			TB.mAlphaDefault.r			= 1.0f;

			MtlInf.mDecalMapID			= AddTexture(TB);
		}

		Texmap* DetailMap = GetDetailTexture(Material);
		if(DetailMap)
		{
			// Detail map
			TextureBlend TB;

			TB.mRGBMap					= DetailMap;
			TB.mRGBMapCoeff				= 1.0f;
			TB.mRGBDefault				= Color(1.0f, 1.0f, 1.0f);

			TB.mAlphaMap				= null;
			TB.mAlphaMapCoeff			= 0.0f;
			TB.mAlphaDefault.r			= 1.0f;

			MtlInf.mDetailMapID			= AddTexture(TB);
		}

		Texmap* BillboardMap = GetBillboardTexture(Material);
		if(BillboardMap)
		{
			// Billboard map
			TextureBlend TB;

			TB.mRGBMap					= BillboardMap;
			TB.mRGBMapCoeff				= 1.0f;
			TB.mRGBDefault				= Color(1.0f, 1.0f, 1.0f);

			TB.mAlphaMap				= null;
			TB.mAlphaMapCoeff			= 0.0f;
			TB.mAlphaDefault.r			= 1.0f;

			MtlInf.mBillboardMapID		= AddTexture(TB);
		}
	}

	// Perform format-dependent export
	SetExtStatus("calling export plug-in...");
	if(mExporter)	mExporter->ExportMaterial(MtlInf);
	DumpLogString();

	// Animation
	// -Attributes
	if(mSettings->mExpMtlCtrl)
	{
		SetExtStatus("exporting material attributes controllers...");
		bool Status;

		// Enable anim export temporarily
	//	bool Saved = mSettings->mSingleFrame;
	//	if(mProps.mExportCamCtrl)	mSettings->mSingleFrame = false;

		Animatable* Maps = GetSubAnim(Material, "Maps");
		if(Maps)
		{
			Animatable* Anim;
			Anim = GetSubAnim(Maps, "pipo");
		}

		Animatable* Basic = GetSubAnim(Material, "Shader Basic Parameters");
		if(Basic)
		{
			Animatable* Anim;
			Anim = GetSubAnim(Basic, "Parameters");
			if(Anim)	Anim = GetSubAnim(Anim, "Ambient Color");
			if(Anim)	Anim = GetSubAnim(Anim, "Diffuse Color");
			if(Anim)	Anim = GetSubAnim(Anim, "Specular Color");
			if(Anim)	Anim = GetSubAnim(Anim, "Self-Illumination");
			if(Anim)	Anim = GetSubAnim(Anim, "Self-Illum Color");
			if(Anim)	Anim = GetSubAnim(Anim, "Specular Level");
			if(Anim)	Anim = GetSubAnim(Anim, "Glossiness");
			if(Anim)	Anim = GetSubAnim(Anim, "Soften");
		}

		Animatable* Extended = GetSubAnim(Material, "Extended Parameters");
		if(Extended)
		{
			Status = AddFloatController(GetSubAnim(Extended, "Opacity"),	&MtlInf, "Opacity",	SampleOpacity,	udword(StandardMaterial));

	/*		GetSubAnim(Extended, "Opacity");
			GetSubAnim(Extended, "Filter Color");
			GetSubAnim(Extended, "Falloff");
			GetSubAnim(Extended, "Index of Refraction");
			GetSubAnim(Extended, "Wire Size");
			GetSubAnim(Extended, "Dim Level");
			GetSubAnim(Extended, "Reflection Level");
	*/
		}

		Animatable* SuperSamp = GetSubAnim(Material, "SuperSampling");
		if(SuperSamp)
		{
			Animatable* Anim;
			Anim = GetSubAnim(SuperSamp, "Sampler Quality");
			if(Anim)	Anim = GetSubAnim(Anim, "pipo");
			Anim = GetSubAnim(SuperSamp, "Sampler Enable");
			if(Anim)	Anim = GetSubAnim(Anim, "pipo");
		}

		Animatable* Dyna = GetSubAnim(Material, "Dynamics Properties");
		if(Dyna)
		{
			Animatable* Anim;
			Anim = GetSubAnim(Dyna, "Bounce Coefficient");
			if(Anim)	Anim = GetSubAnim(Anim, "pipo");
			Anim = GetSubAnim(Dyna, "Static Friction");
			if(Anim)	Anim = GetSubAnim(Anim, "pipo");
			Anim = GetSubAnim(Dyna, "Sliding Friction");
			if(Anim)	Anim = GetSubAnim(Anim, "pipo");
		}

		// Restore anim export status
	//	mSettings->mSingleFrame = Saved;
	}
	return true;
}

/*
Maps
 Amount
Ambient Color: None
 Amount
Diffuse Color: None
 Amount
Specular Color: None
 Amount
Specular Level: None
 Amount
Glossiness: None
 Amount
Self-Illumination: None
 Amount
Opacity: None
 Amount
Filter Color: None
 Amount
Bump: None
 Amount
Reflection: None
 Amount
Refraction: None
 Amount
Displacement: None
 Amount
: None
 Amount
: None
 Amount
: None
 Amount
: None
 Amount
: None
 Amount
: None
 Amount
: None
 Amount
: None
 Amount
: None
 Amount
RGB Level: None
 Amount
RGB Offset: None
 Amount
Composite: None
Maps

Shader Basic Parameters
Parameters
Maps
Shader Basic Parameters
Extended Parameters
Opacity
Filter Color
Falloff
Index of Refraction
Wire Size
Dim Level
Reflection Level
Maps
Shader Basic Parameters
Extended Parameters
SuperSampling
Sampler Quality
Sampler Enable
Maps

Shader Basic Parameters

Extended Parameters

SuperSampling

Dynamics Properties
Bounce Coefficient
Static Friction
Sliding Friction
*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Converts a default StdMaterial to correct format.
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::ConvertDefaultMaterial(udword trueid)
{
	// Create info structure
	MaterialDescriptor MtlInf;

	MtlInf.mObjectID		= trueid;
	MtlInf.mName			= (ubyte*)"DefaultMaterial";
	MtlInf.mMtlDiffuseColor	= NakedPoint(1.0f, 1.0f, 1.0f);

	// Perform format-dependent export
	SetExtStatus("calling export plug-in...");
	if(mExporter)	mExporter->ExportMaterial(MtlInf);
	DumpLogString();

	return true;
}



// Material sharing

static char* GetTextureIdentifier(Texmap* texture)
{
	// Texture type
	if(IsKnownTexmap(texture)!=TEXMAP_BITMAP)	return null;

	// Bitmap data
	BitmapTex* RGBBitmapTex	= (BitmapTex*)texture;
	if(!RGBBitmapTex)	return null;

	return RGBBitmapTex->GetMapName();
}

bool MAXConverter::SameMaterial(Mtl* material0, Mtl* material1)
{
	if(material0==material1)	return true;
	// From this point, both can't be equal
	if(!material0)	return false;	// material1 can't be null simultaneously
	if(!material1)	return false;	// material0 can't be null simultaneously
	// Take care of default material
	if(udword(material0)==INVALID_ID)	return false;	// the other one is different
	if(udword(material1)==INVALID_ID)	return false;	// the other one is different

	// Get time info back
	long		StartTime		= mTimeInfo.mStartTime;
	long		EndTime			= mTimeInfo.mEndTime;
	long		DeltaTime		= mTimeInfo.mDeltaTime;
	long		FrameRate		= mTimeInfo.mFrameRate;
	TimeValue	CurrentTime		= mTimeInfo.mCurrentTime;

	// Convert to standard materials
	StdMat*	StandardMaterial0 = (StdMat*)material0;
	StdMat*	StandardMaterial1 = (StdMat*)material1;

	// Discarded fields:
	// - material name

	// Check textures if needed

#define CHECK_TEXTURE(flag, id)\
	if(mSettings->flag && StandardMaterial0->MapEnabled(id) && StandardMaterial1->MapEnabled(id))\
	{\
		/*if(material0->GetSubTexmap(id) != material1->GetSubTexmap(id))	return false;*/\
		char* TID0 = GetTextureIdentifier(material0->GetSubTexmap(id));	\
		char* TID1 = GetTextureIdentifier(material1->GetSubTexmap(id));	\
		if(!AreEqual(TID0, TID1))	return false;\
		if(StandardMaterial0->GetTexmapAmt(id, CurrentTime) != StandardMaterial1->GetTexmapAmt(id, CurrentTime))	return false;\
	}

	CHECK_TEXTURE(mKeepAmbientTexture, ID_AM)
	CHECK_TEXTURE(mKeepDiffuseTexture, ID_DI)
	CHECK_TEXTURE(mKeepSpecularTexture, ID_SP)
	CHECK_TEXTURE(mKeepShininessTexture, ID_SH)
	CHECK_TEXTURE(mKeepShiningStrengthTexture, ID_SS)
	CHECK_TEXTURE(mKeepSelfIllumTexture, ID_SI)
	CHECK_TEXTURE(mKeepOpacityTexture, ID_OP)
	CHECK_TEXTURE(mKeepFilterTexture, ID_FI)
	CHECK_TEXTURE(mKeepBumpTexture, ID_BU)
	CHECK_TEXTURE(mKeepReflexionTexture, ID_RL)
	CHECK_TEXTURE(mKeepRefractionTexture, ID_RR)
	CHECK_TEXTURE(mKeepDisplacementTexture, ID_DP)

	// Check colors
	if(!mSettings->mForceAmbient && (StandardMaterial0->GetAmbient(CurrentTime) != StandardMaterial1->GetAmbient(CurrentTime)))	return false;
	if(!mSettings->mForceDiffuse && (StandardMaterial0->GetDiffuse(CurrentTime) != StandardMaterial1->GetDiffuse(CurrentTime)))	return false;
	if(!mSettings->mForceSpecular && (StandardMaterial0->GetSpecular(CurrentTime) != StandardMaterial1->GetSpecular(CurrentTime)))	return false;
	if(!mSettings->mForceFilter && (StandardMaterial0->GetFilter(CurrentTime) != StandardMaterial1->GetFilter(CurrentTime)))		return false;

	// Check material static properties
	if(StandardMaterial0->GetShading() != StandardMaterial1->GetShading())						return false;
	if(StandardMaterial0->GetSoften() != StandardMaterial1->GetSoften())						return false;
	if(StandardMaterial0->GetFaceMap() != StandardMaterial1->GetFaceMap())						return false;
	if(StandardMaterial0->GetTwoSided() != StandardMaterial1->GetTwoSided())					return false;
	if(StandardMaterial0->GetWire() != StandardMaterial1->GetWire())							return false;
	if(StandardMaterial0->GetWireUnits() != StandardMaterial1->GetWireUnits())					return false;
	if(StandardMaterial0->GetFalloffOut() != StandardMaterial1->GetFalloffOut())				return false;
	if(StandardMaterial0->GetTransparencyType() != StandardMaterial1->GetTransparencyType())	return false;

	// Check material dynamic properties
	if(StandardMaterial0->GetShininess(CurrentTime) != StandardMaterial1->GetShininess(CurrentTime))		return false;
	if(StandardMaterial0->GetShinStr(CurrentTime) != StandardMaterial1->GetShinStr(CurrentTime))			return false;
	if(StandardMaterial0->GetSelfIllum(CurrentTime) != StandardMaterial1->GetSelfIllum(CurrentTime))		return false;
	if(StandardMaterial0->GetOpacity(CurrentTime) != StandardMaterial1->GetOpacity(CurrentTime))			return false;
	if(StandardMaterial0->GetOpacFalloff(CurrentTime) != StandardMaterial1->GetOpacFalloff(CurrentTime))	return false;
	if(StandardMaterial0->GetWireSize(CurrentTime) != StandardMaterial1->GetWireSize(CurrentTime))			return false;
	if(StandardMaterial0->GetIOR(CurrentTime) != StandardMaterial1->GetIOR(CurrentTime))					return false;

	// Check material dynamic properties
	if(material0->GetDynamicsProperty(CurrentTime, 0, DYN_BOUNCE) != material1->GetDynamicsProperty(CurrentTime, 0, DYN_BOUNCE))	return false;
	if(material0->GetDynamicsProperty(CurrentTime, 0, DYN_STATIC_FRICTION) != material1->GetDynamicsProperty(CurrentTime, 0, DYN_STATIC_FRICTION))	return false;
	if(material0->GetDynamicsProperty(CurrentTime, 0, DYN_SLIDING_FRICTION) != material1->GetDynamicsProperty(CurrentTime, 0, DYN_SLIDING_FRICTION))	return false;

	// Check enhanced self-illum
	if(material0->GetSelfIllumColorOn() != material1->GetSelfIllumColorOn())	return false;
	if(material0->GetSelfIllum() != material1->GetSelfIllum())					return false;
	if(material0->GetSelfIllumColor() != material1->GetSelfIllumColor())		return false;

	return true;
}