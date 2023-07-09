///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains a vertex prelighter built as a Flexporter plug-in.
 *	\file		FLX_VertexPrelighter.h
 *	\author		Pierre Terdiman
 *	\date		April, 25, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_VERTEXPRELIGHTER_H
#define FLX_VERTEXPRELIGHTER_H

#ifdef FLEXPORTER_SUPPORT_PRELIGHTER

	// We derive from ExportFormat so that we don't have to enumerate/convert the scene ourselves...

	class VertexPrelighter : public ExportFormat
	{
	public:
	// Constructor/Destructor
													VertexPrelighter();
		virtual										~VertexPrelighter();

	// Exporter init
		override(ExportFormat)	bool				Init(bool motion);

	// Main scene info
		override(ExportFormat)	bool				SetSceneInfo		(const MainDescriptor& maininfo);

	// Export loop
		override(ExportFormat)	bool				ExportCamera		(const CameraDescriptor& camera);
		override(ExportFormat)	bool				ExportController	(const ControllerDescriptor& controller)	{ return true;	}
		override(ExportFormat)	bool				ExportHelper		(const HelperDescriptor& helper)			{ return true;	}
		override(ExportFormat)	bool				ExportLight			(const LightDescriptor& light);
		override(ExportFormat)	bool				ExportMaterial		(const MaterialDescriptor& material);
		override(ExportFormat)	bool				ExportMesh			(const MeshDescriptor& mesh);
		override(ExportFormat)	bool				ExportMotion		(const MotionDescriptor& motion)			{ return true;	}
		override(ExportFormat)	bool				ExportShape			(const ShapeDescriptor& shape)				{ return true;	}
		override(ExportFormat)	bool				ExportSpaceWarp		(const SpaceWarpDescriptor& space_warp)		{ return true;	}
		override(ExportFormat)	bool				ExportTexture		(const TextureDescriptor& texture)			{ return true;	}

	// End of export notification
		override(ExportFormat)	bool				EndExport			(const StatsDescriptor& stats);

								ExportSettings*		OverrideSettings(ExportSettings* settings);

		inline_					void				SetSmoothing(udword smoothing)									{ mSmoothing = smoothing;	}
		inline_					void				SetShadows(bool shadows)										{ mShadows = shadows;		}
		inline_					void				SetSpecular(bool specular)										{ mSpecular = specular;		}
		inline_					void				SetClamping(bool clamping)										{ mClamp = clamping;		}

		// GUI overrides
		// WARNING: if you return false, Flexporter built-in versions are discarded
		override(ExportFormat)	bool				InitGUI();
		override(ExportFormat)	bool				SetStatus(const char* text);
		override(ExportFormat)	bool				SetExtStatus(const char* text);
		override(ExportFormat)	bool				Progress(int pos);
		override(ExportFormat)	bool				DisplayLogText(const char* text);
		override(ExportFormat)	bool				CloseGUI();

		private:
								Prelighter*			mPrelighter;
								Container			mSurfaces;
								Container			mNodes;
								Container			mObjects;
								Container			mParities;
								udword				mSmoothing;
								bool				mShadows;
								bool				mSpecular;
								bool				mClamp;
		// Internal methods
								bool				CreateModifier(INode* node, IndexedSurface* color_surface, bool parity);
	};
#endif

#endif // FLX_VERTEXPRELIGHTER_H
