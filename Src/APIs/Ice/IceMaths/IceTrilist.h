///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for a triangle container.
 *	\file		IceTrilist.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICETRILIST_H
#define ICETRILIST_H

	class ICEMATHS_API TriList : public Container
	{
		public:
		// Constructor / Destructor
								TriList()					{}
								~TriList()					{}

		inline_	udword			GetNbTriangles()	const	{ return GetNbEntries()/9;			}
		inline_	Triangle*		GetTriangles()		const	{ return (Triangle*)GetEntries();	}

				void			AddTri(const Triangle& tri)
								{
									Add(tri.mVerts[0].x).Add(tri.mVerts[0].y).Add(tri.mVerts[0].z);
									Add(tri.mVerts[1].x).Add(tri.mVerts[1].y).Add(tri.mVerts[1].z);
									Add(tri.mVerts[2].x).Add(tri.mVerts[2].y).Add(tri.mVerts[2].z);
								}

				void			AddTri(const Point& p0, const Point& p1, const Point& p2)
								{
									Add(p0.x).Add(p0.y).Add(p0.z);
									Add(p1.x).Add(p1.y).Add(p1.z);
									Add(p2.x).Add(p2.y).Add(p2.z);
								}
	};

	class ICEMATHS_API TriangleList : public Container
	{
		public:
		// Constructor / Destructor
									TriangleList()				{}
									~TriangleList()				{}

		inline_	udword				GetNbTriangles()	const	{ return GetNbEntries()/3;				}
		inline_	IndexedTriangle*	GetTriangles()		const	{ return (IndexedTriangle*)GetEntries();}

				void				AddTriangle(const IndexedTriangle& tri)
									{
										Add(tri.mRef[0]).Add(tri.mRef[1]).Add(tri.mRef[2]);
									}

				void				AddTriangle(udword vref0, udword vref1, udword vref2)
									{
										Add(vref0).Add(vref1).Add(vref2);
									}
	};

#endif //ICETRILIST_H
