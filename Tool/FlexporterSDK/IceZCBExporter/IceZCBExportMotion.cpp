///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Motion export method.
 *	This method is called once for each exported motion.
 *	\param		motion		[in] a structure filled with current motion information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBFormat::ExportMotion(const MotionDescriptor& motion)
{
	// Checkings
	udword* Data = (udword*)motion.mData;
	if(!Data)	return false;

	// Export database management information
	mMotion
		.Store(motion.mMasterID)
		.Store(motion.mNbBones)
		.Store(motion.mNbVirtualBones)
		.Store(motion.mLocalPRS)
		.Store(motion.mD3DCompliant);	// v1.13, CHUNK_MOVE_VER>=4

	// Export motion name & motion type (v1.09, CHUNK_MOVE_VER>=3)
	mMotion.Store((const char*)motion.mName).Store((ubyte)0);
	mMotion.Store((const char*)motion.mType).Store((ubyte)0);

	// Export samples
	for(udword i=0;i<motion.mNbBones;i++)
	{
		udword CSID		= *Data++;
		udword NbFrames	= *Data++;
		mMotion.Store(CSID).Store(NbFrames);
		float* p = (float*)Data;
		for(udword j=0;j<NbFrames;j++)
		{
			// Translation
			float tx = *p++;
			float ty = *p++;
			float tz = *p++;
			// Rotation
			float qx = *p++;
			float qy = *p++;
			float qz = *p++;
			float qw = *p++;
			mMotion
				.Store(tx).Store(ty).Store(tz)
				.Store(qx).Store(qy).Store(qz).Store(qw);
		}
		Data = (udword*)p;
	}

	return true;
}
