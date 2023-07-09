///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	FLEXPORTER - a flexible exporter
 *	Copyright (C) 2000-2004 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Flexporter.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FLX_MFC_H
#define FLX_MFC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define DISCARD_MFC_NEW

//#define new
#define _AFX_NO_DEBUG_CRT

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define DOWNLOAD_DATA	UpdateData(TRUE);
#define UPLOAD_DATA		UpdateData(FALSE);

	#define	_ASSERTE

//	#include <afxwin.h>         // MFC core and standard components
//	#include <afxext.h>         // MFC extensions
//	#include <afxdisp.h>        // MFC Automation classes
//	#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
//	#ifndef _AFX_NO_AFXCMN_SUPPORT
//	#include <afxcmn.h>			// MFC support for Windows Common Controls
//	#endif // _AFX_NO_AFXCMN_SUPPORT

//	#include <afxtempl.h>


#endif // FLX_MFC_H
