///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains all registry-related code.
 *	\file		IceRegistry.h
 *	\author		Pierre Terdiman
 *	\date		May, 8, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEREGISTRY_H__
#define __ICEREGISTRY_H__

	ICECORE_API bool	Save(const char* appname, const char* folder, const char* paramname, udword param);
	ICECORE_API bool	Save(const char* appname, const char* folder, const char* paramname, const String& param);
	ICECORE_API bool	Save(const char* appname, const char* folder, const char* paramname, ubyte* param, udword size);

	ICECORE_API bool	Load(const char* appname, const char* folder, const char* paramname, udword& param);
	ICECORE_API bool	Load(const char* appname, const char* folder, const char* paramname, String& param);
	ICECORE_API bool	Load(const char* appname, const char* folder, const char* paramname, ubyte* param, udword& size);

#endif // __ICEREGISTRY_H__