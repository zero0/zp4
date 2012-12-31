#pragma once
#ifndef ZP_MATH_H
#define ZP_MATH_H

#define ZP_EPSILON	1e-8f
#define ZP_PI		3.14159265359f
#define ZP_1OVERPI	( 1.0f / ZP_PI )
#define ZP_2PI		( 2.0f * ZP_PI )
#define ZP_PIOVER2	( 0.5f * ZP_PI )
#define ZP_PIOVER4	( 0.25f* ZP_PI ) 

#define ZP_RAD_TO_DEG( r )	( ( (r) * 180.f ) * ZP_1OVERPI )
#define ZP_DEG_TO_RAD( d )	( ( (d) * ZP_PI ) / 180.0f )

#define ZP_MIN( a, b )	( ( (a) < (b) ) ? (a) : (b) )
#define ZP_MAX( a, b )	( ( (a) > (b) ) ? (a) : (b) )

template<typename T>
ZP_FORCE_INLINE T zp_saturate( T& val ) {
	return val > (T)1 ? (T)1 : val < (T)0 ? (T)0 : val;
}

template<typename T>
ZP_FORCE_INLINE T zp_clamp( T& val, T& low, T& high ) {
	return val > high ? high : val < low ? low : val;
}

zp_float zp_cos( zp_float v );
zp_float zp_sin( zp_float v );
zp_float zp_tan( zp_float v );

zp_float zp_pow( zp_float b, zp_float p );

zp_float zp_sqrt( zp_float v );

template<typename T>
ZP_FORCE_INLINE T zp_abs( T& value ) {
	return value > (T)0 ? value : -value;
}

struct zpScalar;
struct zp_vec4;
class zpVector4f;
class zpInteger;
class zpMatrix4f;
class zpPlane;
class zpFrustum;

class zpRay;
class zpBoundingAABB;
class zpBoundingSphere;
class zpCollision;

#include "zpScalar.h"

#if ZP_USE_SIMD
#include "zpBaseTypesSimd.inl"
#else
#include "zpBaseTypesFpu.inl"
#endif

#include "zpVector2.h"
#include "zpVector4.h"

//#include "zpInteger.h"
#include "zpMatrix4.h"

#include "zpVector4.inl"
#include "zpMatrix4f.inl"
#include "zpPlane.h"
#include "zpFrustum.h"

#include "zpRay.h"
#include "zpBoundingAABB.h"
#include "zpBoundingSphere.h"
#include "zpCollision.h"

#if ZP_USE_SIMD
#include "zpRealSimd.inl"
#include "zpVector4Simd.inl"
//#include "zpIntegerSimd.inl"
#include "zpMatrix4Simd.inl"
#else
#include "zpScalarFpu.inl"
#include "zpVector4Fpu.inl"
//#include "zpIntegerFpu.inl"
#include "zpMatrix4Fpu.inl"
#endif

#endif