#include "zpScripting.h"
#include "sqrat.h"

zpScriptingManager::zpScriptingManager() {}
zpScriptingManager::~zpScriptingManager() {}

void zpScriptingManager::receiveMessage( const zpMessage& message ) {}

void zpScriptingManager::serialize( zpSerializedOutput* out ) {}
void zpScriptingManager::deserialize( zpSerializedInput* in ) {}

void zpScriptingManager::onCreate() {
	m_vm = (zp_ptr)sq_open( 512 );
	Sqrat::DefaultVM().Set( (HSQUIRRELVM)m_vm );
}
void zpScriptingManager::onDestroy() {
	sq_close( (HSQUIRRELVM)m_vm );
}

void zpScriptingManager::onUpdate() {}

void zpScriptingManager::onEnabled() {}
void zpScriptingManager::onDisabled() {}