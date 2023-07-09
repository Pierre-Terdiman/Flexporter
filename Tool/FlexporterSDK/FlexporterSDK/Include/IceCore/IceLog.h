///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains log file code.
 *	\file		IceLog.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICELOG_H__
#define __ICELOG_H__

	ICECORE_API void		SetLogLimit(udword limit);

	ICECORE_API void		Log(ubyte value);
	ICECORE_API void		Log(uword value);
	ICECORE_API void		Log(udword value);
	ICECORE_API void		Log(float value);
	ICECORE_API void		Log(sbyte value);
	ICECORE_API void		Log(sword value);
	ICECORE_API void		Log(sdword value);
	ICECORE_API void		Log(const char* msg);
	ICECORE_API void		Log(LPSTR fmt, ...);
	ICECORE_API const char*	_F(LPSTR format, ...);

#endif // __ICELOG_H__