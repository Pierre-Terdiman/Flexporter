///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code to find various roots.
 *	\file		IceRoots.h
 *	\author		Ken Turkowski
 *	\date		1997-1999
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEROOTS_H
#define ICEROOTS_H

	#ifdef DOUBLE_PRECISION
	# define FLOAT double
	#else
	# define FLOAT float
	#endif

	FUNCTION ICEMATHS_API	long	FindCubicRoots(const FLOAT coeff[4], FLOAT x[3]);
	FUNCTION ICEMATHS_API	long	FindQuadraticRoots(const FLOAT coeff[3], FLOAT re[2], FLOAT im[2]);
	FUNCTION ICEMATHS_API	void	FindPolynomialRoots(const FLOAT		*a,			// Coefficients
														FLOAT			*u,			// Real component of each root
														FLOAT			*v,			// Imaginary component of each root
														FLOAT			*conv,		// Convergence constant associated with each root
														register long	n,			// Degree of polynomial (order-1)
														long			maxiter,	// Maximum number of iterations
														long			fig			// The number of decimal figures to be computed
													);
	FUNCTION ICEMATHS_API	long	FindZerosOfFunction(FLOAT	(*func)(FLOAT),	// The function whose zeros are to be found
														FLOAT	*root,			// The roots
														long	n,				// The number of roots sought
														long	maxiter,		// Maximum number of iterations
														FLOAT	epsr,			// Relative convergence criterion on successive iterates
														FLOAT	epsf,			// Absolute convergence criterion on the function values
														FLOAT	epsmr,			// Closeness of multiple roots (see mrspr)
														FLOAT	mrspr			// The spread for multiple roots, that is, if |rt - root[i]| < epsmr,
																				// where root[i] is a previously found root,
																				// then rt is replaced by rt + mrspr.
													);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Computes real quadratic roots for equ. ax^2 + bx + c = 0
	 *
	 *	\param		a	[in] a coeff
	 *	\param		b	[in] b coeff
	 *	\param		c	[in] c coeff
	 *	\param		x0	[out] smallest real root
	 *	\param		x1	[out] largest real root
	 *	\return		number of real roots
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline_ udword FindRealQuadraticRoots(float a, float b, float c, float& x0, float& x1)
	{
		if(a==0.0f)
		{
			// bx + c = 0
			if(b!=0.0f)
			{
				x0 = -c / b;
				return 1;
			}
			else return 0;
		}

		// Compute delta
		float Delta = b*b - 4.0f*a*c;
		if(Delta < 0.0f)	return 0;	// Complex roots

		if(Delta==0.0f)
		{
			x0 = -b / (2.0f * a);
			return 1;
		}
		else
		{
			Delta = sqrtf(Delta);

			b = -b;
			a = 0.5f / a;

			b *= a;
			Delta *= a;

			x0 = b - Delta;
			x1 = b + Delta;

			if(x0>x1)	TSwap(x0,x1);

			return 2;
		}
	}

#endif // ICEROOTS_H

