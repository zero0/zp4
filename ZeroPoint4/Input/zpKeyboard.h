#pragma once
#ifndef ZP_KEYBOARD_H
#define ZP_KEYBOARD_H

enum zpKeyCode : zp_ushort {
	ZP_KEY_CODE_NULL =			0x0000,

	ZP_KEY_CODE_MOD_SHIFT =		0x1000,
	ZP_KEY_CODE_MOD_ALT =		0x2000,
	ZP_KEY_CODE_MOD_CTRL =		0x4000
};

#define ZP_IS_KEY_CODE_MODDED( code )			( 0x7000 & (code) > 0 )
#define ZP_IS_KEY_CODE_MOD_DOWN( mod, code )	( ZP_KEY_CODE_MOD_##mod & (code) == ZP_KEY_CODE_MOD_##mod )
#define ZP_STRIP_KEY_CODE_MOD( code )			( 0x0FFF & (code) )

ZP_PURE_INTERFACE zpKeyboardListener {
public:
	virtual void onKeyDown( zpKeyCode key ) = 0;
	virtual void onKeyRepeat( zpKeyCode key ) = 0;
	virtual void onKeyUp( zpKeyCode key ) = 0;
};

class zpKeyboard {
public:
	~zpKeyboard();

private:
	zpKeyboard();
	
	friend class zpInputManager;
};

#endif