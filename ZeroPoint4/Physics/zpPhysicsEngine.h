#pragma once
#ifndef ZP_PHYSICS_ENGINE_H
#define ZP_PHYSICS_ENGINE_H

struct zpCollisionHitResult
{
	zpVector4f position;
	zpVector4f normal;
	void* hitObject;
};

ZP_PURE_INTERFACE zpIDebugPhysicsDebugDrawer
{
public:
	virtual void drawLine( const zpVector4f& from, const zpVector4f& to, const zpColor4f& fromColor, const zpColor4f& toColor ) = 0;
};

class zpPhysicsEngine
{
	ZP_NON_COPYABLE( zpPhysicsEngine );
public:
	zpPhysicsEngine();
	~zpPhysicsEngine();

	void create();
	void destroy();

	void update( zp_float dt );
	void simulate();

	const zpVector4f& getGravity() const;
	void setGravity( const zpVector4f& gravity );

	void addRigidBody( zpRigidBody* body );
	void removeRigidBody( zpRigidBody* body );

	void addKinematicBody( zpKinematicBody* body );
	void removeKinematicBody( zpKinematicBody* body );

	void addPhantom( zpPhantom* phanton );
	void removePhantom( zpPhantom* phanton );

	void setFixedTimeStep( zp_float fixedTimeStep, zp_int numSteps );

	zp_bool raycast( const zpVector4f& fromWorld, const zpVector4f& toWorld, zpCollisionHitResult& hit ) const;

	void setDebugDrawer( zpIDebugPhysicsDebugDrawer* drawer );
	void debugDraw();

	void fixedTimeStep( zp_float timeStep );

	zpColliderCache* getColliderCache();
	zpCollisionMask* getCollisionMask();

private:
	zp_handle m_dynamicsWorld;

	zp_handle m_broardphase;
	zp_handle m_collisionConfig;
	zp_handle m_dispatcher;
	zp_handle m_solver;
	zp_handle m_debugDrawer;
	zp_handle m_ghostPairCallback;
	zp_handle m_filterCallback;

	zp_float m_timestep;
	zp_float m_fixedTimestep;
	zp_int m_numSubStep;

	zpVector4f m_gravity;

	zpArrayList< zpPhantom* > m_phantoms;

	zpColliderCache m_colliderCache;
	zpCollisionMask m_collisionMask;
};

#endif
