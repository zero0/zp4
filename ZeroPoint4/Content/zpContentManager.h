#pragma once
#ifndef ZP_CONTENT_MANAGER_H
#define ZP_CONTENT_MANAGER_H

class zpContentManager : public zpGameManager {
public:
	zpContentManager();
	virtual ~zpContentManager();
	
	void registerFileExtension( const zpString& extension, zpResourceCreator* creator );
	
	zp_bool loadResource( const zpString& filename, const zpString& alias );
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
#if ZP_DEBUG
		return (T*)getResource_T( (const void*)&typeid( T ), alias );
#else
		return (T*)getResource( alias );
#endif
	}

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zpResource* getResource_T( const void* type, const zpString& alias ) const;

	zpString m_assetsFolder;
	zpString m_rootDirectory;

	zpHashMap<zpString, zpResourceCreator*> m_creators;
	zpHashMap<zpString, zpResource*> m_resources;
	zpHashMap<zpString, zpString> m_fileToAlias;
};

#endif