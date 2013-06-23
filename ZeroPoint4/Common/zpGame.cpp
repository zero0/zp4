#include "zpCommon.h"
/*
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
zpGame::~zpGame()
{}

void zpGame::create()
{
	//if( m_window ) m_window->create();
	//m_managers.setGame( this );
	//m_managers.create();
}
void zpGame::destroy()
{
	if( m_window ) m_window->destroy();
	m_managers.destroy();
	//m_managers.setGame( ZP_NULL );
}

void zpGame::setNextWorld( zpWorld* world )
{
	if( m_currentWorld != ZP_NULL )
	{
		m_nextWorld = world;
	}
	else
	{
		m_currentWorld = world;
	}
}
zpWorld* zpGame::getCurrentWorld() const
{
	return m_currentWorld;
}
zpWorld* zpGame::getNextWorld() const
{
	return m_nextWorld;
}

zpAllGameManagers* zpGame::getGameManagers()
{
	return &m_managers;
}

void zpGame::setRenderable( zpRenderable* renderable )
{
	m_renderable = renderable;
}
zpRenderable* zpGame::getRenderable() const
{
	return m_renderable;
}

void zpGame::process()
{
#if 0
	while( m_window->processMessages() )
	{
		m_timer->tick();
		zp_long now = m_timer->getTime();
		zp_uint numUpdates = 0;

		// update
		m_managers.update();
		if( m_currentWorld ) m_currentWorld->update();

		// simulate
		while( ( now - m_lastTime ) > m_simulateHz && numUpdates < 5 )
		{
			m_managers.simulate();
			if( m_currentWorld ) m_currentWorld->simulate();

			m_lastTime += m_simulateHz;
			++numUpdates;
		}

		// adjust timer
		if( ( now - m_lastTime ) > m_simulateHz )
		{
			m_lastTime = now - m_simulateHz;
		}

		// render
		m_timer->setInterpolation( (zp_float)( now - m_lastTime ) / (zp_float)m_simulateHz );
		if( m_renderable ) m_renderable->render();

		// sleep for the remainder of the frame
		m_timer->sleep( m_renderMsHz );

		// if there is a next world to set, do it now for the next frame
		if( m_nextWorld )
		{
			if( m_currentWorld ) m_currentWorld->receiveMessage( zpMessageTypes::LEAVE_WORLD );
			if( m_nextWorld ) m_nextWorld->receiveMessage( zpMessageTypes::ENTER_WORLD );

			m_currentWorld = m_nextWorld;
			if( !m_currentWorld->isCreated() )
			{
				m_currentWorld->create();
			}

			if( m_currentWorld ) m_currentWorld->receiveMessage( zpMessageTypes::ENTER_WORLD );

			m_nextWorld = ZP_NULL;
		}
	}
#endif
}

void zpGame::setWindow( zpWindow* window )
{
	m_window = window;
}
zpWindow* zpGame::getWindow() const
{
	return m_window;
}

void zpGame::receiveMessage( const zpMessage& message )
{
	//switch( message.getMessageType() ) {
	//case zpMessageTypes::SYS_SET_NEXT_WORLD:
	//	setNextWorld( message.getMessageData<zpString>() );
	//	break;
	//case zpMessageTypes::SYS_SET_SCREEN_SIZE:
	//	m_window->setScreenSize( message.getMessageData<zpVector2i>() );
	//	break;
	//}
}

void zpGame::serialize( zpSerializedOutput* out )
{
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	m_window->serialize( out );

	out->writeBlock( "Managers" );
	{
		m_managers.serialize( out );
		//out->writeBlock( "Manager" );
		//{
		//	m_managers.foreach( [ out ]( zpGameManager* manager ) {
		//		manager->serialize( out );
		//	} );
		//}
		//out->endBlock();
	}
	out->endBlock();

	out->endBlock();
}
void zpGame::deserialize( zpSerializedInput* in )
{
	in->readBlock( ZP_SERIALIZE_TYPE_THIS );

	in->readSerializableOfType( &m_window );

	if( in->readBlock( "Managers" ) )
	{
		m_managers.deserialize( in );

		in->endBlock();
	}
}
*/
