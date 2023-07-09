///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the Void Format exporter plug-in for Flexporter.
 *	\file		VoidExporter.h
 *	\author		Pierre Terdiman
 *	\date		May, 6, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __VOIDEXPORTER_H__
#define __VOIDEXPORTER_H__

	class VoidFormat : public ExportFormat
	{
	public:
	// Constructor/Destructor
										VoidFormat();
		virtual							~VoidFormat();

	// Exporter init
		override(ExportFormat)	bool	Init(bool motion);

	// Main scene info
		override(ExportFormat)	bool	SetSceneInfo		(const MainDescriptor& maininfo);

	// Export loop
		override(ExportFormat)	bool	ExportCamera		(const CameraDescriptor& camera);
		override(ExportFormat)	bool	ExportController	(const ControllerDescriptor& controller);
		override(ExportFormat)	bool	ExportHelper		(const HelperDescriptor& helper);
		override(ExportFormat)	bool	ExportLight			(const LightDescriptor& light);
		override(ExportFormat)	bool	ExportMaterial		(const MaterialDescriptor& material);
		override(ExportFormat)	bool	ExportMesh			(const MeshDescriptor& mesh);
		override(ExportFormat)	bool	ExportMotion		(const MotionDescriptor& motion);
		override(ExportFormat)	bool	ExportShape			(const ShapeDescriptor& shape);
		override(ExportFormat)	bool	ExportTexture		(const TextureDescriptor& texture);
		override(ExportFormat)	bool	ExportSpaceWarp		(const SpaceWarpDescriptor& space_warp);

	// End of export notification
		override(ExportFormat)	bool	EndExport			(const StatsDescriptor& stats);

	private:
	};

#endif // __VOIDEXPORTER_H__
