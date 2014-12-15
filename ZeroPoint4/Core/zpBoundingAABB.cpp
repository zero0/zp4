#include "zpCore.h"

zpBoundingAABB::zpBoundingAABB()
	: m_center( 0.f, 0.f, 0.f )
	, m_extent( ZP_FLT_MIN, ZP_FLT_MIN, ZP_FLT_MIN )
{}
zpBoundingAABB::zpBoundingAABB( zp_float width, zp_float height, zp_float depth )
	: m_center( 0.f, 0.f, 0.f )
	, m_extent( width * 0.5f, height * 0.5f, depth * 0.5f )
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

void zpBoundingAABB::operator=( const zpBoundingAABB& box )
{
	m_center = box.m_center;
	m_extent = box.m_extent;
}
void zpBoundingAABB::operator=( zpBoundingAABB&& box )
{
	m_center = box.m_center;
	m_extent = box.m_extent;
}

void zpBoundingAABB::reset()
{
	m_center = zpVector4f( 0.f, 0.f, 0.f );
	m_extent = zpVector4f( ZP_FLT_MIN, ZP_FLT_MIN, ZP_FLT_MIN );
}

zpVector4f zpBoundingAABB::getMin() const
{
	zpVector4f min;
	zpMath::Sub( min, m_center, m_extent );
	return min;
}
zpVector4f zpBoundingAABB::getMax() const
{
	zpVector4f max;
	zpMath::Add( max, m_center, m_extent );
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
	zpMath::Sub( m_extent, max, min );
	zpMath::Mul( m_extent, m_extent, zpScalar( 0.5f ) );
	zpMath::Add( m_center, min, m_extent );
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
	zpMath::Length3( r, m_extent );

	sphere.setCenter( m_center );
	sphere.setRadius( r );
}

zpScalar zpBoundingAABB::getWidth() const
{
	zpScalar s;
	zpMath::Mul( s, m_extent.getX(), zpScalar( 2.f ) );
	return s;
}
zpScalar zpBoundingAABB::getHeight() const
{
	zpScalar s;
	zpMath::Mul( s, m_extent.getY(), zpScalar( 2.f ) );
	return s;
}
zpScalar zpBoundingAABB::getDepth() const
{
	zpScalar s;
	zpMath::Mul( s, m_extent.getZ(), zpScalar( 2.f ) );
	return s;
}
zpVector4f zpBoundingAABB::getSize() const
{
	zpVector4f s;
	zpMath::Mul( s, m_extent, zpVector4f( 2.f, 2.f, 2.f ) );
	return s;
}

void zpBoundingAABB::translate( const zpVector4f& translate )
{
	zpMath::Add( m_center, m_center, translate );
}
void zpBoundingAABB::scale( const zpScalar& scale )
{
	zpMath::Mul( m_extent, m_extent, scale );
}
void zpBoundingAABB::scale( const zpVector4f& scale )
{
	zpMath::Mul( m_extent, m_extent, scale );
}
void zpBoundingAABB::pad( const zpScalar& padding )
{
	zpMath::Add( m_extent, m_extent, padding );
}
void zpBoundingAABB::pad( const zpVector4f& padding )
{
	zpMath::Add( m_extent, m_extent, padding );
}

void zpBoundingAABB::add( zp_float x, zp_float y, zp_float z )
{
	add( zpVector4f( x, y, z, 1.0f ) );
}
void zpBoundingAABB::add( const zpScalar& x, const zpScalar& y, const zpScalar& z )
{
	add( zpVector4f( x, y, z ) );
}
void zpBoundingAABB::add( const zpVector4f& point )
{
	zpVector4f& min = getMin();
	zpVector4f& max = getMax();

	zpMath::Max( max, point, max );
	zpMath::Min( min, point, min );

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
	zpMath::Neg( nr, sphere.getRadius() );

	zpMath::Add( min, min, nr );
	zpMath::Add( max, max, sphere.getRadius() );

	add( min );
	add( max );
}