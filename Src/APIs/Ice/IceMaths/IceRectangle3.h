///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for rectangles.
 *	\file		IceRectangle3.h
 *	\author		Pierre Terdiman
 *	\date		April, 28, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICERECTANGLE3_H
#define ICERECTANGLE3_H

	class ICEMATHS_API Rectangle3
	{
		public:
		//! Constructor
		inline_					Rectangle3()																											{}
		//! Constructor
		inline_					Rectangle3(const Point& orig, const Point& edge0, const Point& edge1) : mOrig(orig), mEdge0(edge0), mEdge1(edge1)		{}
		//! Copy constructor
		inline_					Rectangle3(const Rectangle3& rectangle) : mOrig(rectangle.mOrig), mEdge0(rectangle.mEdge0), mEdge1(rectangle.mEdge1)	{}
		//! Destructor
		inline_					~Rectangle3()																											{}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 *	Computes a point on the rectangle
		 *	\param		pt	[out] point on rectangle = mOrig + s*mEdge0 + t*mEdge1
		 *	\param		s,t	[in] point's parameters
		 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline_			void	ComputePoint(Point& pt, float s, float t)	const	{	pt = mOrig + s*mEdge0 + t*mEdge1;	}

						Point	mOrig;		//!< Origin
						Point	mEdge0;		//!< First edge
						Point	mEdge1;		//!< Second edge
	};

#endif // ICERECTANGLE3_H
