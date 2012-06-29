#pragma once
#ifndef ZP_VECTOR4F_H
#define ZP_VECTOR4F_H

class zpVector4f {
public:
	/*
	ZP_FORCE_INLINE void* operator new( zp_uint size );
	ZP_FORCE_INLINE void* operator new[]( zp_uint size );
	ZP_FORCE_INLINE void operator delete( void* ptr );
	*/
	ZP_FORCE_INLINE zpVector4f();
	ZP_FORCE_INLINE zpVector4f( zp_float x, zp_float y, zp_float z, zp_float w = 0 );
	ZP_FORCE_INLINE zpVector4f( const zpReal& x, const zpReal& y, const zpReal& z, const zpReal& w );
	ZP_FORCE_INLINE zpVector4f( const zpVector4f& vector );
	ZP_FORCE_INLINE zpVector4f( zpVector4f&& vector );
	ZP_FORCE_INLINE explicit zpVector4f( const zp_vec4& vector );
	ZP_FORCE_INLINE ~zpVector4f();

	ZP_FORCE_INLINE void operator=( const zpVector4f& vector );
	ZP_FORCE_INLINE void operator=( zpVector4f&& vector );

	ZP_FORCE_INLINE zpVector4f operator-() const;
	
	ZP_FORCE_INLINE void set3( const zpReal& x, const zpReal& y, const zpReal& z );
	ZP_FORCE_INLINE void set4( const zpReal& x, const zpReal& y, const zpReal& z, const zpReal& w );
	ZP_FORCE_INLINE void set3( const zpVector4f& vector );
	ZP_FORCE_INLINE void set4( const zpVector4f& vector );

	ZP_FORCE_INLINE zpReal getX() const;
	ZP_FORCE_INLINE zpReal getY() const;
	ZP_FORCE_INLINE zpReal getZ() const;
	ZP_FORCE_INLINE zpReal getW() const;

	ZP_FORCE_INLINE void store3( zpReal* xyz ) const;
	ZP_FORCE_INLINE void store4( zpReal* xyzw ) const;
	ZP_FORCE_INLINE void storef3( zp_float* xyz ) const;
	ZP_FORCE_INLINE void storef4( zp_float* xyzw ) const;

	ZP_FORCE_INLINE void load3( const zpReal* xyz );
	ZP_FORCE_INLINE void load4( const zpReal* xyzw );
	ZP_FORCE_INLINE void loadf3( zp_float* xyz );
	ZP_FORCE_INLINE void loadf4( zp_float* xyzw );

	ZP_FORCE_INLINE void mul3( const zpVector4f& vector );
	ZP_FORCE_INLINE void mul4( const zpVector4f& vector );
	ZP_FORCE_INLINE void div3( const zpVector4f& vector );
	ZP_FORCE_INLINE void div4( const zpVector4f& vector );
	ZP_FORCE_INLINE void add3( const zpVector4f& vector );
	ZP_FORCE_INLINE void add4( const zpVector4f& vector );
	ZP_FORCE_INLINE void sub3( const zpVector4f& vector );
	ZP_FORCE_INLINE void sub4( const zpVector4f& vector );
	
	ZP_FORCE_INLINE void mul3( const zpReal& r );
	ZP_FORCE_INLINE void mul4( const zpReal& r );
	ZP_FORCE_INLINE void div3( const zpReal& r );
	ZP_FORCE_INLINE void div4( const zpReal& r );
	ZP_FORCE_INLINE void add3( const zpReal& r );
	ZP_FORCE_INLINE void add4( const zpReal& r );
	ZP_FORCE_INLINE void sub3( const zpReal& r );
	ZP_FORCE_INLINE void sub4( const zpReal& r );
	
	ZP_FORCE_INLINE void zero3();
	ZP_FORCE_INLINE void zero4();
	ZP_FORCE_INLINE zp_bool isZero3() const;
	ZP_FORCE_INLINE zp_bool isZero4() const;

	ZP_FORCE_INLINE zpReal dot3( const zpVector4f& vector ) const;
	ZP_FORCE_INLINE zpReal dot4( const zpVector4f& vector ) const;

	ZP_FORCE_INLINE zpVector4f cross3( const zpVector4f& vector ) const;
	ZP_FORCE_INLINE zpVector4f cross4( const zpVector4f& vector ) const;

	ZP_FORCE_INLINE zpReal magnitude3() const;
	ZP_FORCE_INLINE zpReal magnitude4() const;
	ZP_FORCE_INLINE zpReal magnitudeSquared3() const;
	ZP_FORCE_INLINE zpReal magnitudeSquared4() const;

	ZP_FORCE_INLINE zpVector4f normalize3() const;
	ZP_FORCE_INLINE void normalize3();
	ZP_FORCE_INLINE zpVector4f normalize4() const;
	ZP_FORCE_INLINE void normalize4();

	ZP_FORCE_INLINE zpVector4f homogenize4() const;
	ZP_FORCE_INLINE void homogenize4();

	ZP_FORCE_INLINE zpVector4f lerp3( const zpVector4f& vector, zpReal alpha ) const;
	ZP_FORCE_INLINE void lerp3( const zpVector4f& vector, zpReal alpha );
	ZP_FORCE_INLINE zpVector4f lerp4( const zpVector4f& vector, zpReal alpha ) const;
	ZP_FORCE_INLINE void lerp4( const zpVector4f& vector, zpReal alpha );

	ZP_FORCE_INLINE zpVector4f slerp3( const zpVector4f& vector, zpReal alpha ) const;
	ZP_FORCE_INLINE void slerp3( const zpVector4f& vector, zpReal alpha );
	ZP_FORCE_INLINE zpVector4f slerp4( const zpVector4f& vector, zpReal alpha ) const;
	ZP_FORCE_INLINE void slerp4( const zpVector4f& vector, zpReal alpha );

	ZP_FORCE_INLINE zp_bool operator==( const zpVector4f& vector ) const;
	ZP_FORCE_INLINE zp_bool operator!=( const zpVector4f& vector ) const;

#define SWIZZEL( x, y, z, w ) ZP_FORCE_INLINE zpVector4f x##y##z##w() const { return zpVector4f( m_xyzw.##x, m_xyzw.##y, m_xyzw.##z, m_xyzw.##w ); }
	SWIZZEL( x, x, x, x );

	SWIZZEL( y, y, y, y );

	SWIZZEL( z, z, z, z );

	SWIZZEL( w, w, w, w );
#undef SWIZZEL

private:
	zp_vec4 m_xyzw;
};

#endif