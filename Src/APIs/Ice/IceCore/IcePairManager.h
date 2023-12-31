///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains a new optimized pair manager.
 *	\file		IcePairManager.h
 *	\author		Pierre Terdiman
 *	\date		December, 16, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEPAIRMANAGER_H
#define ICEPAIRMANAGER_H

	ICE_COMPILE_TIME_ASSERT(sizeof(Pair)==8);

	class ICECORE_API PairManager
	{
		public:
		// Constructor/Destructor
								PairManager();
								~PairManager();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Adds a pair.
		 *	\param		id0		[in] first pair element
		 *	\param		id1		[in] second pair element
		 *	\return		corresponding pair
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				const Pair*		AddPair(udword id0, udword id1);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Removes a pair.
		 *	\param		id0		[in] first pair element
		 *	\param		id1		[in] second pair element
		 *	\return		true if success (false if the pair didn't exist)
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				bool			RemovePair(udword id0, udword id1);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Finds a pair.
		 *	\param		id0		[in] first pair element
		 *	\param		id1		[in] second pair element
		 *	\return		corresponding pair, or null if the pair doesn't exist
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				const Pair*		FindPair(udword id0, udword id1)	const;

		inline_	udword			GetPairIndex(const Pair* pair)		const { return udword((regsize(pair) - regsize(mActivePairs))>>3);	}

				void			DumpPairs(Pairs& pairs)				const;

		private:
				udword			mHashSize;
				udword			mMask;
				udword*			mHashTable;
				udword*			mNext;
				udword			mNbActivePairs;
				Pair*			mActivePairs;
				udword			mFirstFree;
		inline_	Pair*			FindPair(udword id0, udword id1, udword hash_value) const;
	};

	struct UserPair
	{
		uword	mID0;
		uword	mID1;
		void*	mUserData;
	};

	ICE_COMPILE_TIME_ASSERT(sizeof(UserPair)==8);

	class ICECORE_API UserPairManager
	{
		public:
		// Constructor/Destructor
								UserPairManager();
								~UserPairManager();

				const UserPair*	AddPair(uword id0, uword id1, void* user_data);
				bool			RemovePair(uword id0, uword id1);
				const UserPair*	FindPair(uword id0, uword id1)		const;

		inline_	udword			GetPairIndex(const UserPair* pair)	const { return udword((regsize(pair) - regsize(mActivePairs))>>3);	}

		private:
				udword			mHashSize;
				udword			mMask;
				udword*			mHashTable;
				udword*			mNext;
				udword			mNbActivePairs;
				UserPair*		mActivePairs;
				udword			mFirstFree;
		inline_	UserPair*		FindPair(uword id0, uword id1, udword hash_value) const;
	};

#endif // ICEPAIRMANAGER_H
