///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for quaternions.
 *	\file		IceQuat.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ICEQUAT_H
#define ICEQUAT_H

	// Forward declarations
	class AngleAxis;

	#define QUAT_EPSILON	(1.0e-8f)

	class ICEMATHS_API Quat
	{
		public:

		//! Empty constructor
		inline_				Quat()										{}
		//! Copy constructor
		inline_				Quat(const Quat& q) : w(q.w), p(q.p)		{}
		//! Constructor from floats
		inline_				Quat(float s, float x, float y, float z)	{ p.x = x; p.y = y; p.z = z; w = s;		}
		//! Constructor from an angle and a vector.
		inline_				Quat(float _w, const Point& _p)				{ p = _p; w = _w;						}
		//! Constructor from an AngleAxis.
							Quat(const AngleAxis& a);
		//! Destructor
		inline_				~Quat()										{}

		//! Clears the quaternion.
		inline_	void		Zero()										{ p.x = p.y = p.z =			w = 0.0f;	}
		//! Sets the identity quaternion.
		inline_	void		Identity()									{ p.x = p.y = p.z = 0.0f;	w = 1.0f;	}

		inline_	BOOL		IsIdentity()						const
							{
								if(IR(p.x))			return FALSE;
								if(IR(p.y))			return FALSE;
								if(IR(p.z))			return FALSE;
								if(IR(w)!=IEEE_1_0)	return FALSE;
								return TRUE;
							}

		//! Checks quat validity
		inline_	BOOL		IsValid()							const
							{
								if(!IsValidFloat(p.x))	return FALSE;
								if(!IsValidFloat(p.y))	return FALSE;
								if(!IsValidFloat(p.z))	return FALSE;
								if(!IsValidFloat(w))	return FALSE;
								return TRUE;
							}

		//! Assignment from floats.
		inline_	void		Set(float x, float y, float z, float s)		{ p.x = x; p.y = y; p.z = z; w = s;		}
		//! Assignment from a vector and a float.
		inline_	void		Set(const Point _p, float _w)				{ p = _p;					w = _w;		}
		//! Assignment from an AngleAxis..
				void		Set(const AngleAxis& a);

		//! Computes the square magnitude.
		inline_	float		SquareMagnitude()					const	{ return p.x*p.x + p.y*p.y + p.z*p.z + w*w;		}
		//! Computes the magnitude.
		inline_	float		Magnitude()							const	{ return sqrtf(SquareMagnitude());				}

		//! Normalizes the quaternion.
				void		Normalize()
							{
								float Norm = Magnitude();

								if(Norm<QUAT_EPSILON || fabsf(Norm - 1.0f) < QUAT_EPSILON)	return;

								float OneOverNorm = 1.0f / Norm;
								w *= OneOverNorm;
								p *= OneOverNorm;
							}

		//! Checks the quat is normalized or not
		inline_	bool		IsNormalized()						const	{ return fabsf((w*w) + (p|p) - 1.0f ) < QUAT_EPSILON*QUAT_EPSILON;	}

		//! Inverts a quaternion.
				Quat&		Invert()
							{
								float Norm = SquareMagnitude();
								if(Norm < QUAT_EPSILON)	return *this;
								float OneOverNorm = 1.0f / Norm;
								p *= -OneOverNorm;
								w *= OneOverNorm;
								return *this;
							}

		//! Rotates the quaternion by PI/2 (quaternion space metric) about axis.
		inline_	void		QuatOrtho(const Point& axis)				{ *this *= Quat(0.0f, axis);	}

		//! Modifies the quaternion so that it stands on the same side of the hypersphere as q.
		inline_	void		MakeClosest(const Quat& q)					{ if(((*this)|q)<0.0f)	*this = -*this;	/* If dot product is < 0*/	}

		//! Makes the quaternion the Spherical Linear Interpolation quaternion (Original code by Dave Eberly)
		inline_	void		MakeSlerp(const Quat& q0, const Quat& q1, float t);
		//! Same as "MakeSlerp" except the fact it makes a slerp between 2 quaternions on the same hemisphere.
		inline_	void		MakeShortestSlerp(const Quat& q0, const Quat& q1, float t);
		//! TO BE DOCUMENTED
		inline_	void		MakeSquad(const Quat& q0, const Quat& q1, const Quat& a, const Quat& b, float t);
		//! TO BE DOCUMENTED
				void		MakeSquadRev(const AngleAxis& aa, const Quat& q0, const Quat& q1, const Quat& a, const Quat& b, float t);

		//! TO BE DOCUMENTED
		inline_	Quat		Slerp(const Quat& q, float t);

				void		FromMatrix(const Matrix3x3& mat);

		//! Shortest Arc quaternion from Stan Melax
				void		RotationArc(const Point& v0, const Point& v1);

		//! Makes the quaternion logarithmic (Original code by Dave Eberly)
				void		Log();
		//! Takes the natural logarithm of UNIT quaternion.
				void		LogN();
		//! Exponentiates the quaternion (where w==0).
				void		Exp();
		//! Computes the "log difference" of two quaternions, as  ln(qinv('this') * q)
				void		LnDif(const Quat& q);

		//! Stuff magic values in the quat, marking it as explicitely not used.
				void		SetNotUsed();
		//! Checks the quat is marked as not used
				BOOL		IsNotUsed()					const;

		// Cast operators
		//! Cast a Quat to an AngleAxis.
							operator AngleAxis()		const;
		//! Cast a Quat to a Matrix3x3.
		inline_				operator Matrix3x3()		const;
		//! Cast a Quat to a Matrix4x4.
		inline_				operator Matrix4x4()		const;

		// Assignment operators
		//! Operator for Quat = Quat.
		inline_	Quat		operator=(const Quat& q)					{ w = q.w; p = q.p; return *this;											}

		// Arithmetic operators
		//! Operator for Quat = -Quat.
		inline_	Quat		operator-()					const			{ return Quat(-w, -p.x, -p.y, -p.z);										}

		//! Operator for Quat = Quat + Quat.
		inline_	Quat		operator+(const Quat& q)	const			{ return Quat(w + q.w, p + q.p);											}
		//! Operator for Quat = Quat * Quat.
		inline_	Quat		operator*(const Quat& q)	const			{ return Quat((w * q.w) - (p | q.p), (w * q.p) + (p * q.w) + (p ^ q.p));	}
		//! Operator for Quat = Quat * float.
		inline_	Quat		operator*(float s)			const			{ return Quat(w * s, p * s);												}
		//! Operator for Quat = Quat / Quat.
		inline_	Quat		operator/(const Quat& q)	const			{ return Quat(q).Invert() * *this;											}
		//! Operator for Quat = Quat / float.
		inline_	Quat		operator/(float s)			const			{ s = 1.0f / s; return Quat(w * s, p * s);									}
		//! Operator for float = Quat | Quat.
		inline_	float		operator|(const Quat& q)	const			{ return (w * q.w) + (p | q.p);												}

		//! Operator for Quat += Quat.
		inline_	Quat&		operator+=(const Quat& q)					{ w += q.w; p += q.p; return *this;											}
		//! Operator for Quat *= Quat;
		inline_	Quat&		operator*=(const Quat& q)
							{
								float Oldw = w;
								w = (w * q.w) - (p | q.p);
								p = (Oldw * q.p) + (p * q.w) + (p ^ q.p);
								return	*this;
							}

		//! Operator for Quat *= float.
		inline_	Quat&		operator*=(float s)							{ w *= s; p *= s; return *this;												}
		//! Operator for Quat /= Quat.
//		inline_	Quat&		operator/=(const Quat& q)					{ *this *= Quat(q).Invert(); return *this;									}
		inline_	Quat&		operator/=(const Quat& q)					{ *this = Quat(q).Invert() * *this; return *this;							}
		//! Operator for Quat /= float.
		inline_	Quat&		operator/=(float s)							{ s = 1.0f / s; w *= s; p *= s; return *this;								}

		// Logic operators
//		inline	bool	operator==(const Quat& q)						{ return p == q.p && FastFabs(w - q.w) < QUAT_EPSILON;						}
//		inline	bool	operator!=(const Quat& q)						{ return p != q.p || FastFabs(w - q.w) > QUAT_EPSILON;						}

		//! Operator for "if(Quat==Quat)"
		inline_	bool		operator==(const Quat& q)	const			{ return ( (IR(w)==IR(q.w)) && (IR(p.x)==IR(q.p.x)) && (IR(p.y)==IR(q.p.y)) && (IR(p.z)==IR(q.p.z)));	}
		//! Operator for "if(Quat!=Quat)"
		inline_	bool		operator!=(const Quat& q)	const			{ return ( (IR(w)!=IR(q.w)) || (IR(p.x)!=IR(q.p.x)) || (IR(p.y)!=IR(q.p.y)) || (IR(p.z)!=IR(q.p.z)));	}

		// Friend methods
		//! Operator for Quat = float * Quat.
		inline_	friend Quat operator*(float s, const Quat& q)			{ return Quat(s * q.w, s * q.p);											}

		// Removed, dangerous
//		inline_				operator	const	float*()	const		{ return &p.x;																}
//		inline_				operator			float*()				{ return &p.x;																}

		public:
				Point		p;		//!< Vector part
				float		w;		//!< Scalar part
	};

	ICEMATHS_API void EulerToQuat(const float* ang, Quat& q);
	ICEMATHS_API void QuatToEuler(const Quat& q, float* ang);

	ICEMATHS_API void QuatToDir(const Quat& q, Point& dir);
	ICEMATHS_API void DirToQuat(const Point& dir, Quat& q);

	//! Packs a (unit) quat on 3 floats
	inline_	void QuatToPoint(const Quat& q, Point& p)
	{
		// Doing this, branchless:
		// Point Dir = q.p;
		// if(q.w<0.0f)	Dir.Neg();
		udword* Src = (udword*)&q;
		udword* Dst = (udword*)&p;
		udword Sign = Src[3] & SIGN_BITMASK;
		*Dst++ = *Src++ | Sign;
		*Dst++ = *Src++ | Sign;
		*Dst++ = *Src++ | Sign;
	}

	//! Unpacks the (unit) quat from 3 floats
	inline_	void PointToQuat(const Point& p, Quat& q)
	{
		q.p = p;
		q.w = sqrtf(1.0f - p.SquareMagnitude());
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Make the quaternion the Spherical Linear Interpolation quaternion (Original routine by Dave Eberly)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input	: 
//	- q0, source quaternion
//	- q1, destination quaternion
//	- t, interpolation parameter in the interval [0 , 1]
// Output	: None
// Remark	: None
inline_ void Quat::MakeSlerp(const Quat& q0, const Quat& q1, float t)
{
	float Cos = q0|q1;
	float Sin = 1.0f - Cos*Cos;
	if(Sin < QUAT_EPSILON*QUAT_EPSILON)
	{
		*this = q0;
		return;
	}
	Sin=sqrtf(Sin);

	float Angle = atan2f(Sin, Cos);
	float InvSine = 1.0f / Sin;
	float c0 = sinf((1.0f-t) * Angle) * InvSine;
	float c1 = sinf(       t * Angle) * InvSine;

    *this = c0 * q0 + c1 * q1;

/*	float	omega, cosom, sinom, scl0, scl1;
	int		i;
	Quat	qt;

	cosom = q0 | q1;

#define COS_EPSILON 0.000001f

	if ( (1.0f + cosom) > COS_EPSILON)
	{	// Usual case
		if ( (1.0f - cosom) > COS_EPSILON )
		{	// Usual case
			omega = (float) acos( cosom );
			sinom = (float) sin( omega );
			scl0 = (float) sin( (1-t) * omega ) / sinom;
			scl1 = (float) sin(   t   * omega ) / sinom;
		}
		else
		{	// ends very close -- just lerp 
			scl0 = 1 - t;
			scl1 = t;
		}
	}
	else
	{	// p and q nearly opposite on sphere-- this is a 360 degree
		// rotation, but the axis of rotation is undefined, so
		// slerp really is undefined too.  So this apparently picks 
		// an arbitrary plane of rotation. However, I think this 
		// code is incorrect.

		qt.x = -p.y;  qt.y = p.x;  // qt is orthogonal to p 
		qt.z = -p.w;  qt.w = p.z;
		scl0 = (float) sin( (1-t) * HALFPI );
		scl1 = (float) sin( t * HALFPI );
	}

	*this = scl0 * q0 + scl1 * q1;
*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Make the quaternion the Spherical Linear Interpolation quaternion (Original routine from Game Developper)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input	: 
//	- q0, source quaternion
//	- q1, destination quaternion
//	- t, interpolation parameter in the interval [0 , 1]
// Output	: None
// Remark	: None
/*
inline_ void Quat::MakeShortestSlerp(const Quat& q0, const Quat& q1, float t)
{
	Quat qq0(q0);
	qq0.MakeClosest(q1);

	float Cos = qq0|q1;
	float Sin = sqrtf(1.0f - Cos*Cos);
	if(Sin<QUAT_EPSILON)
	{
		*this = qq0;
		return;
	}

	float Angle = atan2f(Sin, Cos);
	float InvSine = 1.0f / Sin;
	float c0 = sinf((1.0f-t) * Angle ) * InvSine;
	float c1 = sinf(     t   * Angle ) * InvSine;

	*this = c0 * qq0 + c1 * q1;
}
*/
inline_ void Quat::MakeShortestSlerp(const Quat& q0, const Quat& q1, float t)
{
	*this = q0;
	MakeClosest(q1);

	float Cos = *this|q1;
	float Sin = 1.0f - Cos*Cos;
	if(Sin<QUAT_EPSILON*QUAT_EPSILON)	return;
	Sin = sqrtf(Sin);

	float Angle = atan2f(Sin, Cos);
	float InvSine = 1.0f / Sin;
	float c0 = sinf((1.0f-t) * Angle ) * InvSine;
	float c1 = sinf(     t   * Angle ) * InvSine;

	*this *= c0;
	*this += q1*c1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Make the quaternion the Spherical Quadratic Interpolation quatrenion (Original routine by Dave Eberly)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input	: 
//	- q0, source quaternion
//	- q1, destination quaterion
//	- a, source tangent quaternion
//	- b, destination tangent quaternion
//	- t, interpolation parameter in the interval [0 , 1]
// Output	: None
// Remark	: None
inline_ void Quat::MakeSquad(const Quat& q0, const Quat& q1, const Quat& a, const Quat& b, float t)
{
	Quat Slerp0, Slerp1;

	Slerp0.MakeSlerp(q0, q1, t);
	Slerp1.MakeSlerp(a, b, t);
    MakeSlerp(Slerp0, Slerp1, 2*t*(1-t));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Compute the Spherical Linear Interpolation (Original routine by Dave Eberly)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input	: 
//	- q, destination quaternion
//	- t, interpolation parameter in the interval [0 , 1]
// Output	: None
// Return	: SLERP( (*this), q, t )  (Type 'Quat')
// Remark	: Same as 'MakeSlerp' except this routine instantiates a new quaternion
inline_ Quat Quat::Slerp(const Quat& q, float t)
{
	float Cos = (*this) | q;
	float Sin = sqrtf(fabsf(1.0f - Cos*Cos));
	if(fabsf(Sin)<QUAT_EPSILON)
		return Quat(*this);

	float Angle = atan2f(Sin, Cos), InvSine = 1.0f / Sin;
	float c0 = sinf( (1.0f-t) * Angle) * InvSine;
	float c1 = sinf(      t   * Angle) * InvSine;

	return Quat(c0 * (*this) + c1 * q);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Operator to cast a quaternion to a 3x3 matrix.
 *	Only works with unit quaternions, pay attention to what you do... I don't normalize quat before proceding for speed reasons.
 *	\return		a 3x3 matrix built from the quaternion
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline_ Quat::operator Matrix3x3() const
{
	float xs = p.x + p.x;	float ys = p.y + p.y;	float zs = p.z + p.z;

	float wx = w * xs;		float wy = w * ys;		float wz = w * zs;
	float xx = p.x * xs;	float xy = p.x * ys;	float xz = p.x * zs;
	float yy = p.y * ys;	float yz = p.y * zs;	float zz = p.z * zs;

	// Ugly but helps the RVO
	return Matrix3x3(
		1.0f -	yy - zz,
				xy + wz,
				xz - wy,

				xy - wz,
		1.0f -	xx - zz,
				yz + wx,

				xz + wy,
				yz - wx,
		1.0f -	xx - yy);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Operator to cast a quaternion to a 4x4 matrix.
 *	Only works with unit quaternions, pay attention to what you do... I don't normalize quat before proceding for speed reasons.
 *	\return		a 4x4 matrix built from the quaternion
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline_ Quat::operator Matrix4x4() const
{
	float xs = p.x + p.x;	float ys = p.y + p.y;	float zs = p.z + p.z;

	float wx = w * xs;		float wy = w * ys;		float wz = w * zs;
	float xx = p.x * xs;	float xy = p.x * ys;	float xz = p.x * zs;
	float yy = p.y * ys;	float yz = p.y * zs;	float zz = p.z * zs;

	// Ugly but helps the RVO
	return Matrix4x4(
		1.0f -	yy - zz,
				xy + wz,
				xz - wy,
				0.0f,

				xy - wz,
		1.0f -	xx - zz,
				yz + wx,
				0.0f,

				xz + wy,
				yz - wx,
		1.0f -	xx - yy,
				0.0f,

				0.0f,
				0.0f,
				0.0f,
				1.0f);
}

#endif // ICEQUAT_H
