///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains...
 *	\file		FLX_VertexLighter.h
 *	\author		Pierre Terdiman
 *	\date		August, 28, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_VERTEXLIGHTER_H
#define FLX_VERTEXLIGHTER_H

#ifdef COMPILE_VERTEX_LIGHTER

	// Forward declarations
	class VertexLighterLight;
	class VertexLighterMesh;

	enum ConversionFlag
	{
		CVF_GEOM_OBJECT		= (1<<0),
		CVF_DERIVED_OBJECT	= (1<<1),
		CVF_CAMERA			= (1<<2),
		CVF_LIGHT			= (1<<3),
		CVF_SHAPE			= (1<<4),
		CVF_HELPER			= (1<<5),
		CVF_OTHER			= (1<<6),

		CVF_ALL				= 0xffffffff
	};

	class VertexLighterSettings
	{
		public:
				VertexLighterSettings() :
					MAXInterface(null),
			//		MergeType( COMPUTE_VERTEX_DATA_MERGE_TYPE_None ),
					Merge(TRUE),
					SmoothingPassCount(0),
					DisplayProfile(FALSE),
					RedrawOnExit(TRUE),
					UseCache(TRUE),
					ClampResults(FALSE),
					SelfShadowing(FALSE),
					ApplyToSelected(FALSE),
					CastShadows(TRUE),
					ExcludeOffLights(FALSE),
					UseExclusionLists(TRUE)
					{
					}

		Interface*	MAXInterface;
//		COMPUTE_VERTEX_DATA_MERGE_TYPE	MergeType;
		BOOL		Merge;
		udword		SmoothingPassCount;
		BOOL		DisplayProfile;
		BOOL		RedrawOnExit;
		BOOL		UseCache;
		BOOL		ClampResults;
		BOOL		SelfShadowing;
		BOOL		ApplyToSelected;
		BOOL		CastShadows;
		BOOL		ExcludeOffLights;
		BOOL		UseExclusionLists;
	};

	class VertexLighter : public VertexLighterBase
	{
		public:
		// Constructor/Destructor
										VertexLighter();
		virtual							~VertexLighter();

				bool					Convert(const VertexLighterSettings& settings);

		inline_	BOOL					IsClampingEnabled()			const	{ return mSettings.ClampResults;		}
		inline_	BOOL					IsSelfShadowingEnabled()	const	{ return mSettings.SelfShadowing;		}
		inline_	BOOL					AreShadowsEnabled()			const	{ return mSettings.CastShadows;			}
		inline_	BOOL					UsesMerge()					const	{ return mSettings.Merge;				}
		inline_	BOOL					UsesCache()					const	{ return mSettings.UseCache;			}
		inline_	udword					GetSmoothingPassCount()		const	{ return mSettings.SmoothingPassCount;	}
		inline_	BOOL					ExcludesOffLights()			const	{ return mSettings.ExcludeOffLights;	}
		inline_	BOOL					UsesExclusionLists()		const	{ return mSettings.UseExclusionLists;	}
		//
		inline_	udword					GetCurrentTime()			const	{ return mCurrentTime;											}
		inline_	udword					GetLightCount()				const	{ return mLightNodeTable.GetNbObjects();						}
		inline_	VertexLighterLight*		GetLight(udword i)			const	{ return (VertexLighterLight*)mLightNodeTable.GetObject(i);		}
		inline_ VertexLighterLight**	GetLightTable()				const	{ return (VertexLighterLight**)mLightNodeTable.GetObjects();	}
		inline_	udword					GetMeshCount()				const	{ return mMeshNodeTable.GetNbObjects();							}
		inline_	VertexLighterMesh*		GetMesh(udword i)			const	{ return (VertexLighterMesh*)mMeshNodeTable.GetObject(i);		}
		inline_	VertexLighterMesh**		GetMeshTable()				const	{ return (VertexLighterMesh**)mMeshNodeTable.GetObjects();		}

		inline_	BOOL*					GetValidLightArray()		const
				{
					if(!mValidLightArray)	// create the array on first call
					{
						mValidLightArray = new BOOL[GetLightCount()];
					}
					return mValidLightArray;
				}

		inline_	BOOL					NeedsLighting(const Point& world_point, const Point& world_normal, const VertexLighterLight* current_light, const VertexLighterMesh* discarded=null, BOOL use_cache=FALSE) const
				{
					// This really needs inlining!

					// This is an early-exit test to quickly discard vertices that don't need lighting

					// 1. Backface culling. By performing it before everything, we avoid doing ray-casts from vertices
					// whose light intensity is null anyway.
					if((world_normal|(current_light->GetPosition() - world_point))<0.0f)
					{
						return FALSE;
					}

					// 2. Occlusion culling
					if(mSettings.CastShadows && current_light->IsShadowCaster() && !IsVisibleFromLight2(world_point, world_normal, current_light, discarded, use_cache))
					{
						// We're in shadow => don't compute light contribution
						return FALSE;
					}

					// NOTE: without clamping we could also discard vertices already totally lit (saturated to pure white)
					return TRUE;
				}
		private:

		mutable	udword					mStabbedTriangleIndex;
				udword					mCurrentTime;
		mutable	udword					mCacheHitCount;
		mutable BOOL*					mValidLightArray;
				udword					mConvertedNodeCount;
				VLObjects				mLightNodeTable;
				VLObjects				mMeshNodeTable;
		// Settings
				VertexLighterSettings	mSettings;

		// Internal methods
		virtual	void					StartConversion();
		virtual	void					EndConversion();
				void					ConvertNode(INode* current_node, udword conversion_mask);
				void					_ConvertNode(INode* current_node, udword conversion_mask);
				bool					ConvertGeomObject(INode* node, Object* derived_object=null);
				bool					ConvertDerivedObject(INode* node);
				bool					ConvertLight(INode* node);
				bool					AddMesh(INode* node, Object* node_object);
				BOOL					IsVisibleFromLight(const Point& world_point, const Point& world_normal, const VertexLighterLight* current_light, const VertexLighterMesh* discarded=null, BOOL use_cache=FALSE) const;
				BOOL					IsVisibleFromLight2(const Point& world_point, const Point& world_normal, const VertexLighterLight* current_light, const VertexLighterMesh* discarded=null, BOOL use_cache=FALSE) const;
	};

#endif

#endif // FLX_VERTEXLIGHTER_H
