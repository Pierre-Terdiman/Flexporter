///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains a vertex prelighter built as a Flexporter plug-in.
 *	\file		FLX_VertexPrelighter.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 25, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

#ifdef FLEXPORTER_SUPPORT_PRELIGHTER

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VertexPrelighter::VertexPrelighter()
{
	mPrelighter = null;
	mSmoothing	= 0;
	mShadows	= false;
	mSpecular	= false;
	mClamp		= true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VertexPrelighter::~VertexPrelighter()
{
	DELETESINGLE(mPrelighter);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Plug-in initialization method.
 *	This method is called once before each export. When this method is called, the mSettings and mFilename members of the base format are valid.
 *	\param		motion			[in] true for pure motion files.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VertexPrelighter::Init(bool motion)
{
	mPrelighter = new Prelighter;
	CHECKALLOC(mPrelighter);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Scene export method.
 *	This method is called once to export the scene global information.
 *	\param		maininfo		[in] main info structure
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VertexPrelighter::SetSceneInfo(const MainDescriptor& maininfo)
{
	// Setup ambient color
	if(mPrelighter)
	{
		mPrelighter->SetAmbientColor(RGBAColor(maininfo.mAmbColor.x, maininfo.mAmbColor.y, maininfo.mAmbColor.z));
	}

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
bool VertexPrelighter::ExportCamera(const CameraDescriptor& camera)
{
	// Setup viewpoint for speculat computations
	if(mPrelighter && mSpecular)
	{
		float x = camera.mPrs.mPosition.x;
		float y = camera.mPrs.mPosition.y;
		float z = camera.mPrs.mPosition.z;
		mPrelighter->SetViewpoint(Point(x,y,z));
	}

	return true;
}

	class MAXToD3DLightConverter
	{
		public:
			MAXToD3DLightConverter()
			{
				memset(this, 0, SIZEOFOBJECT);
			}
			~MAXToD3DLightConverter()
			{
			}

			bool ConvertLight(LightProps& props);

		// Current light parameters
			NakedPRS	mPrs;
			LType		mLightType;
			bool		mAffectDiffuse;
			bool		mAffectSpecular;
			NakedPoint	mColor;
			float		mIntensity;
			float		mAttenEnd;
			float		mHotSpot;
			float		mFallsize;
			bool		mUseAttenNear;
			bool		mUseAtten;
			char		mDecayType;
	};

	class MAXToD3DMaterialConverter
	{
		public:
			MAXToD3DMaterialConverter()
			{
				memset(this, 0, SIZEOFOBJECT);
			}
			~MAXToD3DMaterialConverter()
			{
			}

			bool ConvertMaterial(MaterialProps& props);

				bool				HasExtendedSelfIllum()	const
									{
										if(		mSelfIllumOn
											||	IR(mSelfIllumValue)
											||	IR(mSelfIllumColor.x)
											||	IR(mSelfIllumColor.y)
											||	IR(mSelfIllumColor.z)
											)
										return true;
										else return false;
									}

		// Current material parameters
			sdword		mDiffuseMapID;
			NakedPoint	mMtlAmbientColor;
			NakedPoint	mMtlDiffuseColor;
			NakedPoint	mMtlSpecularColor;
			NakedPoint	mMtlFilterColor;
			bool		mSelfIllumOn;
			float		mSelfIllumValue;
			NakedPoint	mSelfIllumColor;
			float		mShininess;
			float		mShiningStrength;
			float		mSelfIllum;
			float		mOpacity;
	};

	bool MAXToD3DLightConverter::ConvertLight(LightProps& props)
	{
		// Convert type
		props.mType = LIGHT_POINT;
		if(mLightType==FLX_LTYPE_TSPOT || mLightType==FLX_LTYPE_FSPOT)	props.mType = LIGHT_SPOT;
		if(mLightType==FLX_LTYPE_TDIR || mLightType==FLX_LTYPE_DIR)		props.mType = LIGHT_DIRECT;

		// Convert colors
							props.mAmbientColor		= RGBAColor(mColor.x*mIntensity,	mColor.y*mIntensity,	mColor.z*mIntensity,	0.0f);
		if(mAffectDiffuse)	props.mDiffuseColor		= RGBAColor(mColor.x*mIntensity,	mColor.y*mIntensity,	mColor.z*mIntensity,	0.0f);
		else				props.mDiffuseColor		= RGBAColor(1.0f,					1.0f,					1.0f,					0.0f);
		if(mAffectSpecular)	props.mSpecularColor	= RGBAColor(mColor.x*mIntensity,	mColor.y*mIntensity,	mColor.z*mIntensity,	0.0f);
		else				props.mSpecularColor	= RGBAColor(1.0f,					1.0f,					1.0f,					0.0f);

//		lc.mCastShadows		= light.mCastShadows;
//		lc.mLightUsed		= light.mLightUsed;

		// Convert position
		props.mPosition.x = mPrs.mPosition.x;
		props.mPosition.y = mPrs.mPosition.y;
		props.mPosition.z = mPrs.mPosition.z;

		// Convert rotation
		IceMaths::Quat Q;
		Q.p.x = mPrs.mRotation.x;
		Q.p.y = mPrs.mRotation.y;
		Q.p.z = mPrs.mRotation.z;
		Q.w = mPrs.mRotation.w;
		QuatToDir(Q, props.mDirection);

		// Convert range
		props.mRange = mAttenEnd;

		// Convert falloff
		props.mFalloff = 1.0f;

		// Convert attenuation
		if(mDecayType==0 || !mUseAtten || !mUseAttenNear)
		{
			// No attenuation ### à mettre en méthode de props
			props.mAttenuation0	= 1.0f;
			props.mAttenuation1	= 0.0f;
			props.mAttenuation2	= 0.0f;
		}
		else if(mDecayType==1 || mDecayType==2)
		{
			// Linear/Quadratic attenuation. A real pain in DX7.  ### à mettre en méthode de props
			props.mAttenuation0	= 1.0f;
			props.mAttenuation1	= 0.0001f / props.mRange;
			props.mAttenuation2	= 0.0f;
		}

		// Convert Theta / Phi
		props.mTheta	= mHotSpot * DEGTORAD;
		props.mPhi		= mFallsize * DEGTORAD;

		return true;
	}

	bool MAXToD3DMaterialConverter::ConvertMaterial(MaterialProps& props)
	{
		props.mAmbientColor.R	= mMtlAmbientColor.x;
		props.mAmbientColor.G	= mMtlAmbientColor.y;
		props.mAmbientColor.B	= mMtlAmbientColor.z;
		props.mAmbientColor.A	= 0.0f;
		props.mDiffuseColor.R	= mMtlDiffuseColor.x;
		props.mDiffuseColor.G	= mMtlDiffuseColor.y;
		props.mDiffuseColor.B	= mMtlDiffuseColor.z;
		props.mDiffuseColor.A	= 0.0f;
		props.mSpecularColor.R	= mMtlSpecularColor.x;
		props.mSpecularColor.G	= mMtlSpecularColor.y;
		props.mSpecularColor.B	= mMtlSpecularColor.z;
		props.mSpecularColor.A	= 0.0f;

		if(!HasExtendedSelfIllum())
		{
			// Use the old filter color
			props.mEmissiveColor.R	= mMtlFilterColor.x;
			props.mEmissiveColor.G	= mMtlFilterColor.y;
			props.mEmissiveColor.B	= mMtlFilterColor.z;
			props.mEmissiveColor.A	= 0.0f;
		}
		else
		{
			// Use the extended self-illum
			if(mSelfIllumOn)
			{
				// Use the color
				props.mEmissiveColor.R	= mSelfIllumColor.x;
				props.mEmissiveColor.G	= mSelfIllumColor.y;
				props.mEmissiveColor.B	= mSelfIllumColor.z;
				props.mEmissiveColor.A	= 0.0f;
			}
			else
			{
				// Use the value
				props.mEmissiveColor.R	= mSelfIllumValue;
				props.mEmissiveColor.G	= mSelfIllumValue;
				props.mEmissiveColor.B	= mSelfIllumValue;
				props.mEmissiveColor.A	= 0.0f;
			}
		}

		if(mDiffuseMapID==-1)
		{
			props.mAmbientColor.R	= 0.0f;
			props.mAmbientColor.G	= 0.0f;
			props.mAmbientColor.B	= 0.0f;
			props.mEmissiveColor.R	= 0.0f;
			props.mEmissiveColor.G	= 0.0f;
			props.mEmissiveColor.B	= 0.0f;
		}

		// The global opacity value must be stored in the diffuse color's alpha value (hardware constraint)
		props.mDiffuseColor.A	= mOpacity;
		props.mSpecularColor.R	*= mShiningStrength * (1.0f - mSelfIllum);
		props.mSpecularColor.G	*= mShiningStrength * (1.0f - mSelfIllum);
		props.mSpecularColor.B	*= mShiningStrength * (1.0f - mSelfIllum);
		props.mPower			= mShininess * 32.0f;
		return true;
	}


#pragma optimize( "", off )

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Light export method.
 *	This method is called once for each exported light.
 *	\param		light		[in] a structure filled with current light information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VertexPrelighter::ExportLight(const LightDescriptor& light)
{
	// Discard light if not used
	if(!light.mLightUsed)	return true;

	if(mPrelighter)
	{
		PrelighterLight* NewLight = mPrelighter->NewLight();
		if(!NewLight)	return false;

		PRELIGHTER_LIGHT_CREATE Create;
		Create.mName	= (const char*)light.mName;
		Create.mID		= light.mObjectID;
		//
			MAXToD3DLightConverter Cvt;
			Cvt.mPrs			= light.mPrs;
			Cvt.mLightType		= light.mLightType;
			Cvt.mAffectDiffuse	= light.mAffectDiffuse;
			Cvt.mAffectSpecular	= light.mAffectSpecular;
			Cvt.mColor			= light.mColor;
			Cvt.mIntensity		= light.mIntensity;
			Cvt.mAttenEnd		= light.mAttenEnd;
			Cvt.mHotSpot		= light.mHotSpot;
			Cvt.mFallsize		= light.mFallsize;
			Cvt.mUseAttenNear	= light.mUseAttenNear;
			Cvt.mUseAtten		= light.mUseAtten;
			Cvt.mDecayType		= light.mDecayType;
			Cvt.ConvertLight(Create.mProps);

		NewLight->Init(Create);
	}
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
bool VertexPrelighter::ExportMaterial(const MaterialDescriptor& material)
{
	if(mPrelighter)
	{
		PrelighterShader* NewShader = mPrelighter->NewShader();
		if(!NewShader)	return false;

		PRELIGHTER_SHADER_CREATE Create;
		Create.mName	= (const char*)material.mName;
		Create.mID		= material.mObjectID;
		//
			MAXToD3DMaterialConverter Cvt;
			Cvt.mDiffuseMapID		= material.mDiffuseMapID;
			Cvt.mMtlAmbientColor	= material.mMtlAmbientColor;
			Cvt.mMtlDiffuseColor	= material.mMtlDiffuseColor;
			Cvt.mMtlSpecularColor	= material.mMtlSpecularColor;
			Cvt.mMtlFilterColor		= material.mMtlFilterColor;
			Cvt.mSelfIllumOn		= material.mSelfIllumOn;
			Cvt.mSelfIllumValue		= material.mSelfIllumValue;
			Cvt.mSelfIllumColor		= material.mSelfIllumColor;
			Cvt.mShininess			= material.mShininess;
			Cvt.mShiningStrength	= material.mShiningStrength;
			Cvt.mSelfIllum			= material.mSelfIllum;
			Cvt.mOpacity			= material.mOpacity;
			Cvt.ConvertMaterial(Create.mProps);

		NewShader->Init(Create);
	}
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
bool VertexPrelighter::ExportMesh(const MeshDescriptor& mesh)
{
	if(mPrelighter)
	{
		PrelighterMesh* NewMesh = mPrelighter->NewMesh();
		if(!NewMesh)	return false;

		PRELIGHTER_MESH_CREATE Create;
		Create.mName	= (const char*)mesh.mName;
		Create.mID		= mesh.mObjectID;
		//
		{
			PR mPrs;
			// Save PRS (MAX-to-ICE)
			mPrs.mPos.x		= mesh.mPrs.mPosition.x;
			mPrs.mPos.y		= mesh.mPrs.mPosition.y;
			mPrs.mPos.z		= mesh.mPrs.mPosition.z;
			//
			mPrs.mRot.p.x	= mesh.mPrs.mRotation.x;
			mPrs.mRot.p.y	= mesh.mPrs.mRotation.y;
			mPrs.mRot.p.z	= mesh.mPrs.mRotation.z;
			mPrs.mRot.w		= mesh.mPrs.mRotation.w;

			Matrix4x4 mLocalToWorld = PR(mPrs);

			Create.mWorld = &mLocalToWorld;
		}

		{
			MultiSurface* MS = new MultiSurface;
			mSurfaces.Add(udword(MS));

			MAXNativeMesh* M = mesh.mOriginalMesh;

//			Object* NodeObject = derived_object ? derived_object : node->GetObjectRef();
			mNodes.Add(udword(mesh.mInternalData));
			mObjects.Add(udword(mesh.mInternalData3));
			mParities.Add(udword(M->mParity));

			bool Status = MS->Init(M->GetNbFaces(), M->GetNbVerts(), (const Point*)M->GetVerts());

			// Build surface
			for(udword i=0;i<M->GetNbFaces();i++)
			{
				Status = MS->SetFace(M->mFaces[i].VRef[0],
									M->mFaces[i].VRef[1+M->mParity],
									M->mFaces[i].VRef[2-M->mParity],
									i);
			}

			// Setup material IDs & smoothing groups
			KID* MIDS = MS->GetMaterialIDs();
			SmoothingGroups* SMG = MS->GetSmoothingGroups();
			for(udword i=0;i<M->GetNbFaces();i++)
			{
				MIDS[i] = M->mFaces[i].MatID;
				SMG[i] = M->mFaces[i].Smg;
			}

			Create.mSourceMesh = MS;
		}

		NewMesh->Init(Create);
	}
	return true;
}

#pragma optimize( "", on )

extern ExporterPanel gExporterPanel;		// The exporter's panel
static MAXProgressBar* gPB = null;

static bool gPrelighterCallback(PrelighterCallbackCode code, PrelighterCallbackParams* params, void* user_data)
{
	switch(code)
	{
		case PCC_INIT_MESH:
		{
			PrelighterCallbackInitMesh* Params = (PrelighterCallbackInitMesh*)params;

			// Just in case
			if(gPB)
			{
				gPB->EndProgressBar();
				DELETESINGLE(gPB);
			}

			gPB = new MAXProgressBar(gExporterPanel.ip);
			gPB->StartProgressBar(Params->mName, Params->mNbColors);
			break;
		}

		case PCC_MESH_PROGRESS:
		{
			PrelighterCallbackMeshProgress* Params = (PrelighterCallbackMeshProgress*)params;

			if(!gPB->UpdateProgressBar())
			{
				gPB->EndProgressBar();
				DELETESINGLE(gPB);
				return false;
			}
			break;
		}

		case PCC_END_MESH:
		{
			gPB->EndProgressBar();
			DELETESINGLE(gPB);
			break;
		}
	}
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
bool VertexPrelighter::EndExport(const StatsDescriptor& stats)
{
	if(mPrelighter)
	{
		if(mPrelighter->GetNbLights())
		{
			PRELIGHTER_CREATE Create;
			Create.mCallback	= gPrelighterCallback;
			Create.mUserData	= this;
			Create.mShadows		= mShadows;
			Create.mSmoothing	= mSmoothing;
			Create.mMergeAll	= true;
			if(mPrelighter->PrecomputeLighting(Create))
			{
				// Read back prelit surfaces
				for(udword i=0;i<mSurfaces.GetNbEntries();i++)
				{
					MultiSurface* MS = (MultiSurface*)mSurfaces.GetEntry(i);
					IndexedSurface* RGB = MS->GetExtraSurface(SURFACE_RGB);
					if(RGB)
					{
						if(mClamp)
						{
							udword NbVerts = RGB->GetNbVerts();
							for(udword i=0;i<NbVerts;i++)
							{
								Point* Color = (Point*)RGB->GetVertex(i);
								if(Color->x>1.0f)	Color->x = 1.0f;
								if(Color->y>1.0f)	Color->y = 1.0f;
								if(Color->z>1.0f)	Color->z = 1.0f;
							}
						}

						INode* Node = (INode*)mNodes.GetEntry(i);
						Object* NodeObject = (Object*)mObjects.GetEntry(i);
						bool Parity = mParities.GetEntry(i)!=0;
	//					SetVertexColors(NodeObject, 0, MS->GetExtraSurface(SURFACE_RGB), MS->GetExtraSurface(SURFACE_ALPHA), Parity);
	//					ApplyRandomColors(NodeObject, 0);
						CreateModifier(Node, RGB, Parity);

						Node->SetCVertMode(FALSE);
						Node->SetCVertMode(TRUE);
	//					Node->SetShadeCVerts(TRUE);
					}
				}
			}
		}
		else
		{
			// No lights !
			IceCore::MessageBox(null, "There is no lights in the scene. Prelighting requires at least one light.", "Prelighting aborted", MB_OK);
		}

		// Release surfaces
		for(udword i=0;i<mSurfaces.GetNbEntries();i++)
		{
			MultiSurface* MS = (MultiSurface*)mSurfaces.GetEntry(i);
			DELETESINGLE(MS);
		}
		mSurfaces.Empty();
	}
	return true;
}

bool VertexPrelighter::CreateModifier(INode* node, IndexedSurface* color_surface, bool parity)
{
	// Create and/or update modifier

	// :TODO: check GetModifier is not the thing slowing this down

	DeleteModifier(node, VERTEX_DATA_MODIFIER_CLASS_ID);

	VertexDataModifier* mod = (VertexDataModifier*)FindModifier(node, VERTEX_DATA_MODIFIER_CLASS_ID);
	if(!mod)
	{
		mod = (VertexDataModifier*)CreateInstance(OSM_CLASS_ID, VERTEX_DATA_MODIFIER_CLASS_ID);
		if(mod)
		{
			Object* obj = node->GetObjectRef();
			IDerivedObject* dobj = CreateDerivedObject(obj);

			dobj->AddModifier(mod);
			node->SetObjectRef(dobj);
		}
	}

	if(mod)
	{
		mod->SetVertexColors(color_surface, parity);
	}
	return true;
}


ExportSettings* VertexPrelighter::OverrideSettings(ExportSettings* settings)
{
	// Those are the user settings, as setup in the Options panel.
	mSettings = *settings;

	// Override some of them, needed to compute lighting

	// General settings
//	mSettings.mCompression					= FLX_COMPRESSION_NONE;
//	mSettings.mNbBits						= 8;
//	mSettings.mExportWholeScene				= true;
	mSettings.mExportVisTrack				= false;	// Not needed for lighting
//	mSettings.mExportHiddenNodes			= false;
	mSettings.mAutoUnload					= false;	// Not needed for lighting
	mSettings.mAutoLoad						= false;	// Not needed for lighting
//	mSettings.mGlobalScale					= 1.0f;
//	mSettings.mLocale						= FLX_LOCALE_ENGLISH;
	mSettings.mExportPHYSIQUE				= false;	// Not needed for lighting
	mSettings.mExportSKIN					= false;	// Not needed for lighting
	mSettings.mExportBIPED					= false;	// Not needed for lighting
	mSettings.mAlwaysExportMotion			= false;	// Not needed for lighting
	// Mesh settings
	mSettings.mExpUVW						= false;	// Not needed for lighting
	mSettings.mDiscardW						= true;		// Not needed for lighting
	mSettings.mExpVTXColor					= false;	// Not needed for lighting
	mSettings.mUseSmgrp						= true;
	mSettings.mRemoveScaling				= true;
	mSettings.mConvertToD3D					= true;
	mSettings.mAbsolutePRS					= true;
	mSettings.mConvexHull					= false;	// Not needed for lighting
	mSettings.mBoundingSphere				= false;	// Not needed for lighting
	mSettings.mInertiaTensor				= false;	// Not needed for lighting
	mSettings.mEdgeVis						= false;	// Not needed for lighting
	mSettings.mMakeManifold					= false;	// Not needed for lighting
	mSettings.mExpTargetNodes				= false;	// Not needed for lighting
	mSettings.mDiscardInstances				= true;	// ###
	// Consolidation settings
	mSettings.mConsolidateMesh				= false;	// Not needed for lighting
	mSettings.mComputeFaceNormals			= false;	// Not needed for lighting
	mSettings.mComputeVertexNormals			= false;	// Not needed for lighting
	mSettings.mExportNormalInfo				= false;	// Not needed for lighting
	mSettings.mWeightNormalsByArea			= false;	// Not needed for lighting
	mSettings.mCacheOptimize				= false;	// Not needed for lighting
	// Material settings
//	mSettings.mForceAmbient					= false;
//	mSettings.mForceDiffuse					= false;
//	mSettings.mForceSpecular				= false;
//	mSettings.mForceFilter					= false;
	mSettings.mExpMtlCtrl					= false;	// Not needed for lighting
	mSettings.mMaterialSharing				= false;	// Not needed for lighting
	// Texture settings
	mSettings.mOpacityInAlpha				= false;	// Not needed for lighting
	mSettings.mTexnameOnly					= true;
	mSettings.mKeepAmbientTexture			= false;	// Not needed for lighting
	mSettings.mKeepDiffuseTexture			= false;	// Not needed for lighting
	mSettings.mKeepSpecularTexture			= false;	// Not needed for lighting
	mSettings.mKeepShininessTexture			= false;	// Not needed for lighting
	mSettings.mKeepShiningStrengthTexture	= false;	// Not needed for lighting
	mSettings.mKeepSelfIllumTexture			= false;	// Not needed for lighting
	mSettings.mKeepOpacityTexture			= false;	// Not needed for lighting
	mSettings.mKeepFilterTexture			= false;	// Not needed for lighting
	mSettings.mKeepBumpTexture				= false;	// Not needed for lighting
	mSettings.mKeepReflexionTexture			= false;	// Not needed for lighting
	mSettings.mKeepRefractionTexture		= false;	// Not needed for lighting
	mSettings.mKeepDisplacementTexture		= false;	// Not needed for lighting
	mSettings.mTexMaxSize					= 256;		// Not needed for lighting
	mSettings.mFlipHorizontal				= false;	// Not needed for lighting
	mSettings.mFlipVertical					= false;	// Not needed for lighting
	mSettings.mQuantizeTextures				= false;	// Not needed for lighting
	mSettings.mDiscardTextureAlpha			= false;	// Not needed for lighting
	// Camera settings
	mSettings.mExpCameras					= true;		// We need one for the viewpoint
	mSettings.mExpFOVCtrl					= false;	// Not needed for lighting
	mSettings.mExpCamCtrl					= false;	// Not needed for lighting
	// Light settings
	mSettings.mExpLights					= true;		// We sure need them
	mSettings.mExpLitCtrl					= false;	// Not needed for lighting
	mSettings.mExpProjMaps					= false;	// Not needed for lighting

	mSettings.mComputeVtxColors				= false;	// Obsolete
	mSettings.mComputeShadows				= false;	// Obsolete
	mSettings.mColorSmoothing				= false;	// Obsolete
	// Animation settings
	mSettings.mSingleFrame					= true;
	mSettings.mSampling						= true;
	mSettings.mSamplingRate					= 1;
	// Shape settings
	mSettings.mExpShapes					= false;	// Not needed for lighting
	mSettings.mShapeToMesh					= false;	// Not needed for lighting
	// Helper settings
	mSettings.mExpHelpers					= false;	// Not needed for lighting
	// Space warps settings
	mSettings.mExpSpaceWarps				= false;	// Not needed for lighting
	// IK settings
	mSettings.mExportIK						= false;	// Not needed for lighting

	// NB: the same settings will be copied back by the base method SetSettings()...
	return &mSettings;
}

bool VertexPrelighter::InitGUI()
{
	return false;
}

bool VertexPrelighter::SetStatus(const char* text)
{
	return false;
}

bool VertexPrelighter::SetExtStatus(const char* text)
{
	return false;
}

bool VertexPrelighter::Progress(int pos)
{
	return false;
}

bool VertexPrelighter::DisplayLogText(const char* text)
{
	return false;
}

bool VertexPrelighter::CloseGUI()
{
	return false;
}


#endif