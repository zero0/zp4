#include "zpInput.h"

zpInputManager::zpInputManager() :
	m_keyboard( ZP_NULL )
{
	zp_zero_memory_array( m_controllers );
}
zpInputManager::~zpInputManager() {}

zpKeyboard* zpInputManager::getKeyboard() {
	return m_keyboard;
}
zpController* zpInputManager::getController( zpControllerNumber controller ) {
	if( !m_controllers[ controller ] ) {
		m_controllers[ controller ] = new zpController( controller );
	}
	return m_controllers[ controller ];
}

void zpInputManager::onCreate() {}
void zpInputManager::onDestroy() {}

void zpInputManager::onUpdate() {
	for( zp_uint i = 0; i < 4; ++i ) {
		if( m_controllers[i] ) m_controllers[i]->poll();
	}
}

void zpInputManager::onEnabled() {}
void zpInputManager::onDisabled() {}

void zpInputManager::serialize( zpSerializedOutput* out ) {}
void zpInputManager::deserialize( zpSerializedInput* in ) {}