///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDAFX_H__F82AAC41_1412_11D4_8B0F_0050BAC83302__INCLUDED_)
#define AFX_STDAFX_H__F82AAC41_1412_11D4_8B0F_0050BAC83302__INCLUDED_

#include "FLX_MFC.h"

#ifdef _DEBUG
#error Needs release build....
#endif

#include <Stdio.h>
#include <Stdlib.h>
#include <String.h>
#include <Math.h>
#include <Float.h>
#include <Malloc.h>
#include <Conio.h>
#include <io.h>
#include <new.h>

// MAX includes
#include "FLX_CompilationSettings.h"

// ICE includes
#include "FLX_IceIncludes.h"

#define FLEXPORTER_API	__declspec(dllexport)
#include <Flexporter\Flexporter.h>
using namespace Flexporter;

// Flexporter includes
	#define ICEEXPORTER_API __declspec(dllexport)

	#ifdef _DEBUG
		#define new DEBUG_NEW
	#undef THIS_FILE
		static char THIS_FILE[] = __FILE__;
	#endif

	namespace IceExporter
	{
		#include "FLX_Fileselect.h"
		#include "FLX_Panel.h"
		#include "FLX_MAXNodes.h"
		#include "FLX_Exporter.h"

		//
		#include "FLX_VertexColors.h"
		#include "FLX_VertexDataModifier.h"
		#include "FLX_VertexPrelighter.h"
		#include "FLX_ZCBImporter.h"

		// Dialog-related
		#include "Resource.h"
		#include "FLX_HyperLink.h"
		#include "FLX_About.h"
		#include "FLX_ExportDlg.h"
		#include "FLX_Message.h"
		#include "IceOptions.h"
		#include "FLX_MotionDlg.h"
		#include "FLX_EditBox.h"
		#include "VertexLighterDlg.h"

		// Interfaces
		#include "FLX_Controller.h"
		#include "FLX_Ask2MAX.h"
		#include "FLX_MAXMeshPointer.h"
		#include "FLX_MAXProgressBar.h"
		#include "FLX_UserProps.h"
		#include "FLX_MatrixConvert.h"
		#include "FLX_TextureCopy.h"

		#include "FLX_CharacterStudio.h"

		#include "FLX_GlobalInfo.h"

		// Converter
		#include "FLX_Lightmapper.h"
		#include "FLX_MAXConverter.h"

		// Vertex lighter
		#include "FLX_VertexLighterBase.h"
		#include "FLX_VertexLighterLight.h"
		#include "FLX_VertexLighterMaterial.h"
		#include "FLX_VertexLighterMesh.h"
		#include "FLX_VertexLighter.h"

		// Custom
		#include "FLX_ExtendedMaterial.h"
	}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F82AAC41_1412_11D4_8B0F_0050BAC83302__INCLUDED_)
