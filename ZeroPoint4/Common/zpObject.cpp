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

zpObjectContentManager::zpObjectContentManager()
	: m_numObjects( 0 )
{}
zpObjectContentManager::~zpObjectContentManager()
{}

zpObject* zpObjectContentManager::createObject()
{
	zpObject* o = create( getApplication() );
	o->setID( ++m_numObjects );

	return o;
}
zpObject* zpObjectContentManager::createObject( const zp_char* filename )
{
	zpObject* o = ZP_NULL;

	zpObjectResourceInstance obj;
	zp_bool ok = getResource( filename, obj );
	ZP_ASSERT( ok, "Failed to get %s", filename );

	o = create( getApplication(), obj );
	o->setID( ++m_numObjects );

	return o;
}
zpObject* zpObjectContentManager::createObject( const zpBison::Value& def )
{
	zpObject* o = ZP_NULL;

	o = create( getApplication(), def );
	o->setID( ++m_numObjects );

	return o;
}

void zpObjectContentManager::initializeAllObjectsInWorld( zpWorld* world )
{
	zpObject** b = m_used.begin();
	zpObject** e = m_used.end();
	for( ; b != e; ++b )
	{
		zpObject* o = *b;
		if( o->getWorld() == world )
		{
			o->initialize();
		}
	}
}

void zpObjectContentManager::destroyAllObjects()
{
	m_used.foreach( []( zpObject* o ) {
		o->destroy();
	} );
}
void zpObjectContentManager::destroyAllObjectsInWorld( zpWorld* world )
{
	zpObject** b = m_used.begin();
	zpObject** e = m_used.end();
	for( ; b != e; ++b )
	{
		zpObject* o = *b;
		if( o->getWorld() == world && !o->isFlagSet( ZP_OBJECT_FLAG_DONT_DESTROY_ON_UNLOAD ) )
		{
			o->destroy();
		}
	}
}

void zpObjectContentManager::getAllObjectsInLayer( zp_uint layer, zpArrayList< zpObject* >& objects ) const
{
	objects.clear();

	zpObject*const * b = m_used.begin();
	zpObject*const * e = m_used.end();
	for( ; b != e; ++b )
	{
		zpObject* o = *b;
		if( o->getLayers().isMarked( layer ) )
		{
			objects.pushBack( o );
		}
	}
}
void zpObjectContentManager::getAllObjectsWithTag( zp_int tag, zpArrayList< zpObject* >& objects ) const
{
	objects.clear();

	zpObject*const * b = m_used.begin();
	zpObject*const * e = m_used.end();
	for( ; b != e; ++b )
	{
		zpObject* o = *b;
		if( o->getTags().isMarked( tag ) )
		{
			objects.pushBack( o );
		}
	}
}

void zpObjectContentManager::update()
{
	zpObject** b = m_used.begin();
	zpObject** e = m_used.end();
	for( ; b != e; ++b )
	{
		zpObject* o = *b;
		if( !o->isFlagSet( ZP_OBJECT_FLAG_INITIALIZED ) )
		{
			o->initialize();
		}
		if( o->isFlagSet( ZP_OBJECT_FLAG_CAN_UPDATE ) )
		{
			o->update();
		}
		if( o->isFlagSet( ZP_OBJECT_FLAG_SHOULD_DESTROY ) )
		{
			destroy( o );
		}
	}
}
void zpObjectContentManager::simulate()
{
	zpObject** b = m_used.begin();
	zpObject** e = m_used.end();
	for( ; b != e; ++b )
	{
		zpObject* o = *b;
		if( o->isFlagSet( ZP_OBJECT_FLAG_CAN_UPDATE ) )
		{

		}
	}
}

zp_bool zpObjectContentManager::createResource( zpObjectResource* res, const zp_char* filename )
{
	return res->load( filename );
}
void zpObjectContentManager::destroyResource( zpObjectResource* res )
{
	res->unload();
}

zpObject::zpObject( zpApplication* application )
	: m_name()
	, m_tags()
	, m_flags()
	, m_lastLoadTime( 0 )
	, m_prefabId( 0 )
	, m_instanceId( 0 )
	, m_components()
	, m_application( application )
	, m_world( ZP_NULL )
	, m_object()
{
	setFlag( ZP_OBJECT_FLAG_ENABLED );
	load( true, zpBison::null );
}
zpObject::zpObject( zpApplication* application, const zpObjectResourceInstance& res )
	: m_name()
	, m_tags()
	, m_flags()
	, m_lastLoadTime( res.getResource()->getLastTimeLoaded() )
	, m_prefabId( 0 )
	, m_instanceId( 0 )
	, m_components()
	, m_application( application )
	, m_world( ZP_NULL )
	, m_object( res )
{
	setFlag( ZP_OBJECT_FLAG_ENABLED );
	load( true, res.getResource()->getData()->root() );
}
zpObject::zpObject( zpApplication* application, const zpBison::Value& root )
	: m_name()
	, m_tags()
	, m_flags()
	, m_lastLoadTime( 0 )
	, m_prefabId( 0 )
	, m_instanceId( 0 )
	, m_components()
	, m_application( application )
	, m_world( ZP_NULL )
	, m_object()
{
	setFlag( ZP_OBJECT_FLAG_ENABLED );
	load( true, root );
}
zpObject::~zpObject()
{
	unload();
}

zpAllComponents* zpObject::getComponents()
{
	return &m_components;
}

zp_uint zpObject::getPrefabID() const
{
	return m_prefabId;
}
zp_uint zpObject::getID() const
{
	return m_instanceId;
}
void zpObject::setID( zp_uint id )
{
	m_instanceId = id;
}

void zpObject::setFlag( zpObjectFlag flag )
{
	m_flags.mark( flag );
}
void zpObject::unsetFlag( zpObjectFlag flag )
{
	m_flags.unmark( flag );
}
zp_bool zpObject::isFlagSet( zpObjectFlag flag ) const
{
	return m_flags.isMarked( flag );
}

void zpObject::setEnabled( zp_bool enabled )
{
	m_flags.setMarked( ZP_OBJECT_FLAG_ENABLED, enabled );
	m_components.setEnabled( enabled );
}
zp_bool zpObject::isEnabled() const
{
	return m_flags.isMarked( ZP_OBJECT_FLAG_ENABLED );
}

const zpString& zpObject::getName() const
{
	return m_name;
}
void zpObject::setName( const zpString& name )
{
	m_name = name;
}

const zpFlag64& zpObject::getTags() const
{
	return m_tags;
}
void zpObject::clearTags()
{
	m_tags.clear();
}

zp_bool zpObject::hasTag( const zp_char* tag ) const
{
	zp_int index = m_application->getTags()->getTag( tag );
	return m_tags.isMarked( index );
}
void zpObject::addTag( const zp_char* tag )
{
	zp_int index = m_application->getTags()->getTag( tag );
	m_tags.mark( index );
}
void zpObject::removeTag( const zp_char* tag )
{
	zp_int index = m_application->getTags()->getTag( tag );
	m_tags.unmark( index );
}

zp_bool zpObject::hasTag( const zpString& tag ) const
{
	return hasTag( tag.str() );
}
void zpObject::addTag( const zpString& tag )
{
	addTag( tag.str() );
}
void zpObject::removeTag( const zpString& tag )
{
	removeTag( tag.str() );
}

const zpFlag32& zpObject::getLayers() const
{
	return m_layers;
}
void zpObject::setLayers( const zpFlag32& layers )
{
	m_layers = layers;
}
zp_bool zpObject::isOnLayer( zp_uint layer ) const
{
	return m_layers.isMarked( layer );
}

zpApplication* zpObject::getApplication() const
{
	return m_application;
}

zpWorld* zpObject::getWorld() const
{
	return m_world;
}
void zpObject::setWorld( zpWorld* world )
{
	m_world = world;
}

void zpObject::initialize()
{
	if( m_flags.isMarked( ZP_OBJECT_FLAG_CREATED ) && !m_flags.isMarked( ZP_OBJECT_FLAG_INITIALIZED ) )
	{
		m_flags.mark( ZP_OBJECT_FLAG_INITIALIZED );
		m_components.initialize();

		// use object's enabled flag
		zp_bool en = isFlagSet( ZP_OBJECT_FLAG_ENABLED );
		m_components.setEnabled( en );
	}
}
void zpObject::update()
{
	m_flags.unmark( ZP_OBJECT_FLAG_TRANSFORM_DIRTY );
#if ZP_USE_HOT_RELOAD
	if( m_object.isVaild() && m_lastLoadTime != m_object.getResource()->getLastTimeLoaded() )
	{
		unload();
		load( false, m_object.getResource()->getData()->root() );
	}
#endif
}
void zpObject::destroy()
{
	m_flags.unmark( ZP_OBJECT_FLAG_CREATED );
	m_flags.unmark( ZP_OBJECT_FLAG_INITIALIZED );

	m_flags.mark( ZP_OBJECT_FLAG_SHOULD_DESTROY );

	setEnabled( false );

	m_world = ZP_NULL;
}

void zpObject::load( zp_bool isInitialLoad, const zpBison::Value& root )
{
	const zpBison::Value& id = root[ "_id" ];
	if( id.isIntegral() )
	{
		m_prefabId = (zp_uint)id.asInt();
	}

	// set the name of the object if given
	const zpBison::Value& name = root[ "Name" ];
	if( name.isString() )
	{
		m_name = name.asCString();
	}

	// set layer
	const zpBison::Value& layer = root[ "Layer" ];
	if( layer.isIntegral() )
	{
		m_layers.set( layer.asInt() );
	}

	// set tag
	const zpBison::Value& tags = root[ "Tags" ];
	if( tags.isString() )
	{
		addTag( tags.asCString() );
	}
	// set tag array
	else if( tags.isArray() )
	{
		tags.foreachArray( [ this ]( const zpBison::Value& v ) {
			if( v.isString() )
			{
				addTag( v.asCString() );
			}
		} );
	}

	// for initial load, set static and enabled flag
	if( isInitialLoad )
	{
		// set static flag
		const zpBison::Value& isStatic = root[ "IsStatic" ];
		if( isStatic.isBool() )
		{
			m_flags.setMarked( ZP_OBJECT_FLAG_STATIC, isStatic.asBool() );
		}

		// override default enabled status if given
		const zpBison::Value& enabled = root[ "Enabled" ];
		if( enabled.isBool() )
		{
			m_flags.setMarked( ZP_OBJECT_FLAG_ENABLED, enabled.asBool() );
		}
	}

	// setup components with app and object
	m_components.setup( m_application, this );

	// add and create components
	const zpBison::Value& components = root[ "Components" ];
	components.foreachObject( [ this ]( const zpBison::Value& key, const zpBison::Value& value )
	{
		m_components.create( key.asCString(), value );
	} );

	m_flags.mark( ZP_OBJECT_FLAG_CREATED );
}

void zpObject::unload()
{
	m_flags.unmark( ZP_OBJECT_FLAG_CREATED );
	m_flags.unmark( ZP_OBJECT_FLAG_INITIALIZED );

	m_components.unload();
	clearTags();
}
