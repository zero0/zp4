#include "zpCore.h"
#include <typeinfo>

zpGame::zpGame() :
	m_currentWorld( ZP_NULL ),
	m_nextWorld( ZP_NULL ),
	m_renderable( ZP_NULL ),
	m_window( ZP_NULL ),
	m_asynchCreateNextWorld( false )
{}
zpGame::~zpGame() {}

void zpGame::create() {
	m_managers.foreach( []( zpGameManager* manager ) {
		manager->create();
	} );
}
void zpGame::destroy() {
	m_managers.foreach( []( zpGameManager* manager ) {
		manager->destroy();
	} );
}

void zpGame::addWorld( zpWorld* world, zp_bool andCreate ) {
	if( world )	{
		m_worlds.pushBack( world );
		world->addReference();
		world->setGame( this );
		if( andCreate ) world->create();
	}
	if( !m_currentWorld ) m_nextWorld = world;
}
void zpGame::removeWorld( zpWorld* world ) {
	if( world ) {
		m_worlds.remove( world );

		if( m_currentWorld == world ) {
			m_currentWorld = ZP_NULL;
			m_nextWorld = (zpWorld*)m_worlds.front();
		}
		
		world->removeReference();
		world->setGame( ZP_NULL );
	}
}
zpWorld* zpGame::getCurrentWorld() const {
	return m_currentWorld;
}

void zpGame::addGameManager( zpGameManager* manager ) {
	if( manager ) {
		m_managers.pushBack( manager );
		manager->addReference();
		manager->setGame( this );
	}
}
void zpGame::removeGameManager( zpGameManager* manager ) {
	if( manager ) {
		m_managers.remove( manager );
		manager->removeReference();
		manager->setGame( ZP_NULL );
	}
}

void zpGame::setNextWorld( const zpString& worldName, zp_bool asynchCreateNextWorld ) {
	zpWorld* world = m_worlds.findFirstIf( [ &worldName ]( zpWorld* world ) {
		return world->getName() == worldName;
	} );
	if( world && world != m_currentWorld ) {
		m_nextWorld = world;
		m_asynchCreateNextWorld = asynchCreateNextWorld;
	}
}

void zpGame::setRenderable( zpRenderable* renderable ) {
	m_renderable = renderable;
}
zpRenderable* zpGame::getRenderable() const {
	return m_renderable;
}

void zpGame::process() {
	zpTime::getInstance()->tick();

	m_managers.foreach( []( zpGameManager* manager ) {
		manager->update();
	} );

	if( m_currentWorld ) m_currentWorld->update();

	if( m_renderable ) m_renderable->render();
	
	if( m_nextWorld ) {
		if( m_currentWorld ) m_currentWorld->receiveMessage( 0/* leave */ );
		if( m_nextWorld ) m_nextWorld->receiveMessage( 0/* enter */ );

		m_currentWorld = m_nextWorld;

		if( m_currentWorld ) m_currentWorld->receiveMessage( 0/* enter */ );

		m_nextWorld = ZP_NULL;
		m_asynchCreateNextWorld = false;
	}
}

void zpGame::setWindow( zpWindow* window ) {
	m_window = window;
}
zpWindow* zpGame::getWindow() const {
	return m_window;
}

zpGameManager* zpGame::getGameManager_T( const void* type ) const {
	const type_info& typeinfo = *(const type_info*)type;
	zpGameManager* manager = m_managers.findFirstIf( [ &typeinfo ]( const zpGameManager* manager ) {
		return typeinfo == typeid( *manager );
	} );
	return manager;
}

void zpGame::serialize( zpSerializedOutput* out ) {}
void zpGame::deserialize( zpSerializedInput* in ) {}