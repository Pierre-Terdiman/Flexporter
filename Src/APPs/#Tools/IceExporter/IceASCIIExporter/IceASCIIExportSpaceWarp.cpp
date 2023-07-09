///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Space warp export method.
 *	This method is called once for each exported space warp.
 *	\param		space_warp	[in] a structure filled with current space_warp information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ASCIIFormat::ExportSpaceWarp(const SpaceWarpDescriptor& space_warp)
{
	// 1) Export common information
	mSpaceWarps.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("Space warp:       ");
	ExportBasicInfo(&space_warp, &mSpaceWarps);

	// 2) Export space warp parameters
	// 2-1) Export space warp's type
	mHelpers.StoreASCII("Helper type: ");
			if(space_warp.mSWType==FLX_SWHTYPE_GRAVITY)		mSpaceWarps.StoreASCII("gravity\n");
	else	if(space_warp.mSWType==FLX_SWHTYPE_WIND)		mSpaceWarps.StoreASCII("wind\n");
	else	if(space_warp.mSWType==FLX_SWTYPE_UNDEFINED)	mSpaceWarps.StoreASCII("undefined space warp\n");

	// 2-2) Export space warp's data
	switch(space_warp.mSWType)
	{
		case FLX_SWHTYPE_GRAVITY:
		{
			const GravityObject& Gravity = (const GravityObject&)space_warp;
			mSpaceWarps
				.StoreASCII("Strength:   ").StoreASCII(Gravity.mStrength).StoreASCII("\n")
				.StoreASCII("Decay:      ").StoreASCII(Gravity.mDecay).StoreASCII("\n")
				.StoreASCII("DispLength: ").StoreASCII(Gravity.mDispLength).StoreASCII("\n");
		}
		break;
		case FLX_SWHTYPE_WIND:
		{
			const WindObject& Wind = (const WindObject&)space_warp;
			mSpaceWarps
				.StoreASCII("Strength:   ").StoreASCII(Wind.mStrength).StoreASCII("\n")
				.StoreASCII("Decay:      ").StoreASCII(Wind.mDecay).StoreASCII("\n")
				.StoreASCII("DispLength: ").StoreASCII(Wind.mDispLength).StoreASCII("\n")
				.StoreASCII("Turbulence: ").StoreASCII(Wind.mTurbulence).StoreASCII("\n")
				.StoreASCII("Frequency:  ").StoreASCII(Wind.mFrequency).StoreASCII("\n")
				.StoreASCII("Scale:      ").StoreASCII(Wind.mScale).StoreASCII("\n");
		}
		break;
	};

	return true;
}
