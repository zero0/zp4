#include "zpInput.h"
#include <Windows.h>

zpMouse::zpMouse()
	: m_isCreated( false )
	, m_hasFocus( true )
	, m_scrollWheel( 0 )
	, m_scrollWheelBuffer( 0 )
	, m_scrollWheelDelta( 0 )
{}
zpMouse::~zpMouse()
{}

void zpMouse::poll()
{
	if( !m_hasFocus || !m_isCreated ) return;

	for( zp_uint b = 0; b < zpMouseButton_Count; ++b )
	{
		if( m_buttonBuffer.isMarked( b ) )
		{
			if( m_buttonDownBuffer.isMarked( b ) )
			{
				// repeat
				m_listeners.foreach( [ &b ]( zpMouseListener* listener ) {
					listener->onMouseButtonRepeat( (zpMouseButton)b );
				} );
			}
			else
			{
				// down
				m_listeners.foreach( [ &b ]( zpMouseListener* listener ) {
					listener->onMouseButtonDown( (zpMouseButton)b );
				} );
			}
			m_buttonDownBuffer.mark( b );
		}
		else if( m_buttonDownBuffer.isMarked( b ) )
		{
			// up
			m_buttonDownBuffer.unmark( b );

			m_listeners.foreach( [ &b ]( zpMouseListener* listener ) {
				listener->onMouseButtonUp( (zpMouseButton)b );
			} );
		}
	}

	if( m_scrollWheelBuffer != 0 )
	{
		m_scrollWheelDelta = m_scrollWheelBuffer;
		m_scrollWheel += m_scrollWheelBuffer;

		m_listeners.foreach( [ this ]( zpMouseListener* listener ) {
			listener->onMouseScroll( m_scrollWheelBuffer );
		} );

		m_scrollWheelBuffer = 0;
	}
	else
	{
		m_scrollWheelDelta = 0;
	}

	if( !m_locationBuffer.isZero() )
	{
		m_delta = m_locationBuffer - m_location;
		m_location = m_locationBuffer;

		m_listeners.foreach( [ this ]( zpMouseListener* listener ) {
			listener->onMouseMove( m_location );
			listener->onMouseChange( m_delta );
		} );

		m_locationBuffer.zero();
	}
}
void zpMouse::create()
{
	m_isCreated = true;

	m_scrollWheel = 0;
	m_scrollWheelBuffer = 0;

	m_buttonBuffer.clear();
	m_buttonDownBuffer.clear();

	m_buttonBuffer.clear();
	m_buttonDownBuffer.clear();

	m_location.zero();
	m_locationBuffer.zero();
	m_delta.zero();
}
void zpMouse::destroy()
{
	m_isCreated = false;
	removeAllListeners();
}

zp_bool zpMouse::isCreated() const
{
	return m_isCreated;
}

zp_bool zpMouse::isButtonDown( zpMouseButton button ) const
{
	return m_buttonDownBuffer.isMarked( button );
}
zp_bool zpMouse::isButtonUp( zpMouseButton button ) const
{
	return !m_buttonDownBuffer.isMarked( button );
}

const zpVector2i& zpMouse::getLocation() const
{
	return m_location;
}
const zpVector2i& zpMouse::getDelta() const
{
	return m_delta;
}
zp_int zpMouse::getScrollWheel() const
{
	return m_scrollWheel;
}
zp_int zpMouse::getScrollWheelDelta() const
{
	return m_scrollWheelDelta;
}

//void zpMouse::setSensitivity( zp_float sensitivity ) {}
//zp_float zpMouse::getSensitivity() const {}

void zpMouse::addListener( zpMouseListener* listener )
{
	m_listeners.pushBack( listener );
}
void zpMouse::removeListener( zpMouseListener* listener )
{
	m_listeners.eraseAll( listener );
}
void zpMouse::removeAllListeners()
{
	m_listeners.clear();
}

void zpMouse::onFocusGained()
{
	m_hasFocus = true;
}
void zpMouse::onFocusLost()
{
	m_hasFocus = false;
}

void zpMouse::onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam )
{
	if( m_isCreated )
	{
		switch( uMessage )
		{
		case WM_MOUSEMOVE:
			{
				POINTS p = MAKEPOINTS( lParam );
				m_locationBuffer.set( p.x, p.y );
			}
			break;
		case WM_MOUSEWHEEL:
			m_scrollWheelBuffer = GET_WHEEL_DELTA_WPARAM( wParam ) > 0 ? 1 : -1;
			break;
		case WM_LBUTTONDOWN:
			m_buttonBuffer.mark( ZP_MOUSE_BUTTON_LEFT );
			break;
		case WM_LBUTTONUP:
			m_buttonBuffer.unmark( ZP_MOUSE_BUTTON_LEFT );
			break;
		case WM_RBUTTONDOWN:
			m_buttonBuffer.mark( ZP_MOUSE_BUTTON_RIGHT );
			break;
		case WM_RBUTTONUP:
			m_buttonBuffer.unmark( ZP_MOUSE_BUTTON_RIGHT );
			break;
		case WM_MBUTTONDOWN:
			m_buttonBuffer.mark( ZP_MOUSE_BUTTON_MIDDLE );
			break;
		case WM_MBUTTONUP:
			m_buttonBuffer.unmark( ZP_MOUSE_BUTTON_MIDDLE );
			break;
		}
	}
}
