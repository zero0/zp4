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
ZP_FORCE_INLINE T zp_min( T a, T b )
{
	T s = ( a < b ) ? a : b;
	return s;
}

template<typename T>
ZP_FORCE_INLINE T zp_max( T a, T b )
{
	T s = ( a > b ) ? a : b;
	return s;
}

template<typename T>
ZP_FORCE_INLINE T zp_saturate( T val )
{
	T s = val > (T)1 ? (T)1 : val < (T)0 ? (T)0 : val;
	return s;
}

template<typename T>
ZP_FORCE_INLINE T zp_clamp( T val, T low, T high )
{
	T s = val > high ? high : val < low ? low : val;
	return s;
}

template<typename T>
ZP_FORCE_INLINE T zp_lerp( T low, T high, zp_float alpha )
{
	alpha = zp_saturate( alpha );
	T s = low + (T)( ( high - low ) * alpha );
	return s;
}

template<typename T>
ZP_FORCE_INLINE T zp_abs( T val )
{
	T s = val < (T)0 ? -val : val;
	return s;
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


#if ZP_USE_SIMD
#include "zpBaseTypesSimd.inl"
#else
#include "zpBaseTypesFpu.inl"
#endif

typedef zp_vec4 zpScalar;
typedef zp_vec4 zpVector4f;
typedef zp_vec4 zpQuaternion4f;
typedef union ZP_ALIGN16 zpMatrix4f
{
	zpVector4f r[4];
	struct
	{
		zpVector4f m_m1;
		zpVector4f m_m2;
		zpVector4f m_m3;
		zpVector4f m_m4;
	};
} zpMatrix4f;

#if ZP_USE_SIMD

#if ZP_WIN_32
typedef zpScalar zpScalarParamF; // use for first 3 params
typedef const zpScalar& zpScalarParamG; // 4th param
typedef const zpScalar& zpScalarParamH; // 5th and 6th param
typedef const zpScalar& zpScalarParamC; // all the rest

typedef zpVector4f zpVector4fParamF;
typedef const zpVector4f& zpVector4fParamG;
typedef const zpVector4f& zpVector4fParamH;
typedef const zpVector4f& zpVector4fParamC;

typedef zpQuaternion4f zpQuaternion4fParamF;
typedef const zpQuaternion4f& zpQuaternion4fParamG;
typedef const zpQuaternion4f& zpQuaternion4fParamH;
typedef const zpQuaternion4f& zpQuaternion4fParamC;

typedef const zpMatrix4f& zpMatrix4fParamF;
typedef const zpMatrix4f& zpMatrix4fParamC;
#elif ZP_WIN_64
typedef zpScalar zpScalarParamF; // use for first 3 params
typedef zpScalar zpScalarParamG; // 4th param
typedef zpScalar zpScalarParamH; // 5th and 6th param
typedef const zpScalar& zpScalarParamC; // all the rest

typedef zpVector4f zpVector4fParamF;
typedef zpVector4f zpVector4fParamG;
typedef zpVector4f zpVector4fParamH;
typedef const zpVector4f& zpVector4fParamC;

typedef zpQuaternion4f zpQuaternion4fParamF;
typedef zpQuaternion4f zpQuaternion4fParamG;
typedef zpQuaternion4f zpQuaternion4fParamH;
typedef const zpQuaternion4f& zpQuaternion4fParamC;

typedef zpMatrix4f zpMatrix4fParamF;
typedef const zpMatrix4f& zpMatrix4fParamC;
#endif

#else

typedef const zpScalar& zpScalarParamF;
typedef const zpScalar& zpScalarParamG;
typedef const zpScalar& zpScalarParamH;
typedef const zpScalar& zpScalarParamC;

typedef const zpVector4f& zpVector4fParamF;
typedef const zpVector4f& zpVector4fParamG;
typedef const zpVector4f& zpVector4fParamH;
typedef const zpVector4f& zpVector4fParamC;

typedef const zpQuaternion4f& zpQuaternion4fParamF;
typedef const zpQuaternion4f& zpQuaternion4fParamG;
typedef const zpQuaternion4f& zpQuaternion4fParamH;
typedef const zpQuaternion4f& zpQuaternion4fParamC;

typedef const zpMatrix4f& zpMatrix4fParamF;
typedef const zpMatrix4f& zpMatrix4fParamC;

#endif

#include "zpVector2.h"

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

union zpVector4fCmp
{
	zp_int cmp[4];
	struct
	{
		zp_int cmpX;
		zp_int cmpY;
		zp_int cmpZ;
		zp_int cmpW;
	};

};
namespace zpMath
{
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Scalar( zp_float s );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4( zp_float x, zp_float y, zp_float z, zp_float w );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4( zpScalarParamF x, zpScalarParamF y, zpScalarParamF z, zpScalarParamG w );
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL Quaternion( zp_float x, zp_float y, zp_float z, zp_float w );
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL Quaternion( zpScalarParamF x, zpScalarParamF y, zpScalarParamF z, zpScalarParamG w );
	ZP_FORCE_INLINE zp_float ZP_VECTORCALL AsFloat( zpScalarParamF s );

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetX( zpVector4fParamF s );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetY( zpVector4fParamF s );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetZ( zpVector4fParamF s );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetW( zpVector4fParamF s );

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetX( zpVector4fParamF s, zpScalarParamF x );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetY( zpVector4fParamF s, zpScalarParamF y );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetZ( zpVector4fParamF s, zpScalarParamF z );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetW( zpVector4fParamF s, zpScalarParamF w );

	ZP_FORCE_INLINE void ZP_VECTORCALL Vector4Store4( zpVector4f s, zp_float* xyzw );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Load4( const zp_float* xyzw );

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetX( zpQuaternion4fParamF s );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetY( zpQuaternion4fParamF s );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetZ( zpQuaternion4fParamF s );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetW( zpQuaternion4fParamF s );

	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetX( zpQuaternion4fParamF s, zpScalarParamF x );
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetY( zpQuaternion4fParamF s, zpScalarParamF y );
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetZ( zpQuaternion4fParamF s, zpScalarParamF z );
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetW( zpQuaternion4fParamF s, zpScalarParamF w );

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarSin( zpScalarParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarCos( zpScalarParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarTan( zpScalarParamF a );

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarAdd( zpScalarParamF a, zpScalarParamF b );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarSub( zpScalarParamF a, zpScalarParamF b );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarMul( zpScalarParamF a, zpScalarParamF b );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarDiv( zpScalarParamF a, zpScalarParamF b );

	//ZP_FORCE_INLINE zpVector4f Add( zpVector4f a, zpScalar b );
	//ZP_FORCE_INLINE zpVector4f Sub( zpVector4f a, zpScalar b );
	//ZP_FORCE_INLINE zpVector4f Mul( zpVector4f a, zpScalar b );
	//ZP_FORCE_INLINE zpVector4f Div( zpVector4f a, zpScalar b );

	//ZP_FORCE_INLINE zpVector4f Add( zpScalar a, zpVector4f b );
	//ZP_FORCE_INLINE zpVector4f Sub( zpScalar a, zpVector4f b );
	//ZP_FORCE_INLINE zpVector4f Mul( zpScalar a, zpVector4f b );
	//ZP_FORCE_INLINE zpVector4f Div( zpScalar a, zpVector4f b );

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Add( zpVector4fParamF a, zpVector4fParamF b );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Sub( zpVector4fParamF a, zpVector4fParamF b );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Mul( zpVector4fParamF a, zpVector4fParamF b );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Div( zpVector4fParamF a, zpVector4fParamF b );

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Scale( zpVector4fParamF a, zpScalarParamF b );

	//ZP_FORCE_INLINE zpQuaternion4f Add( zpQuaternion4f a, zpScalar b );
	//ZP_FORCE_INLINE zpQuaternion4f Sub( zpQuaternion4f a, zpScalar b );
	//ZP_FORCE_INLINE zpQuaternion4f Mul( zpQuaternion4f a, zpScalar b );
	//ZP_FORCE_INLINE zpQuaternion4f Div( zpQuaternion4f a, zpScalar b );
	//
	//ZP_FORCE_INLINE zpQuaternion4f Add( zpScalar a, zpQuaternion4f b );
	//ZP_FORCE_INLINE zpQuaternion4f Sub( zpScalar a, zpQuaternion4f b );
	//ZP_FORCE_INLINE zpQuaternion4f Mul( zpScalar a, zpQuaternion4f b );
	//ZP_FORCE_INLINE zpQuaternion4f Div( zpScalar a, zpQuaternion4f b );
	//
	//ZP_FORCE_INLINE zpQuaternion4f Add( zpQuaternion4f a, zpVector4f b );
	//ZP_FORCE_INLINE zpQuaternion4f Sub( zpQuaternion4f a, zpVector4f b );
	//ZP_FORCE_INLINE zpQuaternion4f Mul( zpQuaternion4f a, zpVector4f b );
	//ZP_FORCE_INLINE zpQuaternion4f Div( zpQuaternion4f a, zpVector4f b );
	//
	//ZP_FORCE_INLINE zpQuaternion4f Add( zpVector4f a, zpQuaternion4f b );
	//ZP_FORCE_INLINE zpQuaternion4f Sub( zpVector4f a, zpQuaternion4f b );
	//ZP_FORCE_INLINE zpQuaternion4f Mul( zpVector4f a, zpQuaternion4f b );
	//ZP_FORCE_INLINE zpQuaternion4f Div( zpVector4f a, zpQuaternion4f b );

	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionAdd( zpQuaternion4fParamF a, zpQuaternion4fParamF b );
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSub( zpQuaternion4fParamF a, zpQuaternion4fParamF b );
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionMul( zpQuaternion4fParamF a, zpQuaternion4fParamF b );

	ZP_FORCE_INLINE void ZP_VECTORCALL QuaternionStore4( zpQuaternion4f s, zp_float* xyzw );
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionLoad4( const zp_float* xyzw );

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Dot2( zpVector4fParamF a, zpVector4fParamF b );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Dot3( zpVector4fParamF a, zpVector4fParamF b );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Dot4( zpVector4fParamF a, zpVector4fParamF b );

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Cross3( zpVector4fParamF a, zpVector4fParamF b );

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarAbs( zpScalarParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarNeg( zpScalarParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarRcp( zpScalarParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarSqrt( zpScalarParamF a );

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Abs( zpVector4fParamF a );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Neg( zpVector4fParamF a );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Rcp( zpVector4fParamF a );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Sqrt( zpVector4fParamF a );

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarMax( zpScalarParamF a, zpScalarParamF b );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarMin( zpScalarParamF a, zpScalarParamF b );

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Max( zpVector4fParamF a, zpVector4fParamF b );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Min( zpVector4fParamF a, zpVector4fParamF b );

	ZP_FORCE_INLINE zp_int ZP_VECTORCALL ScalarCmp( zpScalarParamF a, zpScalarParamF b );
	ZP_FORCE_INLINE zpVector4fCmp ZP_VECTORCALL Vector4Cmp( zpVector4fParamF a, zpVector4fParamF b );

	//
	// Common functions
	//
	ZP_FORCE_INLINE zp_int ZP_VECTORCALL ScalarCmp0( zpScalarParamF a );

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarDegToRad( zpScalarParamF s );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarRadToDeg( zpScalarParamF s );
	
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Perpendicular3( zpVector4fParamF a );

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Madd( zpVector4fParamF a, zpVector4fParamF b, zpScalarParamF c );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarMadd( zpScalarParamF a, zpScalarParamF b, zpScalarParamF c );

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4MulAdd( zpVector4fParamF a, zpVector4fParamF b, zpVector4fParamF c );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4ScaleAdd( zpVector4fParamF a, zpScalarParamF b, zpVector4fParamF c );

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4LengthSquared2( zpVector4fParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4LengthSquared3( zpVector4fParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4LengthSquared4( zpVector4fParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionLengthSquared4( zpQuaternion4fParamF a );

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Length2( zpVector4fParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Length3( zpVector4fParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Length4( zpVector4fParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionLength4( zpQuaternion4fParamF a );

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Normalize2( zpVector4fParamF a );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Normalize3( zpVector4fParamF a );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Normalize4( zpVector4fParamF a );
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionNormalize4( zpQuaternion4fParamF a );

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL MatrixTransform( zpMatrix4fParamF a, zpVector4fParamC b );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL MatrixTransformNormal3( zpMatrix4fParamF a, zpVector4fParamC b );
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixScale( zpMatrix4fParamF a, zpScalarParamC b );
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixMul( zpMatrix4fParamF a, zpMatrix4fParamC b );

	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL LookAtLH( zpVector4fParamF eye, zpVector4fParamF direction, zpVector4fParamF up );
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL LookAtRH( zpVector4fParamF eye, zpVector4fParamF direction, zpVector4fParamF up );
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL PerspectiveLH( zpScalarParamF fovy, zpScalarParamF aspectRatio, zpScalarParamF zNear, zpScalarParamG zFar );
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL PerspectiveRH( zpScalarParamF fovy, zpScalarParamF aspectRatio, zpScalarParamF zNear, zpScalarParamG zFar );
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL OrthoLH( zpScalarParamF l, zpScalarParamF r, zpScalarParamF t, zpScalarParamG b, zpScalarParamH zNear, zpScalarParamH zFar );
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL OrthoRH( zpScalarParamF l, zpScalarParamF r, zpScalarParamF t, zpScalarParamG b, zpScalarParamH zNear, zpScalarParamH zFar );

	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL Transpose( zpMatrix4fParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Determinant( zpMatrix4fParamF a );
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL Invert( zpMatrix4fParamF a );

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Lerp( zpVector4fParamF a, zpVector4fParamF b, zpScalarParamF alpha );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Smoothstep( zpVector4fParamF a, zpVector4fParamF b, zpScalarParamF alpha );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Smootherstep( zpVector4fParamF a, zpVector4fParamF b, zpScalarParamF alpha );
	
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarLerp( zpScalarParamF a, zpScalarParamF b, zpScalarParamF alpha );

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4RotateX( zpVector4fParamF a, zpScalarParamF rad );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4RotateY( zpVector4fParamF a, zpScalarParamF rad );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4RotateZ( zpVector4fParamF a, zpScalarParamF rad );

	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionFromEulerAngle( zp_float yaw, zp_float pitch, zp_float roll );
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionFromEulerAngle( zpScalarParamF yaw, zpScalarParamF pitch, zpScalarParamF roll );
	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionFromAxisAngle( zpVector4fParamF axis, zpScalarParamF angle );

	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetYaw( zpQuaternion4fParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetRoll( zpQuaternion4fParamF a );
	ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetPitch( zpQuaternion4fParamF a );

	ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionConjugate( zpQuaternion4fParamF a );
	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL QuaternionTransform( zpQuaternion4fParamF a, zpVector4fParamF b );

	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL QuaternionToMatrix( zpQuaternion4fParamF a );

	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL TR( zpVector4fParamF p, zpQuaternion4fParamF r );
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL TRS( zpVector4fParamF p, zpQuaternion4fParamF r, zpVector4fParamF s );
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixIdentity();

	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixLoadOpenGL( const zp_float* matrix );
	ZP_FORCE_INLINE void ZP_VECTORCALL MatrixStoreOpenGL( zpMatrix4fParamF a, zp_float* matrix );

	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixRotationX( zpScalarParamF rad );
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixRotationY( zpScalarParamF rad );
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixRotationZ( zpScalarParamF rad );
	ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixRotationAxis( zpVector4fParamF axis, zpScalarParamF rad );

	ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Reflect( zpVector4fParamF a, zpVector4fParamF n );
};

#if ZP_USE_SIMD
#include "zpMathSimd.inl"
#else
#include "zpMathFpu.inl"
#endif

#include "zpPlane.h"
#include "zpFrustum.h"

#include "zpRay.h"
#include "zpBoundingAABB.h"
#include "zpBoundingSphere.h"
#include "zpCollision.h"



#include "zpMath.inl"

#endif

