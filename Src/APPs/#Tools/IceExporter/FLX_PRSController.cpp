///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the code used to export a PRS controller.
 *	\file		FLX_PRSController.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Creates a PRS controller.
 *	\param		node			[in] the MAX node
 *	\param		exported_node	[in] the exported node's info structure
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::AddPRSController(INode* node, ResourceDescriptor* exported_node)
{
	// Checkings
	if(!node || !exported_node)	return false;

	 // Discard bones
//	if(IsSkeleton(node))		return true;

	// Single frame ? => exit
	if(mSettings->mSingleFrame)	return true;

	// Check user-properties overrides
	if(mProps.mDiscardPRS)		return true;

	// Checkings, unless user forced the export
	if(!mProps.mExportPRS)
	{
		// Nothing to do if the node is static

		// Want keyframes and no keyframes ? => exit
		if(!mSettings->mSampling && !IsKeyframed(node))						return true;
		// Want sampling and no animation ? => exit
		// ### commented temp to fix the IKWalker export bug => now handled in CreateSampledPRSData
//		if(mSettings->mSampling && !HasKeyframedAncestor(node, true))		return true;
	}

	// Export the controller
	{
		// Sampling or keyframing ?
		if(mSettings->mSampling)
		{
			// Sampling path

			// Get correct controlled type according to the settings
			ControllerType CType = mSettings->mRemoveScaling ? FLX_CTRL_PR : FLX_CTRL_PRS;

			// Dump the PR(S) data
			CustomArray PRSData;
			udword Nb = CreateSampledPRSData(node, CType, PRSData);

			// Dump if needed
			if(Nb)
			{
				// Create a new ControllerInfo structure. Not shared => create it, export it, destroy it
				ControllerDescriptor Ctrl;
				Ctrl.mField		= String("PRS");
				Ctrl.mObjectID	= mStats.mNbControllers++;
				Ctrl.mOwnerID	= exported_node->mObjectID;
				Ctrl.mOwnerType	= exported_node->mType;

				// Create ControllerData
				SampleDataCreate cdc;
				cdc.Mode			= CTRL_SAMPLES;
				cdc.Type			= CType;		// PR or PRS here
				cdc.NbSamples		= Nb;
				cdc.SamplingRate	= mSettings->GetSamplingRate();

				Ctrl.mData	= new SampleData;	CHECKALLOC(Ctrl.mData);
				if(Ctrl.mData->Init(&cdc))
				{
					CopyMemory(((SampleData*)Ctrl.mData)->mSamples, PRSData.Collapse(), PRSData.GetSize());
					_DUMP_CONTROLLER("sampled PRS")
				}
			}
		}
		else
		{
			// Keyframing path
			// Here we have to create 3 different controllers (for the position, rotation and scale tracks).
			// There's no global PR or PRS controllers since the number of keyframes can be different in
			// each track anyway. In other words, we follow MAX organization.

			// Position track
			Animatable* PosCtrl = node->GetTMController()->SubAnim(IMAX_POSITION);
			if(PosCtrl)
			{
				if(!ExportKeyframes(PosCtrl, "POSITION", "keyframed position", exported_node))
				{
					ExportKeyframes(PosCtrl->SubAnim(0), "X_POSITION", "keyframed X position", exported_node);
					ExportKeyframes(PosCtrl->SubAnim(1), "Y_POSITION", "keyframed Y position", exported_node);
					ExportKeyframes(PosCtrl->SubAnim(2), "Z_POSITION", "keyframed Z position", exported_node);
				}
#ifdef OLDIES
				// Dump the keyframes
				CustomArray KeyframedData;
				MAXCtrlType Type;
				udword Nb = CreateKeyframedData(PosCtrl, KeyframedData, Type);
				if(Nb)
				{
					// Create a new ControllerInfo structure. Not shared => create it, export it, destroy it
					ControllerDescriptor Ctrl;
					Ctrl.mField		= String("POSITION");
					Ctrl.mObjectID	= mStats.mNbControllers++;
					Ctrl.mOwnerID	= exported_node->mObjectID;
					Ctrl.mOwnerType	= exported_node->mType;

					// Create ControllerData
					KeyframeDataCreate kdc;
					kdc.Mode			= CTRL_KEYFRAMES;
					kdc.Type			= GetControllerType(Type);
					kdc.NbKeyframes		= Nb;
					kdc.Scheme			= GetControllerScheme(Type);

					Ctrl.mData	= new KeyframeData;	CHECKALLOC(Ctrl.mData);
					if(Ctrl.mData->Init(&kdc))
					{
						CopyMemory(((KeyframeData*)Ctrl.mData)->mKeyframes, KeyframedData.Collapse(), KeyframedData.GetOffset());
						_DUMP_CONTROLLER("keyframed position")
					}
				}
				else
				{
					int a = PosCtrl->NumSubs();
					for(int i=0;i<a;i++)
					{
						Animatable* AA = PosCtrl->SubAnim(i);
						TSTR NN = PosCtrl->SubAnimName(i);
					}
/*
					Control* XC = PosCtrl->GetXController();
					if(XC)
					{
						// Dump the keyframes
						CustomArray KeyframedData;
						MAXCtrlType Type;
						udword Nb = CreateKeyframedData(PosCtrl, KeyframedData, Type);
						if(Nb)
						{
							// Create a new ControllerInfo structure. Not shared => create it, export it, destroy it
							ControllerDescriptor Ctrl;
							Ctrl.mField		= String("XPOS");
							Ctrl.mObjectID	= mStats.mNbControllers++;
							Ctrl.mOwnerID	= exported_node->mObjectID;
							Ctrl.mOwnerType	= exported_node->mType;

							// Create ControllerData
							KeyframeDataCreate kdc;
							kdc.Mode			= CTRL_KEYFRAMES;
							kdc.Type			= GetControllerType(Type);
							kdc.NbKeyframes		= Nb;
							kdc.Scheme			= GetControllerScheme(Type);

							Ctrl.mData	= new KeyframeData;	CHECKALLOC(Ctrl.mData);
							if(Ctrl.mData->Init(&kdc))
							{
								CopyMemory(((KeyframeData*)Ctrl.mData)->mKeyframes, KeyframedData.Collapse(), KeyframedData.GetOffset());
								DUMP_CONTROLLER("keyframed position controller")
							}
						}
					}
					Control* YC = node->GetTMController()->GetYController();
					Control* ZC = node->GetTMController()->GetZController();
*/
				}
#endif
			}

			// Rotation track
			Animatable* RotCtrl = node->GetTMController()->SubAnim(IMAX_ROTATION);
			if(RotCtrl)
			{
/*				// Dump the keyframes
				CustomArray KeyframedData;
				MAXCtrlType Type;
				udword Nb = CreateKeyframedData(RotCtrl, KeyframedData, Type);
				if(Nb)
				{
					// Create a new ControllerInfo structure. Not shared => create it, export it, destroy it
					ControllerDescriptor Ctrl;
					Ctrl.mField		= String("ROTATION");
					Ctrl.mObjectID	= mStats.mNbControllers++;
					Ctrl.mOwnerID	= exported_node->mObjectID;
					Ctrl.mOwnerType	= exported_node->mType;

					// Create ControllerData
					KeyframeDataCreate kdc;
					kdc.Mode			= CTRL_KEYFRAMES;
					kdc.Type			= GetControllerType(Type);
					kdc.NbKeyframes		= Nb;
					kdc.Scheme			= GetControllerScheme(Type);

					Ctrl.mData	= new KeyframeData;	CHECKALLOC(Ctrl.mData);
					if(Ctrl.mData->Init(&kdc))
					{
						CopyMemory(((KeyframeData*)Ctrl.mData)->mKeyframes, KeyframedData.Collapse(), KeyframedData.GetOffset());
						_DUMP_CONTROLLER("keyframed rotation")
					}
				}*/
				if(!ExportKeyframes(RotCtrl, "ROTATION", "keyframed rotation", exported_node))
				{
					ExportKeyframes(PosCtrl->SubAnim(0), "X_ROTATION", "keyframed X rotation", exported_node);
					ExportKeyframes(PosCtrl->SubAnim(1), "Y_ROTATION", "keyframed Y rotation", exported_node);
					ExportKeyframes(PosCtrl->SubAnim(2), "Z_ROTATION", "keyframed Z rotation", exported_node);
				}
			}

			// Scale track
			Animatable* SclCtrl = node->GetTMController()->SubAnim(IMAX_SCALE);
			if(SclCtrl)
			{
/*				// Dump the keyframes
				CustomArray KeyframedData;
				MAXCtrlType Type;
				udword Nb = CreateKeyframedData(SclCtrl, KeyframedData, Type);
				if(Nb)
				{
					// Create a new ControllerInfo structure. Not shared => create it, export it, destroy it
					ControllerDescriptor Ctrl;
					Ctrl.mField		= String("SCALE");
					Ctrl.mObjectID	= mStats.mNbControllers++;
					Ctrl.mOwnerID	= exported_node->mObjectID;
					Ctrl.mOwnerType	= exported_node->mType;

					// Create ControllerData
					KeyframeDataCreate kdc;
					kdc.Mode			= CTRL_KEYFRAMES;
					kdc.Type			= GetControllerType(Type);
					kdc.NbKeyframes		= Nb;
					kdc.Scheme			= GetControllerScheme(Type);

					Ctrl.mData	= new KeyframeData;	CHECKALLOC(Ctrl.mData);
					if(Ctrl.mData->Init(&kdc))
					{
						CopyMemory(((KeyframeData*)Ctrl.mData)->mKeyframes, KeyframedData.Collapse(), KeyframedData.GetOffset());
						_DUMP_CONTROLLER("keyframed scale")
					}
				}*/
				if(!ExportKeyframes(SclCtrl, "SCALE", "keyframed scale", exported_node))
				{
					ExportKeyframes(PosCtrl->SubAnim(0), "X_SCALE", "keyframed X scale", exported_node);
					ExportKeyframes(PosCtrl->SubAnim(1), "Y_SCALE", "keyframed Y scale", exported_node);
					ExportKeyframes(PosCtrl->SubAnim(2), "Z_SCALE", "keyframed Z scale", exported_node);
				}
			}

		}
	}

	return true;
}
