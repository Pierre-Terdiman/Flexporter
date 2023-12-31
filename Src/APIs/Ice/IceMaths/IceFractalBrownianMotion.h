///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Fractal Brownian Motion from "Texturing & Modeling - a procedural approach"
 *	\file		IceFractalBrownianMotion.h
 *	\author		Pierre Terdiman
 *	\date		January, 29, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEFRACTALBROWNIANMOTION_H
#define ICEFRACTALBROWNIANMOTION_H

	class ICEMATHS_API FractalBrownianMotion
	{
		public:
		// Constructor/Destructor
								FractalBrownianMotion(float h, float lacunarity, float octaves);
								~FractalBrownianMotion();

				float			Compute(const Point& vector);
		protected:
				PerlinNoise		mNoise;
				float			mLacunarity;
				float			mOctaves;

				float*			mExponentArray;
	};

#endif // ICEFRACTALBROWNIANMOTION_H
