#include "zpCore.h"
#include <typeinfo>

zpGame::zpGame()
	: m_timer( zpTime::getInstance() )
	, m_currentWorld( ZP_NULL )
	, m_nextWorld( ZP_NULL )
	, m_renderable( ZP_NULL )
	, m_window( ZP_NULL )
	, m_lastTime( zpTime::getInstance()->getTime() )
	, m_simulateHz( 10000000 / 30 )
	, m_renderMsHz( 1000 / 120 )
{}
zpGame::~zpGame() {}

void zpGame::create() {
	if( m_window ) m_window->create();

	m_managers.foreach( []( zpGameManager* manager ) {
		manager->create();
	} );
}
void zpGame::destroy() {
	m_managers.foreach( []( zpGameManager* manager ) {
		manager->destroy();
	} );

	if( m_window ) m_window->destroy();
}

void zpGame::addWorld( zpWorld* world, zp_bool andCreate ) {
	if( world )	{
		m_worlds.pushBack( world );
		world->addReference();
		world->setGame( this );
		if( andCreate ) world->create();
	}
	if( m_currentWorld != ZP_NULL ) {
		m_nextWorld = world;
	} else {
		m_currentWorld = world;
	}
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

void zpGame::setNextWorld( const zpString& worldName, zp_bool ) {
	zpWorld* world = m_worlds.findFirstIf( [ &worldName ]( zpWorld* world ) {
		return world->getName() == worldName;
	} );
	if( world && world != m_currentWorld ) {
		m_nextWorld = world;
	}
}

void zpGame::setRenderable( zpRenderable* renderable ) {
	m_renderable = renderable;
}
zpRenderable* zpGame::getRenderable() const {
	return m_renderable;
}

void zpGame::process() {
	while( m_window->processMessages() ) {
		m_timer->tick();
		zp_long now = m_timer->getTime();
		zp_uint numUpdates = 0;

		// update
		m_managers.foreach( []( zpGameManager* manager ) {
			manager->update();
		} );
		if( m_currentWorld ) m_currentWorld->update();

		// simulate
		while( ( now - m_lastTime ) > m_simulateHz && numUpdates < 5 ) {
			m_managers.foreach( []( zpGameManager* manager ) {
				manager->simulate();
			} );
			if( m_currentWorld ) m_currentWorld->simulate();

			m_lastTime += m_simulateHz;
			++numUpdates;
		}

		// adjust timer
		if( ( now - m_lastTime ) > m_simulateHz ) {
			m_lastTime = now - m_simulateHz;
		}

		// render
		m_timer->setInterpolation( (zp_float)( now - m_lastTime ) / (zp_float)m_simulateHz );
		if( m_renderable ) m_renderable->render();

		// sleep for the remainder of the frame
		m_timer->sleep( m_renderMsHz );

		// if there is a next world to set, do it now for the next frame
		if( m_nextWorld ) {
			if( m_currentWorld ) m_currentWorld->receiveMessage( zpMessageTypes::LEAVE_WORLD );
			if( m_nextWorld ) m_nextWorld->receiveMessage( zpMessageTypes::ENTER_WORLD );

			m_currentWorld = m_nextWorld;
			if( !m_currentWorld->isCreated() ) {
				m_currentWorld->create();
			}

			if( m_currentWorld ) m_currentWorld->receiveMessage( zpMessageTypes::ENTER_WORLD );

			m_nextWorld = ZP_NULL;
		}
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

void zpGame::receiveMessage( const zpMessage& message ) {
	switch( message.getMessageType() ) {
	case zpMessageTypes::SYS_SET_NEXT_WORLD:
		setNextWorld( message.getMessageData<zpString>() );
		break;
	case zpMessageTypes::SYS_SET_SCREEN_SIZE:
		m_window->setScreenSize( message.getMessageData<zpVector2i>() );
		break;
	}
}

void zpGame::serialize( zpSerializedOutput* out ) {
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	out->writeSerializable( m_window );

	out->writeBlock( "Managers" );
	{
		out->writeBlock( "Manager" );
		{
			m_managers.foreach( [ out ]( zpGameManager* manager ) {
				manager->serialize( out );
			} );
		}
		out->endBlock();
	}
	out->endBlock();

	out->endBlock();
}
void zpGame::deserialize( zpSerializedInput* in ) {
	in->readBlock( ZP_SERIALIZE_TYPE_THIS );

	in->readSerializableOfType( &m_window );

	if( in->readBlock( "Managers" ) )
	{
		in->readEachBlock( "Manager", [ this ]( zpSerializedInput* in ) {
			zpSerializable* manager = ZP_NULL;
			in->readSerializable( &manager );
			if( manager ) addGameManager( (zpGameManager*)manager );
		} );
		in->endBlock();
	}

	in->endBlock();
}