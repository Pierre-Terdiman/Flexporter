///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for spherical harmonics.
 *	\file		IceSphericalHarmonics.h
 *	\author		Pierre Terdiman, original code by Peter Young
 *	\date		April, 21, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICESPHERICALHARMONICS_H
#define ICESPHERICALHARMONICS_H


	struct ICEPRELIGHTER_API MySHSample
	{
		Point	sph;
		Point	vec;
		double	coeff[25];
	};

	typedef double (*MySH_polar_fn)(double theta, double phi);



//	class SHSample;

	struct ICEPRELIGHTER_API SHCREATE : PRELIGHTER_CREATE
	{
				SHCREATE();

		udword	mNbBands;
		udword	mSqrtNbSamples;
		udword	mNbBounces;
	};

	class ICEPRELIGHTER_API SHPrelighter : public Prelighter
	{
		public:
							SHPrelighter();
		virtual				~SHPrelighter();

		virtual	bool		PrecomputeLighting(const PRELIGHTER_CREATE& create);

		private:
				udword		mSqrtNbSamples;	// sqrt n_samples = 10000
				udword		mNbBands;		// number of SH bands
				udword		mNbSamples;		// total number of samples
				udword		mCoeff;			// number of SH coefficients per sample
				MySHSample*	mSamples;		// samples - direction vector and SH coefficients
				udword		mNbBounces;
				double*		mSHCoeff;
				double*		mSHCoeffWhite;
				double*		mResult;
				double*		mResultWhite;
		// Internal methods
				bool		PrecomputeMeshSH(PrelighterMesh* mesh);
				void		IntegrateSphere(const Point& pos, const Point& normal, const Point& kd, double* coeff, double* coeff_white);

		inline_	bool		IsValidSample(udword i, const Point& normal)
							{
								return (mSamples[i].vec | normal) > 0.0f;
							}
				bool		ClosestFace(const Ray& world_ray, float max_dist, float& u, float& v, udword& v0, udword& v1, udword& v2)	const;
	};

	class ICEPRELIGHTER_API SHData
	{
		public:
							SHData();
		virtual				~SHData();

				bool		Load(const char* filename);

				udword		mNbLightingPoints;
				udword		mCoeff;
				udword		mNbSamples;
				udword		mNbBands;
				udword		mNbBounces;
				float*		mSHCoeff;
				float*		mSHCoeffWhite;
	};

#endif // ICESPHERICALHARMONICS_H
