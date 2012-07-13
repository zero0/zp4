#include "zpScripting.h"
#include "zpAngelScript.h"

void __print( const zpString& str ) {
	zp_printf( "%s", str.c_str() );
}
void __println( const zpString& str ) {
	zp_printfln( "%s", str.c_str() );
}

zpScriptingManager::zpScriptingManager() : 
	m_numContexts( 10 ),
	m_contexts( m_numContexts )
{}
zpScriptingManager::~zpScriptingManager() {}

void zpScriptingManager::receiveMessage( const zpMessage& message ) {}

void zpScriptingManager::serialize( zpSerializedOutput* out ) {}
void zpScriptingManager::deserialize( zpSerializedInput* in ) {}

void zpScriptingManager::onCreate() {
	m_contexts.ensureCapacity( m_numContexts );

	zpAngelScript::createInstance();
	asIScriptEngine* engine = zpAngelScript::getInstance();
	zp_int r;

	r = engine->SetMessageCallback( asMETHOD( zpScriptingManager, messageCallback ), this, asCALL_THISCALL );
	engine->SetUserData( this );

	r = engine->RegisterGlobalFunction( "void print( string &in )", asFUNCTION( __print ), asCALL_CDECL );
	r = engine->RegisterGlobalFunction( "void println( string &in )", asFUNCTION( __println ), asCALL_CDECL );
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