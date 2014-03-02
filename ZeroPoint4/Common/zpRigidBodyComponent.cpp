#include "zpCommon.h"

zpRigidBodyComponent::zpRigidBodyComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_addOnEnable( true )
	, m_addOnCreate( true )
{
	m_rigidBody.create( obj->getTransform(), def );
}
zpRigidBodyComponent::~zpRigidBodyComponent()
{
	m_rigidBody.destroy();
}

void zpRigidBodyComponent::onCreate()
{
	getParentObject()->getApplication()->getPhysicsEngine()->addRigidBody( &m_rigidBody );
}
void zpRigidBodyComponent::onInitialize()
{

}
void zpRigidBodyComponent::onDestroy()
{
	getParentObject()->getApplication()->getPhysicsEngine()->removeRigidBody( &m_rigidBody );
}

void zpRigidBodyComponent::onUpdate()
{
	zpMatrix4f transform;
	if( m_rigidBody.getMatrix( transform ) )
	{
		getParentObject()->setTransform( transform );
	}
}
void zpRigidBodyComponent::onSimulate()
{

}

void zpRigidBodyComponent::onEnabled()
{

}
void zpRigidBodyComponent::onDisabled()
{

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
