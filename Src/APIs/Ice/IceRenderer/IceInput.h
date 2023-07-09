///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for input management.
 *	\file		IceInput.h
 *	\author		Pierre Terdiman
 *	\date		March, 14, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEINPUT_H
#define ICEINPUT_H

	enum InputFlag
	{
		INPUT_KEYBOARD	= (1<<0),
		INPUT_MOUSE		= (1<<1),
		INPUT_JOYSTICK	= (1<<2),
	};

	enum CooperativeLevelFlag
	{
		CL_EXCLUSIVE		= 0x00000001,	// The application requires exclusive access. If exclusive access is granted, no other instance of the
											// device can obtain exclusive access to the device while it is acquired. However, nonexclusive access to
											// the device is always permitted, even if another application has obtained exclusive access.
											// An application that acquires the mouse or keyboard device in exclusive mode should always unacquire the
											// devices when it receives WM_ENTERSIZEMOVE and WM_ENTERMENULOOP messages. Otherwise, the user cannot
											// manipulate the menu or move and resize the window.
		CL_NONEXCLUSIVE		= 0x00000002,	// The application requires nonexclusive access. Access to the device does not interfere with other
											// applications that are accessing the same device.
		CL_FOREGROUND		= 0x00000004,	// The application requires foreground access. If foreground access is granted, the device is automatically
											// unacquired when the associated window moves to the background.
		CL_BACKGROUND		= 0x00000008,	// The application requires background access. If background access is granted, the device can be acquired
											// at any time, even when the associated window is not the active window.
		CL_NOWINKEY			= 0x00000010,	// Disable the Windows key. Setting this flag ensures that the user cannot inadvertently break out of the application 
	};

	// Applications must specify either CL_FOREGROUND or CL_BACKGROUND; it is an error to specify both or neither. Similarly, applications must
	// specify either CL_EXCLUSIVE or CL_NONEXCLUSIVE.

	struct ICERENDERER_API KeyboardState
	{
		KeyboardState()
		{
			ZeroMemory(mKeybuffer, sizeof(mKeybuffer));
		}

		ubyte	mKeybuffer[256];
	};

	struct ICERENDERER_API MouseState
	{
	};

	struct ICERENDERER_API JoystickState
	{
		sdword	mX;				// x-axis position
		sdword	mY;				// y-axis position
		sdword	mZ;				// z-axis position
		sdword	mRx;			// x-axis rotation
		sdword	mRy;			// y-axis rotation
		sdword	mRz;			// z-axis rotation
		sdword	mSlider[2];		// extra axes positions
		sdword	mPOV[4];		// POV directions
		sbyte	mButtons[32];	// 32 buttons
	};

	struct ICERENDERER_API INPUTCREATE
	{
		INPUTCREATE()
		{
			mHWnd				= 0;
			mhInstance			= 0;
			mInputFlags			= 0;
			mCooperativeFlags	= CL_FOREGROUND | CL_NONEXCLUSIVE | CL_NOWINKEY;
		}

		HWND		mHWnd;
		HINSTANCE	mhInstance;
		udword		mInputFlags;
		udword		mCooperativeFlags;
	};

	class ICERENDERER_API Input
	{
		public:
		// Constructor / Destructor
												Input();
		virtual									~Input();

		virtual			bool					Init(const INPUTCREATE* create);
		virtual			bool					Release();

		virtual	const	KeyboardState*			GetKeyboardState()	= 0;
		virtual	const	MouseState*				GetMouseState()		= 0;
		virtual	const	JoystickState*			GetJoystickState()	= 0;

		protected:
		// Windows stuff
						HWND					mHWnd;
						HINSTANCE				mhInstance;
		// Input devices
						udword					mNbKeyboards;
						udword					mNbMouses;
						udword					mNbJoysticks;
						GUID*					mGUIDKeyboards;
						GUID*					mGUIDMouses;
						GUID*					mGUIDJoysticks;
		// Input states
						KeyboardState			mKState;	// Casse-tête ! Humour !
						MouseState				mMState;
						JoystickState			mJState;
	};

#endif // ICEINPUT_H
