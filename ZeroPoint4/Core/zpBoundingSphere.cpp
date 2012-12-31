#include "zpCore.h"

zpBoundingSphere::zpBoundingSphere() :
	m_sphere( 0, 0, 0, 1 )
{}
zpBoundingSphere::zpBoundingSphere( const zpVector4f& center, zp_float radius ) :
	m_sphere( center.getX(), center.getY(), center.getZ(), zpScalar( radius ) )
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
	return m_sphere.getW().getFloat();
}
void zpBoundingSphere::setRadius( zp_float radius ) {
	m_sphere.setW( zpScalar( radius ) );
}

zpBoundingAABB zpBoundingSphere::generateBoundingAABB() const {
	zpScalar r( m_sphere.getW() );
	zpScalar nr;
	zpScalarNeg( nr, r );

	zpVector4f min( nr, nr, nr );
	zpVector4f max( r, r, r );
	min.add3( m_sphere );
	max.add3( m_sphere );

	return zpBoundingAABB( min, max );
}

void zpBoundingSphere::translate( const zpVector4f& translate ) {
	m_sphere.add3( translate );
}
void zpBoundingSphere::scale( zp_float scale ) {
	m_sphere.setW( m_sphere.getW() * zpScalar( scale ) );
}
void zpBoundingSphere::pad( zp_float padding ) {
	m_sphere.setW( m_sphere.getW() + zpScalar( padding )  );
}

void zpBoundingSphere::add( zp_float x, zp_float y, zp_float z ) {
	add( zpVector4f( x, y, z ) );
}
void zpBoundingSphere::add( const zpVector4f& point ) {
	zpVector4f dist( m_sphere );
	dist.sub3( point );
	zpScalar d = dist.magnitude3();

	if( zpScalarGt( d, m_sphere.getW() ) ) m_sphere.setW( d );
}
void zpBoundingSphere::add( const zpBoundingAABB& box ) {
	zpVector4f min( box.getMin() );
	zpVector4f max( box.getMax() );

	min.sub3( m_sphere );
	max.sub3( m_sphere );

	zpScalar d;
	zpScalarMax( d, min.magnitude3(), max.magnitude3() );
	if( zpScalarGt( d, m_sphere.getW() ) ) m_sphere.setW( d );
}
void zpBoundingSphere::add( const zpBoundingSphere& sphere ) {
	zpVector4f dist( m_sphere );
	dist.sub3( sphere.getCenter() );
	zpScalar d( dist.magnitude3() + m_sphere.getW() + sphere.getCenter().getW() );

	if( zpScalarGt( d, m_sphere.getW() ) ) m_sphere.setW( d );
}