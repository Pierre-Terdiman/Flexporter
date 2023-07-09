///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for Point-Triangle intersection
 *	\file		CTC_PointTriangleOverlap.h
 *	\author		Pierre Terdiman
 *	\date		January, 15, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CTCPOINTTRIANGLEOVERLAP_H
#define CTCPOINTTRIANGLEOVERLAP_H

	// From Mr Gamemaker
	// http://pub137.ezboard.com/fmrgamemakerfrm0.showMessage?topicID=2687.topic
	inline_ BOOL PointInTriangle(const Point& p, const Point& p0, const Point& edge10, const Point& edge20)
	{ 
		float a = edge10|edge10;
		float b = edge10|edge20;
		float c = edge20|edge20;
		float ac_bb = (a*c)-(b*b);

		Point vp = p - p0;

		float d = vp|edge10;
		float e = vp|edge20;

		float x = (d*c) - (e*b);
		float y = (e*a) - (d*b);
		float z = x + y - ac_bb;

		// Same as: if(x>0.0f && y>0.0f && z<0.0f)	return TRUE;
		//			else							return FALSE;
		return (( IR(z) & ~(IR(x)|IR(y)) ) & SIGN_BITMASK);
	}

	//! Dedicated 2D version
	inline_ BOOL PointInTriangle2D(	float px, float pz,
									float p0x, float p0z,
									float e10x, float e10z,
									float e20x, float e20z)
	{ 
		float a = e10x*e10x + e10z*e10z;
		float b = e10x*e20x + e10z*e20z;
		float c = e20x*e20x + e20z*e20z;
		float ac_bb = (a*c)-(b*b);

		float vpx = px - p0x;
		float vpz = pz - p0z;

		float d = vpx*e10x + vpz*e10z;
		float e = vpx*e20x + vpz*e20z;

		float x = (d*c) - (e*b);
		float y = (e*a) - (d*b);
		float z = x + y - ac_bb;

		// Same as: if(x>0.0f && y>0.0f && z<0.0f)	return TRUE;
		//			else							return FALSE;
		return (( IR(z) & ~(IR(x)|IR(y)) ) & SIGN_BITMASK);
	}

#endif // CTCPOINTTRIANGLEOVERLAP_H

