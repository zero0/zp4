#include "zpCommon.h"

zpKinematicBodyComponent::zpKinematicBodyComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_addOnCreate( true )
	, m_addOnEnable( true )
	, m_isAdded( false )
{
	m_kinematicBody.create( obj->getTransform(), def );
}
zpKinematicBodyComponent::~zpKinematicBodyComponent()
{
	m_kinematicBody.destroy();
}
	
void zpKinematicBodyComponent::onCreate()
{

}
void zpKinematicBodyComponent::onInitialize()
{
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

void zpKinematicBodyComponent::onUpdate()
{
	zpMatrix4f mat;
	if( m_isAdded && m_kinematicBody.getMatrix( mat ) )
	{
		getParentObject()->setTransform( mat );
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

void zpKinematicBodyComponentPool::update()
{
	m_used.foreach( []( zpKinematicBodyComponent* o )
	{
		o->update();
	} );
}
void zpKinematicBodyComponentPool::simulate()
{
	m_used.foreach( []( zpKinematicBodyComponent* o )
	{
		o->update();
	} );
}
