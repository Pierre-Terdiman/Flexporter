///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains an allocator base class.
 *	\file		IceAllocator.h
 *	\author		Pierre Terdiman
 *	\date		December, 19, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEALLOCATOR_H
#define ICEALLOCATOR_H

#ifdef SUPPORT_ALLOCATOR
	class Allocator
	{
		public:
		virtual void*	malloc(udword size)											= 0;
		virtual void*	mallocDebug(udword size, const char* filename, udword line)	= 0;
		virtual void*	realloc(void* memory, udword size)							= 0;
		virtual void	free(void* memory)											= 0;
	};

	FUNCTION ICECORE_API Allocator* GetAllocator();

	class ICECORE_API Allocateable
	{
		public:
		inline_	void*	operator new		(size_t size)									{ return GetAllocator()->malloc(size);						}
		inline_	void*	operator new		(size_t size, const char * filename, int line)	{ return GetAllocator()->mallocDebug(size, filename, line);	}
		inline_	void*	operator new[]		(size_t size)									{ return GetAllocator()->malloc(size);						}
		inline_	void*	operator new[]		(size_t size, const char * filename, int line)	{ return GetAllocator()->mallocDebug(size, filename, line);	}
		inline_	void	operator delete		(void* p)										{ GetAllocator()->free(p);	}
		inline_	void	operator delete		(void* p, const char*, int)						{ GetAllocator()->free(p);	}
		inline_	void	operator delete[]	(void* p)										{ GetAllocator()->free(p);	}
		inline_	void	operator delete[]	(void* p, const char*, int)						{ GetAllocator()->free(p);	}
	};
#endif

#endif // ICEALLOCATOR_H
