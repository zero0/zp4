#pragma once
#ifndef ZP_CONTENT_MANAGER_H
#define ZP_CONTENT_MANAGER_H
/*
template<typename Resource, zp_uint Count>
class zpContentManager
{
	ZP_NON_COPYABLE( zpContentManager< Resource, count > );

public:
	struct ResourceElement
	{
		zp_uint m_refCount;
		zpString m_filename;
		Resource m_resource;
	};

	zpContentManager();
	~zpContentManager();

	

private:
	zpFixedArrayList< ResourceElement, Count > m_resources;
};
*/
#if 1
struct zpResourceElement
{
	zpResource* resource;
	zp_uint refCount;
	zpString filename;
};

class zpContentManager : public zpGameManager
{
public:
	zpContentManager();
	virtual ~zpContentManager();
	
	void registerFileExtension( const zpString& extension, zpResourceCreator* creator );

	zp_bool reloadResource( const zpString& filename );
	zp_uint reloadResources( const zpArrayList<zpString>& filenames );
	void reloadAllResources();

	zp_bool isFileAlreadyLoaded( const zpString& filename ) const;

	template<typename R>
	zpResourceInstance<R> createInstanceOfResource( const zpString& filename ) {
		return zpResourceInstance<R>( (R*)getResource( filename ) );
	}
		
	template<typename R>
	void destroyInstanceOfResource( zpResourceInstance<R>& instance ) {
		unloadResource( (zpResource*)instance.getResource() );
		instance.m_resource = ZP_NULL;
	}

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	void setRootDirectory( const zpString& rootDirectory );
	const zpString& getRootDirectory() const;
	zp_bool getRootDirectoryForExtension( const zpString& extension, zpString& outRoot ) const;

	zpDelegateEvent<void( const zpString&, zp_bool, zp_uint )>& onResourceLoaded();
	zpDelegateEvent<void()>& onAllResourcesLoaded();

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zpResource* getResource( const zpString& filename );
	void unloadResource( zpResource* resource );

	zp_bool m_shouldCleanUp;
	zpString m_assetsFolder;
	zpString m_rootDirectory;

	zpDelegateEvent<void( const zpString& filename, zp_bool loaded, zp_uint numLeft )> m_onResourceLoaded;
	zpDelegateEvent<void()> m_onAllResourcesLoaded;

	zpHashMap< zpString, zpResourceCreator* > m_creators;

	zpArrayList< zpResource* > m_resourcesToLoad;
	zpArrayList< zpResourceElement > m_elements;
};
#endif

#endif