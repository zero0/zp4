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
		m_totalObjects = m_world.getResource()->getData()->root()[ "Objects" ].size();
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
	const zpBison::Value& objects = m_world.getResource()->getData()->root()[ "Objects" ];
	if( isFlagSet( ZP_WORLD_FLAG_STEP_CREATE ) )
	{
		const zp_char* objFile = objects[ m_numObjectsLoaded ].asCString();
		m_application->getObjectContentManager()->createObject( m_application, objFile );

		++m_numObjectsLoaded;
	}
	else
	{
		m_numObjectsLoaded = m_totalObjects;

		objects.foreachArray( [ this ]( const zpBison::Value& v )
		{
			zpObject* o;
			const zp_char* objFile = v[ "Object" ].asCString();
			o = m_application->getObjectContentManager()->createObject( m_application, objFile );

			const zpBison::Value& transform = v[ "Transform" ];
			if( !transform.isEmpty() )
			{
				const zpMatrix4f& mat = *(const zpMatrix4f*)transform.asData();
				o->setTransform( mat );
			}
		} );
	}

	return m_numObjectsLoaded == m_totalObjects;
}
void zpWorld::destroy()
{
}

const zpString& zpWorld::getName() const
{
	return m_name;
}
zpApplication* zpWorld::getApplication() const
{
	return m_application;
}
