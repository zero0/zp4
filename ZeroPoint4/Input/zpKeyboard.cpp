#include "zpInput.h"
#include <Windows.h>

#define KEY_DOWN	(zp_byte)-1
#define KEY_UP		(zp_byte)0

zpKeyboard::zpKeyboard()
	: m_hasFocus( true )
{
	zp_memset( m_keyDownBuffer, 0, sizeof( m_keyDownBuffer ) );
}
zpKeyboard::~zpKeyboard() {
	destroy();
}

void zpKeyboard::poll() {
	if( !m_hasFocus ) return;

	for( zp_uint k = 256; k --> 0; ) {
		if( m_keyBuffer[ k ] == KEY_DOWN ) {
			if( m_keyDownBuffer[ k ] == KEY_DOWN ) {
				// repeat
				m_listeners.foreach( [ &k ]( zpKeyboardListener* listener ) {
					listener->onKeyRepeat( (zpKeyCode)k );
				} );
			} else {
				// down
				m_listeners.foreach( [ &k ]( zpKeyboardListener* listener ) {
					listener->onKeyDown( (zpKeyCode)k );
				} );
			}
			m_keyDownBuffer[ k ] = KEY_DOWN;
		} else if( m_keyDownBuffer[ k ] == KEY_DOWN ) {
			m_keyDownBuffer[ k ] = KEY_UP;

			// up
			m_listeners.foreach( [ &k ]( zpKeyboardListener* listener ) {
				listener->onKeyUp( (zpKeyCode)k );
			} );
		}
	}
}
void zpKeyboard::create() {
	zp_memset( m_keyDownBuffer, 0, sizeof( m_keyDownBuffer ) );
}
void zpKeyboard::destroy() {
	removeAllListeners();
	zp_memset( m_keyDownBuffer, 0, sizeof( m_keyDownBuffer ) );
}

zp_bool zpKeyboard::isKeyDown( zpKeyCode key ) const {
	return m_keyDownBuffer[ (zp_byte)( ZP_KEY_CODE_STRIP_MOD( key ) ) ] == KEY_DOWN;
}

void zpKeyboard::addListener( zpKeyboardListener* listener ) {
	m_listeners.pushBack( listener );
}
void zpKeyboard::removeListener( zpKeyboardListener* listener ) {
	m_listeners.removeAll( listener );
}
void zpKeyboard::removeAllListeners() {
	m_listeners.clear();
}

void zpKeyboard::onFocusGained() {
	m_hasFocus = true;
}
void zpKeyboard::onFocusLost() {
	m_hasFocus = false;
}

void zpKeyboard::onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam ) {
	switch( uMessage ) {
	case WM_KEYDOWN:
		m_keyBuffer[ (zp_byte)wParam ] = KEY_DOWN;
		break;
	case WM_KEYUP:
		m_keyBuffer[ (zp_byte)wParam ] = KEY_UP;
		break;
	}
}