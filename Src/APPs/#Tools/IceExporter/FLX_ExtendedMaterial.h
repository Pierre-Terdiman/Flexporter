///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file deals with extended custom materials.
 *	\file		FLX_ExtendedMaterial.h
 *	\author		Pierre Terdiman
 *	\date		April, 16, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef FLX_EXTENDEDMATERIAL_H
#define FLX_EXTENDEDMATERIAL_H

	// Flexporter material management
	bool	AddFlexporterMaterialTo(INode* node);
	bool	RemoveFlexporterMaterialFrom(INode* node);

	// Flexporter material access
	bool	GetShaderFile(Mtl* material, String& shader_file);
	bool	GetUserProps(Mtl* material, String& user_props);
	Texmap*	GetDecalTexture(Mtl* material);
	Texmap* GetDetailTexture(Mtl* material);
	Texmap* GetBillboardTexture(Mtl* material);

#endif // FLX_EXTENDEDMATERIAL_H
