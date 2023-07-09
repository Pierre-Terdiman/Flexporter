///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains Character Studio related code.
 *	\file		FLX_CharacterStudio.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

/*
  Character Studio & Skin notes:
  - A standard Character Studio mesh is made of a CSID-compliant skeleton and a Physique skin.
  - A complex character can be made of multiple Physique skins built on a single skeleton
  - A skeleton can be non CSID-compliant, in which case motion blending is more difficult
  - Skin modifier doesn't have offset vectors
  - Number of characters = number of skeletons != number of "skins"
  - A "skin" is a mesh with a Physique or a Skin modifier
  - A skeleton is a list of bones referenced by at least one skin, and possibly many of them
  - A bone is a MAX node, either a BIPED part, a MAX bone or a normal mesh
  - BIPED parts only are CSID-compliant.
  - CSID-compliance allows one skeleton to be handled via a normalized wordlist, which means CSIDs are the same accross motions. Hence
	it's good for motion blending
  -	When CSIDs are not available, we must use virtual CSIDS or the bones' names. Virtual CSIDs depend on the actual scene organization,
	which is very bad. Names are better, but one should ensure two bones don't have the same name!
*/

/*
	WARNING: PARTS OF THIS CODE MAY NOT BE USED YET
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Finds Character Studio's Physique. This code comes from the Character Studio SDK.
 *	\fn			FindPhysiqueModifier(INode* node)
 *	\param		node	[in] a MAX node
 *	\return		Physique modifier or null
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Modifier* IceExporter::FindPhysiqueModifier(INode* node)
{
	// Checkings
	if(!node)	return null;

	// Get object from node. Abort if no object.
	Object* ObjectPtr = node->GetObjectRef();
	if(!ObjectPtr) return null;

	// Is derived object?
	while(ObjectPtr->SuperClassID()==GEN_DERIVOB_CLASS_ID)
	{
		// Yes => cast
		IDerivedObject* DOP = (IDerivedObject*)ObjectPtr;

		// Iterate over all entries of the modifier stack
		int ModStackIndex = 0;
		while(ModStackIndex<DOP->NumModifiers())
		{
			// Get current modifier
			Modifier* Mody = DOP->GetModifier(ModStackIndex++);

			// Look for "Physique"
			if(Mody->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
			{
				// Yes => exit
				return Mody;
			}
		}
		ObjectPtr = DOP->GetObjRef();
	}
	// Not found
	return null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Finds the Skin modifier.
 *	\fn			FindSkinModifier(INode* node)
 *	\param		node	[in] a MAX node
 *	\return		Skin modifier or null
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Modifier* IceExporter::FindSkinModifier(INode* node)
{
	// Checkings
	if(!node)	return null;

	// Get object from node. Abort if no object.
	Object* ObjectPtr = node->GetObjectRef();
	if(!ObjectPtr) return null;

	// Is derived object?
	while(ObjectPtr->SuperClassID()==GEN_DERIVOB_CLASS_ID)
	{
		// Yes => cast
		IDerivedObject* DOP = (IDerivedObject*)ObjectPtr;

		// Iterate over all entries of the modifier stack
		int ModStackIndex = 0;
		while(ModStackIndex<DOP->NumModifiers())
		{
			// Get current modifier
			Modifier* Mody = DOP->GetModifier(ModStackIndex++);

			// Look for Skin
			if(Mody->ClassID() == SKIN_CLASSID)
			{
				// Yes => exit
				return Mody;
			}
		}
		ObjectPtr = DOP->GetObjRef();
	}
	// Not found
	return null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Fixes a modified CS name and transforms it into a default CS name.
 *	\fn			FixCSNodeName(const char* originalname)
 *	\param		originalname	[in] the Character Studio biped-part's name
 *	\return		the Character Studio biped-part's default name
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* IceExporter::FixCSNodeName(const char* originalname)
{
	// Checkings
	if(!originalname)	return null;

	static char FixedName[1024];

	strcpy((char*)FixedName, "Bip01");

	char* Space = (char*)strchr((const char*)originalname, ' ');
	if(Space)	strcat((char*)FixedName, (const char*)Space);
	return FixedName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets the root-node of any Character Studio biped parts.
 *	It uses IsSkeleton() which handles user-defined virtual bones, so it always works.
 *	\fn			GetCSRootNode(INode* currentnode)
 *	\param		currentnode		[in] biped node
 *	\return		biped root node or null if not found
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
INode* IceExporter::GetCSRootNode(INode* currentnode)
{
	while(currentnode)
	{
		if(IsSkeleton(currentnode)==BONE_BIPED_MAIN)	return currentnode;
		currentnode = currentnode->GetParentNode();
	}
	return null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks whether a node is a biped part or not.
 *	It also handles user-defined virtual bones.
 *	\fn			IsSkeleton(INode* node)
 *	\param		node		[in] a MAX node
 *	\return		BoneType enumeration code
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BoneType IceExporter::IsSkeleton(INode* node)
{
	// Checkings
	if(!node)	return BONE_NONE;

	bool IsDummy = false;
	Control* TMController = node->GetTMController();

	// Check BIPED standard parts
	if(TMController)
	{
		Object* NodeObject = node->GetObjectRef();
		if(NodeObject && NodeObject->SuperClassID()==HELPER_CLASS_ID)	IsDummy = true;

		// Test whether or not this is a biped controller
		if(IsDummy)
		{
					if(TMController->ClassID()==BIPSLAVE_CONTROL_CLASS_ID)	return BONE_BIPED_DUMMY;
			else	if(TMController->ClassID()==BIPBODY_CONTROL_CLASS_ID)	return BONE_BIPED_DUMMY;
			else	if(TMController->ClassID()==FOOTPRINT_CLASS_ID)			return BONE_BIPED_DUMMY;
		}
		else
		{
					if(TMController->ClassID()==BIPSLAVE_CONTROL_CLASS_ID)	return BONE_BIPED_SLAVE;
			else	if(TMController->ClassID()==BIPBODY_CONTROL_CLASS_ID)	return BONE_BIPED_MAIN;
			else	if(TMController->ClassID()==FOOTPRINT_CLASS_ID)			return BONE_BIPED_FOOT;
		}
	}

#ifdef BONE_OBJ_CLASSID
	// Check native MAX bones
	ObjectState os = node->EvalWorldState(0);
	//if(os.obj && os.obj->ClassID() == Class_ID(BONE_CLASS_ID, 0))	return BONE_NATIVE;
	if(os.obj && os.obj->ClassID()==BONE_OBJ_CLASSID)	return BONE_NATIVE;
#endif
/*
	// Check virtual bones
	UserProps UP;
	GetUserProperties(node, UP);
//	if(UP.mBone)		return BIPED_VIRTUAL;	// Found a virtual bone
	if(UP.mMainBone)	return BIPED_MAIN;		// Found a virtual main bone
*/
	// Not a bone
	return BONE_NONE;	// BONE_NONE is null.
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Checks whether a node is a skin or not.
 *	\fn			IsSkin(INode* node)
 *	\param		node	[in] a MAX node
 *	\return		SkinType enumeration code
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SkinType IceExporter::IsSkin(INode* node)
{
	// Checkings
	if(!node)	return SKIN_NONE;

	// Catch the object
	Object* NodeObject = node->GetObjectRef();

	// Check skin status
	if(NodeObject && NodeObject->SuperClassID()==GEN_DERIVOB_CLASS_ID)
	{
		// Found a derived object => can be a skin
		if(FindPhysiqueModifier(node))	return SKIN_PHYSIQUE;
		if(FindSkinModifier(node))		return SKIN_NATIVE;
	}

	// Not a skin
	return SKIN_NONE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Gets the total number of Character Studio's available node-names.
 *	\fn			GetCSTotalNbNodes()
 *	\return		total number of Character Studio's available node-names.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static udword gCSTotalNbNodes = INVALID_ID;
udword IceExporter::GetCSTotalNbNodes()
{
	if(gCSTotalNbNodes==INVALID_ID)
	{
		char NodeName[256];
		char *List = CharacterStudioNames;
		char *EndOfList = List + strlen((const char*)CharacterStudioNames);
		udword i;

		udword BoneID = 0;

		while(List!=EndOfList)
		{
			i=0;
			while(*List!='\n')
			{
				NodeName[i++] = *List++;
			}
			NodeName[i] = '\0';
			List++;

			BoneID++;
		}
		gCSTotalNbNodes = BoneID;
	}
	return gCSTotalNbNodes;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Computes a bone's CSID.
 *	\fn			GetCSID(INode* node)
 *	\param		node	[in] a MAX node
 *	\return		a CSID or INVALID_ID if unknown
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword IceExporter::GetCSID(INode* node)
{
	// Checkings
	if(!node)	return INVALID_ID;

	// Catch node's name
	const char* Name = node->GetName();
	if(!Name)	return INVALID_ID;

	// Look into the real bones' list
	char FixedName[1024];
	char NodeName[256];
	char* List = CharacterStudioNames;
	char* EndOfList = List + strlen((const char*)CharacterStudioNames);
	udword BoneID=0, i;

	strcpy((char*)FixedName, FixCSNodeName(Name));

	while(List!=EndOfList)
	{
		i=0;
		while(*List!='\n')
		{
			NodeName[i++] = *List++;
		}
		NodeName[i] = '\0';
		List++;

		if(!strcmp((const char*)FixedName, (const char*)NodeName))	return BoneID;

		BoneID++;
	}
	return INVALID_ID;
}



bool PhysiqueData::CreatePhysiqueData(INode* node)
{
	// Look for Physique modifier
	Modifier* phyMod = FindPhysiqueModifier(node);
	if(!phyMod) return false;

	// Create a Physique Export Interface for the given Physique Modifier
	IPhysiqueExport* phyExport = (IPhysiqueExport*)phyMod->GetInterface(I_PHYINTERFACE);	// I_PHYEXPORT in CS3
	if(!phyExport) return false;

	// Get the physique version number.  
	// If the version number is > 30 you may have floating bones
	int CSVersion = phyExport->Version();	// Should be at least 22

	// Create a ModContext Export Interface for the specific node of the Physique Modifier
	IPhyContextExport* mcExport = (IPhyContextExport*)phyExport->GetContextInterface(node);
	if(!mcExport) return false;

	// We convert all vertices to Rigid. (### Is it possible to do something else?)
	mcExport->ConvertToRigid(TRUE);

	// Allow blending to export multi-link assignments (### safe ?)
	mcExport->AllowBlending(TRUE);

	// Build the skin
	mNbVerts = mcExport->GetNumberVertices();
	mUseMultipleBones = false;

	// Loop through vertices
	for(udword i=0;i<mNbVerts;i++)
	{
		// Get current vertex interface
		IPhyVertexExport* vtxExport = mcExport->GetVertexInterface(i);
		if(vtxExport)
		{
			// Check if vertex has blending
			if(vtxExport->GetVertexType() & BLENDED_TYPE)
			{
				// Here, each vertex is linked to multiple bones.
				IPhyBlendedRigidVertex* vtxBlend = (IPhyBlendedRigidVertex*)vtxExport;

				// Get number of driving bones for current vertex
				udword NbBones = vtxBlend->GetNumberNodes();

				// Keep track of this number
				mBonesNb.Add(NbBones);

				// Loop through driving bones
				for(udword n=0;n<NbBones;n++)
				{
					// Get current bone, offset vector and weight
					Point3 OV = vtxBlend->GetOffsetVector(n);
					mOffsetVectors.AddVertex(Point(OV.x, OV.y, OV.z));
					mWeights.Add(vtxBlend->GetWeight(n));
					mBones.AddNode(vtxBlend->GetNode(n));
				}
				// Mark multiple bones
				mUseMultipleBones = true;
			}
			else
			{
				// Here, each vertex is linked to a single bone.
				IPhyRigidVertex* vtxNoBlend = (IPhyRigidVertex*)vtxExport;

				// Update arrays anyway, so that it works for skins mixing BLENDED_TYPE & non blended ones. (à la Amaury)
				mBonesNb.Add(udword(1));

				// Get current bone, offset vector and weight
				Point3 OV = vtxNoBlend->GetOffsetVector();
				mOffsetVectors.AddVertex(Point(OV.x, OV.y, OV.z));
				mBones.AddNode(vtxNoBlend->GetNode());
				mWeights.Add(1.0f);
			}
			// Release vertex interface
			mcExport->ReleaseVertexInterface(vtxExport);
		}
	}

	// Release interfaces
	phyExport->ReleaseContextInterface(mcExport);
	phyMod->ReleaseInterface(I_PHYINTERFACE, phyExport);
	return true;
}

udword PhysiqueData::CreateSkeleton(MAXNodes& bones)
{
	// Create an optimized list of bones == a signature
	RadixSort RS;
	udword NbOriginalBones = mBones.GetNbNodes();
	INode** OriginalBones = mBones.GetNodes();
	const udword* Sorted = RS.Sort((udword*)OriginalBones, NbOriginalBones, RADIX_UNSIGNED).GetRanks();

	INode* PreviousBone=null;
	for(udword i=0;i<NbOriginalBones;i++)
	{
		udword SortedIndex = Sorted[i];
		INode* SortedBone = OriginalBones[SortedIndex];
		if(SortedBone!=PreviousBone)
		{
			bones.AddNode(SortedBone);
			PreviousBone = SortedBone;
		}
	}
	return bones.GetNbNodes();
}

bool SkinData::CreateSkinData(INode* node)
{
	// Look for Skin modifier
	Modifier* SkinMod = FindSkinModifier(node);
	if(!SkinMod) return false;

	// Catch the Skin interface
	ISkin* pSkin = (ISkin*)SkinMod->GetInterface(I_SKIN);
	if(!pSkin)	return false;

	// Catch the context interface
	ISkinContextData* pSkinContext = pSkin->GetContextInterface(node);
	if(!pSkinContext)	return false;

	// Mark multiple bones
	mUseMultipleBones = true;

	// Build the skin
	mNbVerts = pSkinContext->GetNumPoints();

	// Loop through vertices
	for(udword i=0;i<mNbVerts;i++)
	{
		// Get number of driving bones for current vertex
		udword NbBones = pSkinContext->GetNumAssignedBones(i);

		// Loop through driving bones
		udword RealNbBones = 0;
		for(udword j=0;j<NbBones;j++)
		{
			// Get assigned bone
			int AssignedBone = pSkinContext->GetAssignedBone(i, j);
			if(AssignedBone<0)	continue;
			RealNbBones++;

			// Get current bone and weight
			INode* pBone = pSkin->GetBone(AssignedBone);
			float Weight = pSkinContext->GetBoneWeight(i, j);

			mWeights.Add(Weight);
			mBones.AddNode(pBone);
		}
		// Keep track of this number
		mBonesNb.Add(RealNbBones);
	}

	// Release interfaces
	SkinMod->ReleaseInterface(I_SKIN, pSkin);

	return true;
}






BonesContainer::BonesContainer()
{
}

BonesContainer::~BonesContainer()
{
	for(udword i=0;i<GetNbEntries();i++)
	{
		BoneDescriptor* BD = (BoneDescriptor*)GetEntry(i);
		DELETESINGLE(BD);
	}
}

bool BonesContainer::AddBone(INode* currentnode)
{
	// Check the bone doesn't already belong to our list
	if(Contains(udword(currentnode)))	return true;

	// Is current node a bone?
	BoneType BT = IsSkeleton(currentnode);
//	if(BT!=BONE_NONE)

	if(BT==BONE_NONE)	BT=BONE_MESH;

	{
		BoneDescriptor* BD = new BoneDescriptor;
		CHECKALLOC(BD);
		BD->mNode				= currentnode;
		BD->mBone.mType			= BT;
		BD->mBone.mCSID			= GetCSID(currentnode);
		BD->mBone.mParentCSID	= GetCSID(currentnode->GetParentNode());
		BD->mBone.mName			= currentnode->GetName();
		BD->mBone.mParentName	= currentnode->GetParentNode() ? currentnode->GetParentNode()->GetName() : null;
		Add(udword(BD));
		return true;
	}
	return false;
}

bool BonesContainer::_FindStandardBones(INode* currentnode)
{
	// Checkings
	if(!currentnode)	return false;

	AddBone(currentnode);

	// Recurse
	for(long i=0;i<currentnode->NumberOfChildren();i++)
		if(!_FindStandardBones(currentnode->GetChildNode(i)))	return false;

	return true;
}

bool BonesContainer::Init(INode* rootnode)
{
	_FindStandardBones(rootnode);

	return true;
}

bool BonesContainer::BuildBonesInfo()
{
	// Complete bones info (assign parents, etc)
	// Loop through bones
	for(udword i=0;i<GetNbEntries();i++)
	{
		// Get current bone
		BoneDescriptor* BD = (BoneDescriptor*)GetEntry(i);
		// Get MAX parent
		INode* MAXParent = BD->mNode->GetParentNode();
		if(MAXParent)
		{
			// Look for parent bone in our list
			for(udword j=0;j<GetNbEntries();j++)
			{
				BoneDescriptor* BD2 = (BoneDescriptor*)GetEntry(j);
				if(BD2->mNode==MAXParent)
				{
					BD->mParent = BD2;
				}
			}
		}
	}
	return true;
}



SkinsContainer::SkinsContainer()
{
}

SkinsContainer::~SkinsContainer()
{
	for(udword i=0;i<GetNbEntries();i++)
	{
		SkinDescriptor* SD = (SkinDescriptor*)GetEntry(i);
		DELETESINGLE(SD);
	}
}

bool SkinsContainer::AddSkin(INode* currentnode)
{
	// Check the skin doesn't already belong to our list
	if(Contains(udword(currentnode)))	return true;

	// Is current node a skin?
	SkinType ST = IsSkin(currentnode);
	if(ST!=SKIN_NONE)
	{
		SkinDescriptor* SD = new SkinDescriptor;
		CHECKALLOC(SD);
		SD->mNode	= currentnode;
		SD->mType	= ST;
		Add(udword(SD));
		return true;
	}
	return false;
}

bool SkinsContainer::_FindStandardSkins(INode* currentnode)
{
	// Checkings
	if(!currentnode)	return false;

	AddSkin(currentnode);

	// Recurse
	for(long i=0;i<currentnode->NumberOfChildren();i++)
		if(!_FindStandardSkins(currentnode->GetChildNode(i)))	return false;

	return true;
}

bool SkinsContainer::Init(INode* rootnode)
{
	_FindStandardSkins(rootnode);

	return true;
}


bool SkinsContainer::BuildSkinsInfo()
{
	// Loop through skins and create Physique data
	for(udword i=0;i<GetNbEntries();i++)
	{
		// Get current skin
		SkinDescriptor* SD = (SkinDescriptor*)GetEntry(i);

		SD->mPData = new PhysiqueData;
		CHECKALLOC(SD->mPData);
//		GetPhysiqueData(SD->mNode, *SD->mPData);
		SD->mPData->CreatePhysiqueData(SD->mNode);
	}

	return true;
}


//#ifdef TOFIX


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CharactersDescriptor::CharactersDescriptor()
{
//	mNbSkeletons	= 0;
//	mSkeletons		= null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CharactersDescriptor::~CharactersDescriptor()
{
//	DELETEARRAY(mSkeletons);

	for(udword i=0;i<mSkeletons.GetNbEntries();i++)
	{
		SkeletonDescriptor* KD = (SkeletonDescriptor*)mSkeletons.GetEntry(i);
		DELETESINGLE(KD);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	A method to build the whole descriptor.
 *	\param		rootnode	[in] MAX root node
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CharactersDescriptor::BuildDescriptor(INode* rootnode)
{
	// Checkings
	if(!rootnode)	return false;

	// Roadmap:
	// 1) Find all standard bones (native & BIPED)
	// 2) Find all skins (native & PHYSIQUE)
	// 3) Skins give a list of bones, some of them can be meshes => complete the list of bones
	// Step 1) is needed to catch bones not involved in skins (say when there's a BIPED alone)
	// 4) Now that we have all bones, loop through bones to kill parents which are not bones
	// 5) Then assign a root bone to all bones. The list of roots give the list of skeletons.


	// 1) Find all standard bones (native & BIPED)
	bool s = mBones.Init(rootnode);

	mBones.BuildBonesInfo();

	// 2) Find all skins (native & PHYSIQUE)
	s = mSkins.Init(rootnode);

	mSkins.BuildSkinsInfo();

	// 3) Skins give a list of bones, some of them can be meshes => complete the list of bones
	// Step 1) is needed to catch bones not involved in skins (say when there's a BIPED alone)

	// Loop through skins and complete list of bones
	for(udword i=0;i<mSkins.GetNbSkins();i++)
	{
		// Get current skin
		SkinDescriptor* SD = mSkins.GetSkin(i);
		PhysiqueData* PD = SD->mPData;
		if(PD)
		{
			// That's a Physique skin
			udword NbBones = PD->mBones.GetNbNodes();
			for(udword j=0;j<NbBones;j++)
			{
				// Get current bone
				INode* CurBone = PD->mBones.GetNode(j);

				mBones.AddBone(CurBone);

/*				// Check the bone already belongs to our list
				if(!mBones.Contains(CurBone))
				{
					// ...else store it within
//					mBones.Add(
				}
*/
			}
		}
		// 
	}

#ifdef TOFIX
	// Loop through skins and create Physique data
	for(udword i=0;i<mSkins.GetNbSkins();i++)
	{
		// Get current skin
		SkinDescriptor* SD = mSkins.GetSkin(i);
		SD->mPData = new PhysiqueData;
		CHECKALLOC(SD->mPData);
//		GetPhysiqueData(SD->mNode, *SD->mPData);
		SD->mPData->CreatePhysiqueData(SD->mNode);


		// Complete list of bones
/*		PhysiqueData* PD = SD->mPData;
		udword NbBones = PD->mBones.GetNbEntries();
		for(udword j=0;j<NbBones;j++)
		{
			// Get current bone
			udword CurBone = PD->mBones.GetEntry(j);
//			INode* CurBone = (INode*)PD->mBones.GetEntry(j);
			// Check the bone already belongs to our list
			if(!mBones.Contains(CurBone))
			{
				// ...else store it within
				mBones.Add(
			}
		}
*/
		// Create a skeleton for current skin
//		Container Skeleton;
//		SD->CreateSkeletonFromPhysique(Skeleton);
		MAXNodes Skeleton;
		SD->mPData->CreateSkeleton(Skeleton);

		// Look for this skeleton in already existing ones
		SkeletonDescriptor* Owner = null;
		for(udword j=0;j<mSkeletons.GetNbEntries();j++)
		{
			if(!Owner)
			{
				SkeletonDescriptor* SD = (SkeletonDescriptor*)mSkeletons.GetEntry(j);
//				if(SD->SameAs(Skeleton))	Owner = SD;
			}
		}

		if(!Owner)
		{
			// Create a new skeleton
			SkeletonDescriptor* NewSD = new SkeletonDescriptor;
//			NewSD->Init(Skeleton);

			mSkeletons.Add(udword(NewSD));
		}
	}
#endif
	// Each skin gives a linear list of bones, i.e. a skeleton. If we sort it, we get a signature to compare
	// a skeleton to another one. If we find 2 skins with the same skeleton, we have a character made of multiple
	// skins.


	return true;
}

//#endif





























#ifdef OLDIES


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSExporter::CSExporter()
{
	mNbVirtualBones	= 0;
	mVBones			= new VirtualBone[MAX_VIRTUAL_BONES];
	ZeroMemory(mVBones, MAX_VIRTUAL_BONES*sizeof(VirtualBone));

	ComputeCSTotalNbNodes();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSExporter::~CSExporter()
{
	for(udword i=0;i<mNbVirtualBones;i++)
	{
		DELETEARRAY(mVBones[i].Name);
	}
	DELETEARRAY(mVBones);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	A method to compute a bone's ID thanks to its Character Studio name.
 *	\param		name		[in] the Character Studio biped-part's default name
 *	\return		my CSID or INVALID_ID if unknown
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
udword CSExporter::GetBoneID(const char* name) const
{
	// Now search virtual bones
//	udword VID =  GetVirtualBoneID(name);
//	if(VID!=udword(-1)) return VID;

	// First look into the real bones' list
	char FixedName[1024];
	char NodeName[256];
	char* List = CharacterStudioNames;
	char* EndOfList = List + strlen((const char*)CharacterStudioNames);
	udword BoneID=0, i;

	strcpy((char*)FixedName, (const char*)FixCSNodeName((char*)name));

	while(List!=EndOfList)
	{
		i=0;
		while(*List!='\n')
		{
			NodeName[i++] = *List++;
		}
		NodeName[i] = '\0';
		List++;

		if(!strcmp((const char*)FixedName, (const char*)NodeName))	return BoneID;

		BoneID++;
	}


	// Now search virtual bones
	return GetVirtualBoneID(name);
//	return udword(-1);
}

udword CSExporter::RegisterVirtualBone(INode* vbone, const char* name)
{
	udword VirtualID = mCSTotalNbNodes+mNbVirtualBones;

	mVBones[mNbVirtualBones].MAXNode	= vbone;
	mVBones[mNbVirtualBones].VirtualID	= VirtualID;
	if(name)
	{
		long Length = strlen(name)+1;
		mVBones[mNbVirtualBones].Name = new ubyte[Length];
		CopyMemory(mVBones[mNbVirtualBones].Name, name, Length);
	}
	mNbVirtualBones++;
	return VirtualID;
}

udword CSExporter::GetVirtualBoneID(const char* name) const
{
	for(udword i=0;i<mNbVirtualBones;i++)
	{
//		if(!strcmp(name, (const char*)mVBones[i].Name)) return mVBones[i].VirtualID;
		if(AreEqual((void*)name, (void*)mVBones[i].Name)) return mVBones[i].VirtualID;
	}
	return INVALID_ID;
}
udword CSExporter::GetVirtualBoneID(INode* node) const
{
	for(udword i=0;i<mNbVirtualBones;i++)
	{
		if(mVBones[i].MAXNode==node) return mVBones[i].VirtualID;
	}
	return INVALID_ID;
}

#endif