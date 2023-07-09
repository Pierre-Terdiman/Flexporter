///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains code for vertex colors.
 *	\file		FLX_VertexColors.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 24, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

bool IceExporter::ApplyRandomColors(Object* node_object, TimeValue t)
{
	if(!node_object || !node_object->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))	return false;

	BOOL status;

	MAXMeshPointer MAXMesh(node_object, t);

	status = MAXMesh->setNumVertCol(MAXMesh->numVerts);
	status = MAXMesh->setNumVCFaces(MAXMesh->numFaces);

	for(int i=0;i<MAXMesh->numCVerts;i++)
	{
		MAXMesh->vertCol[i].x = UnitRandomFloat();
		MAXMesh->vertCol[i].y = UnitRandomFloat();
		MAXMesh->vertCol[i].z = UnitRandomFloat();
	}

	for(int i=0;i<MAXMesh->numFaces;i++)
	{
		MAXMesh->vcFace[i].t[0] = MAXMesh->faces[i].v[0];
		MAXMesh->vcFace[i].t[1] = MAXMesh->faces[i].v[1];
		MAXMesh->vcFace[i].t[2] = MAXMesh->faces[i].v[2];
	}

//	NotifyDependents(Interval(t, t), PART_VERTCOLOR, REFMSG_CHANGE);
//	NotifyDependents(Interval(t, t), PART_TOPO, REFMSG_CHANGE);
	node_object->UpdateValidity(VERT_COLOR_CHAN_NUM, FOREVER);

	return true;
}

bool IceExporter::SetVertexColors(Object* node_object, TimeValue t, IndexedSurface* color_surface, IndexedSurface* alpha_surface, bool parity)
{
	if(!color_surface)	return false;

	if(!node_object || !node_object->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))	return false;

	BOOL status;

	MAXMeshPointer MAXMesh(node_object, t);

	if(MAXMesh->numFaces!=color_surface->GetNbFaces())	return false;

	status = MAXMesh->setNumVertCol(color_surface->GetNbVerts());
	status = MAXMesh->setNumVCFaces(color_surface->GetNbFaces());

	for(int i=0;i<MAXMesh->numCVerts;i++)
	{
		MAXMesh->vertCol[i].x = color_surface->GetVertex(i)->x;
		MAXMesh->vertCol[i].y = color_surface->GetVertex(i)->y;
		MAXMesh->vertCol[i].z = color_surface->GetVertex(i)->z;
	}

	for(int i=0;i<MAXMesh->numFaces;i++)
	{
		MAXMesh->vcFace[i].t[0] = color_surface->GetFace(i)->mRef[0];
		MAXMesh->vcFace[i].t[1+parity] = color_surface->GetFace(i)->mRef[1];
		MAXMesh->vcFace[i].t[2-parity] = color_surface->GetFace(i)->mRef[2];
	}

//	NotifyDependents(Interval(t, t), PART_VERTCOLOR, REFMSG_CHANGE);
//	NotifyDependents(Interval(t, t), PART_TOPO, REFMSG_CHANGE);
//	node_object->UpdateValidity(VERT_COLOR_CHAN_NUM, FOREVER);
//	MAXMesh.GetObject()->UpdateValidity(VERT_COLOR_CHAN_NUM, FOREVER);
	MAXMesh.GetObject()->UpdateValidity(VERT_COLOR_CHAN_NUM, Interval(t,t));

	return true;
}
