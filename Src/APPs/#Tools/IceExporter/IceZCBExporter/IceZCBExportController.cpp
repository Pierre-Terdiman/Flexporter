///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Controller export method.
 *	This method is called once for each exported controller.
 *	\param		controller		[in] a structure filled with current controller information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ZCBFormat::ExportController(const ControllerDescriptor& controller)
{
	mControllers
		.Store((const char*)controller.mField).Store((ubyte)0)		// Controlled field
		.Store(controller.mObjectID)								// Controller ID
		.Store(controller.mOwnerID)									// Owner ID
		.Store(controller.mOwnerType);								// Owner type

	// Controller data
	ControllerData* cdata = controller.mData;

	udword NbValues = 0;

	// Save controller type
			if(cdata->mType==FLX_CTRL_FLOAT)		{	mControllers.Store((udword)ZCB_CTRL_FLOAT);		NbValues = 1;	}
	else	if(cdata->mType==FLX_CTRL_VECTOR)		{	mControllers.Store((udword)ZCB_CTRL_VECTOR);	NbValues = 3;	}
	else	if(cdata->mType==FLX_CTRL_QUAT)			{	mControllers.Store((udword)ZCB_CTRL_QUAT);		NbValues = 4;	}
	else	if(cdata->mType==FLX_CTRL_SCALE)		{	mControllers.Store((udword)ZCB_CTRL_SCALE);		NbValues = 7;	}
	else	if(cdata->mType==FLX_CTRL_PR)			{	mControllers.Store((udword)ZCB_CTRL_PR);		NbValues = 7;	}
	else	if(cdata->mType==FLX_CTRL_PRS)			{	mControllers.Store((udword)ZCB_CTRL_PRS);		NbValues = 10;	}
	else	if(cdata->mType==FLX_CTRL_VERTEXCLOUD)	{	mControllers.Store((udword)ZCB_CTRL_VERTEXCLOUD);				}
	else											{	mControllers.Store((udword)ZCB_CTRL_NONE);						}

	// Save controller mode
			if(cdata->mMode==CTRL_SAMPLES)			mControllers.Store((udword)ZCB_CTRL_SAMPLES);
	else	if(cdata->mMode==CTRL_KEYFRAMES)		mControllers.Store((udword)ZCB_CTRL_KEYFRAMES);
	else											mControllers.Store((udword)ZCB_CTRL_NONE);

	// Dump the samples
	if(cdata->mMode==CTRL_SAMPLES)
	{
		if(cdata->mType==FLX_CTRL_VERTEXCLOUD)
		{
			// A morph controller
			MorphData* mdata = (MorphData*)cdata;
			mControllers.Store(mdata->mNbVertices).Store(mdata->mNbSamples).Store(mdata->mSamplingRate);

			// Dump morphed vertices as a list of floats
			float* Values = (float*)mdata->mSamples;
			for(udword i=0;i<3*mdata->mNbSamples*mdata->mNbVertices;i++)	mControllers.Store(*Values++);
		}
		else
		{
			// A standard controller
			SampleData* sdata = (SampleData*)cdata;
			mControllers.Store(sdata->mNbSamples).Store(sdata->mSamplingRate);

			// Dump samples as a list of floats
			float* Values = (float*)sdata->mSamples;
			for(udword i=0;i<NbValues*sdata->mNbSamples;i++)				mControllers.Store(*Values++);
		}
	}

	// WARNING: keyframes not supported in ZCB files as I don't replay them.
	// They should be exposed anyway, see the ASCII format.

	return true;
}
