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
#define ZP_PIOVER180	( ZP_PI / 180.f )
#define ZP_180OVERPI	( 180.f / ZP_PI )

#define ZP_RAD_TO_DEG( r )	( (r) * ZP_180OVERPI )
#define ZP_DEG_TO_RAD( d )	( (d) * ZP_PIOVER180 )

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

zp_float zp_fmod( zp_float x, zp_float y );

zp_bool zp_approximate( zp_float a, zp_float b );

template<typename T>
ZP_FORCE_INLINE void zp_abs( T& a, const T& value )
{
	a = value > (T)0 ? value : -value;
}

#if ZP_USE_SIMD
#include "zpBaseTypesSimd.inl"
#else
#include "zpBaseTypesFpu.inl"
#endif

typedef zp_vec4 zpScalar;

#include "zpVector4.h"
#include "zpVector2.h"
#include "zpQuaternion.h"

//#include "zpInteger.h"
#include "zpMatrix4.h"

//struct zpScalar;
//struct zp_vec4;
//class zpVector4f;
//class zpMatrix4f;
class zpPlane;
class zpFrustum;
//class zpQuaternion4f;

class zpRay;
class zpBoundingAABB;
class zpBoundingSphere;
class zpCollision;

namespace zpMath
{
	ZP_FORCE_INLINE zpScalar Scalar( zp_float s );
	ZP_FORCE_INLINE zpVector4f Vector4( zp_float x, zp_float y, zp_float z, zp_float w );
	ZP_FORCE_INLINE zpVector4f Vector4( zpScalar x, zpScalar y, zpScalar z, zpScalar w );
	ZP_FORCE_INLINE zpQuaternion4f Quaternion4( zp_float x, zp_float y, zp_float z, zp_float w );
	ZP_FORCE_INLINE zp_float AsFloat( zpScalar s );

	ZP_FORCE_INLINE zpScalar GetX( zpVector4f s );
	ZP_FORCE_INLINE zpScalar GetY( zpVector4f s );
	ZP_FORCE_INLINE zpScalar GetZ( zpVector4f s );
	ZP_FORCE_INLINE zpScalar GetW( zpVector4f s );

	ZP_FORCE_INLINE zpVector4f SetX( zpVector4f s, zpScalar x );
	ZP_FORCE_INLINE zpVector4f SetY( zpVector4f s, zpScalar y );
	ZP_FORCE_INLINE zpVector4f SetZ( zpVector4f s, zpScalar z );
	ZP_FORCE_INLINE zpVector4f SetW( zpVector4f s, zpScalar w );

	ZP_FORCE_INLINE void Store4( zpVector4f s, zp_float* xyzw );
	ZP_FORCE_INLINE zpVector4f Load4( const zp_float* xyzw );

	ZP_FORCE_INLINE zpScalar GetX( zpQuaternion4f s );
	ZP_FORCE_INLINE zpScalar GetY( zpQuaternion4f s );
	ZP_FORCE_INLINE zpScalar GetZ( zpQuaternion4f s );
	ZP_FORCE_INLINE zpScalar GetW( zpQuaternion4f s );

	ZP_FORCE_INLINE zpQuaternion4f SetX( zpQuaternion4f s, zpScalar x );
	ZP_FORCE_INLINE zpQuaternion4f SetY( zpQuaternion4f s, zpScalar y );
	ZP_FORCE_INLINE zpQuaternion4f SetZ( zpQuaternion4f s, zpScalar z );
	ZP_FORCE_INLINE zpQuaternion4f SetW( zpQuaternion4f s, zpScalar w );

	ZP_FORCE_INLINE zpScalar Sin( zpScalar a );
	ZP_FORCE_INLINE zpScalar Cos( zpScalar a );
	ZP_FORCE_INLINE zpScalar Tan( zpScalar a );

	ZP_FORCE_INLINE zpScalar Add( zpScalar a, zpScalar b );
	ZP_FORCE_INLINE zpScalar Sub( zpScalar a, zpScalar b );
	ZP_FORCE_INLINE zpScalar Mul( zpScalar a, zpScalar b );
	ZP_FORCE_INLINE zpScalar Div( zpScalar a, zpScalar b );

	ZP_FORCE_INLINE zpVector4f Add( zpVector4f a, zpScalar b );
	ZP_FORCE_INLINE zpVector4f Sub( zpVector4f a, zpScalar b );
	ZP_FORCE_INLINE zpVector4f Mul( zpVector4f a, zpScalar b );
	ZP_FORCE_INLINE zpVector4f Div( zpVector4f a, zpScalar b );

	ZP_FORCE_INLINE zpVector4f Add( zpScalar a, zpVector4f b );
	ZP_FORCE_INLINE zpVector4f Sub( zpScalar a, zpVector4f b );
	ZP_FORCE_INLINE zpVector4f Mul( zpScalar a, zpVector4f b );
	ZP_FORCE_INLINE zpVector4f Div( zpScalar a, zpVector4f b );

	ZP_FORCE_INLINE zpVector4f Add( zpVector4f a, zpVector4f b );
	ZP_FORCE_INLINE zpVector4f Sub( zpVector4f a, zpVector4f b );
	ZP_FORCE_INLINE zpVector4f Mul( zpVector4f a, zpVector4f b );
	ZP_FORCE_INLINE zpVector4f Div( zpVector4f a, zpVector4f b );

	ZP_FORCE_INLINE zpQuaternion4f Add( zpQuaternion4f a, zpScalar b );
	ZP_FORCE_INLINE zpQuaternion4f Sub( zpQuaternion4f a, zpScalar b );
	ZP_FORCE_INLINE zpQuaternion4f Mul( zpQuaternion4f a, zpScalar b );
	ZP_FORCE_INLINE zpQuaternion4f Div( zpQuaternion4f a, zpScalar b );

	ZP_FORCE_INLINE zpQuaternion4f Add( zpScalar a, zpQuaternion4f b );
	ZP_FORCE_INLINE zpQuaternion4f Sub( zpScalar a, zpQuaternion4f b );
	ZP_FORCE_INLINE zpQuaternion4f Mul( zpScalar a, zpQuaternion4f b );
	ZP_FORCE_INLINE zpQuaternion4f Div( zpScalar a, zpQuaternion4f b );

	ZP_FORCE_INLINE zpQuaternion4f Add( zpQuaternion4f a, zpVector4f b );
	ZP_FORCE_INLINE zpQuaternion4f Sub( zpQuaternion4f a, zpVector4f b );
	ZP_FORCE_INLINE zpQuaternion4f Mul( zpQuaternion4f a, zpVector4f b );
	ZP_FORCE_INLINE zpQuaternion4f Div( zpQuaternion4f a, zpVector4f b );

	ZP_FORCE_INLINE zpQuaternion4f Add( zpVector4f a, zpQuaternion4f b );
	ZP_FORCE_INLINE zpQuaternion4f Sub( zpVector4f a, zpQuaternion4f b );
	ZP_FORCE_INLINE zpQuaternion4f Mul( zpVector4f a, zpQuaternion4f b );
	ZP_FORCE_INLINE zpQuaternion4f Div( zpVector4f a, zpQuaternion4f b );

	ZP_FORCE_INLINE zpQuaternion4f Add( zpQuaternion4f a, zpQuaternion4f b );
	ZP_FORCE_INLINE zpQuaternion4f Sub( zpQuaternion4f a, zpQuaternion4f b );
	ZP_FORCE_INLINE zpQuaternion4f Mul( zpQuaternion4f a, zpQuaternion4f b );

	ZP_FORCE_INLINE zpScalar Dot2( zpVector4f a, zpVector4f b );
	ZP_FORCE_INLINE zpScalar Dot3( zpVector4f a, zpVector4f b );
	ZP_FORCE_INLINE zpScalar Dot4( zpVector4f a, zpVector4f b );

	ZP_FORCE_INLINE zpVector4f Cross3( zpVector4f a, zpVector4f b );

	ZP_FORCE_INLINE zpScalar Abs( zpScalar a );
	ZP_FORCE_INLINE zpScalar Neg( zpScalar a );
	ZP_FORCE_INLINE zpScalar Rcp( zpScalar a );
	ZP_FORCE_INLINE zpScalar Sqrt( zpScalar a );

	ZP_FORCE_INLINE zpVector4f Abs( zpVector4f a );
	ZP_FORCE_INLINE zpVector4f Neg( zpVector4f a );
	ZP_FORCE_INLINE zpVector4f Rcp( zpVector4f a );
	ZP_FORCE_INLINE zpVector4f Sqrt( zpVector4f a );

	ZP_FORCE_INLINE zpScalar Max( zpScalar a, zpScalar b );
	ZP_FORCE_INLINE zpScalar Min( zpScalar a, zpScalar b );

	ZP_FORCE_INLINE zpVector4f Max( zpVector4f a, zpVector4f b );
	ZP_FORCE_INLINE zpVector4f Min( zpVector4f a, zpVector4f b );

	ZP_FORCE_INLINE zp_int Cmp( zpScalar a, zpScalar b );

	//
	// Common functions
	//
	ZP_FORCE_INLINE zp_int Cmp0( zpScalar a );

	ZP_FORCE_INLINE zpScalar DegToRad( zpScalar s );
	ZP_FORCE_INLINE zpScalar RadToDeg( zpScalar s );
	
	ZP_FORCE_INLINE zpVector4f Perpendicular3( zpVector4f a );

	ZP_FORCE_INLINE zpVector4f Madd( zpVector4f a, zpVector4f b, zpScalar c );
	ZP_FORCE_INLINE zpVector4f Madd( zpVector4f a, zpScalar b, zpVector4f c );

	ZP_FORCE_INLINE zpScalar LengthSquared2( zpVector4f a );
	ZP_FORCE_INLINE zpScalar LengthSquared3( zpVector4f a );
	ZP_FORCE_INLINE zpScalar LengthSquared4( zpVector4f a );
	ZP_FORCE_INLINE zpScalar LengthSquared4( zpQuaternion4f a );

	ZP_FORCE_INLINE zpScalar Length2( zpVector4f a );
	ZP_FORCE_INLINE zpScalar Length3( zpVector4f a );
	ZP_FORCE_INLINE zpScalar Length4( zpVector4f a );
	ZP_FORCE_INLINE zpScalar Length4( zpQuaternion4f a );

	ZP_FORCE_INLINE zpVector4f Normalize2( zpVector4f a );
	ZP_FORCE_INLINE zpVector4f Normalize3( zpVector4f a );
	ZP_FORCE_INLINE zpVector4f Normalize4( zpVector4f a );
	ZP_FORCE_INLINE zpQuaternion4f Normalize4( zpQuaternion4f a );

	ZP_FORCE_INLINE zpVector4f Mul( zpVector4f a, zpMatrix4f b );
	ZP_FORCE_INLINE zpMatrix4f Mul( zpScalar a, zpMatrix4f b );
	ZP_FORCE_INLINE zpMatrix4f Mul( zpMatrix4f a, zpMatrix4f b );

	ZP_FORCE_INLINE zpMatrix4f LookAtLH( zpVector4f eye, zpVector4f direction, zpVector4f up );
	ZP_FORCE_INLINE zpMatrix4f LookAtRH( zpVector4f eye, zpVector4f direction, zpVector4f up );
	ZP_FORCE_INLINE zpMatrix4f PerspectiveLH( zpScalar fovy, zpScalar aspectRatio, zpScalar zNear, zpScalar zFar );
	ZP_FORCE_INLINE zpMatrix4f PerspectiveRH( zpScalar fovy, zpScalar aspectRatio, zpScalar zNear, zpScalar zFar );
	ZP_FORCE_INLINE zpMatrix4f OrthoLH( zpScalar l, zpScalar r, zpScalar t, zpScalar b, zpScalar zNear, zpScalar zFar );
	ZP_FORCE_INLINE zpMatrix4f OrthoRH( zpScalar l, zpScalar r, zpScalar t, zpScalar b, zpScalar zNear, zpScalar zFar );

	ZP_FORCE_INLINE zpMatrix4f Transpose( zpMatrix4f a );
	ZP_FORCE_INLINE zpMatrix4f Determinant( zpMatrix4f a );
	ZP_FORCE_INLINE zpMatrix4f Invert( zpMatrix4f a );

	ZP_FORCE_INLINE zpVector4f Lerp( zpVector4f a, zpVector4f b, zpScalar alpha );
	ZP_FORCE_INLINE zpScalar Lerp( zpScalar a, zpScalar b, zpScalar alpha );

	ZP_FORCE_INLINE zpVector4f RotateX( zpVector4f a, zpScalar rad );
	ZP_FORCE_INLINE zpVector4f RotateY( zpVector4f a, zpScalar rad );
	ZP_FORCE_INLINE zpVector4f RotateZ( zpVector4f a, zpScalar rad );

	ZP_FORCE_INLINE zpQuaternion4f FromEulerAngle( zpScalar yaw, zpScalar pitch, zpScalar roll );
	ZP_FORCE_INLINE zpQuaternion4f FromAxisAngle( zpVector4f axis, zpScalar angle );

	ZP_FORCE_INLINE zpScalar GetRoll( zpQuaternion4f a );
	ZP_FORCE_INLINE zpScalar GetPitch( zpQuaternion4f a );
	ZP_FORCE_INLINE zpScalar GetYaw( zpQuaternion4f a );

	ZP_FORCE_INLINE zpQuaternion4f Conjugate( zpQuaternion4f a );
	ZP_FORCE_INLINE zpVector4f Transform( zpQuaternion4f a, zpVector4f b );

	ZP_FORCE_INLINE zpMatrix4f ToMatrix( zpQuaternion4f a );

	ZP_FORCE_INLINE zpMatrix4f TRS( zpVector4f p, zpQuaternion4f r, zpVector4f s );
	ZP_FORCE_INLINE zpMatrix4f MatrixIdentity();

	ZP_FORCE_INLINE zpVector4f Reflect( zpVector4f a, zpVector4f n );

};

#if ZP_USE_SIMD
#include "zpMathSimd.inl"
#include "zpVector4Simd.inl"
//#include "zpIntegerSimd.inl"
#include "zpMatrix4Simd.inl"
#include "zpQuaternion4Simd.inl"
#else
#include "zpMathFpu.inl"
#include "zpVector4Fpu.inl"
//#include "zpIntegerFpu.inl"
#include "zpMatrix4Fpu.inl"
#include "zpQuaterionFpu.inl"
#endif

//#include "zpVector4.inl"
//#include "zpMatrix4f.inl"
//#include "zpQuaternion.inl"

#include "zpPlane.h"
#include "zpFrustum.h"

#include "zpRay.h"
#include "zpBoundingAABB.h"
#include "zpBoundingSphere.h"
#include "zpCollision.h"



#include "zpMath.inl"

#endif

