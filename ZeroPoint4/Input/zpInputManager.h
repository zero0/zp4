#pragma once
#ifndef ZP_INPUT_MANAGER_H
#define ZP_INPUT_MANAGER_H

enum
{
	ZP_INPUT_MAX_CONTROLLERS = zpControllerNumber_Count,
};

class zpInputManager : public zpWindowProcListener, public zpWindowFocusListener
{
public:
	zpInputManager();
	~zpInputManager();

	const zpKeyboard* getKeyboard();
	const zpMouse* getMouse();
	const zpController* getController( zpControllerNumber controller );

	void update();

	void onFocusGained();
	void onFocusLost();
	void onWindowProc( zp_uint uMessage, zp_uint wParam, zp_ulong lParam );

private:
	zpKeyboard m_keyboard;
	zpMouse m_mouse;
	zpFixedArrayList< zpController, ZP_INPUT_MAX_CONTROLLERS > m_controllers;
};

#endif