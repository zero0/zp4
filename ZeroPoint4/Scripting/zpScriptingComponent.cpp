#include "zpScripting.h"
#include "angelscript.h"

zpScriptingComponent::zpScriptingComponent() : m_scriptInstance( ZP_NULL ) {}
zpScriptingComponent::~zpScriptingComponent() {}

void zpScriptingComponent::receiveMessage( const zpMessage& message ) {}

void zpScriptingComponent::serialize( zpSerializedOutput* out ) {}
void zpScriptingComponent::deserialize( zpSerializedInput* in ) {}

void zpScriptingComponent::onCreate() {
	zpContentManager* content = getGameManagerOfType<zpContentManager>();
	zpScriptingResource* script = content->getResourceOfType<zpScriptingResource>( m_scriptAlias );

	m_scriptInstance = new zpScriptingInstance( script );

	asIScriptEngine* engine = zpAngelScript::getInstance();

	zp_uint begin = script->getFilename().lastIndexOf( '/' );
	zp_uint end = script->getFilename().lastIndexOf( '.' );
	zpString className = script->getFilename().substring( begin, end );

	asIObjectType* type = engine->GetObjectTypeByName( className.c_str() );

	m_scriptObject = engine->CreateScriptObject( type->GetTypeId() );
	asIScriptFunction* func = type->GetMethodByName( "" );
	
}
void zpScriptingComponent::onDestroy() {}

void zpScriptingComponent::onUpdate() {}

void zpScriptingComponent::onEnabled() {}
void zpScriptingComponent::onDisabled() {}