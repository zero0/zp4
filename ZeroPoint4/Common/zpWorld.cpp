#include "zpCommon.h"

zpWorld::zpWorld() :
	m_isEnabled( true ),
	m_isCreated( false ),
	m_game( ZP_NULL )
{}
zpWorld::~zpWorld() {}

void zpWorld::update() {
	if( m_isEnabled )
	{
		m_gameObjects.foreach( []( zpGameObject* go ) {
			go->update();
		} );
	}
}
void zpWorld::simulate() {
	if( m_isEnabled )
	{
		m_gameObjects.foreach( []( zpGameObject* go ) {
			go->simulate();
		} );
	}
}


void zpWorld::create() {
	if( m_isCreated ) return;
	m_isCreated = true;

	m_gameObjects.foreach( []( zpGameObject* go ) {
		go->create();
	} );
}
void zpWorld::destroy() {
	if( !m_isCreated ) return;
	m_isCreated = false;

	m_gameObjects.foreach( []( zpGameObject* go ) {
		go->destroy();
	} );
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
	//if( m_root ) m_root->receiveMessage( message );
}

void zpWorld::serialize( zpSerializedOutput* out )
{
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	out->writeString( m_name, "@name" );

	//out->writeSerializable( m_root );

	out->endBlock();
}
void zpWorld::deserialize( zpSerializedInput* in )
{
	if( in->readBlock( ZP_SERIALIZE_TYPE_THIS ) )
	{
		in->readString( &m_name, "@name" );

		in->readEachBlock( "GameObject", [ this ]( zpSerializedInput* in ) {
			zpString goFile;
			in->readString( &goFile, "@file" );

			zpMatrix4f transform;
			zpSerializableObject<zpMatrix4f>::deserializeToBlock( in, "Transform", transform );


		} );

		in->endBlock();
	}
}
