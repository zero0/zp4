#include "zpContent.h"

zpWorldResource::zpWorldResource() {}
zpWorldResource::~zpWorldResource() {
	unload();
}

zp_bool zpWorldResource::load() {
	zpXmlParser parser;
	parser.setRootNode( &m_root, false );
	if( parser.parseFile( getFilename(), true ) ) {
		zpXmlSerializedInput in( &m_root, zpProperties() );
		m_world.deserialize( &in );
		m_world.create();
		return true;
	}
	return false;
}
void zpWorldResource::unload() {
	m_root.children.clear();
	m_world.destroy();
}

zpWorld* zpWorldResource::getWorld() {
	return &m_world;
}
