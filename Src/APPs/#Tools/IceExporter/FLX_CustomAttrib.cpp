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

extern ExporterPanel gExporterPanel;

#define MCA_CLASS_ID Class_ID(0x5a05193c, 0x70420c1f)
#define PBLOCK_REF 0
#define MAP_REF 1

	enum { cust_attrib_params };  // pblock ID

	enum 
	{ 
		ca_submap, 
		ca_color,
		ca_checkbox,
		ca_edit,
		ca_filename,
		ca_edit_text
	};

	class MatCustAttrib : public CustAttrib, public ISubMap
	{
					IParamBlock2*		pblock;
					Texmap*				submap;
		public:
					MatCustAttrib();
					~MatCustAttrib();

					String				mFilename;
					void				UpdateButtonText();

		virtual		RefResult			NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, PartID& partID,  RefMessage message){return REF_SUCCEED;}

					int					NumParamBlocks()							{ return 1; }			
					IParamBlock2*		GetParamBlock(int i)						{ if(i == 0) return pblock; else return NULL;} 
					IParamBlock2*		GetParamBlockByID(short id)					{ if(id == cust_attrib_params ) return pblock; else return NULL;} 

					int					NumRefs()									{ return 2;}
					void				SetReference(int i, RefTargetHandle rtarg);
					RefTargetHandle		GetReference(int i);

		virtual		int					NumSubs();
		virtual		Animatable*			SubAnim(int i);
		virtual		TSTR				SubAnimName(int i);

					SClass_ID			SuperClassID()								{ return CUST_ATTRIB_CLASS_ID;									}
					Class_ID			ClassID()									{ return MCA_CLASS_ID;											}

		// Methods to access texture maps of material
					int					NumSubTexmaps()								{ return 1;														}
					Texmap*				GetSubTexmap(int i)							{ if(i == 0) return submap; else return NULL;					}
					void				SetSubTexmap(int i, Texmap *m);
					TSTR				GetSubTexmapSlotName(int i)					{ if(i==0) return _T("SubMap"); else return _T(" ");			}
		virtual		int					MapSlotType(int i)							{ return 0;														}

					ReferenceTarget*	Clone(RemapDir &remap = NoRemap());

		///////////////////////
		// From CustAttrib

		// "A CustAttrib plugin can implement this method in order to provide the name that gets displayed in the TrackView."
		virtual		TCHAR*				GetName()									{ return "DynamicMtl";											}
		// "This method gets called when the material or texture is to be displayed in the material editor parameters area. The plug-in should allocate
		// a new instance of a class derived from ParamDlg to manage the user interface."
		virtual		ParamDlg*			CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp);
		// "This method will check if it possible to copy the current custom attributes to the specified custom attributes container."
		virtual		bool				CheckCopyAttribTo(ICustAttribContainer *to)	{ return true;													}

		///////////////////////

		virtual		void*				GetInterface(ULONG id)						{ if(id==I_SUBMAP) return (ISubMap*)this; else return NULL;		}
		virtual		ReferenceTarget*	GetRefTarget()								{ return this;													}

					void				DeleteThis()								{ delete this;													}

		// Added by Pierre
		override(ReferenceMaker)	IOResult				Load(ILoad *iload);
		override(ReferenceMaker)	IOResult				Save(ISave *isave);


	};




static TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (gExporterPanel.hInstance)
		return LoadString(gExporterPanel.hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}


class MatCustAttribClassDesc:public ClassDesc2
{
	public:
	int 			IsPublic()				{ return 1;								}
	void *			Create(BOOL loading)	{ return new MatCustAttrib;				}
	const TCHAR *	ClassName()				{ return GetString(IDS_MATCACLASSNAME);	}
	SClass_ID		SuperClassID()			{ return CUST_ATTRIB_CLASS_ID;			}
	Class_ID 		ClassID()				{ return MCA_CLASS_ID;					}
	const TCHAR* 	Category()				{ return _T("");						}
	const TCHAR*	InternalName()			{ return _T("MatCustAttrib");			}	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance()				{ return gExporterPanel.hInstance;		}	// returns owning module handle
/*	HINSTANCE		HInstance()				{
		return GetModuleHandle("IceExporter.dlu");
	}	// returns owning module handle
*/

};




	class PSCM_Accessor : public PBAccessor
	{
		void Get(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t, Interval &valid){}
		void Set(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t);

//		TSTR	GetLocalName(ReferenceMaker* owner, ParamID id, int tabIndex);
	};

	static PSCM_Accessor pscm_accessor;

	//--- PSCM_Accessor -------------------------------------------------------

	void PSCM_Accessor::Set(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t)
	{
		MatCustAttrib* MCA = (MatCustAttrib*)owner;
		if(id==ca_filename)
		{
			MCA->mFilename = v.s;
			MCA->UpdateButtonText();
		}
/*		PSCubeMap *map = (PSCubeMap*) owner;
		switch(id)
		{
		case pb_cubemapfile: map->SetCubeMapFile(v.s); break;
		case pb_vsfile: map->SetVertexShaderFile(v.s); break;
		default: break;
		}*/
		GetCOREInterface()->RedrawViews(GetCOREInterface()->GetTime());
	}

/*	TSTR PSCM_Accessor::GetLocalName(ReferenceMaker* owner, ParamID id, int tabIndex)
	{
		MatCustAttrib* MCA = (MatCustAttrib*)owner;
		if(id==ca_filename)
		{
			return MCA->mFilename;
		}
	}
*/








static MatCustAttribClassDesc theMatCustAttribDesc;

ClassDesc2* GetMatCustAttribDesc(){ return &theMatCustAttribDesc;}


//	PB2Export   ParamBlockDesc2(BlockID ID, TCHAR* int_name, StringResID local_name, ClassDesc2* cd, USHORT flags, ...); 

static ParamBlockDesc2 param_blk
(
	/*BlockID ID*/		/*TCHAR* int_name*/		/*int local_name*/		/*ClassDesc2* cd*/		/*BYTE flags*/								/*...*/
	cust_attrib_params, _T("parameters"),		0,						&theMatCustAttribDesc,	P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF, 

	// rollout
	IDD_CUSTOM_MATERIAL, IDS_CUSTOM_MATERIAL, 0, 0, NULL,

	// params

	/* enum */	/* Name */				/* Custom type */	/* Flags */		/* From string table */
	ca_color,	_T("CustAttribColor"),	TYPE_RGBA,			P_ANIMATABLE,	IDS_COLOR,
		p_default,		Color(1.0,1.0,1.0), 
		p_ui,			TYPE_COLORSWATCH, IDC_COLOR, 
		end,

	ca_submap,	_T("CustAttribSubMap"),	TYPE_TEXMAP,		P_OWNERS_REF,	IDS_SUBMAP,
//	ca_submap,	_T("CustAttribSubMap"),	TYPE_BITMAP,		P_OWNERS_REF,	IDS_SUBMAP,
//	ca_submap,	_T("CustAttribSubMap"),	TYPE_FILENAME,		P_OWNERS_REF,	IDS_SUBMAP,
		p_refno,		MAP_REF,
		p_subtexno,		0,
		p_ui,			TYPE_TEXMAPBUTTON, IDC_SUBMAP, 
		end,

	ca_checkbox,_T("CustCheckbox"),		TYPE_BOOL,			P_ANIMATABLE,	_T("Checkbox"),
		p_default,		FALSE, 
		p_ui,			TYPE_SINGLECHEKBOX, IDC_CHECKBOX, 
		end,

	ca_edit, 	_T("Edit"), 			TYPE_INT, 		P_ANIMATABLE, 	_T("Edit"),
		p_default, 		4, 
		p_range, 		0,	10, 
		p_ui, 			TYPE_SPINNER, EDITTYPE_INT, IDC_EDIT, IDC_SPINNER, 0.05f, 
		end, 

	ca_filename, 	_T("Filename"), 	TYPE_FILENAME, 	0, 	_T("Filename"),
		p_default, 		"default.txt", 
		p_ui, 			TYPE_FILEOPENBUTTON, IDC_FILENAME,
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

	ca_edit_text, 	_T("EditText"), 	TYPE_STRING, 	0, 	_T("EditText"),
		p_default, 		"pipo.txt", 
		p_ui, 			TYPE_EDITBOX, IDC_EDITTEXT,
		end, 

		end
		);

MatCustAttrib::MatCustAttrib()
{
//TCHAR* T = GetString(IDS_COLOR);

	theMatCustAttribDesc.MakeAutoParamBlocks(this); 
	submap = NULL;
}

MatCustAttrib::~MatCustAttrib()
{
	DebugPrint("MAT Destructor\n");
}



ReferenceTarget *MatCustAttrib::Clone(RemapDir &remap)
{
	MatCustAttrib *pnew = new MatCustAttrib;
	pnew->MakeRefByID(FOREVER,0,remap.CloneRef(pblock));
	BaseClone(this, pnew, remap);
	return pnew;
}


ParamDlg *MatCustAttrib::CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp)
{
	ParamDlg* Dlg = theMatCustAttribDesc.CreateParamDlgs(hwMtlEdit, imp, this);
	UpdateButtonText();
	return Dlg;
}

void MatCustAttrib::SetReference(int i, RefTargetHandle rtarg) 
{
	switch(i)
	{
		case PBLOCK_REF: pblock = (IParamBlock2 *)rtarg;
			break;
		case MAP_REF : submap = (Texmap *) rtarg;
			break;
	}
}

RefTargetHandle MatCustAttrib::GetReference(int i)
{
	switch(i)
	{
		case PBLOCK_REF: return pblock;
		case MAP_REF : return submap;
		default: return NULL;
	}
}	
void MatCustAttrib::SetSubTexmap(int i, Texmap *m)
{
	if (i==0)
	{
		ReplaceReference(MAP_REF,m);
		param_blk.InvalidateUI(ca_color);
	}
}

int MatCustAttrib::NumSubs()  
{
	return pblock->NumSubs()+1; 
}

Animatable* MatCustAttrib::SubAnim(int i)
{
	if(i < pblock->NumSubs())
		return pblock->SubAnim(i); 
	else if(i == pblock->NumSubs())
		return submap;
	else
		return NULL;
}

TSTR MatCustAttrib::SubAnimName(int i)
{
	if(i < pblock->NumSubs())
		return pblock->SubAnimName(i);
	else if(i == pblock->NumSubs())
		return GetSubTexmapTVName(0);
	else
		return TSTR(_T("????"));


}

IOResult MatCustAttrib::Load(ILoad *iload)
{
	return CustAttrib::Load(iload);
}

IOResult MatCustAttrib::Save(ISave *isave)
{
	return CustAttrib::Save(isave);
}

void MatCustAttrib::UpdateButtonText()
{
	IAutoMParamDlg *dlg = pblock->GetMParamDlg();
	IParamMap2 *map = dlg->GetMap();
	if(map)
	{

		TimeValue CurrentTime = 0;
		TCHAR* S = pblock->GetStr(4, CurrentTime);

		map->SetText(ca_filename, S);
//		map->SetText(ca_filename, mFilename);
	}
}
