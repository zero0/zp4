#include <math.h>

ZP_FORCE_INLINE zp_float zp_real_to_float( zp_real a ) {
	return a;
}
ZP_FORCE_INLINE zp_real zp_real_from_float( zp_float a ) {
	return a;
}

ZP_FORCE_INLINE zp_real zp_real_zero() {
	return 0.f;
}

ZP_FORCE_INLINE zp_real zp_real_add( zp_real a, zp_real b ) {
	return a + b;
}
ZP_FORCE_INLINE zp_real zp_real_sub( zp_real a, zp_real b ) {
	return a - b;
}
ZP_FORCE_INLINE zp_real zp_real_mul( zp_real a, zp_real b ) {
	return a * b;
}
ZP_FORCE_INLINE zp_real zp_real_div( zp_real a, zp_real b ) {
	return a / b;
}

ZP_FORCE_INLINE zp_real zp_real_neg( zp_real a ) {
	return -a;
}
ZP_FORCE_INLINE zp_real zp_real_rcp( zp_real a ) {
	return 1.f / a;
}
ZP_FORCE_INLINE zp_real zp_real_abs( zp_real a ) {
	return abs( a );
}

ZP_FORCE_INLINE zp_real zp_real_sqrt( zp_real a ) {
	return sqrtf( a );
}
ZP_FORCE_INLINE zp_real zp_real_rsqrt( zp_real a ) {
	return 1.f / sqrtf( a );
}

ZP_FORCE_INLINE zp_real zp_real_mod( zp_real a, zp_real b ) {
	return fmodf( a, b );
}

ZP_FORCE_INLINE zp_real zp_real_min( zp_real a, zp_real b ) {
	return ZP_MIN( a, b );
}
ZP_FORCE_INLINE zp_real zp_real_max( zp_real a, zp_real b ) {
	return ZP_MAX( a, b );
}

ZP_FORCE_INLINE zp_real zp_real_clamp( zp_real real, zp_real low, zp_real high ) {
	return ZP_MAX( ZP_MIN( real, high ), low );
}
ZP_FORCE_INLINE zp_real zp_real_clamp01( zp_real real ) {
	return ZP_MAX( ZP_MIN( real, 1.f ), 0.f );
}

ZP_FORCE_INLINE zp_real zp_real_sin( zp_real theta ) {
	return sinf( theta );
}
ZP_FORCE_INLINE zp_real zp_real_cos( zp_real theta ) {
	return cosf( theta );
}
ZP_FORCE_INLINE zp_real zp_real_tan( zp_real theta ) {
	return tanf( theta );
}

ZP_FORCE_INLINE zp_real zp_real_asin( zp_real theta ) {
	return asinf( theta );
}
ZP_FORCE_INLINE zp_real zp_real_acos( zp_real theta ) {
	return acosf( theta );
}
ZP_FORCE_INLINE zp_real zp_real_atan( zp_real theta ) {
	return atan( theta );
}

ZP_FORCE_INLINE zp_real zp_real_csc( zp_real theta ) {
	return 1.f / sinf( theta );
}
ZP_FORCE_INLINE zp_real zp_real_sec( zp_real theta ) {
	return 1.f / cosf( theta );
}
ZP_FORCE_INLINE zp_real zp_real_cot( zp_real theta ) {
	return 1.f / tanf( theta );
}

ZP_FORCE_INLINE zp_bool zp_real_eq( zp_real a, zp_real b ) {
	return a == b;
}
ZP_FORCE_INLINE zp_bool zp_real_neq( zp_real a, zp_real b ) {
	return a != b;
}

ZP_FORCE_INLINE zp_bool zp_real_lt( zp_real a, zp_real b ) {
	return a < b;
}
ZP_FORCE_INLINE zp_bool zp_real_lte( zp_real a, zp_real b ) {
	return a <= b;
}

ZP_FORCE_INLINE zp_bool zp_real_gt( zp_real a, zp_real b ) {
	return a > b;
}
ZP_FORCE_INLINE zp_bool zp_real_gte( zp_real a, zp_real b ) {
	return a >= b;
}
