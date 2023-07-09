///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
- could be made MAX indep
- "light capter", light sampler
- DIY lighting
*/

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
VertexLighter::VertexLighter() :
	mStabbedTriangleIndex(0),
	mCurrentTime(0),
	mCacheHitCount(0),
	mValidLightArray(null),
	mConvertedNodeCount(0)
{
	mCurrentLighter = this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VertexLighter::~VertexLighter()
{
	// Free all meshes
	for(udword i=0;i<GetMeshCount();i++)
	{
		VertexLighterMesh* CurrentMesh = GetMesh(i);
		DELETESINGLE(CurrentMesh);
	}

	// Free all lights
	for(i=0;i<GetLightCount();i++)
	{
		VertexLighterLight* CurrentLight = GetLight(i);
		DELETESINGLE(CurrentLight);
	}

	// Free helpers
	DELETEARRAY(mValidLightArray);

	mSettings.MAXInterface = null;	// t'was just a shortcut
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Callback used when conversion starts.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VertexLighter::StartConversion()
{
	mConvertedNodeCount = 0;

	if(mSettings.MAXInterface)
	{
		mCurrentTime = mSettings.MAXInterface->GetTime();	// get current frame number
	}

//	Clock.GetCurrentTime( ClockTime );

//	ResetTimes();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Callback used when conversion ends.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VertexLighter::EndConversion()
{
	if(mSettings.DisplayProfile)
	{
	}

	if(mSettings.RedrawOnExit && mSettings.MAXInterface)
	{
//		mSettings.MAXInterface->RedrawViews( MAXInterface->GetTime() );    // why doesn't this work ?
		mSettings.MAXInterface->ForceCompleteRedraw();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Main conversion method.
 *	\param		settings	[in] conversion settings
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool VertexLighter::Convert(const VertexLighterSettings& settings)
{
	// 1) Initialize things
	if(!settings.MAXInterface)	return false;

	// Keep track of settings - using a struct is made on purpose so that we can
	// copy the whole thing safely (without forgetting some newcomer)
	mSettings	= settings;
	mInterface	= settings.MAXInterface;

	// 2) Start callback
	StartConversion();

	// 3) Find relevant nodes

	// 3.1) Start by enumerating all lights, so that:
	// - they're taken into account regardless of the user selection
	// - we can early exit when there's no light in the scene
	_ConvertNode(mSettings.MAXInterface->GetRootNode(), CVF_LIGHT);

	if(GetLightCount())
	{
		// 3.2) Now find remaining nodes
		if(!mSettings.ApplyToSelected)
		{
			// Enumerates & converts the whole scene without lights
			_ConvertNode(mSettings.MAXInterface->GetRootNode(), CVF_ALL & (~CVF_LIGHT));
		}
		else
		{
			// Alternative : convert selected nodes only (we could also display the selection panel)
			// (or display it when selection is empty)
			for(udword i=0;i<mSettings.MAXInterface->GetSelNodeCount();i++)
			{
				// Convert everything but the lights
				ConvertNode(mSettings.MAXInterface->GetSelNode(i), CVF_ALL & (~CVF_LIGHT));
			}
		}

		// 3.3. Check there's someone out there
		if(!GetMeshCount())
		{
			IceCore::MessageBox(null, "No mesh to light with current scene/settings - precomputation aborted.", "Oops", MB_OK);
			return true;
		}

		// 3.4. Actual merge
		for(udword i=0;i<GetMeshCount();i++)
		{
			if(!GetMesh(i)->Merge(GetCurrentTime()))
			{
			}
		}

		// 4. Compute lighting
		if(!UsesCache())
		{
			for(udword i=0;i<GetMeshCount();i++)
			{
				if(!GetMesh(i)->Light())
				{
					// User pressed cancel => abort everything
					return false;
				}
			}
		}
		else
		{
			for(udword i=0;i<GetMeshCount();i++)
			{
				if(!GetMesh(i)->Light2())
				{
					// User pressed cancel => abort everything
					return false;
				}
			}
		}

		// 5. Create all modifiers
		for(i=0;i<GetMeshCount();i++)
		{
			if(!GetMesh(i)->ApplyModifiers())
			{
				// User pressed cancel => abort everything
				// Nevertheless, already created modifiers are NOT removed.
				return false;
			}
		}

		// 6. End callback
		EndConversion();
	}
	else
	{
		IceCore::MessageBox(null, "There's no light in that scene - precomputation aborted.", "Oops", MB_OK);
	}
	return true;
}

// This is the main recursive conversion method.
// You should call it once for the root node and let it be...
void VertexLighter::_ConvertNode(INode* current_node, udword conversion_mask)
{
	// Checkings
	if(!current_node)	return;

	ConvertNode(current_node, conversion_mask);

	// Go down deep into the pain: recurse through children..
	for(udword i=0;i<current_node->NumberOfChildren();i++)
	{
		_ConvertNode(current_node->GetChildNode(i), conversion_mask);
	}
}

// Converts a given node
void VertexLighter::ConvertNode(INode* current_node, udword conversion_mask)
{
	Object* node_object = null;

	// Checkings
	if(!current_node)	return;

	// Update stats
	mConvertedNodeCount++;

	// Check the hidden status
	int is_hidden = current_node->IsHidden();

	// Catch the object if it's visible
	if(!is_hidden)	node_object = current_node->GetObjectRef();

	// Convert if needed
	if(node_object)
	{
		switch(node_object->SuperClassID())
		{
			case GEOMOBJECT_CLASS_ID :
				if(conversion_mask & CVF_GEOM_OBJECT)
				{
					ConvertGeomObject(current_node);
				}
				break;

			case GEN_DERIVOB_CLASS_ID:
				if(conversion_mask & CVF_DERIVED_OBJECT)
				{
					ConvertDerivedObject(current_node);
				}
				break;

			case CAMERA_CLASS_ID :
				break;

			case LIGHT_CLASS_ID:
				if(conversion_mask & CVF_LIGHT)
				{
					ConvertLight(current_node);
				}
				break;

			case SHAPE_CLASS_ID:
				break;

			case HELPER_CLASS_ID:
				break;

			default:
				// Unknown node
				break;
		}
	}
	else
	{
		// Only the root node should have no object.
	}
}

bool VertexLighter::ConvertGeomObject(INode* node, Object* derived_object)
{
	// Get right object: it can be the real node or a collapsed derived object
	Object* node_object = derived_object ? derived_object : node->GetObjectRef();

	// Check target state
	// GeomObjects are:
	// - all basic primitives
	// - camera target
	// - spot target
	// - directional target
	if(node_object->ClassID()==Class_ID(TARGET_CLASS_ID, 0))
	{
		// Here we have a target: camera, spot or directional.
		// Hence we do not have to convert it to a tri-mesh.
		return true;
	}

	// Convert geometric object
	if(node_object->CanConvertToType(triObjectClassID))
	{
		if(UsesMerge())
		{
			// Merge all meshes in a single one
			if(GetMeshCount())
			{
				GetMesh(0)->BuildMesh(node_object, node);
			}
			else
			{
				// That's the first mesh
				AddMesh(node, node_object);
			}
		}
		else
		{
			// Create a new mesh for each incoming MAX node
			AddMesh(node, node_object);
		}
	}
	return true;
}

bool VertexLighter::AddMesh(INode* node, Object* node_object)
{
	VertexLighterMesh* NewMesh = new VertexLighterMesh;

	if(NewMesh->BuildMesh(node_object, node))
	{
		mMeshNodeTable.AddObject(NewMesh);
		return true;
	}
	else
	{
		DELETESINGLE(NewMesh);
		return false;
	}
}

bool VertexLighter::ConvertDerivedObject(INode* node)
{
	// Here, look for known derived objects (Physique, Skin, etc)

	// Here, Physique/Skin modifier hasn't been found (or is not wanted). The object is probably a non-collapsed GeomObject.
	// We collapse it ourselves by calling EvalWorldState().

	ObjectState os = node->EvalWorldState(mCurrentTime);	// collapse the object...
	Object* node_object = os.obj;							// ...and get it back

	switch(node_object->SuperClassID())
	{
		case GEOMOBJECT_CLASS_ID :
		// Object is now a true GeomObject, we can try to convert it.
		if(node_object->CanConvertToType(triObjectClassID))
		{
			return ConvertGeomObject(node, node_object);
		}
		break;

		case GEN_DERIVOB_CLASS_ID:
		break;

		case CAMERA_CLASS_ID :
		break;

		case LIGHT_CLASS_ID:
		break;

		case SHAPE_CLASS_ID:
		break;

		case HELPER_CLASS_ID:
		break;

		default:
		break;
	}

	// One more unknown object here
	return true;
}

bool VertexLighter::ConvertLight(INode* node)
{
	VertexLighterLight* NewLight = new VertexLighterLight;

	if(NewLight->Build(node))
	{
		mLightNodeTable.AddObject(NewLight);
		return true;
	}
	else
	{
		DELETESINGLE(NewLight);
		return false;
	}
}

BOOL VertexLighter::IsVisibleFromLight(const Point& world_point, const Point& world_normal, const VertexLighterLight* current_light, const VertexLighterMesh* discarded, BOOL use_cache) const
{
#ifdef TOFIX
    GEOMETRIC_RAY
        world_ray;
    COUNT32
        mesh_count = GetMeshCount();
    COMPUTE_VERTEX_DATA_MESH
        ** meshes = GetMeshTable();
    const GEOMETRIC_MESH
        * geometric_mesh;
    COMPUTE_VERTEX_DATA_MESH
        * current_mesh;

    world_ray.Direction = current_light->GetPosition() - world_point;    // too bad it gets computed again in Illuminate()!
    world_ray.Direction.Normalize();

    if ( discarded )
    {
        world_ray.Origin = world_point;
    }
    else
    {
        world_ray.Origin = world_normal;    // :TODO: check it's always correctly oriented!
//      world_ray.Origin *= 0.01f;    // should be setupable
//        world_ray.Origin *= 1.0f;    // should be setupable
        world_ray.Origin *= 0.1f;    // should be setupable
        world_ray.Origin += world_point;
    }

    // Loop through meshes

    while( mesh_count-- )
    {
        current_mesh = *meshes++;

        if ( current_mesh != discarded )
        {
            geometric_mesh = current_mesh->GetMesh();

            if ( use_cache && StabbedTriangleIndex < geometric_mesh->GetTriangleCount() )
            {
                REAL32
                    triangle_distance,
                    triangle_u,
                    triangle_v;

                if ( GEOMETRIC_CONTACT_FindRayIntersectionToTriangle(
                    triangle_distance,
                    triangle_u,
                    triangle_v,
                    world_ray.Origin,
                    world_ray.Direction,
                    geometric_mesh->GetPointArray()[ geometric_mesh->GetTriangleArray()[ StabbedTriangleIndex ].VertexIndexArray[ 0 ] ],
                    geometric_mesh->GetPointArray()[ geometric_mesh->GetTriangleArray()[ StabbedTriangleIndex ].VertexIndexArray[ 1 ] ],
                    geometric_mesh->GetPointArray()[ geometric_mesh->GetTriangleArray()[ StabbedTriangleIndex ].VertexIndexArray[ 2 ] ],
                    BOOL_True
                    ) )
                {
                    CacheHitCount++;
                    return BOOL32_False;
                }
            }

            StabbedTriangleIndex = geometric_mesh->FindAnyRayIntersection( world_ray, BOOL_True );

            if ( StabbedTriangleIndex != -1 )
            {
                return BOOL32_False;    // return as soon as we know we're shadowed
            }
        }
    }
#endif
	return TRUE;
}

BOOL VertexLighter::IsVisibleFromLight2(const Point& world_point, const Point& world_normal, const VertexLighterLight* current_light, const VertexLighterMesh* discarded, BOOL use_cache) const
{
	Ray world_ray;
	if(discarded)
	{
		world_ray.mOrig = world_point;
	}
	else
	{
		world_ray.mOrig = world_normal;    // :TODO: check it's always correctly oriented!
		world_ray.mOrig *= 0.1f;    // should be setupable
		world_ray.mOrig += world_point;
	}
	world_ray.mDir = (current_light->GetPosition() - world_ray.mOrig).Normalize();
	float max_dist = world_ray.mOrig.Distance(current_light->GetPosition());


	// Loop through meshes
	udword	mesh_count = GetMeshCount();
	VertexLighterMesh** meshes = GetMeshTable();

	while(mesh_count--)
	{
		VertexLighterMesh* current_mesh = *meshes++;

		if(current_mesh!=discarded)
		{
#ifdef TOFIX
#endif
			mStabbedTriangleIndex = current_mesh->Stab(world_ray, max_dist);

			if(mStabbedTriangleIndex!=INVALID_ID)	return FALSE;	// return as soon as we know we're shadowed
		}
	}
	return TRUE;
}

#endif