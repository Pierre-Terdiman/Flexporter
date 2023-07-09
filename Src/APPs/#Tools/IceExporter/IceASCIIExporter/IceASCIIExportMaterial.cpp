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
bool ASCIIFormat::ExportMaterial(const MaterialDescriptor& material)
{
	// Export database management information
	mMaterials.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("Material: ");
	mMaterials.StoreASCII((const char*)material.mName);

	// Export material parameters

	mMaterials
		.StoreASCII("\nMaterialID:              ").StoreASCII(material.mObjectID)
		.StoreASCII("\nAmbient map ID:          ").StoreASCII(material.mAmbientMapID)
		.StoreASCII("\nDiffuse map ID:          ").StoreASCII(material.mDiffuseMapID)
		.StoreASCII("\nSpecular map ID:         ").StoreASCII(material.mSpecularMapID)
		.StoreASCII("\nShininess map ID:        ").StoreASCII(material.mShininessMapID)
		.StoreASCII("\nShining Strength map ID: ").StoreASCII(material.mShiningStrengthMapID)
		.StoreASCII("\nSelfIllum map ID:        ").StoreASCII(material.mSelfIllumMapID)
		.StoreASCII("\nOpacity map ID:          ").StoreASCII(material.mOpacityMapID)
		.StoreASCII("\nFilter map ID:           ").StoreASCII(material.mFilterMapID)
		.StoreASCII("\nBump map ID:             ").StoreASCII(material.mBumpMapID)
		.StoreASCII("\nReflexion map ID:        ").StoreASCII(material.mReflexionMapID)
		.StoreASCII("\nRefraction map ID:       ").StoreASCII(material.mRefractionMapID)
		.StoreASCII("\nDisplacement map ID:     ").StoreASCII(material.mDisplacementMapID)

		.StoreASCII("\nAmbient Coeff:           ").StoreASCII(material.mAmbientCoeff)
		.StoreASCII("\nDiffuse Coeff:           ").StoreASCII(material.mDiffuseCoeff)
		.StoreASCII("\nSpecular Coeff:          ").StoreASCII(material.mSpecularCoeff)
		.StoreASCII("\nShininess Coeff:         ").StoreASCII(material.mShininessCoeff)
		.StoreASCII("\nShining Strength Coeff:  ").StoreASCII(material.mShiningStrengthCoeff)
		.StoreASCII("\nSelf Illum Coeff:        ").StoreASCII(material.mSelfIllumCoeff)
		.StoreASCII("\nOpacity Coeff:           ").StoreASCII(material.mOpacityCoeff)
		.StoreASCII("\nFilter Coeff:            ").StoreASCII(material.mFilterCoeff)
		.StoreASCII("\nBump Coeff:              ").StoreASCII(material.mBumpCoeff)
		.StoreASCII("\nReflexion Coeff:         ").StoreASCII(material.mReflexionCoeff)
		.StoreASCII("\nRefraction Coeff:        ").StoreASCII(material.mRefractionCoeff)
		.StoreASCII("\nDisplacement Coeff:      ").StoreASCII(material.mDisplacementCoeff)

		.StoreASCII("\nAmbient Color:           ").StoreASCII(material.mMtlAmbientColor.r).StoreASCII(" ").StoreASCII(material.mMtlAmbientColor.g).StoreASCII(" ").StoreASCII(material.mMtlAmbientColor.b)
		.StoreASCII("\nDiffuse Color:           ").StoreASCII(material.mMtlDiffuseColor.r).StoreASCII(" ").StoreASCII(material.mMtlDiffuseColor.g).StoreASCII(" ").StoreASCII(material.mMtlDiffuseColor.b)
		.StoreASCII("\nSpecular Color:          ").StoreASCII(material.mMtlSpecularColor.r).StoreASCII(" ").StoreASCII(material.mMtlSpecularColor.g).StoreASCII(" ").StoreASCII(material.mMtlSpecularColor.b)
		.StoreASCII("\nFilter Color:            ").StoreASCII(material.mMtlFilterColor.r).StoreASCII(" ").StoreASCII(material.mMtlFilterColor.g).StoreASCII(" ").StoreASCII(material.mMtlFilterColor.b)

		.StoreASCII("\nShading:                 ").StoreASCII(material.mShading)
		.StoreASCII("\nSoften:                  ").StoreASCII(material.mSoften)
		.StoreASCII("\nFaceMap:                 ").StoreASCII(material.mFaceMap)
		.StoreASCII("\nTwoSided:                ").StoreASCII(material.mTwoSided)
		.StoreASCII("\nWire:                    ").StoreASCII(material.mWire)
		.StoreASCII("\nWireUnits:               ").StoreASCII(material.mWireUnits)
		.StoreASCII("\nFalloffOut:              ").StoreASCII(material.mFalloffOut)
		.StoreASCII("\nTransparency type:       ").StoreASCII(material.mTransparencyType)

		.StoreASCII("\nShininess:               ").StoreASCII(material.mShininess)
		.StoreASCII("\nShiningStrength:         ").StoreASCII(material.mShiningStrength)
		.StoreASCII("\nSelfIllum:               ").StoreASCII(material.mSelfIllum)
		.StoreASCII("\nOpacity:                 ").StoreASCII(material.mOpacity)
		.StoreASCII("\nOpaFalloff:              ").StoreASCII(material.mOpaFalloff)
		.StoreASCII("\nWireSize:                ").StoreASCII(material.mWireSize)
		.StoreASCII("\nIOR:                     ").StoreASCII(material.mIOR)

		.StoreASCII("\nBounce:                  ").StoreASCII(material.mBounce)
		.StoreASCII("\nStatic Friction:         ").StoreASCII(material.mStaticFriction)
		.StoreASCII("\nSliding Friction:        ").StoreASCII(material.mSlidingFriction);

/*	// The cropping values
	const TextureCrop& TC = material.mTransform.mCValues;
	mMaterials
		.StoreASCII("\n\nCropping values:\n")
		.StoreASCII("OffsetU: ").StoreASCII(TC.mOffsetU)
		.StoreASCII("\nOffsetV: ").StoreASCII(TC.mOffsetV)
		.StoreASCII("\nScaleU: ").StoreASCII(TC.mScaleU)
		.StoreASCII("\nScaleV: ").StoreASCII(TC.mScaleV)
		.StoreASCII("\n");

	// The texture matrix
	const TextureMatrix& TM = material.mTransform.mTMtx;
	mMaterials
		.StoreASCII("\nTexture matrix:\n")
		.StoreASCII(TM.m[0][0]).StoreASCII(" ").StoreASCII(TM.m[0][1]).StoreASCII(" ").StoreASCII(TM.m[0][2]).StoreASCII("\n")
		.StoreASCII(TM.m[1][0]).StoreASCII(" ").StoreASCII(TM.m[1][1]).StoreASCII(" ").StoreASCII(TM.m[1][2]).StoreASCII("\n")
		.StoreASCII(TM.m[2][0]).StoreASCII(" ").StoreASCII(TM.m[2][1]).StoreASCII(" ").StoreASCII(TM.m[2][2]).StoreASCII("\n")
		.StoreASCII(TM.m[3][0]).StoreASCII(" ").StoreASCII(TM.m[3][1]).StoreASCII(" ").StoreASCII(TM.m[3][2]).StoreASCII("\n");

	// The texture tiling
	mMaterials.StoreASCII("\nTexture tiling U: ");
	if(material.mTransform.mTextureTilingU & TILING_WRAP)	mMaterials.StoreASCII(" WRAP");
	else													mMaterials.StoreASCII(" CLAMP");
	if(material.mTransform.mTextureTilingU & TILING_MIRROR)	mMaterials.StoreASCII(" MIRROR");
	mMaterials.StoreASCII("\nTexture tiling V: ");
	if(material.mTransform.mTextureTilingV & TILING_WRAP)	mMaterials.StoreASCII(" WRAP");
	else													mMaterials.StoreASCII(" CLAMP");
	if(material.mTransform.mTextureTilingV & TILING_MIRROR)	mMaterials.StoreASCII(" MIRROR");
	mMaterials.StoreASCII("\n");
*/
	ExportTextureTransform(material.mTransform, mMaterials);

	// Extended self-illum
	mMaterials
		.StoreASCII("\nExtended self-illum:")
		.StoreASCII("\nSelfIllumOn: ").StoreASCII(material.mSelfIllumOn)
		.StoreASCII("\nSelfIllumValue: ").StoreASCII(material.mSelfIllumValue)
		.StoreASCII("\nSelfIllumColor: ").StoreASCII(material.mSelfIllumColor.r).StoreASCII(" ").StoreASCII(material.mSelfIllumColor.g).StoreASCII(" ").StoreASCII(material.mSelfIllumColor.b)
		.StoreASCII("\n\n");

	// Flexporter specific parameters
	if(material.mShaderFile.IsValid())
	{
		mMaterials.StoreASCII("Shader file: %s\n", material.mShaderFile);
	}

	if(material.mUserProps.IsValid())
	{
		mMaterials.StoreASCII("User properties: %s\n", material.mUserProps);
	}

	mMaterials
		.StoreASCII("\nDecal map ID:   ").StoreASCII(material.mDecalMapID)
		.StoreASCII("\nDetail map ID:  ").StoreASCII(material.mDetailMapID)
		.StoreASCII("\nBillboard map ID:  ").StoreASCII(material.mBillboardMapID);

	return true;
}
