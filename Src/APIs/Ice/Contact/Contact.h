///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Main file for Contact library.
 *	\file		Contact.h
 *	\author		Pierre Terdiman
 *	\date		January, 14, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CONTACT_H
#define CONTACT_H

	#include "ContactSettings.h"

	namespace Ctc
	{
		// Distances

		#ifndef	CTCRAYAABBDISTANCE_H
			#include "CTC_RayAABBDistance.h"
		#endif
		#ifndef	CTCPOINTAABBDISTANCE_H
			#include "CTC_PointAABBDistance.h"
		#endif
		#ifndef	CTCPOINTOBBDISTANCE_H
			#include "CTC_PointOBBDistance.h"
		#endif
		#ifndef	CTCOBBOBBDISTANCE_H
			#include "CTC_OBBOBBDistance.h"
		#endif
		#ifndef	CTCCIRCLECIRCLEDISTANCE_H
			#include "CTC_CircleCircleDistance.h"
		#endif
		#ifndef	CTCEDGEEDGEDISTANCE_H
			#include "CTC_EdgeEdgeDistance.h"
		#endif
		#ifndef	CTCSEGMENTSEGMENTDISTANCE_H
			#include "CTC_SegmentSegmentDistance.h"
		#endif
		#ifndef	CTCPOINTRECTANGLEDISTANCE_H
			#include "CTC_PointRectangleDistance.h"
		#endif
		#ifndef	CTCSEGMENTRECTANGLEDISTANCE_H
			#include "CTC_SegmentRectangleDistance.h"
		#endif
		#ifndef	CTCSEGMENTOBBDISTANCE_H
			#include "CTC_SegmentOBBDistance.h"
		#endif
		#ifndef	CTCTRIANGLETRIANGLEDISTANCE_H
			#include "CTC_TriangleTriangleDistance.h"
		#endif
		#ifndef	CTCPOINTTRIANGLEDISTANCE_H
			#include "CTC_PointTriangleDistance.h"
		#endif
		#ifndef	CTCSEGMENTTRIANGLEDISTANCE_H
			#include "CTC_SegmentTriangleDistance.h"
		#endif
		#ifndef	CTCTRIANGLERECTANGLEDISTANCE_H
			#include "CTC_TriangleRectangleDistance.h"
		#endif
		#ifndef	CTCRECTANGLERECTANGLEDISTANCE_H
			#include "CTC_RectangleRectangleDistance.h"
		#endif
		#ifndef	CTCQUADQUADDISTANCE_H
			#include "CTC_QuadQuadDistance.h"
		#endif

		// Intersections

		#ifndef	CTCCAPSULEOBBOVERLAP_H
			#include "CTC_CapsuleOBBOverlap.h"
		#endif
		#ifndef	CTCPOINTTRIANGLEOVERLAP_H
			#include "CTC_PointTriangleOverlap.h"
		#endif
		#ifndef	CTCPOINTPOLYGONOVERLAP_H
			#include "CTC_PointPolygonOverlap.h"
		#endif
		#ifndef	CTCRAYAABBOVERLAP_H
			#include "CTC_RayAABBOverlap.h"
		#endif
		#ifndef	CTCSPHERESPHEREOVERLAP_H
			#include "CTC_SphereSphereOverlap.h"
		#endif
		#ifndef	CTCSPHEREAABBOVERLAP_H
			#include "CTC_SphereAABBOverlap.h"
		#endif
		#ifndef	CTCSPHEREPLANEOVERLAP_H
			#include "CTC_SpherePlaneOverlap.h"
		#endif
		#ifndef	CTCSPHEREPOINTOVERLAP_H
			#include "CTC_PointSphereOverlap.h"
		#endif
		#ifndef	CTCSPHEREOBBOVERLAP_H
			#include "CTC_SphereOBBOverlap.h"
		#endif
		#ifndef	CTCDISKPLANEOVERLAP_H
			#include "CTC_DiskPlaneOverlap.h"
		#endif
		#ifndef	CTCRAYPLANEOVERLAP_H
			#include "CTC_RayPlaneOverlap.h"
		#endif
		#ifndef	CTCRAYTRIANGLEOVERLAP_H
			#include "CTC_RayTriangleOverlap.h"
		#endif
		#ifndef	CTCTRIANGLETRIANGLEOVERLAP_H
			#include "CTC_TriangleTriangleOverlap.h"
		#endif
		#ifndef	CTCRAYSPHEREOVERLAP_H
			#include "CTC_RaySphereOverlap.h"
		#endif
		#ifndef	CTCTRIAABBOVERLAP_H
			#include "CTC_TriangleAABBOverlap.h"
		#endif
		#ifndef	CTCPOINTOBBOVERLAP_H
			#include "CTC_PointOBBOverlap.h"
		#endif
		#ifndef	CTCPLANEAABBOVERLAP_H
			#include "CTC_PlaneAABBOverlap.h"
		#endif
		#ifndef	CTCOBBOBBOVERLAP_H
			#include "CTC_OBBOBBOverlap.h"
		#endif
		#ifndef	CTCSPHERECONEOVERLAP_H
			#include "CTC_SphereConeOverlap.h"
		#endif
		#ifndef	CTCTRICYLINDEROVERLAP_H
			#include "CTC_TriangleCylinderOverlap.h"
		#endif
		#ifndef	CTCTRIANGLESPHEREOVERLAP_H
			#include "CTC_TriangleSphereOverlap.h"
		#endif

		// Contacts

		#ifndef	CTCCONTACT_H
			#include "CTC_Contact.h"
		#endif
		#ifndef	CTCPLANEPLANE_H
			#include "CTC_PlanePlane.h"
		#endif
		#ifndef	CTCPLANEPLANEPLANE_H
			#include "CTC_PlanePlanePlane.h"
		#endif
		#ifndef CTCSWEPTELLIPSOID_H
			#include "CTC_SweptEllipsoid.h"
		#endif
		#ifndef CTCSWEPTBOX_H
			#include "CTC_SweptBox.h"
		#endif
		#ifndef CTCSWEPTSPHERE_H
			#include "CTC_SweptSphere.h"
		#endif
		#ifndef CTCCONTINUOUS_H
			#include "CTC_Continuous.h"
		#endif
		#ifndef CTCDISTANCESOLVER_H
			#include "CTC_DistanceSolver.h"
		#endif
		#ifndef CTCEDGEEDGECONTACT_H
			#include "CTC_EdgeEdgeContact.h"
		#endif
		#ifndef CTCPOLYGONPOLYGONCONTACT_H
			#include "CTC_PolygonPolygonContact.h"
		#endif
	}

#endif // CONTACT_H
