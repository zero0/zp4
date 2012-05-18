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
	ZP_FORCE_INLINE void abs( const zp_real& real );

	ZP_FORCE_INLINE void mul( const zp_real& real );
	ZP_FORCE_INLINE void div( const zp_real& real );
	ZP_FORCE_INLINE void add( const zp_real& real );
	ZP_FORCE_INLINE void sub( const zp_real& real );

	ZP_FORCE_INLINE const zp_real operator-() const;
	ZP_FORCE_INLINE const zp_real operator*( const zp_real& real ) const;
	ZP_FORCE_INLINE const zp_real operator/( const zp_real& real ) const;
	ZP_FORCE_INLINE const zp_real operator+( const zp_real& real ) const;
	ZP_FORCE_INLINE const zp_real operator-( const zp_real& real ) const;

	ZP_FORCE_INLINE void setMin( const zp_real& a, const zp_real& b );
	ZP_FORCE_INLINE void setMax( const zp_real& a, const zp_real& b );

	ZP_FORCE_INLINE const zp_real sqrt() const;
	ZP_FORCE_INLINE const zp_real invsqrt() const;

	ZP_FORCE_INLINE void zero();
	ZP_FORCE_INLINE zp_bool isZero() const;

private:
	zp_real m_real;
};

#endif