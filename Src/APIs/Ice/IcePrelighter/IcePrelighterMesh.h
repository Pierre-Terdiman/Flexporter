///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains prelighter code for meshes.
 *	\file		IcePrelighterMesh.h
 *	\author		Pierre Terdiman
 *	\date		April, 20, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEPRELIGHTERMESH_H
#define ICEPRELIGHTERMESH_H

	struct ICEPRELIGHTER_API PRELIGHTER_MESH_CREATE : PRELIGHTER_OBJECT_CREATE
	{
							PRELIGHTER_MESH_CREATE();

		MultiSurface*		mSourceMesh;
		const Matrix4x4*	mWorld;
	};

	class ICEPRELIGHTER_API PrelighterMesh : public PrelighterObject
	{
		public:
		// Constructor/Destructor
										PrelighterMesh();
		virtual							~PrelighterMesh();
		// Settings
		virtual	bool					Init(const PRELIGHTER_OBJECT_CREATE& create);

		// Data access
		inline_	Model*					GetOPCODEModel()
										{
											if(!mOPCODEModel)	BuildOPCODEModel();
											return mOPCODEModel;
										}

		inline_	ConsolidatedSurface*	GetConsolidated()
										{
											if(!mConsolidated)	BuildConsolidated();
											return mConsolidated;
										}

		inline_	const MultiSurface*		GetSourceMesh()		const	{ return mSourceMesh;	}
		inline_	const Matrix4x4&		GetWorldMatrix()	const	{ return mWorld;		}

				bool					Stab(const STABCREATE& stab_info);
				bool					Stab2(const STABCREATE& stab_info, CollisionFace& cf);
		private:
				MultiSurface*			mSourceMesh;	//!< Shortcut to source mesh
				ConsolidatedSurface*	mConsolidated;	//!< Consolidated mesh
		//
				Model*					mOPCODEModel;	//!< OPCODE's tree for our mesh
				MeshInterface			mIMesh;			//!< OPCODE's mesh interface

				Matrix4x4				mWorld;			//!< World matrix
		// Internal methods
				void					Release();
				bool					BuildOPCODEModel();
				bool					BuildConsolidated();
				bool					SetupColors(udword nb_colors, const Point* colors, float* opacity);

		friend	class					Prelighter;
	};

#endif // ICEPRELIGHTERMESH_H
