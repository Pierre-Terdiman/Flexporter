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
 *	\file		FLX_CharacterStudio.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_CHARACTERSTUDIO_H
#define FLX_CHARACTERSTUDIO_H

	enum BoneType
	{
		BONE_NONE			= 0,			//!< Not a bone
		BONE_BIPED_MAIN		= 1,			//!< Main BIPED part
		BONE_BIPED_SLAVE	= 2,			//!< Slave BIPED part
		BONE_BIPED_FOOT		= 3,			//!< Foot BIPED part
		BONE_BIPED_DUMMY	= 4,			//!< Dummy BIPED part
		BONE_NATIVE			= 5,			//!< MAX native bone
		BONE_MESH			= 6,			//!< Mesh bone (weapon, etc)
		BONE_FORCE_DWORD	= 0x7fffffff
	};

	enum SkinType
	{
		SKIN_NONE			= 0,			//!< Not a skin
		SKIN_PHYSIQUE		= 1,			//!< A Physique skin
		SKIN_NATIVE			= 2,			//!< A MAX native skin
		SKIN_FORCE_DWORD	= 0x7fffffff
	};

	class PhysiqueData
	{
		public:
		// Constructor/Destructor
								PhysiqueData()	{}
								~PhysiqueData()	{}

				bool			CreatePhysiqueData(INode* node);
				udword			CreateSkeleton(MAXNodes& bones);
//		private:
				udword			mNbVerts;
				Vertices		mOffsetVectors;
				MAXNodes		mBones;
//						Container		mBonesID;
				Container		mBonesNb;
				Container		mWeights;
				bool			mUseMultipleBones;
	};

	class SkinData
	{
		public:
		// Constructor/Destructor
								SkinData()	{}
								~SkinData()	{}

				bool			CreateSkinData(INode* node);
//		private:
				udword			mNbVerts;
//						Vertices		mOffsetVectors;
				MAXNodes		mBones;
//						Container		mBonesID;
				Container		mBonesNb;
				Container		mWeights;
				bool			mUseMultipleBones;
	};

	// Character-related helpers

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Finds Character Studio's Physique. This code comes from the Character Studio SDK.
	 *	\fn			FindPhysiqueModifier(INode* node)
	 *	\param		node	[in] a MAX node
	 *	\return		Physique modifier or null
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Modifier*	FindPhysiqueModifier(INode* node);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Finds the Skin modifier.
	 *	\fn			FindSkinModifier(INode* node)
	 *	\param		node	[in] a MAX node
	 *	\return		Skin modifier or null
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Modifier*	FindSkinModifier(INode* node);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Fixes a modified CS name and transforms it into a default CS name.
	 *	\fn			FixCSNodeName(const char* originalname)
	 *	\param		originalname	[in] the Character Studio biped-part's name
	 *	\return		the Character Studio biped-part's default name
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const char*	FixCSNodeName(const char* originalname);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Gets the root-node of any Character Studio biped parts.
	 *	It uses IsSkeleton() which handles user-defined virtual bones, so it always works.
	 *	\fn			GetCSRootNode(INode* currentnode)
	 *	\param		currentnode		[in] biped node
	 *	\return		biped root node or null if not found
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	INode*		GetCSRootNode(INode* currentnode);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Checks whether a node is a biped part or not.
	 *	It also handles user-defined virtual bones.
	 *	\fn			IsSkeleton(INode* node)
	 *	\param		node		[in] a MAX node
	 *	\return		BoneType enumeration code
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BoneType	IsSkeleton(INode* node);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Checks whether a node is a skin or not.
	 *	\fn			IsSkin(INode* node)
	 *	\param		node	[in] a MAX node
	 *	\return		SkinType enumeration code
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SkinType	IsSkin(INode* node);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Gets the total number of Character Studio's available node-names.
	 *	\fn			GetCSTotalNbNodes()
	 *	\return		total number of Character Studio's available node-names.
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	udword		GetCSTotalNbNodes();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Computes a bone's CSID.
	 *	\fn			GetCSID(INode* node)
	 *	\param		node	[in] a MAX node
	 *	\return		a CSID or INVALID_ID if unknown
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	udword		GetCSID(INode* node);


	// BELOW NOT CHECKED

	class CharactersDescriptor;
	class SkeletonDescriptor;

	class FlexBone
	{
		public:
		// Constructor/Destructor
										FlexBone()
										{
											mType		= BONE_NONE;
											mCSID		= INVALID_ID;
											mParentCSID	= INVALID_ID;
											mName		= null;
											mParentName	= null;
										}
										~FlexBone()
										{
										}
		// Data access
		inline_	BoneType				GetBoneType()	const	{ return mType;			}
		inline_	udword					GetCSID()		const	{ return mCSID;			}
		inline_	udword					GetParentCSID()	const	{ return mParentCSID;	}
		inline_	const char*				GetName()		const	{ return mName;			}
		inline_	const char*				GetParentName()	const	{ return mParentName;	}
		private:
				BoneType				mType;			//!< Type of bone
				udword					mCSID;			//!< Bone's CSID
				udword					mParentCSID;	//!< Parent bone's CSID
				const char*				mName;			//!< Bone's name
				const char*				mParentName;	//!< Parent bone's name

		friend	class					BonesContainer;
	};

	class BoneDescriptor
	{
		public:
		// Constructor/Destructor
										BoneDescriptor()
										{
											mNode	= null;
											mParent	= null;
										}
										~BoneDescriptor()
										{
										}

				const BoneDescriptor*	GetRoot()		const
				{
					const BoneDescriptor* Current = this;
					while(Current->mParent)
					{
						Current = Current->mParent;
					}
					// Here, Current->mParent is null, hence Current is root.
					return Current;
				}

		// Data access
		inline_	const FlexBone*			GetBone()		const	{ return &mBone;		}
		inline_	INode*					GetMAXNode()	const	{ return mNode;			}
		inline_	const BoneDescriptor*	GetParent()		const	{ return mParent;		}
		private:
				FlexBone				mBone;
				INode*					mNode;			//!< MAX node => current bone
				BoneDescriptor*			mParent;

		friend	class					BonesContainer;
	};

	class BonesContainer : private Container
	{
		public:
		// Constructor/Destructor
										BonesContainer();
										~BonesContainer();

				bool					Init(INode* rootnode);
				bool					AddBone(INode* currentnode);
				bool					BuildBonesInfo();
		private:
				bool					_FindStandardBones(INode* currentnode);
	};

	class SkinDescriptor
	{
		public:
		// Constructor/Destructor
										SkinDescriptor()
										{
											mNode	= null;
											mType	= SKIN_NONE;
											mPData	= null;
											mOwner	= null;
										}
										~SkinDescriptor()
										{
											DELETESINGLE(mPData);
										}
		// Data access
		inline_	INode*					GetMAXNode()	const	{ return mNode;			}
		inline_	SkinType				GetSkinType()	const	{ return mType;			}
		private:
				INode*					mNode;			//!< MAX node => the skin
				SkinType				mType;			//!< Type of skin
				PhysiqueData*			mPData;			//!< Native Physique data
				SkeletonDescriptor*		mOwner;

		friend	class					SkinsContainer;
		friend	class					CharactersDescriptor;
	};

	class SkinsContainer : private Container
	{
		public:
		// Constructor/Destructor
										SkinsContainer();
										~SkinsContainer();

				bool					Init(INode* rootnode);
				bool					AddSkin(INode* currentnode);
				bool					BuildSkinsInfo();

				udword					GetNbSkins()			const	{ return GetNbEntries();					}
				SkinDescriptor*			GetSkin(udword i)		const	{ return (SkinDescriptor*)GetEntry(i);		}
				SkinDescriptor**		GetSkins()				const	{ return (SkinDescriptor**)GetEntries();	}
		private:
				bool					_FindStandardSkins(INode* currentnode);
	};

	class SkeletonDescriptor
	{
		public:
		// Constructor/Destructor
										SkeletonDescriptor()
										{
											mNbNodes	= 0;
											mSkeleton	= null;
										//	mNbSkins	= 0;
										//	mSkins		= null;
										}
										~SkeletonDescriptor()
										{
											DELETEARRAY(mSkeleton);
										//	DELETEARRAY(mSkins);
										}
		// Initialize
				bool					Init(const Container& bones);
		// Data access
		private:
				udword					mNbNodes;		//!< Number of MAX nodes involved in this skeleton
				BoneDescriptor*			mSkeleton;		//!< Linear list of MAX nodes involved in this skeleton
//				udword					mNbSkins;		//!< Number of attached skins
//				SkinDescriptor*			mSkins;			//!< Linear list of attached skins

		friend	class					CharactersDescriptor;
	};

//#ifdef TOFIX
	class CharactersDescriptor
	{
		public:
		// Constructor/Destructor
										CharactersDescriptor();
										~CharactersDescriptor();
		// Initialize
				bool					BuildDescriptor(INode* rootnode);
		// Data access
		private:
//				udword					mNbSkeletons;	//!< Number of skeletons in the scene
//				SkeletonDescriptor*		mSkeletons;		//!< Linear list of skeletons in the scene
		//
				BonesContainer			mBones;
				Container				mSkeletons;
				SkinsContainer			mSkins;
	};

//#endif

/*
	#define MAX_VIRTUAL_BONES	256	// ### FIX

	//! A virtual bone is a non-BIPED part linked to a BIPED skeleton.
	struct VirtualBone{
		INode*	MAXNode;				//!< Virtual bone's MAX node
		udword	VirtualID;				//!< Virtual bone's ID
		ubyte*	Name;					//!< Virtual bone's name
	};
*/

/*
	class CSExporter
	{
		public:
		// Constructor/Destructor
										CSExporter();
										~CSExporter();
		// CSID management
						udword			GetBoneID(const char* name)			const;
		// Virtual bones management
						udword			RegisterVirtualBone(INode* vbone=null, const char* name=null);
						udword			GetVirtualBoneID(const char* name)	const;
						udword			GetVirtualBoneID(INode* node)		const;
		// Data access
		__forceinline	udword			GetCSTotalNbNodes()					const	{ return mCSTotalNbNodes+mNbVirtualBones; }

		private:
		// Real bones
						udword			mCSTotalNbNodes;
		// Virtual bones
						udword			mNbVirtualBones;
						VirtualBone*	mVBones;
		// Internal methods
						CSExporter&		ComputeCSTotalNbNodes();
	};
*/

#endif // FLX_CHARACTERSTUDIO_H
