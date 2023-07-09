///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the ASCII Format exporter plug-in for Flexporter.
 *	\file		IceASCIIExporter.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEASCIIEXPORTER_H__
#define __ICEASCIIEXPORTER_H__

	class ASCIIFormat : public ExportFormat
	{
		public:
		// Constructor/Destructor
												ASCIIFormat();
		virtual									~ASCIIFormat();

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
		override(ExportFormat)	bool			ExportSpaceWarp		(const SpaceWarpDescriptor& space_warp);
		override(ExportFormat)	bool			ExportTexture		(const TextureDescriptor& texture);

		// End of export notification
		override(ExportFormat) bool				EndExport			(const StatsDescriptor& stats);

		// GUI overrides
		// WARNING: if you return false, Flexporter built-in versions are discarded
/*
		override(CustomFormat)	bool			InitGUI();
		override(CustomFormat)	bool			SetStatus(const char* text);
		override(CustomFormat)	bool			SetExtStatus(const char* text);
		override(CustomFormat)	bool			Progress(int pos);
		override(CustomFormat)	bool			DisplayLogText(const char* text);
		override(CustomFormat)	bool			CloseGUI();
*/

		// About box
		override(CustomFormat)	void			About();

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
								CustomArray		mSpaceWarps;
		// Flags
								bool			mIsMotionFile;
		// Internal methods
								bool			ExportBasicInfo(const ObjectDescriptor* obj, CustomArray* array);
								void			ExportTextureTransform(const TextureTransform& tt, CustomArray& array);
	};

#endif // __ICEASCIIEXPORTER_H__
