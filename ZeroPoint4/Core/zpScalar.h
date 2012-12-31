#pragma once
#ifndef ZP_REAL_H
#define ZP_REAL_H

struct zpScalar
{
	zp_float m_x, m_y, m_z, m_w;

	zpScalar()
		: m_x( 0 )
		, m_y( 0 )
		, m_z( 0 )
		, m_w( 0 )
	{}
	explicit zpScalar( const zp_float x )
		: m_x( x )
		, m_y( 0 )
		, m_z( 0 )
		, m_w( 0 )
	{}
	zpScalar( const zpScalar& x )
		: m_x( x.m_x )
		, m_y( 0 )
		, m_z( 0 )
		, m_w( 0 )
	{}
	zpScalar( zpScalar&& x )
		: m_x( x.m_x )
		, m_y( 0 )
		, m_z( 0 )
		, m_w( 0 )
	{}

	zp_float getFloat() const { return m_x; }
};

ZP_FORCE_INLINE void zpScalarAdd( zpScalar& s, const zpScalar& a, const zpScalar& b );
ZP_FORCE_INLINE void zpScalarSub( zpScalar& s, const zpScalar& a, const zpScalar& b );
ZP_FORCE_INLINE void zpScalarMul( zpScalar& s, const zpScalar& a, const zpScalar& b );
ZP_FORCE_INLINE void zpScalarDiv( zpScalar& s, const zpScalar& a, const zpScalar& b );

ZP_FORCE_INLINE void zpScalarNeg( zpScalar& s, const zpScalar& a );
ZP_FORCE_INLINE void zpScalarRcp( zpScalar& s, const zpScalar& a );
ZP_FORCE_INLINE void zpScalarAbs( zpScalar& s, const zpScalar& a );

ZP_FORCE_INLINE void zpScalarSqrt( zpScalar& s, const zpScalar& a );
ZP_FORCE_INLINE void zpScalarRsqrt( zpScalar& s, const zpScalar& a );
				
ZP_FORCE_INLINE void zpScalarMod( zpScalar& s, const zpScalar& a, const zpScalar& b );
				
ZP_FORCE_INLINE void zpScalarMin( zpScalar& s, const zpScalar& a, const zpScalar& b );
ZP_FORCE_INLINE void zpScalarMax( zpScalar& s, const zpScalar& a, const zpScalar& b );
				
ZP_FORCE_INLINE void zpScalarClamp( zpScalar& s, const zpScalar& v, const zpScalar& a, const zpScalar& b );
ZP_FORCE_INLINE void zpScalarSaturate( zpScalar& s, const zpScalar& v );
				
ZP_FORCE_INLINE void zpScalarSin( zpScalar& s, const zpScalar& theta );
ZP_FORCE_INLINE void zpScalarCos( zpScalar& s, const zpScalar& theta );
ZP_FORCE_INLINE void zpScalarTan( zpScalar& s, const zpScalar& theta );
				
ZP_FORCE_INLINE void zpScalarAsin( zpScalar& s, const zpScalar& theta );
ZP_FORCE_INLINE void zpScalarAcos( zpScalar& s, const zpScalar& theta );
ZP_FORCE_INLINE void zpScalarAtan( zpScalar& s, const zpScalar& theta );
				
ZP_FORCE_INLINE void zpScalarCsc( zpScalar& s, const zpScalar& theta );
ZP_FORCE_INLINE void zpScalarSec( zpScalar& s, const zpScalar& theta );
ZP_FORCE_INLINE void zpScalarCot( zpScalar& s, const zpScalar& theta );

ZP_FORCE_INLINE zp_bool zpScalarEq( const zpScalar& a, const zpScalar& b );
ZP_FORCE_INLINE zp_bool zpScalarNeq( const zpScalar& a, const zpScalar& b );

ZP_FORCE_INLINE zp_bool zpScalarLt( const zpScalar& a, const zpScalar& b );
ZP_FORCE_INLINE zp_bool zpScalarLte( const zpScalar& a, const zpScalar& b );

ZP_FORCE_INLINE zp_bool zpScalarGt( const zpScalar& a, const zpScalar& b );
ZP_FORCE_INLINE zp_bool zpScalarGte( const zpScalar& a, const zpScalar& b );

ZP_FORCE_INLINE zpScalar operator*( const zpScalar& a, const zpScalar& b );
ZP_FORCE_INLINE zpScalar operator/( const zpScalar& a, const zpScalar& b );
ZP_FORCE_INLINE zpScalar operator+( const zpScalar& a, const zpScalar& b );
ZP_FORCE_INLINE zpScalar operator-( const zpScalar& a, const zpScalar& b );

#endif