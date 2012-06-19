#include "zpInput.h"
#include <Windows.h>
#include <XInput.h>

#define IS_BUTTON_PRESSED( b )	( ( m_buttons & XINPUT_GAMEPAD_##b ) == XINPUT_GAMEPAD_##b )

#define IS_BUTTON_DOWN(s, b)			(( (s) & (b) ) == (b))
#define IS_BUTTON_UP(s, b)				(( (s) & (b) ) != (b))
#define IS_BUTTON_DOWN_UP( p, c, b )	IS_BUTTON_DOWN( p, b ) && IS_BUTTON_UP( c, b )
#define IS_BUTTON_UP_DOWN( p, c, b )	IS_BUTTON_UP( p, b ) && IS_BUTTON_DOWN( c, b )
#define IS_BUTTON_DOWN_DOWN( p, c, b )	IS_BUTTON_DOWN( p, b ) && IS_BUTTON_DOWN( c, b )

#define FIRE_CONTROLLER_EVENT_TO_LISTENERS_( e, p )	\
	m_listeners.foreach( [ &p ]( zpControllerListener* listener ) {	listener->e( (p) ); } )

#define FIRE_CONTROLLER_EVENT_TO_LISTENERS( e )	\
	m_listeners.foreach( []( zpControllerListener* listener ) {	listener->e(); } )

zpController::zpController( zpControllerNumber number ) {}
zpController::~zpController() {
	destroy();
}

void zpController::poll() {
	// Get the state of the controller
	bool wasConnected = m_isConnected;
	XINPUT_STATE state;
	m_isConnected = XInputGetState( m_controller, &state ) == 0;

	// grab iterators only once
	//XInputControllerEventListenerList::iterator iter;
	//XInputControllerEventListenerList::iterator begin = m_listeners.begin();
	//XInputControllerEventListenerList::iterator end = m_listeners.end();

	// If the controller is connected, update the values
	if( m_isConnected ) {
		zp_short x, y;
		zp_short px, py;
		// Left Thumb Stick
		x = state.Gamepad.sThumbLX;
		y = state.Gamepad.sThumbLY;
		px = (zp_short)m_leftThumb.getX();
		py = (zp_short)m_leftThumb.getY();

		// Left thumb stick deadzone calculations
		if( x < m_leftThumbDeadZone && x > -m_leftThumbDeadZone && y < m_leftThumbDeadZone && y > -m_leftThumbDeadZone  ) {
			x = 0;
			y = 0;
		}

		m_leftThumb.set( x, y );
		zpVector2i lt( m_leftThumb );
		if( x != px || y != py ) {
			FIRE_CONTROLLER_EVENT_TO_LISTENERS_( onLeftStickMove, lt );
		} else if( ( x || y ) && ( x == px && y == py ) ) {
			FIRE_CONTROLLER_EVENT_TO_LISTENERS_( onLeftStickRepeat, lt );
		}
		
		
		// Right Thumb Stick
		x = state.Gamepad.sThumbRX;
		y = state.Gamepad.sThumbRY;
		px = (zp_short)m_rightThumb.getX();
		py = (zp_short)m_rightThumb.getY();

		// Right thumb stick deadzone calculations
		if( x < m_rightThumbDeadZone && x > -m_rightThumbDeadZone && y < m_rightThumbDeadZone && y > -m_rightThumbDeadZone ) {
			x = 0;
			y = 0;
		}

		m_rightThumb.set( x, y );
		zpVector2i rt( m_leftThumb );
		if( x != px || y != py ) {
			FIRE_CONTROLLER_EVENT_TO_LISTENERS_( onRightStickMove, rt );
		} else if( ( x || y ) && ( x == px && y == py ) ) {
			FIRE_CONTROLLER_EVENT_TO_LISTENERS_( onRightStickRepeat, rt );
		}
		
		// Button Presses
		zp_ushort currentButtons = state.Gamepad.wButtons;
		zp_ushort previousButtons = m_buttons;///*m_state.Gamepad.wButtons*/m_buttons;

		static const zpControllerButton buttons[] = {
			ZP_CONTROLLER_BUTTON_A,
			ZP_CONTROLLER_BUTTON_B,
			ZP_CONTROLLER_BUTTON_X,
			ZP_CONTROLLER_BUTTON_Y,
			ZP_CONTROLLER_BUTTON_BACK,
			ZP_CONTROLLER_BUTTON_START,
			ZP_CONTROLLER_BUTTON_DPAD_UP,
			ZP_CONTROLLER_BUTTON_DPAD_DOWN,
			ZP_CONTROLLER_BUTTON_DPAD_LEFT,
			ZP_CONTROLLER_BUTTON_DPAD_RIGHT,
			ZP_CONTROLLER_BUTTON_LEFT_SHOULDER,
			ZP_CONTROLLER_BUTTON_RIGHT_SHOULDER,
			ZP_CONTROLLER_BUTTON_LEFT_THUMB,
			ZP_CONTROLLER_BUTTON_RIGHT_THUMB
		};

		//if( currentButtons != previousButtons ) {
			// Something changed with the buttons, check each button
			for( int i = 14; i --> 0; ) {
				zpControllerButton button = buttons[i];
				if( IS_BUTTON_UP_DOWN( previousButtons, currentButtons, button ) ) {
					FIRE_CONTROLLER_EVENT_TO_LISTENERS_( onButtonDown, button );
				} else if( IS_BUTTON_DOWN_UP( previousButtons, currentButtons, button ) ) {
					FIRE_CONTROLLER_EVENT_TO_LISTENERS_( onButtonUp, button );
				} else if( IS_BUTTON_DOWN_DOWN( previousButtons, currentButtons, button ) ) {
					FIRE_CONTROLLER_EVENT_TO_LISTENERS_( onButtonRepeat, button );
				}
			}
		//}
		m_buttons = currentButtons;

		// Trigger Pulls
		zp_byte t;
		zp_byte pt;
		// Left Trigger
		t = state.Gamepad.bLeftTrigger;
		pt = m_leftTrigger;

		// Left Trigger Threshold
		if( t <= m_leftTriggerThreshold ) {
			t = 0;
		}

		// Set the left trigger
		m_leftTrigger = t;

		// Check for pull/repeat
		if( t != pt ) {
			FIRE_CONTROLLER_EVENT_TO_LISTENERS_( onLeftTriggerPull, t );
		} else if( t != 0 && t == pt ) {
			FIRE_CONTROLLER_EVENT_TO_LISTENERS_( onLeftTriggerRepeat, t );
		}

		// Right Trigger
		t = state.Gamepad.bRightTrigger;
		pt = m_rightTrigger;

		// Right Trigger Threshold
		if( t <= m_rightTriggerThreshold ) {
			t = 0;
		}

		// Set the right trigger
		m_rightTrigger = t;

		// Check for pull/repeat.
		if( t != pt ) {
			FIRE_CONTROLLER_EVENT_TO_LISTENERS_( onRightTriggerPull, t );
		} else if( t != 0 && t == pt ) {
			FIRE_CONTROLLER_EVENT_TO_LISTENERS_( onRightTriggerRepeat, t );
		}
		/*
		if( m_isRumbleEnabled ) {
			// Right Rumble
			if( m_maxRightRumbleAmount > 0 ) {
				m_rightRumbleTime += deltaTime;
				if( m_rightRumbleTime >= m_rightRumbleTimer ) {
					m_rightRumbleAmount = 0;
					m_maxRightRumbleAmount = 0;
					m_rightRumbleTime = 0;
					m_rightRumbleTimer = 0;
				} else {
					WORD rr = m_maxRightRumbleAmount;//zp_lerp( m_rightRumbleAmount, m_maxRightRumbleAmount, m_rightRumbleTime / m_rightRumbleTimer );

					m_rightRumbleAmount = rr;
				}
			}

			// Left Rumble
			if( m_maxLeftRumbleAmount > 0 ) {
				m_leftRumbleTime += deltaTime;
				if( m_leftRumbleTime >= m_leftRumbleTimer ) {
					m_leftRumbleAmount = 0;
					m_maxLeftRumbleAmount = 0;
					m_leftRumbleTime = 0;
					m_leftRumbleTimer = 0;
				} else {
					WORD lr = m_maxLeftRumbleAmount;//zp_lerp( m_leftRumbleAmount, m_maxLeftRumbleAmount, m_leftRumbleTime / m_leftRumbleTimer );

					m_leftRumbleAmount = lr;
				}
			}
			//if( m_rightRumbleAmount || m_leftRumbleAmount ) zp_printfln( "Rumble r(%f/%f %d) l(%f/%f %d)", m_rightRumbleTime, m_rightRumbleTimer, m_rightRumbleAmount, m_leftRumbleTime, m_leftRumbleTimer, m_leftRumbleAmount );
			vibrate( m_rightRumbleAmount, m_leftRumbleAmount );
		}
		*/
	}

	if( wasConnected != m_isConnected ) {
		if( wasConnected ) {
			//was connected now not
			clear();
			FIRE_CONTROLLER_EVENT_TO_LISTENERS( onControllerDisconnected );
		} else {
			//was disconnected now not
			FIRE_CONTROLLER_EVENT_TO_LISTENERS( onControllerReconnected );
		}
	}
}
void zpController::create() {}
void zpController::destroy() {}

void zpController::onFocusGained() {
	XInputEnable( true );
}
void zpController::onFocusLost() {
	XInputEnable( false );
}

zp_bool zpController::isConnected() const {
	return m_isConnected;
}

void zpController::setTriggerThreshold( zp_byte threshold ) {
	m_leftTriggerThreshold = threshold;
	m_rightTriggerThreshold = threshold;
}
void zpController::setLeftTriggerThreshold( zp_byte threshold ) {
	m_leftTriggerThreshold = threshold;
}
void zpController::setRightTriggerThreshold( zp_byte threshold ) {
	m_rightTriggerThreshold = threshold;
}
zp_byte zpController::getLeftTriggerThreshold() const {
	return m_leftTriggerThreshold;
}
zp_byte zpController::getRightTriggerThreshold() const {
	return m_rightTriggerThreshold;
}

void zpController::setLeftThumbDeadZone( zp_short deadzone ) {
	m_leftThumbDeadZone = deadzone;
}
zp_short zpController::getLeftThumbDeadZone() const {
	return m_leftThumbDeadZone;
}

void zpController::setRightThumbDeadZone( zp_short deadzone ) {
	m_rightThumbDeadZone = deadzone;
}
zp_short zpController::getRightThumbDeadZone() const {
	return m_rightThumbDeadZone;
}

void zpController::vibrate( zp_ushort left, zp_ushort right ) const {
	XINPUT_VIBRATION vibrate = { left, right };
	XInputSetState( m_controller, &vibrate );
}
void zpController::stopVibrate() const {
	XInputSetState( m_controller, 0 );
}

zp_bool zpController::isAPressed() const {
	return IS_BUTTON_PRESSED( A );
}
zp_bool zpController::isBPressed() const {
	return IS_BUTTON_PRESSED( B );
}
zp_bool zpController::isXPressed() const {
	return IS_BUTTON_PRESSED( X );
}
zp_bool zpController::isYPressed() const {
	return IS_BUTTON_PRESSED( Y );
}

zp_bool zpController::isBackPressed() const {
	return IS_BUTTON_PRESSED( BACK );
}
zp_bool zpController::isStartPressed() const {
	return IS_BUTTON_PRESSED( START );
}

zp_bool zpController::isDPadUpPressed() const {
	return IS_BUTTON_PRESSED( DPAD_UP );
}
zp_bool zpController::isDPadDownPressed() const {
	return IS_BUTTON_PRESSED( DPAD_DOWN );
}
zp_bool zpController::isDPadLeftPressed() const {
	return IS_BUTTON_PRESSED( DPAD_LEFT );
}
zp_bool zpController::isDPadRightPressed() const {
	return IS_BUTTON_PRESSED( DPAD_RIGHT );
}

zp_bool zpController::isLeftBumperPressed() const {
	return IS_BUTTON_PRESSED( LEFT_SHOULDER );
}
zp_bool zpController::isRightBumperPressed() const {
	return IS_BUTTON_PRESSED( RIGHT_SHOULDER );
}

zp_bool zpController::isLeftThumbPressed() const {
	return IS_BUTTON_PRESSED( LEFT_THUMB );
}
zp_bool zpController::isRightThumbPressed() const {
	return IS_BUTTON_PRESSED( RIGHT_THUMB );
}

zp_bool zpController::isLeftThumbMoved() const {
	return !m_leftThumb.isZero();
}
zp_bool zpController::isRightThumbMoved() const {
	return !m_rightThumb.isZero();
}

zp_bool zpController::isLeftTriggerPressed() const {
	return m_leftTrigger != 0;
}
zp_bool zpController::isRightTriggerPressed() const {
	return m_rightTrigger != 0;
}

zp_byte zpController::getLeftTrigger() const {
	return m_leftTrigger;
}
zp_byte zpController::getRightTrigger() const {
	return m_rightTrigger;
}

const zpVector2i& zpController::getLeftThumb() const {
	return m_leftThumb;
}
const zpVector2i& zpController::getRightThumb() const {
	return m_rightThumb;
}

zpControllerNumber zpController::getController() const {
	return m_controller;
}

void zpController::addListener( zpControllerListener* listener ) {
	m_listeners.pushBack( listener );
}
void zpController::removeListener( zpControllerListener* listener ) {
	m_listeners.removeFirst( listener );
}
void zpController::removeAllListeners() {
	m_listeners.clear();
}

zpControllerBatterLevel zpController::getBatteryLevel() const {
	XINPUT_BATTERY_INFORMATION battery;
	XInputGetBatteryInformation( m_controller, 0, &battery );
	return (zpControllerBatterLevel)battery.BatteryLevel;
}
zpControllerBatterType zpController::getBatteryType() const {
	XINPUT_BATTERY_INFORMATION battery;
	XInputGetBatteryInformation( m_controller, 0, &battery );
	return (zpControllerBatterType)battery.BatteryType;
}

void zpController::setVibrationEnabled( zp_bool enabled ) {
	bool wasEnabled = m_isRumbleEnabled;
	m_isRumbleEnabled = enabled;
	if( wasEnabled ) stopVibrate();
}
zp_bool zpController::isVibrationEnabled() const {
	return m_isRumbleEnabled;
}

void zpController::addRightVibration( zp_float duration, zp_ushort amount ) {}
void zpController::addLeftVibration( zp_float duration, zp_ushort amount ) {}

void zpController::clear() {

}