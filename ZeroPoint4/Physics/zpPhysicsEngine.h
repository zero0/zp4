#pragma once
#ifndef ZP_PHYSICS_ENGINE_H
#define ZP_PHYSICS_ENGINE_H

class zpPhysicsEngine
{
	ZP_NON_COPYABLE( zpPhysicsEngine );
public:
	zpPhysicsEngine();
	~zpPhysicsEngine();

	void create();
	void destroy();

	void update();
	void simulate();

	const zpVector4f& getGravity() const;
	void setGravity( const zpVector4f& gravity );

	void addRigidBody( zpRigidBody* body );
	void removeRigidBody( zpRigidBody* body );

private:
	zp_handle m_dynamicsWorld;

	zp_handle m_broardphase;
	zp_handle m_collisionConfig;
	zp_handle m_dispatcher;
	zp_handle m_solver;

	zp_float m_timestep;
	zp_float m_fixedTimestep;
	zp_int m_numSubStep;

	zpVector4f m_gravity;
};

#endif
