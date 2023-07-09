///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	This file contains the Vertex Data Modifier.
 *	\file		FLX_VertexDataModifier.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 25, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

static VertexDataModifierDesc LOCAL_Descriptor;

VertexDataModifierDesc* IceExporter::GetVertexDataModifierDesc()
{
	return &LOCAL_Descriptor;
}

void* VertexDataModifierDesc::Create(BOOL loading)
{
	return new VertexDataModifier;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VertexDataModifier::VertexDataModifier()
{
	mNbVerts	= 0;
	mNbFaces	= 0;
	mVerts		= null;
	mFaces		= null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VertexDataModifier::~VertexDataModifier()
{
	ReleaseData();
}

void VertexDataModifier::ReleaseData()
{
	DELETEARRAY(mFaces);
	DELETEARRAY(mVerts);
}

Class_ID VertexDataModifier::ClassID()
{
	return VERTEX_DATA_MODIFIER_CLASS_ID;
}

void VertexDataModifier::DeleteThis()
{
	delete this;
}

void VertexDataModifier::GetClassName(TSTR& s)
{
	s = _T("Vertex Data Modifier");
}

TCHAR* VertexDataModifier::GetObjectName()
{
	return _T("Vertex Data Modifier");
}

CreateMouseCallBack * VertexDataModifier::GetCreateMouseCallBack()
{
	return null;
}

// Prevent us from being copied in TrackView/ModifierStack.
BOOL VertexDataModifier::CanCopyAnim()
{
	return FALSE;
}

// These are channels that the modifier needs in order to perform its modification.
// This should at least include the channels specified in ChannelsChanged() but may include more.
// Note that ChannelsUsed() is called many times but the channels returned should not change on the fly.
ChannelMask VertexDataModifier::ChannelsUsed()
{
	return VERTCOLOR_CHANNEL | TOPO_CHANNEL;
}

// These are the channels that the modifier actually modifies. Note that ChannelsChanged() is called many
// times but the channels returned should not change on the fly.
ChannelMask VertexDataModifier::ChannelsChanged()
{
	return VERTCOLOR_CHANNEL | TOPO_CHANNEL;
}

// Modifiers that place a dependency on topology should return TRUE for this method. An example would be a modifier that stores
// a selection set base on vertex indices. This modifier depends on the indices being intact for it to operate correctly.
BOOL VertexDataModifier::DependOnTopology(ModContext& mc)
{
	return TRUE;
}

// This is the type of object that the modifier knows how to modify.
// Simple modifiers that just modify points of an object can operate on generic 'Deformable' objects.
// Deformable objects are any type of object that has points. A modifier could also work on a particular
// type of object such as a TriObject or PatchObject.
Class_ID VertexDataModifier::InputType()
{
	return mapObjectClassID;
}

int VertexDataModifier::NumRefs()
{
	return 0;
}

RefTargetHandle VertexDataModifier::GetReference(int i)
{
	return null;
}

void VertexDataModifier::SetReference(int i, RefTargetHandle rtarg)
{
}

int VertexDataModifier::NumSubs()
{
	return 0;
}

Animatable* VertexDataModifier::SubAnim(int i)
{
	return null;
}

TSTR VertexDataModifier::SubAnimName(int i)
{
	return _T("");
}

void VertexDataModifier::BeginEditParams(IObjParam* ip, ULONG flags, Animatable* prev)
{
}

void VertexDataModifier::EndEditParams(IObjParam* ip, ULONG flags, Animatable* next)
{
}

// This is the method that actually modifies the input object. This method is responsible for altering the object and
// then updating the validity interval of the object to reflect the validity of the modifier.
void VertexDataModifier::ModifyObject(
    TimeValue t,    // the time at which the modification is being done.
    ModContext & mc,    // a reference to the ModContext.
    ObjectState * os,    // the object state flowing through the pipeline. This contains a pointer to the object to modify.
    INode * node    // the node the modifier is applied to. This parameter is always NULL for Object Space Modifiers and non-NULL for
                    // World Space Modifiers (Space Warps). This is because a given WSM is only applied to a single node at a time
                    // whereas an OSM may be applied to several nodes. This may be used for example by particle system space warps
                    // to get the transformation matrix of the node at various times.
    )
{
	// handle TriObjects

	if(os->obj->IsSubClassOf(triObjectClassID))
    {
		TriObject* tobj = (TriObject*)os->obj;

		ApplyVertexColors(tobj);
	}
	// Convert to a tri mesh if possible
	else if(os->obj->CanConvertToType( triObjectClassID))
	{
		TriObject* triOb = (TriObject*)os->obj->ConvertToType(t, triObjectClassID);

		// Now stuff this into the pipeline!
		os->obj = triOb;

		ApplyVertexColors(triOb);
	}
}

// This method returns the validity interval of a modifier. It is simply the combination of the validity of all the modifier's parameters.
// It's used to determine when to cache in the pipeline, but is not directly responsible for determining when ModifyObject() is called.
// ModifyObject() is called when the pipeline needs to be evaluated either because someone sent a REFMSG_CHANGE message or the validity
// of the object does not include the current time.
//
// If a modifier is not animated it's OK to simply return FOREVER from this method. In the case where the modifier changes because a user
// changes a non-animated control in the user interface (for instance a check box), you can cause reevaluation by notifying your dependents
// of the change, i.e.: NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
Interval VertexDataModifier::LocalValidity(TimeValue t)
{
	return FOREVER;
}

// Serialize

#define LOCAL_VERSION_CHUNK			0x0000
#define LOCAL_VERTEX_COLORS_CHUNK	0x0001
#define LOCAL_COLOR_FACES_CHUNK		0x0002

IOResult VertexDataModifier::Load(ILoad* iload)
{
	ULONG loaded_bytes_count;
	IOResult io_result;

	// Call the base method
	Modifier::Load(iload);

	// Flush current data
	ReleaseData();

	udword PlugVersion = INVALID_ID;

	// Load personal data
	while(IO_OK ==(io_result=iload->OpenChunk()))
	{
		switch(iload->CurChunkID())
		{
			// :TODO: maybe I should have a version / chunk as in Ice ?
			case LOCAL_VERSION_CHUNK:
			{
				io_result = iload->Read(&PlugVersion, sizeof(udword), &loaded_bytes_count);
				break;
			}

			case LOCAL_VERTEX_COLORS_CHUNK:
			{
				if(PlugVersion==0x0001)
				{
					io_result = iload->Read(&mNbVerts, sizeof(udword), &loaded_bytes_count);

					if(mNbVerts)
					{
						mVerts = new Point[mNbVerts];
						io_result = iload->Read(mVerts, mNbVerts*sizeof(Point), &loaded_bytes_count);
					}
				}
				break;
			}

			case LOCAL_COLOR_FACES_CHUNK:
			{
				if(PlugVersion==0x0001)
				{
					io_result = iload->Read(&mNbFaces, sizeof(udword), &loaded_bytes_count);

					if(mNbFaces)
					{
						mFaces = new IndexedTriangle[mNbFaces];
						io_result = iload->Read(mFaces, mNbFaces*sizeof(IndexedTriangle), &loaded_bytes_count);
					}
				}
				break;
			}
		}
		iload->CloseChunk();
	}
	return IO_OK;
}

// Serialize
IOResult VertexDataModifier::Save(ISave* isave)
{
	ULONG saved_bytes_count;
	IOResult io_result;

	// Call the base method
	Modifier::Save(isave);

	// Save version information
	isave->BeginChunk(LOCAL_VERSION_CHUNK);
	{
		udword PlugVersion = VERTEX_DATA_MODIFIER_VERSION;
		io_result = isave->Write(&PlugVersion, sizeof(udword), &saved_bytes_count);
	}
	isave->EndChunk();

	// Save vertex colors
	isave->BeginChunk(LOCAL_VERTEX_COLORS_CHUNK);
	{
		io_result = isave->Write(&mNbVerts, sizeof(udword), &saved_bytes_count);
		if(mNbVerts)	io_result = isave->Write(mVerts, mNbVerts*sizeof(Point), &saved_bytes_count);
	}
	isave->EndChunk();

	// Save color faces
	isave->BeginChunk(LOCAL_COLOR_FACES_CHUNK);
	{
		io_result = isave->Write(&mNbFaces, sizeof(udword), &saved_bytes_count);
		if(mNbVerts)	io_result = isave->Write(mFaces, mNbFaces*sizeof(IndexedTriangle), &saved_bytes_count);
	}
	isave->EndChunk();

	return IO_OK;
}

RefResult VertexDataModifier::NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message)
{
	return REF_SUCCEED;
}

bool VertexDataModifier::SetVertexColors(IndexedSurface* color_surface, bool parity)
{
	if(!color_surface)	return false;

	mNbVerts = color_surface->GetNbVerts();
	mNbFaces = color_surface->GetNbFaces();

	DELETEARRAY(mVerts);
	mVerts = new Point[mNbVerts];
	CHECKALLOC(mVerts);

	CopyMemory(mVerts, color_surface->GetVerts(), mNbVerts*sizeof(Point));

	DELETEARRAY(mFaces);
	mFaces = new IndexedTriangle[mNbFaces];
	CHECKALLOC(mFaces);

	for(udword i=0;i<mNbFaces;i++)
	{
		mFaces[i].mRef[0] = color_surface->GetFace(i)->mRef[0];
		mFaces[i].mRef[1+parity] = color_surface->GetFace(i)->mRef[1];
		mFaces[i].mRef[2-parity] = color_surface->GetFace(i)->mRef[2];
	}

	return true;
}

bool VertexDataModifier::ApplyVertexColors(Object* node_object)
{
	if(mFaces && mVerts && mNbVerts && mNbFaces)
	{
		// Build a local surface
		IndexedSurface Local;
		Local.Init(mNbFaces, mNbVerts, mVerts, mFaces);

		if(!IceExporter::SetVertexColors(node_object, 0, &Local, null, false))
		{
			// This happens when:
			// - modifier is applied on a procedural mesh (say a plane)
			// - the underlying mesh's topology gets modified (say changing from a 4x4 plane to a 10*10 plane)
			// In those cases previously precomputed data become invalid, and if we try to use them we'll probably crash.
			// So let's just release everything. Apparently it seems to work quite well. When this happens, MAX has already
			// output a warning, so our own MessageBox would be redundant. We will still fill vertex colors with random
			// values so that the user can see he must re-apply the modifier.
			ReleaseData();
			ApplyRandomColors(node_object, 0);
		}
	}
	else
	{
		ApplyRandomColors(node_object, 0);
	}
	return true;
}
















#ifdef TOFIX

// This is a basic test method, to learn how modifiers work.
// It burns a bunch of random colors as vertex colors of a mesh.

VOID COMPUTE_VERTEX_DATA_MODIFIER::ApplyRandomColors(
    TriObject * obj,
    TimeValue t
    )
{
    INDEX32
        index;
    MAX_BOOL
        status;
    const REAL
        one_over_rand_max = 1.0f / REAL( RAND_MAX );
    Mesh
        & mesh = obj->GetMesh();

    status = mesh.setNumVertCol( mesh.numVerts );
    status = mesh.setNumVCFaces( mesh.numFaces );

    PRIMITIVE_ForEachCheckedIndex( index, mesh.numVerts )
    {
        mesh.vertCol[ index ].x = REAL( rand() ) * one_over_rand_max;
        mesh.vertCol[ index ].y = REAL( rand() ) * one_over_rand_max;
        mesh.vertCol[ index ].z = REAL( rand() ) * one_over_rand_max;
    }

    PRIMITIVE_ForEachCheckedIndex( index, mesh.numFaces )
    {
        mesh.vcFace[ index ].t[ 0 ] = mesh.faces[ index ].v[ 0 ];
        mesh.vcFace[ index ].t[ 1 ] = mesh.faces[ index ].v[ 1 ];
        mesh.vcFace[ index ].t[ 2 ] = mesh.faces[ index ].v[ 2 ];
    }

    NotifyDependents( Interval( t,t ), PART_VERTCOLOR, REFMSG_CHANGE );
    NotifyDependents( Interval( t,t ), PART_TOPO, REFMSG_CHANGE );
    obj->UpdateValidity( VERT_COLOR_CHAN_NUM, LocalValidity( t ) );
}

// ~~

static BOOL LOCAL_FillPerVertexUserChannel(
    Mesh & mesh_from_max,
    INT32 user_channel_id,
    COUNT32 source_item_count,
    const VOID * source_stream,
    INT32 source_stride
    )
{
    REAL32
        * user_channel;

    // Checkings

    if ( !source_stream || !source_item_count )
    {
        return BOOL_False;
    }

    user_channel = mesh_from_max.vertexFloat( user_channel_id );

    if ( user_channel    // must have been enabled beforehand (e.g. in the Utility plug)
        && mesh_from_max.vData[ user_channel_id ].dnum == source_item_count
        && mesh_from_max.vData[ user_channel_id ].DataSize() == sizeof( REAL32 )
        )
    {
        if ( source_stride == sizeof( REAL32 ) )
        {
            memcpy( user_channel, source_stream, source_item_count * sizeof( REAL32 ) );
        }
        else
        {
            const INT8
                * byte_stream = ( const INT8 * ) source_stream;

            while ( source_item_count-- )
            {
                *user_channel++ = * ( REAL32 * ) byte_stream;
                byte_stream += source_stride;
            }
        }
        return BOOL_True;
    }
    else
    {
        return BOOL_False;
    }
}

// This method takes the internal vertex data and replicates it in MAX

VOID COMPUTE_VERTEX_DATA_MODIFIER::ApplyVertexData(
    TriObject * obj,
    TimeValue t
    )
{
    if ( !obj )
    {
        return;
    }
    INDEX32
        current_index;
    MAX_BOOL
        status;
    Mesh
        & mesh = obj->GetMesh();

    if ( GetVertexCount() != mesh.numVerts )
    {
        // This happens when:
        // - modifier is applied on a procedural mesh (say a plane)
        // - the underlying mesh's topology gets modified (say changing from a 4x4 plane to a 10*10 plane)
        // In those cases previously precomputed data become invalid, and if we try to use them we'll probably crash.
        // So let's just release everything. Apparently it seems to work quite well. When this happens, MAX has already
        // output a warning, so our own MessageBox would be redundant. We will still fill vertex colors with random
        // values so that the user can see he must re-apply the modifier.

        ReleaseData();

        return;
    }

    // Setup number of vertices / faces. This is where MAX allocates the arrays.

    status = mesh.setNumVertCol( GetVertexCount() );
    status = mesh.setNumVCFaces( GetTriangleCount() );

    // Copy vertex colors. We replicate our diffuse colors in MAX's vertex colors so that the graphists can
    // see something !

    if ( !DiffuseColorTable.IsEmpty() )
    {
        PRIMITIVE_ForEachIndex( current_index, DiffuseColorTable.GetItemCount() )
        {
            mesh.vertCol[ current_index ].x = DiffuseColorTable[ current_index ].X;
            mesh.vertCol[ current_index ].y = DiffuseColorTable[ current_index ].Y;
            mesh.vertCol[ current_index ].z = DiffuseColorTable[ current_index ].Z;
        }
    }

    // Copy color topology in MAX

    if ( !TriangleTable.IsEmpty() )
    {
        PRIMITIVE_ForEachIndex( current_index, TriangleTable.GetItemCount() )
        {
            mesh.vcFace[ current_index ].t[ 0 ] = TriangleTable[ current_index ].VertexIndexArray[ 0 ];
            mesh.vcFace[ current_index ].t[ 1 ] = TriangleTable[ current_index ].VertexIndexArray[ 1 ];
            mesh.vcFace[ current_index ].t[ 2 ] = TriangleTable[ current_index ].VertexIndexArray[ 2 ];
        }
    }

    // Copy vertex normals in MAX

    if ( !VertexNormalTable.IsEmpty() )
    {
        mesh.buildNormals();

		PRIMITIVE_ForEachIndex( current_index, VertexNormalTable.GetItemCount() )
        {
            mesh.setNormal( current_index,
                            Point3(
                                VertexNormalTable[ current_index ].X,
                                VertexNormalTable[ current_index ].Y,
                                VertexNormalTable[ current_index ].Z
                                )
                            );

			DEBUG_Check(Point3(
                                VertexNormalTable[ current_index ].X,
                                VertexNormalTable[ current_index ].Y,
                                VertexNormalTable[ current_index ].Z
                                ) == mesh.getNormal( current_index ));

        }
    }

    // Setup per-vertex data
    //
    // Apparently this is the way to do it but I might be missing some subtle things here as the SDK is quite unclear.
    // I assume that:
    // - the first vertex data channels are reserved (VDATA_SELECT, VDATA_WEIGHT, VDATA_ALPHA...).
    // - each channel contains a list of items, of a given type. Only floats seem to be supported (PERDATA_TYPE_FLOAT).

    if ( !LightDirectionTable.IsEmpty() )
    {
        REAL32
            * source = ( REAL32* ) LightDirectionTable.GetArray();

        LOCAL_FillPerVertexUserChannel( mesh, LOCAL_VDATA_LIGHT_DIRECTION_X_CHANNEL, LightDirectionTable.GetItemCount(), source++, sizeof( MATH_VECTOR_3 ) );
        LOCAL_FillPerVertexUserChannel( mesh, LOCAL_VDATA_LIGHT_DIRECTION_Y_CHANNEL, LightDirectionTable.GetItemCount(), source++, sizeof( MATH_VECTOR_3 ) );
        LOCAL_FillPerVertexUserChannel( mesh, LOCAL_VDATA_LIGHT_DIRECTION_Z_CHANNEL, LightDirectionTable.GetItemCount(), source++, sizeof( MATH_VECTOR_3 ) );
    }

    // Setup specular color

    if ( !SpecularColorTable.IsEmpty() )
    {
        REAL32
            * source = ( REAL32* ) SpecularColorTable.GetArray();

        LOCAL_FillPerVertexUserChannel( mesh, LOCAL_VDATA_SPECULAR_X_CHANNEL, SpecularColorTable.GetItemCount(), source++, sizeof( MATH_VECTOR_3 ) );
        LOCAL_FillPerVertexUserChannel( mesh, LOCAL_VDATA_SPECULAR_Y_CHANNEL, SpecularColorTable.GetItemCount(), source++, sizeof( MATH_VECTOR_3 ) );
        LOCAL_FillPerVertexUserChannel( mesh, LOCAL_VDATA_SPECULAR_Z_CHANNEL, SpecularColorTable.GetItemCount(), source++, sizeof( MATH_VECTOR_3 ) );
    }

    // Tell MAX about the updates

    NotifyDependents( Interval( t,t ), PART_GEOM, REFMSG_CHANGE );    // for the vertex normals
    NotifyDependents( Interval( t,t ), PART_VERTCOLOR, REFMSG_CHANGE );
    NotifyDependents( Interval( t,t ), PART_TOPO, REFMSG_CHANGE );
    NotifyDependents( Interval( t,t ), PART_EXTENSION, REFMSG_CHANGE );
//    NotifyDependents( FOREVER, PART_ALL, REFMSG_CHANGE );
    obj->UpdateValidity( GEOM_CHAN_NUM, LocalValidity( t ) );    // for the vertex normals
    obj->UpdateValidity( VERT_COLOR_CHAN_NUM, LocalValidity( t ) );
    obj->UpdateValidity( EXTENSION_CHAN_NUM, LocalValidity( t ) );
//    obj->UpdateValidity( ALL_CHANNELS, FOREVER );

//	mesh.InvalidateGeomCache();
//	mesh.InvalidateTopologyCache();
//	et->InvalidateTempData (PART_GEOM);
/*
	if (!tempMove) {
		if (tempData) tempData->Invalidate (parts);
		if (parts & (PART_TOPO|PART_GEOM|PART_SELECT|PART_SUBSEL_TYPE))
			InvalidateAffectRegion ();
	}
	if (parts & PART_TOPO) GetMesh().InvalidateTopologyCache();
	if (parts & PART_GEOM) GetMesh().InvalidateGeomCache ();
*/

}

// This is called by the Utility plug-in to setup the vertex data

BOOL COMPUTE_VERTEX_DATA_MODIFIER::SetVertexData(
    COUNT32 vertex_count,
    COUNT32 triangle_count,
    const MATH_VECTOR_3 * diffuse_color_array,
    const MATH_VECTOR_3 * specular_color_array,
    const MATH_VECTOR_3 * direction_array,
    const MATH_VECTOR_3 * normal_array,
    const GEOMETRIC_TRIANGLE * triangle_array,
    INDEX32 vertex_offset,
    BOOL parity
    )
{
    BOOL
        status;
    INDEX32
        triangle_index,
        vertex_index;

    ReleaseData();

    // Checkings

    if ( !vertex_count || !triangle_count || !diffuse_color_array || !triangle_array )    // other arrays are not compulsory
    {
        return BOOL_False;
    }

    // Handle directions

    if ( direction_array )
    {
        LightDirectionTable.SetItemCount( vertex_count );

        memcpy( LightDirectionTable.GetArray(), direction_array, vertex_count * sizeof( MATH_VECTOR_3 ) );
    }

    // Handle normals

    if ( normal_array )
    {
        VertexNormalTable.SetItemCount( vertex_count );

        memcpy( VertexNormalTable.GetArray(), normal_array, vertex_count * sizeof( MATH_VECTOR_3 ) );
    }

    // Handle vertex colors

    DiffuseColorTable.SetItemCount( vertex_count );

    memcpy( DiffuseColorTable.GetArray(), diffuse_color_array, vertex_count * sizeof( MATH_VECTOR_3 ) );

    // Handle specular colors

    if ( specular_color_array )
    {
        SpecularColorTable.SetItemCount( vertex_count );

        memcpy( SpecularColorTable.GetArray(), specular_color_array, vertex_count * sizeof( MATH_VECTOR_3 ) );
    }

    // Handle topology

    TriangleTable.SetItemCount( triangle_count );

    {
        status = BOOL_True;

        PRIMITIVE_ForEachCheckedIndex( triangle_index, triangle_count )
        {
            vertex_index = triangle_array[ triangle_index ].VertexIndexArray[ 0 ] - vertex_offset;

            if ( !PRIMITIVE_IsValidIndex( vertex_index, vertex_count ) )
            {
                status = BOOL_False;
            }

            TriangleTable[ triangle_index ].VertexIndexArray[ 0 ] = vertex_index;

            vertex_index = triangle_array[ triangle_index ].VertexIndexArray[ 1 + parity ] - vertex_offset;

            if ( !PRIMITIVE_IsValidIndex( vertex_index, vertex_count ) )
            {
                status = BOOL_False;
            }

            TriangleTable[ triangle_index ].VertexIndexArray[ 1 ] = vertex_index;

            vertex_index = triangle_array[ triangle_index ].VertexIndexArray[ 2 - parity ] - vertex_offset;

            if ( !PRIMITIVE_IsValidIndex( vertex_index, vertex_count ) )
            {
                status = BOOL_False;
            }

            TriangleTable[ triangle_index ].VertexIndexArray[ 2 ] = vertex_index;
        }

        if ( !status )
        {
            MessageBox( REF_Null, "Problem in SetVertexData.\nSave that scene and send it over!", "Caption", MB_OK );
        }
    }

    // Tell MAX about the updates

	NotifyDependents( FOREVER, PART_VERTCOLOR, REFMSG_CHANGE );
	NotifyDependents( FOREVER, PART_GEOM, REFMSG_CHANGE );
    NotifyDependents( FOREVER, PART_TOPO, REFMSG_CHANGE );
    NotifyDependents( FOREVER, PART_EXTENSION, REFMSG_CHANGE );

    return BOOL_True;
}

// .. INQUIRIES

BOOL COMPUTE_VERTEX_DATA_MODIFIER::HasValidVertexColors(
    VOID
    ) const
{
    if ( DiffuseColorTable.IsEmpty() || TriangleTable.IsEmpty() )
    {
        return BOOL_False;
    }
    return BOOL_True;
}

#endif
