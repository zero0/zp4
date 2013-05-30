#include "zpCommon.h"

zpComponent::zpComponent()
	: m_flags( 0 )
	, m_parentGameObject( ZP_NULL )
{}
zpComponent::~zpComponent()
{
	destroy();
}

void zpComponent::setParentGameObject( zpGameObject* parent )
{
	m_parentGameObject = parent;
}
zpGameObject* zpComponent::getParentGameObject() const
{
	return m_parentGameObject;
}

void zpComponent::setEnabled( zp_bool enabled )
{
	zp_bool wasEnabled = m_flags.isMarked( ZP_COMPONENT_FLAG_ENABLED );
	m_flags.setMarked( ZP_COMPONENT_FLAG_ENABLED, enabled );

	if( wasEnabled && !enabled ) {
		onDisabled();
	} else if( !wasEnabled && enabled ) {
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
	if( m_flags.isMarked( ZP_COMPONENT_FLAG_CREATED ) ) return;
	m_flags.mark( ZP_COMPONENT_FLAG_CREATED );
	onCreate();
}
void zpComponent::destroy()
{
	if( !m_flags.isMarked( ZP_COMPONENT_FLAG_CREATED ) ) return;
	m_flags.unmark( ZP_COMPONENT_FLAG_CREATED );
	onDestroy();
}

void zpComponent::sendMessageToParentGameObject( const zpMessage& message )
{
	m_parentGameObject->receiveMessage( message );
}
void zpComponent::sendMessageToSiblingComponents( const zpMessage& message )
{
	m_parentGameObject->getComponents()->receiveMessage( message );
}

zpWorld* zpComponent::getWorld() const
{
	return ZP_NULL; //return m_parentGameObject->getWorld();
}
zpGame* zpComponent::getGame() const
{
	return ZP_NULL; //return m_parentGameObject->getWorld()->getGame();
}
