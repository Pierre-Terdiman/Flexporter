///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2003 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the light-related exported structures.
 *	\file		FLX_AddLight.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __FLX_ADDLIGHT_H__
#define __FLX_ADDLIGHT_H__

	//! Light types from MAX. Usual rendering APIs only keep Point, Spot and Directional lights.
	enum LType
	{
		FLX_LTYPE_OMNI			= 0,	//!< Omnidirectional		(PointLight)
		FLX_LTYPE_TSPOT			= 1,	//!< Targeted				(SpotLight)
		FLX_LTYPE_DIR			= 2,	//!< Directional			(DirectionalLight)
		FLX_LTYPE_FSPOT			= 3,	//!< Free					(SpotLight)
		FLX_LTYPE_TDIR			= 4,	//!< Targeted directional	(DirectionalLight)

		FLX_LTYPE_FORCE_DWORD	= 0x7fffffff
	};

	//! Spotlight shape.
	enum SpotShp
	{
		FLX_SPOTSHP_RECT		= 0,	//!< Rectangle.
		FLX_SPOTSHP_CIRCLE		= 1,	//!< Circle.

		FLX_SPOTSHP_FORCE_DWORD	= 0x7fffffff
	};

	//! Decay type
	enum DecayType
	{
		FLX_DECAYTYPE_NONE			= 0,
		FLX_DECAYTYPE_INV			= 1,
		FLX_DECAYTYPE_INVSQ			= 2,

		FLX_DECAYTYPE_FORCE_DWORD	= 0x7fffffff
	};

	//! The light information class.
	class FLEXPORTER_API LightDescriptor : public ObjectDescriptor
	{
		public:
		// Constructor / Destructor
									LightDescriptor();
		virtual						~LightDescriptor();

		// Parameters
				LType				mLightType;				//!< Light's type
				bool				mIsSpot;				//!< Is the light a spotlight?
				bool				mIsDir;					//!< Is the light a directional?
				NakedPoint			mColor;					//!< Light's color											[Animatable]
				float				mIntensity;				//!< Light's intensity										[Animatable]
				float				mContrast;				//!< Light's contrast										[Animatable]
				float				mDiffuseSoft;			//!< Light's diffuse soft									[Animatable]
				bool				mLightUsed;				//!< Is the light used?
				bool				mAffectDiffuse;			//!< Does the light affect diffuse?
				bool				mAffectSpecular;		//!< Does the light affect specular?
				bool				mUseAttenNear;			//
				bool				mAttenNearDisplay;		//
				bool				mUseAtten;				//!< Is attenuation used?
				bool				mShowAtten;				//
				float				mNearAttenStart;		//!< Near atten start										[Animatable]
				float				mNearAttenEnd;			//!< Near atten end											[Animatable]
				float				mAttenStart;			//!< Atten start											[Animatable]
				float				mAttenEnd;				//!< Atten end (use that as a range for non-dir lights)		[Animatable]
				char				mDecayType;				//!< Light's decay type
				float				mHotSpot;				//!< Light's hotspot										[Animatable]
				float				mFallsize;				//!< Light's falloff										[Animatable]
				float				mAspect;				//!< Light's aspect											[Animatable]
				SpotShp				mSpotShape;				//!< Light's spot shape
				long				mOvershoot;				//!< Light's overshoot
				bool				mConeDisplay;			//
				float				mTDist;					//!< Distance to target										[Animatable]
				long				mShadowType;			//!< Light's shadow type
				long				mAbsMapBias;			//!< Light's absolute map bias
				float				mRayBias;				//!< Raytrace bias											[Animatable]
				float				mMapBias;				//!< Map bias												[Animatable]
				float				mMapRange;				//!< Map range												[Animatable]
				long				mMapSize;				//!< Map size												[Animatable]
				bool				mCastShadows;			//!< Cast shadows or not
				NakedPoint			mShadowColor;			//!< Shadow color											[Animatable]
				bool				mLightAffectsShadow;	//!< Light affects shadow or not
				float				mShadowDensity;			//!< Shadow multiplier										[Animatable]
		// Projectors
				sdword				mProjMapID;				//!< Projector texture map [Flexporter 1.15]
				sdword				mShadowProjMapID;		//!< Shadow projector texture map [Flexporter 1.15]
		// Instance information
				LightDescriptor*	mMasterLight;			//!< Master object [Flexporter 1.15]
	};

#endif // __FLX_ADDLIGHT_H__
