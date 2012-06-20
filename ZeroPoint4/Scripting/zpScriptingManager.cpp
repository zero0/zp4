#include "zpScripting.h"
#include "angelscript.h"

zpScriptingManager::zpScriptingManager() {}
zpScriptingManager::~zpScriptingManager() {}

void zpScriptingManager::receiveMessage( const zpMessage& message ) {}

void zpScriptingManager::serialize( zpSerializedOutput* out ) {}
void zpScriptingManager::deserialize( zpSerializedInput* in ) {}

void zpScriptingManager::onCreate() {
	zpAngelScript::createInstance();
	zpAngelScript::getInstance()->SetMessageCallback( asMETHOD( zpScriptingManager, messageCallback ), this, asCALL_THISCALL );
}
void zpScriptingManager::onDestroy() {
	zpAngelScript::destroyInstance();
}

void zpScriptingManager::onUpdate() {}

void zpScriptingManager::onEnabled() {}
void zpScriptingManager::onDisabled() {}

void zpScriptingManager::messageCallback( const asSMessageInfo& msg ) {
	zp_printfln( "%s (%d, %d) [%s]: %s", msg.section, msg.row, msg.col, msg.type == asMSGTYPE_ERROR ? "ERR " : msg.type == asMSGTYPE_WARNING ? "WARN" : "INFO", msg.message );
}