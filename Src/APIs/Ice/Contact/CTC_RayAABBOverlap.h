///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for ray-AABB intersection
 *	\file		CTC_RayAABBOverlap.h
 *	\author		Pierre Terdiman
 *	\date		January, 13, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CTCRAYAABBOVERLAP_H
#define CTCRAYAABBOVERLAP_H

	CONTACT_API bool	SegmentOBB(const Segment& segment, const Point& center, const Point& extents, const Matrix3x3& rot);
	CONTACT_API bool	RayOBB(const Ray& ray, const Point& center, const Point& extents, const Matrix3x3& rot);

	CONTACT_API bool	SegmentAABB(const Segment& segment, const Point& min, const Point& max);
	CONTACT_API bool	RayAABB(const Ray& ray, const Point& min, const Point& max);

	inline_ bool SegmentOBB(const Segment& segment, const OBB& obb)
	{
		return Ctc::SegmentOBB(segment, obb.mCenter, obb.mExtents, obb.mRot);
	}
	inline_ bool RayOBB(const Ray& ray, const OBB& obb)
	{
		return Ctc::RayOBB(ray, obb.mCenter, obb.mExtents, obb.mRot);
	}
	inline_ bool SegmentAABB(const Segment& segment, const AABB& aabb)
	{
#ifdef USE_MINMAX
		const ShadowAABB* Box = (const ShadowAABB*)&aabb;
		return Ctc::SegmentAABB(segment, Box->mMin, Box->mMax);
#else
		Point Min, Max;
		aabb.GetMin(Min);
		aabb.GetMax(Max);
		return Ctc::SegmentAABB(segment, Min, Max);
#endif
	}
	inline_ bool RayAABB(const Ray& ray, const AABB& aabb)
	{
#ifdef USE_MINMAX
		const ShadowAABB* Box = (const ShadowAABB*)&aabb;
		return Ctc::RayAABB(ray, Box->mMin, Box->mMax);
#else
		Point Min, Max;
		aabb.GetMin(Min);
		aabb.GetMax(Max);
		return Ctc::RayAABB(ray, Min, Max);
#endif
	}

#endif // CTCRAYAABBOVERLAP_H

