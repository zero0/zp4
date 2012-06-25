#include "zpCore.h"

zpComponent::zpComponent() :
	m_parentGameObject( ZP_NULL ),
	m_isEnabled( true ),
	m_isCreated( false ),
	m_referenceCount( 1 ),
	m_isMarkedForAutoDelete( false ),
	m_name()
{}
zpComponent::~zpComponent() {
	destroy();
}

void zpComponent::setParentGameObject( zpGameObject* parent ) {
	if( parent ) parent->addReference();
	if( m_parentGameObject ) m_parentGameObject->removeReference();
	m_parentGameObject = parent;
}
zpGameObject* zpComponent::getParentGameObject() const {
	return m_parentGameObject;
}

void zpComponent::setEnabled( zp_bool enabled ) {
	bool wasEnabled = m_isEnabled;
	m_isEnabled = enabled;

	if( wasEnabled && !enabled ) {
		onDisabled();
	} else if( !wasEnabled && enabled ) {
		onEnabled();
	}
}
zp_bool zpComponent::isEnabled() const {
	return m_isEnabled;
}
zp_bool zpComponent::isCreated() const {
	return m_isCreated;
}

void zpComponent::update() {
	if( m_isEnabled && m_isCreated ) onUpdate();
}

void zpComponent::create() {
	if( m_isCreated ) return;
	m_isCreated = true;
	onCreate();
}
void zpComponent::destroy() {
	if( !m_isCreated ) return;
	m_isCreated = false;
	onDestroy();
}

const zpString& zpComponent::getName() const {
	return m_name;
}
void zpComponent::setName( const zpString& name ) {
	m_name = name;
}

void zpComponent::sendMessageToParentGameObject( const zpMessage& message ) {
	m_parentGameObject->receiveMessage( message );
}
void zpComponent::sendMessageToSiblingComponents( const zpMessage& message ) {
	const zpComponent* self = this;
	m_parentGameObject->getComponents()->foreach( [ &self, &message ]( zpComponent* goc ){
		if( goc != self ) goc->receiveMessage( message );
	} );
}

zpWorld* zpComponent::getWorld() const {
	return m_parentGameObject->getWorld();
}
zpGame* zpComponent::getGame() const {
	return m_parentGameObject->getWorld()->getGame();
}

void zpComponent::addReference() const {
	++m_referenceCount;
}
zp_bool zpComponent::removeReference() const {
	--m_referenceCount;
	if( m_referenceCount == 0 ) {
		if( m_isMarkedForAutoDelete ) delete this;
		return true;
	}
	return false;
}

zp_uint zpComponent::getReferenceCount() const {
	return m_referenceCount;
}

void zpComponent::markForAutoDelete( zp_bool marked ) const {
	m_isMarkedForAutoDelete = marked;
}
zp_bool zpComponent::isMarkedForAutoDelete() const {
	return m_isMarkedForAutoDelete;
}