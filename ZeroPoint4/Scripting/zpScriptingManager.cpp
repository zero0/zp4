#include "zpScripting.h"
#include "angelscript.h"



zpScriptingManager::zpScriptingManager() {}
zpScriptingManager::~zpScriptingManager() {}

void zpScriptingManager::receiveMessage( const zpMessage& message ) {}

void zpScriptingManager::serialize( zpSerializedOutput* out ) {}
void zpScriptingManager::deserialize( zpSerializedInput* in ) {}

void zpScriptingManager::onCreate() {
	zpAngelScript::createInstance();
}
void zpScriptingManager::onDestroy() {
	zpAngelScript::destroyInstance();
}

void zpScriptingManager::onUpdate() {}

void zpScriptingManager::onEnabled() {}
void zpScriptingManager::onDisabled() {}