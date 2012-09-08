#include "zpScripting.h"
#include "zpAngelScript.h"

zpScriptingComponent::zpScriptingComponent() : 
	m_scriptInstance( ZP_NULL ),
	m_manager( ZP_NULL ),
	m_scriptAlias()
{}
zpScriptingComponent::~zpScriptingComponent() {}

void zpScriptingComponent::receiveMessage( const zpMessage& message ) {}

void zpScriptingComponent::serialize( zpSerializedOutput* out ) {}
void zpScriptingComponent::deserialize( zpSerializedInput* in ) {}

void zpScriptingComponent::callFunction( const zpString& functionName ) {
	m_manager->callObjectFunction( m_scriptInstance->getScriptObject(), m_scriptInstance->getMethod( functionName ) );
}

void zpScriptingComponent::setScriptAlias( const zpString& alias ) {
	m_scriptAlias = alias;
}
const zpString& zpScriptingComponent::getScriptAlias() const {
	return m_scriptAlias;
}

void zpScriptingComponent::onCreate() {
	zpContentManager* content = getGameManagerOfType<zpContentManager>();

	zpScriptingResource* script = content->getResourceOfType<zpScriptingResource>( m_scriptAlias );

	m_scriptInstance = new zpScriptingInstance( script );

	m_manager = (zpScriptingManager*)zpAngelScript::getInstance()->GetUserData();
	m_manager->callObjectFunction( m_scriptInstance->getScriptObject(), m_scriptInstance->getMethod( "onCreate" ) );
}
void zpScriptingComponent::onDestroy() {
	m_manager->callObjectFunction( m_scriptInstance->getScriptObject(), m_scriptInstance->getMethod( "onDestroy" ) );
}

void zpScriptingComponent::onUpdate() {
	m_manager->callObjectFunction( m_scriptInstance->getScriptObject(), m_scriptInstance->getMethod( "onUpdate" ) );
}

void zpScriptingComponent::onEnabled() {
	m_manager->callObjectFunction( m_scriptInstance->getScriptObject(), m_scriptInstance->getMethod( "onEnabled" ) );
}
void zpScriptingComponent::onDisabled() {
	m_manager->callObjectFunction( m_scriptInstance->getScriptObject(), m_scriptInstance->getMethod( "onDisabled" ) );
}

