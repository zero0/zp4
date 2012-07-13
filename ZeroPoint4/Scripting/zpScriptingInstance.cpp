#include "zpScripting.h"
#include "zpAngelScript.h"

zpScriptingInstance::zpScriptingInstance() : 
	m_resource( ZP_NULL ),
	m_scriptObject( ZP_NULL )
{}
zpScriptingInstance::zpScriptingInstance( zpScriptingResource* resource ) : 
	m_resource( resource ),
	m_scriptObject( ZP_NULL )
{
	createObject();
}
zpScriptingInstance::~zpScriptingInstance() {
	destroyObject();
}

void* zpScriptingInstance::getMethod( const zpString& methodName ) {
	return m_resource->getMethod( methodName );
}
void* zpScriptingInstance::getScriptObject() const {
	return m_scriptObject;
}

void zpScriptingInstance::createObject() {
	if( m_scriptObject ) return;

	asIScriptEngine* engine = zpAngelScript::getInstance();
	asIObjectType* type = (asIObjectType*)m_resource->getScriptObjectType();

	m_scriptObject = engine->CreateScriptObject( type->GetTypeId() );
	engine->AddRefScriptObject( m_scriptObject, type );
}
void zpScriptingInstance::destroyObject() {
	if( !m_scriptObject ) return;

	asIScriptEngine* engine = zpAngelScript::getInstance();
	asIObjectType* type = (asIObjectType*)m_resource->getScriptObjectType();

	engine->ReleaseScriptObject( m_scriptObject, type );
	m_scriptObject = ZP_NULL;
}