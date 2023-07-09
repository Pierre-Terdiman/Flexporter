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
 *	\file		FLX_GlobalInfo.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_GLOBALINFO_H
#define FLX_GLOBALINFO_H

	#define	IMAX_MAX_TEXTURES		2048	// ### TO FIX

	class SceneInfo
	{
		public:
		// Constructor/Destructor
									SceneInfo() :	mNbNodes(0),
													mNbCharacters(0)	{}
									~SceneInfo()						{}

				udword				mNbNodes;			//!< Number of nodes in MAX scene
				udword				mNbCharacters;		//!< Number of characters in MAX scene
	};

	class NodeInfo
	{
		public:
		// Constructor/Destructor
									NodeInfo() :	mMAXNode(null),
													mExportedNode(null),
													mObject(null),
//													mObjectType(OBJ_UNDEFINED),
													mParent(null),
													//mAlreadyConverted(false),
													mGroupHead(false),
													mGroupMember(false)
																				{}
									~NodeInfo()									{ DELETESINGLE(mExportedNode); }

				INode*				mMAXNode;			//!< MAX node pointer
				ResourceDescriptor*	mExportedNode;		//!< Exported node structure
				Object*				mObject;			//!< MAX node object
//				ObjType				mObjectType;		//!< Object type
				NodeInfo*			mParent;			//!< Parent node
				//bool				mAlreadyConverted;	//!< false if master, true if instance => use mExportedNode instead
				bool				mGroupHead;
				bool				mGroupMember;

				inline_				void			AddToGroup(udword id)	{ mGrouped.Add(id);	}
									Container		mGrouped;
	};

	class TMapInfo
	{
		public:
		// Constructor/Destructor
									TMapInfo() :	mMapID(INVALID_ID)	{}
									~TMapInfo()							{}

				String				mMAXRGBBitmap;		//!< MAX RGB bitmap name
				String				mMAXAlphaBitmap;	//!< MAX Alpha bitmap name
				long				mMapID;				//!< Coupled ID
	};

	class MatlInfo
	{
		public:
		// Constructor/Destructor
									MatlInfo() :	mMAXMaterial(null),
													mRemapID(INVALID_ID)	{}
									~MatlInfo()								{}

				Mtl*				mMAXMaterial;		//!< MAX material pointer
				udword				mRemapID;			//!< Remapped ID
	};

	class MaterialContainer : private Container
	{
		public:
		// Constructor / Destructor
		inline_						MaterialContainer()			{}
		inline_						~MaterialContainer()		{}

		inline_	udword				GetNbMaterials()	const	{ return GetNbEntries()/2;						}
		inline_	MatlInfo*			GetMaterials()		const	{ return (MatlInfo*)GetEntries();				}

				void				AddMaterial(Mtl* maxmat)	{ Add(udword(maxmat)).Add(INVALID_ID);			}
	};

	class BoneInfo
	{
		public:
		// Constructor/Destructor
									BoneInfo() :	mBone(null),
													mFatherBone(null),
													mCSID(INVALID_ID),
													mpCSID(INVALID_ID)	{}
									~BoneInfo()							{}

				INode*				mBone;				//!< MAX node => current bone
				INode*				mFatherBone;		//!< MAX node => parent bone
				udword				mCSID;				//!< Bone's ID
				udword				mpCSID;				//!< Parent bone's ID
	};

	class CharInfo
	{
		public:
		// Constructor/Destructor
									CharInfo() :	mRootNode(null),
													mNbNodes(0),
													mNbVirtualBones(0),
													mSkeleton(null)		{}
									~CharInfo()							{ DELETEARRAY(mSkeleton);	}

				INode*				mRootNode;			//!< MAX node => the character's root bone (BIPED_MAIN)
				udword				mNbNodes;			//!< the number of MAX nodes involved in this character
				udword				mNbVirtualBones;	//!< the number of extra bones involved in this character
				BoneInfo*			mSkeleton;
	};

	// Texture creation structure. This is used to create a blended texture with,
	// for example, RGB data from the diffuse texmap and alpha data from the opacity one.
	class TextureBlend
	{
		public:
		// Constructor/Destructor
									TextureBlend() :	mRGBMap(null),
														mAlphaMap(null),
														mRGBMapCoeff(0.0f),
														mAlphaMapCoeff(0.0f),
														mIsBumpMap(false)
														{
															mRGBDefault		= Color(0.0f, 0.0f, 0.0f);
															mAlphaDefault	= Color(0.0f, 0.0f, 0.0f);
														}
									~TextureBlend()		{}

				Texmap*				mRGBMap;			//!< Source texmap for RGB channels
				Texmap*				mAlphaMap;			//!< Source texmap for alpha channel
				Color				mRGBDefault;		//!< RGB default color
				Color				mAlphaDefault;		//!< Alpha default value
				float				mRGBMapCoeff;		//!< We compute: Data from RGBMap * RGBMapCoeff + RGBDefault * (1.0f - RGBMapCoeff)
				float				mAlphaMapCoeff;		//!< We compute: Data from AlphaMap * AlphaMapCoeff + AlphaDefault * (1.0f - AlphaMapCoeff)
				bool				mIsBumpMap;			//!< Bump map flag
	};

	class GlobalInfo
	{
		public:
		// Constructor/Destructor
									GlobalInfo();
									~GlobalInfo();

		// Material stuff
		inline_	MaterialContainer&	GetMaterialContainer()								{ return mMaterials;					}

		// Texture stuff
		inline_	const TMapInfo*		GetTextureInfo()						const		{ return mTexmInfo;						}
		inline_	void				SetMAXRGBBitmapName(udword i, const String& name)	{ mTexmInfo[i].mMAXRGBBitmap = name;	}
		inline_	void				SetMAXAlphaBitmapName(udword i, const String& name)	{ mTexmInfo[i].mMAXAlphaBitmap = name;	}
		inline_	void				SetTextureID(udword i, long id)						{ mTexmInfo[i].mMapID = id;				}

		// Data access
		inline_	const SceneInfo*	GetSceneInfo()							const		{ return &mSceneInfo;					}
		inline_	const NodeInfo*		GetNodeInfo()							const		{ return mNodeInfo;						}
		inline_	const CharInfo*		GetCharacterInfo()						const		{ return mCharInfo;						}

		inline_	udword				GetTotalNbNodes()						const		{ return mSceneInfo.mNbNodes;			}
		inline_	const CharInfo*		GetCharacter(udword id)					const		{ return &mCharInfo[id];				}
				udword				GetCharacterID(INode* node)				const;	//!< Gets a character ID from a MAX node
				udword				GetNodeID(INode* current_node)			const;
				udword				GetNodeID(Object* object, bool master)	const;
		// Q & A
				bool				IsInstance(Object* node_object, sdword& master)	const;
		// Data mining
				bool				PreprocessScene(INode* root_node);
		// Object creation
				ResourceDescriptor*	CreateDescriptor(INode* node, ObjType type);
		private:
		// Scene data
				udword				mNbNodes;
				SceneInfo			mSceneInfo;			//!< Scene info
				NodeInfo*			mNodeInfo;			//!< Node info (one entry/node in MAX scene, including root node)
				CharInfo*			mCharInfo;			//!< Character info (one entry/character)

				TMapInfo*			mTexmInfo;			//!< Texture info
				MaterialContainer	mMaterials;			//!< Material info
		// Internal methods
				void				FixParentPointers();
				NodeInfo*			GetGroupHead(NodeInfo* object) const;
				bool				_CreateSceneInfo		(INode* current_node, Container& invalid);
				bool				_FindCharactersRoots	(INode* current_node);
				bool				_CompleteCharactersInfo	(INode* current_node);
	};

#endif // FLX_GLOBALINFO_H
