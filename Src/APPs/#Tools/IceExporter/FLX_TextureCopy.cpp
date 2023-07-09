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
 *	\file		FLX_TextureCopy.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Copies textures used by a texmap to a destination directory.
 *	\param		tmap			[in] MAX texmap
 *	\param		nb_copied		[out] number of copied textures
 *	\param		nb_not_found	[out] number of textures not found
 *	\param		nb_failed		[out] number of failed texture copy operations
 *	\param		dest_path		[in] destination path
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void CopyTexture(Texmap* tmap, udword& nb_copied, udword& nb_not_found, udword& nb_failed, const char* dest_path)
{
	// Check that's a bitmap texmap
	if(IsKnownTexmap(tmap)==TEXMAP_BITMAP)
	{
		// Get texmap name
		BitmapTex* BTex	= (BitmapTex*)tmap;
		char* MapName = (char*)BTex->GetMapName();

		char NewPath[1024];
		strcpy(NewPath, dest_path);
		char* Name = strrchr(MapName, '\\');
		if(Name)	strcat(NewPath, Name);
		else		strcat(NewPath, MapName);
		if(FileExists((const char*)NewPath))	{ return;					}	// already copied
		if(!FileExists((const char*)MapName))	{ nb_not_found++;	return;	}	// file not found

/*		if(strlen(MapName)>=MAX_PATH || strlen(NewPath)>=MAX_PATH)
		{
			IceCore::MessageBox(null, "Path is too long", "Oops", MB_OK);
		}
*/
		if(CopyFile(MapName, NewPath, TRUE))	nb_copied++;
		else									nb_failed++;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Copies textures used by a material to a destination directory.
 *	\param		material		[in] MAX material
 *	\param		nb_copied		[out] number of copied textures
 *	\param		nb_not_found	[out] number of textures not found
 *	\param		nb_failed		[out] number of failed texture copy operations
 *	\param		dest_path		[in] destination path
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void CheckTexture(Mtl* material, udword& nb_copied, udword& nb_not_found, udword& nb_failed, const char* dest_path)
{
	if(material && dest_path)
	{
		if(material->ClassID()==Class_ID(DMTL_CLASS_ID, 0))
		{
			CopyTexture(material->GetSubTexmap(ID_AM), nb_copied, nb_not_found, nb_failed, dest_path);
			CopyTexture(material->GetSubTexmap(ID_DI), nb_copied, nb_not_found, nb_failed, dest_path);
			CopyTexture(material->GetSubTexmap(ID_SP), nb_copied, nb_not_found, nb_failed, dest_path);
			CopyTexture(material->GetSubTexmap(ID_SH), nb_copied, nb_not_found, nb_failed, dest_path);
			CopyTexture(material->GetSubTexmap(ID_SS), nb_copied, nb_not_found, nb_failed, dest_path);
			CopyTexture(material->GetSubTexmap(ID_SI), nb_copied, nb_not_found, nb_failed, dest_path);
			CopyTexture(material->GetSubTexmap(ID_OP), nb_copied, nb_not_found, nb_failed, dest_path);
			CopyTexture(material->GetSubTexmap(ID_FI), nb_copied, nb_not_found, nb_failed, dest_path);
			CopyTexture(material->GetSubTexmap(ID_BU), nb_copied, nb_not_found, nb_failed, dest_path);
			CopyTexture(material->GetSubTexmap(ID_RL), nb_copied, nb_not_found, nb_failed, dest_path);
			CopyTexture(material->GetSubTexmap(ID_RR), nb_copied, nb_not_found, nb_failed, dest_path);
		}
		else if(material->ClassID()==Class_ID(MULTI_CLASS_ID, 0))
		{
			udword NbSubMtls = (udword)material->NumSubMtls();
			for(udword i=0;i<NbSubMtls;i++)
			{
				Mtl* SubMat = material->GetSubMtl(i);
				CheckTexture(SubMat, nb_copied, nb_not_found, nb_failed, dest_path);
			}
		}
		// ClassID is unknown
	}
	// No material defined
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Copies textures used by a node to a destination directory.
 *	\param		node			[in] MAX node
 *	\param		nb_copied		[out] number of copied textures
 *	\param		nb_not_found	[out] number of textures not found
 *	\param		nb_failed		[out] number of failed texture copy operations
 *	\param		dest_path		[in] destination path
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IceExporter::CopyTextures(INode* node, udword& nb_copied, udword& nb_not_found, udword& nb_failed, const char* dest_path)
{
	// Checkings
	if(!dest_path)	return;

	// Copy textures from current node
	CheckTexture(node->GetMtl(), nb_copied, nb_not_found, nb_failed, dest_path);

	// Recurse through children
	for(udword i=0;i<(udword)node->NumberOfChildren();i++) CopyTextures(node->GetChildNode(i), nb_copied, nb_not_found, nb_failed, dest_path);
}
