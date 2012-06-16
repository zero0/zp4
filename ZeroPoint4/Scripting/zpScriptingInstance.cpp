#include "zpScripting.h"

zpScriptingInstance::zpScriptingInstance() : m_resource( ZP_NULL ) {}
zpScriptingInstance::zpScriptingInstance( zpScriptingResource* resource ) : m_resource( resource ) {}
zpScriptingInstance::~zpScriptingInstance() {}

void zpScriptingInstance::callFunction( const zpString& functionName ) {
	
}