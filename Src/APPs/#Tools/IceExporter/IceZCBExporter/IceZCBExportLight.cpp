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
bool ZCBFormat::ExportLight(const LightDescriptor& light)
{
	// Export common information
	ExportBasicInfo(&light, &mLights);

	// Save parameters
	mLights
		.Store(light.mLightType)
		.Store(light.mIsSpot)
		.Store(light.mIsDir)
		.Store(light.mColor.x)
		.Store(light.mColor.y)
		.Store(light.mColor.z)
		.Store(light.mIntensity)
		.Store(light.mContrast)
		.Store(light.mDiffuseSoft)
		.Store(light.mLightUsed)
		.Store(light.mAffectDiffuse)
		.Store(light.mAffectSpecular)
		.Store(light.mUseAttenNear)
		.Store(light.mAttenNearDisplay)
		.Store(light.mUseAtten)
		.Store(light.mShowAtten)
		.Store(light.mNearAttenStart)
		.Store(light.mNearAttenEnd)
		.Store(light.mAttenStart)
		.Store(light.mAttenEnd)
		.Store(light.mDecayType)
		.Store(light.mHotSpot)
		.Store(light.mFallsize)
		.Store(light.mAspect)
		.Store(light.mSpotShape)
		.Store(light.mOvershoot)
		.Store(light.mConeDisplay)
		.Store(light.mTDist)
		.Store(light.mShadowType)
		.Store(light.mAbsMapBias)
		.Store(light.mRayBias)
		.Store(light.mMapBias)
		.Store(light.mMapRange)
		.Store(light.mMapSize)
		.Store(light.mCastShadows)
		.Store(light.mShadowDensity)
		.Store(light.mShadowColor.x)
		.Store(light.mShadowColor.y)
		.Store(light.mShadowColor.z)
		.Store(light.mLightAffectsShadow)
		// CHUNK_LITE_VER >= 4
		.Store(light.mProjMapID)
		.Store(light.mShadowProjMapID);

	return true;
}
