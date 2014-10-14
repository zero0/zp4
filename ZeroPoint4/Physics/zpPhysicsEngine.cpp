#include "zpPhysics.h"

#include "zpPhysicsLib.inc"
#include "src/btBulletCollisionCommon.h"
#include "src/btBulletDynamicsCommon.h"
#include "src/LinearMath/btIDebugDraw.h"

class zpPhysicsDebugDrawerWrapper : public btIDebugDraw
{
public:
	zpPhysicsDebugDrawerWrapper( zpIDebugPhysicsDebugDrawer* drawer ) : m_drawer( drawer ), m_debugMode( 0 ) {}
	virtual ~zpPhysicsDebugDrawerWrapper() { m_drawer = ZP_NULL; m_debugMode = 0; }

	void drawLine( const btVector3& from, const btVector3& to, const btVector3& color )
	{
		zpVector4f f( from.getX(), from.getY(), from.getZ(), 1.f );
		zpVector4f t( to.getX(), to.getY(), to.getZ(), 1.f );
		zpColor4f c( color.getX(), color.getY(), color.getZ(), 1.f );

		m_drawer->drawLine( f, t, c, c );
	}

	void drawLine( const btVector3& from,const btVector3& to, const btVector3& fromColor, const btVector3& toColor )
	{
		zpVector4f f( from.getX(), from.getY(), from.getZ(), 1.f );
		zpVector4f t( to.getX(), to.getY(), to.getZ(), 1.f );
		zpColor4f fc( fromColor.getX(), fromColor.getY(), fromColor.getZ(), 1.f );
		zpColor4f tc( toColor.getX(), toColor.getY(), toColor.getZ(), 1.f );

		m_drawer->drawLine( f, t, fc, tc );
	}

	void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color) {}
	void reportErrorWarning(const char* warningString) {}
	void draw3dText(const btVector3& location,const char* textString) {}
	void setDebugMode(int debugMode) { m_debugMode = debugMode; }
	int getDebugMode() const { return m_debugMode; }

private:
	zpIDebugPhysicsDebugDrawer* m_drawer;
	zp_int m_debugMode;
};

zpPhysicsEngine::zpPhysicsEngine()
	: m_dynamicsWorld( ZP_NULL )
	, m_broardphase( ZP_NULL )
	, m_collisionConfig( ZP_NULL )
	, m_dispatcher( ZP_NULL )
	, m_solver( ZP_NULL )
	, m_debugDrawer( ZP_NULL )
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
	zpPhysicsDebugDrawerWrapper* debugDrawer = (zpPhysicsDebugDrawerWrapper*)m_debugDrawer;

	dynamicsWorld->setDebugDrawer( ZP_NULL );

	ZP_SAFE_DELETE( broadphase );
	ZP_SAFE_DELETE( collisionConfiguration );
	ZP_SAFE_DELETE( dispatcher );
	ZP_SAFE_DELETE( solver );
	ZP_SAFE_DELETE( dynamicsWorld );
	ZP_SAFE_DELETE( debugDrawer );

	m_broardphase = ZP_NULL;
	m_collisionConfig = ZP_NULL;
	m_dispatcher = ZP_NULL;
	m_solver = ZP_NULL;
	m_dynamicsWorld = ZP_NULL;
	m_debugDrawer = ZP_NULL;
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

void zpPhysicsEngine::addKinematicBody( zpKinematicBody* body )
{
	btActionInterface* action = (btActionInterface*)body->getKinematicController();
	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;
	dynamicsWorld->addAction( action );
}
void zpPhysicsEngine::removeKinematicBody( zpKinematicBody* body )
{
	btActionInterface* action = (btActionInterface*)body->getKinematicController();
	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;
	dynamicsWorld->removeAction( action );
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
		hit.normal.load4( cb.m_hitNormalWorld.m_floats );
		hit.hitObject = cb.m_collisionObject ? cb.m_collisionObject->getUserPointer() : ZP_NULL;
		return true;
	}

	return false;
}

void zpPhysicsEngine::setDebugDrawer( zpIDebugPhysicsDebugDrawer* drawer )
{
	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;
	zpPhysicsDebugDrawerWrapper* debugDrawer = (zpPhysicsDebugDrawerWrapper*)m_debugDrawer;

	if( drawer == ZP_NULL )
	{
		dynamicsWorld->setDebugDrawer( ZP_NULL );

		ZP_SAFE_DELETE( debugDrawer );
		m_debugDrawer = ZP_NULL;
	}
	else
	{
		if( m_debugDrawer == ZP_NULL )
		{
			zpPhysicsDebugDrawerWrapper* wrapper = new zpPhysicsDebugDrawerWrapper( drawer );
			wrapper->setDebugMode( btIDebugDraw::DBG_DrawWireframe );

			debugDrawer = wrapper;
			m_debugDrawer = wrapper;
		}
		dynamicsWorld->setDebugDrawer( debugDrawer );
	}
}
void zpPhysicsEngine::debugDraw()
{
	if( m_debugDrawer != ZP_NULL )
	{
		btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;
		dynamicsWorld->debugDrawWorld();
	}
}
