#include "zpCore.h"

zpBoundingSphere::zpBoundingSphere() :
	m_sphere( 0, 0, 0, 1 )
{}
zpBoundingSphere::zpBoundingSphere( const zpVector4f& center, zp_float radius ) :
	m_sphere( center.getX(), center.getY(), center.getZ(), zp_real_from_float( radius ) )
{}
zpBoundingSphere::zpBoundingSphere( const zpBoundingSphere& sphere ) :
	m_sphere( sphere.m_sphere )
{}
zpBoundingSphere::zpBoundingSphere( zpBoundingSphere&& sphere ) :
	m_sphere( sphere.m_sphere )
{}
zpBoundingSphere::~zpBoundingSphere() {}

void zpBoundingSphere::operator=( const zpBoundingSphere& sphere ) {
	m_sphere = sphere.m_sphere;
}
void zpBoundingSphere::operator=( zpBoundingSphere&& sphere ) {
	m_sphere = sphere.m_sphere;
}

const zpVector4f& zpBoundingSphere::getCenter() const {
	return m_sphere;
}
void zpBoundingSphere::setCenter( const zpVector4f& center ) {
	m_sphere.set3( center );
}

zp_float zpBoundingSphere::getRadius() const {
	return zp_real_to_float( m_sphere.getW() );
}
void zpBoundingSphere::setRadius( zp_float radius ) {
	m_sphere.setW( zp_real_from_float( radius ) );
}

void zpBoundingSphere::translate( const zpVector4f& translate ) {
	m_sphere.add3( translate );
}
void zpBoundingSphere::scale( zp_float scale ) {
	m_sphere.setW( m_sphere.getW() * zp_real_from_float( scale ) );
}
void zpBoundingSphere::pad( zp_float padding ) {
	m_sphere.setW( m_sphere.getW() + zp_real_from_float( padding )  );
}

void zpBoundingSphere::add( zp_float x, zp_float y, zp_float z ) {
	add( zpVector4f( x, y, z ) );
}
void zpBoundingSphere::add( const zpVector4f& point ) {
	zpVector4f dist( m_sphere );
	dist.sub3( point );
	zp_real d = dist.magnitude3();

	if( zp_real_gt( d, m_sphere.getW() ) ) m_sphere.setW( d );
}
void zpBoundingSphere::add( const zpBoundingAABB& box ) {
	zpVector4f min( box.getMin() );
	zpVector4f max( box.getMax() );

	min.sub3( m_sphere );
	max.sub3( m_sphere );

	zp_real d = zp_real_max( min.magnitude3(), max.magnitude3() );
	if( zp_real_gt( d, m_sphere.getW() ) ) m_sphere.setW( d );
}
void zpBoundingSphere::add( const zpBoundingSphere& sphere ) {
	zpVector4f dist( m_sphere );
	dist.sub3( sphere.getCenter() );
	zp_real d = dist.magnitude3();
	d = zp_real_add( d, zp_real_add( m_sphere.getW(), sphere.getCenter().getW() ) );

	if( zp_real_gt( d, m_sphere.getW() ) ) m_sphere.setW( d );
}