#include "zpScripting.h"
#include "angelscript.h"

void __onMessageCallback( const asSMessageInfo& info ) {
	switch( info.type ) {
	case asMSGTYPE_ERROR:
		zp_printfln( "%s [%d,%d] ERR:  %s", info.section, info.row, info.col, info.message );
		break;
	case asMSGTYPE_WARNING:
		zp_printfln( "%s [%d,%d] WARN: %s", info.section, info.row, info.col, info.message );
		break;
	case asMSGTYPE_INFORMATION:
		zp_printfln( "%s [%d,%d] INFO: %s", info.section, info.row, info.col, info.message );
		break;
	}
}

zpAngelScript::zpAngelScript() {}
zpAngelScript::~zpAngelScript() {}

zpMemoryAllocator* zpAngelScript::s_allocator = ZP_NULL;
asIScriptEngine* zpAngelScript::s_engine = ZP_NULL;
zp_bool zpAngelScript::createInstance( zpMemoryAllocator* memoryAllocator ) {
	if( s_engine ) return false;

	if( memoryAllocator ) {
		s_allocator = memoryAllocator;
		asSetGlobalMemoryFunctions( allocate, deallocate );
	}

	s_engine = asCreateScriptEngine( ANGELSCRIPT_VERSION );
	if( !s_engine ) return false;
	
	//s_engine->SetEngineProperty( asEP_COPY_SCRIPT_SECTIONS, true );
	//s_engine->SetMessageCallback( asFUNCTION( __onMessageCallback ), ZP_NULL, asCALL_STDCALL );
	return true;
}
void zpAngelScript::destroyInstance() {
	if( s_engine ) {
		s_engine->Release();
		s_engine = ZP_NULL;
	}
}

asIScriptEngine* zpAngelScript::getInstance() {
	return s_engine;
}

void* zpAngelScript::allocate( zp_uint size ) {
	return s_allocator->allocate( size );
}
void zpAngelScript::deallocate( void* ptr ) {
	s_allocator->deallocate( ptr );
}