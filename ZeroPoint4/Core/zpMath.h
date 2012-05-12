#pragma once
#ifndef ZP_MATH_H
#define ZP_MATH_H

#include <math.h>

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

class zpVector4;
class zpReal;

#include "zpVector4.h"
#include "zpReal.h"
#include "zpMatrix4.h"

#if ZP_USE_SIMD
#include "zpVector4Simd.inl"
#include "zpRealSimd.inl"
#else
#include "zpVector4Fpu.inl"
#include "zpRealFpu.inl"
#endif

#endif