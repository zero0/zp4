#pragma once
#ifndef ZP_GAME_MANAGER_H
#define ZP_GAME_MANAGER_H

enum zpGameManagerFlag
{
	ZP_GAME_MANAGER_FLAG_ENABLED,
	ZP_GAME_MANAGER_FLAG_CREATED,
};

ZP_ABSTRACT_CLASS zpGameManager : public zpMessageReceiver, public zpSerializable
{
public:
	zpGameManager();
	virtual ~zpGameManager();

	void create();
	void destroy();

	void update();
	void simulate();

	void setEnabled( zp_bool enabled );
	zp_bool isEnabled() const;

	void setGame( zpGame* game );
	zpGame* getGame() const;

protected:
	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void onUpdate() = 0;
	virtual void onSimulate() {};

	virtual void onEnabled() = 0;
	virtual void onDisabled() = 0;

private:
	zpFlag8 m_flags;

	zpGame* m_game;
};

#endif