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
bool ASCIIFormat::ExportController(const ControllerDescriptor& controller)
{
	// Export database management information
	mControllers.StoreASCII("\n///////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n").StoreASCII("Controller: ");
	mControllers
		.StoreASCII((const char*)controller.mField)
		.StoreASCII("\nController ID: ")	.StoreASCII(controller.mObjectID)
		.StoreASCII("\nOwner ID: ")			.StoreASCII(controller.mOwnerID);

	// Controller data
	ControllerData* cdata = controller.mData;

	mControllers
		.StoreASCII("\nController data: \n");

	// Number of values (used in sampling only!)
	udword NbValues = 0;

			if(cdata->mType==FLX_CTRL_FLOAT)		{	mControllers.StoreASCII("Float controller, ");		NbValues = 1;	}
	else	if(cdata->mType==FLX_CTRL_VECTOR)		{	mControllers.StoreASCII("Vector controller, ");		NbValues = 3;	}
	else	if(cdata->mType==FLX_CTRL_QUAT)			{	mControllers.StoreASCII("Quaternion controller, ");	NbValues = 4;	}
	else	if(cdata->mType==FLX_CTRL_SCALE)		{	mControllers.StoreASCII("Scale controller, ");		NbValues = 7;	}
	else	if(cdata->mType==FLX_CTRL_PR)			{	mControllers.StoreASCII("PR controller, ");			NbValues = 7;	}
	else	if(cdata->mType==FLX_CTRL_PRS)			{	mControllers.StoreASCII("PRS controller, ");		NbValues = 10;	}
	else	if(cdata->mType==FLX_CTRL_VERTEXCLOUD)	{	mControllers.StoreASCII("Morph controller, ");						}
	else											{	mControllers.StoreASCII("Unknown controller, ");					}

			if(cdata->mMode==CTRL_SAMPLES)			mControllers.StoreASCII("sampling mode.\n\n");
	else	if(cdata->mMode==CTRL_KEYFRAMES)		mControllers.StoreASCII("keyframing mode.\n\n");
	else											mControllers.StoreASCII("Unknown mode.\n\n");

	// Dump the samples
	if(cdata->mMode==CTRL_SAMPLES)
	{
		// Sampling path
		if(cdata->mType==FLX_CTRL_VERTEXCLOUD)
		{
			// A morph controller
			MorphData* mdata = (MorphData*)cdata;
			mControllers
				.StoreASCII(mdata->mNbVertices).StoreASCII(" morphed vertices.\n")
				.StoreASCII(mdata->mNbSamples).StoreASCII(" samples, sampling rate = ").StoreASCII(mdata->mSamplingRate).StoreASCII("\n");

			// Dump morphed vertices as a list of floats
			float* Values = (float*)mdata->mSamples;
			for(udword i=0;i<3*mdata->mNbSamples*mdata->mNbVertices;i++)
			{
				mControllers.StoreASCII(*Values++).StoreASCII(" ");
				if((i&63)==63)	mControllers.StoreASCII("\n");
			}
		}
		else
		{
			// A standard controller
			SampleData* sdata = (SampleData*)cdata;
			mControllers.StoreASCII(sdata->mNbSamples).StoreASCII(" samples, sampling rate = ").StoreASCII(sdata->mSamplingRate).StoreASCII("\n");

			float* Values = (float*)sdata->mSamples;
			for(udword i=0;i<NbValues*sdata->mNbSamples;i++)
			{
				mControllers.StoreASCII(*Values++).StoreASCII(" ");
				if((i&63)==63)	mControllers.StoreASCII("\n");
			}
		}
	}
	else if(cdata->mMode==CTRL_KEYFRAMES)
	{
		// Keyframing path
		KeyframeData* kdata = (KeyframeData*)cdata;
		mControllers.StoreASCII(kdata->mNbKeyframes).StoreASCII(" keyframes, interpolation scheme = ");

		switch(kdata->mScheme)
		{
			case FLX_SCHEME_LINEAR:
			{
				mControllers.StoreASCII("LINEAR\n\n");

				if(controller.mField=="POSITION")
				{
					// Linear Point keyframes
					LinearPointKey* Keys = (LinearPointKey*)kdata->mKeyframes;
					for(udword i=0;i<kdata->mNbKeyframes;i++)
					{
						const LinearPointKey& CurKey = Keys[i];
						mControllers
							.StoreASCII("Key #")		.StoreASCII(i)					.StoreASCII(":\n")
							.StoreASCII("Time: ")		.StoreASCII(CurKey.mTime)		.StoreASCII("\n")
							.StoreASCII("Flags: ")		.StoreASCII(CurKey.mFlags)		.StoreASCII("\n")
							//
							.StoreASCII("x: ")			.StoreASCII(CurKey.mVal.x)		.StoreASCII("\n")
							.StoreASCII("y: ")			.StoreASCII(CurKey.mVal.y)		.StoreASCII("\n")
							.StoreASCII("z: ")			.StoreASCII(CurKey.mVal.z)		.StoreASCII("\n\n");
					}
				}
				else if(controller.mField=="ROTATION")
				{
					// Linear Rot keyframes
					LinearRotKey* Keys = (LinearRotKey*)kdata->mKeyframes;
					for(udword i=0;i<kdata->mNbKeyframes;i++)
					{
						const LinearRotKey& CurKey = Keys[i];
						mControllers
							.StoreASCII("Key #")		.StoreASCII(i)					.StoreASCII(":\n")
							.StoreASCII("Time: ")		.StoreASCII(CurKey.mTime)		.StoreASCII("\n")
							.StoreASCII("Flags: ")		.StoreASCII(CurKey.mFlags)		.StoreASCII("\n")
							//
							.StoreASCII("x: ")			.StoreASCII(CurKey.mVal.x)		.StoreASCII("\n")
							.StoreASCII("y: ")			.StoreASCII(CurKey.mVal.y)		.StoreASCII("\n")
							.StoreASCII("z: ")			.StoreASCII(CurKey.mVal.z)		.StoreASCII("\n")
							.StoreASCII("w: ")			.StoreASCII(CurKey.mVal.w)		.StoreASCII("\n\n");
					}
				}
				else if(controller.mField=="SCALE")
				{
					// Linear scale keyframes
					LinearScaleKey* Keys = (LinearScaleKey*)kdata->mKeyframes;
					for(udword i=0;i<kdata->mNbKeyframes;i++)
					{
						const LinearScaleKey& CurKey = Keys[i];
						mControllers
							.StoreASCII("Key #")		.StoreASCII(i)					.StoreASCII(":\n")
							.StoreASCII("Time: ")		.StoreASCII(CurKey.mTime)		.StoreASCII("\n")
							.StoreASCII("Flags: ")		.StoreASCII(CurKey.mFlags)		.StoreASCII("\n")
							//
							.StoreASCII("sx: ")			.StoreASCII(CurKey.s.x)			.StoreASCII("\n")
							.StoreASCII("sy: ")			.StoreASCII(CurKey.s.y)			.StoreASCII("\n")
							.StoreASCII("sz: ")			.StoreASCII(CurKey.s.z)			.StoreASCII("\n")
							.StoreASCII("qx: ")			.StoreASCII(CurKey.q.x)			.StoreASCII("\n")
							.StoreASCII("qy: ")			.StoreASCII(CurKey.q.y)			.StoreASCII("\n")
							.StoreASCII("qz: ")			.StoreASCII(CurKey.q.z)			.StoreASCII("\n")
							.StoreASCII("qw: ")			.StoreASCII(CurKey.q.w)			.StoreASCII("\n\n");
					}
				}
			}
			break;

			case FLX_SCHEME_TCB:
			{
				mControllers.StoreASCII("TCB\n\n");

				if(controller.mField=="POSITION")
				{
					// TCB Point keyframes
					TCBPointKey* Keys = (TCBPointKey*)kdata->mKeyframes;
					for(udword i=0;i<kdata->mNbKeyframes;i++)
					{
						const TCBPointKey& CurKey = Keys[i];
						mControllers
							.StoreASCII("Key #")		.StoreASCII(i)					.StoreASCII(":\n")
							.StoreASCII("Time: ")		.StoreASCII(CurKey.mTime)		.StoreASCII("\n")
							.StoreASCII("Flags: ")		.StoreASCII(CurKey.mFlags)		.StoreASCII("\n")
							//
							.StoreASCII("Tension: ")	.StoreASCII(CurKey.mTens)		.StoreASCII("\n")
							.StoreASCII("Continuity: ")	.StoreASCII(CurKey.mCont)		.StoreASCII("\n")
							.StoreASCII("Bias: ")		.StoreASCII(CurKey.mBias)		.StoreASCII("\n")
							.StoreASCII("EaseIn: ")		.StoreASCII(CurKey.mEaseIn)		.StoreASCII("\n")
							.StoreASCII("EaseOut: ")	.StoreASCII(CurKey.mEaseOut)	.StoreASCII("\n")
							//
							.StoreASCII("x: ")			.StoreASCII(CurKey.mVal.x)		.StoreASCII("\n")
							.StoreASCII("y: ")			.StoreASCII(CurKey.mVal.y)		.StoreASCII("\n")
							.StoreASCII("z: ")			.StoreASCII(CurKey.mVal.z)		.StoreASCII("\n\n");
					}
				}
				else if(controller.mField=="ROTATION")
				{
					// TCB Rot keyframes
					TCBRotKey* Keys = (TCBRotKey*)kdata->mKeyframes;
					for(udword i=0;i<kdata->mNbKeyframes;i++)
					{
						const TCBRotKey& CurKey = Keys[i];
						mControllers
							.StoreASCII("Key #")		.StoreASCII(i)					.StoreASCII(":\n")
							.StoreASCII("Time: ")		.StoreASCII(CurKey.mTime)		.StoreASCII("\n")
							.StoreASCII("Flags: ")		.StoreASCII(CurKey.mFlags)		.StoreASCII("\n")
							//
							.StoreASCII("Tension: ")	.StoreASCII(CurKey.mTens)		.StoreASCII("\n")
							.StoreASCII("Continuity: ")	.StoreASCII(CurKey.mCont)		.StoreASCII("\n")
							.StoreASCII("Bias: ")		.StoreASCII(CurKey.mBias)		.StoreASCII("\n")
							.StoreASCII("EaseIn: ")		.StoreASCII(CurKey.mEaseIn)		.StoreASCII("\n")
							.StoreASCII("EaseOut: ")	.StoreASCII(CurKey.mEaseOut)	.StoreASCII("\n")
							//
							.StoreASCII("x: ")			.StoreASCII(CurKey.mVal.x)		.StoreASCII("\n")
							.StoreASCII("y: ")			.StoreASCII(CurKey.mVal.y)		.StoreASCII("\n")
							.StoreASCII("z: ")			.StoreASCII(CurKey.mVal.z)		.StoreASCII("\n")
							.StoreASCII("w: ")			.StoreASCII(CurKey.mVal.w)		.StoreASCII("\n\n");
					}
				}
				else if(controller.mField=="SCALE")
				{
					// TCB scale keyframes
					TCBScaleKey* Keys = (TCBScaleKey*)kdata->mKeyframes;
					for(udword i=0;i<kdata->mNbKeyframes;i++)
					{
						const TCBScaleKey& CurKey = Keys[i];
						mControllers
							.StoreASCII("Key #")		.StoreASCII(i)					.StoreASCII(":\n")
							.StoreASCII("Time: ")		.StoreASCII(CurKey.mTime)		.StoreASCII("\n")
							.StoreASCII("Flags: ")		.StoreASCII(CurKey.mFlags)		.StoreASCII("\n")
							//
							.StoreASCII("Tension: ")	.StoreASCII(CurKey.mTens)		.StoreASCII("\n")
							.StoreASCII("Continuity: ")	.StoreASCII(CurKey.mCont)		.StoreASCII("\n")
							.StoreASCII("Bias: ")		.StoreASCII(CurKey.mBias)		.StoreASCII("\n")
							.StoreASCII("EaseIn: ")		.StoreASCII(CurKey.mEaseIn)		.StoreASCII("\n")
							.StoreASCII("EaseOut: ")	.StoreASCII(CurKey.mEaseOut)	.StoreASCII("\n")
							//
							.StoreASCII("sx: ")			.StoreASCII(CurKey.s.x)			.StoreASCII("\n")
							.StoreASCII("sy: ")			.StoreASCII(CurKey.s.y)			.StoreASCII("\n")
							.StoreASCII("sz: ")			.StoreASCII(CurKey.s.z)			.StoreASCII("\n")
							.StoreASCII("qx: ")			.StoreASCII(CurKey.q.x)			.StoreASCII("\n")
							.StoreASCII("qy: ")			.StoreASCII(CurKey.q.y)			.StoreASCII("\n")
							.StoreASCII("qz: ")			.StoreASCII(CurKey.q.z)			.StoreASCII("\n")
							.StoreASCII("qw: ")			.StoreASCII(CurKey.q.w)			.StoreASCII("\n\n");
					}
				}
			}
			break;

			case FLX_SCHEME_BEZIER:
			{
				mControllers.StoreASCII("BEZIER\n\n");

				if(controller.mField=="POSITION")
				{
					// Bezier Point keyframes
					BezierPointKey* Keys = (BezierPointKey*)kdata->mKeyframes;
					for(udword i=0;i<kdata->mNbKeyframes;i++)
					{
						const BezierPointKey& CurKey = Keys[i];
						mControllers
							.StoreASCII("Key #")		.StoreASCII(i)					.StoreASCII(":\n")
							.StoreASCII("Time: ")		.StoreASCII(CurKey.mTime)		.StoreASCII("\n")
							.StoreASCII("Flags: ")		.StoreASCII(CurKey.mFlags)		.StoreASCII("\n")
							//
							.StoreASCII("intan x: ")	.StoreASCII(CurKey.mInTan.x)	.StoreASCII("\n")
							.StoreASCII("intan y: ")	.StoreASCII(CurKey.mInTan.y)	.StoreASCII("\n")
							.StoreASCII("intan z: ")	.StoreASCII(CurKey.mInTan.z)	.StoreASCII("\n")
							.StoreASCII("outtan x: ")	.StoreASCII(CurKey.mOutTan.x)	.StoreASCII("\n")
							.StoreASCII("outtan y: ")	.StoreASCII(CurKey.mOutTan.y)	.StoreASCII("\n")
							.StoreASCII("outtan z: ")	.StoreASCII(CurKey.mOutTan.z)	.StoreASCII("\n")
							.StoreASCII("x: ")			.StoreASCII(CurKey.mVal.x)		.StoreASCII("\n")
							.StoreASCII("y: ")			.StoreASCII(CurKey.mVal.y)		.StoreASCII("\n")
							.StoreASCII("z: ")			.StoreASCII(CurKey.mVal.z)		.StoreASCII("\n\n");
					}
				}
				else if(controller.mField=="ROTATION")
				{
					// Bezier Rot keyframes
					BezierRotKey* Keys = (BezierRotKey*)kdata->mKeyframes;
					for(udword i=0;i<kdata->mNbKeyframes;i++)
					{
						const BezierRotKey& CurKey = Keys[i];
						mControllers
							.StoreASCII("Key #")		.StoreASCII(i)					.StoreASCII(":\n")
							.StoreASCII("Time: ")		.StoreASCII(CurKey.mTime)		.StoreASCII("\n")
							.StoreASCII("Flags: ")		.StoreASCII(CurKey.mFlags)		.StoreASCII("\n")
							//
							.StoreASCII("x: ")			.StoreASCII(CurKey.mVal.x)		.StoreASCII("\n")
							.StoreASCII("y: ")			.StoreASCII(CurKey.mVal.y)		.StoreASCII("\n")
							.StoreASCII("z: ")			.StoreASCII(CurKey.mVal.z)		.StoreASCII("\n")
							.StoreASCII("w: ")			.StoreASCII(CurKey.mVal.w)		.StoreASCII("\n\n");
					}
				}
				else if(controller.mField=="SCALE")
				{
					// Bezier scale keyframes
					BezierScaleKey* Keys = (BezierScaleKey*)kdata->mKeyframes;
					for(udword i=0;i<kdata->mNbKeyframes;i++)
					{
						const BezierScaleKey& CurKey = Keys[i];
						mControllers
							.StoreASCII("Key #")		.StoreASCII(i)					.StoreASCII(":\n")
							.StoreASCII("Time: ")		.StoreASCII(CurKey.mTime)		.StoreASCII("\n")
							.StoreASCII("Flags: ")		.StoreASCII(CurKey.mFlags)		.StoreASCII("\n")
							//
							.StoreASCII("intan x: ")	.StoreASCII(CurKey.mInTan.x)	.StoreASCII("\n")
							.StoreASCII("intan y: ")	.StoreASCII(CurKey.mInTan.y)	.StoreASCII("\n")
							.StoreASCII("intan z: ")	.StoreASCII(CurKey.mInTan.z)	.StoreASCII("\n")
							.StoreASCII("outtan x: ")	.StoreASCII(CurKey.mOutTan.x)	.StoreASCII("\n")
							.StoreASCII("outtan y: ")	.StoreASCII(CurKey.mOutTan.y)	.StoreASCII("\n")
							.StoreASCII("outtan z: ")	.StoreASCII(CurKey.mOutTan.z)	.StoreASCII("\n")
							.StoreASCII("sx: ")			.StoreASCII(CurKey.s.x)			.StoreASCII("\n")
							.StoreASCII("sy: ")			.StoreASCII(CurKey.s.y)			.StoreASCII("\n")
							.StoreASCII("sz: ")			.StoreASCII(CurKey.s.z)			.StoreASCII("\n")
							.StoreASCII("qx: ")			.StoreASCII(CurKey.q.x)			.StoreASCII("\n")
							.StoreASCII("qy: ")			.StoreASCII(CurKey.q.y)			.StoreASCII("\n")
							.StoreASCII("qz: ")			.StoreASCII(CurKey.q.z)			.StoreASCII("\n")
							.StoreASCII("qw: ")			.StoreASCII(CurKey.q.w)			.StoreASCII("\n\n");
					}
				}
			}
			break;

			default:
			{
				mControllers.StoreASCII("UNKNOWN\n\n");
			}
			break;
		}
	}

	return true;
}
