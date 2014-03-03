#include "zpPhysics.h"

#include "zpPhysicsLib.inc"
#include "src/btBulletCollisionCommon.h"
#include "src/btBulletDynamicsCommon.h"

zpPhysicsEngine::zpPhysicsEngine()
	: m_dynamicsWorld( ZP_NULL )
	, m_broardphase( ZP_NULL )
	, m_collisionConfig( ZP_NULL )
	, m_dispatcher( ZP_NULL )
	, m_solver( ZP_NULL )
	, m_timestep( 1.f / 60.f )
	, m_fixedTimestep( 1.f / 60.f )
	, m_numSubStep( 10 )
	, m_gravity( 0, -9.8f, 0, 0 )
{}
zpPhysicsEngine::~zpPhysicsEngine()
{}

void zpPhysicsEngine::create()
{
	btBroadphaseInterface* broadphase = new btDbvtBroadphase;

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration;
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher( collisionConfiguration );

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver, collisionConfiguration );
	dynamicsWorld->setGravity( btVector3( m_gravity.getX().getFloat(), m_gravity.getY().getFloat(), m_gravity.getZ().getFloat() ) );

	m_broardphase = broadphase;
	m_collisionConfig = collisionConfiguration;
	m_dispatcher = dispatcher;
	m_solver = solver;
	m_dynamicsWorld = dynamicsWorld;
}
void zpPhysicsEngine::destroy()
{
	btBroadphaseInterface* broadphase = (btBroadphaseInterface*)m_broardphase;
	btDefaultCollisionConfiguration* collisionConfiguration = (btDefaultCollisionConfiguration*)m_collisionConfig;
	btCollisionDispatcher* dispatcher = (btCollisionDispatcher*)m_dispatcher;
	btSequentialImpulseConstraintSolver* solver = (btSequentialImpulseConstraintSolver*)m_solver;
	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;

	ZP_SAFE_DELETE( broadphase );
	ZP_SAFE_DELETE( collisionConfiguration );
	ZP_SAFE_DELETE( dispatcher );
	ZP_SAFE_DELETE( solver );
	ZP_SAFE_DELETE( dynamicsWorld );

	m_broardphase = ZP_NULL;
	m_collisionConfig = ZP_NULL;
	m_dispatcher = ZP_NULL;
	m_solver = ZP_NULL;
	m_dynamicsWorld = ZP_NULL;
}

void zpPhysicsEngine::update( zp_float dt )
{
	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;
	dynamicsWorld->stepSimulation( dt, m_numSubStep, m_fixedTimestep );
}
void zpPhysicsEngine::simulate()
{
	
}

const zpVector4f& zpPhysicsEngine::getGravity() const
{
	return m_gravity;
}
void zpPhysicsEngine::setGravity( const zpVector4f& gravity )
{
	m_gravity = gravity;

	btVector3 g;
	m_gravity.store4( g.m_floats );

	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;
	dynamicsWorld->setGravity( g );
}

void zpPhysicsEngine::addRigidBody( zpRigidBody* body )
{
	btRigidBody* rigidBody = (btRigidBody*)body->getRigidBody();
	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;
	dynamicsWorld->addRigidBody( rigidBody, body->getGroup(), body->getMask() );
}
void zpPhysicsEngine::removeRigidBody( zpRigidBody* body )
{
	btRigidBody* rigidBody = (btRigidBody*)body->getRigidBody();
	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;
	dynamicsWorld->removeRigidBody( rigidBody );
}

void zpPhysicsEngine::setFixedTimeStep( zp_float fixedTimeStep, zp_int numSteps )
{
	m_fixedTimestep = fixedTimeStep;
	m_numSubStep = numSteps;
}

zp_bool zpPhysicsEngine::raycast( const zpVector4f& fromWorld, const zpVector4f& toWorld, zpCollisionHitResult& hit ) const
{
	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;

	btVector3 from, to;
	fromWorld.store4( from.m_floats );
	toWorld.store4( to.m_floats );

	btCollisionWorld::ClosestRayResultCallback cb( from, to );
	dynamicsWorld->rayTest( from, to, cb );

	if( cb.hasHit() )
	{
		hit.position.load4( cb.m_hitPointWorld.m_floats );
		hit.normal.load4( cb.m_hitPointWorld.m_floats );
		hit.hitObject = cb.m_collisionObject ? cb.m_collisionObject->getUserPointer() : ZP_NULL;
		return true;
	}

	return false;
}
