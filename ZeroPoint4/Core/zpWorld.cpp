#include "zpCore.h"

zpWorld::zpWorld() :
	m_isEnabled( false ),
	m_isCreated( false ),
	m_root( ZP_NULL )
{}
zpWorld::~zpWorld() {}

void zpWorld::setRootGameObject( zpGameObject* root ) {
	if( root ) root->addReference();
	if( m_root ) m_root->removeReference();
	m_root = root;
	if( m_root ) m_root->setWorld( this );
}
zpGameObject* zpWorld::getRootGameObject() const {
	return m_root;
}

void zpWorld::update() {
	if( m_isEnabled && m_root ) m_root->update();
}

void zpWorld::create() {
	if( m_isCreated ) return;
	m_isCreated = true;

	if( m_root ) m_root->create();
}
void zpWorld::destroy() {
	if( !m_isCreated ) return;
	m_isCreated = false;

	if( m_root ) m_root->destroy();
}

void zpWorld::setEnabled( zp_bool enabled ) {
	m_isEnabled = enabled;
}
zp_bool zpWorld::isEnabled() const {
	return m_isEnabled;
}