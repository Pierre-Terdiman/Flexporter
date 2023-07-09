///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Main file for IceImageWork.dll.
 *	\file		IceImageWork.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEIMAGEWORK_H
#define ICEIMAGEWORK_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compilation messages
#if defined(ICEIMAGEWORK_EXPORTS)
	#pragma message("Compiling ICE ImageWork")
#elif !defined(ICEIMAGEWORK_EXPORTS)
	#pragma message("Using ICE ImageWork")
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Automatic linking
	#ifndef BAN_ICEIMAGEWORK_AUTOLINK
		#ifdef _DEBUG
			#pragma comment(lib, "IceImageWork_D.lib")
			#pragma comment(lib, "IML_D.LIB")
		#else
			#pragma comment(lib, "IceImageWork.lib")
			#pragma comment(lib, "IML.LIB")
		#endif
	#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Preprocessor

#ifndef ICE_NO_DLL
	#ifdef ICEIMAGEWORK_EXPORTS
		#define ICEIMAGEWORK_API		__declspec(dllexport)
	#else
		#define ICEIMAGEWORK_API		__declspec(dllimport)
	#endif
#else
		#define ICEIMAGEWORK_API
#endif

	#include "IceImageWorkSettings.h"

	// IML
	#ifndef IML_H
//		#include <IML\IML.h>
	#endif

//	using namespace IML;

	namespace IceImageWork
	{
		// Basic
		#ifndef	ICECOLORDEF_H
			#include "IceColorDef.h"
		#endif
		#ifndef	ICECOLOR_H
			#include "IceColor.h"
		#endif
		#ifndef	ICEPALETTE_H
			#include "IcePalette.h"
		#endif
		#ifndef	ICEPICTURE_H
			#include "IcePicture.h"
		#endif
		#ifndef	ICEFLOATPICTURE_H
			#include "IceFloatPicture.h"
		#endif
		#ifndef	ICEDDS_H
			#include "IceDDS.h"
		#endif
		#ifndef	ICECROPVALUES_H
			#include "IceCropValues.h"
		#endif

		// Advanced
		#ifndef	ICENEURALNETQUANTIZER_H
			#include "IceNeuralNetQuantizer.h"
		#endif

		// Texture synthesis
		#ifndef	ICETEXTURESYNTHESIS_H
			#include "IceTextureSynthesis.h"
		#endif

		// Formats
		#ifndef	ICEPICTUREIMPORTEXPORT_H
			#include "IcePictureImportExport.h"
		#endif
/*		#ifndef	ICEBMP_H
			#include "IceBMP.h"
		#endif
		#ifndef	ICEPCX_H
			#include "IcePCX.h"
		#endif
		#ifndef	ICETGA_H
			#include "IceTGA.h"
		#endif
*/
		// Texture quadtree
		#ifndef	ICETEXTUREQUADTREE_H
			#include "IceTextureQuadtree.h"
		#endif

		#ifndef	ICEIMAGEWORKAPI_H
			#include "IceImageWorkAPI.h"
		#endif
	}

#endif // ICEIMAGEWORK_H
