///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains a string table.
 *	\file		IceStringTable.h
 *	\author		Pierre Terdiman
 *	\date		September, 26, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICESTRINGTABLE_H__
#define __ICESTRINGTABLE_H__

	class ICECORE_API SortedStrings : private Container
	{
		public:
		// Constructor / Destructor
											SortedStrings()	: mSorted(FALSE){}
											~SortedStrings()				{}

		inline_			udword				GetNbStrings()			const	{ return GetNbEntries();											}
		inline_			const String**		GetStrings()					{ if(!mSorted)	Sort();	return (const String**)GetEntries();		}
		inline_			const String*		GetString(udword i)				{ if(!mSorted)	Sort();	return (const String*)GetEntry(i);			}

						SortedStrings&		AddString(const String& str)	{ Add(udword(&str));	mSorted = FALSE; return *this;				}
						udword				Find(const char* str);
		private:
						BOOL				mSorted;
		// Lazy-sorting
						void				Sort();
	};

	class ICECORE_API StringDescriptor : public ListElem
	{
		public:
			inline_			StringDescriptor() : mStr(null)	{}
			inline_			~StringDescriptor()				{}

		const String*		mStr;
	};

	class ICECORE_API StringTable
	{
		public:
						StringTable();
						~StringTable();

		bool			Init(udword nb=10);
		bool			Free();

		bool					Add(const String& str);
		const StringDescriptor*	Find(const String& str);

		private:
						udword			mNbLists;
						udword			mMask;
						LinkedList**	mLists;
	};

#endif // __ICESTRINGTABLE_H__
