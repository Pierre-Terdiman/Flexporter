///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains source code for an optimized QSort.
 *	\file		IceQSort.h
 *	\author		Brent Spillner
 *	\date		1995
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEQSORT_H__
#define __ICEQSORT_H__

	ICECORE_API void QSort(void* base, size_t num, size_t width, int (__cdecl* compare)(const void* elem1, const void* elem2));

	ICECORE_API void QSort(int n, int* x);
	ICECORE_API void QSort(int n, float* x);

	class ICECORE_API QuickSort
	{
		public:
		// Constructor/Destructor
							QuickSort();
							~QuickSort();

		// Sorting method
		const	udword*		Sort(sdword nb, const udword* list);

		private:
				udword*		mIndices;

		// Internal methods
				void		qsort(sdword first, sdword last, const sdword* list);
				void		isort(sdword n, const sdword* list);
	};

#endif // __ICEQSORT_H__
