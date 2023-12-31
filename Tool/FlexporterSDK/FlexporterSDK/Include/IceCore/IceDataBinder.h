///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains a data binder.
 *	\file		IceDataBinder.h
 *	\author		Pierre Terdiman
 *	\date		April, 20, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEDATABINDER_H__
#define __ICEDATABINDER_H__

	class ICECORE_API DataBinder
	{
		public:
		// Constructor/Destructor
								DataBinder();
								~DataBinder();
		// Data binding

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Binds data to an object.
		 *	\param		owner	[in] data owner
		 *	\param		data	[in] bound data
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				bool			BindData(Cell* owner, udword data);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Removes bound data from a given object.
		 *	\param		owner	[in] data owner
		 *	\param		data	[in] data to remove
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				bool			RemoveData(Cell* owner, udword data);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Removes all bound data from a given object.
		 *	\param		owner	[in] data owner
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				bool			RemoveData(Cell* owner);

		// Data retrieval

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets the number of bound data for a given object.
		 *	\param		owner	[in] data owner
		 *	\return		number of data bound to this owner
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				udword			GetNbData(Cell* owner);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets all data bound to a given object.
		 *	\param		owner	[in] data owner
		 *	\param		data	[out] bound data
		 *	\return		true if some data has been found
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				bool			GetData(Cell* owner, Container& data);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Removes a list of indexed entries from the data binder.
		 *	\param		nb_deleted_indices	[in] number of entries to remove
		 *	\param		deleted_indices		[in] indices of deleted entries
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				bool			PurgeList(udword nb_deleted_indices, const udword* deleted_indices);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets internal indices of data bound to a given object
		 *	\param		owner		[in] data owner
		 *	\param		nb_indices	[out] number of data bound to the object
		 *	\return		list of indices of bound data, or null. Indices in GetData().
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_	const udword*	GetDataIndices(Cell* owner, udword& nb_indices)
								{
									nb_indices = GetNbData(owner);
									if(!nb_indices)	return null;

									return &mRS.GetRanks()[mOffsets[(owner->GetKernelID()&0xffff) - mMin]];
								}

		// Data access
		inline_	udword			GetMaxNbEntries()	const	{ return mMaxNbEntries;	}
		inline_	udword			GetNbEntries()		const	{ return mCurNbEntries;	}

		inline_	Cell**			GetOwners()			const	{ return mOwners;		}
		inline_	udword*			GetData()			const	{ return mData;			}

								PREVENT_COPY(DataBinder)
		private:
				RadixSort		mRS;

				udword			mFlags;					//!< System flags

				udword			mMaxNbEntries;			//!< Number of dwords allocated in mOwners & mRefs
				udword			mCurNbEntries;			//!< Number of used entries

				Cell**			mOwners;				//!< List of owners
				udword*			mData;					//!< List of data

				udword*			mOffsets;
				udword			mMin;
				udword			mMax;
		// Internal methods
				bool			CreateOffsets();
	};

#endif // __ICEDATABINDER_H__
