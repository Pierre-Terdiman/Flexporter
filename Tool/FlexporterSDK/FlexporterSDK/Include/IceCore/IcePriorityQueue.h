///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains a simple priority queue.
 *	\file		IcePriorityQueue.h
 *	\author		Pierre Terdiman
 *	\date		February, 5, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEPRIORITYQUEUE_H__
#define __ICEPRIORITYQUEUE_H__

	enum PriorityMode
	{
		PM_SMALL_FIRST,		//!< Small priorities should be served first
		PM_BIG_FIRST,		//!< Big priorities should be served first
	};

	class ICECORE_API PriorityQueue : public Pairs
	{
		public:
		//! Constructor
								PriorityQueue()	: mPriorityMode(PM_BIG_FIRST)	{}
		//! Destructor
								~PriorityQueue()								{}

				void			Push(udword value, udword priority);
				bool			Pop(Pair& next_pair);

		inline_	const Pair*		NextPair()
								{
									if(!HasPairs()) return null;
									return (const Pair*)GetPairs();
								}

		inline_	PriorityMode	GetPriorityMode()						const	{ return mPriorityMode;	}
		inline_	void			SetPriorityMode(PriorityMode mode)				{ mPriorityMode = mode;	}

		protected:

				PriorityMode	mPriorityMode;
	};

#endif // __ICEPRIORITYQUEUE_H__
