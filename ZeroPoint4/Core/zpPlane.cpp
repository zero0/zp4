#include "zpCore.h"

zpPlane::zpPlane() : 
	m_plane()
{}
zpPlane::zpPlane( const zpVector4f& normal ) : 
	m_plane( normal )
{}
zpPlane::zpPlane( zp_float a, zp_float b, zp_float c, zp_float d ) :
	m_plane( a, b, c, d )
{
	zp_real l = m_plane.magnitude3();
	m_plane.div3( l );
	m_plane.setW( zp_real_from_float( d ) / l );
}
zpPlane::zpPlane( const zpPlane& plane ) :
	m_plane( plane.m_plane )
{}
zpPlane::zpPlane( zpPlane&& plane ) : 
	m_plane( plane.m_plane )
{}
zpPlane::~zpPlane() {}

void zpPlane::operator=( const zpPlane& plane ) {
	m_plane = plane.m_plane;
}
void zpPlane::operator=( zpPlane&& plane ) {
	m_plane = zp_move( plane.m_plane );
}

zpPlaneSide zpPlane::getSideOfPlane( const zpVector4f& point ) const {
	zp_real z = zp_real_zero();
	zp_real d = point.dot3( m_plane );
	d = zp_real_add( d, m_plane.getW() );
	
	return zp_real_gt( d, z ) ? ZP_PLANE_SIDE_POSITIVE : zp_real_lt( d, z ) ? ZP_PLANE_SIDE_NEGATIVE : ZP_PLANE_SIDE_ON_PLANE;
}
zp_float zpPlane::getDistanceToPoint( const zpVector4f& point ) const {
	zp_real d = point.dot3( m_plane );
	d = zp_real_add( d, m_plane.getW() );
	return zp_real_to_float( d );
}

void zpPlane::set( zp_float a, zp_float b, zp_float c, zp_float d ) {
	m_plane = zpVector4f( a, b, c, d );
	zp_real l = m_plane.magnitude3();
	m_plane.div3( l );
	m_plane.setW( zp_real_from_float( d ) / l );
}
void zpPlane::set( const zpVector4f& p0, const zpVector4f& p1, const zpVector4f& p2 ) {
	m_plane = ( p1 - p0 ).cross3( p2 - p0 );
	zp_real l = m_plane.magnitude3();
	m_plane.div3( l );
	m_plane.setW( zp_real_neg( m_plane.dot3( m_plane ) ) / l );
}

const zpVector4f& zpPlane::getVector() const {
	return m_plane;
}