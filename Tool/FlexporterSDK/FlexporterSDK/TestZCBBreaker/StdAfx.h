// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__42298A84_802D_11D5_8B0F_0050BAC83302__INCLUDED_)
#define AFX_STDAFX_H__42298A84_802D_11D5_8B0F_0050BAC83302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>												// General purpose Win32 functions
#include <windowsx.h>

// Standard includes
#ifdef _DEBUG
#include <crtdbg.h>													// C runtime debug functions
#endif
#include <stdio.h>													// Standard Input/Output functions
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>													// Assertion functions
#include <shlobj.h>
#include <mmsystem.h>
#include <stdarg.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include <direct.h>
#include <io.h>

// Begin the big hook

#ifndef ASSERT
	#define	ASSERT				assert
#endif

	// Cosmetic stuff
	#define	override(baseclass)	virtual

	// Our own inline keyword, so that:
	// - we can switch to __forceinline to check it's really better or not
	// - we can remove __forceinline if the compiler doesn't support it
	#define inline_				__forceinline
//	#define inline_				inline

	// ANSI compliance
	#ifdef  _DEBUG
		// Remove painful warning in debug
		inline_ bool __False__(){ return false; }
		#define for if(__False__()){}	else for
	#else
		#define for if(0){}	else for
	#endif

	#define FUNCTION
	#define ICECORE_API
	#define MESHMERIZER_API
	#define EC_OUTOFMEMORY	0
	#define SetIceError		false
	#define	PIXEL_OPAQUE	0xff

	struct Point
	{
		__forceinline			Point()														{}
		__forceinline			Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z)	{}
		__forceinline			Point(float f[3]) : x(f[0]), y(f[1]), z(f[2])				{}
		__forceinline			Point(const Point& p) : x(p.x), y(p.y), z(p.z)				{}
		__forceinline			~Point()													{}

		__forceinline	void	Zero()	{ x=y=z=0.0f; }

		float x,y,z;
	};

	struct Quat
	{
		__forceinline	Quat&		Identity()		{ p.x = p.y = p.z = 0.0f;	w = 1.0f;	return *this;	}

		Point	p;
		float	w;
	};

	class PR
	{
		public:
		__forceinline					PR()	{}
		__forceinline					~PR()	{}

						PR&				Reset()
										{
											mPos.Zero();
											mRot.Identity();
											return *this;
										}

						Point			mPos;
						Quat			mRot;
	};

	class PRS
	{
		public:
		__forceinline					PRS()	{}
		__forceinline					~PRS()	{}

						PRS&			Reset()
										{
											mPos.Zero();
											mRot.Identity();
											mScale = Point(1.0f, 1.0f, 1.0f);
											return *this;
										}

						Point			mPos;
						Quat			mRot;
						Point			mScale;
	};

	#include "IceTypes.h"
	#include "IceFPU.h"

	inline_ const char* _F(LPSTR format, ...)
	{
		static char buff[4096];

		va_list va;

		va_start(va, format);
		vsprintf(buff, format, va);
		va_end(va); 

		if(strlen(buff)>=4096)
		{
			MessageBox(null, "_F: buffer overflow!!", "Fatal error", MB_OK);
		}

		return buff;
	}

	namespace IceCore
	{
		#include "IceUtils.h"
		#include "IceString.h"
		#include "IceMemoryMacros.h"
		#include "IceFile.h"
		#include "IceContainer.h"
		#include "IceBuffer.h"
		#include "IceCustomArray.h"
		#include "IceZCBFormat.h"
	}

	using namespace IceCore;

	#include "IceMeshBuilderResults.h"

	namespace IceRenderManager
	{
		#include "IceZCBBreaker.h"
	}

	// to support decompression
//	#include <ZLib/ZLib.h>
//	#include <BZip2/Src/bzlib.h>


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__42298A84_802D_11D5_8B0F_0050BAC83302__INCLUDED_)
