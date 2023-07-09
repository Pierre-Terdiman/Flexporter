///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains main prelighter code.
 *	\file		IcePrelighterMain.h
 *	\author		Pierre Terdiman
 *	\date		April, 20, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEPRELIGHTERMAIN_H
#define ICEPRELIGHTERMAIN_H

	enum PrelighterCallbackCode
	{
		PCC_INIT,
		PCC_INIT_MESH,
		PCC_END_MESH,
		PCC_MESH_PROGRESS,
		PCC_END,

		PCC_FORCE_DWORD	= 0x7fffffff
	};

	struct PrelighterCallbackParams
	{
	};

	struct PrelighterCallbackInit : PrelighterCallbackParams
	{
		udword	mNbObjects;
	};

	struct PrelighterCallbackInitMesh : PrelighterCallbackParams
	{
		const char*	mName;
		udword		mNbColors;
	};

	struct PrelighterCallbackMeshProgress : PrelighterCallbackParams
	{
		udword		mNbColorsDone;
	};

	typedef bool (*PrelighterCallback)	(PrelighterCallbackCode code, PrelighterCallbackParams* params, void* user_data);

	struct ICEPRELIGHTER_API PRELIGHTER_CREATE
	{
							PRELIGHTER_CREATE();

		PrelighterCallback	mCallback;
		void*				mUserData;
		udword				mSmoothing;
		bool				mShadows;
		bool				mMergeAll;
	};

	class ICEPRELIGHTER_API Prelighter
	{
		public:
		// Constructor/Destructor
									Prelighter();
		virtual						~Prelighter();

				PrelighterLight*	NewLight();
				PrelighterShader*	NewShader();
				PrelighterMesh*		NewMesh();

		//////

		inline_	udword				GetNbLights()							const	{ return mLights.GetNbEntries();					}
		inline_	PrelighterLight**	GetLights()								const	{ return (PrelighterLight**)mLights.GetEntries();	}

		inline_	udword				GetNbShaders()							const	{ return mShaders.GetNbEntries();					}
		inline_	PrelighterShader**	GetShaders()							const	{ return (PrelighterShader**)mShaders.GetEntries();	}

		inline_	udword				GetNbMeshes()							const	{ return mMeshes.GetNbEntries();					}
		inline_	PrelighterMesh**	GetMeshes()								const	{ return (PrelighterMesh**)mMeshes.GetEntries();	}

				PrelighterLight*	GetLight(sdword id)						const;
				PrelighterShader*	GetShader(sdword id)					const;
				PrelighterMesh*		GetMesh(sdword id)						const;

		//////

		inline_	const RGBAColor&	GetAmbientColor()						const	{ return mAmbientColor;		}
		inline_	void				SetAmbientColor(const RGBAColor& color)			{ mAmbientColor = color;	}

		inline_	const Point&		GetViewpoint()							const	{ return mViewpoint;		}
		inline_	void				SetViewpoint(const Point& p)					{ mViewpoint = p;			}

		//////

		virtual	bool				PrecomputeLighting(const PRELIGHTER_CREATE& create);

		protected:
				PrelighterCallback	mCallback;
				void*				mUserData;

				Container			mLights;
				Container			mShaders;
				Container			mMeshes;
		// Merging
				Container			mSaved;
				MultiSurface*		mMerged;			//!< Result of merging
		// Settings
				RGBAColor			mAmbientColor;		//!< Global ambient color
				Point				mViewpoint;			//!< Current viewpoint for specular terms
				MaterialProps		mDefaultMat;		//!< Default material used when MaterialID is invalid
				udword				mSmoothing;			//!< Color smoothing power

		// Internal methods
				bool				PrecomputeMeshLighting(PrelighterMesh* mesh, IlluminationModel& model);
				bool				ShadowFeeler(const Ray& world_ray, float max_dist)	const;
				bool				MergeAll();
				bool				UnmergeAll();
				bool				UnmergeSurface(SurfaceType type);
	};

#endif // ICEPRELIGHTERMAIN_H
