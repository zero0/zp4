#include "zpScripting.h"
#include "angelscript.h"

#define AS_ASSERT( r )	if( (r) < asSUCCESS ) { return false; }

zpScriptingResource::zpScriptingResource() : m_script() {}
zpScriptingResource::~zpScriptingResource() {
	unload();
	ZP_SAFE_DELETE( m_script );
}

zp_bool zpScriptingResource::load() {
	asIScriptEngine* engine = zpAngelScript::getInstance();
	zp_int r;

	asIScriptModule* module = engine->GetModule( getFilename().c_str(), asGM_CREATE_IF_NOT_EXISTS );
	if( module == ZP_NULL ) return false;

	// strip extension and directories off to get section name
	zpString code;
	// process code
	r = module->AddScriptSection( getFilename().c_str(), code.c_str(), code.length() );
	AS_ASSERT( r );

	// build the module
	r = module->Build();
	AS_ASSERT( r );

	return false;
}
void zpScriptingResource::unload() {
	zpAngelScript::getInstance()->DiscardModule( getFilename().c_str() );
}