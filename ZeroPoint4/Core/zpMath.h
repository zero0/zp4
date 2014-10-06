#pragma once
#ifndef ZP_MATH_H
#define ZP_MATH_H

#define ZP_EPSILON	( 1e-8f )
#define ZP_PI		( 3.14159265359f )
#define ZP_1OVERPI	( 1.0f / ZP_PI )
#define ZP_2PI		( 2.0f * ZP_PI )
#define ZP_PIOVER2	( 0.5f * ZP_PI )
#define ZP_PIOVER4	( 0.25f* ZP_PI ) 
#define ZP_FLT_MAX	( 3.402823466e+38F )
#define ZP_FLT_MIN	( 1.175494351e-38F )

#define ZP_RAD_TO_DEG( r )	( ( (r) * 180.f ) * ZP_1OVERPI )
#define ZP_DEG_TO_RAD( d )	( ( (d) * ZP_PI ) / 180.0f )

#define ZP_MIN( a, b )	( ( (a) < (b) ) ? (a) : (b) )
#define ZP_MAX( a, b )	( ( (a) > (b) ) ? (a) : (b) )

template<typename T>
ZP_FORCE_INLINE void zp_saturate( T& s, const T& val )
{
	s = val > (T)1 ? (T)1 : val < (T)0 ? (T)0 : val;
}

template<typename T>
ZP_FORCE_INLINE void zp_clamp( T& s, const T& val, const T& low, const T& high )
{
	s = val > high ? high : val < low ? low : val;
}

template<typename T>
ZP_FORCE_INLINE void zp_lerp( T& s, const T& low, const T& high, zp_float alpha )
{
	zp_saturate( alpha, alpha );
	s = low + (T)( ( high - low ) * alpha );
}

zp_float zp_cos( zp_float v );
zp_float zp_sin( zp_float v );
zp_float zp_tan( zp_float v );

zp_float zp_acos( zp_float v );
zp_float zp_asin( zp_float v );
zp_float zp_atan( zp_float v );

zp_float zp_atan2( zp_float y, zp_float x );

zp_float zp_pow( zp_float b, zp_float p );

zp_float zp_sqrt( zp_float v );

zp_float zp_ceil( zp_float v );
zp_float zp_floor( zp_float v );

zp_int zp_ceil_to_int( zp_float v );
zp_int zp_floor_to_int( zp_float v );

zp_bool zp_approximate( zp_float a, zp_float b );

template<typename T>
ZP_FORCE_INLINE void zp_abs( T& a, const T& value )
{
	a = value > (T)0 ? value : -value;
}

struct zpScalar;
struct zp_vec4;
class zpVector4f;
class zpMatrix4f;
class zpPlane;
class zpFrustum;
class zpQuaternion4f;

class zpRay;
class zpBoundingAABB;
class zpBoundingSphere;
class zpCollision;

namespace zpMath
{
	ZP_FORCE_INLINE void DegToRad( zpScalar& s, const zpScalar& a );
	ZP_FORCE_INLINE void RadToDeg( zpScalar& s, const zpScalar& a );

	ZP_FORCE_INLINE void Sin( zpScalar& s, const zpScalar& a );
	ZP_FORCE_INLINE void Cos( zpScalar& s, const zpScalar& a );
	ZP_FORCE_INLINE void Tan( zpScalar& s, const zpScalar& a );

	ZP_FORCE_INLINE void Add( zpScalar& s, const zpScalar& a, const zpScalar& b );
	ZP_FORCE_INLINE void Sub( zpScalar& s, const zpScalar& a, const zpScalar& b );
	ZP_FORCE_INLINE void Mul( zpScalar& s, const zpScalar& a, const zpScalar& b );
	ZP_FORCE_INLINE void Div( zpScalar& s, const zpScalar& a, const zpScalar& b );

	ZP_FORCE_INLINE void Add( zpVector4f& s, const zpVector4f& a, const zpScalar& b );
	ZP_FORCE_INLINE void Sub( zpVector4f& s, const zpVector4f& a, const zpScalar& b );
	ZP_FORCE_INLINE void Mul( zpVector4f& s, const zpVector4f& a, const zpScalar& b );
	ZP_FORCE_INLINE void Div( zpVector4f& s, const zpVector4f& a, const zpScalar& b );

	ZP_FORCE_INLINE void Add( zpVector4f& s, const zpScalar& a, const zpVector4f& b );
	ZP_FORCE_INLINE void Sub( zpVector4f& s, const zpScalar& a, const zpVector4f& b );
	ZP_FORCE_INLINE void Mul( zpVector4f& s, const zpScalar& a, const zpVector4f& b );
	ZP_FORCE_INLINE void Div( zpVector4f& s, const zpScalar& a, const zpVector4f& b );

	ZP_FORCE_INLINE void Add( zpVector4f& s, const zpVector4f& a, const zpVector4f& b );
	ZP_FORCE_INLINE void Sub( zpVector4f& s, const zpVector4f& a, const zpVector4f& b );
	ZP_FORCE_INLINE void Mul( zpVector4f& s, const zpVector4f& a, const zpVector4f& b );
	ZP_FORCE_INLINE void Div( zpVector4f& s, const zpVector4f& a, const zpVector4f& b );

	ZP_FORCE_INLINE void Add( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b );
	ZP_FORCE_INLINE void Sub( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b );
	ZP_FORCE_INLINE void Mul( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b );
	ZP_FORCE_INLINE void Div( zpQuaternion4f& s, const zpQuaternion4f& a, const zpScalar& b );

	ZP_FORCE_INLINE void Add( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b );
	ZP_FORCE_INLINE void Sub( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b );
	ZP_FORCE_INLINE void Mul( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b );
	ZP_FORCE_INLINE void Div( zpQuaternion4f& s, const zpScalar& a, const zpQuaternion4f& b );

	ZP_FORCE_INLINE void Add( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b );
	ZP_FORCE_INLINE void Sub( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b );
	ZP_FORCE_INLINE void Mul( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b );
	ZP_FORCE_INLINE void Div( zpQuaternion4f& s, const zpQuaternion4f& a, const zpVector4f& b );

	ZP_FORCE_INLINE void Add( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b );
	ZP_FORCE_INLINE void Sub( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b );
	ZP_FORCE_INLINE void Mul( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b );
	ZP_FORCE_INLINE void Div( zpQuaternion4f& s, const zpVector4f& a, const zpQuaternion4f& b );

	ZP_FORCE_INLINE void Add( zpQuaternion4f& s, const zpQuaternion4f& a, const zpQuaternion4f& b );
	ZP_FORCE_INLINE void Sub( zpQuaternion4f& s, const zpQuaternion4f& a, const zpQuaternion4f& b );
	ZP_FORCE_INLINE void Mul( zpQuaternion4f& s, const zpQuaternion4f& a, const zpQuaternion4f& b );

	ZP_FORCE_INLINE void Madd( zpVector4f& s, const zpVector4f& a, const zpVector4f& b, const zpScalar& c );
	ZP_FORCE_INLINE void Madd( zpVector4f& s, const zpVector4f& a, const zpScalar& b, const zpVector4f& c );

	ZP_FORCE_INLINE void Dot2( zpScalar& s, const zpVector4f& a, const zpVector4f& b );
	ZP_FORCE_INLINE void Dot3( zpScalar& s, const zpVector4f& a, const zpVector4f& b );
	ZP_FORCE_INLINE void Dot4( zpScalar& s, const zpVector4f& a, const zpVector4f& b );
	ZP_FORCE_INLINE void Dot4( zpScalar& s, const zpQuaternion4f& a, const zpQuaternion4f& b );
	
	ZP_FORCE_INLINE void Cross3( zpVector4f& s, const zpVector4f& a, const zpVector4f& b );
	ZP_FORCE_INLINE void Perpendicular3( zpVector4f& s, const zpVector4f& a );

	ZP_FORCE_INLINE void LengthSquared2( zpScalar& s, const zpVector4f& a );
	ZP_FORCE_INLINE void LengthSquared3( zpScalar& s, const zpVector4f& a );
	ZP_FORCE_INLINE void LengthSquared4( zpScalar& s, const zpVector4f& a );
	ZP_FORCE_INLINE void LengthSquared4( zpScalar& s, const zpQuaternion4f& a );

	ZP_FORCE_INLINE void Length2( zpScalar& s, const zpVector4f& a );
	ZP_FORCE_INLINE void Length3( zpScalar& s, const zpVector4f& a );
	ZP_FORCE_INLINE void Length4( zpScalar& s, const zpVector4f& a );
	ZP_FORCE_INLINE void Length4( zpScalar& s, const zpQuaternion4f& a );

	ZP_FORCE_INLINE void Normalize2( zpVector4f& s, const zpVector4f& a );
	ZP_FORCE_INLINE void Normalize3( zpVector4f& s, const zpVector4f& a );
	ZP_FORCE_INLINE void Normalize4( zpVector4f& s, const zpVector4f& a );
	ZP_FORCE_INLINE void Normalize4( zpQuaternion4f& s, const zpQuaternion4f& a );

	ZP_FORCE_INLINE void Mul( zpVector4f& s, const zpVector4f& a, const zpMatrix4f& b );
	ZP_FORCE_INLINE void Mul( zpMatrix4f& s, const zpMatrix4f& a, const zpMatrix4f& b );
	ZP_FORCE_INLINE void Mul( zpMatrix4f& s, const zpScalar& a, const zpMatrix4f& b );

	ZP_FORCE_INLINE void Abs( zpScalar& s, const zpScalar& a );
	ZP_FORCE_INLINE void Abs( zpVector4f& s, const zpVector4f& a );

	ZP_FORCE_INLINE void Neg( zpScalar& s, const zpScalar& a );
	ZP_FORCE_INLINE void Neg( zpVector4f& s, const zpVector4f& a );

	ZP_FORCE_INLINE void Rcp( zpScalar& s, const zpScalar& a );
	ZP_FORCE_INLINE void Rcp( zpVector4f& s, const zpVector4f& a );

	ZP_FORCE_INLINE void Sqrt( zpScalar& s, const zpScalar& a );
	ZP_FORCE_INLINE void Sqrt( zpVector4f& s, const zpVector4f& a );

	ZP_FORCE_INLINE void Max( zpScalar& s, const zpScalar& a, const zpScalar& b );
	ZP_FORCE_INLINE void Max( zpVector4f& s, const zpVector4f& a, const zpVector4f& b );

	ZP_FORCE_INLINE void Min( zpScalar& s, const zpScalar& a, const zpScalar& b );
	ZP_FORCE_INLINE void Min( zpVector4f& s, const zpVector4f& a, const zpVector4f& b );

	ZP_FORCE_INLINE void Cmp( zp_int& s, const zpScalar& a, const zpScalar& b );
	ZP_FORCE_INLINE void Cmp0( zp_int& s, const zpScalar& a );

	ZP_FORCE_INLINE void Cmp( zp_int& s, const zpVector4f& a, const zpVector4f& b );

	ZP_FORCE_INLINE zp_int Cmp( const zpScalar& a, const zpScalar& b )
	{
		zp_int s;
		Cmp( s, a, b );
		return s;
	}
	ZP_FORCE_INLINE zp_int Cmp0( const zpScalar& a )
	{
		zp_int s;
		Cmp0( s, a );
		return s;
	}

	ZP_FORCE_INLINE zp_int Cmp( const zpVector4f& a, const zpVector4f& b )
	{
		zp_int s;
		Cmp( s, a, b );
		return s;
	}

	ZP_FORCE_INLINE void LookAtLH( zpMatrix4f& s, const zpVector4f& eye, const zpVector4f& direction, const zpVector4f& up );
	ZP_FORCE_INLINE void LookAtRH( zpMatrix4f& s, const zpVector4f& eye, const zpVector4f& direction, const zpVector4f& up );
	ZP_FORCE_INLINE void PerspectiveLH( zpMatrix4f& s, const zpScalar& fovy, const zpScalar& aspectRatio, const zpScalar& zNear, const zpScalar& zFar );
	ZP_FORCE_INLINE void PerspectiveRH( zpMatrix4f& s, const zpScalar& fovy, const zpScalar& aspectRatio, const zpScalar& zNear, const zpScalar& zFar );
	ZP_FORCE_INLINE void OrthoLH( zpMatrix4f& s, const zpScalar& l, const zpScalar& r, const zpScalar& t, const zpScalar& b, const zpScalar& zNear, const zpScalar& zFar );
	ZP_FORCE_INLINE void OrthoRH( zpMatrix4f& s, const zpScalar& l, const zpScalar& r, const zpScalar& t, const zpScalar& b, const zpScalar& zNear, const zpScalar& zFar );

	ZP_FORCE_INLINE void Transpose( zpMatrix4f& s, const zpMatrix4f& a );
	ZP_FORCE_INLINE void Determinant( zpScalar& s, const zpMatrix4f& a );
	ZP_FORCE_INLINE void Invert( zpMatrix4f& s, const zpMatrix4f& a );

	ZP_FORCE_INLINE void Lerp( zpVector4f& s, const zpVector4f& a, const zpVector4f& b, const zpScalar& alpha );
	ZP_FORCE_INLINE void Lerp( zpScalar& s, const zpScalar& a, const zpScalar& b, const zpScalar& alpha );

	ZP_FORCE_INLINE void RotateX( zpVector4f& s, const zpVector4f& a, const zpScalar& rad );
	ZP_FORCE_INLINE void RotateY( zpVector4f& s, const zpVector4f& a, const zpScalar& rad );
	ZP_FORCE_INLINE void RotateZ( zpVector4f& s, const zpVector4f& a, const zpScalar& rad );

	ZP_FORCE_INLINE void SetEulerAngle( zpQuaternion4f& s, const zpScalar& yaw, const zpScalar& pitch, const zpScalar& roll );

	ZP_FORCE_INLINE void GetRoll( zpScalar& s, const zpQuaternion4f& a );
	ZP_FORCE_INLINE void GetPitch( zpScalar& s, const zpQuaternion4f& a );
	ZP_FORCE_INLINE void GetYaw( zpScalar& s, const zpQuaternion4f& a );

	ZP_FORCE_INLINE void Conjugate( zpQuaternion4f& s, const zpQuaternion4f& a );
	ZP_FORCE_INLINE void Transform( zpVector4f& s, const zpQuaternion4f& a, const zpVector4f& b );
	ZP_FORCE_INLINE void AxisAngle( zpQuaternion4f& s, const zpVector4f& a, const zpScalar& b );

	ZP_FORCE_INLINE void ToMatrix( zpMatrix4f& s, const zpQuaternion4f& a );

};

#if ZP_USE_SIMD
#include "zpBaseTypesSimd.inl"
#else
#include "zpBaseTypesFpu.inl"
#endif

#include "zpVector4.h"
#include "zpVector2.h"
#include "zpQuaternion.h"

//#include "zpInteger.h"
#include "zpMatrix4.h"

#include "zpVector4.inl"
#include "zpMatrix4f.inl"
#include "zpQuaternion.inl"

#include "zpPlane.h"
#include "zpFrustum.h"

#include "zpRay.h"
#include "zpBoundingAABB.h"
#include "zpBoundingSphere.h"
#include "zpCollision.h"

#if ZP_USE_SIMD
#include "zpMathSimd.inl"
#include "zpVector4Simd.inl"
//#include "zpIntegerSimd.inl"
#include "zpMatrix4Simd.inl"
#else
#include "zpMathFpu.inl"
#include "zpVector4Fpu.inl"
//#include "zpIntegerFpu.inl"
#include "zpMatrix4Fpu.inl"
#include "zpQuaterionFpu.inl"
#endif

#endif

