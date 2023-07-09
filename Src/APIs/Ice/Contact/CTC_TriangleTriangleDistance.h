///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for triangle-triangle distance
 *	\file		CTC_TriangleTriangleDistance.h
 *	\author		Pierre Terdiman
 *	\date		January, 13, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CTCTRIANGLETRIANGLEDISTANCE_H
#define CTCTRIANGLETRIANGLEDISTANCE_H

	CONTACT_API float TriangleTriangleDist(Point& cp, Point& cq, const Point p[3], const Point q[3]);

	inline_ float TriangleTriangleDist(Point& cp, Point& cq, const Triangle& ptri, const Triangle& qtri)
	{
		return Ctc::TriangleTriangleDist(cp, cq, ptri.mVerts, qtri.mVerts);
	}

#endif // CTCTRIANGLETRIANGLEDISTANCE_H
