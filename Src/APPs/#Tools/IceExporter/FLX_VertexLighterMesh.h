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
 *	\file		FLX_VertexLighterMesh.h
 *	\author		Pierre Terdiman
 *	\date		August, 28, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_VERTEXLIGHTERMESH_H
#define FLX_VERTEXLIGHTERMESH_H

#ifdef COMPILE_VERTEX_LIGHTER

	// Forward declarations
	class OPCODE_Model;

	// Supposed to provide support for:
	// - merging
	// - consolidation
	// - stabbing
	class MAXMeshHelper
	{
		public:
		// Constructor/Destructor
									MAXMeshHelper();
		virtual						~MAXMeshHelper();

				bool				AddMesh(Object* node_object, INode* node);
				bool				Merge(TimeValue current_time);
		virtual	void				MergeCallback(MAXMeshPointer* merged_mesh)
									{
										// Handle this in your derived classes
									}

				udword				Stab(const Ray& world_ray, float max_dist);

		protected:
				Container			mMergedNodeTable;
				Container			mMergedObjectTable;

				udword*				mVertexCountTable;
				udword*				mFaceCountTable;
				BOOL*				mParityTable;

				udword				mNbVerts;
				udword				mNbFaces;
				Point*				mVerts;
				IndexedTriangle*	mFaces;
				OPCODE_Model*		mModel;
    };

	class VertexLighterMesh : public VertexLighterBase, public MAXMeshHelper
	{
		public:
								VertexLighterMesh();
		virtual					~VertexLighterMesh();

		// Override MAX_MESH_HELPER
		virtual	bool			BuildMesh(Object* node_object, INode* node);

		// Override MAX_MESH_HELPER
		virtual	void			MergeCallback(MAXMeshPointer* merged_mesh);

		// no cache, loop on vertices
				BOOL			Light();

		// cache, loop on lights
				BOOL			Light2();

				BOOL			ApplyModifiers();

		private:

		inline_	BOOL*			PrepareValidLightArray();
	};

#endif

#endif // FLX_VERTEXLIGHTERMESH_H
