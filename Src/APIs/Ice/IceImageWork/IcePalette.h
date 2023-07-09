///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains palette classes.
 *	\file		IcePalette.h
 *	\author		Pierre Terdiman
 *	\date		January, 17, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEPALETTE_H
#define ICEPALETTE_H

	class ICEIMAGEWORK_API RGBPalette
	{
		public:
		//! Constructor
										RGBPalette();
		//! Destructor
										~RGBPalette();
		// Settings
					bool				Init(udword nbcolors);
					bool				SetColor(udword index, ubyte r, ubyte g, ubyte b);
					bool				SetColor(udword index, const RGBPixel& color);
					bool				Spread(udword from, udword to);
		// Free
					void				Reset();
		// Data access
		inline_		udword				GetNbColors()		const	{ return mNbColors; }
		inline_		const RGBPixel*		GetColor(udword i)	const	{ return i<mNbColors ? &mColors[i] : null; }

		private:
					udword				mNbColors;
					RGBPixel*			mColors;
	};

	class ICEIMAGEWORK_API RGBAPalette
	{
		public:
		//! Constructor
										RGBAPalette();
		//! Destructor
										~RGBAPalette();
		// Settings
					bool				Init(udword nbcolors);
					bool				SetColor(udword index, ubyte r, ubyte g, ubyte b, ubyte a);
					bool				SetColor(udword index, const RGBAPixel& color);
					bool				Spread(udword from, udword to);
		// Free
					void				Reset();
		// Data access
		inline_		udword				GetNbColors()		const	{ return mNbColors; }
		inline_		const RGBAPixel*	GetColor(udword i)	const	{ return i<mNbColors ? &mColors[i] : null; }

		private:
					udword				mNbColors;
					RGBAPixel*			mColors;
	};

#endif // ICEPALETTE_H
