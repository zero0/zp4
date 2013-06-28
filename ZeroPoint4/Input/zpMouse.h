#pragma once
#ifndef ZP_MOUSE_H
#define ZP_MOUSE_H

enum zpMouseButton
{
	ZP_MOUSE_BUTTON_0 = 0,
	ZP_MOUSE_BUTTON_1,
	ZP_MOUSE_BUTTON_2,
	ZP_MOUSE_BUTTON_3,
	ZP_MOUSE_BUTTON_4,
	ZP_MOUSE_BUTTON_5,
	ZP_MOUSE_BUTTON_6,
	ZP_MOUSE_BUTTON_7,

	zpMouseButton_Count,

	// Button alias
	ZP_MOUSE_BUTTON_LEFT = ZP_MOUSE_BUTTON_0,
	ZP_MOUSE_BUTTON_RIGHT = ZP_MOUSE_BUTTON_1,
	ZP_MOUSE_BUTTON_MIDDLE = ZP_MOUSE_BUTTON_2,
	ZP_MOUSE_BUTTON_SIDE_1 = ZP_MOUSE_BUTTON_3,
	ZP_MOUSE_BUTTON_SIDE_2 = ZP_MOUSE_BUTTON_4,
};

ZP_PURE_INTERFACE zpMouseListener
{
public:
	virtual void onMouseButtonDown( zpMouseButton button ) {};
	virtual void onMouseButtonRepeat( zpMouseButton button ) {};
	virtual void onMouseButtonUp( zpMouseButton button ) {};

	virtual void onMouseScroll( zp_int scrollDelta ) {};

	virtual void onMouseMove( const zpVector2i& location ) {};
	virtual void onMouseChange( const zpVector2i& delta ) {};
};

class zpMouse
{
public:
	~zpMouse();

	void poll();
	void create();
	void destroy();

	zp_bool isCreated() const;

	zp_bool isButtonDown( zpMouseButton button ) const;
	zp_bool isButtonUp( zpMouseButton button ) const;
	const zpVector2i& getLocation() const;
	const zpVector2i& getDelta() const;
	zp_int getScrollWheel() const;

	//void setSensitivity( zp_float sensitivity );
	//zp_float getSensitivity() const;

	void addListener( zpMouseListener* listener );
	void removeListener( zpMouseListener* listener );
	void removeAllListeners();

	void onFocusGained();
	void onFocusLost();

	void onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam );

private:
	zpMouse();

	zp_bool m_isCreated;
	zp_bool m_hasFocus;

	zp_int m_scrollWheel;
	zp_int m_scrollWheelBuffer;

	zpFlag16 m_buttonBuffer;
	zpFlag16 m_buttonDownBuffer;

	zpVector2i m_location;
	zpVector2i m_locationBuffer;
	zpVector2i m_delta;

	zpArrayList<zpMouseListener*> m_listeners;

	friend class zpInputManager;
};

#endif