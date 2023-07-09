///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Main file for IceMaths.dll.
 *	\file		IceMaths.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEMATHS_H
#define ICEMATHS_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compilation messages
#if defined(ICEMATHS_EXPORTS)
	#pragma message("Compiling ICE Maths")
#elif !defined(ICEMATHS_EXPORTS)
	#pragma message("Using ICE Maths")
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Automatic linking
	#ifndef BAN_ICEMATHS_AUTOLINK
		#ifdef _DEBUG
			#pragma comment(lib, "IceMaths_D.lib")
		#else
			#pragma comment(lib, "IceMaths.lib")
		#endif
	#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Preprocessor

#ifndef ICE_NO_DLL
	#ifdef ICEMATHS_EXPORTS
		#define ICEMATHS_API	__declspec(dllexport)
	#else
		#define ICEMATHS_API	__declspec(dllimport)
	#endif
#else
		#define ICEMATHS_API
#endif

	// Includes
	#include <Math.h>

	#include "IceMathsSettings.h"

	#ifdef COMPILER_VISUAL_CPP
		#pragma intrinsic(memcmp)
		#pragma intrinsic(memcpy)
		#pragma intrinsic(memset)
		#pragma intrinsic(strcat)
		#pragma intrinsic(strcmp)
		#pragma intrinsic(strcpy)
		#pragma intrinsic(strlen)
		#pragma intrinsic(abs)
		#pragma intrinsic(labs)

		#pragma intrinsic(fabs)
		#pragma intrinsic(asin)
		#pragma intrinsic(acos)
		#pragma intrinsic(atan)
		#pragma intrinsic(sinh)
		#pragma intrinsic(cosh)
		#pragma intrinsic(tanh)
		#pragma intrinsic(fmod)
		#pragma intrinsic(pow)
		#pragma intrinsic(exp)
		#pragma intrinsic(sqrt)
		#pragma intrinsic(log)
		#pragma intrinsic(log10)
		#pragma intrinsic(sin)
		#pragma intrinsic(cos)
		#pragma intrinsic(tan)
	#endif

	namespace IceMaths
	{
ICEMATHS_API void* CreateClass(const char* name);

		// Numerics
		#ifndef	ICEAPPROX_H
			#include "IceApprox.h"
		#endif
		#ifndef	ICECUBEROOT_H
			#include "IceCubeRoot.h"
		#endif
		#ifndef	ICEROOTS_H
			#include "IceRoots.h"
		#endif
		#ifndef	ICEPOLYNOMIAL_H
			#include "IcePolynomial.h"
		#endif

		// Basic
		#ifndef	ICEAXES_H
			#include "IceAxes.h"
		#endif
		#ifndef	ICEEULERANGLES_H
			#include "IceEulerAngles.h"
		#endif
		#ifndef	ICEPOINT_H
			#include "IcePoint.h"
		#endif
		#ifndef	ICEHPOINT_H
			#include "IceHPoint.h"
		#endif
		#ifndef	ICEMATRIX3X3_H
			#include "IceMatrix3x3.h"
		#endif
		#ifndef	ICEMATRIX4X4_H
			#include "IceMatrix4x4.h"
		#endif
		#ifndef	ICESPECIALMATRICES_H
			#include "IceSpecialMatrices.h"
		#endif
		#ifndef	ICEMATRIXNXN_H
			#include "IceMatrixNxN.h"
		#endif
		#ifndef	ICEQUAT_H
			#include "IceQuat.h"
		#endif
		#ifndef	ICEANGLEAXIS_H
			#include "IceAngleAxis.h"
		#endif
		#ifndef	ICEPRS_H
			#include "IcePRS.h"
		#endif
		#ifndef	ICEBASIS_H
			#include "IceBasis.h"
		#endif

		//! A function to access a global 3x3 identity matrix
		FUNCTION ICEMATHS_API const Matrix3x3* Get3x3IdentityMatrix();

		//! A function to access a global 4x4 identity matrix
		FUNCTION ICEMATHS_API const Matrix4x4* Get4x4IdentityMatrix();

		//
		#ifndef	ICECUBEINDEX_H
			#include "IceCubeIndex.h"
		#endif
		#ifndef	ICEMOMENT_H
			#include "IceMoment.h"
		#endif
	#ifndef	ICEPLANE_H
		#include "IcePlane.h"
	#endif
		#ifndef	ICETRIANGLE_H
			#include "IceTriangle.h"
		#endif
		#ifndef	ICEINDEXEDTRIANGLE_H
			#include "IceIndexedTriangle.h"
		#endif
		#ifndef	ICEQUAD_H
			#include "IceQuad.h"
		#endif
		#ifndef	ICEPOLYGON_H
			#include "IcePolygon.h"
		#endif

		// Shapes
		#ifndef	ICECIRCLE_H
			#include "IceCircle.h"
		#endif
		#ifndef	ICECYLINDER_H
			#include "IceCylinder.h"
		#endif
		#ifndef	ICERAY_H
			#include "IceRay.h"
		#endif
		#ifndef	ICESEGMENT_H
			#include "IceSegment.h"
		#endif
		#ifndef	ICERECTANGLE3_H
			#include "IceRectangle3.h"
		#endif
		#ifndef	ICEAABB_H
			#include "IceAABB.h"
		#endif
		#ifndef	ICEMINIBALL_H
			#include "IceMiniball.h"
		#endif
		#ifndef	ICEBOUNDINGSPHERE_H
			#include "IceBoundingSphere.h"
		#endif
		#ifndef	ICEOBB_H
			#include "IceOBB.h"
		#endif
		#ifndef	ICELSS_H
			#include "IceLSS.h"
		#endif
		#ifndef	ICERSS_H
			#include "IceRSS.h"
		#endif
	#ifndef	ICETSS_H
		#include "IceTSS.h"
	#endif

		// Containers
		#ifndef	ICEVERTICES_H
			#include "IceVertices.h"
		#endif
		#ifndef	ICEMATRIXPALETTE_H
			#include "IceMatrixPalette.h"
		#endif
		#ifndef	ICETRILIST_H
			#include "IceTrilist.h"
		#endif
		#ifndef	ICEPLANES_H
			#include "IcePlanes.h"
		#endif

		// Advanced
		#ifndef	ICEGAUSSMAP_H
			#include "IceGaussMap.h"
		#endif
		#ifndef	ICEPRNODE_H
			#include "IcePRNode.h"
		#endif
		#ifndef	ICEEIGEN_H
			#include "IceEigen.h"
		#endif
		#ifndef	ICEPROGRESSIVEEIGEN_H
			#include "IceProgressiveEigen.h"
		#endif
		#ifndef	ICEFFT_H
			#include "IceFFT.h"
		#endif
		#ifndef	ICEVOLUMEINTEGRATION_H
			#include "IceVolumeIntegration.h"
		#endif
		#ifndef	ICEMASS_H
			#include "IceMass.h"
		#endif
		#ifndef	ICEINERTIATENSOR_H
			#include "IceInertiaTensor.h"
		#endif
		#ifndef	ICECATMULLROM_H
			#include "IceCatmullRom.h"
		#endif
		#ifndef	ICESPLINE_H
			#include "IceSpline.h"
		#endif
		#ifndef	ICEZCURVE_H
			#include "IceZCurve.h"
		#endif
		#ifndef	ICEIK_H
			#include "IceIK.h"
		#endif
		#ifndef	ICEFILTERS_H
			#include "IceFilters.h"
		#endif
		#ifndef	ICEJACOBI_H
			#include "IceJacobi.h"
		#endif
		#ifndef	ICESCREWING_H
			#include "IceScrewing.h"
		#endif
		#ifndef	ICEINTERVAL_H
			#include "IceInterval.h"
		#endif
		#ifndef	ICEINCLUSION_H
			#include "IceInclusion.h"
		#endif

		// Noise
		#ifndef	ICEWHITEGAUSSIANNOISE_H
			#include "IceWhiteGaussianNoise.h"
		#endif
		#ifndef	ICEPERLINNOISE_H
			#include "IcePerlinNoise.h"
		#endif
		#ifndef	ICEFRACTALBROWNIANMOTION_H
			#include "IceFractalBrownianMotion.h"
		#endif
		#ifndef	ICERIDGEDFRACTAL_H
			#include "IceRidgedFractal.h"
		#endif
		#ifndef	ICEWORLEYNOISE_H
			#include "IceWorleyNoise.h"
		#endif
		#ifndef	ICEPHARRNOISE_H
			#include "IcePharrNoise.h"
		#endif
		#ifndef	ICESKINNERNOISE_H
			#include "IceSkinnerNoise.h"
		#endif



		//! remap (0->1) to (0->1) but with derivative continuity
		inline_ float HermiteLerpParameter(const float t)
		{
//			gAssert( t >= - EPSILON && t <= 1.f + EPSILON );
			return (3.0f - 2.0f*t)*t*t;
		}

		//! even better continuity than Hermite lerp, but slower
		inline_ float CosLerpParameter(const float t)
		{
//			gAssert( t >= - EPSILON && t <= 1.f + EPSILON );
			return 0.5f - 0.5f * cosf(t*PI);
		}

		// Returns angle between 0 and PI in radians
		inline_ float SafeACos(float f)
		{
			// Take care of FPU inaccuracies
			if(f>1.0f)	return 0.0f;
			if(f<-1.0f)	return PI;
						return acosf(f);
		}

		// Returns angle between -PI/2 and PI/2 in radians
		inline_ float SafeASin(float f)
		{
			// Take care of FPU inaccuracies
			if(f>1.0f)	return HALFPI;
			if(f<-1.0f)	return -HALFPI;
						return asinf(f);
		}

		// Returns angle between -PI/2 and PI/2 in radians
		inline_ float SafeATan(float f)
		{
			return atanf(f);
		}

		// Returns angle between -PI and PI in radians
		inline_ float SafeATan(float sin, float cos)
		{
			return atan2f(sin, cos);
		}

		// Returns the angle between two (possibly un-normalized) vectors
		inline_ float Angle(const Point& v0, const Point& v1)
		{
			float Cos = v0|v1;					// |v0|*|v1|*Cos(Angle)
			float Sin = (v0^v1).Magnitude();	// |v0|*|v1|*Sin(Angle)
			return SafeATan(Sin, Cos);
		}

		#ifndef	ICEMATHSAPI_H
			#include "IceMathsAPI.h"
		#endif

		class ICEMATHS_API DynamicPR
		{
			public:
			inline_	DynamicPR()		{}
			inline_	DynamicPR(const PR& pr, const Point& v, const Point& w)	: mInitPose(pr), mLinearVelocity(v), mAngularVelocity(w)	{}
			inline_	~DynamicPR()	{}

			PR		mInitPose;			//!< Initial pose at start of time interval
			Point	mLinearVelocity;	//!< Linear velocity over the interval
			Point	mAngularVelocity;	//!< Angular velocity over the interval
		};

		// Temp helpers
		ICEMATHS_API void Integrate(PR& dst, const PR& src, const Point& v, const Point& w, float time_step=1.0f);
		ICEMATHS_API void Desintegrate(Point& v, Point& w, const PR& dst, const PR& src, float time_step=1.0f);

		inline_ void Integrate(PR& dst, const DynamicPR& pose, float time_step=1.0f)
		{
			Integrate(dst, pose.mInitPose, pose.mLinearVelocity, pose.mAngularVelocity, time_step);
		}
	}

#endif // ICEMATHS_H
