///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Material export method.
 *	This method is called once for each exported material.
 *	\param		material		[in] a structure filled with current material information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBFormat::ExportMaterial(const MaterialDescriptor& material)
{
	// 1) Export name
	mMaterials.Store((const char*)material.mName).Store((ubyte)0);

	// 2) Export material ID
	mMaterials.Store(material.mObjectID);

	// 3) Export material parameters
	mMaterials
		.Store(material.mAmbientMapID)
		.Store(material.mDiffuseMapID)
		.Store(material.mSpecularMapID)
		.Store(material.mShininessMapID)
		.Store(material.mShiningStrengthMapID)
		.Store(material.mSelfIllumMapID)
		.Store(material.mOpacityMapID)
		.Store(material.mFilterMapID)
		.Store(material.mBumpMapID)
		.Store(material.mReflexionMapID)
		.Store(material.mRefractionMapID)
		.Store(material.mDisplacementMapID)

		.Store(material.mAmbientCoeff)
		.Store(material.mDiffuseCoeff)
		.Store(material.mSpecularCoeff)
		.Store(material.mShininessCoeff)
		.Store(material.mShiningStrengthCoeff)
		.Store(material.mSelfIllumCoeff)
		.Store(material.mOpacityCoeff)
		.Store(material.mFilterCoeff)
		.Store(material.mBumpCoeff)
		.Store(material.mReflexionCoeff)
		.Store(material.mRefractionCoeff)
		.Store(material.mDisplacementCoeff)

		.Store(material.mMtlAmbientColor.r)		.Store(material.mMtlAmbientColor.g)		.Store(material.mMtlAmbientColor.b)
		.Store(material.mMtlDiffuseColor.r)		.Store(material.mMtlDiffuseColor.g)		.Store(material.mMtlDiffuseColor.b)
		.Store(material.mMtlSpecularColor.r)	.Store(material.mMtlSpecularColor.g)	.Store(material.mMtlSpecularColor.b)
		.Store(material.mMtlFilterColor.r)		.Store(material.mMtlFilterColor.g)		.Store(material.mMtlFilterColor.b)

		.Store(material.mShading)
		.Store(material.mSoften)
		.Store(material.mFaceMap)
		.Store(material.mTwoSided)
		.Store(material.mWire)
		.Store(material.mWireUnits)
		.Store(material.mFalloffOut)
		.Store(material.mTransparencyType)

		.Store(material.mShininess)
		.Store(material.mShiningStrength)
		.Store(material.mSelfIllum)
		.Store(material.mOpacity)
		.Store(material.mOpaFalloff)
		.Store(material.mWireSize)
		.Store(material.mIOR)

		.Store(material.mBounce)
		.Store(material.mStaticFriction)
		.Store(material.mSlidingFriction);

/*	// 4) Export cropping values
	const TextureCrop& TC = material.mTransform.mCValues;
	mMaterials
		.Store(TC.mOffsetU).Store(TC.mOffsetV).Store(TC.mScaleU).Store(TC.mScaleV);

	// 5) Export texture matrix
	const TextureMatrix& TM = material.mTransform.mTMtx;
	mMaterials
		.Store(TM.m[0][0]).Store(TM.m[0][1]).Store(TM.m[0][2])
		.Store(TM.m[1][0]).Store(TM.m[1][1]).Store(TM.m[1][2])
		.Store(TM.m[2][0]).Store(TM.m[2][1]).Store(TM.m[2][2])
		.Store(TM.m[3][0]).Store(TM.m[3][1]).Store(TM.m[3][2]);
*/
	// 4) Export texture transform
	ExportTextureTransform(material.mTransform, mMaterials);

	// 5) Export extended self-illum (CHUNK_MATL_VER>=3)
	mMaterials
		.Store(material.mSelfIllumOn)
		.Store(material.mSelfIllumValue)
		.Store(material.mSelfIllumColor.r).Store(material.mSelfIllumColor.g).Store(material.mSelfIllumColor.b);

	// 6) Flexporter specific parameters (CHUNK_MATL_VER>=5)
	mMaterials
		.Store((const char*)material.mShaderFile).Store(ubyte(0))
		.Store((const char*)material.mUserProps).Store(ubyte(0));

	// Flexporter specific parameters (CHUNK_MATL_VER>=6)
	mMaterials
			.Store(material.mDecalMapID)
			.Store(material.mDetailMapID)
	// Flexporter specific parameters (CHUNK_MATL_VER>=7)
			.Store(material.mBillboardMapID);

	return true;
}
