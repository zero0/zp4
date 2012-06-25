#include "zpCore.h"

zpGameManager::zpGameManager() :
	m_isEnabled( true ),
	m_isCreated( false ),
	m_referenceCount( 1 ),
	m_isMarkedForAutoDelete( false ),
	m_game( ZP_NULL )
{}
zpGameManager::~zpGameManager() {}

void zpGameManager::create() {
	if( m_isCreated ) return;
	m_isCreated = true;
	
	onCreate();
}
void zpGameManager::destroy() {
	if( !m_isCreated ) return;
	m_isCreated = false;

	onDestroy();
}

void zpGameManager::update() {
	if( m_isEnabled && m_isCreated ) onUpdate();
}

void zpGameManager::setEnabled( zp_bool enabled ) {
	bool wasEnabled = m_isEnabled;
	m_isEnabled = enabled;

	if( wasEnabled && !enabled ) {
		onDisabled();
	} else if( !wasEnabled && enabled ) {
		onEnabled();
	}
}
zp_bool zpGameManager::isEnabled() const {
	return m_isEnabled;
}

void zpGameManager::setGame( zpGame* game ) {
	m_game = game;
}
zpGame* zpGameManager::getGame() const {
	return m_game;
}

void zpGameManager::addReference() const {
	++m_referenceCount;
}
zp_bool zpGameManager::removeReference() const {
	--m_referenceCount;
	if( m_referenceCount == 0 ) {
		if( m_isMarkedForAutoDelete ) delete this;
		return true;
	}
	return false;
}

zp_uint zpGameManager::getReferenceCount() const {
	return m_referenceCount;
}

void zpGameManager::markForAutoDelete( zp_bool marked ) const {
	m_isMarkedForAutoDelete = marked;
}
zp_bool zpGameManager::isMarkedForAutoDelete() const {
	return m_isMarkedForAutoDelete;
}