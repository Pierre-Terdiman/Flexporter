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


#define MCA_CLASS_ID Class_ID(0x5a05193c, 0x70420c1f)	// ###
//#define MCA_CLASS_ID Class_ID(0x5a05193c, 0x70420c1e)	// ###

static void DisplayCA(MtlBase* mtl)
{
	ICustAttribContainer* cc = mtl->GetCustAttribContainer();
	if(!cc)	return;

	for(int i=0;i<cc->GetNumCustAttribs();i++)
	{
		CustAttrib* ca = cc->GetCustAttrib(i);

		ParamBlockDesc2* pd = ca->GetParamBlock(0)->GetDesc();

		char *test = (char*) pd->cd->ClassName();
		if(strcmp(pd->cd->ClassName(), "CustomMaterial")==0)
		{
			Interval Inter = FOREVER;
			TimeValue CurrentTime = 0;

			IParamBlock2* PB = ca->GetParamBlock(0);
			if(PB)
			{
				int NbParams = PB->NumParams();
				for(int p=0;p<NbParams;p++)
				{
					int Value;
					PB->GetValue(p, CurrentTime, Value, Inter);
				}

//				Color c;
//				PB->GetValue(0,	CurrentTime, c, Inter);

				Texmap* T = PB->GetTexmap(0, CurrentTime);
				Color C = PB->GetColor(1,	CurrentTime);

				int CheckBox;
				PB->GetValue(2, CurrentTime, CheckBox, Inter);

				int Edit;
				PB->GetValue(3, CurrentTime, Edit, Inter);

				TCHAR* S = PB->GetStr(4, CurrentTime);

				TCHAR* S2 = PB->GetStr(5, CurrentTime);

			}
			return;
		}
	}
}

bool ExporterPanel::HasCustomMaterial(ICustAttribContainer* cc)//, int which, BOOL Mat)
{
//	HWND hWndList = GetDlgItem(hPanel,IDC_CALIST);

	for(int i=0;i<cc->GetNumCustAttribs();i++)
	{
		CustAttrib* ca = cc->GetCustAttrib(i);

		ParamBlockDesc2* pd = ca->GetParamBlock(0)->GetDesc();

/*		if(!Mat)
		{
			TCHAR  buf[50];
			ListBox_GetText(hWndList, which, &buf);
			if(!strcmp(pd->cd->ClassName(),buf))
			{
				return i;
			}
		}
		else
		{
			TCHAR  buf[50];
			ListBox_GetText(hWndList, which, &buf);
			char *test = (char*) pd->cd->ClassName();
			if(!strcmp(pd->cd->ClassName(),buf))
			{
				return i;
			}

		}
*/
		char *test = (char*) pd->cd->ClassName();
		if(strcmp(pd->cd->ClassName(), "CustomMaterial")==0)	// ###
		{
			return true;
		}

	}
	return false; // Not loaded
}

void ExporterPanel::AddCustomMaterial(BOOL Insert)
{
	// Get an interface to the Material Editior
	IMtlEditInterface* mtlEdit = (IMtlEditInterface*)GetCOREInterface(MTLEDIT_INTERFACE);
	if(!mtlEdit)	return;

	// Grap the current material
	MtlBase* mtl = mtlEdit->GetCurMtl();
	if(!mtl)	return;

	ICustAttribContainer* cc = mtl->GetCustAttribContainer();
	if(!cc)
	{
		mtl->AllocCustAttribContainer();
		cc = mtl->GetCustAttribContainer();
	}

/*	HWND hWndList = GetDlgItem(hPanel, IDC_CALIST);

	int curSel = ListBox_GetCurSel(hWndList);
	int index = CheckCCIsLoaded(cc,curSel,TRUE);

	if(index != INVALID_ID)
	{
		IceCore::MessageBox(hPanel, "CC ALready Present,Not Loading" ,"Warning",NULL);
		return;
	}
	theHold.Begin(); 

	if(!Insert)
		cc->AppendCustAttrib((CustAttrib *)CreateInstance(CUST_ATTRIB_CLASS_ID, MCA_CLASS_ID));
	else
		cc->InsertCustAttrib(0,(CustAttrib *)CreateInstance(CUST_ATTRIB_CLASS_ID, MCA_CLASS_ID));
	
	theHold.Accept(_T("Add Material Custom Attribute"));*/


	if(HasCustomMaterial(cc))
	{
		IceCore::MessageBox(null, "Custom material already present", "Oops", MB_OK);
DisplayCA(mtl);
		return;
	}
	theHold.Begin(); 

	CustAttrib* CA = (CustAttrib *)CreateInstance(CUST_ATTRIB_CLASS_ID, MCA_CLASS_ID);

//	MatCustAttrib* MA = (MatCustAttrib*)CA;

	if(!Insert)
		cc->AppendCustAttrib(CA);
	else
		cc->InsertCustAttrib(0, CA);
	
	theHold.Accept(_T("Add Material Custom Attribute"));
}

/*
void DynPBlock::RemoveMat()
{
	HWND hWndList;

	IMtlEditInterface *mtlEdit = (IMtlEditInterface *)GetCOREInterface(MTLEDIT_INTERFACE);
	
	if(!mtlEdit)
		return;
	
	MtlBase *mtl = mtlEdit->GetCurMtl();
	
	if(!mtl)
		return;
	
	ICustAttribContainer* cc = mtl->GetCustAttribContainer();
	if(cc!=NULL)
	{
		hWndList = GetDlgItem(hPanel,IDC_CALIST);
		int curSel = ListBox_GetCurSel(hWndList);

		int index = CheckCCIsLoaded(cc,curSel,TRUE);

		// Make sure we only remove ourselves and not other CAs
		if(index != CC_NOT_LOADED){
			theHold.Begin(); 
			cc->RemoveCustAttrib(index);
			theHold.Accept(_T("Remove Material Custom Attribute"));
		}
		else
			MessageBox(hPanel,"Nothing to Remove","Warning",NULL);
		
	}

}

*/