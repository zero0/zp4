#include "zpContent.h"

zpPrefabs::zpPrefabs() {}
zpPrefabs::~zpPrefabs() {}

zpPrefabs* zpPrefabs::getInstance() {
	static zpPrefabs instance;
	return &instance;
}

void zpPrefabs::setPrefabRoot( const zpString& root ) {
	m_prefabRoot = root;
}
const zpString& zpPrefabs::getPrefabRoot() const {
	return m_prefabRoot;
}

zpSerializable* zpPrefabs::loadPrefab( const zpString& prefabName ) {
	return ZP_NULL;
}
zpSerializable* zpPrefabs::loadPrefabWithOverrides( const zpString& prefabName, const zpProperties& overrides ) {
	return ZP_NULL;
}

zp_bool zpPrefabs::saveAsPrefab( const zpString& prefabName, zpSerializable* obj ) {
	return false;
}
