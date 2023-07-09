///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the mesh-related exported structures.
 *	\file		FLX_AddMesh.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_ADDMESH_H
#define FLX_ADDMESH_H

#define OLD_FLEX_FACES

	//! Edge visibility codes.
	enum EdgeVisCode
	{
		FLX_EDGE_0				= (1<<0),		//!< Bitmask for first edge
		FLX_EDGE_1				= (1<<1),		//!< Bitmask for second edge
		FLX_EDGE_2				= (1<<2),		//!< Bitmask for third edge

		FLX_EDGE_FORCE_DWORD	= 0x7fffffff
	};

	//! Mesh flags. Don't forget to copy them in IceZCBFormat.h
	enum MeshFlag
	{
		MESH_VFACE				= (1<<0),		//!< Mesh has vertex-faces.
		MESH_TFACE				= (1<<1),		//!< Mesh has texture-faces.
		MESH_CFACE				= (1<<2),		//!< Mesh has color-faces.
		MESH_UVW				= (1<<3),		//!< UVW's are exported
		MESH_WDISCARDED			= (1<<4),		//!< W is discarded
		MESH_VERTEXCOLORS		= (1<<5),		//!< Vertex colors are exported
		MESH_ONEBONEPERVERTEX	= (1<<6),		//!< Simple skin with one driving bone/vertex
		MESH_CONVEXHULL			= (1<<7),		//!< The convex hull has been exported
		MESH_BOUNDINGSPHERE		= (1<<8),		//!< The bounding sphere has been exported
		MESH_INERTIATENSOR		= (1<<9),		//!< The inertia tensor has been exported
		MESH_QUANTIZEDVERTICES	= (1<<10),		//!< Vertices have been quantized
		MESH_WORDFACES			= (1<<11),		//!< Vertex references within faces are stored as words instead of dwords
		MESH_COMPRESSED			= (1<<12),		//!< Mesh has been saved in a compression-friendly way
		MESH_EDGEVIS			= (1<<13),		//!< Edge visibility has been exported
		// Extra mesh flags actually used in the MeshInfo class
		MESH_CONSOLIDATION		= (1<<16),		//!< Mesh has been consolidated
		MESH_FACENORMALS		= (1<<17),		//!< Export normals to faces
		MESH_VERTEXNORMALS		= (1<<18),		//!< Export normals to vertices
		MESH_NORMALINFO			= (1<<19),		//!< Export NormalInfo
		// 1.15
		MESH_VERTEX_ALPHA		= (1<<20),		//!< Vertex alpha exported
		MESH_DWORD_COLORS		= (1<<21),		//!< Vertex colors are stored as RGBA dwords in the red channel of the consolidation

		MESH_FORCE_DWORD		= 0x7fffffff
	};

#ifdef OLD_FLEX_FACES
	//! A structure to hold face information.
	struct FLEXPORTER_API MAXFace
	{
		sdword			VRef[3];				//!< Vertex references
		sdword			TRef[3];				//!< Texture-vertex references
		sdword			CRef[3];				//!< Vertex-color references
		sdword			MatID;					//!< Material ID
		udword			Smg;					//!< Smoothing groups
		ubyte			EdgeVis;				//!< Edge visibility code
	};
#else
	struct FLEXPORTER_API IndexedFace
	{
		sdword			Ref[3];					//!< Reference to geometry
	};

	struct FLEXPORTER_API Surface
	{
						Surface();
						~Surface();

		udword			mNbVerts;				//!< Number of vertices in the geometry
		IndexedFace*	mFaces;					//!< Topology
		NakedPoint*		mVerts;					//!< Geometry
	};
#endif	

	//! A simple class holding convex hull data
	class FLEXPORTER_API CHull
	{
		public:
		// Constructor / Destructor
									CHull() : mNbFaces(0), mNbVerts(0), mFaces(null), mVerts(null)	{}
									~CHull();
		// Figures
				udword				mNbFaces;	//!< Number of faces
				udword				mNbVerts;	//!< Number of vertices.
		// Topology
				udword*				mFaces;		//!< List of faces
		// Geometry
				NakedPoint*			mVerts;		//!< List of vertices
	};

	//! A simple structure holding volume integrals
	struct FLEXPORTER_API ITensor
	{
				NakedPoint			COM;						//!< Center of mass
				float				Mass;						//!< Total computed mass
				float				InertiaTensor[3][3];		//!< Intertia tensor (mass matrix) relative to the origin
				float				COMInertiaTensor[3][3];		//!< Intertia tensor (mass matrix) relative to the COM
	};

	//! A class to hold skeletal information.
	class FLEXPORTER_API MAXSkeleton
	{
		public:
		// Constructor / Destructor
									MAXSkeleton(udword nb);
									~MAXSkeleton();

				udword				mNbBones;			//!< Number of bones involved
				udword*				mID;				//!< NbBones couples of IDs (CSID, parent CSID).
	};

	//! A class gathering a mesh in MAX native format.
	class FLEXPORTER_API MAXNativeMesh
	{
		public:
		// Constructor / Destructor
									MAXNativeMesh();
									~MAXNativeMesh();


		inline_	udword				GetFlags()			const	{ return mFlags;		}

#ifdef OLD_FLEX_FACES
		inline_	udword				GetNbFaces()		const	{ return mNbFaces;		}
		inline_	udword				GetNbVerts()		const	{ return mNbVerts;		}
		inline_	udword				GetNbTVerts()		const	{ return mNbTVerts;		}
		inline_	udword				GetNbCVerts()		const	{ return mNbCVerts;		}

		inline_	const MAXFace*		GetFaces()			const	{ return mFaces;		}

		inline_	const NakedPoint*	GetVerts()			const	{ return mVerts;		}
		inline_	const NakedPoint*	GetTVerts()			const	{ return mTVerts;		}
		inline_	const NakedPoint*	GetCVerts()			const	{ return mCVerts;		}
#else
/*				udword				mNbFaces;			//!< Number of faces in MAX native data
				Surface				mPointSurface;		//!< Points & triangles
				Surface				mUVWSurface;		//!< Texture-vertices & texture-faces
				Surface				mColorSurface;		//!< Vertex-colors & color-faces
				sdword*				mMatID;				//!< Material IDs (one/face)
				udword*				mSmg;				//!< Smoothing groups (one/face)
				ubyte*				mEdgeVis;			//!< Edge visibility codes (one/face)*/
#endif
		inline_	const float*		GetVertexAlpha()	const	{ return mVertexAlpha;	}
			// Skin data
		inline_	const udword*		GetBonesNb()		const	{ return mBonesNb;		}
		inline_	const udword*		GetBonesCSID()		const	{ return mBonesCSID;	}
		inline_	const udword*		GetBonesLocalID()	const	{ return mBonesLocalID;	}
		inline_	const char**		GetBonesName()		const	{ return mBonesName;	}
		inline_	const NakedPoint*	GetOffsetVectors()	const	{ return mOffsetVectors;}
		inline_	const float*		GetWeights()		const	{ return mWeights;		}
		inline_	const MAXSkeleton*	GetSkeleton()		const	{ return mSkeleton;		}

		inline_	const CHull*		GetConvexHull()		const	{ return mConvexHull;	}

		inline_	const NakedPoint&	GetBSCenter()		const	{ return mBSCenter;		}
		inline_	float				GetBSRadius()		const	{ return mBSRadius;		}

		inline_	const ITensor*		GetTensor()			const	{ return mTensor;		}

		inline_	bool				GetParity()			const	{ return mParity;		}

				NakedPoint			mInternal;

		// Mesh flags
				udword				mFlags;				//!< See flags above (combination of MeshFlag)
//		protected:
#ifdef OLD_FLEX_FACES
		// Figures
				udword				mNbFaces;			//!< Number of faces in MAX native data
				udword				mNbVerts;			//!< Number of vertices.
				udword				mNbTVerts;			//!< Number of texture-vertices (mapping coordinates)
				udword				mNbCVerts;			//!< Number of vertex-colors
		// Topologies
				MAXFace*			mFaces;				//!< List of faces
		// Geometries
				NakedPoint*			mVerts;				//!< List of vertices (null for skins, use offset vectors)
				NakedPoint*			mTVerts;			//!< List of texture-vertices
				NakedPoint*			mCVerts;			//!< List of vertex-colors
#else
				udword				mNbFaces;			//!< Number of faces in MAX native data
				Surface				mPointSurface;		//!< Points & triangles
				Surface				mUVWSurface;		//!< Texture-vertices & texture-faces
				Surface				mColorSurface;		//!< Vertex-colors & color-faces
				sdword*				mMatID;				//!< Material IDs (one/face)
				udword*				mSmg;				//!< Smoothing groups (one/face)
				ubyte*				mEdgeVis;			//!< Edge visibility codes (one/face)
#endif
				float*				mVertexAlpha;		//!< List of mNbVerts alpha-values [Flexporter 1.14]
			// Skin data
				udword*				mBonesNb;			//!< [only for skins] Number of bones for each vertex, or null if one bone/vertex.
				udword*				mBonesCSID;			//!< [only for skins] CSIDs of driving bones.
				udword*				mBonesLocalID;		//!< [only for skins] Local IDs of driving bones, in MAX hierarchy.[Flexporter 1.14]
				const char**		mBonesName;			//!< [only for skins] Names of driving bones.[Flexporter 1.14]
				NakedPoint*			mOffsetVectors;		//!< [only for skins] Character Studio's offset vectors.
				float*				mWeights;			//!< [only for skins] Bones weights when there's more than one bone/vertex, else null.
				MAXSkeleton*		mSkeleton;			//!< [only for skins] Skeletal information
		// Possible convex hull
				CHull*				mConvexHull;		//!< Convex hull, only if the ConvexHull field is true in the ExportSettings.
		// Possible bounding sphere
				NakedPoint			mBSCenter;			//!< Bounding sphere center, only if the BoundingSphere field is true in the ExportSettings.
				float				mBSRadius;			//!< Bounding sphere radius, only if the BoundingSphere field is true in the ExportSettings.
		// Possible inertia tensor
				ITensor*			mTensor;			//!< Volume integrals, only if the InertiaTensor field is true in the ExportSettings.
		// Misc
				bool				mParity;			//!< Faces are CW or CCW.
		// Following methods are only available from Flexporter
				bool				Transform(const Matrix4x4& mat);
				bool				ComputeGeometricCenter(Point& center);
				bool				SetVerts(udword nb_verts, const NakedPoint* verts);
				bool				SetTVerts(udword nb_verts, const NakedPoint* verts);
				bool				SetCVerts(udword nb_verts, const NakedPoint* verts);
	};

	//! The mesh information class.
	class FLEXPORTER_API MeshDescriptor : public ObjectDescriptor
	{
		public:
		// Constructor / Destructor
									MeshDescriptor();
		virtual						~MeshDescriptor();

		// Parameters
				bool				mIsCollapsed;		//!< true => mesh comes from a collapsed modifier stack
				bool				mIsSkeleton;		//!< true => mesh is a BIPED part (i.e. a bone)
				bool				mIsTarget;			//!< true => mesh is a target node (camera, spot or directional target)
				bool				mIsConvertible;		//!< false => mesh can't be converted to triangles (and the native format is unsupported)
				bool				mIsSkin;			//!< true => the mesh is a PHYSIQUE skin
				bool				mCastShadows;		//!< true => a shadow volume can be built from the mesh
				bool				mReceiveShadows;	//!< true => mesh can receive shadows [Flexporter 1.15]
				bool				mMotionBlur;		//!< true => mesh can be motion blured [Flexporter 1.15]

		// Instance information (valid if mIsInstance==true)
				MeshDescriptor*		mMasterMesh;		//!< Master object
		// Mesh data
				MAXNativeMesh*		mOriginalMesh;		//!< Original mesh from MAX (null for instances)
				MBResult*			mCleanMesh;			//!< Mesh after consolidation

		// Biped/Physique-related information (valid if mIsSkeleton==true || mIsSkin==true)
				udword				mCharID;			//!< Owner character's ID
		// Biped-related information (valid if mIsSkeleton==true)
				udword				mCSID;				//!< CSID code.

		// Lightmapper data
#ifdef SUPPORT_OLD_LIGHTMAPPER
				udword				mNbColors;			//!< Number of computed colors
				NakedPoint*			mColors;			//!< Computed colors
#endif
		// Primitive parameters
				PrimitiveParams*	mPrimParams;		//!< null if the mesh is not a supported primitive [Flexporter 1.15]
		private:
				MeshBuilder2*		mBuilder;			//!< A mesh cleaner.

		public:
		// Following methods are only available from Flexporter
				bool				CreateOriginalMesh();
				bool				Consolidate(const ExportSettings* settings, bool force_normals, bool discard_normals, bool morph);
	};

#endif // FLX_ADDMESH_H
