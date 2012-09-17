#include "zpInput.h"

zpInputManager::zpInputManager()
	: m_keyboard( ZP_NULL )
	, m_mouse( ZP_NULL )
{
	zp_zero_memory_array( m_controllers );
}
zpInputManager::~zpInputManager() {}

zpKeyboard* zpInputManager::getKeyboard() {
	if( m_keyboard == ZP_NULL ) {
		m_keyboard = new zpKeyboard;
		getGame()->getWindow()->addFocusListener( m_keyboard );
		getGame()->getWindow()->addProcListener( m_keyboard );
	}
	return m_keyboard;
}
zpMouse* zpInputManager::getMouse() {
	if( m_mouse == ZP_NULL ) {
		m_mouse = new zpMouse;
		getGame()->getWindow()->addFocusListener( m_mouse );
		getGame()->getWindow()->addProcListener( m_mouse );
	}
	return m_mouse;
}
zpController* zpInputManager::getController( zpControllerNumber controller ) {
	if( !m_controllers[ controller ] ) {
		m_controllers[ controller ] = new zpController( controller );
		getGame()->getWindow()->addFocusListener( m_controllers[ controller ] );
	}
	return m_controllers[ controller ];
}

void zpInputManager::onCreate() {}
void zpInputManager::onDestroy() {}

void zpInputManager::onUpdate() {
	if( m_keyboard ) m_keyboard->poll();

	if( m_mouse ) m_mouse->poll();

	for( zp_uint i = 0; i < 4; ++i ) {
		if( m_controllers[i] ) m_controllers[i]->poll();
	}
}

void zpInputManager::onEnabled() {}
void zpInputManager::onDisabled() {}

void zpInputManager::serialize( zpSerializedOutput* out ) {}
void zpInputManager::deserialize( zpSerializedInput* in ) {}

void zpInputManager::receiveMessage( const zpMessage& message ) {}