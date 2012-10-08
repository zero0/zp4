#pragma once
#ifndef ZP_CONTENT_MANAGER_H
#define ZP_CONTENT_MANAGER_H

class zpContentManager : public zpGameManager {
public:
	zpContentManager();
	virtual ~zpContentManager();
	
	void registerFileExtension( const zpString& extension, zpResourceCreator* creator );
	
	zp_bool loadResource( const zpString& filename, const zpString& alias, zp_bool immediateLoad = false );
	zp_uint loadResources( const zpProperties& aliasToFilenames );

	zp_bool unloadResource( const zpString& alias );
	zp_uint unloadResources( const zpArrayList<zpString>& aliases );
	void unloadAllResources();

	zp_bool reloadResource( const zpString& alias );
	zp_uint reloadResources( const zpArrayList<zpString>& aliases );
	void reloadAllResources();

	zp_bool isFileAlreadyLoaded( const zpString& filename, zpString* outAlias = ZP_NULL ) const;

	zpResource* getResource( const zpString& alias ) const;

	template<typename T>
	T* getResourceOfType( const zpString& alias ) const {
		return (T*)getResource( alias );
	}

	template<typename R>
	zpResourceInstance<R> createInstanceOfResource( const zpString& alias ) const {
		return zpResourceInstance<R>( getResourceOfType<R>( alias ) );
	}

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	void setRootDirectory( const zpString& rootDirectory );
	const zpString& getRootDirectory() const;

	zpDelegateEvent<void( const zpString&, zp_bool, zp_uint )>& onResourceLoaded();
	zpDelegateEvent<void()>& onAllResourcesLoaded();

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zpString m_assetsFolder;
	zpString m_rootDirectory;

	zpDelegateEvent<void( const zpString& filename, zp_bool loaded, zp_uint numLeft )> m_onResourceLoaded;
	zpDelegateEvent<void()> m_onAllResourcesLoaded;

	zpArrayList<zpResource*> m_resourcesToLoad;
	zpHashMap<zpString, zpResourceCreator*> m_creators;
	zpHashMap<zpString, zpResource*> m_resources;
	zpHashMap<zpString, zpString> m_fileToAlias;
};

#endif