///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a texture.
 *	\file		FLX_AddTexture.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

static UVWSource GetUVWSource(int value)
{
	switch(value)
	{
		case UVWSRC_EXPLICIT:	return UVW_EXPLICIT;
		case UVWSRC_OBJXYZ:		return UVW_LOCAL_XYZ;
		case UVWSRC_EXPLICIT2:	return UVW_VERTEX_COLORS;
		case UVWSRC_WORLDXYZ:	return UVW_WORLD_XYZ;
	};

	return UVW_FORCE_DWORD;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Exports texmaps. Only bitmap-texmap are exported.
 *	\param		tb		[in] texture blend info
 *	\return		0 if failed, else the texmap's ID
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long MAXConverter::AddTexture(TextureBlend& tb)
{
	// Get useful time info
	long		StartTime		= mTimeInfo.mStartTime;
	long		EndTime			= mTimeInfo.mEndTime;
	long		DeltaTime		= mTimeInfo.mDeltaTime;
	long		FrameRate		= mTimeInfo.mFrameRate;
	TimeValue	CurrentTime		= mTimeInfo.mCurrentTime;

	// Update log file
	// We don't reuse the "texture name" otherwise
	{
		static const char* Noname = "(unnamed)";
		const char*	TextureName = tb.mRGBMap ? tb.mRGBMap->GetName() : Noname;
		if(!TextureName)	TextureName = Noname;

		mArrayLog.StoreASCII("  Converting texture: %s\n", TextureName);
	}

//### Hack
bool SkipQuantize = false;

	// 1) Check creation structure
	char RGBStatus		= IsKnownTexmap(tb.mRGBMap);
	char AlphaStatus	= IsKnownTexmap(tb.mAlphaMap);
	// 1-1) If no texture is defined for RGB and Alpha, there's nothing to do and we can exit
	if(RGBStatus==TEXMAP_UNDEFINED && AlphaStatus==TEXMAP_UNDEFINED)
	{
		// No texture defined.
		mArrayLog.StoreASCII("WARNING! No texture defined.\n");
		return TEXTURE_INVALID;
	}
	// 1-2) If one of the texmap is not a bitmap one, we can't deal with it (but we still can export the other one)
	if(tb.mRGBMap && RGBStatus!=TEXMAP_BITMAP)
	{
		tb.mRGBMap = null;	// Override it
		mArrayLog.StoreASCII("WARNING! Diffuse texture discarded, I can only export bitmap-texmaps!\n");
	}
	if(tb.mAlphaMap && AlphaStatus!=TEXMAP_BITMAP)
	{
		tb.mAlphaMap = null;	// Override it
		mArrayLog.StoreASCII("WARNING! Opacity texture discarded, I can only export bitmap-texmaps!\n");
	}
	// 1-3) We must test again now since both textures may have been discarded.
	if(!tb.mRGBMap && !tb.mAlphaMap)
	{
		// No texture defined.
		mArrayLog.StoreASCII("WARNING! No more textures defined.\n");
		return TEXTURE_INVALID;
	}
	// 1-4) Check bitmap data
	BitmapTex*	RGBBitmapTex	= (BitmapTex*)tb.mRGBMap;
	BitmapTex*	AlphaBitmapTex	= (BitmapTex*)tb.mAlphaMap;
	TSTR RGBMapName;
	TSTR AlphaMapName;
	Bitmap* RGBTexData		= null;
	Bitmap* AlphaTexData	= null;
	char* RGBTexName		= null;
	char* AlphaTexName		= null;
	char* FullRGBTexName	= null;
	char* FullAlphaTexName	= null;
	if(RGBBitmapTex)
	{
		// Get map name
		RGBMapName	= RGBBitmapTex->GetMapName();
		RGBTexName	= strrchr(RGBMapName.data(), '\\');
		if(!RGBTexName)	RGBTexName=RGBMapName.data();
		else			RGBTexName++;
		FullRGBTexName = RGBMapName.data();
		// Here RGBTexName is the name without the path, FullRGBTexName is the name with the path.

		// Get bitmap data
		RGBTexData	= RGBBitmapTex->GetBitmap(CurrentTime);
		if(!RGBTexData)
		{
			// No bitmap data defined! Just make a render and export again...
			mArrayLog.StoreASCII("Texture ").StoreASCII((const char*)RGBMapName).StoreASCII(" not available in MAX. \tHint: make a render and export again?\n");
			tb.mRGBMap		= null;
//			RGBTexName		= null;
//			FullRGBTexName	= null;
SkipQuantize = true;
		}
	}
	if(AlphaBitmapTex)
	{
		// Get map name
		AlphaMapName	= AlphaBitmapTex->GetMapName();
		AlphaTexName	= strrchr(AlphaMapName.data(), '\\');
		if(!AlphaTexName)	AlphaTexName=AlphaMapName.data();
		else				AlphaTexName++;
		FullAlphaTexName = AlphaMapName.data();
		// Here AlphaTexName is the name without the path, FullAlphaTexName is the name with the path.

		// Get bitmap data
		AlphaTexData	= AlphaBitmapTex->GetBitmap(CurrentTime);
		if(!AlphaTexData)
		{
			// No bitmap data defined! Just make a render and export again...
			mArrayLog.StoreASCII("WARNING! Texture ").StoreASCII((const char*)AlphaMapName).StoreASCII(" not available in MAX. \tHint: make a render and export again?\n");
			tb.mAlphaMap		= null;
			AlphaTexName		= null;
			FullAlphaTexName	= null;
		}
	}

	// 1-5) We must check whether that's the first time we meet this Texmap or not, else we could multiply define the same texmap
	// 1-5-1) Here we just loop through all exported texmaps in search of the current one. (### What? A O(n^2) process??)
	const TMapInfo* TMI = mGlobalInfo.GetTextureInfo();
	for(udword i=0;i<mStats.mNbTexmaps;i++)
	{
		// Compare pointers
//		if(mGlobalInfo.mTexmInfo[i].MAXRGBBitmap==RGBStorage && mGlobalInfo.mTexmInfo[i].MAXAlphaBitmap==AlphaStorage)
//		if(Local::AreEqual(mGlobalInfo.mTexmInfo[i].mMAXRGBBitmap, FullRGBTexName) && Local::AreEqual(mGlobalInfo.mTexmInfo[i].mMAXAlphaBitmap, FullAlphaTexName))
		if(AreEqual(TMI[i].mMAXRGBBitmap, FullRGBTexName) && AreEqual(TMI[i].mMAXAlphaBitmap, FullAlphaTexName))
		{
			// Return old texmap ID
//			return mGlobalInfo.mTexmInfo[i].mMapID;
			return TMI[i].mMapID;
		}
	}
	// 1-5-2) Not found? Then create a new entry. Texmap ID ranges from 0 to the actual #texmaps-1.
//	if(FullRGBTexName)		mGlobalInfo.mTexmInfo[mStats.mNbTexmaps].mMAXRGBBitmap		= (const char*)FullRGBTexName;
//	if(FullAlphaTexName)	mGlobalInfo.mTexmInfo[mStats.mNbTexmaps].mMAXAlphaBitmap	= (const char*)FullAlphaTexName;
//							mGlobalInfo.mTexmInfo[mStats.mNbTexmaps].mMapID				= mStats.mNbTexmaps;
	if(FullRGBTexName)		mGlobalInfo.SetMAXRGBBitmapName(mStats.mNbTexmaps, FullRGBTexName);
	if(FullAlphaTexName)	mGlobalInfo.SetMAXAlphaBitmapName(mStats.mNbTexmaps, FullAlphaTexName);
							mGlobalInfo.SetTextureID(mStats.mNbTexmaps, mStats.mNbTexmaps);



// Create info structure
TextureDescriptor TexInf;
TexInf.mObjectID	=	mStats.mNbTexmaps;
TexInf.mName		=	(ubyte*)FullRGBTexName;

// Get crop values from the RGB texmap
{
	Matrix3 UVTrans;
	GetCropValues(tb.mRGBMap, TexInf.mTransform.mCValues, UVTrans, CurrentTime);
	CopyMemory(&TexInf.mTransform.mTMtx, &UVTrans, sizeof(TextureMatrix));
	GetTextureTiling(tb.mRGBMap, TexInf.mTransform.mTextureTilingU, TexInf.mTransform.mTextureTilingV);
}


	if(tb.mRGBMap)
	{
		TexInf.mUVWSource	= GetUVWSource(tb.mRGBMap->GetUVWSource());
		TexInf.mMapChannel	= tb.mRGBMap->GetMapChannel();
	}

/*
		virtual int GetUVWSource() { return UVWSRC_EXPLICIT; }
		virtual int GetMapChannel () { return 1; }	// only relevant if above returns UVWSRC_EXPLICIT

		virtual UVGen *GetTheUVGen() { return NULL; }  // maps with a UVGen should implement this
		virtual XYZGen *GetTheXYZGen() { return NULL; } // maps with a XYZGen should implement this
*/


	NeuralNetQuantizer* NNQ = null;


	if(mSettings->mTexnameOnly)
	{
		// We don't directly export the texture data but we still can check the original sizes
		// against user-defined bounds.
		if(RGBTexData)
		{
			CheckTextureSize(RGBTexData->Width(), "Diffuse texture width");
			CheckTextureSize(RGBTexData->Height(), "Diffuse texture height");
		}
		if(AlphaTexData)
		{
			CheckTextureSize(AlphaTexData->Width(), "Alpha texture width");
			CheckTextureSize(AlphaTexData->Height(), "Alpha texture height");
		}
	}
	else
	{
		// 1-6) Check sizes
		Picture	ExportedBitmap;
		bool	FinalAlpha=false;	// Is there finally an alpha channel in the final data?
		bool	RGBAlpha=false;		// The RGB texture may already have an alpha channel!
		if(RGBTexData && !mSettings->mDiscardTextureAlpha)	RGBAlpha = RGBTexData->HasAlpha()!=0;

		if(RGBTexData && AlphaTexData)
		{
			if( (RGBTexData->Width()!=AlphaTexData->Width()) || (RGBTexData->Height()!=AlphaTexData->Height()) )
			{
				// Both maps are available but sizes are different
				mArrayLog.StoreASCII("WARNING! Can't blend textures whose sizes are different.\n");
				// Discard alpha channel
				AlphaTexData = null;
				tb.mAlphaMap = null;
			}
		}

		// Check whether at least there's one texture left
		if(RGBTexData==null && AlphaTexData==null)
		{
			// nothing survived! => create a default 16*16 map
			ExportedBitmap.Init(16, 16);
			RGBAPixel* Data = ExportedBitmap.GetPixels();

			for(udword n=0;n<ExportedBitmap.GetWidth()*ExportedBitmap.GetHeight();n++)
			{
				Data[n].R = ubyte(tb.mRGBDefault.r		* 255.0f);
				Data[n].G = ubyte(tb.mRGBDefault.g		* 255.0f);
				Data[n].B = ubyte(tb.mRGBDefault.b		* 255.0f);
				Data[n].A = ubyte(tb.mAlphaDefault.r	* 255.0f);
			}
			FinalAlpha=false;
		}
		else
		{
			// 2) At least we can create the source texture
			String EStat("stretching texture ");
			EStat += RGBTexName;
			EStat += "...";
			SetExtStatus(EStat);

			// 2-1) Get original sizes
			if(RGBTexData)	ExportedBitmap.Init(RGBTexData->Width(), RGBTexData->Height());
			else			ExportedBitmap.Init(AlphaTexData->Width(), AlphaTexData->Height());
			RGBAPixel* SrcData = ExportedBitmap.GetPixels();

			// 2-2) Alloc some bytes
			BMM_Color_64*	Line		= null;
			BMM_Color_64*	AlphaLine	= null;

			if(RGBTexData)
			{
				Line = new BMM_Color_64[ExportedBitmap.GetWidth()];
				ASSERT(Line);
			}
			if(AlphaTexData)
			{
				AlphaLine = new BMM_Color_64[ExportedBitmap.GetWidth()];
				ASSERT(AlphaLine);
			}

			// 2-4) At last! Build source picture!
			udword r=0,g=0,b=0,a=0;
			if(!Line)	tb.mRGBMapCoeff	= 0.0f;	// Override RGB amount if no texmap is available

			if(!AlphaLine)
			{
				if(RGBAlpha)
				{
					// No opacity texture, but an alpha channel in the diffuse one
					tb.mAlphaMapCoeff	= tb.mRGBMapCoeff;
				}
				else
				{
					// No alpha channel
					tb.mAlphaMapCoeff	= 0.0f;
				}
			}

			if(RGBAlpha || AlphaLine)	FinalAlpha=true;

			for(udword y=0;y<ExportedBitmap.GetHeight();y++)
			{
				if(Line)		RGBTexData->GetPixels(0, y, ExportedBitmap.GetWidth(), Line);
				if(AlphaLine)	AlphaTexData->GetPixels(0, y, ExportedBitmap.GetWidth(), AlphaLine);
				for(udword x=0;x<ExportedBitmap.GetWidth();x++)
				{
					if(Line)
					{
						r = (udword)(Line[x].r >> 8) & 0xff;
						g = (udword)(Line[x].g >> 8) & 0xff;
						b = (udword)(Line[x].b >> 8) & 0xff;
						a = (udword)(Line[x].a >> 8) & 0xff;	// only relevant if RGBAlpha!=0
					}
					if(AlphaLine)	a = (udword)(AlphaLine[x].r >> 8) & 0xff;	// WARNING! THIS IS THE RED COMPONENT OF THE ALPHA TEXTURE!

					r = (udword)(float(r) * tb.mRGBMapCoeff + (tb.mRGBDefault.r*255.0f) * (1.0f - tb.mRGBMapCoeff));
					g = (udword)(float(g) * tb.mRGBMapCoeff + (tb.mRGBDefault.g*255.0f) * (1.0f - tb.mRGBMapCoeff));
					b = (udword)(float(b) * tb.mRGBMapCoeff + (tb.mRGBDefault.b*255.0f) * (1.0f - tb.mRGBMapCoeff));
					a = (udword)(float(a) * tb.mAlphaMapCoeff + (tb.mAlphaDefault.r*255.0f) * (1.0f - tb.mAlphaMapCoeff));

					// Clamping [SECURITY PURPOSE]
					if(r>255)	r=255;
					if(g>255)	g=255;
					if(b>255)	b=255;
					if(a>255)	a=255;
					// Store final values
					SrcData[x+y*ExportedBitmap.GetWidth()].R = (ubyte)r;
					SrcData[x+y*ExportedBitmap.GetWidth()].G = (ubyte)g;
					SrcData[x+y*ExportedBitmap.GetWidth()].B = (ubyte)b;
					SrcData[x+y*ExportedBitmap.GetWidth()].A = (ubyte)a;
				}
			}
			// 3) Stretch picture => create destination texture
	//		if(!mSettings->mTexnameOnly)
			{
				if(mSettings->mTexMaxSize==2)
				{
					ExportedBitmap.Stretch(FirstLOD(ExportedBitmap.GetWidth()), FirstLOD(ExportedBitmap.GetHeight()));
				}
				else if(mSettings->mTexMaxSize)
				{
					udword DestSize = ComputeTextureSize(ExportedBitmap);
					ExportedBitmap.Stretch(DestSize, DestSize);
				}
			}

			// Free possible used ram
			DELETEARRAY(Line);
			DELETEARRAY(AlphaLine);

			// Override alpha if needed
	//		if(mValidExpSet->TexDiscardAlpha)	FinalAlpha = false;

			// Check bump maps
			if(tb.mIsBumpMap)	ExportedBitmap.MakeNormalMap();
		}

	// Fill export structure
//	if(!mSettings->mTexnameOnly)
	{
		if(mSettings->mFlipHorizontal)	ExportedBitmap.FlipHorizontal();
		if(mSettings->mFlipVertical)	ExportedBitmap.FlipVertical();

		TexInf.mWidth		=	ExportedBitmap.GetWidth();
		TexInf.mHeight		=	ExportedBitmap.GetHeight();
		TexInf.mPixels		=	new ubyte[TexInf.mWidth * TexInf.mHeight * 4];
		CopyMemory(TexInf.mPixels, ExportedBitmap.GetPixels(), TexInf.mWidth * TexInf.mHeight * 4);
		TexInf.mHasAlpha	=	FinalAlpha;
	}



	// Quantize texture if needed
	if(mSettings->mQuantizeTextures && TexInf.mPixels && !TexInf.mHasAlpha && TexInf.mWidth*TexInf.mHeight && !SkipQuantize)
	{
		SetExtStatus("quantizing texture...");

		// Create a neural net quantizer
		NNQ = new NeuralNetQuantizer;

		// Discard alpha channel
		ubyte* nqpic = new ubyte[TexInf.mWidth*TexInf.mHeight*3];
		for(udword i=0;i<TexInf.mWidth*TexInf.mHeight;i++)
		{
			nqpic[i*3+0] = TexInf.mPixels[i*4+0];
			nqpic[i*3+1] = TexInf.mPixels[i*4+1];
			nqpic[i*3+2] = TexInf.mPixels[i*4+2];
		}

		// Quantization
		QUANTIZERCREATE qc;
		qc.Field	= nqpic;
		qc.Length	= TexInf.mWidth*TexInf.mHeight*3;
		qc.Sample	= 1;
		NNQ->Quantize(qc);

		// Setup shortcuts to quantized texture
		TexInf.mQuantized	= NNQ->GetQuantizedPic();
		TexInf.mPalette		= NNQ->GetPalette();

		// Free used ram
		DELETEARRAY(nqpic);
	}

#ifdef TEST_DDS
	if(0)
	{
		SetExtStatus("building DXTC texture...");

	// Quite low - profile !
	char DDSName[1024];
	strcpy(DDSName, FullRGBTexName);
//	char* Dot = strrchr(DDSName, '.');
//	Dot++;
//	strcat(Dot, "dds");
	strcat(DDSName, ".dds");

		DDSCREATE dds;
		dds.mFilename	= DDSName;
		dds.mSource		= &ExportedBitmap;
ExportedBitmap.FixAlpha();
		dds.mFormat		= DDS_NONE;
		bool Status = SaveDDS(dds);
	}
#endif

	}
	// ~if texname only

	// Perform format-dependent export
	SetExtStatus("calling export plug-in...");
	if(mExporter)	mExporter->ExportTexture(TexInf);
	DumpLogString();

	DELETESINGLE(NNQ);

	// Update members
	mStats.mNbTexmaps++;
	if(mStats.mNbTexmaps==IMAX_MAX_TEXTURES)	::MessageBox(null, "IMAX_MAX_TEXTURES limit has been reached!\nHint: increase value and recompile.", "Oops", MB_OK);

	return TexInf.mObjectID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Computes the size of the destination texture, according to user settings.
 *	\param		source		[in] the source picture
 *	\return		the destination texture size (for both width & height)
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword MAXConverter::ComputeTextureSize(const Picture& source)	const
{
	// Take the lowest value as an input
	udword Size = source.GetWidth() < source.GetHeight() ? source.GetWidth() : source.GetHeight();

	// Then clamp it to the nearest power of 2
	Size = FirstLOD(Size);

	// Then clamp it according to user-settings
			if(mSettings->mTexMaxSize==2048)	{	return Size>2048	? 2048:	Size;	}
	else	if(mSettings->mTexMaxSize==1024)	{	return Size>1024	? 1024:	Size;	}
	else	if(mSettings->mTexMaxSize==512)		{	return Size>512		? 512:	Size;	}
	else	if(mSettings->mTexMaxSize==256)		{	return Size>256		? 256:	Size;	}
	else	if(mSettings->mTexMaxSize==128)		{	return Size>128		? 128:	Size;	}
	else	if(mSettings->mTexMaxSize==64)		{	return Size>64		? 64:	Size;	}
	else	if(mSettings->mTexMaxSize==32)		{	return Size>32		? 32:	Size;	}
	else	if(mSettings->mTexMaxSize==16)		{	return Size>16		? 16:	Size;	}
	else										{	return Size;						}
}

bool MAXConverter::CheckTextureSize(udword size, const char* name)
{
	if(!name)	return false;
	if(!mSettings->mTexMaxSize)	return true;	// "unlimited"
	if(mSettings->mTexMaxSize==2 && !IsPowerOfTwo(size))
	{
		mArrayLog.StoreASCII("WARNING! %s : not power of two.\n", name);
		return false;
	}
	if(size>mSettings->mTexMaxSize)
	{
		mArrayLog.StoreASCII("WARNING! %s : greater than max allowed size.\n", name);
		return false;
	}
	return true;
}

/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A method to convert a height field image into a normal map.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input	:	srcpic,		the creation structure.
// Output	:	-
// Return	:	false if failed, else true.
// Remark	:	This involves differencing each texel with its right and upper neighboor, then normalizing the cross product of the two difference vectors.
bool Converter::ConvertHeightFieldToNormalMap(IcePicture& srcpic)
{
	// Get some ram for the bump map
	IcePixel* BumpMap = new IcePixel[srcpic.GetWidth() * srcpic.GetHeight()];
	CHECKALLOC(BumpMap);

	const float oneOver255 = 1.0f/255.0f;

	float Scale = 4.0f;
	udword wr = srcpic.GetWidth();
	udword hr = srcpic.GetHeight();
	IcePixel* Data = srcpic.GetPixels();

	for(udword i=0;i<srcpic.GetHeight();i++)
	{
		for(udword j=0;j<srcpic.GetWidth();j++)
		{
			// Expand [0,255] texel values to the [0,1] range.
			float c = Data[i*wr + j].R * oneOver255;
			// Expand the texel to its right.
			float cx = Data[i*wr + (j+1)%wr].R * oneOver255;
			// Expand the texel one up.
			float cy = Data[((i+1)%hr)*wr + j].R * oneOver255;
			float dcx = Scale * (c - cx);
			float dcy = Scale * (c - cy);

			// Normalize the vector.
			float sqlen = dcx*dcx + dcy*dcy + 1.0f;
			float reciplen = 1.0f/sqrt(sqlen);
			float nx = dcx*reciplen;
			float ny = dcy*reciplen;
			float nz = reciplen;

			// Repack the normalized vector into an RGB unsigned byte vector in the normal map image.
			BumpMap[i*srcpic.GetWidth()+j].R = 128 + 127*nx;
			BumpMap[i*srcpic.GetWidth()+j].G = 128 + 127*ny;
			BumpMap[i*srcpic.GetWidth()+j].B = 128 + 127*nz;
			BumpMap[i*srcpic.GetWidth()+j].A = 0;
		}
	}
	CopyMemory(Data, BumpMap, srcpic.GetWidth() * srcpic.GetHeight() * sizeof(IcePixel));
	DELETEARRAY(BumpMap);

	return true;
}
*/

