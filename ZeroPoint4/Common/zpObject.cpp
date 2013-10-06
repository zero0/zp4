#include "zpCommon.h"

zp_bool zpObjectResource::load( const zp_char* filename )
{
	m_filename = filename;

	zp_bool loaded = false;
	loaded = m_resource.readFromFile( m_filename );
	return loaded;
}
void zpObjectResource::unload()
{
	m_filename.clear();
}

zpObject* zpObjectContentManager::createObject( zpApplication* application, const zp_char* filename )
{
	zpObject* o = ZP_NULL;

	zpObjectResourceInstance obj;
	if( getResource( filename, obj ) )
	{
		o = create( application, obj );
	}

	return o;
}

void zpObjectContentManager::update()
{
	m_used.foreach( [ this ]( zpObject* o )
	{
		if( o->isFlagSet( ZP_OBJECT_FLAG_SHOULD_DESTROY ) )
		{
			destroy( o );
		}
	} );
}

zp_bool zpObjectContentManager::createResource( zpObjectResource* res, const zp_char* filename )
{
	return res->load( filename );
}
void zpObjectContentManager::destroyResource( zpObjectResource* res )
{
	res->unload();
}

zpObject::zpObject( zpApplication* application, const zpObjectResourceInstance& res )
	: m_transform()
	, m_name()
	, m_flags( ZP_OBJECT_FLAG_ENABLED )
	, m_lastLoadTime( 0 )
	, m_components()
	, m_world( ZP_NULL )
	, m_application( application )
	, m_object( res )
{
	loadObject();
}
zpObject::~zpObject()
{
	unloadObject();
}

zpAllComponents* zpObject::getComponents()
{
	return &m_components;
}

void zpObject::setFlag( zpObjectFlag flag )
{
	m_flags.mark( flag );
	if( flag == ZP_OBJECT_FLAG_SHOULD_DESTROY )
	{
		m_flags.unmark( ZP_OBJECT_FLAG_ENABLED );
	}
}
void zpObject::unsetFlag( zpObjectFlag flag )
{
	m_flags.unmark( flag );
}
zp_bool zpObject::isFlagSet( zpObjectFlag flag ) const
{
	return m_flags.isMarked( flag );
}

const zpString& zpObject::getName() const
{
	return m_name;
}
void zpObject::setName( const zpString& name )
{
	m_name = name;
}

const zpMatrix4f& zpObject::getTransform() const
{
	return m_transform;
}
void zpObject::setTransform( const zpMatrix4f& transform )
{
	m_transform = transform;
}

zpWorld* zpObject::getWorld() const
{
	return m_world;
}

void zpObject::setWorld( zpWorld* world )
{
	m_world = world;
}

zpApplication* zpObject::getApplication() const
{
	return m_application;
}
void zpObject::setApplication( zpApplication* application )
{
	m_application = application;
}

void zpObject::update()
{
	if( m_object.isVaild() && m_lastLoadTime != m_object.getResource()->getLastTimeLoaded() )
	{
		unloadObject();
		loadObject();
	}
}

void zpObject::loadObject()
{
	if( m_object.isVaild() )
	{
		m_lastLoadTime = m_object.getResource()->getLastTimeLoaded();

		const zpBison::Value& root = m_object.getResource()->getData()->root();
		
		m_name = root[ "Name" ].asCString();

		const zpBison::Value tags = root[ "Tags" ];
		const zpBison::Value layer = root[ "Layer" ];
		const zpBison::Value transform = root[ "Transform" ];
		const zpBison::Value components = root[ "Components" ];

		m_transform.setIdentity();
		if( !transform.isEmpty() )
		{
			m_transform = *(zpMatrix4f*)transform.asData();
		}

		m_components.setApplication( m_application );
		components.foreachObject( [ this ]( const zpBison::Value& key, const zpBison::Value& value )
		{
			m_components.load( this, key.asCString(), value );
		} );
	}
	else
	{
		setFlag( ZP_OBJECT_FLAG_SHOULD_DESTROY );
	}
}
void zpObject::unloadObject()
{
	m_components.unload();
}
