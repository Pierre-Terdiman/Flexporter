///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains the notification system.
 *	\file		IceNotifSystem.h
 *	\author		Pierre Terdiman
 *	\date		April, 20, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICENOTIFSYSTEM_H__
#define __ICENOTIFSYSTEM_H__

	// Forward declarations
	class NotifSystem;

	// Create & return the global notification system
	FUNCTION ICECORE_API NotifSystem*	GetNotifSystem();
	// Release the global notification system
	FUNCTION ICECORE_API void			ReleaseNotifSystem();

	class ICECORE_API NotifSystem
	{
		private:
		// Constructor/Destructor
										NotifSystem();
										~NotifSystem();
		public:
		// Register notification
						bool			AddNotif	(Cell* owner, udword notif_index, NotifCallback callback, udword user_data);
		// Unregister notification
						bool			RemoveNotif	(Cell* owner, udword notif_index, NotifCallback callback=null);
						bool			RemoveNotif	(Cell* owner);
		// Execute notification
						bool			ExecNotif	(udword notif_index, udword param, bool& result);
						bool			ExecNotif	(Cell* owner, udword notif_index, udword param, bool& result);
		// Per object array
		inline_			udword			GetNotifArraySize()	const		{ return mNotifArraySize;	}
		inline_			void			SetNotifArraySize(udword size)	{ mNotifArraySize = size;	}
		// Memory stats
						udword			GetUsedRam();

										PREVENT_COPY(NotifSystem)
		private:
						udword			mFlags;					//!< System flags

						udword			mMaxNbNotifs;			//!< Number of dwords allocated in mOwners & mRefs
						udword			mCurNbNotifs;			//!< Number of used entries

						Cell**			mOwners;				//!< List of owners
						udword*			mNotifIndex;			//!< List of notification indices
						NotifCallback*	mCallbacks;				//!< List of callbacks
						udword*			mUserData;				//!< List of user-defined data

						udword*			mSortedNotifIndices;	//!< List of sorted notification indices
						udword*			mNotifsOffsets;			//!< Radix-like offsets for notification indices
						udword			mMinNotif;				//!< Smallest notification index
						udword			mMaxNotif;				//!< Largest notification index

						udword			mNotifArraySize;		//!< Size of the notif array for each object
		// Internal methods
						udword			GetNbOwners(udword notif_index);
						bool			CreateOffsets();
						bool			PurgeList(const Container& killed);

		friend ICECORE_API NotifSystem*	GetNotifSystem();
		friend ICECORE_API void			ReleaseNotifSystem();
	};

#endif // __ICENOTIFSYSTEM_H__
