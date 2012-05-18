#include "zpCore.h"

zpGameManager::zpGameManager() {}
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