// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__75269F23_16A4_11D4_8B0F_0050BAC83302__INCLUDED_)
#define AFX_STDAFX_H__75269F23_16A4_11D4_8B0F_0050BAC83302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <Math.h>

#define FLEXPORTER_API
#include <Flexporter\Sdk\FlexporterSDK_IceCore.h>
#include <Flexporter\Sdk\FlexporterSDK.h>
#include "IceZCBFormat.h"
#include "IceZCBExporter.h"

// ZLib include
#include <ZLib/ZLib.h>

// BZip2 include
#include <BZip2/Src/bzlib.h>

#include "Resource.h"

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__75269F23_16A4_11D4_8B0F_0050BAC83302__INCLUDED_)
