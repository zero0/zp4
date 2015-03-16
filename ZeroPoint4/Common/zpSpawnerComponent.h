#pragma once
#ifndef ZP_SPAWNER_COMPONENT_H
#define ZP_SPAWNER_COMPONENT_H

struct zpSpawnerGroup
{
	zp_bool spawnRandomObject;
	zp_bool repeatSpawnedObjects;
	zp_bool trackSpawnedObjects;

	zp_int numSpawnedFromGroup;
	zp_int maxSpawnedFromGroup;

	zp_int minNumToSpawn;
	zp_int maxNumToSpawn;

	zpVector4f range;
	zpVector4f position;
	zpQuaternion4f rotation;

	zpArrayList< zpString > objectsToSpawn;
};

class zpSpawnerComponent : public zpComponent, public zpEventListener
{
public:
	zpSpawnerComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpSpawnerComponent();

	void handleEvent( const zpEvent& e, zpObject* sender );

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate( zp_float deltaTime, zp_float realTime );
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	void spawnFromGroup( zp_int group );
	zpObject* spawnObjectAt( const zpString& object, const zpVector4f& position, const zpQuaternion4f& rotation );

	zp_int m_numSpawned;
	zp_int m_maxTotalSpawned;

	zpString m_spawnOnEvent;
	zpString m_spawnOnMessage;

	zpEventHandler m_spawnEvent;

	zpString m_eventOnSpawningComplete;
	zpString m_messageOnSpawningComplete;

	zpArrayList< zpSpawnerGroup > m_spawnGroups;
	zpArrayList< zpObject* > m_objectsSpawned;
};


class zpSpawnerComponentPool : public zpContentPool< zpSpawnerComponent, 8 >
{
public:
	zpSpawnerComponentPool();
	virtual ~zpSpawnerComponentPool();

	void update( zp_float deltaTime, zp_float realTime );
};

#endif
