#include "zpCore.h"

zpGame::zpGame() :
	m_currentWorld( ZP_NULL ),
	m_nextWorld( ZP_NULL )
{}
zpGame::~zpGame() {}

void zpGame::addWorld( zpWorld* world, zp_bool andCreate ) {
	if( world )	{
		m_worlds.pushBack( world );
		world->addReference();
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
	}
}
zpWorld* zpGame::getCurrentWorld() const {
	return m_currentWorld;
}

void zpGame::addGameManager( zpGameManager* manager ) {
	if( manager ) {
		m_managers.pushBack( manager );
		manager->addReference();
	}
}
void zpGame::removeGameManager( zpGameManager* manager ) {
	if( manager ) {
		m_managers.remove( manager );
		manager->removeReference();
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