
/*#include <malloc.h>

ZP_FORCE_INLINE void* zpVector4f::operator new( zp_uint size ) {
	return malloc( size );
};
ZP_FORCE_INLINE void zpVector4f::operator delete( void* ptr ) {
	free( ptr );
}
*/

ZP_FORCE_INLINE zpVector4f zpVector4f::operator-() const {
	return zpVector4f( -m_xyzw.x, -m_xyzw.y, -m_xyzw.z, m_xyzw.w );
}

ZP_FORCE_INLINE void zpVector4f::set3( const zp_real& x, const zp_real& y, const zp_real& z ) {
	m_xyzw.x = x;
	m_xyzw.y = y;
	m_xyzw.z = y;
}
ZP_FORCE_INLINE void zpVector4f::set4( const zp_real& x, const zp_real& y, const zp_real& z, const zp_real& w ) {
	m_xyzw.x = x;
	m_xyzw.y = y;
	m_xyzw.z = z;
	m_xyzw.w = w;
}
ZP_FORCE_INLINE void zpVector4f::set3( const zpVector4f& vector ) {
	m_xyzw.x = vector.m_xyzw.x;
	m_xyzw.y = vector.m_xyzw.y;
	m_xyzw.z = vector.m_xyzw.z;
}
ZP_FORCE_INLINE void zpVector4f::set4( const zpVector4f& vector ) {
	m_xyzw.x = vector.m_xyzw.x;
	m_xyzw.y = vector.m_xyzw.y;
	m_xyzw.z = vector.m_xyzw.z;
	m_xyzw.w = vector.m_xyzw.w;
}

ZP_FORCE_INLINE void zpVector4f::setX( const zp_real& x ) {
	m_xyzw.x = x;
}
ZP_FORCE_INLINE void zpVector4f::setY( const zp_real& y ) {
	m_xyzw.y = y;
}
ZP_FORCE_INLINE void zpVector4f::setZ( const zp_real& z ) {
	m_xyzw.z = z;
}
ZP_FORCE_INLINE void zpVector4f::setW( const zp_real& w ) {
	m_xyzw.w = w;
}

ZP_FORCE_INLINE void zpVector4f::store3( zp_float* xyz ) const {
	xyz[0] = m_xyzw.x;
	xyz[1] = m_xyzw.y;
	xyz[2] = m_xyzw.z;
}
ZP_FORCE_INLINE void zpVector4f::store4( zp_float* xyzw ) const {
	xyzw[0] = m_xyzw.x;
	xyzw[1] = m_xyzw.y;
	xyzw[2] = m_xyzw.z;
	xyzw[3] = m_xyzw.w;
}

ZP_FORCE_INLINE void zpVector4f::load3( const zp_float* xyz ) {
	m_xyzw.x = xyz[0];
	m_xyzw.y = xyz[1];
	m_xyzw.z = xyz[2];
}
ZP_FORCE_INLINE void zpVector4f::load4( const zp_float* xyzw ) {
	m_xyzw.x = xyzw[0];
	m_xyzw.y = xyzw[1];
	m_xyzw.z = xyzw[2];
	m_xyzw.w = xyzw[3];
}

ZP_FORCE_INLINE void zpVector4f::mul3( const zpVector4f& vector ) {
	m_xyzw.x *= vector.m_xyzw.x;
	m_xyzw.y *= vector.m_xyzw.y;
	m_xyzw.z *= vector.m_xyzw.z;
}
ZP_FORCE_INLINE void zpVector4f::mul4( const zpVector4f& vector ) {
	m_xyzw.x *= vector.m_xyzw.x;
	m_xyzw.y *= vector.m_xyzw.y;
	m_xyzw.z *= vector.m_xyzw.z;
	m_xyzw.w *= vector.m_xyzw.w;
}
ZP_FORCE_INLINE void zpVector4f::div3( const zpVector4f& vector ) {
	m_xyzw.x /= vector.m_xyzw.x;
	m_xyzw.y /= vector.m_xyzw.y;
	m_xyzw.z /= vector.m_xyzw.z;
}
ZP_FORCE_INLINE void zpVector4f::div4( const zpVector4f& vector ) {
	m_xyzw.x /= vector.m_xyzw.x;
	m_xyzw.y /= vector.m_xyzw.y;
	m_xyzw.z /= vector.m_xyzw.z;
	m_xyzw.w /= vector.m_xyzw.w;
}
ZP_FORCE_INLINE void zpVector4f::add3( const zpVector4f& vector ) {
	m_xyzw.x += vector.m_xyzw.x;
	m_xyzw.y += vector.m_xyzw.y;
	m_xyzw.z += vector.m_xyzw.z;
}
ZP_FORCE_INLINE void zpVector4f::add4( const zpVector4f& vector ) {
	m_xyzw.x += vector.m_xyzw.x;
	m_xyzw.y += vector.m_xyzw.y;
	m_xyzw.z += vector.m_xyzw.z;
	m_xyzw.w += vector.m_xyzw.w;
}
ZP_FORCE_INLINE void zpVector4f::sub3( const zpVector4f& vector ) {
	m_xyzw.x -= vector.m_xyzw.x;
	m_xyzw.y -= vector.m_xyzw.y;
	m_xyzw.z -= vector.m_xyzw.z;
}
ZP_FORCE_INLINE void zpVector4f::sub4( const zpVector4f& vector ) {
	m_xyzw.x -= vector.m_xyzw.x;
	m_xyzw.y -= vector.m_xyzw.y;
	m_xyzw.z -= vector.m_xyzw.z;
	m_xyzw.w -= vector.m_xyzw.w;
}

ZP_FORCE_INLINE void zpVector4f::mul3( const zp_real& r ) {
	m_xyzw.x *= r;
	m_xyzw.y *= r;
	m_xyzw.z *= r;
}
ZP_FORCE_INLINE void zpVector4f::mul4( const zp_real& r ) {
	m_xyzw.x *= r;
	m_xyzw.y *= r;
	m_xyzw.z *= r;
	m_xyzw.w *= r;
}
ZP_FORCE_INLINE void zpVector4f::div3( const zp_real& r ) {
	zp_real invR = 1.f / r;
	m_xyzw.x *= invR;
	m_xyzw.y *= invR;
	m_xyzw.z *= invR;
}
ZP_FORCE_INLINE void zpVector4f::div4( const zp_real& r ) {
	zp_real invR = 1.f / r;
	m_xyzw.x *= invR;
	m_xyzw.y *= invR;
	m_xyzw.z *= invR;
	m_xyzw.w *= invR;
}
ZP_FORCE_INLINE void zpVector4f::add3( const zp_real& r ) {
	m_xyzw.x += r;
	m_xyzw.y += r;
	m_xyzw.z += r;
}
ZP_FORCE_INLINE void zpVector4f::add4( const zp_real& r ) {
	m_xyzw.x += r;
	m_xyzw.y += r;
	m_xyzw.z += r;
	m_xyzw.w += r;
}
ZP_FORCE_INLINE void zpVector4f::sub3( const zp_real& r ) {
	m_xyzw.x -= r;
	m_xyzw.y -= r;
	m_xyzw.z -= r;
}
ZP_FORCE_INLINE void zpVector4f::sub4( const zp_real& r ) {
	m_xyzw.x -= r;
	m_xyzw.y -= r;
	m_xyzw.z -= r;
	m_xyzw.w -= r;
}

ZP_FORCE_INLINE void zpVector4f::zero3() {
	m_xyzw.x = 0;
	m_xyzw.y = 0;
	m_xyzw.z = 0;
}
ZP_FORCE_INLINE void zpVector4f::zero4() {
	m_xyzw.x = 0;
	m_xyzw.y = 0;
	m_xyzw.z = 0;
	m_xyzw.w = 0;
}
ZP_FORCE_INLINE zp_bool zpVector4f::isZero3() const {
	return m_xyzw.x == 0 && m_xyzw.y == 0 && m_xyzw.z == 0;
}
ZP_FORCE_INLINE zp_bool zpVector4f::isZero4() const {
	return m_xyzw.x == 0 && m_xyzw.y == 0 && m_xyzw.z == 0 && m_xyzw.w == 0;
}

ZP_FORCE_INLINE zp_real zpVector4f::dot3( const zpVector4f& vector ) const {
	return m_xyzw.x * vector.m_xyzw.x + m_xyzw.y * vector.m_xyzw.y + m_xyzw.z * vector.m_xyzw.z;
}
ZP_FORCE_INLINE zp_real zpVector4f::dot4( const zpVector4f& vector ) const {
	return m_xyzw.x * vector.m_xyzw.x + m_xyzw.y * vector.m_xyzw.y + m_xyzw.z * vector.m_xyzw.z + m_xyzw.w * vector.m_xyzw.w;
}

ZP_FORCE_INLINE zpVector4f zpVector4f::cross3( const zpVector4f& vector ) const {
	return zpVector4f(
		m_xyzw.y * vector.m_xyzw.z - m_xyzw.z * vector.m_xyzw.y,
		m_xyzw.z * vector.m_xyzw.x - m_xyzw.x * vector.m_xyzw.z,
		m_xyzw.x * vector.m_xyzw.y - m_xyzw.y * vector.m_xyzw.x
	);
}
ZP_FORCE_INLINE zpVector4f zpVector4f::cross4( const zpVector4f& vector ) const {
	zp_float Pxy = m_xyzw.x * vector.m_xyzw.y - vector.m_xyzw.x * m_xyzw.y;
	zp_float Pxz = m_xyzw.x * vector.m_xyzw.z - vector.m_xyzw.x * m_xyzw.z;
	zp_float Pxw = m_xyzw.x * vector.m_xyzw.w - vector.m_xyzw.x * m_xyzw.w;
	zp_float Pyz = m_xyzw.y * vector.m_xyzw.z - vector.m_xyzw.y * m_xyzw.z;
	zp_float Pyw = m_xyzw.y * vector.m_xyzw.w - vector.m_xyzw.y * m_xyzw.w;
	zp_float Pzw = m_xyzw.z * vector.m_xyzw.w - vector.m_xyzw.z * m_xyzw.w;

	return zpVector4f(
		m_xyzw.y * Pzw - m_xyzw.z * Pyw + m_xyzw.w * Pyz,
		m_xyzw.z * Pxw - m_xyzw.x * Pzw - m_xyzw.w * Pxz,
		m_xyzw.x * Pyw - m_xyzw.y * Pxw + m_xyzw.w * Pxy,
		m_xyzw.y * Pxz - m_xyzw.x * Pyz - m_xyzw.z * Pxy
	);
}

ZP_FORCE_INLINE void zpVector4f::abs3() {
	m_xyzw.x = zp_abs( m_xyzw.x );
	m_xyzw.y = zp_abs( m_xyzw.y );
	m_xyzw.z = zp_abs( m_xyzw.z );
}
ZP_FORCE_INLINE void zpVector4f::abs4() {
	m_xyzw.x = zp_abs( m_xyzw.x );
	m_xyzw.y = zp_abs( m_xyzw.y );
	m_xyzw.z = zp_abs( m_xyzw.z );
	m_xyzw.w = zp_abs( m_xyzw.w );
}
ZP_FORCE_INLINE zpVector4f zpVector4f::abs3() const {
	return zpVector4f( zp_abs( m_xyzw.x ), zp_abs( m_xyzw.y ), zp_abs( m_xyzw.z ), m_xyzw.w );
}
ZP_FORCE_INLINE zpVector4f zpVector4f::abs4() const {
	return zpVector4f( zp_abs( m_xyzw.x ), zp_abs( m_xyzw.y ), zp_abs( m_xyzw.z ), zp_abs( m_xyzw.w ) );
}

ZP_FORCE_INLINE zp_bool zpVector4f::equals3( const zpVector4f& vector ) const {
	return m_xyzw.x == vector.m_xyzw.x && m_xyzw.y == vector.m_xyzw.y && m_xyzw.z == vector.m_xyzw.z;
}
ZP_FORCE_INLINE zp_bool zpVector4f::equals4( const zpVector4f& vector ) const {
	return m_xyzw.x == vector.m_xyzw.x && m_xyzw.y == vector.m_xyzw.y && m_xyzw.z == vector.m_xyzw.z && m_xyzw.w == vector.m_xyzw.w;
}

ZP_FORCE_INLINE zp_bool zpVector4f::operator==( const zpVector4f& vector ) const {
	return m_xyzw.x == vector.m_xyzw.x && m_xyzw.y == vector.m_xyzw.y && m_xyzw.z == vector.m_xyzw.z && m_xyzw.w == vector.m_xyzw.w;
}
ZP_FORCE_INLINE zp_bool zpVector4f::operator!=( const zpVector4f& vector ) const {
	return m_xyzw.x != vector.m_xyzw.x || m_xyzw.y != vector.m_xyzw.y || m_xyzw.z != vector.m_xyzw.z || m_xyzw.w != vector.m_xyzw.w;
}
