#include "zpCore.h"

zpPlane::zpPlane()
	: m_plane()
{}
zpPlane::zpPlane( const zpVector4f& normal ) 
	: m_plane( normal )
{}
zpPlane::zpPlane( zp_float a, zp_float b, zp_float c, zp_float d )
	: m_plane( zpMath::Vector4( a, b, c, d ) )
{
	zpScalar l;
	l = zpMath::Vector4Length3( m_plane );
	m_plane = zpMath::Vector4Div( m_plane, l );
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
	zpScalar z = zpMath::Scalar( 0.0f );
	zpScalar d;
	d = zpMath::Vector4Dot3( point, m_plane );
	d = zpMath::Vector4Add( d, zpMath::Vector4GetW( m_plane ) );

	zp_int c;
	c = zpMath::ScalarCmp( d, z );

	return c > 0 ? ZP_PLANE_SIDE_POSITIVE : c < 0 ? ZP_PLANE_SIDE_NEGATIVE : ZP_PLANE_SIDE_ON_PLANE;
}
zpScalar zpPlane::getDistanceToPoint( const zpVector4f& point ) const
{
	zpScalar d;
	d = zpMath::Vector4Dot3( point, m_plane );
	d = zpMath::Vector4Add( d, zpMath::Vector4GetW( m_plane ) );

	return d;
}

void zpPlane::set( zp_float a, zp_float b, zp_float c, zp_float d )
{
	m_plane = zpMath::Vector4( a, b, c, d );

	zpScalar l;
	l = zpMath::Vector4Length3( m_plane );
	m_plane = zpMath::Vector4Div( m_plane, l );
}
void zpPlane::set( const zpVector4f& p0, const zpVector4f& p1, const zpVector4f& p2 )
{
	zpVector4f p01, p02;
	zpScalar d;

	p01 = zpMath::Vector4Sub( p0, p1 );
	p02 = zpMath::Vector4Sub( p2, p1 );
	m_plane = zpMath::Vector4Cross3( p01, p02 );
	m_plane = zpMath::Vector4Normalize3( m_plane );

	d = zpMath::Vector4Dot3( m_plane, p1 );
	d = zpMath::Vector4Neg( d );
	m_plane = zpMath::Vector4SetW( m_plane, d );
}

const zpVector4f& zpPlane::getVector() const
{
	return m_plane;
}