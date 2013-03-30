#include "zpCore.h"

zpBoundingSphere::zpBoundingSphere()
	: m_center( 0 )
	, m_radius( ZP_FLT_MIN )
{}
zpBoundingSphere::zpBoundingSphere( const zpVector4f& center, const zpScalar& radius )
	: m_center( center )
	, m_radius( radius )
{}
zpBoundingSphere::zpBoundingSphere( const zpBoundingSphere& sphere )
	: m_center( sphere.m_center )
	, m_radius( sphere.m_radius )
{}
zpBoundingSphere::zpBoundingSphere( zpBoundingSphere&& sphere )
	: m_center( sphere.m_center )
	, m_radius( sphere.m_radius )
{}
zpBoundingSphere::~zpBoundingSphere()
{}

void zpBoundingSphere::operator=( const zpBoundingSphere& sphere )
{
	m_center = sphere.m_center;
	m_radius = sphere.m_radius;
}
void zpBoundingSphere::operator=( zpBoundingSphere&& sphere )
{
	m_center = sphere.m_center;
	m_radius = sphere.m_radius;
}

const zpVector4f& zpBoundingSphere::getCenter() const
{
	return m_center;
}
void zpBoundingSphere::setCenter( const zpVector4f& center )
{
	m_center = center;
}

const zpScalar& zpBoundingSphere::getRadius() const
{
	return m_radius;
}
void zpBoundingSphere::setRadius( const zpScalar& radius )
{
	m_radius = radius;
}

zpBoundingAABB zpBoundingSphere::generateBoundingAABB() const
{
	zpScalar nr;
	zpMath::Neg( nr, m_radius );

	zpVector4f min( nr );
	zpVector4f max( m_radius );

	zpMath::Add( min, min, m_center );
	zpMath::Add( max, max, m_center );

	return zpBoundingAABB( min, max );
}

void zpBoundingSphere::translate( const zpVector4f& translate )
{
	zpMath::Add( m_center, m_center, translate );
}
void zpBoundingSphere::scale( const zpScalar& scale )
{
	zpMath::Mul( m_radius, m_radius, scale );
}
void zpBoundingSphere::pad( const zpScalar& padding )
{
	zpMath::Add( m_radius, m_radius, padding );
}

void zpBoundingSphere::add( zp_float x, zp_float y, zp_float z )
{
	add( zpVector4f( x, y, z ) );
}
void zpBoundingSphere::add( const zpScalar& x, const zpScalar& y, const zpScalar& z )
{
	add( zpVector4f( x, y, z ) );
}
void zpBoundingSphere::add( const zpVector4f& point )
{
	zpVector4f dist;
	zpScalar d;

	zpMath::Sub( dist, m_center, point );
	zpMath::Length3( d, dist );

	zpMath::Max( m_radius, m_radius, d );
}
void zpBoundingSphere::add( const zpBoundingAABB& box )
{
	zpVector4f min( box.getMin() );
	zpVector4f max( box.getMax() );

	zpMath::Sub( min, min, m_center );
	zpMath::Sub( max, max, m_center );

	zpScalar minD, maxD;
	zpMath::Length3( minD, min );
	zpMath::Length3( maxD, max );

	zpMath::Max( minD, minD, maxD );
	zpMath::Max( m_radius, m_radius, minD );
}
void zpBoundingSphere::add( const zpBoundingSphere& sphere )
{
	zpVector4f dist;
	zpScalar d;

	zpMath::Sub( dist, m_center, sphere.m_center );
	zpMath::Length3( d, dist );

	zpMath::Add( d, d, sphere.m_radius );

	zpMath::Max( m_radius, m_radius, d );
}