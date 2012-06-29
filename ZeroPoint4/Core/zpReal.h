#pragma once
#ifndef ZP_REAL_H
#define ZP_REAL_H

class zpReal {
public:
	ZP_FORCE_INLINE zpReal();
	ZP_FORCE_INLINE zpReal( zp_float real );
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

	ZP_FORCE_INLINE void cos();
	ZP_FORCE_INLINE void cos( const zpReal& real );

	ZP_FORCE_INLINE void tan();
	ZP_FORCE_INLINE void tan( const zpReal& real );

private:
	zp_real m_real;
};

#endif