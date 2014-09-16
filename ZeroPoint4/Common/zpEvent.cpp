#include "zpCommon.h"

zpEventHandler::zpEventHandler()
	: m_handlesEvent( ZP_EVENT_NULL )
{}
zpEventHandler::~zpEventHandler()
{
	m_handlesEvent = ZP_EVENT_NULL;
}

zp_bool zpEventHandler::isHandlerForEvent( const zpEvent& e ) const
{
	return e == m_handlesEvent;
}


zpEventManager::zpEventManager()
{}
zpEventManager::~zpEventManager()
{}

void zpEventManager::addEventListener( const zp_char* eventName, zpEventListener* listener, zpEventHandler& handler )
{
	zp_uint e = findEventAndCreate( eventName );
	handler.m_handlesEvent = e;

	m_eventListeners[ e ].pushBack( listener );
}
void zpEventManager::addEventListener( const zpString& eventName, zpEventListener* listener, zpEventHandler& handler )
{
	zp_uint e = findEventAndCreate( eventName );
	handler.m_handlesEvent = e;

	m_eventListeners[ e ].pushBack( listener );
}

void zpEventManager::removeEventListener( const zp_char* eventName, zpEventListener* listener, zpEventHandler& handler )
{
	zp_uint e;
	if( findEvent( eventName, e ) )
	{
		m_eventListeners[ e ].eraseAll( listener );
		handler.m_handlesEvent = ZP_EVENT_NULL;
	}
}
void zpEventManager::removeEventListener( const zpString& eventName, zpEventListener* listener, zpEventHandler& handler )
{
	zp_uint e;
	if( findEvent( eventName, e ) )
	{
		m_eventListeners[ e ].eraseAll( listener );
		handler.m_handlesEvent = ZP_EVENT_NULL;
	}
}

void zpEventManager::removeAllEventListeners( const zp_char* eventName )
{
	zp_uint e;
	if( findEvent( eventName, e ) )
	{
		m_eventListeners[ e ].clear();
	}
}
void zpEventManager::removeAllEventListeners( const zpString& eventName )
{
	zp_uint e;
	if( findEvent( eventName, e ) )
	{
		m_eventListeners[ e ].clear();
	}
}

void zpEventManager::sendEvent( const zp_char* eventName, zpObject* sender )
{
	zp_uint e;
	if( findEvent( eventName, e ) )
	{
		m_eventListeners[ e ].foreach( [ &e, sender ]( zpEventListener* listener )
		{
			listener->handleEvent( e, sender );
		} );
	}
}
void zpEventManager::sendEvent( const zpString& eventName, zpObject* sender )
{
	zp_uint e;
	if( findEvent( eventName, e ) )
	{
		m_eventListeners[ e ].foreach( [ &e, sender ]( zpEventListener* listener )
		{
			listener->handleEvent( e, sender );
		} );
	}
}

zp_bool zpEventManager::findEvent( const zp_char* eventName, zp_uint& index )
{
	return m_eventNames.findIndexIf( [ eventName ]( const zpString& e )
	{
		return e == eventName;
	}, index );
}
zp_bool zpEventManager::findEvent( const zpString& eventName, zp_uint& index )
{
	zp_int i = m_eventNames.indexOf( eventName );
	if( i < 0 ) return false;

	index = (zp_uint)i;
	return true;
}

zp_uint zpEventManager::findEventAndCreate( const zp_char* eventName )
{
	zp_uint index;
	zp_bool found = m_eventNames.findIndexIf( [ eventName ]( const zpString& e )
	{
		return e == eventName;
	}, index );

	if( !found )
	{
		index = m_eventNames.size();
		m_eventNames.pushBackEmpty() = eventName;
		m_eventListeners.pushBackEmpty();
	}

	return index;
}

zp_uint zpEventManager::findEventAndCreate( const zpString& eventName )
{
	zp_int index = m_eventNames.indexOf( eventName );
	if( index < 0 )
	{
		index = m_eventNames.size();
		m_eventNames.pushBackEmpty() = eventName;
		m_eventListeners.pushBackEmpty();
	}

	return (zp_uint)index;
}
