#include <math.h>

ZP_FORCE_INLINE void zpScalarAdd( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = a.m_x + b.m_x;
}
ZP_FORCE_INLINE void zpScalarSub( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = a.m_x - b.m_x;
}
ZP_FORCE_INLINE void zpScalarMul( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = a.m_x * b.m_x;
}
ZP_FORCE_INLINE void zpScalarDiv( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = a.m_x / b.m_x;
}

ZP_FORCE_INLINE void zpScalarNeg( zpScalar& s, const zpScalar& a )
{
	s.m_x = -a.m_x;
}
ZP_FORCE_INLINE void zpScalarRcp( zpScalar& s, const zpScalar& a )
{
	s.m_x = 1.0f / a.m_x;
}
ZP_FORCE_INLINE void zpScalarAbs( zpScalar& s, const zpScalar& a )
{
	s.m_x = zp_abs( a.m_x );
}

ZP_FORCE_INLINE void zpScalarSqrt( zpScalar& s, const zpScalar& a )
{
	s.m_x = zp_sqrt( a.m_x );
}
ZP_FORCE_INLINE void zpScalarRsqrt( zpScalar& s, const zpScalar& a )
{
	s.m_x = 1.0f / zp_sqrt( a.m_x );
}
				
ZP_FORCE_INLINE void zpScalarMod( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	
}
				
ZP_FORCE_INLINE void zpScalarMin( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = ZP_MIN( a.m_x, b.m_x );
}
ZP_FORCE_INLINE void zpScalarMax( zpScalar& s, const zpScalar& a, const zpScalar& b )
{
	s.m_x = ZP_MAX( a.m_x, b.m_x );
}
				
ZP_FORCE_INLINE void zpScalarClamp( zpScalar& s, const zpScalar& v, const zpScalar& a, const zpScalar& b )
{
	s.m_x = ZP_MAX( ZP_MIN( v.m_x, b.m_x ), a.m_x );
}
ZP_FORCE_INLINE void zpScalarSaturate( zpScalar& s, const zpScalar& v )
{
	s.m_x = ZP_MAX( ZP_MIN( v.m_x, 1.0f ), 0.0f );
}
				
ZP_FORCE_INLINE void zpScalarSin( zpScalar& s, const zpScalar& theta )
{
	s.m_x = zp_sin( theta.m_x );
}
ZP_FORCE_INLINE void zpScalarCos( zpScalar& s, const zpScalar& theta )
{
	s.m_x = zp_cos( theta.m_x );
}
ZP_FORCE_INLINE void zpScalarTan( zpScalar& s, const zpScalar& theta )
{
	s.m_x = zp_tan( theta.m_x );
}
				
ZP_FORCE_INLINE void zpScalarAsin( zpScalar& s, const zpScalar& theta );
ZP_FORCE_INLINE void zpScalarAcos( zpScalar& s, const zpScalar& theta );
ZP_FORCE_INLINE void zpScalarAtan( zpScalar& s, const zpScalar& theta );

ZP_FORCE_INLINE void zpScalarCsc( zpScalar& s, const zpScalar& theta )
{
	s.m_x = 1.0f / zp_sin( theta.m_x );
}
ZP_FORCE_INLINE void zpScalarSec( zpScalar& s, const zpScalar& theta )
{
	s.m_x = 1.0f / zp_cos( theta.m_x );
}
ZP_FORCE_INLINE void zpScalarCot( zpScalar& s, const zpScalar& theta )
{
	s.m_x = 1.0f / zp_tan( theta.m_x );
}

ZP_FORCE_INLINE zp_bool zpScalarEq( const zpScalar& a, const zpScalar& b )
{
	return a.m_x == b.m_x;
}
ZP_FORCE_INLINE zp_bool zpScalarNeq( const zpScalar& a, const zpScalar& b )
{
	return a.m_x != b.m_x;
}

ZP_FORCE_INLINE zp_bool zpScalarLt( const zpScalar& a, const zpScalar& b )
{
	return a.m_x < b.m_x;
}
ZP_FORCE_INLINE zp_bool zpScalarLte( const zpScalar& a, const zpScalar& b )
{
	return a.m_x <= b.m_x;
}

ZP_FORCE_INLINE zp_bool zpScalarGt( const zpScalar& a, const zpScalar& b )
{
	return a.m_x > b.m_x;
}
ZP_FORCE_INLINE zp_bool zpScalarGte( const zpScalar& a, const zpScalar& b )
{
	return a.m_x >= b.m_x;
}

ZP_FORCE_INLINE zpScalar operator*( const zpScalar& a, const zpScalar& b )
{
	zpScalar r;
	zpScalarMul( r, a, b );
	return r;
}
ZP_FORCE_INLINE zpScalar operator/( const zpScalar& a, const zpScalar& b )
{
	zpScalar r;
	zpScalarDiv( r, a, b );
	return r;
}
ZP_FORCE_INLINE zpScalar operator+( const zpScalar& a, const zpScalar& b )
{
	zpScalar r;
	zpScalarAdd( r, a, b );
	return r;
}
ZP_FORCE_INLINE zpScalar operator-( const zpScalar& a, const zpScalar& b )
{
	zpScalar r;
	zpScalarSub( r, a, b );
	return r;
}
