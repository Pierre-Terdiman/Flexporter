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
bool ASCIIFormat::ExportMotion(const MotionDescriptor& motion)
{
	// Export database management information
	mMotion.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("Motion for character: ");
	mMotion.StoreASCII(motion.mMasterID).StoreASCII("\n");

	mMotion.StoreASCII("Number of bones:  ").StoreASCII(motion.mNbBones).StoreASCII("\n");
	mMotion.StoreASCII("Number of vbones: ").StoreASCII(motion.mNbVirtualBones).StoreASCII("\n");
	mMotion.StoreASCII("Local PRS: ").StoreASCII(motion.mLocalPRS).StoreASCII("\n");
	mMotion.StoreASCII("Converted to D3D: ").StoreASCII(motion.mD3DCompliant).StoreASCII("\n");

	// Export motion name & motion type (v1.09)
	mMotion.StoreASCII("Motion name: ").StoreASCII((const char*)motion.mName).StoreASCII("\n");
	mMotion.StoreASCII("Motion type: ").StoreASCII((const char*)motion.mType).StoreASCII("\n");

	// Export samples
	udword* Data = (udword*)motion.mData;
	for(udword i=0;i<motion.mNbBones;i++)
	{
		udword CSID = *Data++;
		udword NbFrames = *Data++;
		mMotion.StoreASCII("CSID:    ").StoreASCII(CSID).StoreASCII("\n");
		mMotion.StoreASCII("#frames: ").StoreASCII(NbFrames).StoreASCII("\n");
		mMotion.StoreASCII("(tx, ty, tz)\n(qx qy qz qw)\n");
		float* p = (float*)Data;
		for(udword j=0;j<NbFrames;j++)
		{
			float tx = *p++;
			float ty = *p++;
			float tz = *p++;
			float qx = *p++;
			float qy = *p++;
			float qz = *p++;
			float qw = *p++;
			mMotion.StoreASCII(tx).StoreASCII(" ").StoreASCII(ty).StoreASCII(" ").StoreASCII(tz).StoreASCII("\n");
			mMotion.StoreASCII(qx).StoreASCII(" ").StoreASCII(qy).StoreASCII(" ").StoreASCII(qz).StoreASCII(" ").StoreASCII(qw).StoreASCII("\n");
		}
		Data = (udword*)p;
	}

	return true;
}
