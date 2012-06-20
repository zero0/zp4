#include "zpScripting.h"
#include "angelscript.h"

zpScriptingComponent::zpScriptingComponent() : 
	m_scriptInstance( ZP_NULL ),
	m_scriptAlias()
{}
zpScriptingComponent::~zpScriptingComponent() {}

void zpScriptingComponent::receiveMessage( const zpMessage& message ) {}

void zpScriptingComponent::serialize( zpSerializedOutput* out ) {}
void zpScriptingComponent::deserialize( zpSerializedInput* in ) {}

void zpScriptingComponent::callFunction( const zpString& functionName ) {

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

	asIScriptContext* cxt = zpAngelScript::getInstance()->CreateContext();

	cxt->SetObject( m_scriptInstance->getScriptObject() );
	cxt->Prepare( (asIScriptFunction*)m_scriptInstance->getMethod( "onCreate" ) );
	cxt->Execute();
	cxt->Release();
}
void zpScriptingComponent::onDestroy() {}

void zpScriptingComponent::onUpdate() {}

void zpScriptingComponent::onEnabled() {}
void zpScriptingComponent::onDisabled() {}

