#pragma once
#ifndef ZP_REAL_H
#define ZP_REAL_H

ZP_FORCE_INLINE zp_float zp_real_to_float( zp_real a );
ZP_FORCE_INLINE zp_real zp_real_from_float( zp_float a );

ZP_FORCE_INLINE zp_real zp_real_zero();

ZP_FORCE_INLINE zp_real zp_real_add( zp_real a, zp_real b );
ZP_FORCE_INLINE zp_real zp_real_sub( zp_real a, zp_real b );
ZP_FORCE_INLINE zp_real zp_real_mul( zp_real a, zp_real b );
ZP_FORCE_INLINE zp_real zp_real_div( zp_real a, zp_real b );

ZP_FORCE_INLINE zp_real zp_real_neg( zp_real a );
ZP_FORCE_INLINE zp_real zp_real_rcp( zp_real a );
ZP_FORCE_INLINE zp_real zp_real_abs( zp_real a );

ZP_FORCE_INLINE zp_real zp_real_sqrt( zp_real a );
ZP_FORCE_INLINE zp_real zp_real_rsqrt( zp_real a );

ZP_FORCE_INLINE zp_real zp_real_mod( zp_real a, zp_real b );

ZP_FORCE_INLINE zp_real zp_real_min( zp_real a, zp_real b );
ZP_FORCE_INLINE zp_real zp_real_max( zp_real a, zp_real b );

ZP_FORCE_INLINE zp_real zp_real_clamp( zp_real real, zp_real low, zp_real high );
ZP_FORCE_INLINE zp_real zp_real_clamp01( zp_real real );

ZP_FORCE_INLINE zp_real zp_real_sin( zp_real theta );
ZP_FORCE_INLINE zp_real zp_real_cos( zp_real theta );
ZP_FORCE_INLINE zp_real zp_real_tan( zp_real theta );

ZP_FORCE_INLINE zp_real zp_real_asin( zp_real theta );
ZP_FORCE_INLINE zp_real zp_real_acos( zp_real theta );
ZP_FORCE_INLINE zp_real zp_real_atan( zp_real theta );

ZP_FORCE_INLINE zp_real zp_real_csc( zp_real theta );
ZP_FORCE_INLINE zp_real zp_real_sec( zp_real theta );
ZP_FORCE_INLINE zp_real zp_real_cot( zp_real theta );

ZP_FORCE_INLINE zp_bool zp_real_eq( zp_real a, zp_real b );
ZP_FORCE_INLINE zp_bool zp_real_neq( zp_real a, zp_real b );

ZP_FORCE_INLINE zp_bool zp_real_lt( zp_real a, zp_real b );
ZP_FORCE_INLINE zp_bool zp_real_lte( zp_real a, zp_real b );

ZP_FORCE_INLINE zp_bool zp_real_gt( zp_real a, zp_real b );
ZP_FORCE_INLINE zp_bool zp_real_gte( zp_real a, zp_real b );

#if ZP_USE_SIMD
ZP_FORCE_INLINE zp_real operator+( zp_real a, zp_real b );
ZP_FORCE_INLINE zp_real operator-( zp_real a, zp_real b );
ZP_FORCE_INLINE zp_real operator*( zp_real a, zp_real b );
ZP_FORCE_INLINE zp_real operator/( zp_real a, zp_real b );
#endif

#endif