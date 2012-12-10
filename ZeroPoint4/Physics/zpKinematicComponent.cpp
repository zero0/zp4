#include "zpPhysics.h"
#include "src\btBulletCollisionCommon.h"
#include "src\BulletCollision\CollisionDispatch\btGhostObject.h"
#include "src\btBulletDynamicsCommon.h"
#include "src\BulletDynamics\Character\btKinematicCharacterController.h"
#include "src\BulletDynamics\Character\btCharacterControllerInterface.h"

zpKinematicComponent::zpKinematicComponent() 
	: m_character( ZP_NULL )
	, m_ghostObject( ZP_NULL )
	, m_manager( ZP_NULL )
	, m_shouldAddOnCreate( true )
	, m_shouldAddOnEnableDisable( true )
	, m_isInWorld( false )
	, m_stepHeight( .35f )
{}
zpKinematicComponent::~zpKinematicComponent() {}

void zpKinematicComponent::receiveMessage( const zpMessage& message ) {}

void zpKinematicComponent::serialize( zpSerializedOutput* out ) {}
void zpKinematicComponent::deserialize( zpSerializedInput* in ) {}

void zpKinematicComponent::addToWorld() const {
	m_isInWorld = true;
	m_manager->getWorld()->addCharacter( m_character );
}
void zpKinematicComponent::removeFromWorld() const {
	m_manager->getWorld()->removeCharacter( m_character );
	m_isInWorld = false;
}
zp_bool zpKinematicComponent::isInWorld() const {
	return m_isInWorld;
}

void zpKinematicComponent::onCreate() {
	if( m_character ) return;

	zpColliderComponent* collider = getParentGameObject()->getComponents()->getColliderComponent();
	if( !collider ) return;
	if( !collider->isCreated() ) collider->create();

	m_manager = getGame()->getGameManagers()->getPhysicsManager();
	
	btConvexShape* shape = (btConvexShape*)collider->getShape();
	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setCollisionShape( shape );
	m_ghostObject->setCollisionFlags( btCollisionObject::CF_CHARACTER_OBJECT );

	m_character = new btKinematicCharacterController( m_ghostObject, shape, m_stepHeight );

	m_manager->getWorld()->addCharacter( m_character );
}
void zpKinematicComponent::onDestroy() {}

void zpKinematicComponent::onUpdate() {
	zp_float dt = zpTime::getInstance()->getDeltaSeconds();

	btTransform& transform = m_ghostObject->getWorldTransform();

	btVector3 strafeDir = transform.getBasis()[0];
	btVector3 upDir = transform.getBasis()[1];
	btVector3 forwardDir = transform.getBasis()[2];

	btVector3 walkDirection;
	zp_float movementSpeed = m_movementVelocity * dt;

	if( m_jump ) {
		if( m_character->canJump() ) {
			m_character->jump();
		}
		m_jump = false;
	}
	
	walkDirection *= movementSpeed;
	m_character->setWalkDirection( walkDirection );
}

void zpKinematicComponent::onEnabled() {
	if( m_shouldAddOnEnableDisable ) addToWorld();
}
void zpKinematicComponent::onDisabled() {
	if( m_shouldAddOnEnableDisable ) removeFromWorld();
}
