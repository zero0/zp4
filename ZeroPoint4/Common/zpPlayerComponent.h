#pragma once
#ifndef ZP_PLAYER_COMPONENT_H
#define ZP_PLAYER_COMPONENT_H

enum zpPlayerNumber
{
	ZP_PLAYER_NUMBER_1,
	ZP_PLAYER_NUMBER_2,
	ZP_PLAYER_NUMBER_3,
	ZP_PLAYER_NUMBER_4,

	zpPlayerNumber_Count,
};

class zpPlayerComponent : public zpComponent
{
public:
	zpPlayerComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpPlayerComponent();

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate( zp_float deltaTime, zp_float realTime );
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	zpPlayerNumber m_player;
};

#endif
