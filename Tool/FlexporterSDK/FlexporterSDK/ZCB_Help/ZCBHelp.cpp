
	class ICERENDERMANAGER_API RMMesh : public RMObject, public MultiSurface
	{
						..........

		// Consolidation
						MeshBuilder*		mBuilder;
						MESHBUILDERRESULT*	mResult;
						udword				mNbStrips;
						udword*				mStripLengths;
						uword**				mStripRuns;

						..........
	};




// This is how I call MeshBuilder to consolidate the mesh. Direct results are saved to disk by Flexporter.
bool RMMesh::Consolidate()
{
	// Check the mesh is a valid one
	if(!mIsConvertible || mIsTarget || mIsInstance)	return true;

	// Free possibly already existing data
	DELETESINGLE(mResult);
	DELETESINGLE(mBuilder);

	// Get fresh stuff
	mBuilder	= new MeshBuilder;			CHECKALLOC(mBuilder);
	mResult		= new MESHBUILDERRESULT;	CHECKALLOC(mResult);

	// Get surfaces
	IndexedSurface* UVSurface		= GetExtraSurface(mUVs);
	IndexedSurface* ColorSurface	= GetExtraSurface(mVCol);
	IndexedSurface* LightSurface	= GetExtraSurface(mLM);
	if(LightSurface) ColorSurface = LightSurface;

	MESHBUILDERCREATE			Create;
	Create.UseW					= false;
	Create.ComputeVNorm			= true;
	Create.ComputeFNorm			= false;
	Create.ComputeNormInfo		= false;
	Create.RelativeIndices		= false;
	Create.IndexedGeo			= true;			// Always ask for indexed results
	Create.IndexedUVW			= true;			// Always ask for indexed results
	Create.NbFaces				= mNbFaces;
	Create.NbVerts				= mNbVerts;
	Create.NbTVerts				= UVSurface ? UVSurface->GetNbVerts() : 0;
	Create.NbCVerts				= ColorSurface ? ColorSurface->GetNbVerts() : 0;
	Create.Verts				= mVerts;
	Create.TVerts				= UVSurface ? UVSurface->GetVerts() : null;
	Create.CVerts				= ColorSurface ? ColorSurface->GetVerts() : null;

	bool Status = mBuilder->Init(Create);
	if(!Status)	{ SetIceErrorMsg("MeshBuilder init failed!"); return false; }

	FaceProp* FP = (FaceProp*)GetFaceProperties();
	for(udword i=0;i<Create.NbFaces;i++)
	{
		udword* VRefs	= (udword*)GetFace(i);
		udword*	TRefs	= UVSurface ? (udword*)UVSurface->GetFace(i) : null;
		udword*	CRefs	= ColorSurface ? (udword*)ColorSurface->GetFace(i) : null;
		udword	Smg		= FP[i].Smg;
		sdword	MatID	= FP[i].MatID;
		ubyte EdgeVis	= FP[i].EdgeVis;
		bool Status = mBuilder->AddFace(MatID, Smg, EdgeVis, VRefs, TRefs, CRefs, GetParity());
		if(!Status)	{ SetIceErrorMsg("MeshBuilder AddFace failed!"); return false; }
	}

	// Perform mesh conversion
	Status = mBuilder->Build(*mResult);
	if(!Status)	{ SetIceErrorMsg("Mesh building failed!"); return false; }

	return true;
}

// This is how I create triangle strips (see www.codercorner.com\Strips.htm for the code)
bool RMMesh::CreateStrips()
{
	// I need the consolidated mesh
	if(!mResult)	{ SetIceErrorMsg("Consolidated mesh not available."); return false; }

	// Release possibly already existing strips
	DeleteStrips();

	// Create strips
	uword* VRefs = mResult->Topology.VRefs;
	mNbStrips = mResult->Materials.NbMtls;
	mStripRuns		= new uword*[mNbStrips];	CHECKALLOC(mStripRuns);		ZeroMemory(mStripRuns, mNbStrips*sizeof(udword*));
	mStripLengths	= new udword[mNbStrips];	CHECKALLOC(mStripLengths);
	// Create one strip / material
	for(udword i=0;i<mNbStrips;i++)
	{
		udword NbFaces = mResult->Materials.MaterialInfo[i].NbFaces;

		// Create triangle strips
		STRIPERCREATE stc;
		stc.NbFaces				= NbFaces;
		stc.WFaces				= VRefs;
		stc.AskForWords			= true;	// words needed to directly feed DirectX with
		stc.OneSided			= true;	// Single sided strips wanted
		stc.SGIAlgorithm		= true;	// Pick least connected faces first
		stc.ConnectAllStrips	= true;	// And finally we want a single strip

		// Init striper
		Striper Strip;
		bool Status = Strip.Init(stc);
		if(!Status)	{ DeleteStrips(); return false; }

		// Create strips
		STRIPERRESULT str;
		Status = Strip.Compute(str);
		if(!Status)	{ DeleteStrips(); return false;	}

		//
		mStripLengths[i] = *str.StripLengths;
		mStripRuns[i] = new uword[mStripLengths[i]];	CHECKALLOC(mStripRuns[i]);
		CopyMemory(mStripRuns[i], str.StripRuns, mStripLengths[i]*sizeof(uword));

		// Next strip for next material
		VRefs+= NbFaces*3;
	}
	return true;
}

// Use the consolidation results to create a DX7 Vertex Buffer
bool RMMesh::Register(Renderer* renderer, RMScene* scene)
{
	// We need a consolidated mesh
	if(!mResult)
	{
		// Here the mesh hasn't been consolidated, so let's try that...
		if(!Consolidate())	return false;
		if(!mResult)		return true;
	}

	// Mesh has successfully been consolidated
	udword NumVertices			=	mResult->Geometry.NbVerts;
	udword* VertsRefs			=	mResult->Geometry.VertsRefs;
	udword* TVertsRefs			=	mResult->Geometry.TVertsRefs;
	float* Verts				=	mResult->Geometry.Verts;
	float* TVerts				=	mResult->Geometry.TVerts;
	float* Normals				=	mResult->Geometry.Normals;
	float* CVerts				=	mResult->Geometry.CVerts;
	udword NbMtls				=	mResult->Materials.NbMtls;
	MESHBUILDERMATINFO* MtlInfo	=	mResult->Materials.MaterialInfo;

	// Release possibly already existing vertex buffer
	if(mVB)	renderer->DeleteVertexBuffer(mVB);

	// Create new vertex buffer
	VERTEXBUFFERCREATE vbc;
	vbc.Flags			= VBFLAGS_NORMAL;
	vbc.NbVertices		= NumVertices;
	vbc.VF				= 0;
	if(Verts)		vbc.VF|=VF_XYZ;
	if(TVerts)		vbc.VF|=VF_TEX1;
	if(CVerts)		vbc.VF|=VF_DIFFUSE;
	if(Normals)		vbc.VF|=VF_NORMAL;

	if(GetExtraSurface(mLM))
	{
		vbc.VF&=~VF_TEX1;
		vbc.VF&=~VF_DIFFUSE;
		vbc.VF|=VF_TEX2;
	}

	// Is there any lighting data available?
//	if(mColors)		vbc.VF|=VF_DIFFUSE;

	// Create a new vertex buffer
	mVB = renderer->CreateVertexBuffer(vbc);

	// Check the VB descriptor
	VBDescriptor Desc;
	mVB->GetDescriptor(Desc);

	// Fill the vertex buffer
	float* Pool = (float*)mVB->Lock();
	udword i=0;
	for(udword m=0;m<NbMtls;m++)
	{
		// Cropping values & texture matrix
		float OffsetU	= 0.0f;
		float OffsetV	= 0.0f;
		float ScaleU	= 1.0f;
		float ScaleV	= 1.0f;
		Matrix4x4 TexMat;
		TexMat.Identity();

		// Get cropping values for current material
		udword MatID = MtlInfo[m].MatID;
		if(MatID!=INVALID_ID)
		{
			RMMaterial* CurMat = (RMMaterial*)PTR(MatID);

			udword TexID = CurMat->mTexID;
			if(TexID!=INVALID_ID)
			{
				RMTexture* CurTex = (RMTexture*)PTR(TexID);

				OffsetU	= CurTex->mOffsetU;
				OffsetV	= CurTex->mOffsetV;
				ScaleU	= CurTex->mScaleU;
				ScaleV	= CurTex->mScaleV;
				TexMat	= CurTex->mTexMat;
			}
		}

		for(udword j=0;j<MtlInfo[m].NbVerts;j++)
		{
			// Vertices
			if(Verts)
			{
				uword Ref = VertsRefs[i];
				*Pool++ = Verts[Ref*3+0];
				*Pool++ = Verts[Ref*3+1];
				*Pool++ = Verts[Ref*3+2];
			}

			// Normals
			if(Normals)
			{
				*Pool++ = Normals[i*3+0];
				*Pool++ = Normals[i*3+1];
				*Pool++ = Normals[i*3+2];
			}

			// Vertex colors
			if(CVerts && (vbc.VF&VF_DIFFUSE))
			{
				float Red	= CVerts[i*3+0];
				float Green	= CVerts[i*3+1];
				float Blue	= CVerts[i*3+2];
				udword VColor = ColorRGBA(Red, Green, Blue, 1.0f);
				*Pool++	= *(float*)&VColor;
			}
/*			else if(mColors)
			{
				float Red	= mColors[i].r;
				float Green	= mColors[i].g;
				float Blue	= mColors[i].b;
				udword VColor = ColorRGBA(Red, Green, Blue, 1.0f);
				*Pool++	= *(float*)&VColor;
			}
*/
			// UVs
			if(TVerts)
			{
				uword TRef = TVertsRefs[i];
				Point TextureVector;
				TextureVector.x = TVerts[TRef*2+0];
				TextureVector.y = TVerts[TRef*2+1];
				TextureVector.z = 0.0f;
				Point CoordMaps = TextureVector * TexMat;
				float u = CoordMaps.x;
				float v = CoordMaps.y;
				*Pool++ = (u*ScaleU)+OffsetU;
				*Pool++ = ((v-1.0f)*ScaleV)-OffsetV;
			}
			if(CVerts && (vbc.VF&VF_TEX2))
			{
				*Pool++ = CVerts[i*3+0];
				*Pool++ = CVerts[i*3+1];
			}
			i++;
		}
	}
	mVB->Unlock();

	return true;
}

// Basic mesh render code
bool RMMesh::Render(Renderer* renderer, RMScene* scene)
{
	// Setup the world matrix
	Matrix4x4 World;
	World.World(mPRS);
	renderer->mRS->SetWorldMatrix(0, World);

	// Get the mesh data pointer
	RMMesh* CurMesh = mIsInstance ? (RMMesh*)PTR(mLinkID-1) : this;

	// NB: Character Studio meshes obviously take another code path :)

	// Draw mesh: we need the consolidation results as well as the vertex buffer
	if(CurMesh->mResult)
	{
		uword* VRefs = CurMesh->mResult->Topology.VRefs;
		for(udword i=0;i<CurMesh->mResult->Materials.NbMtls;i++)
		{
			// Select correct material
			udword CurMatID = CurMesh->mResult->Materials.MaterialInfo[i].MatID;
			if(CurMatID==INVALID_ID)
			{
				// Set default material
				Material	Mat0;
				Mat0.mProps.DiffuseColor	= ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f);
				Mat0.mProps.AmbientColor	= ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
				Mat0.mProps.SpecularColor	= ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f);
				Mat0.mProps.EmissiveColor	= ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
				Mat0.mProps.Power			= 0.0f;

				renderer->mRS->SetMaterial(&Mat0);
				renderer->mRS->SetTexturing(false);
				renderer->mRS->SetCullMode(CULL_CCW);
			}
			else
			{
				// Get current material
				RMMaterial* CurMat = (RMMaterial*)PTR(CurMatID);

				// Set material
				renderer->mRS->SetMaterial(CurMat);
				// Set texture
				scene->SelectTexture(CurMat->mTexID);
				// Set cull mode
				renderer->mRS->SetCullMode(CurMat->mCullMode);
			}

			// Draw mesh - this is the useful part for Flexporter users...
			if(CurMesh->mStripRuns)	renderer->DrawIndexedPrimitiveVB(PRIMTYPE_TRISTRIP, CurMesh->mVB, 0, CurMesh->mResult->Geometry.NbVerts, CurMesh->mStripRuns[i], CurMesh->mStripLengths[i]);
			else					renderer->DrawIndexedPrimitiveVB(PRIMTYPE_TRILIST, CurMesh->mVB, 0, CurMesh->mResult->Geometry.NbVerts, VRefs, CurMesh->mResult->Materials.MaterialInfo[i].NbFaces*3);

			VRefs+=CurMesh->mResult->Materials.MaterialInfo[i].NbFaces*3;
		}
	}

	return true;
}
