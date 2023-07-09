///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains a simple fog wrapper.
 *	\file		IceFog.h
 *	\author		Pierre Terdiman
 *	\date		August, 9, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEFOG_H
#define ICEFOG_H

	struct ICERENDERER_API PIXELFOGCREATE
	{
		udword		mColor;				//!< Fog color (e.g. background color)
		FOGMODE		mMode;				//!< Fog mode
		// Linear mode parameters
		float		mStart;
		float		mEnd;
		// Exponential modes parameters
		float		mDensity;
	};

	struct ICERENDERER_API VERTEXFOGCREATE : PIXELFOGCREATE
	{
		bool		mRangeBased;
	};

	class ICERENDERER_API FogMachine
	{
		public:
		// Constructor/Destructor
											FogMachine(RenderStateManager* rsm);
											~FogMachine();

		// Enable pixel fog
						bool				SetPixelFog(const PIXELFOGCREATE& pfc);
		// Enable vertex fog
						bool				SetVertexFog(const VERTEXFOGCREATE& vfc);
		// Disable fog
						bool				DisableFog();
		private:
					RenderStateManager*		mRSM;
	};

#endif // ICEFOG_H
