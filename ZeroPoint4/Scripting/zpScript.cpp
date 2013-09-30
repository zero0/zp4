#include "zpScripting.h"
#include "angelscript.h"

zp_handle zpScriptResource::getMethod( const zp_char* methodName ) const
{
	zp_handle method = ZP_NULL;
	zp_handle* foundMethod = ZP_NULL;
	zpString methodStr( methodName );
	if( m_cachedMethods.find( methodStr, &foundMethod ) )
	{
		method = *foundMethod;
	}
	else
	{
		asIObjectType* objectType = (asIObjectType*)m_resource;
		method = objectType->GetMethodByName( methodName );
		m_cachedMethods[ methodStr ] = method;
	}

	return method;
}
zp_bool zpScriptResource::load( const zp_char* filename )
{
	m_filename = filename;

	asIScriptEngine* engine = (asIScriptEngine*)zpAngelScript::getInstance()->getEngine();

	asIScriptModule* module = engine->GetModule( filename, asGM_CREATE_IF_NOT_EXISTS );
	if( module == ZP_NULL ) return false;
	
	zpStringBuffer code;
	zpFile codeFile( filename );
	if( codeFile.open( ZP_FILE_MODE_ASCII_READ ) )
	{
		codeFile.readFile( code );
		codeFile.close();
	}
	else
	{
		return false;
	}

	zp_int r;
	r = module->AddScriptSection( filename, code.str(), code.length() );
	if( r < asSUCCESS ) return false;

	r = module->Build();
	if( r < asSUCCESS ) return false;

	zp_int b = m_filename.lastIndexOf( zpFile::sep ) + 1;
	zp_int e = m_filename.lastIndexOf( '.' );

	zpString className;
	m_filename.substring( className, b, e );

	asIObjectType* objectType = module->GetObjectTypeByName( className.str() );
	if( objectType == ZP_NULL ) return false;

	objectType->AddRef();
	m_resource = objectType;

	asIScriptFunction* method;
	zp_uint count = objectType->GetMethodCount();
	for( zp_uint i = 0; i < count; ++i )
	{
		method = objectType->GetMethodByIndex( i );
		method->AddRef();
		m_cachedMethods.put( zpString( method->GetName() ), method );
	}

	return true;
}
void zpScriptResource::unload()
{
	m_cachedMethods.foreach( []( const zpString& key, zp_handle value )
	{
		( (asIScriptFunction*)value )->Release();
	} );
	m_cachedMethods.clear();

	if( m_resource )
	{
		( (asIObjectType*)m_resource )->Release();
		m_resource = ZP_NULL;
	}

	asIScriptEngine* engine = (asIScriptEngine*)zpAngelScript::getInstance();
	engine->DiscardModule( m_filename.str() );

	m_filename.clear();
}

zpScriptResourceInstance::zpScriptResourceInstance()
	: m_scriptObject( ZP_NULL )
{}
zpScriptResourceInstance::~zpScriptResourceInstance()
{
	destroyed();
}
void zpScriptResourceInstance::callMethod( const zp_char* methodName )
{
	if( isVaild() )
	{
		zp_handle method = getResource()->getMethod( methodName );
		zpAngelScript::getInstance()->callObjectMethod( m_scriptObject, method );
	}
}
void zpScriptResourceInstance::initialized()
{
	if( m_scriptObject != ZP_NULL )
	{
		destroyed();
	}

	zp_handle objectType = *getResource()->getData();
	m_scriptObject = zpAngelScript::getInstance()->createScriptObject( objectType );
}
void zpScriptResourceInstance::destroyed()
{
	if( isVaild() && m_scriptObject != ZP_NULL )
	{
		zp_handle objectType = *getResource()->getData();
		zpAngelScript::getInstance()->destroyScriptObject( m_scriptObject, objectType );
		m_scriptObject = ZP_NULL;
	}
}

zpScriptContentManager::zpScriptContentManager()
{
	zpAngelScript::getInstance()->createEngine();
}
zpScriptContentManager::~zpScriptContentManager()
{
	zpAngelScript::getInstance()->destroyEngine();
}

zp_bool zpScriptContentManager::createResource( zpScriptResource* res, const zp_char* filename )
{
	return res->load( filename );
}
void zpScriptContentManager::destroyResource( zpScriptResource* res )
{
	res->unload();
}
