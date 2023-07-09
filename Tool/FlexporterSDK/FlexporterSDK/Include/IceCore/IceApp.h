///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains an ICE application skeleton.
 *	\file		IceApp.h
 *	\author		Pierre Terdiman
 *	\date		June, 28, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEAPP_H__
#define __ICEAPP_H__

#define SUPPORT_REPLAY

	struct ICECORE_API ICE_EVENT
	{
		sdword	x;
		sdword	y;
		sdword	Type;
		sdword	Key;
		sdword	State;
	};

	//! Application's base creation structure
	struct ICECORE_API ICEAPPCREATE
	{
						ICEAPPCREATE();

		// WinMain parameters, to be passed to the base app
		HINSTANCE		hInstance;			//!< Copied from WinMain
		HINSTANCE		hPrevInstance;		//!< Copied from WinMain
		LPSTR			lpCmdLine;			//!< Copied from WinMain
		int				nCmdShow;			//!< Copied from WinMain
		// Application settings
		const char*		mAppName;			//!< Application's name
		const char*		mMenuName;			//!< Possible menu name
		const char*		mScriptName;		//!< Possible script name
		sdword			mPosX;				//!< X position on screen
		sdword			mPosY;				//!< Y position on screen
		sdword			mWidth;				//!< Width or -1 for screen size
		sdword			mHeight;			//!< Height or -1 for screen size
		BOOL			mForceSingleRun;	//!< TRUE: allows only one instance of the app at the same time
		BOOL			mFullscreen;		//!< TRUE: run in fullscreen mode, FALSE: run in windowed mode
	};

#ifdef SUPPORT_REPLAY
	enum RecordMode
	{
		REC_DISABLED,
		REC_CAPTURING,
		REC_CAPTURED,
		REC_REPLAYING,

		REC_FORCE_DWORD	= 0x7fffffff
	};
#endif

	class ICECORE_API IceApp
	{
		public:
		// Constructor/Destructor
										IceApp();
		virtual							~IceApp();

		virtual				bool		Run(const ICEAPPCREATE* create);
		virtual				udword		EventProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		protected:

		virtual				bool		Init(const ICEAPPCREATE* create);
		virtual				bool		Close();

		virtual				bool		PreInit();
		virtual				bool		PostInit();
		virtual				bool		ProcessCommandLineParam(const String& parameter);
		virtual				bool		EventLoop(const ICE_EVENT& event)	{ return true;			}
		virtual				bool		PreClose();
		virtual				bool		PostClose();

		virtual				bool		Frame(float delta_t)				{ return true;			}

		// Data access
		inline_				sdword		GetWidth()					const	{ return mWidth;		}
		inline_				sdword		GetHeight()					const	{ return mHeight;		}
		inline_				BOOL		IsFullscreen()				const	{ return mFullscreen;	}

							void		StartPause();
							void		EndPause();
#ifdef SUPPORT_REPLAY
		virtual				bool		StartRecording();
		virtual				bool		EndRecording();
		virtual				bool		PlayRecording();

		virtual				bool		RecordCallback(bool msg);
		virtual				bool		ReplayCallback();

		virtual				bool		LoadRecording(const char* filename);
		virtual				bool		SaveRecording(const char* filename);

		inline_				bool		IsRecording()				const	{ return (mRecordMode==REC_CAPTURING && mRecord);	}

		protected:
		// Replay
							RecordMode	mRecordMode;
						CustomArray*	mRecord;
#endif
		protected:
							HWND		mhWnd;
							MSG			mMsg;
							HINSTANCE	mHandle;
							int			mCmdShow;
							LPSTR		mCmdLine;
							String		mAppName;
		private:
							HANDLE		mMutex;
		// Saved initialization settings
							sdword		mWidth;
							sdword		mHeight;
							BOOL		mFullscreen;
		// Time-related data
							udword		mCurrentTime;
							udword		mPrevTime;
							udword		mElapsedTime;		//!< Elapsed time in milli-seconds
							BOOL		mPause;
		// Internal methods
							bool		CheckSingleRun(const char* app_name);
	};

#endif // __ICEAPP_H__
