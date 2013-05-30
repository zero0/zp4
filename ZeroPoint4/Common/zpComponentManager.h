#pragma once
#ifndef ZP_COMPONENT_MANAGER_H
#define ZP_COMPONENT_MANAGER_H

template< typename Component, typename ComponentManager, zp_uint Count >
class zpComponentManager
{
public:
	zpComponentManager();
	~zpComponentManager();

	void update();
	void simlate();

	Component* createComponent( const zpBison::Value& componentData );
	void destroyComponent( Component* component );

protected:
	zp_uint m_firstFree;
	zp_bool m_isUsed[ Count ];
	zp_byte m_pool[ sizeof( Component ) * Count ];
};

#include "zpComponentManager.inl"

#if 0
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

#endif