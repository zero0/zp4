#include "zpInput.h"

zpInputManager::zpInputManager()
{
	m_controllers.resize( ZP_INPUT_MAX_CONTROLLERS );
}
zpInputManager::~zpInputManager()
{}

const zpKeyboard* zpInputManager::getKeyboard()
{
	if( !m_keyboard.isCreated() )
	{
		m_keyboard.create();
	}
	return &m_keyboard;
}
const zpMouse* zpInputManager::getMouse()
{
	if( !m_mouse.isCreated() )
	{
		m_mouse.create();
	}
	return &m_mouse;
}
const zpController* zpInputManager::getController( zpControllerNumber controller )
{
	zpController& c = m_controllers[ controller ];
	if( !c.isCreated() )
	{
		c.create( controller );
	}
	return &c;
}

void zpInputManager::update()
{
	m_keyboard.poll();

	m_mouse.poll();

	zpController* c = m_controllers.begin();
	zpController* end = m_controllers.end();
	for( ; c != end; ++c )
	{
		c->poll();
	}
}

void zpInputManager::onFocusGained()
{
	m_keyboard.onFocusGained();

	m_mouse.onFocusGained();

	zpController* c = m_controllers.begin();
	zpController* end = m_controllers.end();
	for( ; c != end; ++c )
	{
		c->onFocusGained();
	}
}
void zpInputManager::onFocusLost()
{
	m_keyboard.onFocusLost();

	m_mouse.onFocusLost();

	zpController* c = m_controllers.begin();
	zpController* end = m_controllers.end();
	for( ; c != end; ++c )
	{
		c->onFocusLost();
	}
}
void zpInputManager::onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam )
{
	m_keyboard.onWindowProc( uMessage, wParam, lParam );

	m_mouse.onWindowProc( uMessage, wParam, lParam );
}
