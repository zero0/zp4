#include "zpCore.h"

zpWorld::zpWorld() :
	m_isEnabled( true ),
	m_isCreated( false ),
	m_referenceCount( 1 ),
	m_isMarkedForAutoDelete( false ),
	m_root( ZP_NULL ),
	m_game( ZP_NULL )
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

zp_bool zpWorld::isCreated() const {
	return m_isCreated;
}

void zpWorld::setEnabled( zp_bool enabled ) {
	m_isEnabled = enabled;
}
zp_bool zpWorld::isEnabled() const {
	return m_isEnabled;
}

const zpString& zpWorld::getName() const {
	return m_name;
}
void zpWorld::setName( const zpString& name ) {
	m_name = name;
}

void zpWorld::setGame( zpGame* game ) {
	m_game = game;
}
zpGame* zpWorld::getGame() const {
	return m_game;
}

void zpWorld::receiveMessage( const zpMessage& message ) {
	if( m_root ) m_root->receiveMessage( message );
}

void zpWorld::serialize( zpSerializedOutput* out ) {}
void zpWorld::deserialize( zpSerializedInput* in ) {}

void zpWorld::addReference() const {
	++m_referenceCount;
}
zp_bool zpWorld::removeReference() const {
	--m_referenceCount;
	if( m_referenceCount == 0 ) {
		if( m_isMarkedForAutoDelete ) delete this;
		return true;
	}
	return false;
}

zp_uint zpWorld::getReferenceCount() const {
	return m_referenceCount;
}

void zpWorld::markForAutoDelete( zp_bool marked ) const {
	m_isMarkedForAutoDelete = marked;
}
zp_bool zpWorld::isMarkedForAutoDelete() const {
	return m_isMarkedForAutoDelete;
}