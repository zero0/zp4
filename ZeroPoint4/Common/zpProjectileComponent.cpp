#include "zpCommon.h"

zpProjectileComponent::zpProjectileComponent( zpObject* obj, const zpBison::Value& def )
    : zpComponent( obj )
{

}
zpProjectileComponent::~zpProjectileComponent()
{

}

void zpProjectileComponent::handleEvent( const zpEvent& e, zpObject* sender )
{
    if( m_fireEventHandler.isHandlerForEvent( e ) )
    {
        
    }
}

void zpProjectileComponent::fire( zpObject* target )
{
    m_endTarget = target;
    m_endPosition = target->getComponents()->getTransformComponent()->getWorldPosition();

    onFire();

}
void zpProjectileComponent::fire( const zpVector4f& end )
{
    m_endTarget = ZP_NULL;
    m_endPosition = end;

    onFire();
}

void zpProjectileComponent::onCreate()
{

}
void zpProjectileComponent::onInitialize()
{

}
void zpProjectileComponent::onDestroy()
{

}

void zpProjectileComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
    if( m_isRunning )
    {
        zpScalar dt     = zpMath::Scalar( deltaTime );
        zpScalar speed  = zpMath::Scalar( m_speed );
        zpScalar accel  = zpMath::Scalar( m_acceleration );
        zpScalar angVel = zpMath::Scalar( m_angularVelocity );
        zpScalar dist;
        zpVector4f vel;
        zpVector4f pos;
        zpVector4f acc = zpMath::Vector4( 0, 0, 0, 0 );
        zpVector4f ang = zpMath::Vector4( 0, 0, 0, 0 );
        zpVector4f dis;

        zpMatrix4f mat( getParentObject()->getComponents()->getTransformComponent()->getWorldTransform() );
        zpVector4f p = mat.r[ 3 ];

        if( m_followTarget && m_endTarget != ZP_NULL )
        {
            const zpVector4f& e = m_endTarget->getComponents()->getTransformComponent()->getWorldPosition();

            angVel = zpMath::Vector4Mul( angVel, dt );
            ang = zpMath::Vector4Sub( m_endPosition, e );
            ang = zpMath::Vector4Normalize3( ang );
            ang = zpMath::Vector4Mul( ang, angVel );

            m_endPosition = e;
            m_endSphere.setCenter( e );
        }

        speed = zpMath::ScalarMul( speed, dt );

        vel = zpMath::Vector4Sub( m_endPosition, p );
        vel = zpMath::Vector4Normalize3( vel );
        vel = zpMath::Vector4Mul( vel, speed );
        m_velocity = zpMath::Vector4Add( vel, ang );

        pos = zpMath::Vector4Add( p, m_velocity );

        accel = zpMath::Vector4Mul( accel, dt );
        speed = zpMath::Vector4Add( speed, accel );

        m_speed = zpMath::AsFloat( speed );

        mat.r[ 3 ] = pos;
        m_projectileSphere.setCenter( pos );

        getParentObject()->getComponents()->getTransformComponent()->setLocalPosition( pos );

        dis =  zpMath::Vector4Sub( pos, m_startPosition );
        dist = zpMath::Vector4Dot3( dis, dis );

        zpScalar maxRange = zpMath::Scalar( m_maxRange );
        maxRange = zpMath::ScalarMul( maxRange, maxRange );

        if( ZP_IS_COLLISION( m_projectileSphere, m_endSphere ) || zpMath::ScalarCmp( dist, maxRange ) > 0 )
        {
            projectileCollided();
        }
    }
}
void zpProjectileComponent::onSimulate()
{

}

void zpProjectileComponent::onEnabled()
{

}
void zpProjectileComponent::onDisabled()
{

}

void zpProjectileComponent::onFire()
{
    m_isRunning = true;

    const zpVector4f& s = getParentObject()->getComponents()->getTransformComponent()->getWorldPosition();

    m_startPosition = s;

    m_projectileSphere.setCenter( m_startPosition );
    m_projectileSphere.setRadius( zpMath::Scalar( m_projectileRadius ) );

    m_endSphere.setCenter( m_endPosition );
    m_endSphere.setRadius( zpMath::Scalar( m_endRadius ) );

    m_velocity = m_initialVelocity;
}

void zpProjectileComponent::projectileCollided()
{
    m_isRunning = false;

    if( !m_eventOnCollide.isEmpty() )
    {
        getApplication()->getEventManager()->sendEvent( m_eventOnCollide, m_destroyOnCollide ? ZP_NULL : getParentObject() );
    }

    if( m_destroyOnCollide )
    {
        getParentObject()->destroy();
    }
    else
    {
        getParentObject()->setEnabled( false );
    }
}

zpProjectileComponentPool::zpProjectileComponentPool()
{
}
zpProjectileComponentPool::~zpProjectileComponentPool()
{
}

void zpProjectileComponentPool::update( zp_float deltaTime, zp_float realTime )
{
    m_used.foreach( [ &deltaTime, &realTime ]( zpProjectileComponent* o )
    {
        o->update( deltaTime, realTime );
    } );
}
