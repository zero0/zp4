#include "zpCommon.h"

zp_bool zpPrefabResource::load( const zp_char* filename )
{
    m_filename = filename;

    zp_bool loaded = false;
    loaded = m_resource.readFromFile( m_filename );
    return loaded;
}

void zpPrefabResource::unload()
{
    m_resource.clear();
}


void zpPrefabContentManager::instantiatePrefab( const zpPrefabResourceInstance& prefab )
{
    const zpBison::Value& root = prefab.getResource()->getData()->root();

    const zpBison::Value& objects = root[ "Objects" ];
    const zpBison::Value& components = root[ "Components" ];

    zpObjectContentManager* objManager = getApplication()->getObjectContentManager();

    zpArrayList< zpObject* > createdObjects;
    if( !objects.isEmpty() )
    {
        createdObjects.reserve( objects.size() );

        objects.foreachArray( [ objManager ]( const zpBison::Value& val ) {
            zpObject* obj = objManager->createObject( val );

        } );

        createdObjects.sort( []( zpObject* a, zpObject* b ) {
            return a->getPrefabID() < b->getPrefabID();
        } );
    }

    if( !components.isEmpty() )
    {
        components.foreachArray( [ objManager, &createdObjects ]( const zpBison::Value& val ) {
            zp_size_t index;
            zp_uint id = val[ "_object_id" ].asInt();

            if( createdObjects.findIndexIf( [ id ]( zpObject* o ) {
                return o->getPrefabID() == id;
            }, index ) )
            {
                zpObject* obj = createdObjects[ index ];
            }
        } );
    }
}

zp_bool zpPrefabContentManager::saveObjectAsPrefab( zpObject* obj, const zp_char* filename )
{
    zp_bool ok = false;
    return ok;
}
zp_bool zpPrefabContentManager::saveObjectsAsPrefab( const zpArrayList< zpObject* >& objs, const zp_char* filename )
{
    zp_bool ok = false;
    return ok;
}

zp_bool zpPrefabContentManager::createResource( zpPrefabResource* res, const zp_char* filename )
{
    return res->load( filename );
}
void zpPrefabContentManager::destroyResource( zpPrefabResource* res )
{
    res->unload();
}
