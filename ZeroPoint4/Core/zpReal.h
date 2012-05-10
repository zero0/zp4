#pragma once
#ifndef ZP_REAL_H
#define ZP_REAL_H

class zpReal {
public:
	zpReal();
	zpReal( zp_float real );
	zpReal( const zpReal& real );
	zpReal( zpReal&& real );
	~zpReal();

	void operator=( const zpReal& real );
	void operator=( zpReal&& real );

	zp_float getReal() const;

	void abs();
	void abs( const zp_real& real );

	void mul( const zp_real& real );
	void div( const zp_real& real );
	void add( const zp_real& real );
	void sub( const zp_real& real );

	const zp_real operator-() const;
	const zp_real operator*( const zp_real& real ) const;
	const zp_real operator/( const zp_real& real ) const;
	const zp_real operator+( const zp_real& real ) const;
	const zp_real operator-( const zp_real& real ) const;

	void setMin( const zp_real& a, const zp_real& b );
	void setMax( const zp_real& a, const zp_real& b );

	const zp_real sqrt() const;
	const zp_real invsqrt() const;

	void zero();
	zp_bool isZero() const;

private:
	zp_real m_real;
};

#endif