///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code to play AVI files.
 *	\file		IceAVI.h
 *	\author		Pierre Terdiman
 *	\date		April, 11, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEAVI_H__
#define __ICEAVI_H__

	FUNCTION ICECORE_API	bool			OpenAVI(const char* filename);
	FUNCTION ICECORE_API	const ubyte*	GrabAVIFrame(int frame);
	FUNCTION ICECORE_API	bool			CloseAVI();

#endif // __ICEAVI_H__
