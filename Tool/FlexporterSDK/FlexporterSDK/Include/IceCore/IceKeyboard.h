///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains key codes.
 *	\file		IceKeyboard.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEKEYBOARD_H__
#define __ICEKEYBOARD_H__

	class ICECORE_API KeyboardMapping
	{
		public:
		// Constructor/Destructor
								KeyboardMapping();
								~KeyboardMapping();

				bool			CreateAccelerator(udword keycode, const char* command);
				bool			CreateAccelerator(const char* key, const char* command);
				void			DeleteAccelerators();
				const String*	GetCommand(udword keycode);
		protected:
				SymbolicList	mList;
				String			mInternal;
	};

	ICECORE_API udword			GetKeyCode(const char* key);
	// For Windows
	ICECORE_API bool			IsKeyPressed(udword key);
	// For a DOS console
	ICECORE_API bool			IsConsoleKeyPressed();
	ICECORE_API int				GetConsoleKeyPressed();

	// Accelerators
	ICECORE_API bool			SetMapping(const KeyboardMapping* mapping);
	ICECORE_API bool			CreateAccelerator(udword keycode, const char* command);
	ICECORE_API bool			CreateAccelerator(const char* key, const char* command);
	ICECORE_API void			DeleteAccelerators();
	ICECORE_API const String*	GetCommand(udword keycode);

#endif // __ICEKEYBOARD_H__
