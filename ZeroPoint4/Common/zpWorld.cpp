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
zpWorld* zpWorldContentManager::createWorld( zpApplication* application, const zp_char* filename )
{
	zpWorld* w = ZP_NULL;

	zpWorldResourceInstance world;
	if( getResource( filename, world ) )
	{
		w = create( application, world );
	}

	return w;
}
void zpWorldContentManager::update()
{
	if( !m_used.isEmpty() )
	{
		for( zp_int i = 0; i < (zp_int)m_used.size(); ++i )
		{
			zpWorld* world = m_used[ i ];
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
				--i;
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
	zpBison::Value objects = m_world.getResource()->getData()->root()[ "Objects" ];
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
			const zp_char* objFile = v.asCString();
			m_application->getObjectContentManager()->createObject( m_application, objFile );
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
