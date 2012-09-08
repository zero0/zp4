#include "zpScripting.h"
#include "zpAngelScript.h"

#define AS_ASSERT( r )	if( (r) < asSUCCESS ) { return false; }

zpScriptingResource::zpScriptingResource() : 
	m_scriptObjectType( ZP_NULL )
{}
zpScriptingResource::~zpScriptingResource() {
	unload();
}

zp_bool zpScriptingResource::load() {
	asIScriptEngine* engine = zpAngelScript::getInstance();
	zp_int r;

	asIScriptModule* module = engine->GetModule( getFilename().c_str(), asGM_CREATE_IF_NOT_EXISTS );
	if( !module ) return false;

	// strip extension and directories off to get section name
	zpStringBuffer code;

	zpFile codeFile( getFilename(), ZP_FILE_MODE_READ );
	if( codeFile.open() ) {
		codeFile.readFile( &code );
		codeFile.close();
	} else {
		return false;
	}

	zpString codeString( code.toString() );

	// process code
	r = module->AddScriptSection( getFilename().c_str(), codeString.c_str(), codeString.length() );
	AS_ASSERT( r );
	
	// build the module
	r = module->Build();
	AS_ASSERT( r );

	// get the class name from the file name
	zp_uint begin = getFilename().lastIndexOf( zpFile::sep );
	begin = begin == zpString::npos ? 0 : begin + 1;
	zp_uint end = getFilename().lastIndexOf( '.' );
	zpString className = getFilename().substring( begin, end );
	
	asIObjectType* type = ZP_NULL;
	for( zp_uint i = module->GetObjectTypeCount(); i --> 0; ) {
		asIObjectType* objType = module->GetObjectTypeByIndex( i );
		if( className == objType->GetName() ) {
			type = objType;
			break;
		}
	}
	if( type == ZP_NULL ) return false;
	
	type->AddRef();
	m_scriptObjectType = type;

	// cache functions when created
	zp_uint funcCount = type->GetMethodCount();
	for( zp_uint i = 0; i < funcCount; ++i ) {
		asIScriptFunction* func = type->GetMethodByIndex( i );
		if( func == ZP_NULL ) continue;
		func->AddRef();
		
		m_cachedMethods.put( func->GetName(), func );
	}

	return true;
}
void zpScriptingResource::unload() {
	if( m_scriptObjectType ) {
		( (asIObjectType*)m_scriptObjectType )->Release();
		m_scriptObjectType = ZP_NULL;
	}

	m_cachedMethods.foreach( []( const zpString& key, void* value ) {
		( (asIScriptFunction*)value )->Release();
	} );
	m_cachedMethods.clear();

	zpAngelScript::getInstance()->DiscardModule( getFilename().c_str() );
}

void* zpScriptingResource::getScriptObjectType() const {
	return m_scriptObjectType;
}
void* zpScriptingResource::getMethod( const zpString& functionName ) {
	void* method = ZP_NULL;
	if( !m_cachedMethods.find( functionName, &method ) ) {
		asIScriptFunction* func = ( (asIObjectType*)m_scriptObjectType )->GetMethodByName( functionName.c_str() );
		if( func ) {
			func->AddRef();
			method = func;
		}
		m_cachedMethods.put( functionName, func );
	}
	return method;
}