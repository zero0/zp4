#pragma once
#ifndef ZP_PROJECTILE_COMPONENT_H
#define ZP_PROJECTILE_COMPONENT_H

class zpProjectileComponent : public zpComponent, public zpEventListener
{
public:
	zpProjectileComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpProjectileComponent();

	void handleEvent( const zpEvent& e, zpObject* sender );

	void fire( zpObject* target );
	void fire( const zpVector4f& end );

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate( zp_float deltaTime, zp_float realTime );
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	void onFire();

	void projectileCollided();

	zp_bool m_isRunning;
	zp_bool m_followTarget;
	zp_bool m_faceTarget;
	zp_bool m_rotate;
	zp_bool m_destroyOnCollide;

	zp_float m_angularVelocity;
	zp_float m_acceleration;
	zp_float m_speed;

	zp_float m_rotation;
	zp_float m_rotationSpeed;

	zpVector4f m_rotationAxis;
	zpVector4f m_velocity;
	zpVector4f m_initialVelocity;

	zpVector4f m_startPosition;
	zpVector4f m_endPosition;

	zpObject* m_endTarget;

	zp_float m_maxRange;
	zp_float m_projectileRadius;
	zp_float m_endRadius;

	zpBoundingSphere m_projectileSphere;
	zpBoundingSphere m_endSphere;

	zpString m_fireOnEvent;

	zpString m_eventOnCollide;
	zpString m_messageOnCollide;

	zpEventHandler m_fireEventHandler;
};

class zpProjectileComponentPool : public zpContentPool< zpProjectileComponent, 8 >
{
public:
	zpProjectileComponentPool();
	virtual ~zpProjectileComponentPool();

	void update( zp_float deltaTime, zp_float realTime );
};

#endif
