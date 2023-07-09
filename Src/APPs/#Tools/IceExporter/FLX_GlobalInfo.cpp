///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains code to preprocess the scene and create a global info structure.
 *	\file		FLX_GlobalInfo.cpp
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
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GlobalInfo::GlobalInfo() : mNbNodes(0), mNodeInfo(null), mCharInfo(null)
{
	mTexmInfo	= new TMapInfo[IMAX_MAX_TEXTURES];	ASSERT(mTexmInfo);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GlobalInfo::~GlobalInfo()
{
	DELETEARRAY(mNodeInfo);
	DELETEARRAY(mCharInfo);
	DELETEARRAY(mTexmInfo);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	A method to preprocess the whole scene and get some useful information.
 *	\param		root_node	[in] MAX root node
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::PreprocessScene(INode* root_node)
{
	// 1) Init: Get number of nodes in the whole MAX scene (including root)
	mNbNodes = GetNbNodes(root_node);

	// Get some bytes
	mNodeInfo = new NodeInfo[mNbNodes];
	CHECKALLOC(mNodeInfo);

	// 2) Create basic scene info [updates mNodeInfo & mSceneInfo]
	FlushErrors();
	Container Invalid;
	_CreateSceneInfo(root_node, Invalid);
	if(Invalid.GetNbEntries())
	{
		CustomArray Msg;
		Msg.StoreASCII("The scene contains invalid materials:\n");
		for(udword i=0;i<Invalid.GetNbEntries();i++)
		{
			Mtl* InvMat = (Mtl*)Invalid.GetEntry(i);
			char* MatName = InvMat->GetName();
			Msg
				.StoreASCII("- ")
				.StoreASCII(MatName ? MatName : "(unnamed)")
				.StoreASCII("\n");
		}
		Msg.StoreASCII("Do you want Flexporter to export the scene anyway,\n");
		Msg.StoreASCII("using a default standard material instead?");
		Msg.Store(ubyte(0));
		if(::MessageBox(null, (const char*)Msg.Collapse(), "Found invalid materials", MB_YESNO)==IDNO)	return false;
	}

	// 3) Complete preprocessing for characters.
	// Here mSceneInfo.mNbCharacters has been updated with the number of characters in the scene.
	if(mSceneInfo.mNbCharacters)
	{
		// Get one entry/character
		mCharInfo = new CharInfo[mSceneInfo.mNbCharacters];
		CHECKALLOC(mCharInfo);

		// Reset mNbCharacters
		mSceneInfo.mNbCharacters = 0;

		// Init character-specific info
		if(!_FindCharactersRoots(root_node))	return false;

		// Find virtual bones
//		if(!_FindVirtualBones(rootnode))	return false;

		// Alloc skeleton
//		udword Total = GetCSTotalNbNodes();
		udword Total = GetCSTotalNbNodes();
		for(udword i=0;i<mSceneInfo.mNbCharacters;i++)
		{
			mCharInfo[i].mSkeleton = new BoneInfo[Total];
			CHECKALLOC(mCharInfo[i].mSkeleton);
		}

		// ### ...
		if(!_CompleteCharactersInfo(root_node))	return false;
	}

	// 4) Rebuild groups

	// 4-1) We need parent pointers
	FixParentPointers();

	// 4-2) Loop through all enumerated nodes
	for(udword i=0;i<mNbNodes;i++)
	{
		// For each node, find its possible group head (that is, if it belongs to a group)
		NodeInfo* GroupHead = GetGroupHead(&mNodeInfo[i]);
		// Found a group head ? Then add the node to the group head's list
		if(GroupHead)	GroupHead->AddToGroup(i);
	}

	return true;
}

void GlobalInfo::FixParentPointers()
{
	// Loop through all enumerated nodes
	for(udword i=0;i<mNbNodes;i++)
	{
		// Catch current MAX node
		INode* CurrentNode = mNodeInfo[i].mMAXNode;
		// Catch current MAX parent
		INode* ParentNode = CurrentNode->GetParentNode();
		if(ParentNode)
		{
			// Look for the parent in our database
			udword ID = GetNodeID(ParentNode);
			// Fix parent pointer if needed
			if(ID!=INVALID_ID)	mNodeInfo[i].mParent = &mNodeInfo[ID];
		}
	}
}

// Finds group head for given object
NodeInfo* GlobalInfo::GetGroupHead(NodeInfo* object) const
{
	if(!object || !object->mGroupMember)	return null;

	// Jump to parent
	object = object->mParent;

	while(object)
	{
		// Are we a helper ?
//		NovodexHelper* Helper = DYNAMIC_CAST(object, NovodexHelper);

		// Helper and group head ? Found it!
//		if(Helper && Helper->IsGroupHead())	return Helper;
		if(object->mGroupHead)	return object;

		// Jump to parent
		object = object->mParent;
	}
	return null;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Collects some information for all nodes in a preprocess loop. This is used to resolve some interdependencies between nodes.
 *	This is also useful for instances. The method also checks all materials are valid.
 *
 *	After the call, you get scene-related info in mSceneInfo and node-related info in mNodeInfo.
 *
 *	\param		current_node	[in] MAX root node
 *	\param		invalid			[out] list of invalid materials
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::_CreateSceneInfo(INode* current_node, Container& invalid)
{
	// Checkings
	if(!current_node)	return false;

	// Check current node's material and get back possibly invalid ones
	Mtl* CurMat = current_node->GetMtl();
	_IsValidMaterial(CurMat, &invalid);

/*	Container Invalid;
	Mtl* CurMat = currentnode->GetMtl();
	if(!_IsValidMaterial(CurMat, &Invalid))
	{
		CustomArray Msg;
		Msg.StoreASCII("The scene contains invalid materials:\n");
		for(udword i=0;i<Invalid.GetNbEntries();i++)
		{
			Mtl* InvMat = (Mtl*)Invalid.GetEntry(i);
			char* MatName = InvMat->GetName();
			Msg
				.StoreASCII("- ")
				.StoreASCII(MatName ? MatName : "(unnamed)")
				.StoreASCII("\n");
		}
		SetIceError((const char*)Msg.Collapse());
//		char* MatName = CurMat->GetName();
//		SetIceError((const char*)LogText("Material %s is invalid", MatName ? MatName : "(unnamed)"));
//		return false;
	}
*/
	// Update mNodeInfo
	mNodeInfo[mSceneInfo.mNbNodes].mMAXNode		= current_node;
	mNodeInfo[mSceneInfo.mNbNodes].mObject		= current_node->GetObjectRef();

	// Group stuff
	{
		// For all nodes
		mNodeInfo[mSceneInfo.mNbNodes].mGroupMember	= current_node->IsGroupMember()!=0;

		// For helpers
		if(mNodeInfo[mSceneInfo.mNbNodes].mObject && mNodeInfo[mSceneInfo.mNbNodes].mObject->SuperClassID()==HELPER_CLASS_ID)
		{
			mNodeInfo[mSceneInfo.mNbNodes].mGroupHead	= current_node->IsGroupHead()!=0;
		}
	}

	// Update mSceneInfo
	mSceneInfo.mNbNodes++;
	BoneType ID = IsSkeleton(current_node);
	// The number of characters in the scene is the number of BIPED_MAIN elements.
	if(ID==BONE_BIPED_MAIN)	mSceneInfo.mNbCharacters++;

	// If this is a Biped part, compute its length. It will get exported as the bone's length, and used in IK.
//	if(ID

	// Recurse
	for(long i=0;i<current_node->NumberOfChildren();i++)
		if(!_CreateSceneInfo(current_node->GetChildNode(i), invalid))	return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Loops through the scene, looks for characters (root bones only), update the characters info structure.
 *	After the call, mCharInfo is updated, mSceneInfo.mNbCharacters is updated [clear it before the call]
 *	\param		current_node	[in] MAX root node
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::_FindCharactersRoots(INode* current_node)
{
	// Checkings
	if(!current_node)	return false;

	// Is current node a biped part?
	BoneType ID = IsSkeleton(current_node);
	// Look for character's root bone
	if(ID==BONE_BIPED_MAIN)
	{
		// Get total possible CS bones
//		udword Total = mCSExp->GetCSTotalNbNodes();

		// Clear info structure for new character
//		mCharInfo[mSceneInfo.NbCharacters].NbNodes			= 0;
//		mCharInfo[mSceneInfo.NbCharacters].NbVirtualBones	= 0;
//		mCharInfo[mSceneInfo.NbCharacters].IrionSkeleton	= null;
//		mCharInfo[mSceneInfo.NbCharacters].Skeleton			= new MAX_Bone[Total];
//		if(!mCharInfo[mSceneInfo.NbCharacters].Skeleton) return false;
		mCharInfo[mSceneInfo.mNbCharacters++].mRootNode		= current_node;
	}

	// Recurse
	for(long i=0;i<current_node->NumberOfChildren();i++)
	{
		bool Status = _FindCharactersRoots(current_node->GetChildNode(i));
		if(!Status) return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input	:	currentnode		=	MAX root node
// Output	:	
// Return	:	true if success
// Remark	:	recursive method
/*
bool GlobalInfo::_FindVirtualBones(INode* currentnode)
{
	// Checkings
	if(!currentnode)	return false;

	// Is current node a biped part?
//	char ID = IsSkeleton(currentnode);
	char ID = IsSkeleton(currentnode);
	// Look for virtual bones
	if(ID==BIPED_VIRTUAL)
	{
		// Get owner character
		udword CharID = GetCharacterID(currentnode);
		// Update skeleton
		mCharInfo[CharID].mNbVirtualBones++;

		/////// ### ONLY WORKS WITH 1 CHAR
		RegisterVirtualBone(currentnode, currentnode->GetName());
	}

	// Recurse
	for(long i=0;i<currentnode->NumberOfChildren();i++)
	{
		bool Status = _FindVirtualBones(currentnode->GetChildNode(i));
		if(!Status) return false;
	}
	return true;
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A method to complete the character related structure once they have been initialized.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input	:	currentnode		=	MAX root node
// Output	:	mCharInfo completed
// Return	:	true if success
// Remark	:	recursive method
bool GlobalInfo::_CompleteCharactersInfo(INode* current_node)
{
	// Checkings
	if(!current_node)	return false;

	// Look for biped parts which aren't dummies, include them in MAX skeleton
	BoneType ID = IsSkeleton(current_node);
	if(ID && ID!=BONE_BIPED_DUMMY)
	{
		// Get owner character
		udword CharID = GetCharacterID(current_node);
		// Get bone's ID
		udword CSID = GetCSID(current_node);
//		if(CSID==udword(-1)) return false;

		if(CharID!=udword(-1))
		{
			mCharInfo[CharID].mNbNodes++;
			// Update skeleton
			mCharInfo[CharID].mSkeleton[CSID].mBone			= current_node;
			mCharInfo[CharID].mSkeleton[CSID].mFatherBone	= current_node->GetParentNode();
			mCharInfo[CharID].mSkeleton[CSID].mCSID			= GetCSID(current_node);
			mCharInfo[CharID].mSkeleton[CSID].mpCSID		= GetCSID(current_node->GetParentNode());
		}
	}

	// Recurse
	for(long i=0;i<current_node->NumberOfChildren();i++)
	{
		bool Status = _CompleteCharactersInfo(current_node->GetChildNode(i));
		if(!Status) return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Transforms a MAX node pointer into a valid ID. ID is just the node's position in the array.
 *	\param		current_node	[in] a MAX node
 *	\return		the node's ID, or INVALID_ID
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword GlobalInfo::GetNodeID(INode* current_node)	const
{
	// Checkings
	if(!current_node)	return INVALID_ID;

	// Loop through nodes
	for(udword i=0;i<mSceneInfo.mNbNodes;i++)
	{
		if(mNodeInfo[i].mMAXNode==current_node) return i;
	}
	// Not found in the list? Node is invalid.
	return INVALID_ID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Transforms a MAX object pointer into a valid ID. ID is just the node's position in the array. This is useful to find object's instances.
 *	\param		object	[in] a MAX object
 *	\param		master	[in] true if we want the master object, else return first one we find
 *	\return		the object's ID, or INVALID_ID
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword GlobalInfo::GetNodeID(Object* object, bool master)	const
{
	// Checkings
	if(!object)	return INVALID_ID;

	// Loop through nodes
	for(udword i=0;i<mSceneInfo.mNbNodes;i++)
	{
		if(mNodeInfo[i].mObject==object)
		{
			if(master)
			{
				// We want to keep that one only if it has already been converted
				if(mNodeInfo[i].mExportedNode)	return i;
			}
			else return i;	// Return first one we find
		}
	}
	// Not found in the list? Object is invalid.
	return INVALID_ID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks whether an object is an instance or not.
 *	\param		node_object	[in] the MAX node's object
 *	\param		master		[out] the object's ID
 *	\return		true if the object has already been converted
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GlobalInfo::IsInstance(Object* node_object, sdword& master)	const
{
	master = GetNodeID(node_object, true);	// If we call it with node_object we'll always get the first instance found

	return master!=INVALID_ID;

/*	// Check instance state
	// Look for possible master
	master = GetNodeID(node_object);	// If we call it with node_object we'll always get the first instance found

	if(master!=INVALID_ID)
	{
		if(mNodeInfo[master].mAlreadyConverted)
		{
			// Oh! We already had to face this one...
			// Let's create an instance.
			return true;
		}
		// Here we have to deal for the first time with this node_object.
		// Thus, we're about to convert it. Then we mark it as already converted in order to see it as an instance next time.
		mNodeInfo[master].mAlreadyConverted = true;
	}
	return false;*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	The descriptor factory.
 *	\param		node	[in] a MAX node
 *	\param		type	[in] the requested object's type
 *	\return		the newly created descriptor
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ResourceDescriptor* GlobalInfo::CreateDescriptor(INode* node, ObjType type)
{
	// Checkings
	if(!mNodeInfo)						return null;	// Must preprocess scene first

	udword NodeID = GetNodeID(node);					// Look for the input node
	if(NodeID==INVALID_ID)				return null;	// Input node is invalid

	if(mNodeInfo[NodeID].mExportedNode)	return null;	// Input node has already been processed

	ResourceDescriptor* NewObj = null;

	switch(type)
	{
		case FLX_OBJ_CAMERA:		NewObj = new CameraDescriptor;			break;
		case FLX_OBJ_LIGHT:			NewObj = new LightDescriptor;			break;
		case FLX_OBJ_MESH:			NewObj = new MeshDescriptor;			break;
		case FLX_OBJ_BPATCH:		/*NewObj = new Info;*/					break;
//		case FLX_OBJ_CONTROLLER:	NewObj = new ControllerDescriptor;		break;
		case FLX_OBJ_MATERIAL:		NewObj = new MaterialDescriptor;		break;
		case FLX_OBJ_TEXTURE:		NewObj = new TextureDescriptor;			break;
		case FLX_OBJ_MOTION:		NewObj = new MotionDescriptor;			break;
		case FLX_OBJ_SHAPE:			NewObj = new ShapeDescriptor;			break;
		case FLX_OBJ_GRAVITY:		NewObj = new GravityObject;				break;
		case FLX_OBJ_WIND:			NewObj = new WindObject;				break;

		case FLX_OBJ_BOXGIZMO:		NewObj = new BoxGizmoDescriptor;		break;
		case FLX_OBJ_SPHEREGIZMO:	NewObj = new SphereGizmoDescriptor;		break;
		case FLX_OBJ_CYLINDERGIZMO:	NewObj = new CylinderGizmoDescriptor;	break;
		case FLX_OBJ_BILLBOARD:		NewObj = new BillboardDescriptor;		break;

		// Special case for helpers
		case FLX_OBJ_HELPER:
		{
			NewObj = new HelperDescriptor;

			if(mNodeInfo[NodeID].mGroupHead)
			{
				// Copy grouped IDs so that users don't have to recreate this
				// Ok, I admit the way it's done looks a bit ugly.
				((HelperDescriptor*)NewObj)->mGroupedObjects = new Container;
				*((HelperDescriptor*)NewObj)->mGroupedObjects = mNodeInfo[NodeID].mGrouped;
			}
			break;
		}
	}

	// The descriptor we just created will be used to export current node. We keep track of
	// that descriptor here, to support instances later.
	mNodeInfo[NodeID].mExportedNode = NewObj;

	return NewObj;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets a character ID from a BIPED part.
 *	\param		node	[in] a BIPED part
 *	\return		the character's ID or INVALID_ID if not found
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword GlobalInfo::GetCharacterID(INode* node)	const
{
	// Checkings
	if(!node)	return INVALID_ID;

	// Catch Character Studio root node
	INode* Root = GetCSRootNode(node);
	if(!Root)	return INVALID_ID;

	// Loop through characters in search of that one
	for(udword i=0;i<mSceneInfo.mNbCharacters;i++)
	{
		if(mCharInfo[i].mRootNode==Root)	return i;
	}

	// Not found!
	return INVALID_ID;
}
