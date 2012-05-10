#pragma once
#ifndef ZP_VECTOR4_H
#define ZP_VECTOR4_H

class zpVector4 {
public:
	ZP_FORCE_INLINE zpVector4();
	ZP_FORCE_INLINE zpVector4( zp_float x, zp_float y, zp_float z, zp_float w = 0 );
	ZP_FORCE_INLINE zpVector4( const zpVector4& vector );
	ZP_FORCE_INLINE zpVector4( zpVector4&& vector );
	ZP_FORCE_INLINE explicit zpVector4( const zp_vec4& vector );
	ZP_FORCE_INLINE ~zpVector4();
	/*
	operator zp_vec4() const;

	void operator=( const zpVector4& vector );
	void operator=( zpVector4&& vector );

	void set( zp_float x, zp_float y, zp_float z, zp_float w = 0 );
	void set( const zpReal& x, const zpReal& y, const zpReal& z, const zpReal& w );

	void mul( const zpVector4& vector );
	void div( const zpVector4& vector );
	void add( const zpVector4& vector );
	void sub( const zpVector4& vector );

	void mul( const zpReal& r );
	void div( const zpReal& r );
	void add( const zpReal& r );
	void sub( const zpReal& r );

	void zero();
	zp_bool isZero() const;
	*/
private:
	zp_vec4 m_xyzw;
};

#endif