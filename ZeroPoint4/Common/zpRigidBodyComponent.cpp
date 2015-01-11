#include "zpCommon.h"

zpRigidBodyComponent::zpRigidBodyComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_addOnEnable( true )
	, m_addOnCreate( true )
	, m_isAdded( false )
{
	m_rigidBody.create( def, obj->isFlagSet( ZP_OBJECT_FLAG_STATIC ) );
}
zpRigidBodyComponent::~zpRigidBodyComponent()
{
	m_rigidBody.destroy();
}

void zpRigidBodyComponent::onCreate()
{
}
void zpRigidBodyComponent::onInitialize()
{
	m_rigidBody.initialize( getParentObject()->getComponents()->getTransformComponent()->getLocalTransform() );
	if( m_addOnCreate )
	{
		getParentObject()->getApplication()->getPhysicsEngine()->addRigidBody( &m_rigidBody );
		m_isAdded = true;
	}
}
void zpRigidBodyComponent::onDestroy()
{
	if( m_isAdded )
	{
		getParentObject()->getApplication()->getPhysicsEngine()->removeRigidBody( &m_rigidBody );
		m_isAdded = false;
	}
}

void zpRigidBodyComponent::onUpdate()
{
	if( m_isAdded && !m_rigidBody.isStatic() )
	{
		zpVector4f pos;
		zpQuaternion4f rot;
		m_rigidBody.getPositionRotation( pos, rot );
	
		getParentObject()->getComponents()->getTransformComponent()->setLocalTransform( pos, rot );
	}
}
void zpRigidBodyComponent::onSimulate()
{

}

void zpRigidBodyComponent::onEnabled()
{
	if( m_addOnEnable && !m_isAdded )
	{
		getParentObject()->getApplication()->getPhysicsEngine()->addRigidBody( &m_rigidBody );
		m_isAdded = true;
	}
}
void zpRigidBodyComponent::onDisabled()
{
	if( m_addOnEnable && m_isAdded )
	{
		getParentObject()->getApplication()->getPhysicsEngine()->removeRigidBody( &m_rigidBody );
		m_isAdded = false;
	}
}


zpRigidBodyComponentPool::zpRigidBodyComponentPool() {}
zpRigidBodyComponentPool::~zpRigidBodyComponentPool() {}

void zpRigidBodyComponentPool::update()
{
	m_used.foreach( []( zpRigidBodyComponent* o )
	{
		o->update();
	} );
}
void zpRigidBodyComponentPool::simulate()
{
	m_used.foreach( []( zpRigidBodyComponent* o )
	{
		o->simulate();
	} );
}
