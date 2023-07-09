///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for threads.
 *	\file		IceThreads.h
 *	\author		Pierre Terdiman
 *	\date		March, 10, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICETHREAD_H__
#define __ICETHREAD_H__

	struct IceMutex
	{
		HANDLE id;
	};

	FUNCTION ICECORE_API	IceMutex*	CreateMutex();
	FUNCTION ICECORE_API	void		DestroyMutex(IceMutex* mutex);
	FUNCTION ICECORE_API	int			LockMutex(IceMutex* mutex);
	FUNCTION ICECORE_API	int			UnlockMutex(IceMutex* mutex);

	//////////////////////

	struct IceSemaphore
	{
		HANDLE id;
		udword volatile count;
	};

	typedef struct IceSemaphore IceSem;

	FUNCTION ICECORE_API	IceSem*		CreateSemaphore(udword initial_value);
	FUNCTION ICECORE_API	void		DestroySemaphore(IceSem* sem);
	FUNCTION ICECORE_API	int			SemWaitTimeout(IceSem* sem, udword timeout);
	FUNCTION ICECORE_API	int			SemTryWait(IceSem* sem);
	FUNCTION ICECORE_API	int			SemWait(IceSem* sem);
	FUNCTION ICECORE_API	udword		SemValue(IceSem* sem);
	FUNCTION ICECORE_API	int			SemPost(IceSem* sem);

	//////////////////////

	typedef HANDLE SYS_ThreadHandle;

	struct ThreadError
	{
	};

	struct IceThread
	{
		udword				threadid;
		SYS_ThreadHandle	handle;
		int					status;
		ThreadError			errbuf;
		void*				data;
	};

	FUNCTION ICECORE_API	IceThread*	CreateThread(int (*fn)(void *), void *data);
	FUNCTION ICECORE_API	void		WaitThread(IceThread* thread, int* status);
	FUNCTION ICECORE_API	udword		GetThreadID(IceThread* thread);
	FUNCTION ICECORE_API	void		KillThread(IceThread* thread);

#endif // __ICETHREAD_H__
