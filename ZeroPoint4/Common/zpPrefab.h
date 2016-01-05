#pragma once
#ifndef ZP_PREFAB_H
#define ZP_PREFAB_H

class zpPrefabResource : public zpResource< zpBison >
{
private:
	zp_bool load( const zp_char* filename );
	void unload();

	friend class zpPrefabContentManager;
};

class zpPrefabResourceInstance : public zpResourceInstance< zpPrefabResource >
{};

class zpPrefabContentManager : public zpContentManager< zpPrefabResource, zpPrefabResourceInstance, zpPrefabContentManager, 64 >
{
public:
	void instantiatePrefab( const zpPrefabResourceInstance& prefab );

	zp_bool saveObjectAsPrefab( zpObject* obj, const zp_char* filename );
	zp_bool saveObjectsAsPrefab( const zpArrayList< zpObject* >& objs, const zp_char* filename );

private:
	zp_bool createResource( zpPrefabResource* res, const zp_char* filename );
	void destroyResource( zpPrefabResource* res );
	void initializeInstance( zpPrefabResourceInstance& instance ) {}

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif
