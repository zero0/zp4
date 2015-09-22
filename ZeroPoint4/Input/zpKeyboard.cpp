#include "zpInput.h"
#include <Windows.h>

#define KEY_DOWN	(zp_byte)-1
#define KEY_UP		(zp_byte)0

zpKeyboard::zpKeyboard()
	: m_isCreated( false )
	, m_hasFocus( true )
{
}
zpKeyboard::~zpKeyboard()
{
	destroy();
}

void zpKeyboard::poll()
{
	if( m_hasFocus && m_isCreated )
	{
		for( zp_uint k = 0; k < ZP_INPUT_MAX_KEYS; ++k )
		{
			m_keyPressBuffer[ k ] = KEY_UP;

			if( m_keyBuffer[ k ] == KEY_DOWN )
			{
				if( m_keyDownBuffer[ k ] == KEY_DOWN )
				{
					// repeat
					m_listeners.foreach( [ k ]( zpKeyboardListener* listener ) {
						listener->onKeyRepeat( (zpKeyCode)k );
					} );
				}
				else
				{
					// down
					m_listeners.foreach( [ k ]( zpKeyboardListener* listener ) {
						listener->onKeyDown( (zpKeyCode)k );
					} );
				}

				m_keyDownBuffer[ k ] = KEY_DOWN;
			}
			else if( m_keyDownBuffer[ k ] == KEY_DOWN )
			{
				m_keyDownBuffer[ k ] = KEY_UP;
				m_keyPressBuffer[ k ] = KEY_DOWN;

				// up
				m_listeners.foreach( [ k ]( zpKeyboardListener* listener ) {
					listener->onKeyUp( (zpKeyCode)k );
				} );
			}
		}
	}
}
void zpKeyboard::create()
{
	m_isCreated = true;
	zp_memset( m_keyBuffer, KEY_UP, sizeof( m_keyDownBuffer ) );
	zp_memset( m_keyDownBuffer, KEY_UP, sizeof( m_keyDownBuffer ) );
	zp_memset( m_keyPressBuffer, KEY_UP, sizeof( m_keyPressBuffer ) );
}
void zpKeyboard::destroy()
{
	m_isCreated = false;
	removeAllListeners();
}

zp_bool zpKeyboard::isCreated() const
{
	return m_isCreated;
}

zp_bool zpKeyboard::isKeyDown( zpKeyCode key ) const
{
	return m_keyDownBuffer[ (zp_byte)( ZP_KEY_CODE_STRIP_MOD( key ) ) ] == KEY_DOWN;
}
zp_bool zpKeyboard::isKeyUp( zpKeyCode key ) const
{
	return m_keyDownBuffer[ (zp_byte)( ZP_KEY_CODE_STRIP_MOD( key ) ) ] == KEY_UP;
}
zp_bool zpKeyboard::isKeyPressed( zpKeyCode key ) const
{
	return m_keyPressBuffer[ (zp_byte)( ZP_KEY_CODE_STRIP_MOD( key ) ) ] == KEY_DOWN;
}

zp_char zpKeyboard::keyToChar( zpKeyCode key ) const
{
	zp_char c = '\0';
	zp_bool caps = ZP_KEY_CODE_IS_MOD_DOWN( SHIFT, key ) || isKeyDown( ZP_KEY_CODE_LSHIFT ) || isKeyDown( ZP_KEY_CODE_RSHIFT );

	switch( key )
	{
		case ZP_KEY_CODE_TAB:			c = '\t'; break;

		case ZP_KEY_CODE_RETURN:		c = '\n'; break;

		case ZP_KEY_CODE_SPACE:			c = ' '; break;

		case ZP_KEY_CODE_0:				c = caps ? ')' : '0'; break;
		case ZP_KEY_CODE_1:				c = caps ? '!' : '1'; break;
		case ZP_KEY_CODE_2:				c = caps ? '@' : '2'; break;
		case ZP_KEY_CODE_3:				c = caps ? '#' : '3'; break;
		case ZP_KEY_CODE_4:				c = caps ? '$' : '4'; break;
		case ZP_KEY_CODE_5:				c = caps ? '%' : '5'; break;
		case ZP_KEY_CODE_6:				c = caps ? '^' : '6'; break;
		case ZP_KEY_CODE_7:				c = caps ? '&' : '7'; break;
		case ZP_KEY_CODE_8:				c = caps ? '*' : '8'; break;
		case ZP_KEY_CODE_9:				c = caps ? '(' : '9'; break;

		case ZP_KEY_CODE_A:				c = caps ? 'A' : 'a'; break;
		case ZP_KEY_CODE_B:				c = caps ? 'B' : 'b'; break;
		case ZP_KEY_CODE_C:				c = caps ? 'C' : 'c'; break;
		case ZP_KEY_CODE_D:				c = caps ? 'D' : 'd'; break;
		case ZP_KEY_CODE_E:				c = caps ? 'E' : 'e'; break;
		case ZP_KEY_CODE_F:				c = caps ? 'F' : 'f'; break;
		case ZP_KEY_CODE_G:				c = caps ? 'G' : 'g'; break;
		case ZP_KEY_CODE_H:				c = caps ? 'H' : 'h'; break;
		case ZP_KEY_CODE_I:				c = caps ? 'I' : 'i'; break;
		case ZP_KEY_CODE_J:				c = caps ? 'J' : 'j'; break;
		case ZP_KEY_CODE_K:				c = caps ? 'K' : 'k'; break;
		case ZP_KEY_CODE_L:				c = caps ? 'L' : 'l'; break;
		case ZP_KEY_CODE_M:				c = caps ? 'M' : 'm'; break;
		case ZP_KEY_CODE_N:				c = caps ? 'N' : 'n'; break;
		case ZP_KEY_CODE_O:				c = caps ? 'O' : 'o'; break;
		case ZP_KEY_CODE_P:				c = caps ? 'P' : 'p'; break;
		case ZP_KEY_CODE_Q:				c = caps ? 'Q' : 'q'; break;
		case ZP_KEY_CODE_R:				c = caps ? 'R' : 'r'; break;
		case ZP_KEY_CODE_S:				c = caps ? 'S' : 's'; break;
		case ZP_KEY_CODE_T:				c = caps ? 'T' : 't'; break;
		case ZP_KEY_CODE_U:				c = caps ? 'U' : 'u'; break;
		case ZP_KEY_CODE_V:				c = caps ? 'V' : 'v'; break;
		case ZP_KEY_CODE_W:				c = caps ? 'W' : 'w'; break;
		case ZP_KEY_CODE_X:				c = caps ? 'X' : 'x'; break;
		case ZP_KEY_CODE_Y:				c = caps ? 'Y' : 'y'; break;
		case ZP_KEY_CODE_Z:				c = caps ? 'Z' : 'z'; break;

		case ZP_KEY_CODE_NUMPAD_0:		c = '0'; break;
		case ZP_KEY_CODE_NUMPAD_1:		c = '1'; break;
		case ZP_KEY_CODE_NUMPAD_2:		c = '2'; break;
		case ZP_KEY_CODE_NUMPAD_3:		c = '3'; break;
		case ZP_KEY_CODE_NUMPAD_4:		c = '4'; break;
		case ZP_KEY_CODE_NUMPAD_5:		c = '5'; break;
		case ZP_KEY_CODE_NUMPAD_6:		c = '6'; break;
		case ZP_KEY_CODE_NUMPAD_7:		c = '7'; break;
		case ZP_KEY_CODE_NUMPAD_8:		c = '8'; break;
		case ZP_KEY_CODE_NUMPAD_9:		c = '9'; break;

		case ZP_KEY_CODE_MULTIPLY:		c = '*'; break;
		case ZP_KEY_CODE_ADD:			c = '+'; break;
		case ZP_KEY_CODE_SUBTRACT:		c = '-'; break;
		case ZP_KEY_CODE_DECIMAL:		c = '.'; break;
		case ZP_KEY_CODE_DIVIDE:		c = '/'; break;
	}
	return c;
}

void zpKeyboard::addListener( zpKeyboardListener* listener )
{
	m_listeners.pushBack( listener );
}
void zpKeyboard::removeListener( zpKeyboardListener* listener )
{
	m_listeners.eraseAll( listener );
}
void zpKeyboard::removeAllListeners()
{
	m_listeners.clear();
}

void zpKeyboard::onFocusGained()
{
	m_hasFocus = true;
}
void zpKeyboard::onFocusLost()
{
	m_hasFocus = false;
}

void zpKeyboard::onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam )
{
	if( m_isCreated )
	{
		switch( uMessage )
		{
		case WM_KEYDOWN:
			m_keyBuffer[ wParam ] = KEY_DOWN;
			break;
		case WM_KEYUP:
			m_keyBuffer[ wParam ] = KEY_UP;
			break;
		}
	}
}