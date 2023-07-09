///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains a doubly linked list.
 *	\file		IceLinkedList.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICELINKEDLIST_H__
#define __ICELINKEDLIST_H__

	class ICECORE_API ListElem
	{
		public:
		// Constructor / Destructor
								ListElem() : mNext(null), mPrev(null)	{}
		virtual					~ListElem()								{}

				ListElem*		mNext;			//!< Next list element
				ListElem*		mPrev;			//!< Previous list element
	};

	class ICECORE_API LinkedList
	{
		public:
		// Constructor / Destructor
		inline_					LinkedList() : mHead(null), mTail(null), mNb(0)	{}
		inline_					~LinkedList()									{}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Adds an element to the list
		 *	\param		list_element	[in] new list element
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				void			AddElem(ListElem* list_element);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Adds an element after a given one.
		 *	\param		list_element	[in] new list element
		 *	\param		after_me		[in] add after that one
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				void			AddAfter(ListElem* list_element, ListElem* after_me);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Adds an element before a given one.
		 *	\param		list_element	[in] new list element
		 *	\param		before_me		[in] add before that one
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				void			AddBefore(ListElem* list_element, ListElem* before_me);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Removes an element from the list (not deleted).
		 *	\param		list_element	[in] list element
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				void			RemElem(ListElem* list_element);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Removes all list elements.
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				void			RemAll();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Deletes all list elements.
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				void			DeleteAll();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Returns the length of the list.
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_	udword			Length()						const	{ return mNb; }

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets first list element.
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_	ListElem*		GetFirst()						const	{ return mHead; }

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets last list element.
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_	ListElem*		GetLast()						const	{ return mTail; }

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets next list element.
		 *	\param		list_element	[in] list element
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_	ListElem*		GetNext(ListElem* list_element)	const	{ return list_element ? list_element->mNext : mHead;	}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets previous list element.
		 *	\param		list_element	[in] list element
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_	ListElem*		GetPrev(ListElem* list_element)	const	{ return list_element ? list_element->mPrev : mTail;	}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets a list element.
		 *	\param		i		[in] element index
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				ListElem*		GetElement(udword i)			const;
		private:
				ListElem*		mHead;		//!< Head of list
				ListElem*		mTail;		//!< Tail of list
				udword			mNb;		//!< Number of elements in the list
	}; 

#endif // __ICELINKEDLIST_H__
