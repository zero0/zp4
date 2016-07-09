#include "zpCommon.h"

zp_bool zpWorldResource::load( const zp_char* filename )
{
    m_filename = filename;

    zp_bool loaded = false;
    loaded = m_resource.readFromFile( m_filename );
    return loaded;
}
void zpWorldResource::unload()
{
    m_filename.clear();
}

zp_bool zpWorldContentManager::createResource( zpWorldResource* res, const zp_char* filename )
{
    return res->load( filename );
}
void zpWorldContentManager::destroyResource( zpWorldResource* res )
{
    res->unload();
}
zpWorld* zpWorldContentManager::createWorld( const zp_char* filename, zp_bool destroyAfterLoad )
{
    zpWorld* w = ZP_NULL;

    zpWorldResourceInstance world;
    if( getResource( filename, world ) )
    {
        w = create( getApplication(), world );
        w->setFlag( ZP_WORLD_FLAG_SHOULD_CREATE );

        if( destroyAfterLoad )
        {
            w->setFlag( ZP_WORLD_FLAG_DESTROY_AFTER_LOAD );
        }
    }

    return w;
}
void zpWorldContentManager::destroyAllWorlds()
{
    zpWorld** b = m_used.begin();
    zpWorld** e = m_used.end();
    for( ; b != e; ++b )
    {
        (*b)->setFlag( ZP_WORLD_FLAG_SHOULD_DESTROY );
    }
}

void zpWorldContentManager::update()
{
    if( !m_used.isEmpty() )
    {
        zpWorld** b = m_used.begin();
        zpWorld** e = m_used.end();
        for( ; b != e; ++b )
        {
            zpWorld* world = *b;
            if( world->isFlagSet( ZP_WORLD_FLAG_SHOULD_CREATE ) )
            {
                if( world->createStep() )
                {
                    world->unsetFlag( ZP_WORLD_FLAG_SHOULD_CREATE );
                    world->setFlag( ZP_WORLD_FLAG_CREATED );

                    if( world->isFlagSet( ZP_WORLD_FLAG_DESTROY_AFTER_LOAD ) )
                    {
                        world->setFlag( ZP_WORLD_FLAG_SHOULD_DESTROY );
                    }
                }
            }
            else if( world->isFlagSet( ZP_WORLD_FLAG_SHOULD_DESTROY ) )
            {
                destroy( world );
            }
        }
    }
}

zpWorld::zpWorld( zpApplication* application, const zpWorldResourceInstance& res )
    : m_application( application )
    , m_flags( 0 )
    , m_numObjectsLoaded( 0 )
    , m_totalObjects( 0 )
    , m_world( res )
{
    if( m_world.isVaild() )
    {
        m_objects = m_world.getResource()->getData()->root()[ "Objects" ];
        m_totalObjects = m_objects.size();
    }
    else
    {
        m_totalObjects = 1;
    }
}
zpWorld::~zpWorld()
{
    destroy();
}

zp_bool zpWorld::isFlagSet( zpWorldFlag flag ) const
{
    return m_flags.isMarked( flag );
}
void zpWorld::setFlag( zpWorldFlag flag )
{
    m_flags.mark( flag );
}
void zpWorld::unsetFlag( zpWorldFlag flag )
{
    m_flags.unmark( flag );
}

zp_bool zpWorld::createStep()
{
    if( isFlagSet( ZP_WORLD_FLAG_STEP_CREATE ) )
    {
        const zpBison::Value& v = m_objects[ m_numObjectsLoaded ];
        createWorldObject( v );

        ++m_numObjectsLoaded;
    }
    else
    {
        m_numObjectsLoaded = m_totalObjects;

        m_objects.foreachArray( [ this ]( const zpBison::Value& v )
        {
            createWorldObject( v );
        } );
    }

    return m_numObjectsLoaded == m_totalObjects;
}
void zpWorld::destroy()
{
    m_objects = zpBison::null;
}

const zpString& zpWorld::getName() const
{
    return m_name;
}
zpApplication* zpWorld::getApplication() const
{
    return m_application;
}

zp_uint zpWorld::getTotalObjects() const
{
    return m_totalObjects;
}
zp_uint zpWorld::getNumObjectsLoaded() const
{
    return m_numObjectsLoaded;
}
zp_float zpWorld::getLoadingProgress() const
{
    return (zp_float)m_numObjectsLoaded / (zp_float)m_totalObjects;
}

void zpWorld::createWorldObject( const zpBison::Value& def )
{
    zpObject* o;
    const zpBison::Value& obj = def[ "Object" ];

    // if object defined in world, load external object
    if( obj.isString() )
    {
        const zp_char* objFile = obj.asCString();
        o = m_application->getObjectContentManager()->createObject( objFile );

        // set the transform of the object that is loaded
        const zpBison::Value& transform = def[ "Transform" ];
        if( !transform.isEmpty() )
        {
            zpTransformComponent* t = o->getComponents()->getTransformComponent();
            if( t != ZP_NULL )
            {
                const zpBison::Value& position = transform[ "Position" ];
                const zpBison::Value& rotation = transform[ "Rotation" ];
                const zpBison::Value& scale = transform[ "Scale" ];

                // position
                if( position.isArray() && !position.isEmpty() )
                {
                    zpVector4f pos = zpMath::Vector4( position[ 0 ].asFloat(), position[ 1 ].asFloat(), position[ 2 ].asFloat(), 1.f );
                    t->setLocalPosition( pos );
                }

                // rotation
                if( rotation.isArray() && !rotation.isEmpty() )
                {
                    zpScalar yaw   = zpMath::Scalar( rotation[ 0 ].asFloat() );
                    zpScalar pitch = zpMath::Scalar( rotation[ 1 ].asFloat() );
                    zpScalar roll  = zpMath::Scalar( rotation[ 2 ].asFloat() );

                    zpQuaternion4f rot;
                    rot = zpMath::QuaternionFromEulerAngle( yaw, pitch, roll );
                    t->setLocalRotation( rot );
                }

                // scale
                if( scale.isArray() && !scale.isEmpty() )
                {
                    zpVector4f scl = zpMath::Vector4( scale[ 0 ].asFloat(), scale[ 1 ].asFloat(), scale[ 2 ].asFloat(), 1.f );
                    t->setLocalScale( scl );
                }
            }
        }
    }
    // otherwise, load the object inline
    else
    {
        o = m_application->getObjectContentManager()->createObject( def );
    }
    
    // set the world if it does not need to be destroyed after load
    if( !isFlagSet( ZP_WORLD_FLAG_DESTROY_AFTER_LOAD ) )
    {
        o->setWorld( this );
    }
}
