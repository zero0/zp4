#include "zpPhysics.h"
#include "src\btBulletCollisionCommon.h"
#include "src\BulletCollision\CollisionDispatch\btGhostObject.h"
#include "src\btBulletDynamicsCommon.h"

zpPhysicsManager::zpPhysicsManager() 
	: m_broadphase( ZP_NULL )
	, m_collisionConfiguration( ZP_NULL )
	, m_dispatcher( ZP_NULL )
	, m_solver( ZP_NULL )
	, m_world( ZP_NULL )
	, m_fixedTimeStep( 1.f / 60.f )
	, m_gravity( 0, -9.81f, 0 )
{}
zpPhysicsManager::~zpPhysicsManager() {}

void zpPhysicsManager::serialize( zpSerializedOutput* out ) {}
void zpPhysicsManager::deserialize( zpSerializedInput* in ) {}

void zpPhysicsManager::receiveMessage( const zpMessage& message ) {}

btDynamicsWorld* zpPhysicsManager::getWorld() const {
	return m_world;
}

void zpPhysicsManager::onCreate() {
	m_broadphase = new btDbvtBroadphase();
	m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback( new btGhostPairCallback() );
	
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher( m_collisionConfiguration );
	
	m_solver = new btSequentialImpulseConstraintSolver();

	m_world = new btDiscreteDynamicsWorld( m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration );
	
	btVector3 gravity;
	m_gravity.store3( gravity );
	m_world->setGravity( gravity );
}
void zpPhysicsManager::onDestroy() {
	ZP_SAFE_DELETE( m_world );
	ZP_SAFE_DELETE( m_solver );
	ZP_SAFE_DELETE( m_dispatcher );
	ZP_SAFE_DELETE( m_collisionConfiguration );
	ZP_SAFE_DELETE( m_broadphase );
}

void zpPhysicsManager::onUpdate() {
	m_world->stepSimulation( zpTime::getInstance()->getDeltaSeconds(), 6, m_fixedTimeStep );
}

void zpPhysicsManager::onEnabled() {}
void zpPhysicsManager::onDisabled() {}