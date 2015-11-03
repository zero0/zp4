#include "zpCommon.h"

zpPhantomComponent::zpPhantomComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_addOnEnable( true )
	, m_addOnCreate( true )
	, m_isAdded( false )
{
	zpPhysicsEngine* engine = getApplication()->getPhysicsEngine();
	zp_short group = engine->getCollisionMask()->getCollisionMask( def[ "Group" ].asCString() );
	zp_short mask =  engine->getCollisionMask()->getCollisionMask( def[ "Mask" ].asCString() );
	zpCollider* collider = engine->getColliderCache()->getCollider( def[ "Collider" ] );
	zpMatrix4f transform = obj->getComponents()->getTransformComponent()->getWorldTransform();

	m_phantom.create( transform, group, mask, collider, engine );
}
zpPhantomComponent::~zpPhantomComponent()
{
	zpPhysicsEngine* engine = getApplication()->getPhysicsEngine();
	engine->getColliderCache()->removeCollider( m_phantom.getCollider() );

	m_phantom.destroy( engine );
}

void zpPhantomComponent::onCollisionEnter( const zpPhantomCollisionHitInfo& hit )
{
	if( !m_eventOnEnter.isEmpty() )
	{
		getApplication()->getEventManager()->sendEvent( m_eventOnEnter, getParentObject() );
	}

	if( !m_messageOnEnter.isEmpty() )
	{

	}
}
void zpPhantomComponent::onCollisionStay( const zpPhantomCollisionHitInfo& hit )
{
	if( !m_eventOnStay.isEmpty() )
	{
		getApplication()->getEventManager()->sendEvent( m_eventOnStay, getParentObject() );
	}

	if( !m_messageOnStay.isEmpty() )
	{

	}
}
void zpPhantomComponent::onCollisionLeave( zp_handle otherObject )
{
	if( !m_eventOnLeave.isEmpty() )
	{
		getApplication()->getEventManager()->sendEvent( m_eventOnLeave, getParentObject() );
	}

	if( !m_messageOnLeave.isEmpty() )
	{

	}
}

void zpPhantomComponent::onCreate()
{

}
void zpPhantomComponent::onInitialize()
{
	if( m_addOnCreate )
	{
		m_phantom.setCollisionCallback( this );
		getApplication()->getPhysicsEngine()->addPhantom( &m_phantom );
		m_isAdded = true;
	}
}
void zpPhantomComponent::onDestroy()
{
	if( m_isAdded )
	{
		getApplication()->getPhysicsEngine()->removePhantom( &m_phantom );
		m_phantom.setCollisionCallback( ZP_NULL );
		m_isAdded = false;
	}
}

void zpPhantomComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
	if( m_isAdded )
	{
		zpMatrix4f transform = getParentObject()->getComponents()->getTransformComponent()->getWorldTransform();
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
		m_phantom.setCollisionCallback( this );
		getApplication()->getPhysicsEngine()->addPhantom( &m_phantom );
		m_isAdded = true;
	}
}
void zpPhantomComponent::onDisabled()
{
	if( m_addOnEnable && m_isAdded )
	{
		getApplication()->getPhysicsEngine()->removePhantom( &m_phantom );
		m_phantom.setCollisionCallback( ZP_NULL );
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
