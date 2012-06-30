#pragma once
#ifndef ZP_REAL_H
#define ZP_REAL_H

/* // Old style
class zpReal {
public:
	ZP_FORCE_INLINE zpReal();
	ZP_FORCE_INLINE zpReal( zp_real real );
	ZP_FORCE_INLINE zpReal( const zpReal& real );
	ZP_FORCE_INLINE zpReal( zpReal&& real );
	ZP_FORCE_INLINE ~zpReal();

	ZP_FORCE_INLINE operator zp_real() const;

	ZP_FORCE_INLINE void operator=( const zpReal& real );
	ZP_FORCE_INLINE void operator=( zpReal&& real );

	ZP_FORCE_INLINE zp_float getReal() const;

	ZP_FORCE_INLINE void abs();
	ZP_FORCE_INLINE void abs( const zpReal& real );

	ZP_FORCE_INLINE void mul( const zpReal& real );
	ZP_FORCE_INLINE void div( const zpReal& real );
	ZP_FORCE_INLINE void add( const zpReal& real );
	ZP_FORCE_INLINE void sub( const zpReal& real );

	ZP_FORCE_INLINE const zpReal operator-() const;
	ZP_FORCE_INLINE const zpReal operator*( const zpReal& real ) const;
	ZP_FORCE_INLINE const zpReal operator/( const zpReal& real ) const;
	ZP_FORCE_INLINE const zpReal operator+( const zpReal& real ) const;
	ZP_FORCE_INLINE const zpReal operator-( const zpReal& real ) const;

	ZP_FORCE_INLINE void setMin( const zpReal& a, const zpReal& b );
	ZP_FORCE_INLINE void setMax( const zpReal& a, const zpReal& b );

	ZP_FORCE_INLINE const zpReal sqrt() const;
	ZP_FORCE_INLINE void sqrt( const zpReal& real );
	ZP_FORCE_INLINE const zpReal invsqrt() const;
	ZP_FORCE_INLINE void invsqrt( const zpReal& real );

	ZP_FORCE_INLINE void zero();
	ZP_FORCE_INLINE zp_bool isZero() const;

	ZP_FORCE_INLINE void clamp01();
	ZP_FORCE_INLINE void clamp01( const zpReal& real );

	ZP_FORCE_INLINE void clamp( const zpReal& low, const zpReal& high );
	ZP_FORCE_INLINE void clamp( const zpReal& real, const zpReal& low, const zpReal& high );

	ZP_FORCE_INLINE void sin();
	ZP_FORCE_INLINE void sin( const zpReal& real );
	ZP_FORCE_INLINE void asin();
	ZP_FORCE_INLINE void asin( const zpReal& real );

	ZP_FORCE_INLINE void cos();
	ZP_FORCE_INLINE void cos( const zpReal& real );
	ZP_FORCE_INLINE void acos();
	ZP_FORCE_INLINE void acos( const zpReal& real );

	ZP_FORCE_INLINE void tan();
	ZP_FORCE_INLINE void tan( const zpReal& real );
	ZP_FORCE_INLINE void atan();
	ZP_FORCE_INLINE void atan( const zpReal& real );

private:
	zp_real m_real;
};
*/
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