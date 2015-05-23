#include "zpCore.h"

zpBoundingSphere::zpBoundingSphere()
	: m_center( zpMath::Vector4( 0, 0, 0, 0 ) )
	, m_radius( zpMath::Scalar( ZP_FLT_MIN ) )
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

void zpBoundingSphere::generateBoundingAABB( zpBoundingAABB& box ) const
{
	zpScalar nr;
	nr = zpMath::ScalarNeg( m_radius );

	zpVector4f min = zpMath::Vector4( nr, nr, nr, zpMath::Scalar( 0 ) );
	zpVector4f max = zpMath::Vector4( m_radius, m_radius, m_radius, zpMath::Scalar( 0 ) );

	min = zpMath::Vector4Add( min, m_center );
	max = zpMath::Vector4Add( max, m_center );

	box.setMin( min );
	box.setMax( max );
}

void zpBoundingSphere::translate( const zpVector4f& translate )
{
	m_center = zpMath::Vector4Add( m_center, translate );
}
void zpBoundingSphere::scale( const zpScalar& scale )
{
	m_radius = zpMath::ScalarMul( m_radius, scale );
}
void zpBoundingSphere::pad( const zpScalar& padding )
{
	m_radius = zpMath::ScalarAdd( m_radius, padding );
}

void zpBoundingSphere::add( zp_float x, zp_float y, zp_float z )
{
	add( zpMath::Vector4( x, y, z, 0 ) );
}
void zpBoundingSphere::add( const zpScalar& x, const zpScalar& y, const zpScalar& z )
{
	add( zpMath::Vector4( x, y, z, zpMath::Scalar( 0 ) ) );
}
void zpBoundingSphere::add( const zpVector4f& point )
{
	zpVector4f dist;
	zpScalar d;

	dist = zpMath::Vector4Sub( m_center, point );
	d = zpMath::Vector4Length3( dist );

	m_radius = zpMath::ScalarMax( m_radius, d );
}
void zpBoundingSphere::add( const zpBoundingAABB& box )
{
	zpVector4f min( box.getMin() );
	zpVector4f max( box.getMax() );

	min = zpMath::Vector4Sub( min, m_center );
	max = zpMath::Vector4Sub( max, m_center );

	zpScalar minD, maxD;
	minD = zpMath::Vector4Length3( min );
	maxD = zpMath::Vector4Length3( max );

	minD = zpMath::ScalarMax( minD, maxD );
	m_radius = zpMath::ScalarMax( m_radius, minD );
}
void zpBoundingSphere::add( const zpBoundingSphere& sphere )
{
	zpVector4f dist;
	zpScalar d;

	dist = zpMath::Vector4Sub( m_center, sphere.m_center );
	d = zpMath::Vector4Length3( dist );

	d = zpMath::ScalarAdd( d, sphere.m_radius );

	m_radius = zpMath::ScalarMax( m_radius, d );
}