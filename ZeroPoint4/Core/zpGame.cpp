#include "zpCore.h"

zpGame::zpGame() :
	m_currentWorld( ZP_NULL ),
	m_nextWorld( ZP_NULL )
{}
zpGame::~zpGame() {}

void zpGame::addWorld( zpWorld* world ) {
	if( world )	{
		m_worlds.prepend( world );
		world->addReference();
	}
	if( !m_currentWorld ) m_nextWorld = world;
}
void zpGame::removeWorld( zpWorld* world ) {
	if( world ) {
		m_worlds.detatch( world );

		if( m_currentWorld == world ) {
			m_currentWorld = ZP_NULL;
			m_nextWorld = m_worlds.getNext()->m_parent;
		}
		
		world->removeReference();
	}
}
zpWorld* zpGame::getCurrentWorld() const {
	return m_currentWorld;
}

void zpGame::addGameManager( zpGameManager* manager ) {
	if( manager ) {
		m_managers.prepend( manager );
		manager->addReference();
	}
}
void zpGame::removeGameManager( zpGameManager* manager ) {
	if( manager ) {
		m_managers.detatch( manager );
		manager->removeReference();
	}
}

void zpGame::update() {
	m_managers.foreach( []( zpGameManager* manager ) {
		manager->update();
	} );

	if( m_currentWorld ) m_currentWorld->update();

	if( m_nextWorld ) {
		m_currentWorld = m_nextWorld;
		m_nextWorld = ZP_NULL;
	}
}