#pragma once
#ifndef ZP_CHARACTER_COMPONENT_H
#define ZP_CHARACTER_COMPONENT_H

class zpCharacterState
{
public:

private:
};

class zpCharacterComponent : public zpComponent
{
public:
	zpCharacterComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpCharacterComponent();

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate( zp_float deltaTime, zp_float realTime );
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
};

#endif
