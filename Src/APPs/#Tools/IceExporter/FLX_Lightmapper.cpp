///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// OBSOLETE code - still working but the one included in ICE is way better
// You probably shouldn't use this.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

#ifdef SUPPORT_OLD_LIGHTMAPPER

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LM_Material::LM_Material()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LM_Material::~LM_Material()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LM_Light::LM_Light()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LM_Light::~LM_Light()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LM_Mesh::LM_Mesh()
{
	mNbFaces	= 0;
	mNbVerts	= 0;
	mVerts		= null;
	mFaces		= null;
	mFNormals	= null;
	mVNormals	= null;
	mColors		= null;

	mNbColors	= 0;
	mColors2	= null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LM_Mesh::~LM_Mesh()
{
	DELETEARRAY(mVerts);
	DELETEARRAY(mFaces);
	DELETEARRAY(mFNormals);
	DELETEARRAY(mVNormals);
	DELETEARRAY(mColors);
	DELETEARRAY(mColors2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Lightmapper::Lightmapper()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Lightmapper::~Lightmapper()
{
	// Release lights
	for(udword i=0;i<mLights.GetNbEntries();i++)
	{
		LM_Light* CurLight = (LM_Light*)mLights.GetEntries()[i];
		DELETESINGLE(CurLight);
	}

	// Release meshes
	for(udword i=0;i<mMeshes.GetNbEntries();i++)
	{
		LM_Mesh* CurMesh = (LM_Mesh*)mMeshes.GetEntries()[i];
		DELETESINGLE(CurMesh);
	}

	// No need to release clean meshes, since they're just copies of pointers
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Plug-in initialization method.
 *	This method is called once before each export. When this method is called, the mSettings and mFilename members of the base format are valid.
 *	\param		true for pure motion files.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Lightmapper::Init(bool motion)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Scene export method.
 *	This method is called once to export the scene global information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Lightmapper::SetSceneInfo(const MainDescriptor& maininfo)
{
	// Save ambient color
	mAmbientLight.x	= maininfo.mAmbColor.x;
	mAmbientLight.y	= maininfo.mAmbColor.y;
	mAmbientLight.z	= maininfo.mAmbColor.z;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Camera export method.
 *	This method is called once for each exported camera.
 *	\param		camera		a structure filled with current camera information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Lightmapper::ExportCamera(const CameraDescriptor& camera)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Controller export method.
 *	This method is called once for each exported controller.
 *	\param		controller		a structure filled with current controller information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Lightmapper::ExportController(const ControllerDescriptor& controller)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Helper export method.
 *	This method is called once for each exported helper.
 *	\param		helper		a structure filled with current helper information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Lightmapper::ExportHelper(const HelperDescriptor& helper)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Light export method.
 *	This method is called once for each exported light.
 *	\param		light		a structure filled with current light information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Lightmapper::ExportLight(const LightDescriptor& light)
{
	// Create a new light
	LM_Light* NewLight = new LM_Light;
	CHECKALLOC(NewLight);

	// Save relevant info
	NewLight->mPosition.x	= light.mPrs.mPosition.x;
	NewLight->mPosition.y	= light.mPrs.mPosition.y;
	NewLight->mPosition.z	= light.mPrs.mPosition.z;

	NewLight->mColor.x		= light.mColor.x;
	NewLight->mColor.y		= light.mColor.y;
	NewLight->mColor.z		= light.mColor.z;

	// Add new light
	mLights.Add(udword(NewLight));

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Material export method.
 *	This method is called once for each exported material.
 *	\param		material		a structure filled with current material information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Lightmapper::ExportMaterial(const MaterialDescriptor& material)
{
	// Create a new material
	LM_Material* NewMaterial = new LM_Material;
	CHECKALLOC(NewMaterial);

	// Save relevant info
	NewMaterial->mAmbientColor.x	= material.mMtlAmbientColor.x;
	NewMaterial->mAmbientColor.y	= material.mMtlAmbientColor.y;
	NewMaterial->mAmbientColor.z	= material.mMtlAmbientColor.z;

	NewMaterial->mDiffuseColor.x	= material.mMtlDiffuseColor.x;
	NewMaterial->mDiffuseColor.y	= material.mMtlDiffuseColor.y;
	NewMaterial->mDiffuseColor.z	= material.mMtlDiffuseColor.z;

/*	// If a texture exists, override diffuse color
	if(material.mDiffuseMapID!=0xffffffff)
	{
		NewMaterial->mDiffuseColor = Point(1.0f, 1.0f, 1.0f);
	}
*/
	// Add new material
	mMaterials.Add(udword(NewMaterial));

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Mesh export method.
 *	This method is called once for each exported mesh.
 *	\param		mesh		a structure filled with current mesh information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Lightmapper::ExportMesh(const MeshDescriptor& mesh)
{
	// ### instances & cie

	// Create a new mesh
	LM_Mesh* NewMesh = new LM_Mesh;
	CHECKALLOC(NewMesh);

	// Save relevant info
	NewMesh->mID			= mesh.mObjectID;

	NewMesh->mPosition.x	= mesh.mPrs.mPosition.x;
	NewMesh->mPosition.y	= mesh.mPrs.mPosition.y;
	NewMesh->mPosition.z	= mesh.mPrs.mPosition.z;
	NewMesh->mRotation.p.x	= mesh.mPrs.mRotation.x;
	NewMesh->mRotation.p.y	= mesh.mPrs.mRotation.y;
	NewMesh->mRotation.p.z	= mesh.mPrs.mRotation.z;
	NewMesh->mRotation.w	= mesh.mPrs.mRotation.w;
	NewMesh->mScale.x		= mesh.mPrs.mScale.x;
	NewMesh->mScale.y		= mesh.mPrs.mScale.y;
	NewMesh->mScale.z		= mesh.mPrs.mScale.z;

	// Create world matrix
	Matrix4x4 World = NewMesh->mRotation;
	World.Scale(NewMesh->mScale);
	World.SetTrans(NewMesh->mPosition);

	// Export MAX native mesh
	const MAXNativeMesh* Mesh = mesh.mIsInstance ? mesh.mMasterMesh->mOriginalMesh : mesh.mOriginalMesh;

	NewMesh->mNbFaces	= Mesh->mNbFaces;
	NewMesh->mNbVerts	= Mesh->mNbVerts;

	// Save vertices (null for skins)
	if(Mesh->mVerts)
	{
		NakedPoint* v = Mesh->mVerts;
		NewMesh->mVerts = new Point[NewMesh->mNbVerts];
		CHECKALLOC(NewMesh->mVerts);

		Point Maxi(MIN_FLOAT, MIN_FLOAT, MIN_FLOAT);
		Point Mini(MAX_FLOAT, MAX_FLOAT, MAX_FLOAT);

		for(udword i=0;i<Mesh->mNbVerts;i++)
		{
			NewMesh->mVerts[i].x = v[i].x;
			NewMesh->mVerts[i].y = v[i].y;
			NewMesh->mVerts[i].z = v[i].z;
			// Transform to world space
			NewMesh->mVerts[i] *= World;
			// Catch min & max
			if(NewMesh->mVerts[i].x > Maxi.x)	Maxi.x = NewMesh->mVerts[i].x;
			if(NewMesh->mVerts[i].y > Maxi.y)	Maxi.y = NewMesh->mVerts[i].y;
			if(NewMesh->mVerts[i].z > Maxi.z)	Maxi.z = NewMesh->mVerts[i].z;
			if(NewMesh->mVerts[i].x < Mini.x)	Mini.x = NewMesh->mVerts[i].x;
			if(NewMesh->mVerts[i].y < Mini.y)	Mini.y = NewMesh->mVerts[i].y;
			if(NewMesh->mVerts[i].z < Mini.z)	Mini.z = NewMesh->mVerts[i].z;
		}

		// Compute AABB in world space
		NewMesh->mAABB.SetMinMax(Mini, Maxi);
	}
	else
	{
		long bug=1;
	}

	// Save faces
	NewMesh->mFaces = new udword[NewMesh->mNbFaces*3];
	CHECKALLOC(NewMesh->mFaces);

	MAXFace* f = Mesh->mFaces;
	for(udword i=0;i<Mesh->GetNbFaces();i++)
	{
		if(Mesh->mFlags&MESH_VFACE)
		{
			NewMesh->mFaces[i*3+0] = f[i].VRef[0];
			NewMesh->mFaces[i*3+1] = f[i].VRef[1];
			NewMesh->mFaces[i*3+2] = f[i].VRef[2];
		}
	}

	// Add new mesh
	mMeshes.Add(udword(NewMesh));
	if(!mesh.mIsInstance)	mCleanMeshes.Add(udword(mesh.mCleanMesh));
	else					mCleanMeshes.Add(udword(mesh.mMasterMesh->mCleanMesh));

	// Compute normals
	NewMesh->mFNormals = new Point[NewMesh->mNbFaces];	CHECKALLOC(NewMesh->mFNormals);
	NewMesh->mVNormals = new Point[NewMesh->mNbVerts];	CHECKALLOC(NewMesh->mVNormals);

	NORMALSCREATE nc;
	nc.NbVerts	= NewMesh->mNbVerts;
	nc.Verts	= NewMesh->mVerts;
	nc.NbFaces	= NewMesh->mNbFaces;
	nc.dFaces	= NewMesh->mFaces;
	nc.wFaces	= null;
	nc.FNormals	= NewMesh->mFNormals;
	nc.VNormals	= NewMesh->mVNormals;

	SmoothNormals NormalComputer;
	bool Status = NormalComputer.Compute(nc);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Motion export method.
 *	This method is called once for each exported motion.
 *	\param		motion		a structure filled with current motion information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Lightmapper::ExportMotion(const MotionDescriptor& motion)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Shape export method.
 *	This method is called once for each exported shape.
 *	\param		shape		a structure filled with current shape information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Lightmapper::ExportShape(const ShapeDescriptor& shape)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Texture export method.
 *	This method is called once for each exported texture.
 *	\param		texture		a structure filled with current texture information.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Lightmapper::ExportTexture(const TextureDescriptor& texture)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	End of export notification.
 *	This method is called once all nodes have been exported. This is a convenient place to free all used ram, etc.
 *	\param		stats		a structure filled with some export statistics.
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Lightmapper::EndExport(const StatsDescriptor& stats)
{
// ### opacity in alpha...
	// Nothing to do if there's no light...
	if(!GetNbLights())	return true;

	LM_Material	DefaultMat;
	DefaultMat.mDiffuseColor	= Point(1.0f, 1.0f, 1.0f);

	// Loop through meshes
	for(udword m=0;m<mMeshes.GetNbEntries();m++)
	{
		// Get current mesh
		LM_Mesh* CurMesh = (LM_Mesh*)mMeshes.GetEntries()[m];
		MBResult* CurCleanMesh = (MBResult*)mCleanMeshes.GetEntries()[m];

		// Create world matrix
		Matrix4x4 World = CurMesh->mRotation;
		World.Scale(CurMesh->mScale);
		World.SetTrans(CurMesh->mPosition);

		// Create normal matrix
		Matrix4x4 NormMatrix = CurMesh->mRotation;

		// Lighting is performed on *replicated* vertices, so that the correct normal is used anytime.

		MBTopology& Topo	= CurCleanMesh->Topology;
		MBGeometry& Geo		= CurCleanMesh->Geometry;
		MBMaterials& Mtl	= CurCleanMesh->Materials;

		//
		CurMesh->mColors2 = new Point[Geo.NbVerts];
		CHECKALLOC(CurMesh->mColors2);

		udword* VRefs = Geo.VertsRefs;
		udword v = 0;

		for(udword i=0;i<Mtl.NbMtls;i++)
		{
			// Get current material
			MBMatInfo* CurMtl = &Mtl.MaterialInfo[i];

			LM_Material* CurMat = CurMtl->MatID==0xffffffff ? &DefaultMat : GetMaterials()[CurMtl->MatID];

			for(udword j=0;j<CurMtl->NbVerts;j++)
			{
				// Initialize color
				CurMesh->mColors2[v] = mAmbientLight * CurMat->mAmbientColor;

				// Get indexed vertex position
				udword VRef = VRefs[v];
				float x = Geo.Verts[VRef*3+0];
				float y = Geo.Verts[VRef*3+1];
				float z = Geo.Verts[VRef*3+2];

				// Get vertex normal
				float nx = Geo.Normals[v*3+0];
				float ny = Geo.Normals[v*3+1];
				float nz = Geo.Normals[v*3+2];

				Point VPos(x, y, z);
				Point VNorm(nx, ny, nz);

				// Transform to world space
				VPos *= World;
				VNorm *= NormMatrix;

				// Loop through lights
				for(udword l=0;l<mLights.GetNbEntries();l++)
				{
					// Get current light
					LM_Light* CurLight = (LM_Light*)mLights.GetEntries()[l];

					// Compute light vector
					Point L = (CurLight->mPosition - VPos).Normalize();

					float Intensity = L|VNorm;
					if(Intensity<0.0f)	Intensity = 0.0f;

					Point Color = CurMat->mDiffuseColor * CurLight->mColor * Intensity;

////////////
					// Is current vertex visible from current light?
					// False ? True ?
					if(mSettings.mComputeShadows)
					{
						if(StabFaces(VPos, L, true, CurMesh, CurLight->mPosition.Distance(VPos)))	Color.Zero();
					}
////////////
					CurMesh->mColors2[v] += Color;
				}
				v++;
			}
		}
		CurMesh->mNbColors = v;

		// Color smoothing
		if(mSettings.mColorSmoothing)
		{
			Point* SmoothColors = new Point[CurCleanMesh->Geometry.NbVerts];
			CHECKALLOC(SmoothColors);
			ZeroMemory(SmoothColors, CurCleanMesh->Geometry.NbVerts*sizeof(Point));

			udword* TVRefs = CurCleanMesh->Topology.VRefs;
			for(udword i=0;i<CurCleanMesh->Materials.NbMtls;i++)
			{
				udword NbFaces = CurCleanMesh->Materials.MaterialInfo[i].NbFaces;

				VALENCIESCREATE	vc;
				vc.NbVerts		= CurCleanMesh->Geometry.NbVerts;
				vc.NbFaces		= NbFaces;
				vc.dFaces		= TVRefs;
				vc.AdjacentList	= true;

				Valencies VL;
				VL.Compute(vc);

				for(udword j=0;j<CurCleanMesh->Geometry.NbVerts;j++)
				{
					udword Valency	= VL.GetValency(j);
					if(Valency)
					{
						udword Offset	= VL.GetOffset(j);
						Point Contrib = CurMesh->mColors2[j];
						for(udword k=0;k<Valency;k++)
						{
							udword Neighbor = VL.GetAdjacentVerts()[Offset+k];
							Contrib += CurMesh->mColors2[Neighbor];
						}
						//SmoothColors[j] += Contrib / float(Valency+1);
						SmoothColors[j] = Contrib / float(Valency+1);
					}
				}

				TVRefs+=NbFaces*3;
			}
			CopyMemory(CurMesh->mColors2, SmoothColors, CurCleanMesh->Geometry.NbVerts*sizeof(Point));
			DELETEARRAY(SmoothColors);
		}
	}

/*
// ### shadow pass after ?? cf Lara
	// Shadow pass
	if(mSettings.ComputeShadows)
	{
		// Loop through meshes
		for(udword m=0;m<mMeshes.GetNbEntries();m++)
		{
			// Get current mesh
			LM_Mesh* CurMesh = (LM_Mesh*)mMeshes.GetEntries()[m];
			MESHBUILDERRESULT* CurCleanMesh = (MESHBUILDERRESULT*)mCleanMeshes.GetEntries()[m];

			// Create world matrix
			Matrix4x4 World = CurMesh->mRotation;
			World.Scale(CurMesh->mScale);
			World.SetTrans(CurMesh->mPosition);

			// Lighting is performed on *replicated* vertices, so that the correct normal is used anytime.

			MESHBUILDERTOPOLOGY& Topo	= CurCleanMesh->Topology;
			MESHBUILDERGEOMETRY& Geo	= CurCleanMesh->Geometry;
			MESHBUILDERMATERIALS& Mtl	= CurCleanMesh->Materials;

			udword* VRefs = Geo.VertsRefs;
			udword v = 0;

			for(udword i=0;i<Mtl.NbMtls;i++)
			{
				// Get current material
				MESHBUILDERMATINFO* CurMtl = &Mtl.MaterialInfo[i];

				LM_Material* CurMat = GetMaterials()[CurMtl->MatID];	// ### check

				for(udword j=0;j<CurMtl->NbVerts;j++)
				{
					// Get indexed vertex position
					udword VRef = VRefs[v];
					float x = Geo.Verts[VRef*3+0];
					float y = Geo.Verts[VRef*3+1];
					float z = Geo.Verts[VRef*3+2];
					Point VPos(x, y, z);
					VPos *= World;

					udword ShadowValue = 0;
					// Loop through lights
					for(udword l=0;l<mLights.GetNbEntries();l++)
					{
						// Get current light
						LM_Light* CurLight = (LM_Light*)mLights.GetEntries()[l];

						// Compute light vector
						Point L = (CurLight->mPosition - VPos).Normalize();

						// Is current vertex visible from current light?
						// False ? True ?
						if(StabFaces(VPos, L, true, CurMesh))
						{
							ShadowValue++;
						}
					}

					// Is the vertex shadowed ?
					if(ShadowValue)
					{
						// Vertex is in the shadow of ShadowValue lights
						for(udword s=0;s<ShadowValue;s++)	CurMesh->mColors2[v] *= 0.1f;
					}

					v++;
				}
			}
		}
	}
*/




if(0)
{





	// ### mettre une callback dans les plug-ins....
	// ### VC anim?
	// ### vitraux....

	// Loop through meshes
	for(udword i=0;i<mMeshes.GetNbEntries();i++)
	{
		// Get current mesh
		LM_Mesh* CurMesh = (LM_Mesh*)mMeshes.GetEntries()[i];
		MBResult* CurCleanMesh = (MBResult*)mCleanMeshes.GetEntries()[i];

		// Get some bytes for vertex-colors
		CurMesh->mColors = new Point[CurMesh->mNbVerts];
		CHECKALLOC(CurMesh->mColors);

		// Initialize with constant term
		for(udword j=0;j<CurMesh->mNbVerts;j++)
		{
			CurMesh->mColors[j] = mAmbientLight;
		}

		// Loop through vertices
		for(udword j=0;j<CurMesh->mNbVerts;j++)
		{
			// Loop through lights
			for(udword l=0;l<mLights.GetNbEntries();l++)
			{
				// Get current light
				LM_Light* CurLight = (LM_Light*)mLights.GetEntries()[l];

				// Compute light vector
				Point L = (CurLight->mPosition - CurMesh->mVerts[j]).Normalize();

				float Intensity = L|CurMesh->mVNormals[j];
				if(Intensity<0.0f)	Intensity = 0.0f;

				Point Color = CurLight->mColor * Intensity;

				CurMesh->mColors[j] += Color;
			}
		}
	}

	// Shadow pass
	if(mSettings.mComputeShadows)
	{
		// Loop through meshes
		for(udword i=0;i<mMeshes.GetNbEntries();i++)
		{
			// Get current mesh
			LM_Mesh* CurMesh = (LM_Mesh*)mMeshes.GetEntries()[i];

			// Loop through vertices
			for(udword j=0;j<CurMesh->mNbVerts;j++)
			{
				udword ShadowValue = 0;
				// Loop through lights
				for(udword l=0;l<mLights.GetNbEntries();l++)
				{
					// Get current light
					LM_Light* CurLight = (LM_Light*)mLights.GetEntries()[l];

					// Compute light vector
					Point L = (CurLight->mPosition - CurMesh->mVerts[j]).Normalize();

					// Is current vertex visible from current light?
					// False ? True ?
					if(StabFaces(CurMesh->mVerts[j], L, true, CurMesh, MAX_FLOAT))
					{
						ShadowValue++;
					}
				}

				// Is the vertex shadowed ?
				if(ShadowValue)
				{
					// Vertex is in the shadow of ShadowValue lights
					for(udword s=0;s<ShadowValue;s++)	CurMesh->mColors[j] *= 0.1f;
				}
			}
		}
	}
}
	return true;
}

ExportSettings* Lightmapper::OverrideSettings(ExportSettings* settings)
{
	// Those are the user settings, as setup in the Options panel.
	mSettings = *settings;

	// Override some of them, needed to compute lightmaps

	// General settings
	mSettings.mCompression					= FLX_COMPRESSION_NONE;
//	mSettings.mExportWholeScene				= true;
	mSettings.mExportVisTrack				= false;	// Not needed for lightmaps
//	mSettings.mExportHiddenNodes			= false;
	// Mesh settings
//	mSettings.mExpUVW						= true;
//	mSettings.mDiscardW						= true;
	mSettings.mExpVTXColor					= false;	// Vertex colors may already exist, but we will override them here.
//	mSettings.mUseSmgrp						= true;
	mSettings.mRemoveScaling				= true;		// Easier.
	mSettings.mConvertToD3D					= true;		// Easier.
	mSettings.mAbsolutePRS					= true;		// Sure!
	mSettings.mConvexHull					= false;	// Not needed for lightmaps
	mSettings.mBoundingSphere				= false;	// Not needed for lightmaps
	mSettings.mInertiaTensor				= false;	// Not needed for lightmaps
	// Consolidation settings
	mSettings.mConsolidateMesh				= true;		// Better.
	mSettings.mComputeFaceNormals			= true;		// Better.
	mSettings.mComputeVertexNormals			= true;		// Compulsory.
	mSettings.mExportNormalInfo				= false;	// Not needed for lightmaps
	// Material settings
//	mSettings.mForceAmbient					= false;
//	mSettings.mForceDiffuse					= false;
	// Texture settings: discard all textures, not needed by the lighting process
	mSettings.mOpacityInAlpha				= false;
	mSettings.mTexnameOnly					= true;
	mSettings.mKeepAmbientTexture			= false;
	mSettings.mKeepDiffuseTexture			= false;
	mSettings.mKeepSpecularTexture			= false;
	mSettings.mKeepShininessTexture			= false;
	mSettings.mKeepShiningStrengthTexture	= false;
	mSettings.mKeepSelfIllumTexture			= false;
	mSettings.mKeepOpacityTexture			= false;
	mSettings.mKeepFilterTexture			= false;
	mSettings.mKeepBumpTexture				= false;
	mSettings.mKeepReflexionTexture			= false;
	mSettings.mKeepRefractionTexture		= false;
	mSettings.mKeepDisplacementTexture		= false;
	mSettings.mTexMaxSize					= 256;
	// Camera settings: discard all cameras, not needed by the lighting process
	mSettings.mExpCameras					= false;
	mSettings.mExpTargetNodes				= false;
	// Light settings: keep lights, of course....
	mSettings.mExpLights					= true;
//	mSettings.mComputeVtxColors				= false;
//	mSettings.mComputeShadows				= false;
	// color smoothing = user-selected
	// Animation settings
	mSettings.mSingleFrame					= true;
	mSettings.mSampling						= true;
	mSettings.mSamplingRate					= 1;

	// NB: the same settings will be copied back by the base method SetSettings()...
	return &mSettings;
}




LM_Mesh* Lightmapper::GetMesh(udword id)
{
	for(udword i=0;i<mMeshes.GetNbEntries();i++)
	{
		LM_Mesh* CurMesh = (LM_Mesh*)mMeshes.GetEntries()[i];
		if(CurMesh->mID==id)	return CurMesh;
	}
	return null;
}

// ### to optimize
// Invert world
// bbox
// report first face only
bool Lightmapper::StabFaces(Point& orig, Point& dir, bool culling, LM_Mesh* discarded, float dist)
{
	// Loop through all meshes and all faces....
	for(udword i=0;i<GetNbMeshes();i++)
	{
		LM_Mesh* CurMesh = GetMeshes()[i];

		if(CurMesh!=discarded)
		{
			// Test ray against AABB
			Point HitPoint;
			if(RayAABB2(CurMesh->mAABB, orig, dir, HitPoint))
			{
				// Test against faces
				udword* Faces = CurMesh->mFaces;
				Point* Verts = CurMesh->mVerts;
				for(udword j=0;j<CurMesh->mNbFaces;j++)
				{
					udword Ref0 = *Faces++;
					udword Ref1 = *Faces++;
					udword Ref2 = *Faces++;

					float t, u, v;
					if(RayTri(orig, dir, Verts[Ref0], Verts[Ref1], Verts[Ref2], t, u, v, culling))
					{
						if(t>0.0f && t<dist)	return true;
					}
				}
			}
		}
	}

	return false;
}


#endif

