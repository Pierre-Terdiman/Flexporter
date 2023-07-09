///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the ZCB Format exporter plug-in for Flexporter.
 *	\file		IceZCBExporter.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEZCBEXPORTER_H__
#define __ICEZCBEXPORTER_H__

	class ZCBFormat : public ExportFormat
	{
		public:
		// Constructor/Destructor
												ZCBFormat();
		virtual									~ZCBFormat();

		// Exporter init
		override(ExportFormat)	bool			Init(bool motion);

		// Main scene info
		override(ExportFormat)	bool			SetSceneInfo		(const MainDescriptor& maininfo);

		// Export loop
		override(ExportFormat)	bool			ExportCamera		(const CameraDescriptor& camera);
		override(ExportFormat)	bool			ExportController	(const ControllerDescriptor& controller);
		override(ExportFormat)	bool			ExportHelper		(const HelperDescriptor& helper);
		override(ExportFormat)	bool			ExportLight			(const LightDescriptor& light);
		override(ExportFormat)	bool			ExportMaterial		(const MaterialDescriptor& material);
		override(ExportFormat)	bool			ExportMesh			(const MeshDescriptor& mesh);
		override(ExportFormat)	bool			ExportMotion		(const MotionDescriptor& motion);
		override(ExportFormat)	bool			ExportShape			(const ShapeDescriptor& shape);
		override(ExportFormat)	bool			ExportTexture		(const TextureDescriptor& texture);
		override(ExportFormat)	bool			ExportSpaceWarp		(const SpaceWarpDescriptor& space_warp);

		// End of export notification
		override(ExportFormat)	bool			EndExport			(const StatsDescriptor& stats);

		// About box
		override(CustomFormat)	void			About();

								bool			SaveOptions();
								bool			LoadOptions();
		inline_					ZCBCompression	GetCompression()						const	{ return mCompression;		}
		inline_					void			SetCompression(ZCBCompression compress)			{ mCompression = compress;	}
		inline_					udword			GetNbBits()								const	{ return mNbBits;			}
		inline_					void			SetNbBits(udword nb_bits)						{ mNbBits = nb_bits;		}
		private:
		// Time info local copy
								MAXTimeInfo		mTimeInfo;
		// Export arrays
								CustomArray		mGeneral;
								CustomArray		mGeomObjects;
								CustomArray		mCameras;
								CustomArray		mLights;
								CustomArray		mShapes;
								CustomArray		mHelpers;
								CustomArray		mTexmaps;
								CustomArray		mMaterials;
								CustomArray		mControllers;
								CustomArray		mMotion;
		// Flags
								bool			mIsMotionFile;
		// ZCB format specific options
								ZCBCompression	mCompression;
								udword			mNbBits;
		// Internal methods
								bool			ExportFloats(const NakedPoint* points, udword nb_points, CustomArray& array, udword nb_bits, bool keep_last);
								bool			ExportBasicInfo(const ObjectDescriptor* obj, CustomArray* array);
								void			ExportTextureTransform(const TextureTransform& tt, CustomArray& array);
	};

#endif // __ICEZCBEXPORTER_H__
