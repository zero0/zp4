#pragma once
#ifndef ZP_PREFABS_H
#define ZP_PREFABS_H

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

	zpString m_prefabRoot;
};

#endif