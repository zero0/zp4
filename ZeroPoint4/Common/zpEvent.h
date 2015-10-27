#pragma once
#ifndef ZP_EVENT_H
#define ZP_EVENT_H

typedef zp_size_t zpEvent;
ZP_PURE_INTERFACE zpEventListener;
class zpEventManager;

#define ZP_EVENT_NULL	( (zpEvent)-1 )

class zpEventHandler
{
public:
	zpEventHandler();
	~zpEventHandler();

	zp_bool isHandlerForEvent( const zpEvent& e ) const;

private:
	zpEvent m_handlesEvent;

	friend class zpEventManager;
};

ZP_PURE_INTERFACE zpEventListener
{
public:
	virtual void handleEvent( const zpEvent& e, zpObject* sender ) = 0;
};

class zpEventManager
{
	ZP_NON_COPYABLE( zpEventManager );
public:
	zpEventManager();
	~zpEventManager();

	void addEventListener( const zp_char* eventName, zpEventListener* listener, zpEventHandler& handler );
	void addEventListener( const zpString& eventName, zpEventListener* listener, zpEventHandler& handler );

	void removeEventListener( const zp_char* eventName, zpEventListener* listener, zpEventHandler& handler );
	void removeEventListener( const zpString& eventName, zpEventListener* listener, zpEventHandler& handler );

	void removeAllEventListeners( const zp_char* eventName );
	void removeAllEventListeners( const zpString& eventName );

	void sendEvent( const zp_char* eventName, zpObject* sender );
	void sendEvent( const zpString& eventName, zpObject* sender );

private:
	zp_bool findEvent( const zp_char* eventName, zp_size_t& index );
	zp_bool findEvent( const zpString& eventName, zp_size_t& index );

	zp_size_t findEventAndCreate( const zp_char* eventName );
	zp_size_t findEventAndCreate( const zpString& eventName );

	zpArrayList< zpString > m_eventNames;
	zpArrayList< zpArrayList< zpEventListener* > > m_eventListeners;
};

#endif
