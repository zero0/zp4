#include "zpCommon.h"

zpPhantomComponent::zpPhantomComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_addOnEnable( true )
	, m_addOnCreate( true )
	, m_isAdded( false )
{
	m_phantom.create( getApplication()->getPhysicsEngine(), obj->getComponents()->getTransformComponent()->getWorldTransform(), def );
}
zpPhantomComponent::~zpPhantomComponent()
{
	m_phantom.destroy( getApplication()->getPhysicsEngine() );
}

void zpPhantomComponent::onCreate()
{

}
void zpPhantomComponent::onInitialize()
{
	if( m_addOnCreate )
	{
		getParentObject()->getApplication()->getPhysicsEngine()->addPhantom( &m_phantom );
		m_isAdded = true;
	}
}
void zpPhantomComponent::onDestroy()
{
	if( m_isAdded )
	{
		getParentObject()->getApplication()->getPhysicsEngine()->removePhantom( &m_phantom );
		m_isAdded = false;
	}
}

void zpPhantomComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
	if( m_isAdded )
	{
		const zpMatrix4f& transform = getParentObject()->getComponents()->getTransformComponent()->getWorldTransform();
		m_phantom.setMatrix( transform );
	}
}
void zpPhantomComponent::onSimulate()
{

}

void zpPhantomComponent::onEnabled()
{
	if( m_addOnEnable && !m_isAdded )
	{
		getParentObject()->getApplication()->getPhysicsEngine()->addPhantom( &m_phantom );
		m_isAdded = true;
	}
}
void zpPhantomComponent::onDisabled()
{
	if( m_addOnEnable && m_isAdded )
	{
		getParentObject()->getApplication()->getPhysicsEngine()->removePhantom( &m_phantom );
		m_isAdded = false;
	}
}


zpPhantomComponentPool::zpPhantomComponentPool()
{

}
zpPhantomComponentPool::~zpPhantomComponentPool()
{

}

void zpPhantomComponentPool::update( zp_float deltaTime, zp_float realTime )
{
	m_used.foreach( [ &deltaTime, &realTime ]( zpPhantomComponent* o )
	{
		o->update( deltaTime, realTime );
	} );
}
