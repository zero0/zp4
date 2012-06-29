#pragma once
#ifndef ZP_MATH_H
#define ZP_MATH_H

#define ZP_EPSILON	0.0001f
#define ZP_PI		3.1415926f
#define ZP_1OVERPI	( 1.0f / ZP_PI )
#define ZP_2PI		( 2.0f * ZP_PI )
#define ZP_PIOVER2	( 0.5f * ZP_PI )
#define ZP_PIOVER4	( 0.25f* ZP_PI ) 

#define ZP_RAD_TO_DEG( r )	( ( (r) * 180.f ) * ZP_1OVERPI )
#define ZP_DEG_TO_RAD( d )	( ( (d) * ZP_PI ) * ( 1.0f / 180.0f ) )

#define ZP_MIN( a, b )	( ( (a) < (b) ) ? (a) : (b) )
#define ZP_MAX( a, b )	( ( (a) > (b) ) ? (a) : (b) )

template<typename T>
ZP_FORCE_INLINE T zp_clamp01( T& val ) { return val > (T)1 ? (T)1 : val < (T)0 ? (T)0 : val; }

template<typename T>
ZP_FORCE_INLINE T zp_clamp( T& val, T& low, T& high ) { return val > high ? high : val < low ? low : val; }

class zpVector4f;
class zpReal;
class zpInteger;
class zpMatrix4f;

#include "zpVector2.h"
#include "zpVector4.h"
#include "zpReal.h"
//#include "zpInteger.h"
#include "zpMatrix4.h"

#if ZP_USE_SIMD
#include "zpVector4Simd.inl"
#include "zpRealSimd.inl"
//#include "zpIntegerSimd.inl"
#include "zpMatrix4Simd.inl"
#else
#include "zpVector4Fpu.inl"
#include "zpRealFpu.inl"
//#include "zpIntegerFpu.inl"
#include "zpMatrix4Fpu.inl"
#endif

#endif