///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Texture export method.
 *	This method is called once for each exported texture.
 *	\param		texture		[in] a structure filled with current texture information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ASCIIFormat::ExportTexture(const TextureDescriptor& texture)
{
	// Export database management information
	mTexmaps.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("Texture: ");
	mTexmaps.StoreASCII((const char*)texture.mName);

	// We only export the texture's name in ascii, as well as the texture ID as a reminder.
	mTexmaps.StoreASCII("\n\nTextureID:    ").StoreASCII(texture.mObjectID);

/*	// The cropping values
	const TextureCrop& TC = texture.mTransform.mCValues;
	mTexmaps
		.StoreASCII("\n\nCropping values:\n")
		.StoreASCII("OffsetU: ").StoreASCII(TC.mOffsetU)
		.StoreASCII("\nOffsetV: ").StoreASCII(TC.mOffsetV)
		.StoreASCII("\nScaleU: ").StoreASCII(TC.mScaleU)
		.StoreASCII("\nScaleV: ").StoreASCII(TC.mScaleV)
		.StoreASCII("\n");

	// The texture matrix
	const TextureMatrix& TM = texture.mTransform.mTMtx;
	mTexmaps
		.StoreASCII("\nTexture matrix:\n")
		.StoreASCII(TM.m[0][0]).StoreASCII(" ").StoreASCII(TM.m[0][1]).StoreASCII(" ").StoreASCII(TM.m[0][2]).StoreASCII("\n")
		.StoreASCII(TM.m[1][0]).StoreASCII(" ").StoreASCII(TM.m[1][1]).StoreASCII(" ").StoreASCII(TM.m[1][2]).StoreASCII("\n")
		.StoreASCII(TM.m[2][0]).StoreASCII(" ").StoreASCII(TM.m[2][1]).StoreASCII(" ").StoreASCII(TM.m[2][2]).StoreASCII("\n")
		.StoreASCII(TM.m[3][0]).StoreASCII(" ").StoreASCII(TM.m[3][1]).StoreASCII(" ").StoreASCII(TM.m[3][2]).StoreASCII("\n\n");
*/
	ExportTextureTransform(texture.mTransform, mTexmaps);

	return true;
}

