///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains DLL related code.
 *	\file		IceDll.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEDLL_H__
#define __ICEDLL_H__

extern void ModuleAttach(HINSTANCE hinstance);
extern void ModuleDetach();

#if defined(_USRDLL) && defined(ICE_MAIN)

	BOOL WINAPI DllMain(
	  HINSTANCE hinstDLL,  // handle to the DLL module
	  DWORD fdwReason,     // reason for calling function
	  LPVOID /*lpvReserved*/   // reserved
	)
	{
		switch(fdwReason)
		{
			case DLL_PROCESS_ATTACH:
				ModuleAttach(hinstDLL);
				break;

			case DLL_PROCESS_DETACH:
				ModuleDetach();
				break;
		}
		return TRUE;
	}

#endif

#ifdef _DEBUG

ICECORE_API inline_ void IceTrace(const char* lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	sdword nBuf;
	ubyte szBuffer[512];

	nBuf = _vsnprintf((LPSTR)szBuffer, lstrlen((LPCSTR)szBuffer), (LPCSTR)lpszFormat, args);

	_RPT0(_CRT_WARN, szBuffer);

	va_end(args);
}

#endif // _DEBUG

#endif // __ICEDLL_H__
