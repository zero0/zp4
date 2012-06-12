#include "zpContent.h"
#include <typeinfo>

zpContentManager::zpContentManager() {}
zpContentManager::~zpContentManager() {}
	
void zpContentManager::registerFileExtension( const zpString& extension, zpResourceCreator* creator ) {}
	
zp_bool zpContentManager::loadResource( const zpString& filename, const zpString& alias ) {
	return false;
}
zp_uint zpContentManager::loadResources( const zpProperties& aliasToFilenames ) {
	return 0;
}

zp_bool zpContentManager::unloadResource( const zpString& alias ) {
	return false;
}
zp_uint zpContentManager::unloadResources( const zpArrayList<zpString>& aliases ) {
	return 0;
}
void zpContentManager::unloadAllResources() {}

zp_bool zpContentManager::reloadResource( const zpString& alias ) {
	return false;
}
zp_uint zpContentManager::reloadResources( const zpArrayList<zpString>& aliases ) {
	return 0;
}
void zpContentManager::reloadAllResources() {}

zp_bool zpContentManager::isFileAlreadyLoaded( const zpString& filename, zpString* outAlias ) const {
	return false;
}

zpResource* zpContentManager::getResource( const zpString& alias ) const {
	return m_resources.get( alias );
}

void zpContentManager::receiveMessage( const zpMessage& message ) {}

void zpContentManager::serialize( zpSerializedOutput* out ) {}
void zpContentManager::deserialize( zpSerializedInput* in ) {}

void zpContentManager::onCreate() {
	
}
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