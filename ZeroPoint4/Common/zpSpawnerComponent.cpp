#include "zpCommon.h"

zpSpawnerComponent::zpSpawnerComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_numSpawned( 0 )
	, m_maxTotalSpawned( 1 )
{
	m_spawnOnEvent = def[ "SpawnOnEvent" ].asCString();
	m_spawnOnMessage = def[ "SpawnOnMessage" ].asCString();

	const zpBison::Value& groups = def[ "SpawnGroups" ];
	if( groups.isArray() && !groups.isEmpty() )
	{
		m_spawnGroups.reserve( groups.size() );

		groups.foreachArray( [ this ]( zpBison::Value& v ) {
			zpSpawnerGroup& g = m_spawnGroups.pushBackEmpty();
			g.spawnRandomObject = v[ "SpawnRandomObject" ].asBool();
			g.repeatSpawnedObjects = v[ "RepeatSpawnedObjects" ].asBool();
			g.trackSpawnedObjects = v[ "TrackSpawnedObjects" ].asBool();

			g.numSpawnedFromGroup = 0;
			g.maxSpawnedFromGroup = v[ "MaxSpawnedFromGroup" ].asInt();

			g.minNumToSpawn = v[ "MinNumToSpawn" ].asInt();
			g.maxNumToSpawn = v[ "MaxNumToSpawn" ].asInt();

			const zpBison::Value& r = v[ "Range" ];
			g.range = zpMath::Vector4( r[0].asFloat(), r[1].asFloat(), r[2].asFloat(), 0.f );

			const zpBison::Value& p = v[ "Position" ];
			g.position = zpMath::Vector4( p[0].asFloat(), p[1].asFloat(), p[2].asFloat(), 1.f );

			const zpBison::Value& o = v[ "Objects" ];
			if( o.isArray() && !o.isEmpty() )
			{
				g.objectsToSpawn.reserve( o.size() );
				o.foreachArray( [ &g ]( const zpBison::Value& objs ) {
					g.objectsToSpawn.pushBackEmpty() = objs.asCString();
				} );
			}
		} );
	}
}
zpSpawnerComponent::~zpSpawnerComponent()
{

}

void zpSpawnerComponent::handleEvent( const zpEvent& e, zpObject* sender )
{
	if( m_spawnEvent.isHandlerForEvent( e ) )
	{

	}
}

void zpSpawnerComponent::onCreate()
{
	getApplication()->getEventManager()->addEventListener( m_spawnOnEvent, this, m_spawnEvent );
}
void zpSpawnerComponent::onInitialize()
{

}
void zpSpawnerComponent::onDestroy()
{
	getApplication()->getEventManager()->removeEventListener( m_spawnOnEvent, this, m_spawnEvent );
}

void zpSpawnerComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
	m_objectsSpawned.eraseIf( []( zpObject* o ) {
		return o->isFlagSet( ZP_OBJECT_FLAG_SHOULD_DESTROY );
	} );
}
void zpSpawnerComponent::onSimulate()
{

}

void zpSpawnerComponent::onEnabled()
{

}
void zpSpawnerComponent::onDisabled()
{

}

void zpSpawnerComponent::spawnFromGroup( zp_int group )
{
	zpSpawnerGroup& spawnGroup = m_spawnGroups[ group ];

	if( spawnGroup.maxSpawnedFromGroup < 0 || spawnGroup.numSpawnedFromGroup < spawnGroup.maxSpawnedFromGroup )
	{
		zpRandom* rnd = zpRandom::getInstance();

		zp_int numToSpawn = spawnGroup.minNumToSpawn;
		if( spawnGroup.minNumToSpawn < spawnGroup.maxNumToSpawn )
		{
			numToSpawn = rnd->randomInt( spawnGroup.minNumToSpawn, spawnGroup.maxNumToSpawn );
		}

		zp_int objectCount = spawnGroup.objectsToSpawn.size();
		zpTransformComponent* t = getParentObject()->getComponents()->getTransformComponent();

		for( zp_int i = 0; i < numToSpawn; ++i )
		{
			spawnGroup.numSpawnedFromGroup++;

			zp_int objNameIndex = 0;
			if( spawnGroup.spawnRandomObject )
			{
				objNameIndex = rnd->randomInt( 0, objectCount );
			}
			else if( spawnGroup.repeatSpawnedObjects )
			{
				objNameIndex = spawnGroup.numSpawnedFromGroup % objectCount;
			}
			else
			{
				objNameIndex = ZP_MIN( spawnGroup.numSpawnedFromGroup, objectCount );
			}

			const zpString& objName = spawnGroup.objectsToSpawn[ objNameIndex ];

			zpVector4f pos( spawnGroup.position );
			if( t != ZP_NULL )
			{
				pos = zpMath::MatrixTransform( t->getWorldTransform(), pos );
			}

			zpObject* obj = spawnObjectAt( objName, pos, spawnGroup.rotation );

			if( spawnGroup.trackSpawnedObjects )
			{
				m_objectsSpawned.pushBack( obj );
			}
		}
	}
}
zpObject* zpSpawnerComponent::spawnObjectAt( const zpString& object, const zpVector4f& position, const zpQuaternion4f& rotation )
{
	zpObject* obj = getApplication()->getObjectContentManager()->createObject( object.str() );

	zpTransformComponent* t = obj->getComponents()->getTransformComponent();
	if( t != ZP_NULL )
	{
		t->setLocalTransform( position, rotation );
	}

	return obj;
}



zpSpawnerComponentPool::zpSpawnerComponentPool()
{}
zpSpawnerComponentPool::~zpSpawnerComponentPool()
{}

void zpSpawnerComponentPool::update( zp_float deltaTime, zp_float realTime )
{
	m_used.foreach( [ &deltaTime, &realTime ]( zpSpawnerComponent* o ) {
		o->update( deltaTime, realTime );
	} );
}
