#include "zpPhysics.h"

#include "zpPhysicsLib.inl"
#include "src/btBulletCollisionCommon.h"
#include "src/btBulletDynamicsCommon.h"

#include "src/LinearMath/btIDebugDraw.h"
#include "src/BulletCollision/CollisionDispatch/btGhostObject.h"

class zpPhysicsDebugDrawerWrapper : public btIDebugDraw
{
public:
	zpPhysicsDebugDrawerWrapper( zpIDebugPhysicsDebugDrawer* drawer ) : m_drawer( drawer ), m_debugMode( 0 ) {}
	virtual ~zpPhysicsDebugDrawerWrapper() { m_drawer = ZP_NULL; m_debugMode = 0; }

	void drawLine( const btVector3& from, const btVector3& to, const btVector3& color )
	{
		zpVector4f f = zpMath::Vector4( from.x(), from.y(), from.z(), 1 );
		zpVector4f t = zpMath::Vector4( to.x(), to.y(), to.z(), 1 );
		zpColor4f c( color.x(), color.y(), color.z(), 1 );

		m_drawer->drawLine( f, t, c, c );
	}

	void drawLine( const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor )
	{
		zpVector4f f = zpMath::Vector4( from.x(), from.y(), from.z(), 1 );
		zpVector4f t = zpMath::Vector4( to.x(), to.y(), to.z(), 1 );
		zpColor4f fc( fromColor.x(), fromColor.y(), fromColor.z(), 1 );
		zpColor4f tc( toColor.x(), toColor.y(), toColor.z(), 1 );

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

void zpPhysicTimeStepCallback( btDynamicsWorld *world, btScalar timeStep )
{
	zpPhysicsEngine* engine = static_cast< zpPhysicsEngine* >( world->getWorldUserInfo() );
	engine->fixedTimeStep( timeStep );
}

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
	, m_gravity( zpMath::Vector4( 0, -9.8f, 0, 0 ) )
{}
zpPhysicsEngine::~zpPhysicsEngine()
{}

struct zpFilterCallback : public btOverlapFilterCallback
{
	// return true when pairs need collision
	bool needBroadphaseCollision( btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1 ) const
	{
		bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) &&
			(proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
		//add some additional logic here that modified 'collides'
		return collides;
	}
};

void zpPhysicsEngine::create()
{
	btBroadphaseInterface* broadphase = new btDbvtBroadphase;

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration;
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher( collisionConfiguration );

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btGhostPairCallback* ghostCallback = new btGhostPairCallback;
	btOverlapFilterCallback* filterCallback = new zpFilterCallback;

	btVector3 gravity;
	zpMath::Vector4Store4( m_gravity, gravity.m_floats );

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver, collisionConfiguration );
	dynamicsWorld->setGravity( gravity );
	dynamicsWorld->getPairCache()->setInternalGhostPairCallback( ghostCallback );
	dynamicsWorld->getPairCache()->setOverlapFilterCallback( filterCallback );
	dynamicsWorld->setInternalTickCallback( zpPhysicTimeStepCallback, this, false );

	m_broardphase = broadphase;
	m_collisionConfig = collisionConfiguration;
	m_dispatcher = dispatcher;
	m_solver = solver;
	m_dynamicsWorld = dynamicsWorld;
	m_ghostPairCallback = ghostCallback;
	m_filterCallback = filterCallback;

	m_colliderCache.create();
	m_collisionMask.create();
}
void zpPhysicsEngine::destroy()
{
	btBroadphaseInterface* broadphase = (btBroadphaseInterface*)m_broardphase;
	btDefaultCollisionConfiguration* collisionConfiguration = (btDefaultCollisionConfiguration*)m_collisionConfig;
	btCollisionDispatcher* dispatcher = (btCollisionDispatcher*)m_dispatcher;
	btSequentialImpulseConstraintSolver* solver = (btSequentialImpulseConstraintSolver*)m_solver;
	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;
	zpPhysicsDebugDrawerWrapper* debugDrawer = (zpPhysicsDebugDrawerWrapper*)m_debugDrawer;
	btGhostPairCallback* ghostCallback = (btGhostPairCallback*)m_ghostPairCallback;
	btOverlapFilterCallback* filterCallback = (zpFilterCallback*)m_filterCallback;

	dynamicsWorld->setDebugDrawer( ZP_NULL );

	ZP_SAFE_DELETE( broadphase );
	ZP_SAFE_DELETE( collisionConfiguration );
	ZP_SAFE_DELETE( dispatcher );
	ZP_SAFE_DELETE( solver );
	ZP_SAFE_DELETE( dynamicsWorld );
	ZP_SAFE_DELETE( ghostCallback );
	ZP_SAFE_DELETE( filterCallback );

	ZP_SAFE_DELETE( debugDrawer );

	CProfileManager::CleanupMemory();

	m_colliderCache.destroy();
	m_collisionMask.destroy();

	m_broardphase = ZP_NULL;
	m_collisionConfig = ZP_NULL;
	m_dispatcher = ZP_NULL;
	m_solver = ZP_NULL;
	m_dynamicsWorld = ZP_NULL;
	m_debugDrawer = ZP_NULL;
	m_ghostPairCallback = ZP_NULL;
	m_filterCallback = ZP_NULL;
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
	zpMath::Vector4Store4( m_gravity, g.m_floats );

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
	btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)body->getCollisionGhost();
	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;

	dynamicsWorld->addAction( action );
	dynamicsWorld->addCollisionObject( ghost, body->getGroup(), body->getMask() );
}
void zpPhysicsEngine::removeKinematicBody( zpKinematicBody* body )
{
	btActionInterface* action = (btActionInterface*)body->getKinematicController();
	btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)body->getCollisionGhost();
	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;
	
	dynamicsWorld->removeAction( action );
	dynamicsWorld->removeCollisionObject( ghost );
}

void zpPhysicsEngine::addPhantom( zpPhantom* phantom )
{
	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;
	btGhostObject* ghost = (btGhostObject*)phantom->getPhantom();

	dynamicsWorld->addCollisionObject( ghost, phantom->getGroup(), phantom->getMask() );
	m_phantoms.pushBack( phantom );
}
void zpPhysicsEngine::removePhantom( zpPhantom* phantom )
{
	btDiscreteDynamicsWorld* dynamicsWorld = (btDiscreteDynamicsWorld*)m_dynamicsWorld;
	btGhostObject* ghost = (btGhostObject*)phantom->getPhantom();

	dynamicsWorld->removeCollisionObject( ghost );
	m_phantoms.eraseAll( phantom );
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
	zpMath::Vector4Store4( fromWorld, from.m_floats );
	zpMath::Vector4Store4( toWorld, to.m_floats );

	btCollisionWorld::ClosestRayResultCallback cb( from, to );
	dynamicsWorld->rayTest( from, to, cb );

	if( cb.hasHit() )
	{
		hit.position = zpMath::Vector4Load4( cb.m_hitPointWorld.m_floats );
		hit.normal = zpMath::Vector4Load4( cb.m_hitNormalWorld.m_floats );
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

void zpPhysicsEngine::fixedTimeStep( zp_float timeStep )
{
	m_phantoms.foreach( [ this ]( zpPhantom* p ) {
		p->processCollisions( m_dynamicsWorld );
	} );
}

zpColliderCache* zpPhysicsEngine::getColliderCache()
{
	return &m_colliderCache;
}
zpCollisionMask* zpPhysicsEngine::getCollisionMask()
{
	return &m_collisionMask;
}
