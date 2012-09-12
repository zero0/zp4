#include "zpCore.h"

zpRay::zpRay() :
	m_origin(),
	m_direction()
{}
zpRay::zpRay( const zpVector4f& origin, const zpVector4f& direction ) :
	m_origin( origin ),
	m_direction( direction.normalize3() )
{}
zpRay::zpRay( const zpRay& ray ) :
	m_origin( ray.m_origin ),
	m_direction( ray.m_direction )
{}
zpRay::zpRay( zpRay&& ray ) :
	m_origin( ray.m_origin ),
	m_direction( ray.m_direction )
{}
zpRay::~zpRay() {}

void zpRay::operator=( const zpRay& ray ) {
	m_origin = ray.m_origin;
	m_direction = ray.m_direction;
}
void zpRay::operator=( zpRay&& ray ) {
	m_origin = ray.m_origin;
	m_direction = ray.m_direction;
}

const zpVector4f& zpRay::getOrigin() const {
	return m_origin;
}
void zpRay::setOrigin( const zpVector4f& origin ) {
	m_origin = origin;
}

const zpVector4f& zpRay::getDirection() const {
	return m_direction;
}
void zpRay::setDirection( const zpVector4f& direction ) {
	m_direction = direction.normalize3();
}

zpVector4f zpRay::getPointAt( zp_float t ) const {
	return m_origin + ( m_direction * t );
}
zp_bool zpRay::isPointOnRay( const zpVector4f& point ) const {
	zpVector4f r( point );
	r.sub3( m_origin );
	r.normalize3();
	return zp_real_eq( r.dot3( m_direction ), zp_real_from_float( 1.f ) );
}