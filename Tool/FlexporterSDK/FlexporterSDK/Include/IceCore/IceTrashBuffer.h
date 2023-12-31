///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains trash-buffer related code.
 *	\file		IceTrashBuffer.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICETRASHBUFFER_H__
#define __ICETRASHBUFFER_H__

	// Trash buffer
	FUNCTION ICECORE_API void*	GetTrashBuffer(udword size, bool discardcontents=true);
	FUNCTION ICECORE_API void	ReleaseTrashBuffer();
	FUNCTION ICECORE_API udword	GetTrashBufferSize();

#endif // __ICETRASHBUFFER_H__
