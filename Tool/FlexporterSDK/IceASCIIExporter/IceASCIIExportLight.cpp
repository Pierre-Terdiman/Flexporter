///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Light export method.
 *	This method is called once for each exported light.
 *	\param		light		[in] a structure filled with current light information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ASCIIFormat::ExportLight(const LightDescriptor& light)
{
	// Export common information
	mLights.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("Light:        ");
	ExportBasicInfo(&light, &mLights);

	// Export light parameters

	// Get light type
	mLights.StoreASCII("Type: ");
	if (light.mLightType==FLX_LTYPE_OMNI)					mLights.StoreASCII("omnidirectional light");
	else if (light.mLightType==FLX_LTYPE_TSPOT)				mLights.StoreASCII("targeted spot light");
	else if (light.mLightType==FLX_LTYPE_DIR)				mLights.StoreASCII("directional light");
	else if (light.mLightType==FLX_LTYPE_FSPOT)				mLights.StoreASCII("free spot light");
	else if (light.mLightType==FLX_LTYPE_TDIR)				mLights.StoreASCII("targeted directional light");
	mLights.StoreASCII("\n");

	// Shadows
	if(light.mCastShadows)								mLights.StoreASCII("(Cast shadows)\n");
	else												mLights.StoreASCII("(Doesn't cast shadows)\n");

	mLights
		.StoreASCII("Shadow density (Multiplier): ")	.StoreASCII(light.mShadowDensity)				// Shadow density (multiplier)
		.StoreASCII("\nShadow color: ")					.StoreASCII(light.mShadowColor.x)				// Shadow color
		.StoreASCII(" ")								.StoreASCII(light.mShadowColor.y)				// Shadow color
		.StoreASCII(" ")								.StoreASCII(light.mShadowColor.z)				// Shadow color
		.StoreASCII("\nLight affects shadow: ")			.StoreASCII((BOOL)light.mLightAffectsShadow)	// Light affects shadow or not
		.StoreASCII("\n");

	// Color
	mLights
		.StoreASCII("Intensity (Multiplier): ")			.StoreASCII(light.mIntensity)					// Intensity (multiplier)
		.StoreASCII("\nContrast: ")						.StoreASCII(light.mContrast)					// Contrast
		.StoreASCII("\nDiffuse Soft: ")					.StoreASCII(light.mDiffuseSoft)					// Diffuse soft
		.StoreASCII("\nColor: ")						.StoreASCII(light.mColor.x)						// Color
		.StoreASCII(" ")								.StoreASCII(light.mColor.y)						// Color
		.StoreASCII(" ")								.StoreASCII(light.mColor.z)						// Color
		.StoreASCII("\nUsed: ")							.StoreASCII((BOOL)light.mLightUsed)				// Light used or not
		.StoreASCII("\nAffect diffuse: ")				.StoreASCII((BOOL)light.mAffectDiffuse)			// Affect diffuse
		.StoreASCII("\nAffect specular: ")				.StoreASCII((BOOL)light.mAffectSpecular)		// Affect specular
		.StoreASCII("\n");

	// Attenuation parameters
	mLights
		.StoreASCII("Use near atten: ")					.StoreASCII((BOOL)light.mUseAttenNear)			// Use near atten
		.StoreASCII("\nShow near atten: ")				.StoreASCII((BOOL)light.mAttenNearDisplay)		// Show near atten
		.StoreASCII("\nNearAttenStart: ")				.StoreASCII(light.mNearAttenStart)				// Near atten start
		.StoreASCII("\nNearAttenEnd: ")					.StoreASCII(light.mNearAttenEnd)				// Near atten end
		.StoreASCII("\nUseAtten: ")						.StoreASCII(light.mUseAtten)					// Use atten
		.StoreASCII("\nShowAtten: ")					.StoreASCII(light.mShowAtten)					// Show atten
		.StoreASCII("\nAttenStart: ")					.StoreASCII(light.mAttenStart)					// Atten start
		.StoreASCII("\nAttenEnd: ")						.StoreASCII(light.mAttenEnd)					// Atten end
		.StoreASCII("\nDecay type: ")					.StoreASCII(light.mDecayType)					// Decay type
		.StoreASCII("\n");

	// Spotlight parameters or directional parameters
	if(light.mIsSpot)							mLights.StoreASCII("\nSPOTLIGHT PARAMETERS:\n\n");
	else if(light.mIsDir)						mLights.StoreASCII("\nDIRECTIONAL PARAMETERS:\n\n");

	if(light.mLightType!=FLX_LTYPE_OMNI)
	{
		mLights
			.StoreASCII("Hotspot: ")					.StoreASCII(light.mHotSpot)						// Hotspot
			.StoreASCII("\nFalloff: ")					.StoreASCII(light.mFallsize)					// Falloff
			.StoreASCII("\nAspect: ")					.StoreASCII(light.mAspect)						// Aspect
			.StoreASCII("\nSpot shape: ");																// Spot shape (RECT_LIGHT, CIRCLE_LIGHT)
				if(light.mSpotShape==FLX_SPOTSHP_RECT)		mLights.StoreASCII("rectangle\n");
		else	if(light.mSpotShape==FLX_SPOTSHP_CIRCLE)	mLights.StoreASCII("circle\n");
		else												mLights.StoreASCII("unknown!\n");
		mLights
			.StoreASCII("\nOvershoot: ")				.StoreASCII(light.mOvershoot)					// Overshoot
			.StoreASCII("\nShow cone: ")				.StoreASCII(light.mConeDisplay)					// Show cone
			.StoreASCII("\nTarget distance: ")			.StoreASCII(light.mTDist)						// Target distance
			.StoreASCII("\n");
	}

	// Projectors
	if(light.mProjMapID!=-1)		mLights.StoreASCII("\nProjector map ID: %d\n", light.mProjMapID);
	if(light.mShadowProjMapID!=-1)	mLights.StoreASCII("\nShadow projector map ID: %d\n", light.mShadowProjMapID);

	return true;
}
