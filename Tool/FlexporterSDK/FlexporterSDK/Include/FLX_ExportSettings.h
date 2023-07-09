///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2003 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the exportation settings structure.
 *	\file		FLX_ExportSettings.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __FLX_EXPORTSETTINGS_H__
#define __FLX_EXPORTSETTINGS_H__

	//! Various compression schemes
/*	enum CompressionValues
	{
		FLX_COMPRESSION_NONE		= 0,
		FLX_COMPRESSION_ZLIB		= 1,
		FLX_COMPRESSION_BZIP2		= 2,

		FLX_COMPRESSION_FORCE_DWORD	= 0x7fffffff
	};
*/
	//! SetLocale() for printf
	enum LocalSettings
	{
		FLX_LOCALE_ENGLISH			= 0,
		FLX_LOCALE_FRENCH			= 1,

		FLX_LOCALE_FORCE_DWORD		= 0x7fffffff
	};

	//! Exportation settings. You can here customize the way Flexporter exports information from MAX.
	class FLEXPORTER_API ExportSettings
	{
		public:
		// Constructor / Destructor
							ExportSettings()
							{
//								mCompression				= FLX_COMPRESSION_NONE;
//								mNbBits						= 8;
								mExportWholeScene			= true;
								mExportVisTrack				= false;
								mExportHiddenNodes			= false;
								mAutoUnload					= false;
								mAutoLoad					= false;
								mGlobalScale				= 1.0f;
								mLocale						= FLX_LOCALE_ENGLISH;
								mExportPHYSIQUE				= true;
								mExportSKIN					= false;
								mExportBIPED				= false;
								mAlwaysExportMotion			= false;

								mExpUVW						= false;
								mDiscardW					= true;
								mExpVTXColor				= false;
								mUseSmgrp					= true;
								mRemoveScaling				= true;
								mConvertToD3D				= true;
								mAbsolutePRS				= false;
								mConvexHull					= false;
								mBoundingSphere				= false;
								mInertiaTensor				= false;
								mEdgeVis					= false;
								mMakeManifold				= false;
								mExpTargetNodes				= false;
								mDiscardInstances			= false;

								mConsolidateMesh			= false;
								mComputeFaceNormals			= false;
								mComputeVertexNormals		= false;
								mExportNormalInfo			= false;
								mWeightNormalsByArea		= false;
								mCacheOptimize				= false;

								mForceAmbient				= false;
								mForceDiffuse				= false;
								mForceSpecular				= false;
								mForceFilter				= false;
								mExpMtlCtrl					= false;
								mMaterialSharing			= false;

								mOpacityInAlpha				= true;
								mTexnameOnly				= false;
								mTexMaxSize					= 256;
								mKeepAmbientTexture			= false;
								mKeepDiffuseTexture			= true;
								mKeepSpecularTexture		= false;
								mKeepShininessTexture		= false;
								mKeepShiningStrengthTexture	= false;
								mKeepSelfIllumTexture		= false;
								mKeepOpacityTexture			= true;
								mKeepFilterTexture			= false;
								mKeepBumpTexture			= false;
								mKeepReflexionTexture		= false;
								mKeepRefractionTexture		= false;
								mKeepDisplacementTexture	= false;
								mFlipHorizontal				= false;
								mFlipVertical				= false;
								mQuantizeTextures			= false;
								mDiscardTextureAlpha		= false;

								mExpCameras					= true;
								mExpFOVCtrl					= false;
								mExpCamCtrl					= false;

								mExpLights					= true;
								mExpLitCtrl					= false;
								mExpProjMaps				= false;
								mComputeVtxColors			= false;
								mComputeShadows				= false;
								mColorSmoothing				= false;

								mSingleFrame				= true;
								mSampling					= true;
								mSamplingRate				= 1;

								mExpShapes					= false;
								mShapeToMesh				= false;
								mExpHelpers					= false;
								mExpSpaceWarps				= false;

								mExportIK					= false;
							}

							~ExportSettings()
							{
							}

		inline_	long		GetSamplingRate()	const		{ return mSamplingRate>0 ? mSamplingRate : 1;	}

		// General settings
//				udword		mCompression;					//!< Compression settings (see above)
//				long		mNbBits;						//!< Number of bits / quantized coordinate
				bool		mExportWholeScene;				//!< Export whole scene or single node
				bool		mExportVisTrack;				//!< Export visibility track
				bool		mExportHiddenNodes;				//!< Export hidden nodes or discard them
				bool		mAutoUnload;					//!< Automatically unload plug-ins after each export
				bool		mAutoLoad;						//!< Automatically load settings from the registry
				float		mGlobalScale;					//!< Global scaling factor
				udword		mLocale;						//!< Local settings
				bool		mExportPHYSIQUE;				//!< Export PHYSIQUE modifiers as characters (else as standard meshes)
				bool		mExportSKIN;					//!< Export SKIN modifiers as characters (else as standard meshes)
				bool		mExportBIPED;					//!< Export BIPED parts or discard them
				bool		mAlwaysExportMotion;			//!< Export motion data in main file
		// Meshes settings
		//		bool		mCreateStrips;					//!< Create triangle strips or not
				bool		mExpUVW;						//!< Export UVW mappings or discard them
				bool		mDiscardW;						//!< Discard W in UVW or keep it
				bool		mExpVTXColor;					//!< Export vertex colors or discard them
				bool		mUseSmgrp;						//!< Use smoothing groups or discard them
		//		bool		mExpNorm;						//!< Export normals or not
		//		bool		mLOD;							//!< Compute progressive meshes or not
				bool		mRemoveScaling;					//!< Remove non-uniform scaling of all nodes
				bool		mConvertToD3D;					//!< Convert from MAX frame to D3D frame
				bool		mAbsolutePRS;					//!< Export absolute PRS values (else relative to parent)
				bool		mConvexHull;					//!< Flexporter can compute the convex hull for you
				bool		mBoundingSphere;				//!< Flexporter can compute the bounding sphere for you
				bool		mInertiaTensor;					//!< Flexporter can compute the inertia tensor for you
				bool		mEdgeVis;						//!< Export edge visibility
				bool		mMakeManifold;					//!< Make manifold mesh (remove singular edges)
				bool		mExpTargetNodes;				//!< Export target nodes or discard them
				bool		mDiscardInstances;				//!< Export instances as real meshes
		// Consolidation settings
				bool		mConsolidateMesh;				//!< Flexporter can rebuild a clean mesh for you
				bool		mComputeFaceNormals;			//!< Flexporter can compute and export face normals
				bool		mComputeVertexNormals;			//!< Flexporter can compute and export vertex normals
				bool		mExportNormalInfo;				//!< Export NormalInfo field or not
				bool		mWeightNormalsByArea;			//!< Take face areas into account in normal computation
				bool		mCacheOptimize;					//!< Optimize consolidation for cache coherency
		// Material settings
				bool		mForceAmbient;					//!< Force material ambient color to pure white.
				bool		mForceDiffuse;					//!< Force material diffuse color to pure white.
				bool		mForceSpecular;					//!< Force material specular color to pure white.
				bool		mForceFilter;					//!< Force material filter color to black.
				bool		mExpMtlCtrl;					//!< Export material controllers or discard them
				bool		mMaterialSharing;				//!< Try to share materials accross objects (by discarding its name)
		// Texture settings
				bool		mOpacityInAlpha;				//!< Pack the opacity map in the diffuse map alpha channel
				bool		mTexnameOnly;					//!< Only reports the texture's name, not the pixel buffer
				udword		mTexMaxSize;					//!< Texture maximum size
				bool		mKeepAmbientTexture;			//!< Export ambient textures, or discard them
				bool		mKeepDiffuseTexture;			//!< Export diffuse textures, or discard them
				bool		mKeepSpecularTexture;			//!< Export specular textures, or discard them
				bool		mKeepShininessTexture;			//!< Export shininess textures, or discard them
				bool		mKeepShiningStrengthTexture;	//!< Export shining strength textures, or discard them
				bool		mKeepSelfIllumTexture;			//!< Export self-illum textures, or discard them
				bool		mKeepOpacityTexture;			//!< Export opacity textures, or discard them
				bool		mKeepFilterTexture;				//!< Export filter textures, or discard them
				bool		mKeepBumpTexture;				//!< Export bump textures, or discard them
				bool		mKeepReflexionTexture;			//!< Export reflexion textures, or discard them
				bool		mKeepRefractionTexture;			//!< Export refraction textures, or discard them
				bool		mKeepDisplacementTexture;		//!< Export displacement textures, or discard them
				bool		mFlipHorizontal;				//!< Flip texture horizontally before exporting
				bool		mFlipVertical;					//!< Flip texture vertically before exporting
				bool		mQuantizeTextures;				//!< Convert to 256-colors textures
				bool		mDiscardTextureAlpha;			//!< Discard texture's alpha channel
		// Camera settings
				bool		mExpCameras;					//!< Export cameras or discard them
				bool		mExpFOVCtrl;					//!< Export FOV controller or discard them
				bool		mExpCamCtrl;					//!< Export cameras attributes controllers or discard them
		// Light settings
				bool		mExpLights;						//!< Export lights or discard them
				bool		mExpLitCtrl;					//!< Export lights attributes controllers or discard them
				bool		mExpProjMaps;					//!< Export projector maps or discard them
				bool		mComputeVtxColors;				//!< Pack lighting in vertex colors
				bool		mComputeShadows;				//!< Compute shadows or not
				bool		mColorSmoothing;				//!< Extra color smoothing pass (reduce aliasing)
		// Animation settings
				bool		mSingleFrame;					//!< Export a single frame
				bool		mSampling;						//!< Export samples (true) or keyframes (false)
				long		mSamplingRate;					//!< Sampling rate
		// Shape settings
				bool		mExpShapes;						//!< Export shapes or discard them
				bool		mShapeToMesh;					//!< Try to convert shapes to meshes
		// Helper settings
				bool		mExpHelpers;					//!< Export helpers or discard them
		// Space warps settings
				bool		mExpSpaceWarps;					//!< Export space warp objects or discard them
		// IK settings
				bool		mExportIK;						//!< Export IK data or not
	};

	//! Settings enabled/disabled status
	class FLEXPORTER_API Enabled
	{
		public:
		// Constructor / Destructor
							Enabled()
							{
								// Default: everything is enabled
//								mCompression				= true;
								mExportWholeScene			= true;
								mExportVisTrack				= true;
								mExportHiddenNodes			= true;
								mAutoUnload					= true;
								mAutoLoad					= true;
								mGlobalScale				= true;
								mLocale						= true;
								mExportPHYSIQUE				= true;
								mExportSKIN					= true;
								mExportBIPED				= true;
								mAlwaysExportMotion			= true;

								mExpUVW						= true;
								mDiscardW					= true;
								mExpVTXColor				= true;
								mUseSmgrp					= true;
								mRemoveScaling				= true;
								mConvertToD3D				= true;
								mAbsolutePRS				= true;
								mConvexHull					= true;
								mBoundingSphere				= true;
								mInertiaTensor				= true;
								mEdgeVis					= true;
								mMakeManifold				= true;
								mExpTargetNodes				= true;
								mDiscardInstances			= true;

								mConsolidateMesh			= true;
								mComputeFaceNormals			= true;
								mComputeVertexNormals		= true;
								mExportNormalInfo			= true;
								mWeightNormalsByArea		= true;
								mCacheOptimize				= true;

								mForceAmbient				= true;
								mForceDiffuse				= true;
								mForceSpecular				= true;
								mForceFilter				= true;
								mExpMtlCtrl					= true;
								mMaterialSharing			= true;

								mOpacityInAlpha				= true;
								mTexnameOnly				= true;
								mTexMaxSize					= true;
								mKeepAmbientTexture			= true;
								mKeepDiffuseTexture			= true;
								mKeepSpecularTexture		= true;
								mKeepShininessTexture		= true;
								mKeepShiningStrengthTexture	= true;
								mKeepSelfIllumTexture		= true;
								mKeepOpacityTexture			= true;
								mKeepFilterTexture			= true;
								mKeepBumpTexture			= true;
								mKeepReflexionTexture		= true;
								mKeepRefractionTexture		= true;
								mKeepDisplacementTexture	= true;
								mFlipHorizontal				= true;
								mFlipVertical				= true;
								mQuantizeTextures			= true;
								mDiscardTextureAlpha		= true;

								mExpCameras					= true;
								mExpFOVCtrl					= true;
								mExpCamCtrl					= true;

								mExpLights					= true;
								mExpLitCtrl					= true;
								mExpProjMaps				= true;
								mComputeVtxColors			= true;
								mComputeShadows				= true;
								mColorSmoothing				= true;

								mSingleFrame				= true;
								mSampling					= true;
								mSamplingRate				= true;

								mExpShapes					= true;
								mShapeToMesh				= true;
								mExpHelpers					= true;
								mExpSpaceWarps				= true;

								mExportIK					= true;
							}

							~Enabled()
							{
							}

		// General permissions
//				bool		mCompression;					//!< Enable/disable compression
				bool		mExportWholeScene;				//!< ...
				bool		mExportVisTrack;				//!< 
				bool		mExportHiddenNodes;				//!< 
				bool		mAutoUnload;					//!<
				bool		mAutoLoad;						//!<
				bool		mGlobalScale;					//!<
				bool		mLocale;						//!<
				bool		mExportPHYSIQUE;				//!< 
				bool		mExportSKIN;					//!<
				bool		mExportBIPED;					//!< 
				bool		mAlwaysExportMotion;			//!< 
		// Meshes permissions
				bool		mExpUVW;						//!< 
				bool		mDiscardW;						//!< 
				bool		mExpVTXColor;					//!< 
				bool		mUseSmgrp;						//!< 
				bool		mRemoveScaling;					//!< 
				bool		mConvertToD3D;					//!< 
				bool		mAbsolutePRS;					//!< 
				bool		mConvexHull;					//!< 
				bool		mBoundingSphere;				//!< 
				bool		mInertiaTensor;					//!< 
				bool		mEdgeVis;						//!< 
				bool		mMakeManifold;					//!< 
				bool		mExpTargetNodes;				//!< 
				bool		mDiscardInstances;				//!<
		// Consolidation permissions
				bool		mConsolidateMesh;				//!< 
				bool		mComputeFaceNormals;			//!< 
				bool		mComputeVertexNormals;			//!< 
				bool		mExportNormalInfo;				//!< 
				bool		mWeightNormalsByArea;			//!< 
				bool		mCacheOptimize;					//!< 
		// Material permissions
				bool		mForceAmbient;					//!< 
				bool		mForceDiffuse;					//!< 
				bool		mForceSpecular;					//!< 
				bool		mForceFilter;					//!< 
				bool		mExpMtlCtrl;					//!<
				bool		mMaterialSharing;				//!<
		// Texture permissions
				bool		mOpacityInAlpha;				//!< 
				bool		mTexnameOnly;					//!< 
				bool		mTexMaxSize;					//!< 
				bool		mKeepAmbientTexture;			//!< 
				bool		mKeepDiffuseTexture;			//!< 
				bool		mKeepSpecularTexture;			//!< 
				bool		mKeepShininessTexture;			//!< 
				bool		mKeepShiningStrengthTexture;	//!< 
				bool		mKeepSelfIllumTexture;			//!< 
				bool		mKeepOpacityTexture;			//!< 
				bool		mKeepFilterTexture;				//!< 
				bool		mKeepBumpTexture;				//!< 
				bool		mKeepReflexionTexture;			//!< 
				bool		mKeepRefractionTexture;			//!< 
				bool		mKeepDisplacementTexture;		//!< 
				bool		mFlipHorizontal;				//!< 
				bool		mFlipVertical;					//!< 
				bool		mQuantizeTextures;				//!< 
				bool		mDiscardTextureAlpha;			//!< 
		// Camera permissions
				bool		mExpCameras;					//!< 
				bool		mExpFOVCtrl;					//!< 
				bool		mExpCamCtrl;					//!<
		// Light permissions
				bool		mExpLights;						//!< 
				bool		mExpLitCtrl;					//!<
				bool		mExpProjMaps;					//!<
				bool		mComputeVtxColors;				//!< 
				bool		mComputeShadows;				//!< 
				bool		mColorSmoothing;				//!< 
		// Animation permissions
				bool		mSingleFrame;					//!< 
				bool		mSampling;						//!< 
				bool		mSamplingRate;					//!< 
		// Shape permissions
				bool		mExpShapes;						//!< 
				bool		mShapeToMesh;					//!<
		// Helper permissions
				bool		mExpHelpers;					//!< 
		// Space warps permissions
				bool		mExpSpaceWarps;					//!< 
		// IK permissions
				bool		mExportIK;						//!< 
	};

#endif // __FLX_EXPORTSETTINGS_H__
