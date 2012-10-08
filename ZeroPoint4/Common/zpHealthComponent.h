#pragma once
#ifndef ZP_HEALTH_COMPONENT_H
#define ZP_HEALTH_COMPONENT_H

struct zpHealthChangeData {
	zp_float prevHealth;
	zp_float currentHealth;
	zp_float maxHealth;
	zp_float healthChange;
};

class zpHealthComponent : public zpComponent {
public:
	zpHealthComponent();
	virtual ~zpHealthComponent();

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	zp_float getHealth() const;
	zp_float getMaxHealth() const;
	zp_float getHealthPercent() const;

	void gainHealth( zp_float health );
	void loseHealth( zp_float health );
	void setMaxHealth( zp_float health, zp_bool scaleCurrentHealth = false );
	void setInvulnerable( zp_bool invulnerable );

	zp_bool isDead() const;
	zp_bool isInvulnerable() const;

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zp_float m_health;
	zp_float m_maxHealth;

	zp_bool m_isInvulnerable;
};

#endif