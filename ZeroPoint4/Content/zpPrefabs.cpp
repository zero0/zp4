#include "zpContent.h"

zpPrefabResource::zpPrefabResource()
	: m_root()
{}
zpPrefabResource::~zpPrefabResource() {}

zp_bool zpPrefabResource::load() {
	zpXmlParser parser;
	parser.setRootNode( &m_root, false );

	return parser.parseFile( getFilename(), true );
}
void zpPrefabResource::unload() {
	m_root.children.clear();
}

zpXmlNode* zpPrefabResource::getRootNode() {
	return &m_root;
}

const zpProperties& zpResourceInstance<zpPrefabResource>::getOverrides() const {
	return m_overrides;
}

void zpResourceInstance<zpPrefabResource>::copy( const zpResourceInstance<zpPrefabResource>& c ) {
	m_overrides = c.m_overrides;
}

zpPrefabResourceCreator::zpPrefabResourceCreator() {}
zpPrefabResourceCreator::~zpPrefabResourceCreator() {}

zpResource* zpPrefabResourceCreator::createResource( const zpString& filename ) {
	zpResource* resource = ZP_NULL;
	if( filename.endsWith( ".prefab" ) ) {
		resource = new zpPrefabResource;
	}
	return resource;
}


zpPrefabs::zpPrefabs()
	: m_contentManager( ZP_NULL )
	, m_prefabRoot( "prefabs/" )
{}
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
	return loadPrefabWithOverrides( prefabName, zpProperties() );
}
zpSerializable* zpPrefabs::loadPrefabWithOverrides( const zpString& prefabName, const zpProperties& overrides ) {
	zpSerializable* serializable = ZP_NULL;
	zp_bool lazyLoad = false;
	if( m_contentManager ) {
		//zpString root;
		//m_contentManager->getRootDirectoryForExtension( "prefab", root );

		zpStringBuffer buff;
		buff << prefabName << ".prefab";
		zpString prefabFile = buff.toString();

		zpPrefabResource* resource = ZP_NULL;
		zpResourceInstance<zpPrefabResource>* prefabInstance;
		if( m_prefabs.find( prefabName, &prefabInstance ) ) {
			resource = prefabInstance->getResource();
		} else {
			// load the prefab immediately so it can be used later
			//m_contentManager->loadResource( prefabFile, prefabName, true );

			m_prefabs[ prefabName ] = m_contentManager->createInstanceOfResource<zpPrefabResource>( prefabFile );
			resource = m_prefabs[ prefabName ].getResource();
		}

		// create a serializable that will be de-serialized later
		serializable = zpRegisterSerializable::createSerializable( resource->getRootNode()->children[ 0 ]->name );

		if( lazyLoad ) {
			// store this instance for later parsing
			zpPrefabToLoad load = { resource, serializable, overrides };
			m_prefabsToLoad.pushBack( load );
		} else {
			zpXmlSerializedInput in( resource->getRootNode(), overrides );
			in.readSerializable( &serializable, ZP_NULL );
		}
	}

	// return the created serializable
	return serializable;
}

zp_bool zpPrefabs::saveAsPrefab( const zpString& prefabName, zpSerializable* obj ) {
	zpStringBuffer buff;
	buff << m_contentManager->getRootDirectory() << m_prefabRoot << prefabName << ".prefab";
	zpString prefabFile = buff.toString();

	zpXmlNode root;
	zpXmlSerializedOutput out( &root );
	obj->serialize( &out );

	return zpXmlParser::writeToFile( &root, prefabFile );
}

void zpPrefabs::setContentManager( zpContentManager* content ) {
	m_prefabCreator.setRootDirectory( m_prefabRoot );
	m_contentManager = content;
	m_contentManager->registerFileExtension( "prefab", &m_prefabCreator );
}
void zpPrefabs::update() {
	if( !m_prefabsToLoad.isEmpty() ) {
		zpPrefabToLoad load = m_prefabsToLoad.back();
		m_prefabsToLoad.popBack();

		zpXmlSerializedInput in( load.prefab->getRootNode(), load.overrides );
		load.serializable->deserialize( &in );
	}
}