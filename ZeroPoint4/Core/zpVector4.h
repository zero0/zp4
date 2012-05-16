#pragma once
#ifndef ZP_VECTOR4_H
#define ZP_VECTOR4_H

class zpVector4 {
public:
	enum zpVector4Component {
		X = 0,
		Y = 1,
		Z = 2,
		W = 3
	};
	/*
	ZP_FORCE_INLINE void* operator new( zp_uint size );
	ZP_FORCE_INLINE void* operator new[]( zp_uint size );
	ZP_FORCE_INLINE void operator delete( void* ptr );
	*/
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
	*/
	ZP_FORCE_INLINE void mul( const zpVector4& vector );
	ZP_FORCE_INLINE void div( const zpVector4& vector );
	ZP_FORCE_INLINE void add( const zpVector4& vector );
	ZP_FORCE_INLINE void sub( const zpVector4& vector );
	
	ZP_FORCE_INLINE void mul( const zpReal& r );
	ZP_FORCE_INLINE void div( const zpReal& r );
	ZP_FORCE_INLINE void add( const zpReal& r );
	ZP_FORCE_INLINE void sub( const zpReal& r );
	
	ZP_FORCE_INLINE void zero();
	ZP_FORCE_INLINE zp_bool isZero() const;

	ZP_FORCE_INLINE zp_float dot3( const zpVector4& vector ) const;
	ZP_FORCE_INLINE zp_float dot4( const zpVector4& vector ) const;
	
private:
	zp_vec4 m_xyzw;
};

#endif