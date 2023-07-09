///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Camera export method.
 *	This method is called once for each exported camera.
 *	\param		camera		[in] a structure filled with current camera information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ASCIIFormat::ExportCamera(const CameraDescriptor& camera)
{
	// Export common information
	mCameras.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("Camera:       ");
	ExportBasicInfo(&camera, &mCameras);

	// Export camera parameters
	if(camera.mOrthoCam)
	{
		mCameras.StoreASCII("Type: orthographic\nWidth: ").StoreASCII(camera.mFOV).StoreASCII("\n");
	}
	else
	{
		mCameras.StoreASCII("Type: perspective\nField Of View: ").StoreASCII(camera.mFOV).StoreASCII("\n");
	}

	// FOV types
	mCameras.StoreASCII("FOV type: ");
			if(camera.mFOVType==FLX_FOV_HORIZONTAL)	mCameras.StoreASCII("horizontal\n");
	else	if(camera.mFOVType==FLX_FOV_VERTICAL)	mCameras.StoreASCII("vertical\n");
	else	if(camera.mFOVType==FLX_FOV_DIAGONAL)	mCameras.StoreASCII("diagonal\n");
	else											mCameras.StoreASCII("unknown\n");

	// Camera types
	mCameras.StoreASCII("Camera type: ");
			if(camera.mCamType==FLX_CTYPE_FREE)		mCameras.StoreASCII("free camera\n");
	else	if(camera.mCamType==FLX_CTYPE_TARGET)	mCameras.StoreASCII("target camera\n");
	else	if(camera.mCamType==FLX_CTYPE_PARALLEL)	mCameras.StoreASCII("parallel camera\n");
	else											mCameras.StoreASCII("unknown type\n");

	// Horizon line display
	mCameras.StoreASCII("Horizon line display: ")	.StoreASCII(camera.mHLineDisplay).StoreASCII("\n");	// Horizon line display

	// Export environment ranges if needed
	// Environment ranges
	mCameras
		.StoreASCII("\nENVIRONMENT RANGES:\n")
		.StoreASCII("\nNear range: ")				.StoreASCII(camera.mEnvNearRange)					// Near range
		.StoreASCII("\nFar range: ")				.StoreASCII(camera.mEnvFarRange)					// Far range
	// Environment display
		.StoreASCII("\nEnv display: ")				.StoreASCII(camera.mEnvDisplay)						// Environment display
		.StoreASCII("\n");

	// Export clipping planes if needed
	mCameras
		.StoreASCII("\nCLIPPING PLANES:\n")
		.StoreASCII("\nManual clip: ")				.StoreASCII(camera.mManualClip)						// Manual clip
		.StoreASCII("\nNear clip: ")				.StoreASCII(camera.mNearClip)						// Near clip
		.StoreASCII("\nFar clip: ")					.StoreASCII(camera.mFarClip)						// Far clip
		.StoreASCII("\n");

	return true;
}
