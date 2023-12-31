///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains a quad class.
 *	\file		IceQuad.h
 *	\author		Pierre Terdiman
 *	\date		January, 17, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEQUAD_H
#define ICEQUAD_H

	class ICEMATHS_API Quad
	{
		public:
		//! Constructor
		inline_			Quad()																		{}
		//! Constructor
						Quad(const Point& p0, const Point& p1, const Point& p2, const Point& p3)	{ mVerts[0]=p0; mVerts[1]=p1; mVerts[2]=p2; mVerts[3]=p3; }
		//! Destructor
		inline_			~Quad()																		{}
		//! Vertices
				Point	mVerts[4];
	};

#endif // ICEQUAD_H
