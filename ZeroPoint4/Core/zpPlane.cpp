#include "zpCore.h"

zpPlane::zpPlane()
	: m_plane()
{}
zpPlane::zpPlane( const zpVector4f& normal ) 
	: m_plane( normal )
{}
zpPlane::zpPlane( zp_float a, zp_float b, zp_float c, zp_float d )
	: m_plane( a, b, c, d )
{
	zpScalar l( m_plane.magnitude3() );
	m_plane.div3( l );

	zpScalar sd( d );
	zpScalarDiv( sd, sd, l );
	m_plane.setW( sd );
}
zpPlane::zpPlane( const zpPlane& plane )
	: m_plane( plane.m_plane )
{}
zpPlane::zpPlane( zpPlane&& plane )
	: m_plane( plane.m_plane )
{}
zpPlane::~zpPlane() {}

void zpPlane::operator=( const zpPlane& plane ) {
	m_plane = plane.m_plane;
}
void zpPlane::operator=( zpPlane&& plane ) {
	m_plane = zp_move( plane.m_plane );
}

zpPlaneSide zpPlane::getSideOfPlane( const zpVector4f& point ) const {
	zpScalar z;
	zpScalar d( point.dot3( m_plane ) );
	zpScalarAdd( d, d, m_plane.getW() );
	
	return zpScalarGt( d, z ) ? ZP_PLANE_SIDE_POSITIVE : zpScalarLt( d, z ) ? ZP_PLANE_SIDE_NEGATIVE : ZP_PLANE_SIDE_ON_PLANE;
}
zp_float zpPlane::getDistanceToPoint( const zpVector4f& point ) const {
	zpScalar d( point.dot3( m_plane ) );
	zpScalarAdd( d, d, m_plane.getW() );
	return d.getFloat();
}

void zpPlane::set( zp_float a, zp_float b, zp_float c, zp_float d ) {
	m_plane = zpVector4f( a, b, c, d );
	zpScalar l( m_plane.magnitude3() );
	m_plane.div3( l );

	zpScalar sd( d );
	zpScalarDiv( sd, sd, l );
	m_plane.setW( sd );
}
void zpPlane::set( const zpVector4f& p0, const zpVector4f& p1, const zpVector4f& p2 ) {
	m_plane = ( p1 - p0 ).cross3( p2 - p0 );
	zpScalar l = m_plane.magnitude3();
	m_plane.div3( l );

	zpScalar d( m_plane.dot3( m_plane ) );
	zpScalarNeg( d, d );
	zpScalarDiv( d, d, l );
	m_plane.setW( d );
}

const zpVector4f& zpPlane::getVector() const {
	return m_plane;
}