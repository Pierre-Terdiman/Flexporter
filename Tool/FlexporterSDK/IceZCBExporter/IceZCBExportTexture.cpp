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
bool ZCBFormat::ExportTexture(const TextureDescriptor& texture)
{
	// 1) Export name
	mTexmaps.Store((const char*)texture.mName).Store((ubyte)0);

	// 2) Export texture ID
	mTexmaps.Store(texture.mObjectID);

	// 3) Export texture data if needed
	ubyte* Pixels = texture.mPixels;
	if(Pixels)
	{
		// Pixels exist => the user didn't select "Export filenames only", so let's export the whole texture bitmap.
		// We can export RGBA texture or quantized ones. 
		if(!texture.mQuantized || !texture.mPalette)
		{
			// A byte to tell the bitmap follows: 1 => RGBA textures
			mTexmaps.Store(ubyte(1));

			// Then export the bitmap info...
			mTexmaps.Store(texture.mWidth).Store(texture.mHeight).Store(texture.mHasAlpha);
			// Then export the bitmap data...
			for(udword i=0;i<texture.mWidth*texture.mHeight;i++)
			{
				ubyte R = *Pixels++;
				ubyte G = *Pixels++;
				ubyte B = *Pixels++;
				ubyte A = *Pixels++;
				mTexmaps.Store(R).Store(G).Store(B);
				if(texture.mHasAlpha)	mTexmaps.Store(A);
			}
		}
		else
		{
			// A byte to tell the bitmap follows: 2 => quantized textures
			mTexmaps.Store(ubyte(2));

			// Then export the bitmap info...
			mTexmaps.Store(texture.mWidth).Store(texture.mHeight).Store(texture.mHasAlpha);
			// Then export the palette...
			for(udword i=0;i<256;i++)
			{
				mTexmaps
					.Store(texture.mPalette[i*3+0])
					.Store(texture.mPalette[i*3+1])
					.Store(texture.mPalette[i*3+2]);
			}
			// Then export the quantized bitmap data...
			for(udword i=0;i<texture.mWidth*texture.mHeight;i++)	mTexmaps.Store(texture.mQuantized[i]);
		}
	}
	else
	{
		// A byte to tell the bitmap is not included
		mTexmaps.Store(ubyte(0));
	}

/*	// 4) Export cropping values
	const TextureCrop& TC = texture.mTransform.mCValues;
	mTexmaps
		.Store(TC.mOffsetU).Store(TC.mOffsetV).Store(TC.mScaleU).Store(TC.mScaleV);

	// 5) Export texture matrix
	const TextureMatrix& TM = texture.mTransform.mTMtx;
	mTexmaps
		.Store(TM.m[0][0]).Store(TM.m[0][1]).Store(TM.m[0][2])
		.Store(TM.m[1][0]).Store(TM.m[1][1]).Store(TM.m[1][2])
		.Store(TM.m[2][0]).Store(TM.m[2][1]).Store(TM.m[2][2])
		.Store(TM.m[3][0]).Store(TM.m[3][1]).Store(TM.m[3][2]);
*/
	// 4) Export texture transform
	ExportTextureTransform(texture.mTransform, mTexmaps);

	return true;
}
