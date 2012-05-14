
#include <malloc.h>

ZP_FORCE_INLINE void* zpVector4::operator new( zp_uint size ) {
	return malloc( size );
};
ZP_FORCE_INLINE void zpVector4::operator delete( void* ptr ) {
	free( ptr );
}

ZP_FORCE_INLINE zpVector4::zpVector4() {
	m_xyzw.x = 0;
	m_xyzw.y = 0;
	m_xyzw.z = 0;
	m_xyzw.w = 0;
}
ZP_FORCE_INLINE zpVector4::zpVector4( zp_float x, zp_float y, zp_float z, zp_float w ) {
	m_xyzw.x = x;
	m_xyzw.y = y;
	m_xyzw.z = z;
	m_xyzw.w = w;
}
ZP_FORCE_INLINE zpVector4::zpVector4( const zpVector4& vector ) : m_xyzw( vector.m_xyzw ) {}
ZP_FORCE_INLINE zpVector4::zpVector4( zpVector4&& vector ) : m_xyzw( vector.m_xyzw ) {}
ZP_FORCE_INLINE zpVector4::zpVector4( const zp_vec4& vector ) : m_xyzw( vector ) {}
ZP_FORCE_INLINE zpVector4::~zpVector4() {}

ZP_FORCE_INLINE void zpVector4::mul( const zpVector4& vector ) {
	m_xyzw.x *= vector.m_xyzw.x;
	m_xyzw.y *= vector.m_xyzw.y;
	m_xyzw.z *= vector.m_xyzw.z;
	m_xyzw.w *= vector.m_xyzw.w;
}
ZP_FORCE_INLINE void zpVector4::div( const zpVector4& vector ) {
	m_xyzw.x /= vector.m_xyzw.x;
	m_xyzw.y /= vector.m_xyzw.y;
	m_xyzw.z /= vector.m_xyzw.z;
	m_xyzw.w /= vector.m_xyzw.w;
}
ZP_FORCE_INLINE void zpVector4::add( const zpVector4& vector ) {
	m_xyzw.x += vector.m_xyzw.x;
	m_xyzw.y += vector.m_xyzw.y;
	m_xyzw.z += vector.m_xyzw.z;
	m_xyzw.w += vector.m_xyzw.w;
}
ZP_FORCE_INLINE void zpVector4::sub( const zpVector4& vector ) {
	m_xyzw.x -= vector.m_xyzw.x;
	m_xyzw.y -= vector.m_xyzw.y;
	m_xyzw.z -= vector.m_xyzw.z;
	m_xyzw.w -= vector.m_xyzw.w;
}

ZP_FORCE_INLINE void zpVector4::mul( const zpReal& r ) {
	m_xyzw.x *= r;
	m_xyzw.y *= r;
	m_xyzw.z *= r;
	m_xyzw.w *= r;
}
ZP_FORCE_INLINE void zpVector4::div( const zpReal& r ) {
	zp_real invR = 1.f / r;
	m_xyzw.x *= invR;
	m_xyzw.y *= invR;
	m_xyzw.z *= invR;
	m_xyzw.w *= invR;
}
ZP_FORCE_INLINE void zpVector4::add( const zpReal& r ) {
	m_xyzw.x += r;
	m_xyzw.y += r;
	m_xyzw.z += r;
	m_xyzw.w += r;
}
ZP_FORCE_INLINE void zpVector4::sub( const zpReal& r ) {
	m_xyzw.x -= r;
	m_xyzw.y -= r;
	m_xyzw.z -= r;
	m_xyzw.w -= r;
}

ZP_FORCE_INLINE void zpVector4::zero() {
	m_xyzw.x = 0;
	m_xyzw.y = 0;
	m_xyzw.z = 0;
	m_xyzw.w = 0;
}
ZP_FORCE_INLINE zp_bool zpVector4::isZero() const {
	return m_xyzw.x == 0 && m_xyzw.y == 0 && m_xyzw.z == 0 && m_xyzw.w == 0;
}

ZP_FORCE_INLINE zp_float zpVector4::dot3( const zpVector4& vector ) const {
	return m_xyzw.x * vector.m_xyzw.x + m_xyzw.y * vector.m_xyzw.y + m_xyzw.z * vector.m_xyzw.z;
}
ZP_FORCE_INLINE zp_float zpVector4::dot4( const zpVector4& vector ) const {
	return m_xyzw.x * vector.m_xyzw.x + m_xyzw.y * vector.m_xyzw.y + m_xyzw.z * vector.m_xyzw.z + m_xyzw.w * vector.m_xyzw.w;
}