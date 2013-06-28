#include "zpInput.h"

zpInputManager::zpInputManager()
{
	for( zp_uint i = 0; i < ZP_INPUT_MAX_CONTROLLERS; ++i )
	{
		m_controllers.pushBackEmpty();
	}
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
		c.m_controller = controller;
		c.create();
	}
	return &c;
}

void zpInputManager::update()
{
	m_keyboard.poll();

	m_mouse.poll();

	for( zp_uint i = 0; i < ZP_INPUT_MAX_CONTROLLERS; ++i )
	{
		m_controllers[ i ].poll();
	}
}

void zpInputManager::onFocusGained()
{
	m_keyboard.onFocusGained();

	m_mouse.onFocusGained();

	for( zp_uint i = 0; i < ZP_INPUT_MAX_CONTROLLERS; ++i )
	{
		m_controllers[ i ].onFocusGained();
	}
}
void zpInputManager::onFocusLost()
{
	m_keyboard.onFocusLost();

	m_mouse.onFocusLost();

	for( zp_uint i = 0; i < ZP_INPUT_MAX_CONTROLLERS; ++i )
	{
		m_controllers[ i ].onFocusLost();
	}
}
void zpInputManager::onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam )
{
	m_keyboard.onWindowProc( uMessage, wParam, lParam );

	m_mouse.onWindowProc( uMessage, wParam, lParam );
}
