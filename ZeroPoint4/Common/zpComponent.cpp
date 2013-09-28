#include "zpCommon.h"

zpComponent::zpComponent( zpObject* obj )
	: m_flags( 0 )
	, m_parentObject( obj )
{}
zpComponent::~zpComponent()
{
	destroy();
}

zpObject* zpComponent::getParentObject() const
{
	return m_parentObject;
}

void zpComponent::setEnabled( zp_bool enabled )
{
	zp_bool wasEnabled = m_flags.isMarked( ZP_COMPONENT_FLAG_ENABLED );
	m_flags.setMarked( ZP_COMPONENT_FLAG_ENABLED, enabled );

	if( wasEnabled && !enabled )
	{
		onDisabled();
	}
	else if( !wasEnabled && enabled )
	{
		onEnabled();
	}
}
zp_bool zpComponent::isEnabled() const
{
	return m_flags.isMarked( ZP_COMPONENT_FLAG_ENABLED );
}
zp_bool zpComponent::isCreated() const
{
	return m_flags.isMarked( ZP_COMPONENT_FLAG_CREATED );
}

void zpComponent::update()
{
	if( m_flags.isAllMarked( 1 << ZP_COMPONENT_FLAG_CREATED | 1 << ZP_COMPONENT_FLAG_ENABLED ) )
	{
		onUpdate();
	}
}
void zpComponent::simulate()
{
	if( m_flags.isAllMarked( 1 << ZP_COMPONENT_FLAG_CREATED | 1 << ZP_COMPONENT_FLAG_ENABLED ) )
	{
		onSimulate();
	}
}

void zpComponent::create()
{
	if( !m_flags.isMarked( ZP_COMPONENT_FLAG_CREATED ) )
	{
		m_flags.mark( ZP_COMPONENT_FLAG_CREATED );
		onCreate();
	}
}
void zpComponent::initialize()
{
	if( m_flags.isMarked( ZP_COMPONENT_FLAG_CREATED ) )
	{
		onInitialize();
	}
}
void zpComponent::destroy()
{
	if( m_flags.isMarked( ZP_COMPONENT_FLAG_CREATED ) )
	{
		m_flags.unmark( ZP_COMPONENT_FLAG_CREATED );
		onDestroy();
	}
}

void zpComponent::sendMessageToParentGameObject( const zpMessage& message )
{
	//m_parentGameObject->receiveMessage( message );
}
void zpComponent::sendMessageToSiblingComponents( const zpMessage& message )
{
	//m_parentObject->getComponents()->receiveMessage( message );
}

zpWorld* zpComponent::getWorld() const
{
	return ZP_NULL; //return m_parentGameObject->getWorld();
}
zpApplication* zpComponent::getApplication() const
{
	return ZP_NULL; //return m_parentGameObject->getWorld()->getGame();
}
