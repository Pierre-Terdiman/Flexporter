///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for ray-triangle intersection
 *	\file		CTC_RayTriangleOverlap.h
 *	\author		Pierre Terdiman - original code by Tomas M�ller
 *	\date		January, 13, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CTCRAYTRIANGLEOVERLAP_H
#define CTCRAYTRIANGLEOVERLAP_H

	// Ray-triangle intersection
	CONTACT_API bool RayTri(const Point& orig, const Point& dir, const Point& vert0, const Point& vert1, const Point& vert2, float& t, float& u, float& v, bool cull);
	CONTACT_API bool RayTri3(const Point& orig, const Point& dir, const Point& vert0, const Point& vert1, const Point& vert2, float& t, float& u, float& v);

	inline_ bool RayTri(const Ray& ray, const Point& vert0, const Point& vert1, const Point& vert2, float& t, float& u, float& v, bool cull)
	{
		return RayTri(ray.mOrig, ray.mDir, vert0, vert1, vert2, t, u, v, cull);
	}

	inline_ bool RayTri3(const Ray& ray, const Point& vert0, const Point& vert1, const Point& vert2, float& t, float& u, float& v)
	{
		return RayTri3(ray.mOrig, ray.mDir, vert0, vert1, vert2, t, u, v);
	}

#endif // CTCRAYTRIANGLEOVERLAP_H

