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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Displays a standard file-select dialog.
 *	\param		extension		[in] file extension
 *	\param		destname		[in] default destination name
 *	\param		alias			[in] alias string
 *	\param		mode			[in] false (LOAD), true (SAVE)
 *	\param		filename		[out] user-returned filename
 *	\return		true if selected file must be processed, else false to abort
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IceExporter::FileSelect(FormatDescriptor& desc, const char* extension, const char* destname, const char* alias, bool mode, String& filename)
{
	// Fileselect override
	if(desc.GetPlug() && desc.GetPlug()->UseCustomFileselect())
	{
		return desc.GetPlug()->FileSelect(extension, destname, alias, mode, filename);
	}
/*
#ifdef DYNAMIC_MFC
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
*/
	// Setup flags
	long Flags = mode ? OFN_PATHMUSTEXIST|OFN_HIDEREADONLY : OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY;

	// Create a fileselect dialog
	CFileDialog FileDlg(mode, extension, destname, Flags, alias);

	// Setup title
	FileDlg.m_ofn.lpstrTitle = "Select file";

	// Call dialog
	if(FileDlg.DoModal() != IDOK)	return false;

	// Get the filename back and copy it
	CString FName = FileDlg.GetPathName();

	// The CFileDialog class doesn't handle complex extensions like ".abc.def" so let's fix that.............
	filename.Set((const char*)FName);
	RemoveExtension(filename);
	filename+=String(".");
	filename+=String(extension);

	// Check file attributes.
	struct _finddata_t c_file;
	long hFile = _findfirst(filename, &c_file );
	if(hFile!=-1)
	{
		_findclose(hFile);
		// File already exists
		if(c_file.attrib & _A_RDONLY)
		{
			IceMessage	MsgDlg;
			MsgDlg.m_Text = "File has read-only attribute!\nCan't overwrite.\nChoose another name.";
			MsgDlg.CenterWindow();
			MsgDlg.DoModal();
			return false;
		}
	}

	return true;
}
