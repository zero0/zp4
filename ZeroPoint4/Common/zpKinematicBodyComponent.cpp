#include "zpCommon.h"

zpKinematicBodyComponent::zpKinematicBodyComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_addOnCreate( true )
	, m_addOnEnable( true )
	, m_isAdded( false )
{
	m_kinematicBody.create( getApplication()->getPhysicsEngine(), def );
}
zpKinematicBodyComponent::~zpKinematicBodyComponent()
{
	m_kinematicBody.destroy( getApplication()->getPhysicsEngine() );
}

zp_bool zpKinematicBodyComponent::canJump() const
{
	return m_kinematicBody.canJump();
}

void zpKinematicBodyComponent::jump()
{
	m_kinematicBody.jump();
}
void zpKinematicBodyComponent::walk( const zpVector4f& direction, const zpScalar& speed )
{
	zpVector4f dir;
	dir = zpMath::Vector4Scale( direction, speed );

	m_kinematicBody.setWalkDirection( dir );
}
void zpKinematicBodyComponent::stop()
{
	m_kinematicBody.setWalkDirection( zpMath::Vector4( 0, 0, 0, 0 ) );
}

void zpKinematicBodyComponent::onCreate()
{

}
void zpKinematicBodyComponent::onInitialize()
{
	m_kinematicBody.initialize( getParentObject()->getComponents()->getTransformComponent()->getLocalTransform() );
	if( m_addOnCreate )
	{
		getParentObject()->getApplication()->getPhysicsEngine()->addKinematicBody( &m_kinematicBody );
		m_isAdded = true;
	}
}
void zpKinematicBodyComponent::onDestroy()
{
	if( m_isAdded )
	{
		getParentObject()->getApplication()->getPhysicsEngine()->removeKinematicBody( &m_kinematicBody );
		m_isAdded = false;
	}
}

void zpKinematicBodyComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
	if( m_isAdded )
	{
		zpVector4f pos;
		zpQuaternion4f rot;
		m_kinematicBody.getPositionRotation( pos, rot );

		zpTransformComponent* t = getParentObject()->getComponents()->getTransformComponent();
		t->setLocalTransform( pos, rot );
	}
}
void zpKinematicBodyComponent::onSimulate()
{

}

void zpKinematicBodyComponent::onEnabled()
{
	if( m_addOnEnable && !m_isAdded )
	{
		getParentObject()->getApplication()->getPhysicsEngine()->addKinematicBody( &m_kinematicBody );
		m_isAdded = true;
	}
}
void zpKinematicBodyComponent::onDisabled()
{
	if( m_addOnEnable && m_isAdded )
	{
		getParentObject()->getApplication()->getPhysicsEngine()->removeKinematicBody( &m_kinematicBody );
		m_isAdded = false;
	}
}



zpKinematicBodyComponentPool::zpKinematicBodyComponentPool()
{

}
zpKinematicBodyComponentPool::~zpKinematicBodyComponentPool()
{

}

void zpKinematicBodyComponentPool::update( zp_float deltaTime, zp_float realTime )
{
	m_used.foreach( [ &deltaTime, &realTime ]( zpKinematicBodyComponent* o )
	{
		o->update( deltaTime, realTime );
	} );
}
void zpKinematicBodyComponentPool::simulate()
{
	m_used.foreach( []( zpKinematicBodyComponent* o )
	{
		o->simulate();
	} );
}
