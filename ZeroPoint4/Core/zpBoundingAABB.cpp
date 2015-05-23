#include "zpCore.h"

zpBoundingAABB::zpBoundingAABB()
	: m_center( zpMath::Vector4( 0.f, 0.f, 0.f, 1.f ) )
	, m_extent( zpMath::Vector4( ZP_FLT_MIN, ZP_FLT_MIN, ZP_FLT_MIN, 0.f ) )
{}
zpBoundingAABB::zpBoundingAABB( zp_float width, zp_float height, zp_float depth )
	: m_center( zpMath::Vector4( 0.f, 0.f, 0.f, 1.f ) )
	, m_extent( zpMath::Vector4( width * 0.5f, height * 0.5f, depth * 0.5f, 0.f ) )
{}
zpBoundingAABB::zpBoundingAABB( const zpBoundingAABB& box )
	: m_center( box.m_center )
	, m_extent( box.m_extent )
{}
zpBoundingAABB::zpBoundingAABB( zpBoundingAABB&& box )
	: m_center( box.m_center )
	, m_extent( box.m_extent )
{}
zpBoundingAABB::~zpBoundingAABB()
{}

void zpBoundingAABB::reset()
{
	m_center = zpMath::Vector4( 0.f, 0.f, 0.f, 1.f );
	m_extent = zpMath::Vector4( ZP_FLT_MIN, ZP_FLT_MIN, ZP_FLT_MIN, 0.f );
}

zpVector4f zpBoundingAABB::getMin() const
{
	zpVector4f min;
	min = zpMath::Vector4Sub( m_center, m_extent );
	return min;
}
zpVector4f zpBoundingAABB::getMax() const
{
	zpVector4f max;
	max = zpMath::Vector4Add( m_center, m_extent );
	return max;
}
void zpBoundingAABB::setMin( const zpVector4f& min )
{
	setMinMax( min, getMax() );
}
void zpBoundingAABB::setMax( const zpVector4f& max )
{
	setMinMax( getMin(), max );
}
void zpBoundingAABB::setMinMax( const zpVector4f& min, const zpVector4f& max )
{
	m_extent = zpMath::Vector4Sub( max, min );
	m_extent = zpMath::Vector4Mul( m_extent, zpMath::Scalar( 0.5f ) );
	m_center = zpMath::Vector4Add( min, m_extent );
}

const zpVector4f& zpBoundingAABB::getCenter() const
{
	return m_center;
}
void zpBoundingAABB::setCenter( const zpVector4f& center )
{
	m_center = center;
}
const zpVector4f& zpBoundingAABB::getExtents() const
{
	return m_extent;
}
void zpBoundingAABB::setExtents( const zpVector4f& extents )
{
	m_extent = extents;
}

void zpBoundingAABB::generateBoundingSphere( zpBoundingSphere& sphere ) const
{
	zpScalar r;
	r = zpMath::Vector4Length3( m_extent );

	sphere.setCenter( m_center );
	sphere.setRadius( r );
}

zpScalar zpBoundingAABB::getWidth() const
{
	zpScalar s;
	s = zpMath::Vector4Mul( zpMath::Vector4GetX( m_extent ), zpMath::Scalar( 2.f ) );
	return s;
}
zpScalar zpBoundingAABB::getHeight() const
{
	zpScalar s;
	s = zpMath::Vector4Mul( zpMath::Vector4GetY( m_extent ), zpMath::Scalar( 2.f ) );
	return s;
}
zpScalar zpBoundingAABB::getDepth() const
{
	zpScalar s;
	s = zpMath::Vector4Mul( zpMath::Vector4GetZ( m_extent ), zpMath::Scalar( 2.f ) );
	return s;
}
zpVector4f zpBoundingAABB::getSize() const
{
	zpVector4f s;
	s = zpMath::Vector4Mul( m_extent, zpMath::Vector4( 2.f, 2.f, 2.f, 0.f ) );
	return s;
}

void zpBoundingAABB::translate( const zpVector4f& translate )
{
	m_center = zpMath::Vector4Add( m_center, translate );
}
void zpBoundingAABB::scaleUniform( const zpScalar& scale )
{
	m_extent = zpMath::Vector4Mul( m_extent, scale );
}
void zpBoundingAABB::scale( const zpVector4f& scale )
{
	m_extent = zpMath::Vector4Mul( m_extent, scale );
}
void zpBoundingAABB::padUniform( const zpScalar& padding )
{
	m_extent = zpMath::Vector4Add( m_extent, padding );
}
void zpBoundingAABB::pad( const zpVector4f& padding )
{
	m_extent = zpMath::Vector4Add( m_extent, padding );
}

void zpBoundingAABB::add( zp_float x, zp_float y, zp_float z )
{
	add( zpMath::Vector4( x, y, z, 0.f ) );
}
void zpBoundingAABB::add( const zpScalar& x, const zpScalar& y, const zpScalar& z )
{
	add( zpMath::Vector4( x, y, z, zpMath::Scalar( 0.f ) ) );
}
void zpBoundingAABB::add( const zpVector4f& point )
{
	zpVector4f min = getMin();
	zpVector4f max = getMax();

	max = zpMath::Vector4Max( point, max );
	min = zpMath::Vector4Min( point, min );

	setMinMax( min, max );
}
void zpBoundingAABB::add( const zpBoundingAABB& box )
{
	add( box.getMin() );
	add( box.getMax() );
}
void zpBoundingAABB::add( const zpBoundingSphere& sphere )
{
	zpVector4f min( sphere.getCenter() );
	zpVector4f max( min );

	zpScalar nr;
	nr = zpMath::ScalarNeg( sphere.getRadius() );

	min = zpMath::Vector4Add( min, nr );
	max = zpMath::Vector4Add( max, sphere.getRadius() );

	add( min );
	add( max );
}