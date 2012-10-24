#include "zpContent.h"
#include <typeinfo>

zpContentManager::zpContentManager()
	: m_shouldCleanUp( false )
{}
zpContentManager::~zpContentManager() {}
	
void zpContentManager::registerFileExtension( const zpString& extension, zpResourceCreator* creator ) {
	m_creators.put( extension, creator );
}
	
zp_bool zpContentManager::loadResource( const zpString& filename, const zpString& alias, zp_bool immediateLoad ) {
	zpString extension = filename.substring( filename.lastIndexOf( '.' ) + 1 );
	extension.toLower();

	zpResourceCreator* creator;
	if( m_creators.find( extension, &creator ) ) {
		zpStringBuffer filepath;
		filepath << m_rootDirectory << m_assetsFolder << creator->getRootDirectory() << filename;
		zpString fullFilePath = filepath.toString();

		zpResource* resource = creator->createResource( fullFilePath );
		ZP_ASSERT( resource != ZP_NULL, "Unable to create resource %s => %s", alias.c_str(), filename.c_str() );
		
		resource->setContentManager( this );
		resource->setFilename( fullFilePath );

		zpResourceElement element = { resource, 0, alias, filename };
		m_elements.pushBack( element );
		
		zp_bool loaded = true;
		if( immediateLoad ) {
			loaded = resource->load();
			resource->setIsLoaded( loaded );

			m_onResourceLoaded( resource->getFilename(), loaded, 1 );
		} else {
			m_resourcesToLoad.pushBack( resource );
		}
		
		return loaded;
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
	zpResourceElement* found = ZP_NULL;
	if( m_elements.findIf( [ &alias ]( zpResourceElement& element ) {
		return alias == element.alias;
	}, &found ) ) {
		if( found->refCount == 0 ) return true;

		found->refCount--;
		if( found->refCount == 0 ) {
			m_shouldCleanUp = true;
			return true;
		}
	}
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
	m_elements.foreach( [ this ]( zpResourceElement& element ) {
		if( element.refCount == 0 ) return;

		element.refCount--;
		if( element.refCount == 0 ) {
			m_shouldCleanUp = true;
		}
	} );
}

zp_bool zpContentManager::reloadResource( const zpString& alias ) {
	zpResourceElement* found = ZP_NULL;
	if( m_elements.findIf( [ &alias, this ]( zpResourceElement& element ) {
		return alias == element.alias;
	}, &found ) ) {
		found->resource->unload();
		found->resource->setIsLoaded( false );
		m_resourcesToLoad.pushBack( found->resource );
		return true;
	}
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
	m_elements.foreach( [ this ]( zpResourceElement& element ) {
		element.resource->unload();
		element.resource->setIsLoaded( false );
		m_resourcesToLoad.pushBack( element.resource );
	} );
}

zp_bool zpContentManager::isFileAlreadyLoaded( const zpString& filename, zpString* outAlias ) const {
	zpResourceElement* found = ZP_NULL;
	if( m_elements.findIf( [ &filename ]( zpResourceElement& element ) {
		return filename == element.file;
	}, &found ) ) {
		if( outAlias ) *outAlias = found->alias;
		return true;
	}
	return false;
}

zpResource* zpContentManager::getResource( const zpString& alias ) {
	zpResourceElement* found;
	zpResource* resource = ZP_NULL;
	if( m_elements.findIf( [ &alias ]( zpResourceElement& element ) {
		return alias == element.alias;
	}, &found ) ) {
		found->refCount++;
		resource = found->resource;
	}
	return resource;
}
void zpContentManager::unloadResource( zpResource* resource ) {
	zpResourceElement* found = ZP_NULL;
	if( m_elements.findIf( [ &resource ]( zpResourceElement& element ) {
		return resource == element.resource;
	}, &found ) ) {
		if( found->refCount == 0 ) return;

		found->refCount--;
		if( found->refCount == 0 ) {
			m_shouldCleanUp = true;
		}
	}
}

void zpContentManager::receiveMessage( const zpMessage& message ) {}

void zpContentManager::serialize( zpSerializedOutput* out ) {}
void zpContentManager::deserialize( zpSerializedInput* in ) {}

void zpContentManager::setRootDirectory( const zpString& rootDirectory ) {
	m_assetsFolder = rootDirectory;
	zpFile::convertToFilePath( m_assetsFolder );
}
const zpString& zpContentManager::getRootDirectory() const {
	return m_rootDirectory;
}
zp_bool zpContentManager::getRootDirectoryForExtension( const zpString& extension, zpString& outRoot ) const {
	zpResourceCreator* creator = ZP_NULL;
	if( m_creators.find( extension, &creator ) ) {
		outRoot = creator->getRootDirectory();
		return true;
	}
	return false;
}

zpDelegateEvent<void( const zpString&, zp_bool, zp_uint )>& zpContentManager::onResourceLoaded() {
	return m_onResourceLoaded;
}
zpDelegateEvent<void()>& zpContentManager::onAllResourcesLoaded() {
	return m_onAllResourcesLoaded;
}

void zpContentManager::onCreate() {
	//m_rootDirectory = zpFile::getCurrentDirectory();
	m_rootDirectory = "./";
	m_rootDirectory[1] = zpFile::sep;
}
void zpContentManager::onDestroy() {}

void zpContentManager::onUpdate() {
	if( !m_resourcesToLoad.isEmpty() ) {
		zpResource* r = m_resourcesToLoad.back();
		m_resourcesToLoad.popBack();
		
		zp_bool loaded = r->load();
		r->setIsLoaded( loaded );
		
		m_onResourceLoaded( r->getFilename(), loaded, m_resourcesToLoad.size() );

		if( m_resourcesToLoad.isEmpty() ) {
			m_onAllResourcesLoaded();
		}
	}

	if( m_shouldCleanUp ) {
		m_shouldCleanUp = false;
		for( zp_uint i = m_elements.size(); i --> 0; ) {
			if( m_elements[i].refCount == 0 ) {
				m_elements[i].resource->unload();
				ZP_SAFE_DELETE( m_elements[i].resource );
				m_elements.erase( i );
			}
		}
	}
}

void zpContentManager::onEnabled() {}
void zpContentManager::onDisabled() {}
