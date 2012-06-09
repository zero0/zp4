#pragma once
#ifndef ZP_CONTROLLER_H
#define ZP_CONTROLLER_H

#pragma comment(lib, "xinput.lib" )

enum zpControllerNumber : zp_ushort {
	ZP_CONTROLLER_1 = 0,
	ZP_CONTROLLER_2,
	ZP_CONTROLLER_3,
	ZP_CONTROLLER_4
};
enum zpControllerBatterLevel : zp_ubyte {
	ZP_CONTROLLER_BATTERY_LEVEL_EMPTY =		0,
	ZP_CONTROLLER_BATTERY_LEVEL_LOW,
	ZP_CONTROLLER_BATTERY_LEVEL_MEDIUM,
	ZP_CONTROLLER_BATTERY_LEVEL_FULL,
};
enum zpControllerBatterType : zp_ubyte {
	ZP_CONTROLLER_BATTERY_TYPE_DISCONNECTED =		0,
	ZP_CONTROLLER_BATTERY_TYPE_WIRED,
	ZP_CONTROLLER_BATTERY_TYPE_ALKALINE,
	ZP_CONTROLLER_BATTERY_TYPE_NIMH,
	ZP_CONTROLLER_BATTERY_TYPE_UNKNOWN =			0xFF,
};
enum zpControllerButton : zp_ushort {
	ZP_CONTROLLER_BUTTON_DPAD_UP =			0x0001,
	ZP_CONTROLLER_BUTTON_DPAD_DOWN =		0x0002,
	ZP_CONTROLLER_BUTTON_DPAD_LEFT =		0x0004,
	ZP_CONTROLLER_BUTTON_DPAD_RIGHT =		0x0008,
	ZP_CONTROLLER_BUTTON_START =			0x0010,
	ZP_CONTROLLER_BUTTON_BACK =				0x0020,
	ZP_CONTROLLER_BUTTON_LEFT_THUMB =		0x0040,
	ZP_CONTROLLER_BUTTON_RIGHT_THUMB =		0x0080,
	ZP_CONTROLLER_BUTTON_LEFT_SHOULDER =	0x0100,
	ZP_CONTROLLER_BUTTON_RIGHT_SHOULDER =	0x0200,
	ZP_CONTROLLER_BUTTON_A =				0x1000,
	ZP_CONTROLLER_BUTTON_B =				0x2000,
	ZP_CONTROLLER_BUTTON_X =				0x4000,
	ZP_CONTROLLER_BUTTON_Y =				0x8000,
};

ZP_PURE_INTERFACE zpControllerListener {
	virtual void onButtonDown( zpControllerButton button ) {};
	virtual void onButtonRepeat( zpControllerButton button ) {};
	virtual void onButtonUp( zpControllerButton button ) {};

	virtual void onLeftStickMove( const zpVector2i& move ) {};
	virtual void onLeftStickRepeat( const zpVector2i& move ) {};

	virtual void onRightStickMove( const zpVector2i& move ) {};
	virtual void onRightStickRepeat( const zpVector2i& move ) {};

	virtual void onLeftTriggerPull( zp_ushort amount ) {};
	virtual void onLeftTriggerRepeat( zp_ushort amount ) {};
			
	virtual void onRightTriggerPull( zp_ushort amount ) {};
	virtual void onRightTriggerRepeat( zp_ushort amount ) {};

	virtual void onControllerDisconnected() = 0;
	virtual void onControllerReconnected() = 0;
};

class zpController : public zpWindowFocusListener {
public:
	~zpController();

	void poll();
	void create();
	void destroy();

	void onFocusGained();
	void onFocusLost();

	zp_bool isConnected() const;

	void setTriggerThreshold( zp_ubyte threshold );
	void setLeftTriggerThreshold( zp_ubyte threshold );
	void setRightTriggerThreshold( zp_ubyte threshold );
	zp_ubyte getLeftTriggerThreshold() const;
	zp_ubyte getRightTriggerThreshold() const;

	void setLeftThumbDeadZone( zp_short deadzone );
	zp_short getLeftThumbDeadZone() const;

	void setRightThumbDeadZone( zp_short deadzone );
	zp_short getRightThumbDeadZone() const;

	void vibrate( zp_ushort left, zp_ushort right ) const;
	void stopVibrate() const;

	zp_bool isAPressed() const;
	zp_bool isBPressed() const;
	zp_bool isXPressed() const;
	zp_bool isYPressed() const;

	zp_bool isBackPressed() const;
	zp_bool isStartPressed() const;

	zp_bool isDPadUpPressed() const;
	zp_bool isDPadDownPressed() const;
	zp_bool isDPadLeftPressed() const;
	zp_bool isDPadRightPressed() const;

	zp_bool isLeftBumperPressed() const;
	zp_bool isRightBumperPressed() const;

	zp_bool isLeftThumbPressed() const;
	zp_bool isRightThumbPressed() const;

	zp_bool isLeftThumbMoved() const;
	zp_bool isRightThumbMoved() const;

	zp_bool isLeftTriggerPressed() const;
	zp_bool isRightTriggerPressed() const;

	zp_ubyte getLeftTrigger() const;
	zp_ubyte getRightTrigger() const;

	const zpVector2i& getLeftThumb() const;
	const zpVector2i& getRightThumb() const;

	zpControllerNumber getController() const;

	void addListener( zpControllerListener* listener );
	void removeListener( zpControllerListener* listener );
	void removeAllListeners();

	zpControllerBatterLevel getBatteryLevel() const;
	zpControllerBatterType getBatteryType() const;

	void setVibrationEnabled( zp_bool enabled );
	zp_bool isVibrationEnabled() const;

	void addRightVibration( zp_float duration, zp_ushort amount );
	void addLeftVibration( zp_float duration, zp_ushort amount );

private:
	zpController( zpControllerNumber number = ZP_CONTROLLER_1 );
	
	void clear();

	zpControllerNumber m_controller;

	zp_short m_leftThumbDeadZone;
	zp_short m_rightThumbDeadZone;

	zpVector2i m_leftThumb;
	zpVector2i m_rightThumb;

	zp_bool m_isConnected;
	zp_ushort m_buttons;

	zp_ubyte m_leftTrigger;
	zp_ubyte m_rightTrigger;

	zp_ubyte m_leftTriggerThreshold;
	zp_ubyte m_rightTriggerThreshold;

	zp_bool m_isRumbleEnabled;
	zp_float m_rightRumbleTime;
	zp_float m_rightRumbleTimer;
			
	zp_float m_leftRumbleTime;
	zp_float m_leftRumbleTimer;

	zp_ushort m_rightRumbleAmount;
	zp_ushort m_maxRightRumbleAmount;

	zp_ushort m_leftRumbleAmount;
	zp_ushort m_maxLeftRumbleAmount;

	zpArrayList<zpControllerListener*> m_listeners;

	friend class zpInputManager;
};

#endif