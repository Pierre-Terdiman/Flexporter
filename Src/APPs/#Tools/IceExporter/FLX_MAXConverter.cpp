///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the main MAX converter.
 *	\file		FLX_Converter.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

// For SetLocale
#include <locale.h>

using namespace IceExporter;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXConverter::MAXConverter()
{
	mDialog					= null;
	mCurrentConvertedNode	= 0;

	mIp						= null;
	mRootNode				= null;
	mSelectedNodes			= null;
#ifdef SUPPORT_OLD_LIGHTMAPPER
	mMapper					= null;
#endif

	mNbSubMtls				= 0;
	mCurrentNbMtls			= 0;
	mNbUsedMaterials		= 0;

	mExportUVW				= false;
	mExportColors			= false;
	mParity					= false;
#ifdef SUPPORT_OLD_LIGHTMAPPER
	mLightmapPass			= false;
#endif
	mParseSceneInfos		= false;
	mHelpText				= false;

	mGroundPlane			= false;
	mCollisionDetection		= true;
	mGravity.SetNotUsed();
	mRestitution			= -1.0f;
	mStaticFriction			= -1.0f;
	mFriction				= -1.0f;

	mTesselation			= INVALID_ID;
	mLightingMode			= INVALID_ID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXConverter::~MAXConverter()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Updates the Status Text of the Export Dialog.
 *	The status extended text is always cleared when you use this method.
 *
 *	\param		text	[in] the string to display
 *	\return		Self-reference.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXConverter& MAXConverter::SetStatus(const char* text)
{
	// Call user plug
	bool DefaultGUI = true;
	if(mExporter)	DefaultGUI = mExporter->SetStatus(text);

	if(DefaultGUI && mDialog)
	{
		// Handle the text with default GUI
		udword NbChars = strlen(text);
		char* StatusMsg = new char[NbChars+64];
		if(StatusMsg)
		{
			strcpy(StatusMsg, "Status: ");	// This way the word "Status" always appear
			strcat(StatusMsg, text);		// Append status message...
			mDialog->m_Status = StatusMsg;	// ...and copy it to dialog
			mDialog->UpdateData(FALSE);		// Update dialog
			DELETEARRAY(StatusMsg);
		}
	}

	SetExtStatus(" ");						// Clear extended status
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Updates the Status Extended Text of the Export Dialog.
 *	\param		text	[in] the string to display
 *	\return		Self-reference.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXConverter& MAXConverter::SetExtStatus(const char* text)
{
	// Call user plug
	bool DefaultGUI = true;
	if(mExporter)	DefaultGUI = mExporter->SetExtStatus(text);

	if(DefaultGUI && mDialog)
	{
		// Handle the text with default GUI
		udword NbChars = strlen(text);
		char* StatusMsg = new char[NbChars+64];
		if(StatusMsg)
		{
			strcpy(StatusMsg, "=> ");			// Looks nicer
			strcat(StatusMsg, text);			// Append extended status message...
			mDialog->m_ExtStatus = StatusMsg;	// ...and copy it to dialog
			mDialog->UpdateData(FALSE);			// Update dialog
			DELETEARRAY(StatusMsg);
		}
	}
/*
	char StatusMsg[256];							// Some bytes...
	if((strlen(text)+10)>=256) return *this;		// Prevent bugs...
	strcpy(StatusMsg, "=> ");						// Looks nicer
	strcat(StatusMsg, text);						// Append extended status message...
	if(mDialog)	mDialog->m_ExtStatus = StatusMsg;	// ...and copy it to dialog
	if(mDialog)	mDialog->UpdateData(FALSE);			// Update dialog
*/
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Displays the final log text.
 *	\return		Self-reference.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXConverter& MAXConverter::DisplayLogText()
{
	// Store final message
	mArrayLog.StoreASCII("\n\n\nExport completed.\n");
	mArrayLog.Store((ubyte)0);
	// Transform into linear buffer
	const char* LogText = (const char*)mArrayLog.Collapse();

	// Call user plug
	bool DefaultGUI = true;
	if(mExporter)	DefaultGUI = mExporter->DisplayLogText(LogText);

	if(DefaultGUI)
	{
		// There seems to be some issues when text is too big....
		IceMessage MsgDlg;
		MsgDlg.m_Text = LogText;
		MsgDlg.CenterWindow();

		// Display log text (not for lightmapping)
#ifdef SUPPORT_OLD_LIGHTMAPPER
		if(!mLightmapPass)
#endif
		MsgDlg.DoModal();
	}

	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This method is called once before starting the export process.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MAXConverter::StartExport()
{
	// Initialize log text
	mArrayLog.StoreASCII("ICE Exporter Log Text\n\n");

	// Call user plug
	bool DefaultGUI = true;
	if(mExporter)	DefaultGUI = mExporter->InitGUI();

	if(DefaultGUI)
	{
		// Create dialog
		mDialog = new IceExportDlg;
		ASSERT(mDialog);
		if(!mDialog)	return;

		mDialog->Create(IceExportDlg::IDD, null);
		// Register dialog in MAX
		mIp->RegisterDlgWnd(mDialog->m_hWnd);
		DisableAccelerators();

		// Init dialog
		mDialog->SetForegroundWindow();
		mDialog->m_Progress.SetRange(0, 100);
		mDialog->CenterWindow();
	}

	SetStatus("Initializing");

	if(mDialog)
	{
		mDialog->Invalidate();
		mDialog->ShowWindow(SW_SHOW);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This method is called once after the export process.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MAXConverter::EndExport()
{
	SetStatus("export complete!");

	// Call user plug
	bool DefaultGUI = true;
	if(mExporter)	DefaultGUI = mExporter->CloseGUI();

	// 1) Free dialog (we don't want to override that)
	if(mDialog)
	{
		mDialog->m_Progress.SetPos(100);
		mIp->UnRegisterDlgWnd(mDialog->m_hWnd);
		EnableAccelerators();
		mDialog->Invalidate();
		DELETESINGLE(mDialog);
	}
	// 2) Display log text
	DisplayLogText();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	[DEBUG] Dumps the sub-anims of a given node
 *	\param		node	[in] MAX node
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MAXConverter::DumpSubAnimsNames(Animatable* node)
{
	if(!node)	return;

	static udword Indent = 0;

	int Nb = node->NumSubs();

	if(Nb)
	{
		for(int j=0;j<Indent+2;j++)	mArrayLog.StoreASCII(" ");
		mArrayLog.StoreASCII("%d sub-anims:\n", Nb);
	}

	Indent+=4;

	for(int i=0;i<Nb;i++)
	{
		for(int j=0;j<Indent;j++)	mArrayLog.StoreASCII(" ");

		mArrayLog.StoreASCII("%d : %s   (%d)\n", i, node->SubAnimName(i), node->SubAnim(i));
		DumpSubAnimsNames(node->SubAnim(i));
	}

	Indent-=4;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets the scale factor of a given node.
 *	This is used to help removing non-uniform scaling in biped objects. After the call mScaling is updated with the new scale.
 *	Removing non-uniform scaling:
 *		It seems it should only be done on biped (skeleton) parts, since we're sure the scale won't change from one frame to another.
 *		For other objects, removing non-uniform scaling seems to be pointless, unless the scale is constant over the active time segment.
 *
 *	\param		node		[in] a MAX node
 *	\param		from_node	[in] true to get scaling from the NodeTM, false to get it from the ObjectTM.
 *	\return		scaling vector reference
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Point& MAXConverter::GetScalingFrom(INode* node, bool from_node)
{
	// We remove scaling:
	// 1) for BIPED parts
	// 2) for any node if that's the user's will
	BoneType BT = IsSkeleton(node);
	if(BT!=BONE_NONE || mSettings->mRemoveScaling)
	{
#ifdef OLDIES
		Interval Inter=FOREVER;		// Validity interval
#endif

		// One single frame must be saved ? We catch the scale from this frame.
		// All frames must be saved? We catch the scale from first frame.
		TimeValue Frame = mSettings->mSingleFrame ? mTimeInfo.mCurrentTime : 0;
#ifdef OLDIES
		Matrix3 ObjTM;

		if(from_node)	ObjTM = node->GetNodeTM(Frame, &Inter);	// And not GetObjectTM, else Damien's Battoo is whacked. (and the CS Guy as well)
		else			ObjTM = node->GetObjectTM(Frame, &Inter);
/*
//		ObjTM = node->GetNodeTM(Frame, &Inter);	// And not GetObjectTM, else Damien's Battoo is whacked. (and the CS Guy as well)

		if(BT==BONE_BIPED_MAIN || BT==BONE_BIPED_SLAVE || BT==BONE_BIPED_FOOT || BT==BONE_BIPED_DUMMY)
		{
//			ObjTM = node->GetNodeTM(Frame, &Inter);	// And not GetObjectTM, else Damien's Battoo is whacked. (and the CS Guy as well)
			ObjTM = node->GetObjectTM(Frame, &Inter);
		}
		else
		{
			ObjTM = node->GetObjectTM(Frame, &Inter);
		}
*/
		AffineParts	StaticPRS;
		decomp_affine(ObjTM, &StaticPRS);

		SetScaling(
			StaticPRS.k.x * StaticPRS.f,
			StaticPRS.k.y * StaticPRS.f,
			StaticPRS.k.z * StaticPRS.f
			);
#endif
		Point Scale;
		IceExporter::GetScaling(Scale, node, Frame, from_node);
		SetScaling(Scale);
	}
	else
	{
		SetScaling(1.0f, 1.0f, 1.0f);
	}

	// Setup XForm
	ResetXForm();

	return GetScaling();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Exports a header for a given node.
 *	\param		node	[in] a MAX node
 *	\param		obj		[out] destination structure
 *	\return		Self-reference.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MAXConverter& MAXConverter::AddHeader(INode* node, ObjectDescriptor* obj)
{
	// Checkings
	if(!node || !obj)	return *this;

	INode* ParentNode	= node->GetParentNode();								// Parent node pointer in MAX
	long NodeID			= mGlobalInfo.GetNodeID(node);							// Current node's ID
	long ParentNodeID	= mGlobalInfo.GetNodeID(ParentNode);					// Parent node's ID

	obj->mInternalData	= node;													// Give internal access to MAX nodes
	obj->mInternalData2	= mIp;													// Give internal access to the MAX interface
	obj->mName			= (ubyte*)node->GetName();								// Save name
	obj->mParentName	= ParentNode ? (ubyte*)ParentNode->GetName() : null;	// Save parent name
	obj->mIsGroupMember	= node->IsGroupMember()!=0;								// Save group belonging
	obj->mIsHidden		= node->IsHidden()!=0;									// Save hidden status
	obj->mObjectID		= NodeID;												// Save ID
	obj->mParentID		= ParentNodeID;											// Save parent ID
	obj->mWireColor		= (udword)node->GetWireColor();							// Save wire color

	// Extra user-defined properties
	mProps.SetupObject(*obj);

	// Possible target ID
	INode* Target		= node->GetTarget();									// Get possible target
	obj->mTargetID		= Target ? mGlobalInfo.GetNodeID(Target) : INVALID_ID;

	// Get user-defined properties
	static CStr Buffer;
	node->GetUserPropBuffer(Buffer);
	obj->mUserProps = strlen(Buffer.data()) ? (ubyte*)Buffer.data() : null;

	// Get PRS
	ConvertPRS(node, mTimeInfo.mCurrentTime, (PRS&)obj->mPrs);
	obj->mLocalPRS		= !mSettings->mAbsolutePRS;
	obj->mD3DCompliant	= mSettings->mConvertToD3D;

	// Get pivot
	GetPivot(node, obj->mPivot);

	// Get IK data
	GetIK(node, *obj);

	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	The main conversion entry point.
 *	\param		params		[in] the conversion settings
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MAXConverter::Convert(const ConversionSettings* params)
{
	// Save conversion parameters ### we should copy the struct ?
	const MAXConversionSettings* MAX_Params = (const MAXConversionSettings*)params;
	mIp				= MAX_Params->mIp;
	mRootNode		= MAX_Params->mRootNode;
	mSelectedNodes	= MAX_Params->mSelectedNodes;
#ifdef SUPPORT_OLD_LIGHTMAPPER
	mMapper			= MAX_Params->mLightmapper;
#endif
	// ### From this point we should actually call the base class
	mExporter		= params->mExporter;
	mSettings		= params->mSettings;
	bool ExpMotion	= !params->mSceneFile;
#ifdef SUPPORT_OLD_LIGHTMAPPER
	mLightmapPass	= params->mLightmapPass;
#endif
	// Checkings
	if(!mIp || !mSettings)	return false;

	// Create export dialog
	StartExport();

	// Catch scene properties, parse them, expose them to user plugins. We do that before calling SetSettings, since
	// some settings can be overrided here.
	{
		const char* SceneInfo = null;

		// 1) Catch MAX scene properies if they exist
		const PROPVARIANT* SceneProps = mIp->GetPropertyVariant(PROPSET_SUMMARYINFO, 0);
		SceneInfo = SceneProps ? SceneProps->pszVal : null;

		// 2) Parse a slightly modified version
		if(SceneInfo)
		{
			String StringToParse(SceneInfo);
//			StringToParse.Replace('=', ' ');	// We want to treat "Item = value" like "Item value"
//			StringToParse.ToLower();
			mParseSceneInfos = true;
			Parse(StringToParse);
		}

		// 3) SceneInfo (original version) will further be exposed in MainInf
	}

	// The exporter needs access to the settings too. Note: the exporter can be a lightmapper as well.
	if(mExporter)
	{
		mExporter->SetSettings(*mSettings);
		mExporter->SetOutputFilename(params->mOutputName);
		mExporter->ResetLogString();
	}

	// Setup local settings
	switch(mSettings->mLocale)
	{
		default:
			setlocale(LC_NUMERIC, "English");
			break;

		case FLX_LOCALE_FRENCH:
			setlocale(LC_NUMERIC, "French");
			break;
	};

	// Initialize exporter
	if(mExporter)	mExporter->Init(ExpMotion);
	DumpLogString();

	// Get time info
	GetTimeInfo(mIp, mTimeInfo);
	long		StartTime		= mTimeInfo.mStartTime;
	long		EndTime			= mTimeInfo.mEndTime;
	long		DeltaTime		= mTimeInfo.mDeltaTime;
	long		FrameRate		= mTimeInfo.mFrameRate;
	TimeValue	CurrentTime		= mTimeInfo.mCurrentTime;

	// Precompute number of samples
	udword NbSamples=0;	long SamplingRate = mSettings->GetSamplingRate();
	for(long i=mTimeInfo.mStartTime;i<=mTimeInfo.mEndTime;i+=mTimeInfo.mDeltaTime * SamplingRate)	NbSamples++;
	mTimeInfo.mNbFrames = NbSamples;

	MainDescriptor MainInf;
//	MainInf.mConverter			= this;
	MainInf.mTime				= mTimeInfo;
	MainInf.mGlobalScale		= mSettings->mGlobalScale;
	MainInf.mSceneInfos			= mSceneInfo;
	MainInf.mSceneHelpText		= mSceneHelpText;
	MainInf.mGravity.x			= mGravity.x;
	MainInf.mGravity.y			= mGravity.y;
	MainInf.mGravity.z			= mGravity.z;
	MainInf.mRestitution		= mRestitution;
	MainInf.mStaticFriction		= mStaticFriction;
	MainInf.mFriction			= mFriction;
	MainInf.mGroundPlane		= mGroundPlane;
	MainInf.mCollisionDetection	= mCollisionDetection;
	MainInf.mTesselation		= mTesselation;
	MainInf.mLightingMode		= mLightingMode;
	GetUnits(MainInf.mUnits);

	// Gather general information - preprocessing
	// Since a new converter is instanced each time, mGlobalInfo has never been used when we reach this point.
	{
		bool Status = mGlobalInfo.PreprocessScene(mRootNode);
		if(!Status)
		{
			mArrayLog.StoreASCII(GetLastIceError()->mErrorText.Get());
			EndExport();
			return false;
		}

//		MainInf.mNbCharacters	= mGlobalInfo.mSceneInfo.mNbCharacters;
//		MainInf.mNbNodes		= mGlobalInfo.mSceneInfo.mNbNodes;
		MainInf.mNbCharacters	= mGlobalInfo.GetSceneInfo()->mNbCharacters;
		MainInf.mNbNodes		= mGlobalInfo.GetSceneInfo()->mNbNodes;
	}

	// TEST
	if(0)
	{
		CharactersDescriptor CD;
		CD.BuildDescriptor(mRootNode);
	}

	// Environment & ambient color
	{
		// Look for environment texture
/*		long EnvTexture = AddTexmap(ip->GetEnvironmentMap());
		if(mFormat==ES_FORMAT_ZCB)
		{
			mGeneral.Store(EnvTexture);
		}
		else if(mFormat==ES_FORMAT_ASCII)
		{
			mGeneral.StoreASCII("Environment texture: ");
			mGeneral.StoreASCII(EnvTexture);
			mGeneral.StoreASCII("\n");
		}
*/
		// Look for background color
		Color BackColor = mIp->GetBackGround(CurrentTime, FOREVER);
		MainInf.mBackColor.r	= BackColor.r;
		MainInf.mBackColor.g	= BackColor.g;
		MainInf.mBackColor.b	= BackColor.b;

//		Control* bgColCont = mIp->GetBackGroundController();
//		if (bgColCont && bgColCont->IsAnimated()) mNbControllers++;

		// Ambient color
		Color AmbColor = mIp->GetAmbient(CurrentTime, FOREVER);
		MainInf.mAmbColor.r	= AmbColor.r;
		MainInf.mAmbColor.g	= AmbColor.g;
		MainInf.mAmbColor.b	= AmbColor.b;

/*		Control *ambLightCont = mIp->GetAmbientController();
		if (ambLightCont && ambLightCont->IsAnimated())
		{
			// Ambient light is animated.	
//			//DumpPoint3Keys(ambLightCont, 0);
		}
		else
		{
			// Ambient light is not animated
		}*/
	}

	// Notify exporter
	if(mExporter && !ExpMotion)	mExporter->SetSceneInfo(MainInf);
	DumpLogString();

//###log message
	// Export character motions
	if(ExpMotion || mSettings->mAlwaysExportMotion)
	{
		for(udword i=0;i<MainInf.mNbCharacters;i++)
		{
			bool Status = AddMotion(i);
			if(!Status)
			{
				mArrayLog.StoreASCII(GetLastIceError()->mErrorText.Get());
				EndExport();
				return false;
			}
		}
	}

	if(!ExpMotion)
	{
		// Main export loop
		mCurrentConvertedNode = 0;		// Dialog-useful member only
		_AddNode(mRootNode);

		// Materials
		ConvertMaterials();
/*
		// End of export -> export the computed lightmaps
		if(!mLightmapPass)
		{
			// Create info structure
			TextureInfo TexInf;
			TexInf.mObjectID	=	mStats.mNbTexmaps;
			TexInf.mName		=	(ubyte*)"Lightmap";

			// Cropping values
//			{
//				Matrix3 UVTrans;
//				GetCropValues(tb.mRGBMap, TexInf.mCValues, UVTrans, CurrentTime);
//				CopyMemory(&TexInf.mTMtx, &UVTrans, sizeof(TextureMatrix));
//			}

			TexInf.mWidth		=	256;
			TexInf.mHeight		=	256;
			TexInf.mPixels		=	new ubyte[TexInf.mWidth * TexInf.mHeight * 4];
			//CopyMemory(TexInf.mPixels, ExportedBitmap.GetPixels(), TexInf.mWidth * TexInf.mHeight * 4);
			TexInf.mHasAlpha	=	false;

			// Perform format-dependent export
			SetExtStatus("calling export plug-in...");
			if(mExporter)	mExporter->ExportTexture(TexInf);

			// Update members
			mStats.mNbTexmaps++;
			if(mStats.mNbTexmaps==IMAX_MAX_TEXTURES)	Error("IMAX_MAX_TEXTURES limit has been reached!\nHint: increase value and recompile.");
		}*/
	}

	// End-of-export notification
	mStats.mNbMaterials	= mNbUsedMaterials;
#ifdef SUPPORT_OLD_LIGHTMAPPER
	if(!mLightmapPass)	
#endif
		SetStatus("Ending export...");
#ifdef SUPPORT_OLD_LIGHTMAPPER
	else				SetStatus("Calling lightmapper...");
#endif
	if(mExporter)	mExporter->EndExport(mStats);
	DumpLogString();

	// 1.18: auto copy textures if needed
	if(mSettings->mAutoCopyTextures)
	{
		String Path;
		IceCore::GetPath(params->mOutputName, Path);

		udword NbCopied=0;
		udword NbNotFound=0;
		udword NbFailed=0;
		CopyTextures(mRootNode, NbCopied, NbNotFound, NbFailed, Path);

		mArrayLog.StoreASCII(_F("%d textures copied (%d not found, %d failed)\n", NbCopied, NbNotFound, NbFailed));
	}

	// Free & restore
	EndExport();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This is the main recursive conversion method.
 *	You should call it once for the root node and let it be...
 *	\param		current_node	[in] a MAX node
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MAXConverter::_AddNode(INode* current_node)
{
	// Update dialog
	{
		String InfoText("exporting node ");
#ifdef SUPPORT_OLD_LIGHTMAPPER
		if(mLightmapPass)
#endif
			InfoText = "lightmapping node ";
		InfoText += (const char*)current_node->GetName();

		int Pos = (mCurrentConvertedNode * 100)/mGlobalInfo.GetSceneInfo()->mNbNodes;

		// Call user plug
		bool DefaultGUI = true;
		if(mExporter)	DefaultGUI = mExporter->Progress(Pos);
		if(mDialog && DefaultGUI)	mDialog->m_Progress.SetPos(Pos);

		SetStatus(InfoText);
		mCurrentConvertedNode++;
	}

	// Check the hidden status
	int Hidden = mSettings->mExportHiddenNodes ? FALSE : current_node->IsHidden();

	// Update log file
	{
		mArrayLog.StoreASCII("\n-----------------------------------\n\nConverting node %s\n", current_node->GetName());
		if(Hidden)	mArrayLog.StoreASCII("(hidden node)\n");
	}

	// Catch the object if it's visible
	Object* NodeObject = Hidden ? null : current_node->GetObjectRef();

	// Catch the user properties
	{
// OLD CODE:
//		GetUserProperties(current_node, mProps);

		// Reset all user-controlled values 
		mProps.Reset();

		// Catch this node's user properties
		CStr Buffer;
		current_node->GetUserPropBuffer(Buffer);

		// Parse them and setup relevant flags
		mParseSceneInfos = false;
		Parse(Buffer.data());
	}

	// Export if needed
	if(NodeObject && !mProps.mDiscardExport && (mProps.mForceExport || !mSelectedNodes || mSelectedNodes->Contains(current_node)))
	{
		// Setup various flags
		{
			// Get final UVs settings for this node
			mExportUVW = mSettings->mExpUVW;			// Global settings
				if(mProps.mExportUVW)	mExportUVW = true;	// Override
				if(mProps.mDiscardUVW)	mExportUVW = false;	// Override

			// Get final vertex-colors settings for this node
			mExportColors = mSettings->mExpVTXColor;				// Global settings
				if(mProps.mExportVtxColors)		mExportColors = true;	// Override
				if(mProps.mDiscardVtxColors)	mExportColors = false;	// Override

			// Get final parity
			mParity	= GetParity(current_node, mTimeInfo.mCurrentTime);
			if(mSettings->mConvertToD3D)	mParity = !mParity;

			// User-defined frame conversion
			if(mExporter)	mExporter->Parity(mParity);
		}

		switch(NodeObject->SuperClassID())
		{
			case GEOMOBJECT_CLASS_ID :
				GetScalingFrom(current_node);
				AddGeomObject(current_node);
/*
current_node->SetUserPropString("StringKey", "StringValue");
current_node->SetUserPropInt("IntKey", 42);
current_node->SetUserPropFloat("FloatKey", 42.0f);
current_node->SetUserPropBool("BoolKey", TRUE);
/*		virtual void SetUserPropString(const TSTR &key,const TSTR &string)=0;
		virtual void SetUserPropInt(const TSTR &key,int val)=0;
		virtual void SetUserPropFloat(const TSTR &key,float val)=0;
		virtual void SetUserPropBool(const TSTR &key,BOOL b)=0;
*/
				break;

			case GEN_DERIVOB_CLASS_ID:
				GetScalingFrom(current_node);
				AddDerivedObject(current_node);
				break;

			case CAMERA_CLASS_ID :
				GetScalingFrom(current_node);
				AddCamera(current_node);
				break;

			case LIGHT_CLASS_ID:
				GetScalingFrom(current_node);
				AddLight(current_node);
				break;

			case SHAPE_CLASS_ID:
				GetScalingFrom(current_node);
				AddShape(current_node);
				break;

			case HELPER_CLASS_ID:
				GetScalingFrom(current_node);
				AddHelper(current_node);
				break;

			case WSM_CLASS_ID:
				GetScalingFrom(current_node);
//				AddGravity(current_node);
				break;

			case WSM_OBJECT_CLASS_ID:
				GetScalingFrom(current_node);
				AddSpaceWarp(current_node);
				break;

			default:
				mArrayLog.StoreASCII("(unknown node)\n");
				mStats.mNbUnknowns++;
				break;
		}
	}
	else
	{
		// Only the root node should have no object.
		mStats.mNbInvalidNodes++;
	}

	// Debug dump
	{
		if(mProps.mDebugDumpSubanims)
		{
			DumpSubAnimsNames(current_node);
		}
	}

	// Go down deep into the pain: recurse through children..
	for(long i=0;i<current_node->NumberOfChildren();i++)	_AddNode(current_node->GetChildNode(i));
}
