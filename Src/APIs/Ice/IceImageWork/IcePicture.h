///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains picture-related code.
 *	\file		IcePicture.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEPICTURE_H
#define ICEPICTURE_H

	// Forward references
	class FloatPicture;

	enum PixelComponent
	{
		PIXEL_R		= (1<<0),
		PIXEL_G		= (1<<1),
		PIXEL_B		= (1<<2),
		PIXEL_A		= (1<<3),
		PIXEL_FULL	= PIXEL_R|PIXEL_G|PIXEL_B|PIXEL_A,
		PIXEL_COLOR	= PIXEL_R|PIXEL_G|PIXEL_B,
	};

	enum AlphaType
	{
		AT_NONE,			//!< No alpha used - All 0xff
		AT_BINARY,			//!< 0x00 / 0xff
		AT_CONSTANT,		//!< Single, constant non-0xff value used 
		AT_CONSTANTBINARY,	//!< 0x00 / 0x??
		AT_DUALCONSTANT,	//!< 0x?? / 0x?? - Two constants
		AT_MODULATED,		//!< Multiple values used
	};

	// Functions
	FUNCTION ICEIMAGEWORK_API udword FirstLOD(udword nb);
	FUNCTION ICEIMAGEWORK_API udword NbMipMaps(udword width, udword height);
	FUNCTION ICEIMAGEWORK_API udword MakeColorMask(udword flags);

	struct ICEIMAGEWORK_API FntInfo
	{
		float		u0;
		float		v0;
		float		u1;
		float		v1;
		udword		dx;
		udword		dy;
	};

	class ICEIMAGEWORK_API Picture
	{
		public:
		// Constructor/Destructor
									Picture();
									Picture(const Picture* source);
									Picture(const Picture& source);
									Picture(uword width, uword height);
									~Picture();
		// Data access
		inline_		uword			GetWidth()			const	{ return mWidth;			}			//!< Returns the picture's width
		inline_		uword			GetHeight()			const	{ return mHeight;			}			//!< Returns the picture's height
		inline_		udword			GetNbPixels()		const	{ return mWidth*mHeight;	}			//!< Returns the nmber of pixels in the picture
		inline_		RGBAPixel*		GetPixels()			const	{ return mData;				}			//!< Returns the picture's pixels

		// Helpers
		inline_		udword			GetDataSize()		const	{ return mWidth*mHeight*sizeof(RGBAPixel); }
					bool			IsTranslucent()		const;
		// Fnts
					bool			IsLineEmpty(uword current_line);
					bool			IsColumnEmpty(uword current_column, uword y0, uword y1);
					bool			CopyRegion(const Picture& src, uword region_width, uword region_height, uword src_x, uword src_y, uword dst_x, uword dst_y);
					bool			CreateFntData(Container& fntuvs, udword texsize, udword& fntwidth, udword& fntheight, ubyte* xref=null, ubyte* alphabet=null);
		// Image operations
					bool			Init(uword width, uword height);										// Initialize a picture
					Picture&		Copy(const Picture& source);											// Copy from a source picture
					Picture&		Copy(const FloatPicture& source);										// Copy from a source picture
					Picture&		Clear(ubyte r=0, ubyte g=0, ubyte b=0, ubyte a=0);						// Clear a picture
					Picture&		FlipVertical();															// Flip the picture around the X axis
					Picture&		FlipHorizontal();														// Flip the picture around the Y axis
					Picture&		Rotate();																// Rotate 90 degrees
					Picture&		SwapColumns(udword nb=8);												// Random column permutations
					bool			Stretch(uword width, uword height);										// Stretch a picture
					bool			Quantize(ubyte* pal=null, ubyte* indices=null);							// Quantize a picture
					bool			Smooth(udword flags=PIXEL_FULL);										// Smooth a picture
					Picture&		CreatePhongTable(float power=3.0f, float max=255.0f);					// Create a Phong table
					Picture&		CreateSelfShadowMap(const RGBAPixel& color0, const RGBAPixel& color1);	// Create a self-shadowing map
					Picture&		CreateAnisotropicTable(float r=0.0f, float g=0.0f, float b=0.0f, float kd=1.0f, float ks=1.0f, float roughness=0.1f, bool metal=false);
					Picture&		CreateCubeMap();
					void			CreateMipmap();
					bool			MakeNormalMap();														// Transforms to normal map
					bool			MakePerlin(bool colored=true);
					bool			MakeMarble();
					bool			MakeWood();
					bool			MakeVoronoi(udword nb_seeds=16, udword flags=PIXEL_FULL);
					Picture&		MakeAlphaGrid(udword value);
					bool			Shade(const RGBAPixel& start_color, const RGBAPixel& end_color, udword start_line=INVALID_ID, udword end_line=INVALID_ID);
					bool			Fill(const RGBAPixel& color);
					bool			MakeRasters(const RGBAPixel& start_color, const RGBAPixel& mid_color, const RGBAPixel& end_color, udword mid_line=INVALID_ID);
					Picture&		Luminance();
					Picture&		Invert();
					Picture&		MakeClearer(udword offset=10);
					Picture&		MakeDarker(udword offset=10);
					bool			DiffuseError(Picture& dest, udword abits, udword rbits, udword gbits, udword bbits)	const;
		// Alpha stuff
					Picture&		ForceOpaque();
					Picture&		AlphaToColor();
					Picture&		ColorToAlpha();
					Picture&		AlphaLuminance();
					Picture&		BinaryAlpha();
					AlphaType		AlphaUsage(ubyte* alpha1=null, ubyte* alpha0=null);
					AlphaType		FixAlpha();

					Picture&		InPlaceZoomIn();
					Picture&		InPlaceZoomOut();
					Picture&		Spread(const RGBAPixel* default_color=null);
		//
					Picture&		Offset(uword offsetx, uword offsety);
					Picture&		Add(const Picture& img);
					Picture&		Sub(const Picture& img);
					Picture&		Modulate(const Picture& img);
					Picture&		Scale(float factor);

					Picture&		Tile(float power=3.0f, float amplitude=255.0f, float scale=1.0f);

		inline_		void			SetLine(uword y, const RGBAPixel* line)
									{
										CopyMemory(&mData[y*mWidth], line, mWidth*sizeof(RGBAPixel));
									}

		inline_		void			FillLine(uword y, const RGBAPixel& color)
									{
										uword Nb = mWidth;
										RGBAPixel* Dest = &mData[y*mWidth];
										while(Nb--)	*Dest++ = color;
									}

		//! Operator for "Picture A = Picture B"
		inline_		void			operator = (const Picture& source)	{ Copy(source);	}

		private:
					uword			mWidth;		//!< Picture's width
					uword			mHeight;	//!< Picture's height
					RGBAPixel*		mData;		//!< Picture's pixels
	};

#endif // ICEPICTURE_H

