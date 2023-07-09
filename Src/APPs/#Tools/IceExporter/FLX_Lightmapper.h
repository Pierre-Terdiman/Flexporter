///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_LIGHTMAPPER_H
#define FLX_LIGHTMAPPER_H

#ifdef SUPPORT_OLD_LIGHTMAPPER

	class LM_Material
	{
		public:
						LM_Material();
						~LM_Material();

				Point	mAmbientColor;
				Point	mDiffuseColor;
	};

	class LM_Light
	{
		public:
						LM_Light();
						~LM_Light();

				Point	mPosition;
				Point	mColor;
	};

	class LM_Mesh
	{
	public:
						LM_Mesh();
						~LM_Mesh();

			udword		mID;

			Point		mPosition;
			IceMaths::Quat		mRotation;
			Point		mScale;

			udword		mNbFaces;
			udword		mNbVerts;
			Point*		mVerts;
			udword*		mFaces;

			AABB		mAABB;

			Point*		mFNormals;
			Point*		mVNormals;

			Point*		mColors;

			udword		mNbColors;
			Point*		mColors2;
	};

	class Lightmapper : public ExportFormat
	{
	public:
	// Constructor/Destructor
										Lightmapper();
		virtual							~Lightmapper();

	// Exporter init
		virtual	bool					Init(bool motion);

	// Main scene info
		virtual bool					SetSceneInfo		(const MainDescriptor& maininfo);

	// Export loop
		virtual bool					ExportCamera		(const CameraDescriptor& camera);
		virtual bool					ExportController	(const ControllerDescriptor& controller);
		virtual bool					ExportHelper		(const HelperDescriptor& helper);
		virtual bool					ExportLight			(const LightDescriptor& light);
		virtual bool					ExportMaterial		(const MaterialDescriptor& material);
		virtual bool					ExportMesh			(const MeshDescriptor& mesh);
		virtual bool					ExportMotion		(const MotionDescriptor& motion);
		virtual bool					ExportShape			(const ShapeDescriptor& shape);
		virtual bool					ExportTexture		(const TextureDescriptor& texture);
		virtual	bool					ExportSpaceWarp		(const SpaceWarpDescriptor& space_warp){ return false;}

	// End of export notification
		virtual bool					EndExport			(const StatsDescriptor& stats);

				bool					StabFaces(Point& orig, Point& dir, bool culling, LM_Mesh* discarded, float dist);

				ExportSettings*			OverrideSettings(ExportSettings* settings);

				LM_Mesh*				GetMesh(udword id);

				udword					GetNbMeshes()			{ return mMeshes.GetNbEntries();							}
				LM_Mesh**				GetMeshes()				{ return (LM_Mesh**)mMeshes.GetEntries();					}
//				MESHBUILDERRESULT**		GetCleanMeshes()		{ return (MESHBUILDERRESULT**)mCleanMeshes.GetEntries();	}
				MBResult**				GetCleanMeshes()		{ return (MBResult**)mCleanMeshes.GetEntries();	}

				udword					GetNbLights()			{ return mLights.GetNbEntries();							}
				LM_Light**				GetLights()				{ return (LM_Light**)mLights.GetEntries();					}

				udword					GetNbMaterials()		{ return mMaterials.GetNbEntries();							}
				LM_Material**			GetMaterials()			{ return (LM_Material**)mMaterials.GetEntries();			}
	private:
				Point					mAmbientLight;

				Container				mMaterials;
				Container				mLights;
				Container				mMeshes;
				Container				mCleanMeshes;
	};
#endif


#endif // FLX_LIGHTMAPPER_H
