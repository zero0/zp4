#pragma once
#ifndef ZP_PHYSICS_MANAGER_H
#define ZP_PHYSICS_MANAGER_H

class btBroadphaseInterface;
class btCollisionConfiguration;
class btCollisionDispatcher;
class btConstraintSolver;
class btDynamicsWorld;

class zpPhysicsManager : public zpGameManager {
public:
	zpPhysicsManager();
	virtual ~zpPhysicsManager();

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	void receiveMessage( const zpMessage& message );

	btDynamicsWorld* getWorld() const;

	zp_short getCollisionGroup( const zpString& groupName );
	zp_short getCollisionMask( const zpString& mask );

	void setGravity( const zpVector4f& gravity );
	const zpVector4f& getGravity() const;

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	btBroadphaseInterface* m_broadphase;
	btCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher;
	btConstraintSolver* m_solver;
	btDynamicsWorld* m_world;

	zp_float m_fixedTimeStep;
	zp_int m_maxSubSteps;
	zpVector4f m_gravity;

	zpArrayList<zpString> m_collisionGroups;
	zpHashMap<zpString, zp_short> m_collisionMasks;
};

#endif