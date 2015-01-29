#pragma once
#ifndef ZP_BREAKABLE_COMPONENT_H
#define ZP_BREAKABLE_COMPONENT_H

enum zpBreakableSetMaxHealthType
{
	ZP_BREAKABLE_SET_MAX_HEALTH_NONE,
	ZP_BREAKABLE_SET_MAX_HEALTH_SCALE,
	ZP_BREAKABLE_SET_MAX_HEALTH_FILL,
};

class zpBreakableComponent : public zpComponent, public zpEventListener
{
public:
	zpBreakableComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpBreakableComponent();

	zp_bool isBroken() const;

	zp_float getHealth() const;
	zp_float getMaxHealth() const;

	zp_int getHealthAsInt() const;
	zp_int getMaxHealthAsInt() const;

	void setHealth( zp_float health );
	void setMaxHeath( zp_float maxHealth, zpBreakableSetMaxHealthType scale = ZP_BREAKABLE_SET_MAX_HEALTH_NONE );

	void gainHealth( zp_float change );
	void loseHealth( zp_float change );

	void handleEvent( const zpEvent& e, zpObject* sender );

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate( zp_float deltaTime, zp_float realTime );
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	void doBreak();

	zp_bool m_justBroke;
	zp_bool m_destroyObjectOnBreak;

	zp_float m_health;
	zp_float m_maxHealth;

	zp_float m_healthDecayPerSecond;

	zpString m_breakOnMessage;
	zpString m_messageOnBreak;

	zpString m_breakOnEvent;
	zpString m_eventOnBreak;

	zpString m_replacementObject;
	zpArrayList< zpString > m_breakIntoObjects;

	zpEventHandler m_breakEventHandler;
};

class zpBreakableComponentPool : public zpContentPool< zpBreakableComponent, 8 >
{
public:
	zpBreakableComponentPool();
	virtual ~zpBreakableComponentPool();

	void update( zp_float deltaTime, zp_float realTime );
};

#endif
