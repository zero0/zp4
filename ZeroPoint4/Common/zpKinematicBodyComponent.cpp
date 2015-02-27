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

		getParentObject()->getComponents()->getTransformComponent()->setLocalTransform( pos, rot );

		const zpKeyboard* k = getApplication()->getInputManager()->getKeyboard();
		if( k->isKeyDown( ZP_KEY_CODE_W ) )
		{
			m_kinematicBody.setWalkDirection( zpVector4f( 0, 0, 10.f ) );
		}
		else if( k->isKeyDown( ZP_KEY_CODE_S ) )
		{
			m_kinematicBody.setWalkDirection( zpVector4f( 0, 0, -10.f ) );
		}
		else if( k->isKeyDown( ZP_KEY_CODE_A ) )
		{
			m_kinematicBody.setWalkDirection( zpVector4f( 10.f, 0, 0 ) );
		}
		else if( k->isKeyDown( ZP_KEY_CODE_D ) )
		{
			m_kinematicBody.setWalkDirection( zpVector4f( -10.f, 0, 0 ) );
		}
		else if( k->isKeyPressed( ZP_KEY_CODE_SPACE ) )
		{
			m_kinematicBody.jump();
		}
		else if( k->isKeyPressed( ZP_KEY_CODE_R ) )
		{
			m_kinematicBody.warp( zpVector4f( 0, 10, 0 ) );
		}
		else
		{
			m_kinematicBody.setWalkDirection( zpVector4f( 0, 0, 0 ) );
		}
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
