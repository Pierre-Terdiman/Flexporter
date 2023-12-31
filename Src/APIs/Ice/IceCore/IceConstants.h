///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for constants.
 *	\file		IceConstants.h
 *	\author		Pierre Terdiman
 *	\date		September, 26, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICECONSTANTS_H
#define ICECONSTANTS_H

	struct ICECORE_API Constant
	{
		inline_			Constant()	{}
		inline_			~Constant()	{}

		const String*	mStr;
		udword			mUserdata;
	};

	class ICECORE_API Constants : private Container
	{
		public:
		//! Constructor
											Constants();
		//! Destructor
											~Constants();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets the number of stored constants. O(1)
		 *	\return		number of constants
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_			udword				GetNbConstants()		const	{ return GetNbEntries()>>1;											}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Gets the list of stored constants.
		 *	That list is lazy-sorted on-the-fly if needed, so this is O(1) most of the time, but can be O(log n).
		 *	\return		list of constants
		 *	\warning	do not modify a string (const access), as it would invalidate the sort and produce unpredictable results
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_			const Constant*		GetConstants()					{ if(!mSorted)	Sort();	return (const Constant*)GetEntries();		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Adds a constant to the list. The list is searched for duplicate names, so this has the same running time as Find().
		 *	\param		str			[in] constant's name
		 *	\param		user_data	[in] constant's value
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool				AddConstant(const char* str, udword user_data);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Removes a constant from the list.
		 *	\param		str			[in] constant's name
		 *	\param		user_data	[out] constant's value, or null if not needed
		 *	\return		true if the constant has been found and removed
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool				RemoveConstant(const char* str, udword* user_data=null);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Finds a constant given its name. Lazy-sorts the list on-the-fly if needed. So O(log n) most of the time, sometimes O(2*log n).
		 *	\param		str			[in] constant's name
		 *	\param		user_data	[out] constant's value, or null if not needed
		 *	\param		pos			[out] constant's position in the list, or null if not needed
		 *	\return		true if the constant has been found in the list
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool				Find(const char* str, udword* user_data=null, udword* pos=null);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Checks if a given constant already exists or not.
		 *	\param		str			[in] constant's name
		 *	\return		true if the constant already exists
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_			bool				HasConstant(const char* str)				{ return Find(str);	}

		private:
						BOOL				mSorted;	//!< TRUE in sorted state
		// Lazy-sorting
						void				Sort();
	};

	class ICECORE_API ConstantManager
	{
		public:
		// Constructor / Destructor
										ConstantManager();
										~ConstantManager();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Initializes the constant manager.
		 *	\param		nb		[in] hash size => yields the number of internal sorted lists
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool			Init(udword nb=10);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Releases everything.
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool			Free();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Adds a constant to the constant manager.
		 *	\param		str			[in] constant's name
		 *	\param		user_data	[in] constant's value
		 *	\return		true if success
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool			Add(const char* str, udword user_data);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Removes a constant from the constant manager.
		 *	\param		str			[in] constant's name
		 *	\return		true if the constant has been found and removed
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool			RemoveConstant(const char* str);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Finds a constant given its name. Between O(1) and O(log n).
		 *	\param		str			[in] constant's name
		 *	\param		user_data	[out] constant's value, or null if not needed
		 *	\return		true if the constant has been found
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bool			Find(const char* str, udword* user_data=null);

		private:
						udword			mNbLists;	//!< Number of internal sorted lists
						udword			mMask;		//!< Hash mask
						Constants**		mLists;		//!< List of sorted lists
	};

#endif // ICECONSTANTS_H
