#pragma once
#ifndef ZP_INPUT_MANAGER_H
#define ZP_INPUT_MANAGER_H

class zpInputManager : public zpGameManager {
public:
	zpInputManager();
	virtual ~zpInputManager();

	zpKeyboard* getKeyboard();
	zpController* getController( zpControllerNumber controller );
	zpMouse* getMouse();

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	void receiveMessage( const zpMessage& message );

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zpKeyboard* m_keyboard;
	zpMouse* m_mouse;
	zpController* m_controllers[4];
};

#endif