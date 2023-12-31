///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains code for the "CopyTextures" button.
 *	\file		FLX_TextureCopy.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_TEXTURECOPY_H
#define FLX_TEXTURECOPY_H

	// Texture-files copy
	void CopyTextures(INode* node, udword& nb_copied, udword& nb_not_found, udword& nb_failed, const char* dest_path);

#endif // FLX_TEXTURECOPY_H
