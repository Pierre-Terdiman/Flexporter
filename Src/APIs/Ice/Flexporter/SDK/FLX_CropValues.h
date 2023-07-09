///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the cropping values structures.
 *	\file		FLX_CropValues.h
 *	\author		Pierre Terdiman
 *	\date		February, 13, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_CROPVALUES_H
#define FLX_CROPVALUES_H

	enum Tiling
	{
		TILING_WRAP		= (1<<0),
		TILING_MIRROR	= (1<<1),
	};

	//! A possible texture matrix.
	class FLEXPORTER_API TextureMatrix
	{
		public:
				TextureMatrix()	{ Identity();	}

		void	Identity()
		{
			ZeroMemory(m, sizeof(m));
			m[0][0] = m[1][1] = m[2][2] = 1.0f;
		}

		void	Apply(float& u, float& v, float& w)
		{
			float x = u * m[0][0] + v * m[1][0] + w * m[2][0] + m[3][0];
			float y = u * m[0][1] + v * m[1][1] + w * m[2][1] + m[3][1];
			float z = u * m[0][2] + v * m[1][2] + w * m[2][2] + m[3][2];
			u = x;
			v = y;
			w = z;
		}

		//! Warning: that's a direct dump from MAX (untransformed)
		float	m[4][3];
	};

	//! Crop values. Useful for T-pages. Warning: equations to use them are weird.
	class FLEXPORTER_API TextureCrop
	{
		public:
				TextureCrop()	{ Reset();	}

		void	Reset()
		{
			mOffsetU = mOffsetV = 0.0f;
			mScaleU = mScaleV = 1.0f;
		}

		// tu= (u*ScaleU) + OffsetU;
		// tv= ((v-1.0f)*ScaleV) - OffsetV + 1.0f;
		void	Apply(float& u, float& v)
		{
							u*=mScaleU;	u+=mOffsetU;
				v-=1.0f;	v*=mScaleV;	v-=mOffsetV;
				v+=1.0f;	// Flex 1.17 fix
		}

		float	mOffsetU;	//!< Offset for U
		float	mOffsetV;	//!< Offset for V
		float	mScaleU;	//!< Scale for U
		float	mScaleV;	//!< Scale for V
	};

	class FLEXPORTER_API TextureTransform
	{
		public:
						TextureTransform()	{ mTextureTilingU = mTextureTilingV = 0; }

		TextureCrop		mCValues;			//!< Cropping values
		TextureMatrix	mTMtx;				//!< Texture matrix
		udword			mTextureTilingU;	//!< Tiling for U
		udword			mTextureTilingV;	//!< Tiling for V
	};

#endif // FLX_CROPVALUES_H
