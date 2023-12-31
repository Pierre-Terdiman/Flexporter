///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

#ifdef COMPILE_VERTEX_LIGHTER

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





VertexLighterLight::VertexLighterLight() :
	mLightDescriptor(null),
	mCastShadows(FALSE)
{
	mPosition.Zero();
}

VertexLighterLight::~VertexLighterLight()
{
	// It is quite unclear whether we own the LightDescriptor or not. They seem to
	// delete them in EvalCol.cpp so let's do that.
	DELETESINGLE(mLightDescriptor);
}

bool VertexLighterLight::Build(INode* node)
{
#ifdef TOFIX
#endif
	return true;
}

BOOL VertexLighterLight::GetLightingContribution(Point& lighting_sample, Point& specular_sample, Point& lighting_direction, const Point& world_point, const Point& world_normal) const
{
#ifdef TOFIX
#endif
	return TRUE;
}

BOOL VertexLighterLight::Exclude(INode* max_node) const
{
	if(mLightDescriptor)
	{
		ExclList* exclusion_list = mLightDescriptor->GetExclList();

		if(exclusion_list)
		{
			if(-1 != exclusion_list->FindNode(max_node))	// let's just hope the MAX implementation is not O(n)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

#endif