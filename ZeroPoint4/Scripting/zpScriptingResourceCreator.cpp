#include "zpScripting.h"

zpScriptingResourceCreator::zpScriptingResourceCreator() {}
zpScriptingResourceCreator::~zpScriptingResourceCreator() {}

zpResource* zpScriptingResourceCreator::createResource( const zpString& filename ) {
	return new zpScriptingResource();
}