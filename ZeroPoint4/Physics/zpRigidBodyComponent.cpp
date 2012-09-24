#include "zpPhysics.h"
#include "src\btBulletCollisionCommon.h"
#include "src\btBulletDynamicsCommon.h"

zpRigidBodyComponent::zpRigidBodyComponent() 
	: m_body( ZP_NULL )
	, m_motionState( ZP_NULL )
	, m_manager( ZP_NULL )
	, m_shouldAddOnCreate( false )
	, m_shouldAddOnEnableDisable( true )
	, m_collisionGroup( 0 )
	, m_collisionMask( 0 )
	, m_mass( 1.f )
{}
zpRigidBodyComponent::~zpRigidBodyComponent() {
	destroy();
}

void zpRigidBodyComponent::receiveMessage( const zpMessage& message ) {}

void zpRigidBodyComponent::serialize( zpSerializedOutput* out ) {}
void zpRigidBodyComponent::deserialize( zpSerializedInput* in ) {}

void zpRigidBodyComponent::addToWorld() const {
	m_manager->getWorld()->addRigidBody( m_body, m_collisionGroup, m_collisionMask );
}
void zpRigidBodyComponent::removeFromWorld() const {
	m_manager->getWorld()->removeRigidBody( m_body );
}
zp_bool zpRigidBodyComponent::isInWorld() const {
	return m_body->isInWorld();
}

void zpRigidBodyComponent::onCreate() {
	if( m_body ) return;

	zpColliderComponent* collider = getParentGameObject()->getComponentOfType<zpColliderComponent>();
	if( !collider ) return;
	if( !collider->isCreated() ) collider->create();

	m_manager = getGameManagerOfType<zpPhysicsManager>();

	btCollisionShape* shape = collider->getShape();

	btVector3 inertia;
	shape->calculateLocalInertia( m_mass, inertia );
	m_motionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo info ( m_mass, m_motionState, shape, inertia );
	
	m_body = new btRigidBody( info );

	if( m_shouldAddOnCreate ) {
		addToWorld();
	}
}
void zpRigidBodyComponent::onDestroy() {
	removeFromWorld();
	ZP_SAFE_DELETE( m_motionState );
	ZP_SAFE_DELETE( m_body );
	m_manager = ZP_NULL;
}

void zpRigidBodyComponent::onUpdate() {
	btTransform transform;
	m_motionState->getWorldTransform( transform );

	zpMatrix4f matrix;
	transform.getOpenGLMatrix( matrix );

	getParentGameObject()->setTransform( matrix );
}

void zpRigidBodyComponent::onEnabled() {
	if( m_shouldAddOnEnableDisable ) addToWorld();
}
void zpRigidBodyComponent::onDisabled() {
	if( m_shouldAddOnEnableDisable ) removeFromWorld();
}
