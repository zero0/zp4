#include "zpContent.h"

zpGameObjectResource::zpGameObjectResource() {}
zpGameObjectResource::~zpGameObjectResource() {
	unload();
	m_instances.clear();
}

zp_bool zpGameObjectResource::load() {
	zpXmlParser parser;
	parser.setRootNode( &m_root, false );
	if( parser.parseFile( getFilename(), true ) ) {
		m_instances.foreach( [ this ]( zpGameObject* go ) {
			zpXmlSerializedInput in( &m_root, zpProperties() );
			go->deserialize( &in );
			go->create();
		} );
		return true;
	}
	return false;
}
void zpGameObjectResource::unload() {
	m_root.children.clear();
	m_instances.foreach( []( zpGameObject* go ) {
		go->destroy();
	} );
}

void zpGameObjectResource::addInstance( zpGameObject* go ) {
	m_instances.pushBack( go );
}
void zpGameObjectResource::removeInstance( zpGameObject* go ) {
	m_instances.eraseAll( go );
}

zpXmlNode* zpGameObjectResource::getRoot() {
	return &m_root;
}