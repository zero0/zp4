#pragma once
#ifndef ZP_INTEGER_H
#define ZP_INTEGER_H

class zpInteger {
public:
	zpInteger();
	zpInteger( zp_int integer );
	zpInteger( const zpInteger& integer );
	zpInteger( zpInteger&& integer );
	~zpInteger();

	operator zp_integer() const;

	void operator=( const zpInteger& integer );
	void operator=( zpInteger&& integer );

	zp_int getInteger() const;

	void abs();
	void abs( const zp_integer& integer );

	void mul( const zp_integer& integer );
	void div( const zp_integer& integer );
	void add( const zp_integer& integer );
	void sub( const zp_integer& integer );

	const zp_integer operator-() const;
	const zp_integer operator*( const zp_integer& integer ) const;
	const zp_integer operator/( const zp_integer& integer ) const;
	const zp_integer operator+( const zp_integer& integer ) const;
	const zp_integer operator-( const zp_integer& integer ) const;

	void setMin( const zp_integer& a, const zp_integer& b );
	void setMax( const zp_integer& a, const zp_integer& b );

	const zp_integer sqrt() const;
	const zp_integer invsqrt() const;

	void zero();
	zp_bool isZero() const;

private:
	zp_integer m_integer;
};

#endif