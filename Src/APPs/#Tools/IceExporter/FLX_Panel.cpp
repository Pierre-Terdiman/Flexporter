///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	The Flexporter panel.
 *	\file		FLX_Panel.cpp
 *	\author		Pierre Terdiman
 *	\date		April, 10, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

//#include "Y:\APPs\NewPicker\NewPicker.h"
//#include "Y:\APPs\NewPicker\NewPickerDlg.h"
//#include "Y:\APPs\NewPicker\FOColorDialog.h"

using namespace IceExporter;

//! Flexporter version
static const char* gVersion = "Version 1.18";

extern ExporterPanel gExporterPanel;		// The exporter's panel

int				ExporterDesc::IsPublic()			{ return 1;						}	
void*			ExporterDesc::Create(BOOL loading)	{ return &gExporterPanel;		}
const TCHAR*	ExporterDesc::ClassName()			{ return _T("ICE Flexporter");	}	// PlugIn's name in MAX
SClass_ID		ExporterDesc::SuperClassID()		{ return UTILITY_CLASS_ID;		}
Class_ID		ExporterDesc::ClassID()				{ return ICEEXPORTER_CLASS_ID;	}
const TCHAR* 	ExporterDesc::Category()			{ return _T("ICE Flexporter");	}

#ifndef DISCARD_PLUGINS
	// Global stuff
//	bool				gInitDone			= false;
//	udword				gSelectedExporter	= 0;
//	udword				gNbExporters		= 0;
//	FormatDescriptor	gPlugs[256];
	PluginManager		gPlugManager;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExporterPanel::ExporterPanel()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExporterPanel::~ExporterPanel()
{
	gPlugManager.UnloadPlugins();
}

////////////////////////////////////////////////////////////////////////////////
// ExporterPanel Dialog Procedure to handle User and System Interactions
static BOOL CALLBACK ExporterPanelProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef DYNAMIC_MFC
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
	switch (msg)
	{
		// Extra initialization
		case WM_INITDIALOG:
			gExporterPanel.Init(hWnd);
			break;

		// Extra destruction
		case WM_DESTROY:
			gExporterPanel.Destroy(hWnd);
			break;

		// Controls
		case WM_COMMAND :
			switch (LOWORD(wParam))
			{
				case IDC_ABOUT:
				{
//	CNewPickerDlg dlg;
//	int nResponse = dlg.DoModal();

					DoSomeTest();
					// Just create an About dialog and call it.
					IceAbout Dlg;
					Dlg.m_Version = gVersion;
					Dlg.CenterWindow();
					Dlg.DoModal();
					break;
				}

				case IDC_ADDUSERPROP:
				{
					MAXNodes Selection;
					if(GetSelectedNodes(gExporterPanel.ip, Selection))
					{
						if(!Selection.GetNbNodes())
						{
							IceCore::MessageBox(null, "Please select some nodes...", "Message", MB_OK);
						}
						else
						{
							FLX_EditBox Dlg;
							Dlg.m_Msg0 = "Enter a string. It will be added to user properties of selected nodes.";
							Dlg.CenterWindow();
							if(Dlg.DoModal()==1)	// "1" ? => IDOK
							{
								char NextLine[3] = { 0x0d, 0x0a, 0x00 };

								// Loop through selected nodes
								for(udword i=0;i<Selection.GetNbNodes();i++)
								{
									// Catch current user properties
									TSTR UserProps;
									Selection.GetNode(i)->GetUserPropBuffer(UserProps);
									if(UserProps.data())
									{
										udword Length = UserProps.Length();
										if(Length)
										{
											// Check last chars
											if(UserProps.data()[Length-1]!=0x0a || UserProps.data()[Length-2]!=0x0d)
											{
												UserProps += CStr(NextLine);
											}
										}
									}
									UserProps += CStr(Dlg.m_EditedString);
									UserProps += CStr(NextLine);

									// Setup new string
									Selection.GetNode(i)->SetUserPropBuffer(UserProps);
//									Selection.GetNode(i)->SetUserPropString("StringKey", Dlg.m_EditedString);
								}
							}
						}
					}
					break;
				}

				case IDC_OPTIONS:
				{
#ifndef DISCARD_PLUGINS
					// Load format plug-ins
					if(!gPlugManager.LoadPlugins(gExporterPanel.ip->GetDir(APP_MAXROOT_DIR)))
					{
						::MessageBox(null, "No exporter plug-ins found!", "Oops", 0);
						return TRUE;
					}

					// Display options panel
					IceOptions Dlg;
					Dlg.CenterWindow();
					Dlg.DoModal();
#endif
					break;
				}

				case IDC_COPY:
				{
					char DestPath[1024];
					BROWSEINFO		bi;
					ITEMIDLIST*		id;
					bi.hwndOwner		= null;
					bi.pidlRoot			= null;
					bi.pszDisplayName	= DestPath;
					bi.lpszTitle		= "Select the destination folder";
					bi.ulFlags			= 0;
					bi.lpfn				= 0;
					id = IceCore::SHBrowseForFolder(&bi);
					if(id)	// if id is null, then the user pressed CANCEL
					{
						IceCore::SHGetPathFromIDList(id, DestPath);

						udword NbCopied=0;
						udword NbNotFound=0;
						udword NbFailed=0;
						CopyTextures(gExporterPanel.ip->GetRootNode(), NbCopied, NbNotFound, NbFailed, DestPath);
						::MessageBox(null, _F("%d textures copied.\n%d textures not found.\n%d copies failed (possibly too long path)", NbCopied, NbNotFound, NbFailed), "Done", MB_OK);
					}
					break;
				}

				case IDC_EXPORT:
				{
					#ifndef DISCARD_PLUGINS
						if(!gPlugManager.LoadPlugins(gExporterPanel.ip->GetDir(APP_MAXROOT_DIR)))
						{
							::MessageBox(null, "No exporter plug-ins found!", "Oops", MB_OK);
							return TRUE;
						}
					#endif

					// Come up with a correct filename
					String Filename("scene");

					const char* Suggested = gPlugManager.GetSelectedExporter().GetPlug()->SuggestedFilename();
					if(Suggested)
					{
						Filename = Suggested;
					}
					else
					{
	//					Load("Flexporter", "Parameters", "Filename", Filename);
						gPlugManager.GetSelectedExporter().GetLastSavedFilename(Filename);

						{
							// 1.17: use current MAX filename
							TSTR& MAXFilename = gExporterPanel.ip->GetCurFileName();
							if(MAXFilename.Length())
							{
								const char* Data = MAXFilename.data();
								if(Data)
								{
									// We want to keep last used path if possible!
									String Path;
									if(GetPath(Filename, Path))
									{
										Filename = Path;
		//								Filename += "\\";
										Filename += Data;
									}
									else
									{
										Filename.Set(Data);
									}

									// Now adapt filename's extension according to current format
									RemoveExtension(Filename);
									Filename += ".";
									Filename += gPlugManager.GetSelectedExporter().GetExtension();
								}
							}
						}
					}

					#ifndef DISCARD_PLUGINS
					String	Descriptor("Flexporter file (*.");
//					Descriptor += gFormats[gSelectedExporter];
					Descriptor += gPlugManager.GetSelectedExporter().GetExtension();
					Descriptor += ")|*.";
//					Descriptor += gFormats[gSelectedExporter];
					Descriptor += gPlugManager.GetSelectedExporter().GetExtension();
					Descriptor += "||";
//					if(FileSelect(gFormats[gSelectedExporter], Filename, Descriptor, false, Filename))
					if(FileSelect(gPlugManager.GetSelectedExporter(), gPlugManager.GetSelectedExporter().GetExtension(), Filename, Descriptor, false, Filename))
					#else
					if(FileSelect(gPlugManager.GetSelectedExporter(), "pipo", (const char*)"scene", "pipo", false, Filename))
					#endif
					{
						gExporterPanel.Export(Filename, true);
//						Save("Flexporter", "Parameters", "Filename", Filename);
						gPlugManager.GetSelectedExporter().SaveLastFilename(Filename);

						// Auto-unload if needed
//						if(gUserSettings[gSelectedExporter]->mAutoUnload)	FreeLibs();
						if(gPlugManager.GetSelectedExporter().GetSettings()->mAutoUnload)	gPlugManager.UnloadPlugins();
					}
					break;
				}

				case IDC_EXP_BIPED:
/*					{
						long SaveFormat = g_ValidExportSettings->OutputFormat;
						g_ValidExportSettings->OutputFormat = ES_FORMAT_MOTION;
						thePlugPanel.PerformExport();
						g_ValidExportSettings->OutputFormat = SaveFormat;
					}
					break;*/
				{
					// Exports a motion file
					#ifndef DISCARD_PLUGINS
					// Load exporters
					if(!gPlugManager.LoadPlugins(gExporterPanel.ip->GetDir(APP_MAXROOT_DIR)))
					{
						::MessageBox(null, "No exporter plug-ins found!", "Oops", 0);
						return TRUE;
					}
					#endif
					String	Filename("scene");
//					Load("Flexporter", "Parameters", "Filename", Filename);
					gPlugManager.GetSelectedExporter().GetLastSavedFilename(Filename);

					#ifndef DISCARD_PLUGINS
					String	Descriptor("Flexporter file (*.");
//					Descriptor += gFormats[gSelectedExporter];
					Descriptor += gPlugManager.GetSelectedExporter().GetExtension();
					Descriptor += ")|*.";
//					Descriptor += gFormats[gSelectedExporter];
					Descriptor += gPlugManager.GetSelectedExporter().GetExtension();
					Descriptor += "||";
//					if(FileSelect(gFormats[gSelectedExporter], Filename, Descriptor, false, Filename))
					if(FileSelect(gPlugManager.GetSelectedExporter(), gPlugManager.GetSelectedExporter().GetExtension(), Filename, Descriptor, false, Filename))
					#else
					if(FileSelect(gPlugManager.GetSelectedExporter(), "pipo", (const char*)"scene", "pipo", false, Filename))
					#endif
					{
						gExporterPanel.Export(Filename, false);
//						Save("Flexporter", "Parameters", "Filename", Filename);
						gPlugManager.GetSelectedExporter().SaveLastFilename(Filename);

						// Auto-unload if needed
//						if(gUserSettings[gSelectedExporter]->mAutoUnload)	FreeLibs();
						if(gPlugManager.GetSelectedExporter().GetSettings()->mAutoUnload)	gPlugManager.UnloadPlugins();
					}
					break;
				}

				case IDC_UPDATE:
				{
					// Free libs so that they'll get automatically reloaded for the next export
					gPlugManager.UnloadPlugins();
					::MessageBox(null, "Flexporter plug-ins have been unloaded.\nYou can safely recompile your .flx file without leaving MAX.", "Message", MB_OK);
				}
				break;

				case IDC_CUSTOM_MATERIAL:
				{
					gExporterPanel.AddCustomMaterial(FALSE);
				}
				break;

				case IDC_PRECOMPUTE_LIGHTING:
				{

#ifdef FLEXPORTER_SUPPORT_PRELIGHTER

					VertexLighterDlg Dlg;
					// Default settings used the first time
					Dlg.m_ColorSmoothing	= 3;
					Dlg.m_ComputeShadows	= TRUE;
					Dlg.m_Specular			= FALSE;
					Dlg.m_Clamp				= TRUE;
					// Then we use registry settings
					Dlg.LoadSettings();
					Dlg.CenterWindow();
					if(Dlg.DoModal()==1)
					{
						Dlg.SaveSettings();

						VertexPrelighter LM;
						LM.SetSmoothing(Dlg.m_ColorSmoothing);
						LM.SetShadows(Dlg.m_ComputeShadows!=0);
						LM.SetSpecular(Dlg.m_Specular!=0);
						LM.SetClamping(Dlg.m_Clamp!=0);

						// Setup lightmapper
				//		LM->SetShadows(UserSettings.ComputeShadows);

						ExportSettings UserSettings;
						// 
						MAXConversionSettings cs;
						cs.mExporter		= &LM;
						cs.mIp				= gExporterPanel.ip;
						cs.mRootNode		= gExporterPanel.ip->GetRootNode();
						cs.mSelectedNodes	= null;
						cs.mSettings		= LM.OverrideSettings(&UserSettings);
//						cs.mOutputName		= null;
						cs.mSceneFile		= true;
#ifdef SUPPORT_OLD_LIGHTMAPPER
						cs.mLightmapper		= null;
						cs.mLightmapPass	= true;
#endif
						MAXConverter Cvt;
						Cvt.Convert(&cs);

//						gExporterPanel.ip->RedrawViews(0);    // why doesn't this work ?
//						gExporterPanel.ip->ForceCompleteRedraw();
					}
#endif
				}
				break;

				case IDC_ADD_FLX_MATERIAL:
				{
					MAXNodes Selection;
					if(GetSelectedNodes(gExporterPanel.ip, Selection))
					{
						if(!Selection.GetNbNodes())
						{
							IceCore::MessageBox(null, "Please select some nodes...", "Message", MB_OK);
						}
						else
						{
							for(udword i=0;i<Selection.GetNbNodes();i++)
							{
								INode* CurNode = Selection.GetNode(i);
								AddFlexporterMaterialTo(CurNode);
							}
						}
					}
				}
				break;

				case IDC_REMOVE_FLX_MATERIAL:
				{
					MAXNodes Selection;
					if(GetSelectedNodes(gExporterPanel.ip, Selection))
					{
						if(!Selection.GetNbNodes())
						{
							IceCore::MessageBox(null, "Please select some nodes...", "Message", MB_OK);
						}
						else
						{
							for(udword i=0;i<Selection.GetNbNodes();i++)
							{
								INode* CurNode = Selection.GetNode(i);
								RemoveFlexporterMaterialFrom(CurNode);
							}
						}
					}
				}
				break;

			}
			break;

		////////////////////////////////////////////////////////////////////////
		//The following lines are not necessary for MAX2 Plugins
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
			gExporterPanel.ip->RollupMouseMessage(hWnd, msg, wParam, lParam);
			break;
		////////////////////////////////////////////////////////////////////////

		default:
			return FALSE;
		}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// Display the Plugin's Panel
void ExporterPanel::BeginEditParams(Interface* ip, IUtil* iu)
{
	this->ip = ip;		// Get the Plugin's Interface
	this->iu = iu;		// Get the Utility Plugin's Interface

	// Display the plugin's Rollup Dialog
	hPanel = ip->AddRollupPage(hInstance, MAKEINTRESOURCE(IDD_UTILITY_PANEL), ExporterPanelProc, _T("Ice Flexporter"), 0);		// Plug's name in MAX, on the right
}

////////////////////////////////////////////////////////////////////////////////
// Free the Plugins' Panel
void ExporterPanel::EndEditParams(Interface* ip, IUtil* iu)
{
	this->iu = null;
	this->ip = null;

	// Remove the Rollup Dialog
	ip->DeleteRollupPage(hPanel);
	hPanel = null;
}

////////////////////////////////////////////////////////////////////////////////
// Panel Initialization callback
void ExporterPanel::Init(HWND hwnd)
{
}

////////////////////////////////////////////////////////////////////////////////
// Panel Destruction callback
void ExporterPanel::Destroy(HWND hwnd)
{
}


////////////////////////////////////////////////////////////////////////////////
// Called when the Export Button is pushed
ExporterPanel& ExporterPanel::Export(const String& filename, bool scene_file)
{
	// We use a local copy of settings since some values
	// can get modified by current scene properties
	ExportSettings UserSettings;

#ifndef DISCARD_PLUGINS
//	ExportFormat*	UserExporter	= gUserExporters[gSelectedExporter];
//	ExportSettings*	UserSettings	= gUserSettings[gSelectedExporter];
	ExportFormat* UserExporter = gPlugManager.GetSelectedExporter().GetPlug();
	UserSettings = *gPlugManager.GetSelectedExporter().GetSettings();
#else
	ExportFormat* UserExporter = null;
//	ExportSettings*	UserSettings	= new ExportSettings;
#endif

	if(UserExporter)
	{
		UserExporter->hInstance = hInstance;
		UserExporter->hWnd = hPanel;
	}

//	ExportFormat*	UserExporter	= null;
//	ExportSettings*	UserSettings	= new ExportSettings();

//	UserExporter	= null;



#ifdef SUPPORT_OLD_LIGHTMAPPER
	// Lightmapper stuff
	Lightmapper* LM = null;
	if(scenefile && UserSettings.mComputeVtxColors)
	{
		LM					= new Lightmapper;

		// Setup lightmapper
//		LM->SetShadows(UserSettings.ComputeShadows);

		// 
		MAXConversionSettings cs;
		cs.mExporter		= LM;
		cs.mIp				= ip;
		cs.mRootNode		= ip->GetRootNode();
		cs.mSelectedNodes	= null;
		cs.mSettings		= LM->OverrideSettings(&UserSettings);
		cs.mOutputName		= filename;
		cs.mLightmapper		= null;
		cs.mSceneFile		= scenefile;
		cs.mLightmapPass	= true;

		MAXConverter Cvt;
		Cvt.Convert(&cs);
		// NB: here, consolidated meshes are discarded, and saved pointers in the LM are no more valid.
		// But since the lightmapping has already been computed, we don't care.
	}
#endif

	// If user wants to export the whole hierarchy, then get root node
	// else ask for wanted node(s).
	if(!UserSettings.mExportWholeScene && scene_file)
	{
		Selection SelectedNodes;

		if(ip->DoHitByNameDialog(&SelectedNodes))
		{
			MAXConversionSettings cs;
			cs.mExporter		= UserExporter;
			cs.mIp				= ip;
			cs.mRootNode		= ip->GetRootNode();
			cs.mSelectedNodes	= &SelectedNodes;
			cs.mSettings		= &UserSettings;
			cs.mOutputName		= filename;
			cs.mSceneFile		= scene_file;
#ifdef SUPPORT_OLD_LIGHTMAPPER
			cs.mLightmapper		= LM;
			cs.mLightmapPass	= false;
#endif
			MAXConverter Cvt;
			Cvt.Convert(&cs);
		}
	}
	else
	{
		MAXConversionSettings cs;
		cs.mExporter		= UserExporter;
		cs.mIp				= ip;
		cs.mRootNode		= ip->GetRootNode();
		cs.mSelectedNodes	= null;
		cs.mSettings		= &UserSettings;
		cs.mOutputName		= filename;
		cs.mSceneFile		= scene_file;
#ifdef SUPPORT_OLD_LIGHTMAPPER
		cs.mLightmapper		= LM;
		cs.mLightmapPass	= false;
#endif
		MAXConverter Cvt;
		Cvt.Convert(&cs);
	}

#ifdef SUPPORT_OLD_LIGHTMAPPER
	DELETESINGLE(LM);
#endif

	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A method to know whether a node was selected for export by user, or not.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input	:	node				= a MAX node
// Output	:	-
// Return	:	true if selected
// Remark	:	this one is a O(N^2) process, so we might recode it one day.
bool Selection::Contains(INode* node)
{
	for(udword i=0;i<(udword)NodeTab.Count();i++)
	{
		if(node==*NodeTab.Addr(i)) return true;
	}
	return false;
}








/*
void IceExporter::LoadLibs(Interface* ip)
{
#ifndef DISCARD_PLUGINS
	if(!gInitDone)
	{
		char Dir[1024], DllPath[1024];
		strcpy(Dir, "c:\\");
		strcpy(DllPath, "c:\\*.flx");
		if(ip)
		{
			strcpy((char*)Dir, ip->GetDir(APP_MAXROOT_DIR));
			strcpy(DllPath, Dir);
			strcat(DllPath, "\\*.flx");
		}

		// Look for exporters
		gNbExporters = 0;

		STARTFINDFILES(DllPath);

		char* ExporterName = FILEFOUND;

		strcpy(DllPath, Dir);
		strcat(DllPath, ExporterName);

		if(gPlugs[gNbExporters].LoadPlug(DllPath))
		{
			gNbExporters++;
		}

		ENDFINDFILES;

		gInitDone = true;

		// Check selected exporter is still ok
		if(gSelectedExporter>=gNbExporters)	gSelectedExporter = 0;

		// Try to get back most recently used exporter
		String LastExporter;
		Load("Flexporter", "LastExporter", "ExporterName", LastExporter);
		if(LastExporter.IsValid())
		{
			for(udword i=0;i<gNbExporters;i++)
			{
				if(strcmp(LastExporter, gPlugs[i].GetName())==0)
				{
					gSelectedExporter = i;
					break;
				}
			}
		}
	}
#endif
}

void IceExporter::FreeLibs()
{
#ifndef DISCARD_PLUGINS
	for(udword i=0;i<gNbExporters;i++)
	{
		gPlugs[i].UnloadPlug();
	}

//	gSelectedExporter	= 0;	### don't reset this one!
	gNbExporters		= 0;
	gInitDone			= false;
#endif
}
*/