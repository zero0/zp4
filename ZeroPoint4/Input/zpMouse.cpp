#include "zpInput.h"
#include <Windows.h>

zpMouse::zpMouse() 
	: m_hasFocus( false )
	, m_scrollWheel( 0 )
	, m_scrollWheelBuffer( 0 )
{}
zpMouse::~zpMouse() {}

void zpMouse::poll() {
	if( !m_hasFocus ) return;

	for( zp_uint b = 8; b --> 0; ) {
		if( m_buttonBuffer.isMarked( b ) ) {
			if( m_buttonDownBuffer.isMarked( b ) ) {
				// repeat
				m_listeners.foreach( [ &b ]( zpMouseListener* listener ) {
					listener->onMouseButtonRepeat( (zpMouseButton)b );
				} );
			} else {
				// down
				m_listeners.foreach( [ &b ]( zpMouseListener* listener ) {
					listener->onMouseButtonDown( (zpMouseButton)b );
				} );
			}
			m_buttonDownBuffer.mark( b );
		} else if( m_buttonDownBuffer.isMarked( b ) ) {
			// up
			m_buttonDownBuffer.unmark( b );

			m_listeners.foreach( [ &b ]( zpMouseListener* listener ) {
				listener->onMouseButtonUp( (zpMouseButton)b );
			} );
		}
	}

	if( m_scrollWheelBuffer != 0 ) {
		m_scrollWheel += m_scrollWheelBuffer;

		m_listeners.foreach( [ this ]( zpMouseListener* listener ) {
			listener->onMouseScroll( m_scrollWheelBuffer );
		} );

		m_scrollWheelBuffer = 0;
	}

	if( !m_locationBuffer.isZero() ) {
		m_delta = m_locationBuffer - m_location;
		m_location = m_locationBuffer;

		m_listeners.foreach( [ this ]( zpMouseListener* listener ) {
			listener->onMouseMove( m_location );
			listener->onMouseChange( m_delta );
		} );

		m_locationBuffer.zero();
	}
}
void zpMouse::create() {}
void zpMouse::destroy() {
	m_listeners.clear();
}

zp_bool zpMouse::isButtonDown( zpMouseButton button ) const {
	return m_buttonDownBuffer.isMarked( button );
}
const zpVector2i& zpMouse::getLocation() const {
	return m_location;
}
const zpVector2i& zpMouse::getDelta() const {
	return m_delta;
}
zp_int zpMouse::getScrollWheel() const {
	return m_scrollWheel;
}

//void zpMouse::setSensitivity( zp_float sensitivity ) {}
//zp_float zpMouse::getSensitivity() const {}

void zpMouse::addListener( zpMouseListener* listener ) {
	m_listeners.pushBack( listener );
}
void zpMouse::removeListener( zpMouseListener* listener ) {
	m_listeners.removeFirst( listener );
}
void zpMouse::removeAllListeners() {
	m_listeners.clear();
}

void zpMouse::onFocusGained() {
	m_hasFocus = true;
}
void zpMouse::onFocusLost() {
	m_hasFocus = false;
}

void zpMouse::onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam ) {
	switch( uMessage ) {
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
