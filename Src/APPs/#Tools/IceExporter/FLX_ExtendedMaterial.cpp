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
 *	\file		FLX_ExtendedMaterial.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 16, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace IceExporter;

extern ExporterPanel gExporterPanel;

#define FLEXPORTER_MATERIAL_CLASS_ID	Class_ID(0x39465896, 0x27a05f84)
#define PBLOCK_REF			0
#define DECAL_MAP_REF		1
#define DETAIL_MAP_REF		2
#define BILLBOARD_MAP_REF	3

	enum
	{
		cust_attrib_params		// pblock ID
	};

	enum 
	{ 
		FLX_SHADER_FILENAME,
		FLX_USER_PROPS,
		FLX_DECAL_TEXTURE,
		FLX_DETAIL_TEXTURE,
		FLX_BILLBOARD_TEXTURE,
	};

	class FlexporterMaterial : public CustAttrib, public ISubMap
	{
		public:
										FlexporterMaterial();
										~FlexporterMaterial();

		virtual		RefResult			NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, PartID& partID,  RefMessage message){return REF_SUCCEED;}

					int					NumParamBlocks()							{ return 1;														}
					IParamBlock2*		GetParamBlock(int i)						{ if(!i) return pblock; else return null;						} 
					IParamBlock2*		GetParamBlockByID(short id)					{ if(id==cust_attrib_params) return pblock; else return null;	}

					int					NumRefs()									{ return 4;														}
					void				SetReference(int i, RefTargetHandle rtarg);
					RefTargetHandle		GetReference(int i);

		virtual		int					NumSubs();
		virtual		Animatable*			SubAnim(int i);
		virtual		TSTR				SubAnimName(int i);

					SClass_ID			SuperClassID()								{ return CUST_ATTRIB_CLASS_ID;									}
					Class_ID			ClassID()									{ return FLEXPORTER_MATERIAL_CLASS_ID;							}

		// Methods to access texture maps of material
					int					NumSubTexmaps()								{ return 3;														}
					Texmap*				GetSubTexmap(int i);
					void				SetSubTexmap(int i, Texmap *m);
					TSTR				GetSubTexmapSlotName(int i);
		virtual		int					MapSlotType(int i);

					ReferenceTarget*	Clone(RemapDir &remap = NoRemap());

		///////////////////////
		// From CustAttrib

		// "A CustAttrib plugin can implement this method in order to provide the name that gets displayed in the TrackView."
		virtual		TCHAR*				GetName()									{ return "FlexporterMtl";										}
		// "This method gets called when the material or texture is to be displayed in the material editor parameters area. The plug-in should allocate
		// a new instance of a class derived from ParamDlg to manage the user interface."
		virtual		ParamDlg*			CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp);
		// "This method will check if it possible to copy the current custom attributes to the specified custom attributes container."
		virtual		bool				CheckCopyAttribTo(ICustAttribContainer *to)	{ return true;													}

		///////////////////////

		virtual		void*				GetInterface(ULONG id)						{ if(id==I_SUBMAP) return (ISubMap*)this; else return null;		}
		virtual		ReferenceTarget*	GetRefTarget()								{ return this;													}

					void				DeleteThis()								{ delete this;													}

		// Added by Pierre
		override(ReferenceMaker)	IOResult				Load(ILoad *iload);
		override(ReferenceMaker)	IOResult				Save(ISave *isave);

					void				UpdateButtonText();

		// Data access
		inline_		Texmap*				GetDecalTexture()							{ return mDecalMap;		}
		inline_		Texmap*				GetDetailTexture()							{ return mDetailMap;	}
		inline_		Texmap*				GetBillboardTexture()						{ return mBillboardMap;	}

		private:
					IParamBlock2*		pblock;
					Texmap*				mDecalMap;
					Texmap*				mDetailMap;
					Texmap*				mBillboardMap;
		// Internal methods
	};


static TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (gExporterPanel.hInstance)
		return LoadString(gExporterPanel.hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}


	class FlexporterMaterialClassDesc:public ClassDesc2
	{
		public:
		int 			IsPublic()				{ return 1;									}
		void *			Create(BOOL loading)	{ return new FlexporterMaterial;			}
		const TCHAR *	ClassName()				{ return GetString(IDS_FLXMATCLASSNAME);	}
		SClass_ID		SuperClassID()			{ return CUST_ATTRIB_CLASS_ID;				}
		Class_ID 		ClassID()				{ return FLEXPORTER_MATERIAL_CLASS_ID;		}
		const TCHAR* 	Category()				{ return _T("");							}
		const TCHAR*	InternalName()			{ return _T("FlexporterMaterial");			}	// returns fixed parsable name (scripter-visible name)
		HINSTANCE		HInstance()				{ return gExporterPanel.hInstance;			}	// returns owning module handle
	};

	class FLX_Accessor : public PBAccessor
	{
		void Get(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t, Interval &valid){}
		void Set(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t);
	};

	static FLX_Accessor pscm_accessor;

	void FLX_Accessor::Set(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t)
	{
		FlexporterMaterial* MCA = (FlexporterMaterial*)owner;

		if(id==FLX_SHADER_FILENAME)
		{
			// Display the shader's filename on our custom button
			MCA->UpdateButtonText();
		}
		GetCOREInterface()->RedrawViews(GetCOREInterface()->GetTime());
	}








static FlexporterMaterialClassDesc theMatCustAttribDesc;

ClassDesc2* GetFlexporterMaterialDesc()
{
	return &theMatCustAttribDesc;
}

static ParamBlockDesc2 param_blk
(
	/*BlockID ID*/		/*TCHAR* int_name*/				/*int local_name*/		/*ClassDesc2* cd*/		/*BYTE flags*/								/*...*/
	cust_attrib_params, _T("Flexporter parameters"),	0,						&theMatCustAttribDesc,	P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF, 

	// rollout
	IDD_FLEXPORTER_MATERIAL, IDS_FLEXPORTER_MATERIAL, 0, 0, NULL,

	// params

	/* enum */				/* Name */			/* Custom type */	/* Flags */		/* From string table */
	FLX_SHADER_FILENAME,	_T("Filename"), 	TYPE_FILENAME, 		0, 				_T("Filename"),
		p_default, 		"(No shader selected)", 
		p_ui, 			TYPE_FILEOPENBUTTON, IDC_SHADER_FILENAME,
		p_file_types,	IDS_SHADER_TYPES,
//		p_init_file,	IDS_DEFAULT_SHADER,
//		p_caption,		_T("Select shader"),
		p_caption,		IDS_SELECT_SHADER,
//		p_init_file,	_T("DefaultShader.sha"),
//		p_init_file,	IDS_DEFAULT_SHADER,
//		p_file_types,	_T("Shader files (*.sha)|*.sha|All|*.*|"),
//		p_file_types,	IDS_SHADER_TYPES,
		p_accessor,		&pscm_accessor,
		end, 

	FLX_USER_PROPS, 	_T("UserProps"), 		TYPE_STRING, 		0, 				_T("UserProps"),
		p_default, 		"", 
		p_ui, 			TYPE_EDITBOX,			IDC_USER_PROPS,
		end, 

	FLX_DECAL_TEXTURE,	_T("DecalMap"),			TYPE_TEXMAP,		P_OWNERS_REF,	_T("DecalMap"),
		p_refno,		DECAL_MAP_REF,
		p_subtexno,		0,
		p_ui,			TYPE_TEXMAPBUTTON,		IDC_DECAL_TEXTURE, 
		end,

	FLX_DETAIL_TEXTURE,	_T("DetailMap"),		TYPE_TEXMAP,		P_OWNERS_REF,	_T("DetailMap"),
		p_refno,		DETAIL_MAP_REF,
		p_subtexno,		1,
		p_ui,			TYPE_TEXMAPBUTTON,		IDC_DETAIL_TEXTURE, 
		end,

	FLX_BILLBOARD_TEXTURE,	_T("BillboardMap"),	TYPE_TEXMAP,		P_OWNERS_REF,	_T("BillboardMap"),
		p_refno,		BILLBOARD_MAP_REF,
		p_subtexno,		2,
		p_ui,			TYPE_TEXMAPBUTTON,		IDC_BILLBOARD_TEXTURE, 
		end,

		end
);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FlexporterMaterial::FlexporterMaterial()
{
	theMatCustAttribDesc.MakeAutoParamBlocks(this); 
	mDecalMap = null;
	mDetailMap = null;
	mBillboardMap = null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FlexporterMaterial::~FlexporterMaterial()
{
}

ReferenceTarget* FlexporterMaterial::Clone(RemapDir& remap)
{
	FlexporterMaterial* pnew = new FlexporterMaterial;
	pnew->MakeRefByID(FOREVER, 0, remap.CloneRef(pblock));
	BaseClone(this, pnew, remap);
	return pnew;
}

ParamDlg* FlexporterMaterial::CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp)
{
	ParamDlg* Dlg = theMatCustAttribDesc.CreateParamDlgs(hwMtlEdit, imp, this);
	UpdateButtonText();
	return Dlg;
}

void FlexporterMaterial::SetReference(int i, RefTargetHandle rtarg) 
{
	switch(i)
	{
		case PBLOCK_REF:		pblock = (IParamBlock2*)rtarg;
			break;
		case DECAL_MAP_REF:		mDecalMap = (Texmap*)rtarg;
			break;
		case DETAIL_MAP_REF:	mDetailMap = (Texmap*)rtarg;
			break;
		case BILLBOARD_MAP_REF:	mBillboardMap = (Texmap*)rtarg;
			break;
	}
}

RefTargetHandle FlexporterMaterial::GetReference(int i)
{
	switch(i)
	{
		case PBLOCK_REF:		return pblock;
		case DECAL_MAP_REF:		return mDecalMap;
		case DETAIL_MAP_REF:	return mDetailMap;
		case BILLBOARD_MAP_REF:	return mBillboardMap;
		default:				return null;
	}
}	

Texmap* FlexporterMaterial::GetSubTexmap(int i)
{
	if(i==0)	return mDecalMap;
	if(i==1)	return mDetailMap;
	if(i==2)	return mBillboardMap;
				return null;
}

void FlexporterMaterial::SetSubTexmap(int i, Texmap* m)
{
	if(i==0)
	{
		ReplaceReference(DECAL_MAP_REF,m);
//		param_blk.InvalidateUI(ca_color);
	}
	else if(i==1)
	{
		ReplaceReference(DETAIL_MAP_REF,m);
//		param_blk.InvalidateUI(ca_color);
	}
	else if(i==2)
	{
		ReplaceReference(BILLBOARD_MAP_REF,m);
//		param_blk.InvalidateUI(ca_color);
	}
}

TSTR FlexporterMaterial::GetSubTexmapSlotName(int i)
{
	if(i==0) return _T("SubMap");
	if(i==1) return _T("SubMap");
	if(i==2) return _T("SubMap");
	return _T(" ");
}

int FlexporterMaterial::MapSlotType(int i)
{
	return 0;
}

int FlexporterMaterial::NumSubs()  
{
	return pblock->NumSubs()+3;
}

Animatable* FlexporterMaterial::SubAnim(int i)
{
			if(i < pblock->NumSubs())		return pblock->SubAnim(i); 
	else	if(i == pblock->NumSubs())		return mDecalMap;
	else	if(i == (pblock->NumSubs()+1))	return mDetailMap;
	else	if(i == (pblock->NumSubs()+2))	return mBillboardMap;
	else									return null;
}

TSTR FlexporterMaterial::SubAnimName(int i)
{
			if(i < pblock->NumSubs())		return pblock->SubAnimName(i);
	else	if(i == pblock->NumSubs())		return GetSubTexmapTVName(0);
	else	if(i == (pblock->NumSubs()+1))	return GetSubTexmapTVName(1);
	else	if(i == (pblock->NumSubs()+2))	return GetSubTexmapTVName(2);
	else									return TSTR(_T("????"));
}

IOResult FlexporterMaterial::Load(ILoad *iload)
{
	return CustAttrib::Load(iload);
}

IOResult FlexporterMaterial::Save(ISave *isave)
{
	return CustAttrib::Save(isave);
}

void FlexporterMaterial::UpdateButtonText()
{
	IAutoMParamDlg *dlg = pblock->GetMParamDlg();
	IParamMap2 *map = dlg->GetMap();
	if(map)
	{
		TimeValue CurrentTime = 0;
		TCHAR* S = pblock->GetStr(FLX_SHADER_FILENAME, CurrentTime);

		map->SetText(FLX_SHADER_FILENAME, S);
	}
}




















bool IceExporter::AddFlexporterMaterialTo(INode* node)
{
	if(HasCustomMaterial(node, GetString(IDS_FLXMATCLASSNAME)))
	{
//		IceCore::MessageBox(null, "Flexporter material already present", "Oops", MB_OK);
		return false;
	}

	// Catch the node's material pointer
	Mtl* Material = node->GetMtl();
	if(!Material)	return false;

	// Catch the container of custom attributes
	ICustAttribContainer* cc = Material->GetCustAttribContainer();
	if(!cc)
	{
		Material->AllocCustAttribContainer();
		cc = Material->GetCustAttribContainer();
		if(!cc)	return false;
	}

	theHold.Begin(); 

	CustAttrib* CA = (CustAttrib*)CreateInstance(CUST_ATTRIB_CLASS_ID, FLEXPORTER_MATERIAL_CLASS_ID);
	if(!CA)	return false;

	cc->AppendCustAttrib(CA);
	
	theHold.Accept(_T("Add Material Custom Attribute"));

	return true;
}

bool IceExporter::RemoveFlexporterMaterialFrom(INode* node)
{
	int Index;
	if(!HasCustomMaterial(node, GetString(IDS_FLXMATCLASSNAME), &Index))
	{
//		IceCore::MessageBox(null, "Flexporter material already present", "Oops", MB_OK);
		return false;
	}

	// Catch the node's material pointer
	Mtl* Material = node->GetMtl();
	if(!Material)	return false;

	// Catch the container of custom attributes
	ICustAttribContainer* cc = Material->GetCustAttribContainer();
	if(!cc)
	{
		Material->AllocCustAttribContainer();
		cc = Material->GetCustAttribContainer();
		if(!cc)	return false;
	}

	theHold.Begin(); 
	cc->RemoveCustAttrib(Index);
	theHold.Accept(_T("Remove Material Custom Attribute"));

	return true;
}

static FlexporterMaterial* GetFlexporterMaterial(Mtl* material)
{
	if(!material)	return null;

	ICustAttribContainer* cc = material->GetCustAttribContainer();
	if(!cc)	return null;

	int Nb = cc->GetNumCustAttribs();
	for(int i=0;i<Nb;i++)
	{
		CustAttrib* ca = cc->GetCustAttrib(i);
		if(!ca)	continue;

		if(!ca->GetParamBlock(0))	continue;

		ParamBlockDesc2* pd = ca->GetParamBlock(0)->GetDesc();
		if(!pd)	continue;

		if(strcmp(pd->cd->ClassName(), GetString(IDS_FLXMATCLASSNAME))==0)
		{
			return (FlexporterMaterial*)ca;
		}
	}
	return null;
}

static IParamBlock2* GetFlexporterBlock(Mtl* material)
{
/*	if(!material)	return null;

	ICustAttribContainer* cc = material->GetCustAttribContainer();
	if(!cc)	return null;

	int Nb = cc->GetNumCustAttribs();
	for(int i=0;i<Nb;i++)
	{
		CustAttrib* ca = cc->GetCustAttrib(i);
		if(!ca)	continue;

		if(!ca->GetParamBlock(0))	continue;

		ParamBlockDesc2* pd = ca->GetParamBlock(0)->GetDesc();
		if(!pd)	continue;

		if(strcmp(pd->cd->ClassName(), GetString(IDS_FLXMATCLASSNAME))==0)
		{
			return ca->GetParamBlock(0);
		}
	}*/

	FlexporterMaterial* FM = GetFlexporterMaterial(material);

	return FM ? FM->GetParamBlock(0) : null;
}

bool IceExporter::GetShaderFile(Mtl* material, String& shader_file)
{
	IParamBlock2* PB = GetFlexporterBlock(material);
	if(!PB)	return false;

	TimeValue CurrentTime = 0;

	TCHAR* S = PB->GetStr(FLX_SHADER_FILENAME, CurrentTime);
	if(!S)	return false;

	shader_file.Set(S);

	return true;
}

bool IceExporter::GetUserProps(Mtl* material, String& user_props)
{
	IParamBlock2* PB = GetFlexporterBlock(material);
	if(!PB)	return false;

	TimeValue CurrentTime = 0;

	TCHAR* S = PB->GetStr(FLX_USER_PROPS, CurrentTime);
	if(!S)	return false;

	user_props.Set(S);

	return true;
}

Texmap* IceExporter::GetDecalTexture(Mtl* material)
{
	FlexporterMaterial* FM = GetFlexporterMaterial(material);
	if(!FM)	return null;

	return FM->GetDecalTexture();
}

Texmap* IceExporter::GetDetailTexture(Mtl* material)
{
	FlexporterMaterial* FM = GetFlexporterMaterial(material);
	if(!FM)	return null;

	return FM->GetDetailTexture();
}

Texmap* IceExporter::GetBillboardTexture(Mtl* material)
{
	FlexporterMaterial* FM = GetFlexporterMaterial(material);
	if(!FM)	return null;

	return FM->GetBillboardTexture();
}
