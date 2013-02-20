#include "zpCore.h"

zpRay::zpRay()
	: m_origin()
	, m_direction()
{}
zpRay::zpRay( const zpVector4f& origin, const zpVector4f& direction )
	: m_origin( origin )
	, m_direction( direction )
{
	zpMath::Normalize3( m_direction, m_direction );
}
zpRay::zpRay( const zpRay& ray )
	: m_origin( ray.m_origin )
	, m_direction( ray.m_direction )
{}
zpRay::zpRay( zpRay&& ray )
	: m_origin( ray.m_origin )
	, m_direction( ray.m_direction )
{}
zpRay::~zpRay() {}

void zpRay::operator=( const zpRay& ray )
{
	m_origin = ray.m_origin;
	m_direction = ray.m_direction;
}
void zpRay::operator=( zpRay&& ray )
{
	m_origin = ray.m_origin;
	m_direction = ray.m_direction;
}

const zpVector4f& zpRay::getOrigin() const
{
	return m_origin;
}
void zpRay::setOrigin( const zpVector4f& origin )
{
	m_origin = origin;
}

const zpVector4f& zpRay::getDirection() const
{
	return m_direction;
}
void zpRay::setDirection( const zpVector4f& direction )
{
	zpMath::Normalize3( m_direction, direction );
}

zpVector4f zpRay::getPointAt( const zpScalar& t ) const
{
	zpVector4f p;
	zpMath::Madd( p, m_origin, m_direction, t );
	return p;
}
zp_bool zpRay::isPointOnRay( const zpVector4f& point ) const
{
	zpVector4f r;
	zpMath::Sub( r, point, m_origin );
	zpMath::Normalize3( r, r );

	zpScalar d;
	zpMath::Dot3( d, r, m_direction );

	zp_int c;
	zpMath::Cmp( c, d, zpScalar( 1.0f ) );
	return c == 0;
}