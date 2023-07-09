///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2003 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the material-related exported structures.
 *	\file		FLX_AddMaterial.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __FLX_ADDMATERIAL_H__
#define __FLX_ADDMATERIAL_H__

	//! The material information class.
	class FLEXPORTER_API MaterialDescriptor : public ResourceDescriptor
	{
		public:
		// Constructor / Destructor
									MaterialDescriptor();
		virtual						~MaterialDescriptor();

		// Textures
				sdword				mAmbientMapID;				//!< Ambient texture map (seems not to exist anymore in MAX 3)
				sdword				mDiffuseMapID;				//!< Diffuse texture map
				sdword				mSpecularMapID;				//!< Specular texture map
				sdword				mShininessMapID;			//!< Shininess texture map
				sdword				mShiningStrengthMapID;		//!< Shining Strength texture map
				sdword				mSelfIllumMapID;			//!< Self Illum texture map
				sdword				mOpacityMapID;				//!< Opacity texture map
				sdword				mFilterMapID;				//!< Filter texture map
				sdword				mBumpMapID;					//!< Bump texture map
				sdword				mReflexionMapID;			//!< Reflexion texture map
				sdword				mRefractionMapID;			//!< Refraction texture map
				sdword				mDisplacementMapID;			//!< Displacement texture map
		// Amounts
				float				mAmbientCoeff;				//!< Ambient texture %									[Animatable]
				float				mDiffuseCoeff;				//!< Diffuse texture %									[Animatable]
				float				mSpecularCoeff;				//!< Specular texture %									[Animatable]
				float				mShininessCoeff;			//!< Shininess texture %								[Animatable]
				float				mShiningStrengthCoeff;		//!< Shining Strength texture %							[Animatable]
				float				mSelfIllumCoeff;			//!< Self Illum texture %								[Animatable]
				float				mOpacityCoeff;				//!< Opacity texture %									[Animatable]
				float				mFilterCoeff;				//!< Filter texture %									[Animatable]
				float				mBumpCoeff;					//!< Bump texture %										[Animatable]
				float				mReflexionCoeff;			//!< Reflexion texture %								[Animatable]
				float				mRefractionCoeff;			//!< Refraction texture %								[Animatable]
				float				mDisplacementCoeff;			//!< Displacement texture %								[Animatable]
		// Colors
				NakedPoint			mMtlAmbientColor;			//!< Ambient color										[Animatable]
				NakedPoint			mMtlDiffuseColor;			//!< Diffuse color										[Animatable]
				NakedPoint			mMtlSpecularColor;			//!< Specular color										[Animatable]
				NakedPoint			mMtlFilterColor;			//!< Filter color										[Animatable]
		// Static properties
				long				mShading;					//!< Material Shading
				bool				mSoften;					//!< MaterialSoften
				bool				mFaceMap;					//!< MaterialFaceMap
				bool				mTwoSided;					//!< MaterialTwoSided
				bool				mWire;						//!< MaterialWire
				bool				mWireUnits;					//!< MaterialWireUnits
				bool				mFalloffOut;				//!< MaterialFalloffOut
				long				mTransparencyType;			//!< MaterialTransparencyType
		// Dynamic properties
				float				mShininess;					//!< MaterialShininess									[Animatable]
				float				mShiningStrength;			//!< MaterialShiningStrength							[Animatable]
				float				mSelfIllum;					//!< MaterialSelfIllum									[Animatable]
				float				mOpacity;					//!< MaterialOpacity									[Animatable]
				float				mOpaFalloff;				//!< MaterialOpacityFalloff								[Animatable]
				float				mWireSize;					//!< MaterialWireSize									[Animatable]
				float				mIOR;						//!< MaterialIOR										[Animatable]
		// Dynamic properties
				float				mBounce;					//!< Bounce												[Animatable]
				float				mStaticFriction;			//!< Static Friction									[Animatable]
				float				mSlidingFriction;			//!< Sliding Friction									[Animatable]
		// Diffuse map cropping values
				TextureTransform	mTransform;					//!< Texture transform
		// Enhanced self-illum
				bool				mSelfIllumOn;				//!< On => use the color, Off => use the value [Flexporter 1.15]
				float				mSelfIllumValue;			//!< Self-illum value [Flexporter 1.15]
				NakedPoint			mSelfIllumColor;			//!< Self-illum color [Flexporter 1.15]
		// Extended Flexporter parameters
				String				mShaderFile;				//!< Shader filename [Flexporter 1.17]
				String				mUserProps;					//!< User properties [Flexporter 1.17]
				sdword				mDecalMapID;				//!< Decal texture map [Flexporter 1.17]
				sdword				mDetailMapID;				//!< Detail texture map [Flexporter 1.17]
				sdword				mBillboardMapID;			//!< Billboard texture map [Flexporter 1.17]
	};

#endif // __FLX_ADDMATERIAL_H__
