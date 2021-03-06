#include "zpCore.h"

zpRay::zpRay()
    : m_origin()
    , m_direction()
{}
zpRay::zpRay( zpVector4fParamF origin, zpVector4fParamF direction )
    : m_origin( origin )
    , m_direction( direction )
{
    m_direction = zpMath::Vector4Normalize3( m_direction );
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

zpVector4f zpRay::getOrigin() const
{
    return m_origin;
}
void zpRay::setOrigin( zpVector4fParamF origin )
{
    m_origin = origin;
}

zpVector4f zpRay::getDirection() const
{
    return m_direction;
}
void zpRay::setDirection( zpVector4fParamF direction )
{
    m_direction = zpMath::Vector4Normalize3( direction );
}

zpVector4f zpRay::getPointAt( zpScalarParamF t ) const
{
    zpVector4f p;
    p = zpMath::Vector4Madd( m_origin, m_direction, t );
    return p;
}
zp_bool zpRay::isPointOnRay( zpVector4fParamF point ) const
{
    zpVector4f r;
    r = zpMath::Vector4Sub( point, m_origin );
    r = zpMath::Vector4Normalize3( r );

    zpScalar d;
    d = zpMath::Vector4Dot3( r, m_direction );

    zp_int c;
    c = zpMath::ScalarCmp( d, zpMath::Scalar( 1.0f ) );
    return c == 0;
}