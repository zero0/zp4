#include "zpCore.h"

zpBoundingAABB::zpBoundingAABB()
	: m_min( ZP_FLT_MAX )
	, m_max( ZP_FLT_MIN )
{}
zpBoundingAABB::zpBoundingAABB( zp_float width, zp_float height, zp_float depth )
	: m_min( width * -0.5f, height * -0.5f, depth * -0.5f )
	, m_max( width * 0.5f, height * 0.5f, depth * 0.5f )
{}
zpBoundingAABB::zpBoundingAABB( const zpVector4f& min, const zpVector4f& max )
	: m_min( min )
	, m_max( max )
{}
zpBoundingAABB::zpBoundingAABB( const zpBoundingAABB& box )
	: m_min( box.m_min )
	, m_max( box.m_max )
{}
zpBoundingAABB::zpBoundingAABB( zpBoundingAABB&& box )
	: m_min( box.m_min )
	, m_max( box.m_max )
{}
zpBoundingAABB::~zpBoundingAABB()
{}

void zpBoundingAABB::operator=( const zpBoundingAABB& box )
{
	m_min = box.m_min;
	m_max = box.m_max;
}
void zpBoundingAABB::operator=( zpBoundingAABB&& box )
{
	m_min = box.m_min;
	m_max = box.m_max;
}

const zpVector4f& zpBoundingAABB::getMin() const
{
	return m_min;
}
const zpVector4f& zpBoundingAABB::getMax() const
{
	return m_max;
}
void zpBoundingAABB::setMin( const zpVector4f& min )
{
	m_min = min;
}
void zpBoundingAABB::setMax( const zpVector4f& max )
{
	m_max = max;
}

void zpBoundingAABB::getCenter( zpVector4f& outCenter ) const
{
	zpMath::Add( outCenter, m_min, m_max );
	zpMath::Mul( outCenter, outCenter, zpScalar( 0.5f ) );
}
void zpBoundingAABB::setCenter( const zpVector4f& center )
{
	zpVector4f t;
	zpVector4f c;
	getCenter( c );

	zpMath::Sub( t, center, c );

	translate( t );
}
void zpBoundingAABB::getExtents( zpVector4f& outExtense ) const
{
	zpMath::Sub( outExtense, m_max, m_min );
	zpMath::Mul( outExtense, outExtense, zpScalar( 0.5f ) );
}

void zpBoundingAABB::generateBoundingSphere( zpBoundingSphere& sphere, zp_bool isSphereContained ) const
{
	zpScalar r, a, b;
	zpVector4f center;
	getCenter( center );

	if( isSphereContained )
	{
		zpMath::Sub( a, m_max.getX(), m_min.getX() );
		zpMath::Sub( b, m_max.getY(), m_min.getY() );

		zpMath::Min( r, a, b );

		zpMath::Sub( a, m_max.getZ(), m_min.getZ() );
		zpMath::Min( r, a, r );

		zpMath::Mul( r, r, zpScalar( 0.5f ) );
	}
	else
	{
		zpVector4f maxLength( m_max );
		zpVector4f minLength( m_min );

		zpMath::Sub( maxLength, maxLength, center );
		zpMath::Sub( minLength, minLength, center );
		
		zpMath::Length3( a, minLength );
		zpMath::Length3( b, maxLength );

		zpMath::Max( r, a, b );
	}

	sphere.setCenter( center );
	sphere.setRadius( r );
}

zpScalar zpBoundingAABB::getWidth() const
{
	zpScalar s;
	zpMath::Sub( s, m_max.getX(), m_min.getX() );
	return s;
}
zpScalar zpBoundingAABB::getHeight() const
{
	zpScalar s;
	zpMath::Sub( s, m_max.getY(), m_min.getY() );
	return s;
}
zpScalar zpBoundingAABB::getDepth() const
{
	zpScalar s;
	zpMath::Sub( s, m_max.getZ(), m_min.getZ() );
	return s;
}

void zpBoundingAABB::translate( const zpVector4f& translate )
{
	zpMath::Add( m_min, m_min, translate );
	zpMath::Add( m_max, m_max, translate );
}
void zpBoundingAABB::scale( const zpScalar& scale )
{
	zpMath::Mul( m_min, m_min, scale );
	zpMath::Mul( m_max, m_max, scale );
}
void zpBoundingAABB::scale( const zpVector4f& scale )
{
	zpMath::Mul( m_min, m_min, scale );
	zpMath::Mul( m_max, m_max, scale );
}
void zpBoundingAABB::pad( const zpScalar& padding )
{
	zpMath::Sub( m_min, m_min, padding );
	zpMath::Add( m_max, m_max, padding );
}
void zpBoundingAABB::pad( const zpVector4f& padding )
{
	zpMath::Sub( m_min, m_min, padding );
	zpMath::Add( m_max, m_max, padding );
}

void zpBoundingAABB::add( zp_float x, zp_float y, zp_float z )
{
	add( zpScalar( x ), zpScalar( y ), zpScalar( z ) );
}
void zpBoundingAABB::add( const zpScalar& x, const zpScalar& y, const zpScalar& z )
{
	zpScalar r;

	// x
	zpMath::Max( r, x, m_max.getX() );
	m_max.setX( r );

	zpMath::Min( r, x, m_min.getX() );
	m_min.setX( r );

	// y
	zpMath::Max( r, y, m_max.getY() );
	m_max.setY( r );

	zpMath::Min( r, y, m_min.getY() );
	m_min.setY( r );

	// z
	zpMath::Max( r, z, m_max.getZ() );
	m_max.setZ( r );

	zpMath::Min( r, z, m_min.getZ() );
	m_min.setZ( r );
}
void zpBoundingAABB::add( const zpVector4f& point )
{
	add( point.getX(), point.getY(), point.getZ() );
}
void zpBoundingAABB::add( const zpBoundingAABB& box )
{
	add( box.m_min );
	add( box.m_max );
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