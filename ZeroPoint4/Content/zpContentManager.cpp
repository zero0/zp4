#include "zpContent.h"
#include <typeinfo>

zpContentManager::zpContentManager() {}
zpContentManager::~zpContentManager() {}
	
void zpContentManager::registerFileExtension( const zpString& extension, zpResourceCreator* creator ) {}
	
zp_bool zpContentManager::loadResource( const zpString& filename, const zpString& alias ) {
	zpString extension = filename.substring( filename.lastIndexOf( '.' ) + 1 );
	extension.toLower();

	zpResourceCreator* creator;
	if( m_creators.find( extension, &creator ) ) {
		zpStringBuffer filepath; //( m_rootDirectory.length() + m_assetsFolder.length() + creator->getRootDirectory().length() + filename.length() + 5 );

		filepath << m_rootDirectory << m_assetsFolder << creator->getRootDirectory() << filename;
		zpString fullFilePath = filepath.toString();

		zpResource* resource = creator->createResource( fullFilePath );
		ZP_ASSERT_RETURN_( resource, false, "Unable to create resource %s => %s", alias.c_str(), filename.c_str() );
		resource->setContentManager( this );

		m_resources[ alias ] = resource;
		m_fileToAlias[ filename ] = alias;

		return resource->load();
	}

	ZP_ON_DEBUG_MSG( "No resource creator registered for file extension '%s'", extension.c_str() );
	return false;
}
zp_uint zpContentManager::loadResources( const zpProperties& aliasToFilenames ) {
	zp_uint count = 0;
	aliasToFilenames.foreach( [ &count, this ]( const zpString& key, const zpString& value ){
		if( loadResource( value, key ) ) count++;
	} );
	return count;
}

zp_bool zpContentManager::unloadResource( const zpString& alias ) {
	zpResource* resource;
	if( m_resources.find( alias, &resource ) ) {
		resource->unload();
		return true;
	}
	ZP_ON_DEBUG_MSG( "Unable to unload resource, alias not found '%s'", alias.c_str() );
	return false;
}
zp_uint zpContentManager::unloadResources( const zpArrayList<zpString>& aliases ) {
	zp_uint count = 0;
	aliases.foreach( [ &count, this ]( const zpString& alias ){
		if( unloadResource( alias ) ) count++;
	} );
	return count;
}
void zpContentManager::unloadAllResources() {
	m_resources.foreach( []( const zpString& alias, zpResource* resource ){
		resource->unload();
	} );
}

zp_bool zpContentManager::reloadResource( const zpString& alias ) {
	zpResource* resource;
	if( m_resources.find( alias, &resource ) ) {
		resource->unload();
		return resource->load();
	}
	ZP_ON_DEBUG_MSG( "Unable to reload resource, alias not found '%s'", alias.c_str() );
	return false;
}
zp_uint zpContentManager::reloadResources( const zpArrayList<zpString>& aliases ) {
	zp_uint count = 0;
	aliases.foreach( [ &count, this ]( const zpString& alias ){
		if( reloadResource( alias ) ) count++;
	} );
	return count;
}
void zpContentManager::reloadAllResources() {
	m_resources.foreach( []( const zpString& alias, zpResource* resource ){
		resource->unload();
		resource->load();
	} );
}

zp_bool zpContentManager::isFileAlreadyLoaded( const zpString& filename, zpString* outAlias ) const {
	return m_fileToAlias.find( filename, outAlias );
}

zpResource* zpContentManager::getResource( const zpString& alias ) const {
	return m_resources.get( alias );
}

void zpContentManager::receiveMessage( const zpMessage& message ) {}

void zpContentManager::serialize( zpSerializedOutput* out ) {}
void zpContentManager::deserialize( zpSerializedInput* in ) {}

void zpContentManager::onCreate() {}
void zpContentManager::onDestroy() {}

void zpContentManager::onUpdate() {}

void zpContentManager::onEnabled() {}
void zpContentManager::onDisabled() {}

zpResource* zpContentManager::getResource_T( const void* type, const zpString& alias ) const {
	const std::type_info& info = *(const std::type_info*)type;
	zpResource* resource = m_resources.get( alias );
	
	ZP_ASSERT_RETURN_( ( resource && info == typeid( *resource ) ), ZP_NULL, "Resource '%s' is not type %s", alias.c_str(), info.name() );
	
	return resource;
}