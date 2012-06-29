
#include <malloc.h>
/*
ZP_FORCE_INLINE void* zpVector4f::operator new( zp_uint size ) {
	return malloc( size );
};
ZP_FORCE_INLINE void zpVector4f::operator delete( void* ptr ) {
	free( ptr );
}
*/
ZP_FORCE_INLINE zpVector4f::zpVector4f() : 
	m_xyzw()
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( zp_float x, zp_float y, zp_float z, zp_float w ) :
	m_xyzw( x, y, z, w )
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( const zpReal& x, const zpReal& y, const zpReal& z, const zpReal& w ) :
	m_xyzw( x, y, z, w )
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( const zpVector4f& vector ) : 
	m_xyzw( vector.m_xyzw )
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( zpVector4f&& vector ) : 
	m_xyzw( vector.m_xyzw )
{}
ZP_FORCE_INLINE zpVector4f::zpVector4f( const zp_vec4& vector ) : 
	m_xyzw( vector ) 
{}
ZP_FORCE_INLINE zpVector4f::~zpVector4f() {}

ZP_FORCE_INLINE void zpVector4f::operator=( const zpVector4f& vector ) {
	m_xyzw = vector.m_xyzw;
}
ZP_FORCE_INLINE void zpVector4f::operator=( zpVector4f&& vector ) {
	m_xyzw = vector.m_xyzw;
}

ZP_FORCE_INLINE zpVector4f zpVector4f::operator-() const {
	return zpVector4f( -m_xyzw.x, -m_xyzw.y, -m_xyzw.z, m_xyzw.w );
}

ZP_FORCE_INLINE void zpVector4f::set3( const zpReal& x, const zpReal& y, const zpReal& z ) {
	m_xyzw.x = x.getReal();
	m_xyzw.y = y.getReal();
	m_xyzw.z = z.getReal();
}
ZP_FORCE_INLINE void zpVector4f::set4( const zpReal& x, const zpReal& y, const zpReal& z, const zpReal& w ) {
	m_xyzw.x = x.getReal();
	m_xyzw.y = y.getReal();
	m_xyzw.z = z.getReal();
	m_xyzw.w = w.getReal();
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

ZP_FORCE_INLINE zpReal zpVector4f::getX() const {
	return m_xyzw.x;
}
ZP_FORCE_INLINE zpReal zpVector4f::getY() const {
	return m_xyzw.y;
}
ZP_FORCE_INLINE zpReal zpVector4f::getZ() const {
	return m_xyzw.z;
}
ZP_FORCE_INLINE zpReal zpVector4f::getW() const {
	return m_xyzw.w;
}

ZP_FORCE_INLINE void zpVector4f::store3( zpReal* xyz ) const {
	xyz[0] = m_xyzw.x;
	xyz[1] = m_xyzw.y;
	xyz[2] = m_xyzw.z;
}
ZP_FORCE_INLINE void zpVector4f::store4( zpReal* xyzw ) const {
	xyzw[0] = m_xyzw.x;
	xyzw[1] = m_xyzw.y;
	xyzw[2] = m_xyzw.z;
	xyzw[3] = m_xyzw.w;
}
ZP_FORCE_INLINE void zpVector4f::storef3( zp_float* xyz ) const {
	xyz[0] = m_xyzw.x;
	xyz[1] = m_xyzw.y;
	xyz[2] = m_xyzw.z;
}
ZP_FORCE_INLINE void zpVector4f::storef4( zp_float* xyzw ) const {
	xyzw[0] = m_xyzw.x;
	xyzw[1] = m_xyzw.y;
	xyzw[2] = m_xyzw.z;
	xyzw[3] = m_xyzw.w;
}

ZP_FORCE_INLINE void zpVector4f::load3( const zpReal* xyz ) {
	m_xyzw.x = xyz[0];
	m_xyzw.y = xyz[1];
	m_xyzw.z = xyz[2];
}
ZP_FORCE_INLINE void zpVector4f::load4( const zpReal* xyzw ) {
	m_xyzw.x = xyzw[0];
	m_xyzw.y = xyzw[1];
	m_xyzw.z = xyzw[2];
	m_xyzw.w = xyzw[3];
}
ZP_FORCE_INLINE void zpVector4f::loadf3( zp_float* xyz ) {
	m_xyzw.x = xyz[0];
	m_xyzw.y = xyz[1];
	m_xyzw.z = xyz[2];
}
ZP_FORCE_INLINE void zpVector4f::loadf4( zp_float* xyzw ) {
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

ZP_FORCE_INLINE void zpVector4f::mul3( const zpReal& r ) {
	m_xyzw.x *= r;
	m_xyzw.y *= r;
	m_xyzw.z *= r;
}
ZP_FORCE_INLINE void zpVector4f::mul4( const zpReal& r ) {
	m_xyzw.x *= r;
	m_xyzw.y *= r;
	m_xyzw.z *= r;
	m_xyzw.w *= r;
}
ZP_FORCE_INLINE void zpVector4f::div3( const zpReal& r ) {
	zp_real invR = 1.f / r;
	m_xyzw.x *= invR;
	m_xyzw.y *= invR;
	m_xyzw.z *= invR;
}
ZP_FORCE_INLINE void zpVector4f::div4( const zpReal& r ) {
	zp_real invR = 1.f / r;
	m_xyzw.x *= invR;
	m_xyzw.y *= invR;
	m_xyzw.z *= invR;
	m_xyzw.w *= invR;
}
ZP_FORCE_INLINE void zpVector4f::add3( const zpReal& r ) {
	m_xyzw.x += r;
	m_xyzw.y += r;
	m_xyzw.z += r;
}
ZP_FORCE_INLINE void zpVector4f::add4( const zpReal& r ) {
	m_xyzw.x += r;
	m_xyzw.y += r;
	m_xyzw.z += r;
	m_xyzw.w += r;
}
ZP_FORCE_INLINE void zpVector4f::sub3( const zpReal& r ) {
	m_xyzw.x -= r;
	m_xyzw.y -= r;
	m_xyzw.z -= r;
}
ZP_FORCE_INLINE void zpVector4f::sub4( const zpReal& r ) {
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

ZP_FORCE_INLINE zpReal zpVector4f::dot3( const zpVector4f& vector ) const {
	return m_xyzw.x * vector.m_xyzw.x + m_xyzw.y * vector.m_xyzw.y + m_xyzw.z * vector.m_xyzw.z;
}
ZP_FORCE_INLINE zpReal zpVector4f::dot4( const zpVector4f& vector ) const {
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

ZP_FORCE_INLINE zpReal zpVector4f::magnitude3() const {
	return m_xyzw.x * m_xyzw.x + m_xyzw.y * m_xyzw.y + m_xyzw.z * m_xyzw.z;
}
ZP_FORCE_INLINE zpReal zpVector4f::magnitude4() const {
	return m_xyzw.x * m_xyzw.x + m_xyzw.y * m_xyzw.y + m_xyzw.z * m_xyzw.z + m_xyzw.w * m_xyzw.w;
}
ZP_FORCE_INLINE zpReal zpVector4f::magnitudeSquared3() const {
	return magnitude3().sqrt();
}
ZP_FORCE_INLINE zpReal zpVector4f::magnitudeSquared4() const {
	return magnitude4().sqrt();
}

ZP_FORCE_INLINE zpVector4f zpVector4f::normalize3() const {
	zpReal mag = magnitude3();
	zpVector4f v( m_xyzw );
	if( mag == 0 ) return v;
	v.div3( mag );
	return v;
}
ZP_FORCE_INLINE void zpVector4f::normalize3() {
	zpReal mag = magnitude3();
	if( mag != 0 ) div3( mag );
}
ZP_FORCE_INLINE zpVector4f zpVector4f::normalize4() const {
	zpReal mag = magnitude4();
	zpVector4f v( m_xyzw );
	if( mag == 0 ) return v;
	v.div4( mag );
	return v;
}
ZP_FORCE_INLINE void zpVector4f::normalize4() {
	zpReal mag = magnitude4();
	if( mag != 0 ) div4( mag );
}

ZP_FORCE_INLINE zpVector4f zpVector4f::homogenize4() const {
	return m_xyzw.w == 0 ?
	zpVector4f() :
	zpVector4f(
		m_xyzw.x / m_xyzw.w,
		m_xyzw.y / m_xyzw.w,
		m_xyzw.z / m_xyzw.w,
		1
	);
}
ZP_FORCE_INLINE void zpVector4f::homogenize4() {
	if( m_xyzw.w == 0 ) {
		zero4();
	} else {
		m_xyzw.x /= m_xyzw.w;
		m_xyzw.y /= m_xyzw.w;
		m_xyzw.z /= m_xyzw.w;
		m_xyzw.w = 1;
	}
}

ZP_FORCE_INLINE zpVector4f zpVector4f::lerp3( const zpVector4f& vector, zpReal alpha ) const {
	alpha.clamp01();
	zpVector4f v( vector );
	v.sub3( *this );
	v.mul3( alpha );
	v.add3( *this );
	return v;
}
ZP_FORCE_INLINE void zpVector4f::lerp3( const zpVector4f& vector, zpReal alpha ) {
	alpha.clamp01();
	zpVector4f v( vector );
	v.sub3( *this );
	v.mul3( alpha );
	add3( v );
}
ZP_FORCE_INLINE zpVector4f zpVector4f::lerp4( const zpVector4f& vector, zpReal alpha ) const {
	alpha.clamp01();
	zpVector4f v( vector );
	v.sub4( *this );
	v.mul4( alpha );
	v.add4( *this );
	return v;
}
ZP_FORCE_INLINE void zpVector4f::lerp4( const zpVector4f& vector, zpReal alpha ) {
	alpha.clamp01();
	zpVector4f v( vector );
	v.sub4( *this );
	v.mul4( alpha );
	add4( v );
}

ZP_FORCE_INLINE zpVector4f zpVector4f::slerp3( const zpVector4f& vector, zpReal alpha ) const {
	alpha.clamp01();
	
	zpReal omega = dot3( vector );
	zpReal somao = ( zpReal( 1 ) - alpha ) * omega;
	somao.sin();
	zpReal sao = alpha * omega;
	sao.sin();
	zpReal so = omega;
	so.sin();

	somao.div( so );
	sao.div( so );

	zpVector4f t( *this );
	t.mul3( somao );

	zpVector4f v( vector );
	v.mul3( sao );

	t.add3( v );
	return t;
}
ZP_FORCE_INLINE void zpVector4f::slerp3( const zpVector4f& vector, zpReal alpha ) {
	alpha.clamp01();
	
	zpReal omega = dot3( vector );
	zpReal somao = ( zpReal( 1 ) - alpha ) * omega;
	somao.sin();
	zpReal sao = alpha * omega;
	sao.sin();
	zpReal so = omega;
	so.sin();

	somao.div( so );
	sao.div( so );

	zpVector4f t( *this );
	t.mul3( somao );

	zpVector4f v( vector );
	v.mul3( sao );

	t.add3( v );
	m_xyzw = t.m_xyzw;
}
ZP_FORCE_INLINE zpVector4f zpVector4f::slerp4( const zpVector4f& vector, zpReal alpha ) const {
	alpha.clamp01();
	
	zpReal omega = dot4( vector );
	zpReal somao = ( zpReal( 1 ) - alpha ) * omega;
	somao.sin();
	zpReal sao = alpha * omega;
	sao.sin();
	zpReal so = omega;
	so.sin();

	somao.div( so );
	sao.div( so );

	zpVector4f t( *this );
	t.mul4( somao );

	zpVector4f v( vector );
	v.mul4( sao );

	t.add4( v );
	return t;
}
ZP_FORCE_INLINE void zpVector4f::slerp4( const zpVector4f& vector, zpReal alpha ) {
	alpha.clamp01();
	
	zpReal omega = dot4( vector );
	zpReal somao = ( zpReal( 1 ) - alpha ) * omega;
	somao.sin();
	zpReal sao = alpha * omega;
	sao.sin();
	zpReal so = omega;
	so.sin();

	somao.div( so );
	sao.div( so );

	zpVector4f t( *this );
	t.mul4( somao );

	zpVector4f v( vector );
	v.mul4( sao );

	t.add4( v );
	m_xyzw = t.m_xyzw;
}

ZP_FORCE_INLINE zp_bool zpVector4f::operator==( const zpVector4f& vector ) const {
	return m_xyzw.x == vector.m_xyzw.x && m_xyzw.y == vector.m_xyzw.y && m_xyzw.z == vector.m_xyzw.z && m_xyzw.w == vector.m_xyzw.w;
}
ZP_FORCE_INLINE zp_bool zpVector4f::operator!=( const zpVector4f& vector ) const {
	return m_xyzw.x != vector.m_xyzw.x || m_xyzw.y != vector.m_xyzw.y || m_xyzw.z != vector.m_xyzw.z || m_xyzw.w != vector.m_xyzw.w;
}
