#pragma once
#ifndef ZP_PREFABS_H
#define ZP_PREFABS_H

class zpPrefabResource : public zpResource {
public:
	zpPrefabResource();
	virtual ~zpPrefabResource();

	zp_bool load();
	void unload();

	zpXmlNode* getRootNode();

private:
	zpXmlNode m_root;
};

ZP_RESOURCE_INSTANCE_TEMPLATE_START_COPY( zpPrefabResource, copy )
public:
	const zpProperties& getOverrides() const;

private:
	void copy( const zpResourceInstance<zpPrefabResource>& other );

	zpProperties m_overrides;
ZP_RESOURCE_INSTANCE_TEMPLATE_END

class zpPrefabResourceCreator : public zpResourceCreator {
public:
	zpPrefabResourceCreator();
	virtual ~zpPrefabResourceCreator();

	zpResource* createResource( const zpString& filename );
};

class zpPrefabs {
public:
	~zpPrefabs();

	static zpPrefabs* getInstance();

	void setPrefabRoot( const zpString& root );
	const zpString& getPrefabRoot() const;

	zpSerializable* loadPrefab( const zpString& prefabName );
	zpSerializable* loadPrefabWithOverrides( const zpString& prefabName, const zpProperties& overrides );

	template<typename T>
	T* loadPrefabOfType( const zpString& prefabName ) {
		return (T*)loadPrefab( prefabName );
	}
	template<typename T>
	T* loadPrefabWithOverridesOfType( const zpString& prefabName, const zpProperties& overrides ) {
		return (T*)loadPrefabWithOverrides( prefabName, overrides );
	}

	zp_bool saveAsPrefab( const zpString& prefabName, zpSerializable* obj );

private:
	zpPrefabs();
	
	void setContentManager( zpContentManager* content );
	void update();

	struct zpPrefabToLoad {
		zpPrefabResource* prefab;
		zpSerializable* serializable;
		zpProperties overrides;
	};

	zpContentManager* m_contentManager;
	zpPrefabResourceCreator m_prefabCreator;
	zpString m_prefabRoot;
	zpHashMap<zpString, zpResourceInstance<zpPrefabResource>> m_prefabs;
	zpArrayList<zpPrefabToLoad> m_prefabsToLoad;

	friend class zpContentManager;
};


#endif