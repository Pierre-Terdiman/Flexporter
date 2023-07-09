///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for standard callbacks.
 *	\file		IceCallbacks.h
 *	\author		Pierre Terdiman
 *	\date		July, 8, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICECALLBACKS_H__
#define __ICECALLBACKS_H__

	//! Base structure for callback parameters
	struct ICECORE_API IceCallbackParams
	{
	};

	//! Callback codes
	enum IceCoreCallbackCode
	{
		ICCB_REQUEST				= 0,	//!<
		ICCB_ERROR					= 1,	//!< 
		ICCB_LOG					= 2,	//!< 
		ICCB_MESSAGE				= 3,	//!< 
//		ICCB_NEWPLUGIN				= 4,	//!< 
//		ICCB_DISCARDPLUGIN			= 5,	//!< 
		ICCB_OPEN_INTERFACE			= 4,	//!< 
		ICCB_CLOSE_INTERFACE		= 5,	//!< 
		ICCB_REGISTRY_ACCESS		= 6,	//!< 
		ICCB_DUPLICATE_NAME_FOUND	= 7,	//!< 
		ICCB_EVENT_PROC				= 8,	//!<

		ICCB_SIZE					= 9,

		ICCB_FORCE_DWORD			= 0x7fffffff
	};

	//! Callback definition
	typedef bool (*IceCallback)	(IceCoreCallbackCode callback_code, IceCallbackParams* params, udword user_data);

	#include "IceCallbacksDef.h"
	#include "IceCallbacksTemplateDef.h"

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Returns the callback manager.
	 *	\relates	IceCoreCallbacksManager
	 *	\fn			GetCallbacksManager()
	 *	\return		the global callback manager for this dll
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FUNCTION ICECORE_API IceCoreCallbacksManager* GetCallbacksManager();

	//

	//! Callback parameters for ICCB_REQUEST
	struct ICECORE_API RequestParams : public IceCallbackParams
	{
		const char*	mRequestCode;
	};

	//! Callback parameters for ICCB_ERROR
	struct ICECORE_API ErrorParams : public IceCallbackParams
	{
		const char*	mError;
		const char*	mErrrorCode;
		bool		mUpdateLog;
	};

	//! Callback parameters for ICCB_MESSAGE
	struct ICECORE_API MessageParams : public IceCallbackParams
	{
		const IceMsg*	mMsg;
		float			mDelay;
	};

	//! Callback parameters for ICCB_OPEN_INTERFACE and ICCB_CLOSE_INTERFACE
	struct ICECORE_API InterfaceParams : public IceCallbackParams
	{
		IceInterface*	mInterface;
	};

	//! Callback parameters for ICCB_DUPLICATE_NAME_FOUND
	struct ICECORE_API DuplicateParams : public IceCallbackParams
	{
		Cell*	mCurrent;		//!< Object for which conflict has been detected
		Cell*	mDuplicate;		//!< Already existing object with same name
	};

	//! Callback parameters for ICCB_EVENT_PROC
	struct ICECORE_API EventProcParams : public IceCallbackParams
	{
		HWND	mHWnd;
		UINT	mMsg;
		WPARAM	mWParam;
		LPARAM	mLParam;
	};

#endif // __ICECALLBACKS_H__
