#include "zpScripting.h"
#include "zpAngelScript.h"
#include "angelscript.h"

void* zpResourceInstance<zpScriptResource>::getMethod( const zpString& methodName ) {
	return getResource()->getMethod( methodName );
}
void* zpResourceInstance<zpScriptResource>::getScriptObject() const {
	return m_scriptObject;
}

void zpResourceInstance<zpScriptResource>::createObject() {
	if( m_scriptObject ) return;
	
	asIScriptEngine* engine = zpAngelScript::getInstance();
	asIObjectType* type = (asIObjectType*)getResource()->getScriptObjectType();
	if( type ) {
		m_scriptObject = engine->CreateScriptObject( type->GetTypeId() );
		engine->AddRefScriptObject( m_scriptObject, type );
	} else {
		zpLog::error() << "Failed to create script object." << zpLog::endl;
	}
}
void zpResourceInstance<zpScriptResource>::destroyObject() {
	if( !m_scriptObject ) return;

	asIScriptEngine* engine = zpAngelScript::getInstance();
	asIObjectType* type = (asIObjectType*)getResource()->getScriptObjectType();
	if( type ) {
		engine->ReleaseScriptObject( m_scriptObject, type );
		m_scriptObject = ZP_NULL;
	}
}