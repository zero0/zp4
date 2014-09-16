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
	m_endPosition = target->getTransform().getRow( 3 );

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

void zpProjectileComponent::onUpdate()
{
	if( m_isRunning )
	{
		zpScalar dt( zpTime::getInstance()->getDeltaSeconds() );
		zpScalar speed( m_speed );
		zpScalar accel( m_acceleration );
		zpScalar angVel( m_angularVelocity );
		zpScalar dist;
		zpVector4f vel;
		zpVector4f pos;
		zpVector4f acc( 0.f );
		zpVector4f ang( 0.f );
		zpVector4f dis;

		zpMatrix4f mat( getParentObject()->getTransform() );
		const zpVector4f& p = mat.getRow( 3 );

		if( m_followTarget && m_endTarget != ZP_NULL )
		{
			const zpVector4f& e = m_endTarget->getTransform().getRow( 3 );

			zpMath::Mul( angVel, angVel, dt );
			zpMath::Sub( ang, m_endPosition, e );
			zpMath::Normalize3( ang, ang );
			zpMath::Mul( ang, ang, angVel );

			m_endPosition = e;
			m_endSphere.setCenter( e );
		}

		zpMath::Mul( speed, speed, dt );

		zpMath::Sub( vel, m_endPosition, p );
		zpMath::Normalize3( vel, vel );
		zpMath::Mul( vel, vel, speed );
		zpMath::Add( m_velocity, vel, ang );

		zpMath::Add( pos, p, m_velocity );

		zpMath::Mul( accel, accel, dt );
		zpMath::Add( speed, speed, accel );

		m_speed = speed.getFloat();

		mat.setRow( 3, pos );
		m_projectileSphere.setCenter( pos );

		getParentObject()->setTransform( mat );

		zpMath::Sub( dis, pos, m_startPosition );
		zpMath::Dot3( dist, dis, dis );

		zpScalar maxRange( m_maxRange );
		zpMath::Mul( maxRange, maxRange, maxRange );

		if( ZP_IS_COLLISION( m_projectileSphere, m_endSphere ) || zpMath::Cmp( dist, maxRange ) > 0 )
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

	const zpVector4f& s = getParentObject()->getTransform().getRow( 3 );

	m_startPosition = s;

	m_projectileSphere.setCenter( m_startPosition );
	m_projectileSphere.setRadius( zpScalar( m_projectileRadius ) );

	m_endSphere.setCenter( m_endPosition );
	m_endSphere.setRadius( zpScalar( m_endRadius ) );

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

void zpProjectileComponentPool::update()
{
	m_used.foreach( []( zpProjectileComponent* o )
	{
		o->update();
	} );
}
