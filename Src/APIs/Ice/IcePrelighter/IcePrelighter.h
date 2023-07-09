///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Main file for IcePrelighter.dll.
 *	\file		IcePrelighter.h
 *	\author		Pierre Terdiman
 *	\date		April, 19, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEPRELIGHTER_H
#define ICEPRELIGHTER_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compilation messages
#if defined(ICEPRELIGHTER_EXPORTS)
	#pragma message("Compiling ICE Prelighter")
#elif !defined(ICEPRELIGHTER_EXPORTS)
	#pragma message("Using ICE Prelighter")
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Automatic linking
	#ifndef BAN_ICEPRELIGHTER_AUTOLINK
		#ifdef _DEBUG
			#pragma comment(lib, "IcePrelighter_D.lib")
		#else
			#pragma comment(lib, "IcePrelighter.lib")
		#endif
	#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Preprocessor

#ifndef ICE_NO_DLL
	#ifdef ICEPRELIGHTER_EXPORTS
		#define ICEPRELIGHTER_API	__declspec(dllexport)
	#else
		#define ICEPRELIGHTER_API	__declspec(dllimport)
	#endif
#else
		#define ICEPRELIGHTER_API
#endif

	#include "IcePrelighterSettings.h"
//	#include "IcePrelighterErrors.h"

	// Includes
	namespace IcePrelighter
	{
		#include "IcePrelighterObject.h"
		#include "IcePrelighterLight.h"
		#include "IcePrelighterShader.h"
		#include "IcePrelighterMesh.h"
		#include "IcePrelighterMain.h"

		#include "IceSphericalHarmonics.h"

		#include "IcePrelighterAPI.h"
	}

#endif // ICEPRELIGHTER_H
