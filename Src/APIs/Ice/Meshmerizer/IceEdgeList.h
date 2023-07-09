///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code to create efficient edge lists.
 *	\file		IceEdgeList.h
 *	\author		Pierre Terdiman
 *	\date		January, 17, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEEDGELIST_H
#define ICEEDGELIST_H

	enum EdgeType
	{
		EDGE_UNDEFINED,

		EDGE_BOUNDARY,		//!< Edge belongs to a single triangle
		EDGE_INTERNAL,		//!< Edge belongs to exactly two triangles
		EDGE_SINGULAR,		//!< Edge belongs to three or more triangles

		EDGE_FORCE_DWORD	= 0x7fffffff
	};

	enum EdgeFlag
	{
		EDGE_ACTIVE	= (1<<0)
	};

#ifndef MSH_NEW_EDGE_LIST
	//! Edge structure for each face
	struct MESHMERIZER_API EdgesRefs
	{
		udword	Ref[3];		//!< Indices in mEdges
		//! Ref[0] is edge 0-1,
		//! Ref[1] is edge 1-2,
		//! Ref[2] is edge 2-0.
	};
#endif

	//! A basic edge class
	class MESHMERIZER_API Edge
	{
		public:
		//! Constructor
		inline_				Edge()		{}
		//! Destructor
		inline_				~Edge()		{}

				udword		mRef0;		//!< First vertex reference
				udword		mRef1;		//!< Second vertex reference
	};

	//! A count/offset pair = an edge descriptor
	struct MESHMERIZER_API EdgeDesc
	{
		//! Constructor
		inline_				EdgeDesc() : Flags(0), Count(0), Offset(0)	{}

				uword		Flags;		//!< Edge flags
				uword		Count;		//!< Number of faces sharing this edge
				udword		Offset;		//!< Corresponding offset in mFacesByEdges

		inline_	BOOL		IsActive()	const	{ return Flags & EDGE_ACTIVE;	}

		inline_	EdgeType	GetType()	const
							{
								if(Count>=3)	return EDGE_SINGULAR;
								return EdgeType(Count);
							}
	};

	#define MSH_EDGE_LINK_MASK		0x3fffffff
	#define MSH_ACTIVE_EDGE_MASK	0x80000000
	#define MSH_ACTIVE_VERTEX_MASK	0x40000000

	class EdgeTriangle
	{
		public:
		inline_							EdgeTriangle()									{}
		inline_							~EdgeTriangle()									{}

		inline_	udword					GetEdge01()								const	{ return mLink[0] & MSH_EDGE_LINK_MASK;		}
		inline_	udword					GetEdge12()								const	{ return mLink[1] & MSH_EDGE_LINK_MASK;		}
		inline_	udword					GetEdge20()								const	{ return mLink[2] & MSH_EDGE_LINK_MASK;		}
		inline_	udword					GetEdge(udword i)						const	{ return mLink[i] & MSH_EDGE_LINK_MASK;		}

		inline_	BOOL					HasActiveEdge01()						const	{ return mLink[0] & MSH_ACTIVE_EDGE_MASK;	}
		inline_	BOOL					HasActiveEdge12()						const	{ return mLink[1] & MSH_ACTIVE_EDGE_MASK;	}
		inline_	BOOL					HasActiveEdge20()						const	{ return mLink[2] & MSH_ACTIVE_EDGE_MASK;	}
		inline_	BOOL					HasActiveEdge(udword i)					const	{ return mLink[i] & MSH_ACTIVE_EDGE_MASK;	}

		inline_	BOOL					HasActiveVertex0()						const	{ return mLink[0] & MSH_ACTIVE_VERTEX_MASK;	}
		inline_	BOOL					HasActiveVertex1()						const	{ return mLink[1] & MSH_ACTIVE_VERTEX_MASK;	}
		inline_	BOOL					HasActiveVertex2()						const	{ return mLink[2] & MSH_ACTIVE_VERTEX_MASK;	}
		inline_	BOOL					HasActiveVertex(udword i)				const	{ return mLink[i] & MSH_ACTIVE_VERTEX_MASK;	}

		private:
				//! mLink[0] is edge 0-1,
				//! mLink[1] is edge 1-2,
				//! mLink[2] is edge 2-0.
				udword					mLink[3];

		friend class EdgeList;
	};

	//! The edge-list creation structure.
	struct EDGELISTCREATE
	{
		//! Constructor
								EDGELISTCREATE()	: NbFaces(0), DFaces(null), WFaces(null), FacesToEdges(false), EdgesToFaces(false), Verts(null), Epsilon(0.001f)	{}
				
				udword			NbFaces;		//!< Number of faces in source topo
				const udword*	DFaces;			//!< List of faces (dwords) or null
				const uword*	WFaces;			//!< List of faces (words) or null

				bool			FacesToEdges;
				bool			EdgesToFaces;
				const Point*	Verts;
				float			Epsilon;
	};

	class MESHMERIZER_API EdgeList
	{
		public:
		// Constructor / destructor
										EdgeList();
										~EdgeList();

				bool					Init(const EDGELISTCREATE& create);

		// Data access
		inline_	udword					GetNbEdges()							const	{ return mNbEdges;						}
		inline_	const Edge*				GetEdges()								const	{ return mEdges;						}
		inline_	const Edge&				GetEdge(udword edge_index)				const	{ return mEdges[edge_index];			}
		//
		inline_	udword					GetNbFaces()							const	{ return mNbFaces;						}
#ifndef MSH_NEW_EDGE_LIST
		inline_	const EdgesRefs*		GetEdgesRef()							const	{ return mEdgesRef;						}
		inline_	const EdgesRefs&		GetEdgesRef(udword face_index)			const	{ return mEdgesRef[face_index];			}
#else
		inline_	const EdgeTriangle*		GetEdgeTriangles()						const	{ return mEdgeFaces;					}
		inline_	const EdgeTriangle&		GetEdgeTriangle(udword face_index)		const	{ return mEdgeFaces[face_index];		}
#endif
		//
		inline_	const EdgeDesc*			GetEdgeToTriangles()					const	{ return mEdgeToTriangles;				}
		inline_	const EdgeDesc&			GetEdgeToTriangles(udword edge_index)	const	{ return mEdgeToTriangles[edge_index];	}
		inline_	const udword*			GetFacesByEdges()						const	{ return mFacesByEdges;					}
		inline_	udword					GetFacesByEdges(udword face_index)		const	{ return mFacesByEdges[face_index];		}

		private:
		// The edge list
				udword					mNbEdges;				//!< Number of edges in the list
				Edge*					mEdges;					//!< List of edges
		// Faces to edges
				udword					mNbFaces;				//!< Number of faces for which we have data
#ifndef MSH_NEW_EDGE_LIST
				EdgesRefs*				mEdgesRef;				//!< A EdgesRefs structure for each face
#else
				EdgeTriangle*			mEdgeFaces;				//!< Array of edge-triangles referencing mEdges
#endif
		// Edges to faces
				EdgeDesc*				mEdgeToTriangles;		//!< An EdgeDesc structure for each edge
				udword*					mFacesByEdges;			//!< A pool of face indices
		// Internal methods
				bool					CreateFacesToEdges(udword nb_faces, const udword* dfaces, const uword* wfaces);
				bool					CreateEdgesToFaces(udword nb_faces, const udword* dfaces, const uword* wfaces);
				bool					ComputeActiveEdges(udword nb_faces, const udword* dfaces, const uword* wfaces, const Point* verts, float epsilon);
	};

	class MESHMERIZER_API SubmeshExtractor
	{
		public:
		// Constructor / destructor
										SubmeshExtractor();
										~SubmeshExtractor();

				const udword*			ExtractSubMeshes(udword& nb_parts, const EdgeList& list);

		//! These calls are only valid after ExtractSubMeshes() has been called
		inline_	udword					GetNbSubmeshes()	const	{ return mID;	}
		inline_	const udword*			GetTags()			const	{ return mTags;	}

		private:
				udword					mNbConnectedFaces;		//!< Number of connected faces for current submesh
				udword					mID;					//!< Submesh ID
				udword*					mTags;					//!< One tag/face, each face is marked with a submesh ID
		// Internal methods
				bool					ExtractSubmeshFromFace(udword face_nb, const EdgeList& list);
				void					_TrackTriangle(udword face_nb, const EdgeList& list);
	};

#endif // ICEEDGELIST_H
