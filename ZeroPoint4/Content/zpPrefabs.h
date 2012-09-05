#pragma once
#ifndef ZP_PREFABS_H
#define ZP_PREFABS_H

class zpPrefabs {
public:
	~zpPrefabs();

	static zpPrefabs* getInstance();

	void setPrefabRoot( const zpString& root );
	const zpString& getPrefabRoot() const;

	zpSerializable* loadPrefab( const zpString& prefabName, zpSerializedInput* in = ZP_NULL );
	zpSerializable* loadPrefabWithOverrides( const zpString& prefabName, const zpProperties& overrides, zpSerializedInput* in = ZP_NULL );

	template<typename T>
	T* loadPrefabOfType( const zpString& prefabName, zpSerializedInput* in = ZP_NULL ) {
		return (T*)loadPrefab( prefabName, in );
	}
	template<typename T>
	T* loadPrefabWithOverridesOfType( const zpString& prefabName, const zpProperties& overrides, zpSerializedInput* in = ZP_NULL ) {
		return (T*)loadPrefabWithOverrides( prefabName, overrides, in );
	}

	zp_bool saveAsPrefab( const zpString& prefabName, zpSerializable* obj, zpSerializedOutput* out = ZP_NULL );

	void setDefaultSerializedInput( zpSerializedInput* in );
	zpSerializedInput* getDefaultSerializedInput() const;

	void setDefaultSerializedOutput( zpSerializedOutput* out );
	zpSerializedOutput* getDefaultSerializedOutput() const;

private:
	zpPrefabs();

	zpSerializedInput* m_defaultIn;
	zpSerializedOutput* m_defaultOut;

	zpString m_prefabRoot;
};

#endif