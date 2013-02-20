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
	zpScalar l;
	zpMath::Length3( l, m_plane );
	zpMath::Div( m_plane, m_plane, l );
}
zpPlane::zpPlane( const zpPlane& plane )
	: m_plane( plane.m_plane )
{}
zpPlane::zpPlane( zpPlane&& plane )
	: m_plane( plane.m_plane )
{}
zpPlane::~zpPlane()
{}

void zpPlane::operator=( const zpPlane& plane )
{
	m_plane = plane.m_plane;
}
void zpPlane::operator=( zpPlane&& plane )
{
	m_plane = zp_move( plane.m_plane );
}

zpPlaneSide zpPlane::getSideOfPlane( const zpVector4f& point ) const
{
	zpScalar z( 0.0f );
	zpScalar d;
	zpMath::Dot3( d, point, m_plane );
	zpMath::Add( d, d, m_plane.getW() );

	zp_int c;
	zpMath::Cmp( c, d, z );

	return c > 0 ? ZP_PLANE_SIDE_POSITIVE : c < 0 ? ZP_PLANE_SIDE_NEGATIVE : ZP_PLANE_SIDE_ON_PLANE;
}
zpScalar zpPlane::getDistanceToPoint( const zpVector4f& point ) const
{
	zpScalar d;
	zpMath::Dot3( d, point, m_plane );
	zpMath::Add( d, d, m_plane.getW() );

	return d;
}

void zpPlane::set( zp_float a, zp_float b, zp_float c, zp_float d )
{
	m_plane = zpVector4f( a, b, c, d );

	zpScalar l;
	zpMath::Length3( l, m_plane );
	zpMath::Div( m_plane, m_plane, l );
}
void zpPlane::set( const zpVector4f& p0, const zpVector4f& p1, const zpVector4f& p2 )
{
	zpVector4f p01, p02;
	zpScalar d, l;

	zpMath::Sub( p01, p1, p0 );
	zpMath::Sub( p02, p2, p1 );
	zpMath::Cross3( m_plane, p01, p02 );
	zpMath::Length3( l, m_plane );
	zpMath::Div( m_plane, m_plane, l );

	zpMath::Dot3( d, m_plane, m_plane );
	zpMath::Neg( d, d );
	zpMath::Div( d, d, l );
	m_plane.setW( d );
}

const zpVector4f& zpPlane::getVector() const
{
	return m_plane;
}