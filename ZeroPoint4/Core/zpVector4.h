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

#define SWIZZEL4( x, y, z, w )	ZP_FORCE_INLINE zpVector4f x##y##z##w() const	{ return zpVector4f( m_xyzw.##x, m_xyzw.##y, m_xyzw.##z, m_xyzw.##w ); }
#define SWIZZEL3( x, y, z )		ZP_FORCE_INLINE zpVector4f x##y##z() const		{ return zpVector4f( m_xyzw.##x, m_xyzw.##y, m_xyzw.##z, m_xyzw.##z ); }
#define SWIZZEL2( x, y )		ZP_FORCE_INLINE zpVector4f x##y##() const		{ return zpVector4f( m_xyzw.##x, m_xyzw.##y, m_xyzw.##y, m_xyzw.##y ); }
#define SWIZZEL1( x )			ZP_FORCE_INLINE zpVector4f x##() const			{ return zpVector4f( m_xyzw.##x, m_xyzw.##x, m_xyzw.##x, m_xyzw.##x ); }
	// swizzel 4 ( abcd )
	//====
	SWIZZEL4( x, x, x, x );
	SWIZZEL4( x, x, x, y );
	SWIZZEL4( x, x, x, z );
	SWIZZEL4( x, x, x, w );

	SWIZZEL4( x, x, y, x );
	SWIZZEL4( x, x, y, y );
	SWIZZEL4( x, x, y, z );
	SWIZZEL4( x, x, y, w );

	SWIZZEL4( x, x, z, x );
	SWIZZEL4( x, x, z, y );
	SWIZZEL4( x, x, z, z );
	SWIZZEL4( x, x, z, w );

	SWIZZEL4( x, x, w, x );
	SWIZZEL4( x, x, w, y );
	SWIZZEL4( x, x, w, z );
	SWIZZEL4( x, x, w, w );

	//----
	SWIZZEL4( x, y, x, x );
	SWIZZEL4( x, y, x, y );
	SWIZZEL4( x, y, x, z );
	SWIZZEL4( x, y, x, w );

	SWIZZEL4( x, y, y, x );
	SWIZZEL4( x, y, y, y );
	SWIZZEL4( x, y, y, z );
	SWIZZEL4( x, y, y, w );

	SWIZZEL4( x, y, z, x );
	SWIZZEL4( x, y, z, y );
	SWIZZEL4( x, y, z, z );
	SWIZZEL4( x, y, z, w );

	SWIZZEL4( x, y, w, x );
	SWIZZEL4( x, y, w, y );
	SWIZZEL4( x, y, w, z );
	SWIZZEL4( x, y, w, w );

	//----
	SWIZZEL4( x, z, x, x );
	SWIZZEL4( x, z, x, y );
	SWIZZEL4( x, z, x, z );
	SWIZZEL4( x, z, x, w );
	
	SWIZZEL4( x, z, y, x );
	SWIZZEL4( x, z, y, y );
	SWIZZEL4( x, z, y, z );
	SWIZZEL4( x, z, y, w );

	SWIZZEL4( x, z, z, x );
	SWIZZEL4( x, z, z, y );
	SWIZZEL4( x, z, z, z );
	SWIZZEL4( x, z, z, w );

	SWIZZEL4( x, z, w, x );
	SWIZZEL4( x, z, w, y );
	SWIZZEL4( x, z, w, z );
	SWIZZEL4( x, z, w, w );

	//----
	SWIZZEL4( x, w, x, x );
	SWIZZEL4( x, w, x, y );
	SWIZZEL4( x, w, x, z );
	SWIZZEL4( x, w, x, w );

	SWIZZEL4( x, w, y, x );
	SWIZZEL4( x, w, y, y );
	SWIZZEL4( x, w, y, z );
	SWIZZEL4( x, w, y, w );

	SWIZZEL4( x, w, z, x );
	SWIZZEL4( x, w, z, y );
	SWIZZEL4( x, w, z, z );
	SWIZZEL4( x, w, z, w );

	SWIZZEL4( x, w, w, x );
	SWIZZEL4( x, w, w, y );
	SWIZZEL4( x, w, w, z );
	SWIZZEL4( x, w, w, w );

	//====
	SWIZZEL4( y, x, x, x );
	SWIZZEL4( y, x, x, y );
	SWIZZEL4( y, x, x, z );
	SWIZZEL4( y, x, x, w );

	SWIZZEL4( y, x, y, x );
	SWIZZEL4( y, x, y, y );
	SWIZZEL4( y, x, y, z );
	SWIZZEL4( y, x, y, w );

	SWIZZEL4( y, x, z, x );
	SWIZZEL4( y, x, z, y );
	SWIZZEL4( y, x, z, z );
	SWIZZEL4( y, x, z, w );

	SWIZZEL4( y, x, w, x );
	SWIZZEL4( y, x, w, y );
	SWIZZEL4( y, x, w, z );
	SWIZZEL4( y, x, w, w );

	//----
	SWIZZEL4( y, y, x, x );
	SWIZZEL4( y, y, x, y );
	SWIZZEL4( y, y, x, z );
	SWIZZEL4( y, y, x, w );

	SWIZZEL4( y, y, y, x );
	SWIZZEL4( y, y, y, y );
	SWIZZEL4( y, y, y, z );
	SWIZZEL4( y, y, y, w );

	SWIZZEL4( y, y, z, x );
	SWIZZEL4( y, y, z, y );
	SWIZZEL4( y, y, z, z );
	SWIZZEL4( y, y, z, w );

	SWIZZEL4( y, y, w, x );
	SWIZZEL4( y, y, w, y );
	SWIZZEL4( y, y, w, z );
	SWIZZEL4( y, y, w, w );

	//----
	SWIZZEL4( y, z, x, x );
	SWIZZEL4( y, z, x, y );
	SWIZZEL4( y, z, x, z );
	SWIZZEL4( y, z, x, w );
	
	SWIZZEL4( y, z, y, x );
	SWIZZEL4( y, z, y, y );
	SWIZZEL4( y, z, y, z );
	SWIZZEL4( y, z, y, w );

	SWIZZEL4( y, z, z, x );
	SWIZZEL4( y, z, z, y );
	SWIZZEL4( y, z, z, z );
	SWIZZEL4( y, z, z, w );

	SWIZZEL4( y, z, w, x );
	SWIZZEL4( y, z, w, y );
	SWIZZEL4( y, z, w, z );
	SWIZZEL4( y, z, w, w );

	//----
	SWIZZEL4( y, w, x, x );
	SWIZZEL4( y, w, x, y );
	SWIZZEL4( y, w, x, z );
	SWIZZEL4( y, w, x, w );

	SWIZZEL4( y, w, y, x );
	SWIZZEL4( y, w, y, y );
	SWIZZEL4( y, w, y, z );
	SWIZZEL4( y, w, y, w );

	SWIZZEL4( y, w, z, x );
	SWIZZEL4( y, w, z, y );
	SWIZZEL4( y, w, z, z );
	SWIZZEL4( y, w, z, w );

	SWIZZEL4( y, w, w, x );
	SWIZZEL4( y, w, w, y );
	SWIZZEL4( y, w, w, z );
	SWIZZEL4( y, w, w, w );

	//====
	SWIZZEL4( z, x, x, x );
	SWIZZEL4( z, x, x, y );
	SWIZZEL4( z, x, x, z );
	SWIZZEL4( z, x, x, w );

	SWIZZEL4( z, x, y, x );
	SWIZZEL4( z, x, y, y );
	SWIZZEL4( z, x, y, z );
	SWIZZEL4( z, x, y, w );

	SWIZZEL4( z, x, z, x );
	SWIZZEL4( z, x, z, y );
	SWIZZEL4( z, x, z, z );
	SWIZZEL4( z, x, z, w );

	SWIZZEL4( z, x, w, x );
	SWIZZEL4( z, x, w, y );
	SWIZZEL4( z, x, w, z );
	SWIZZEL4( z, x, w, w );

	//----
	SWIZZEL4( z, y, x, x );
	SWIZZEL4( z, y, x, y );
	SWIZZEL4( z, y, x, z );
	SWIZZEL4( z, y, x, w );

	SWIZZEL4( z, y, y, x );
	SWIZZEL4( z, y, y, y );
	SWIZZEL4( z, y, y, z );
	SWIZZEL4( z, y, y, w );

	SWIZZEL4( z, y, z, x );
	SWIZZEL4( z, y, z, y );
	SWIZZEL4( z, y, z, z );
	SWIZZEL4( z, y, z, w );

	SWIZZEL4( z, y, w, x );
	SWIZZEL4( z, y, w, y );
	SWIZZEL4( z, y, w, z );
	SWIZZEL4( z, y, w, w );

	//----
	SWIZZEL4( z, z, x, x );
	SWIZZEL4( z, z, x, y );
	SWIZZEL4( z, z, x, z );
	SWIZZEL4( z, z, x, w );
	
	SWIZZEL4( z, z, y, x );
	SWIZZEL4( z, z, y, y );
	SWIZZEL4( z, z, y, z );
	SWIZZEL4( z, z, y, w );

	SWIZZEL4( z, z, z, x );
	SWIZZEL4( z, z, z, y );
	SWIZZEL4( z, z, z, z );
	SWIZZEL4( z, z, z, w );

	SWIZZEL4( z, z, w, x );
	SWIZZEL4( z, z, w, y );
	SWIZZEL4( z, z, w, z );
	SWIZZEL4( z, z, w, w );

	//----
	SWIZZEL4( z, w, x, x );
	SWIZZEL4( z, w, x, y );
	SWIZZEL4( z, w, x, z );
	SWIZZEL4( z, w, x, w );

	SWIZZEL4( z, w, y, x );
	SWIZZEL4( z, w, y, y );
	SWIZZEL4( z, w, y, z );
	SWIZZEL4( z, w, y, w );

	SWIZZEL4( z, w, z, x );
	SWIZZEL4( z, w, z, y );
	SWIZZEL4( z, w, z, z );
	SWIZZEL4( z, w, z, w );

	SWIZZEL4( z, w, w, x );
	SWIZZEL4( z, w, w, y );
	SWIZZEL4( z, w, w, z );
	SWIZZEL4( z, w, w, w );
	
	//====
	SWIZZEL4( w, x, x, x );
	SWIZZEL4( w, x, x, y );
	SWIZZEL4( w, x, x, z );
	SWIZZEL4( w, x, x, w );

	SWIZZEL4( w, x, y, x );
	SWIZZEL4( w, x, y, y );
	SWIZZEL4( w, x, y, z );
	SWIZZEL4( w, x, y, w );

	SWIZZEL4( w, x, z, x );
	SWIZZEL4( w, x, z, y );
	SWIZZEL4( w, x, z, z );
	SWIZZEL4( w, x, z, w );

	SWIZZEL4( w, x, w, x );
	SWIZZEL4( w, x, w, y );
	SWIZZEL4( w, x, w, z );
	SWIZZEL4( w, x, w, w );

	//----
	SWIZZEL4( w, y, x, x );
	SWIZZEL4( w, y, x, y );
	SWIZZEL4( w, y, x, z );
	SWIZZEL4( w, y, x, w );

	SWIZZEL4( w, y, y, x );
	SWIZZEL4( w, y, y, y );
	SWIZZEL4( w, y, y, z );
	SWIZZEL4( w, y, y, w );

	SWIZZEL4( w, y, z, x );
	SWIZZEL4( w, y, z, y );
	SWIZZEL4( w, y, z, z );
	SWIZZEL4( w, y, z, w );

	SWIZZEL4( w, y, w, x );
	SWIZZEL4( w, y, w, y );
	SWIZZEL4( w, y, w, z );
	SWIZZEL4( w, y, w, w );

	//----
	SWIZZEL4( w, z, x, x );
	SWIZZEL4( w, z, x, y );
	SWIZZEL4( w, z, x, z );
	SWIZZEL4( w, z, x, w );
	
	SWIZZEL4( w, z, y, x );
	SWIZZEL4( w, z, y, y );
	SWIZZEL4( w, z, y, z );
	SWIZZEL4( w, z, y, w );

	SWIZZEL4( w, z, z, x );
	SWIZZEL4( w, z, z, y );
	SWIZZEL4( w, z, z, z );
	SWIZZEL4( w, z, z, w );

	SWIZZEL4( w, z, w, x );
	SWIZZEL4( w, z, w, y );
	SWIZZEL4( w, z, w, z );
	SWIZZEL4( w, z, w, w );

	//----
	SWIZZEL4( w, w, x, x );
	SWIZZEL4( w, w, x, y );
	SWIZZEL4( w, w, x, z );
	SWIZZEL4( w, w, x, w );

	SWIZZEL4( w, w, y, x );
	SWIZZEL4( w, w, y, y );
	SWIZZEL4( w, w, y, z );
	SWIZZEL4( w, w, y, w );

	SWIZZEL4( w, w, z, x );
	SWIZZEL4( w, w, z, y );
	SWIZZEL4( w, w, z, z );
	SWIZZEL4( w, w, z, w );

	SWIZZEL4( w, w, w, x );
	SWIZZEL4( w, w, w, y );
	SWIZZEL4( w, w, w, z );
	SWIZZEL4( w, w, w, w );

	// swizzel 3 ( abcc )
	SWIZZEL3( x, x, x );
	SWIZZEL3( x, x, y );
	SWIZZEL3( x, x, z );
	SWIZZEL3( x, x, w );

	SWIZZEL3( x, y, x );
	SWIZZEL3( x, y, y );
	SWIZZEL3( x, y, z );
	SWIZZEL3( x, y, w );

	SWIZZEL3( x, z, x );
	SWIZZEL3( x, z, y );
	SWIZZEL3( x, z, z );
	SWIZZEL3( x, z, w );

	SWIZZEL3( x, w, x );
	SWIZZEL3( x, w, y );
	SWIZZEL3( x, w, z );
	SWIZZEL3( x, w, w );

	SWIZZEL3( y, x, x );
	SWIZZEL3( y, x, y );
	SWIZZEL3( y, x, z );
	SWIZZEL3( y, x, w );

	SWIZZEL3( y, y, x );
	SWIZZEL3( y, y, y );
	SWIZZEL3( y, y, z );
	SWIZZEL3( y, y, w );

	SWIZZEL3( y, z, x );
	SWIZZEL3( y, z, y );
	SWIZZEL3( y, z, z );
	SWIZZEL3( y, z, w );

	SWIZZEL3( y, w, x );
	SWIZZEL3( y, w, y );
	SWIZZEL3( y, w, z );
	SWIZZEL3( y, w, w );

	SWIZZEL3( z, x, x );
	SWIZZEL3( z, x, y );
	SWIZZEL3( z, x, z );
	SWIZZEL3( z, x, w );

	SWIZZEL3( z, y, x );
	SWIZZEL3( z, y, y );
	SWIZZEL3( z, y, z );
	SWIZZEL3( z, y, w );

	SWIZZEL3( z, z, x );
	SWIZZEL3( z, z, y );
	SWIZZEL3( z, z, z );
	SWIZZEL3( z, z, w );

	SWIZZEL3( z, w, x );
	SWIZZEL3( z, w, y );
	SWIZZEL3( z, w, z );
	SWIZZEL3( z, w, w );

	SWIZZEL3( w, x, x );
	SWIZZEL3( w, x, y );
	SWIZZEL3( w, x, z );
	SWIZZEL3( w, x, w );

	SWIZZEL3( w, y, x );
	SWIZZEL3( w, y, y );
	SWIZZEL3( w, y, z );
	SWIZZEL3( w, y, w );

	SWIZZEL3( w, z, x );
	SWIZZEL3( w, z, y );
	SWIZZEL3( w, z, z );
	SWIZZEL3( w, z, w );

	SWIZZEL3( w, w, x );
	SWIZZEL3( w, w, y );
	SWIZZEL3( w, w, z );
	SWIZZEL3( w, w, w );

	// swizzel 2 ( abbb )
	SWIZZEL2( x, x );
	SWIZZEL2( x, y );
	SWIZZEL2( x, z );
	SWIZZEL2( x, w );

	SWIZZEL2( y, x );
	SWIZZEL2( y, y );
	SWIZZEL2( y, z );
	SWIZZEL2( y, w );

	SWIZZEL2( z, x );
	SWIZZEL2( z, y );
	SWIZZEL2( z, z );
	SWIZZEL2( z, w );

	SWIZZEL2( w, x );
	SWIZZEL2( w, y );
	SWIZZEL2( w, z );
	SWIZZEL2( w, w );
	
	// swizzel 1 ( aaaa )
	SWIZZEL1( x );
	SWIZZEL1( y );
	SWIZZEL1( z );
	SWIZZEL1( w );

#undef SWIZZEL4
#undef SWIZZEL3
#undef SWIZZEL2
#undef SWIZZEL1

private:
	zp_vec4 m_xyzw;
};

#endif