///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains an allocator base class.
 *	\file		OPC_Allocator.h
 *	\author		Pierre Terdiman
 *	\date		December, 19, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef OPC_ALLOCATOR_H
#define OPC_ALLOCATOR_H

	class OPCODE_API OpcodeAllocator
	{
		public:
		virtual void*	malloc(size_t size)											= 0;
		virtual void*	mallocDebug(size_t size, const char* filename, udword line)	= 0;
		virtual void*	realloc(void* memory, size_t size)							= 0;
		virtual void	free(void* memory)											= 0;
	};

	FUNCTION OPCODE_API OpcodeAllocator* GetOpcodeAllocator();

	class OPCODE_API OpcodeAllocated
	{
		public:
		inline_	void*	operator new		(size_t size)									{ return GetOpcodeAllocator()->malloc(size);						}
		inline_	void*	operator new		(size_t size, const char * filename, int line)	{ return GetOpcodeAllocator()->mallocDebug(size, filename, line);	}
		inline_	void*	operator new[]		(size_t size)									{ return GetOpcodeAllocator()->malloc(size);						}
		inline_	void*	operator new[]		(size_t size, const char * filename, int line)	{ return GetOpcodeAllocator()->mallocDebug(size, filename, line);	}
		inline_	void	operator delete		(void* p)										{ GetOpcodeAllocator()->free(p);	}
		inline_	void	operator delete		(void* p, const char*, int)						{ GetOpcodeAllocator()->free(p);	}
		inline_	void	operator delete[]	(void* p)										{ GetOpcodeAllocator()->free(p);	}
		inline_	void	operator delete[]	(void* p, const char*, int)						{ GetOpcodeAllocator()->free(p);	}
	};

#endif // OPC_ALLOCATOR_H
