#include "zpCore.h"

zpGameObjectComponent::zpGameObjectComponent() :
	m_parentGameObject( ZP_NULL ),
	m_isEnabled( true ),
	m_name()
{
	m_parent = this;
}
zpGameObjectComponent::~zpGameObjectComponent() {}

void zpGameObjectComponent::setParentGameObject( zpGameObject* parent ) {
	if( parent ) parent->addReference();
	if( m_parentGameObject ) m_parentGameObject->removeReference();
	m_parentGameObject = parent;
}
zpGameObject* zpGameObjectComponent::getParentGameObject() const {
	return m_parentGameObject;
}

void zpGameObjectComponent::setEnabled( zp_bool enabled ) {
	bool wasEnabled = m_isEnabled;
	m_isEnabled = enabled;

	if( wasEnabled && !enabled ) {
		onDisabled();
	} else if( !wasEnabled && enabled ) {
		onEnabled();
	}
}
zp_bool zpGameObjectComponent::isEnabled() const {
	return m_isEnabled;
}

const zpString& zpGameObjectComponent::getName() const {
	return m_name;
}
void zpGameObjectComponent::setName( const zpString& name ) {
	m_name = name;
}