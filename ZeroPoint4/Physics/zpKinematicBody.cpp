#include "zpPhysics.h"

#include "zpPhysicsLib.inc"
#include "src/btBulletCollisionCommon.h"
#include "src/btBulletDynamicsCommon.h"
#include "src/BulletDynamics/Character/btCharacterControllerInterface.h"
#include "src/BulletDynamics/Character/btKinematicCharacterController.h"

zpKinematicBody::zpKinematicBody()
	: m_kinematicBody( ZP_NULL )
	, m_motionState( ZP_NULL )
	, m_collider( ZP_NULL )
	, m_mass( 1.0f )
	, m_group( 0 )
	, m_mask( 0 )
{
}
zpKinematicBody::~zpKinematicBody()
{
}

class zpKinematicCharacterController : public btCharacterControllerInterface
{
public:
	void setup( btRigidBody* body, btMotionState* motion )
	{
		m_rigidBody = body;
		m_motionState = motion;
	}
	void destroy()
	{
		m_rigidBody = ZP_NULL;
		m_motionState = ZP_NULL;
	}

	void updateAction( btCollisionWorld* collisionWorld, btScalar deltaTimeStep )
	{
		playerStep( collisionWorld, deltaTimeStep );
	}

	void debugDraw( btIDebugDraw* debugDrawer )
	{
	}

	void setWalkDirection( const btVector3& walkDirection )
	{
		btVector3FloatData d;
		walkDirection.serializeFloat( d );
		m_walkDirection.load4( d.m_floats );
	}
	void setVelocityForTimeInterval( const btVector3& velocity, btScalar timeInterval )
	{

	}
	void reset()
	{
		m_walkDirection = zpVector4f( 0.f );
	}
	void warp( const btVector3& origin )
	{
		btTransform xform;
		m_motionState->getWorldTransform( xform );

		xform.setOrigin( origin );

		m_motionState->setWorldTransform( xform );
	}

	void preStep( btCollisionWorld* collisionWorld )
	{
		btTransform xform;
		m_motionState->getWorldTransform( xform );

		btVector3 down = -xform.getBasis()[1];
		btVector3 forward = xform.getBasis()[2];

		down.normalize();
		forward.normalize();

		m_raySource[0] = xform.getOrigin();
		m_raySource[1] = xform.getOrigin();

		m_rayTarget[0] = m_raySource[0] + down * m_halfHeight * btScalar(1.1);
		m_rayTarget[1] = m_raySource[1] + forward * m_halfHeight * btScalar(1.1);

		class ClosestNotMe : public btCollisionWorld::ClosestRayResultCallback
		{
		public:
			ClosestNotMe( btRigidBody* me ) : btCollisionWorld::ClosestRayResultCallback( btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f) )
			{
				m_me = me;
			}

			btScalar addSingleResult( btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace )
			{
				return rayResult.m_collisionObject == m_me ? 1.f : ClosestRayResultCallback::addSingleResult( rayResult, normalInWorldSpace );
			}
		protected:
			btRigidBody* m_me;
		};

		ClosestNotMe rayCallback( m_rigidBody );
		for( zp_int i = 0; i < 2; ++i )
		{
			collisionWorld->rayTest( m_raySource[ i ], m_rayTarget[ i ], rayCallback );
			if( rayCallback.hasHit() )
			{
				m_rayLambda[ i ] = rayCallback.m_closestHitFraction;
			}
			else
			{
				m_rayLambda[ i ] = 1.f;
			}
		}
	}
	void playerStep( btCollisionWorld* collisionWorld, btScalar dt )
	{
		btTransform xform;
		m_motionState->getWorldTransform( xform );

		btVector3 linearVel = m_rigidBody->getLinearVelocity();
		btScalar speed = linearVel.length();

		btVector3 forwardDir = xform.getBasis()[2];
		forwardDir.normalize();

		btVector3 walkDirection = btVector3(0.0f, 0.0f, 0.0f);
		btScalar walkSpeed = m_walkVelocity * dt;

		walkDirection += forwardDir;

		if( onGround() )
		{
			linearVel *= m_groundVelocityDamp;
			m_rigidBody->setLinearVelocity( linearVel );
		}
		else if( speed < m_maxLinearVelocity )
		{
			linearVel += walkDirection * walkSpeed;
			m_rigidBody->setLinearVelocity( linearVel );
		}

		m_motionState->setWorldTransform( xform );
		m_rigidBody->setCenterOfMassTransform( xform );
	}
	bool canJump() const
	{
		return onGround();
	}
	void jump()
	{
		if( !canJump() )
		{
			return;
		}

		btTransform xform;
		m_motionState->getWorldTransform( xform );

		btVector3 up = xform.getBasis()[1];
		up.normalize();

		btScalar s = btScalar( m_jumpVelocity ) / m_rigidBody->getInvMass();
		m_rigidBody->applyCentralImpulse( up * s );
	}

	bool onGround() const
	{
		return m_rayLambda[0] < 1.f;
	}

private:
	zp_float m_halfHeight;
	zp_float m_groundVelocityDamp;
	zp_float m_jumpVelocity;
	zp_float m_turnAngle;
	zp_float m_maxLinearVelocity;
	zp_float m_walkVelocity;
	zp_float m_turnVelocity;
	zpVector4f m_walkDirection;

	btVector3 m_raySource[2];
	btVector3 m_rayTarget[2];
	btVector3 m_rayNormal[2];
	btScalar m_rayLambda[2];

	btRigidBody* m_rigidBody;
	btMotionState* m_motionState;
};

void zpKinematicBody::create( const zpMatrix4f& transform, const zpBison::Value& v )
{
	m_mass = v[ "Mass" ].asFloat();

	m_group = zpCollisionMask::getInstance()->getCollisionMask( v[ "Group" ].asCString() );
	m_mask = zpCollisionMask::getInstance()->getCollisionMask( v[ "Mask" ].asCString() );
	m_collider = zpColliderCache::getInstance()->getCollider( v[ "Collider" ] );
	zp_float stepHeight = v[ "StepHeight" ].asFloat();

	btCollisionShape* colliderShape = (btCollisionShape*)m_collider->getCollider();

	btQuaternion rot( btQuaternion::getIdentity() );
	btVector3 pos;
	transform.getRow( 3 ).store4( pos.m_floats );

	btTransform startTransform( rot, pos );

	zpKinematicCharacterController* controller = new zpKinematicCharacterController;
	btMotionState* motion = new btDefaultMotionState( startTransform );

	btRigidBody::btRigidBodyConstructionInfo info( m_mass, motion, colliderShape );
	btRigidBody* body = new btRigidBody( info );
	body->setCollisionFlags( btRigidBody::CF_KINEMATIC_OBJECT );
	body->setSleepingThresholds( 0.f, 0.f );
	body->setAngularFactor( 0.f );

	controller->setup( body, motion );

	m_controller = controller;
	m_motionState = motion;
	m_kinematicBody = body;
	
}
void zpKinematicBody::destroy()
{
	zpColliderCache::getInstance()->removeCollider( (zpCollider*)m_collider );
	m_collider = ZP_NULL;

	zpKinematicCharacterController* controller = (zpKinematicCharacterController*)m_controller;
	btMotionState* motion = (btMotionState*)m_motionState;
	btRigidBody* body = (btRigidBody*)m_kinematicBody;

	controller->destroy();

	ZP_SAFE_DELETE( controller );
	ZP_SAFE_DELETE( motion );
	ZP_SAFE_DELETE( body );

	m_controller = ZP_NULL;
	m_kinematicBody = ZP_NULL;
	m_motionState = ZP_NULL;
}

zp_bool zpKinematicBody::getMatrix( zpMatrix4f& transform )
{
	btMotionState* motion = (btMotionState*)m_motionState;

	btTransform t;
	motion->getWorldTransform( t );

	t.getOpenGLMatrix( transform.getData() );
	return true;
}

zp_short zpKinematicBody::getGroup() const
{
	return m_group;
}
zp_short zpKinematicBody::getMask() const
{
	return m_mask;
}

void zpKinematicBody::jump()
{
	zpKinematicCharacterController* controller = (zpKinematicCharacterController*)m_controller;
	controller->jump();
}
zp_bool zpKinematicBody::canJump() const
{
	zpKinematicCharacterController* controller = (zpKinematicCharacterController*)m_controller;
	return controller->canJump();
}
zp_bool zpKinematicBody::onGround() const
{
	zpKinematicCharacterController* controller = (zpKinematicCharacterController*)m_controller;
	return controller->onGround();
}

void zpKinematicBody::warp( const zpVector4f& position )
{
	btVector3FloatData data;
	position.store4( data.m_floats );

	btVector3 pos;
	pos.deSerializeFloat( data );

	zpKinematicCharacterController* controller = (zpKinematicCharacterController*)m_controller;
	controller->warp( pos );
}

void zpKinematicBody::setWalkDirection( const zpVector4f& direction )
{
	btVector3FloatData data;
	direction.store4( data.m_floats );

	btVector3 dir;
	dir.deSerializeFloat( data );

	zpKinematicCharacterController* controller = (zpKinematicCharacterController*)m_controller;
	controller->setWalkDirection( dir );
}

zp_handle zpKinematicBody::getKinematicController() const
{
	return m_controller;
}
zp_handle zpKinematicBody::getKinematicBody() const
{
	return m_kinematicBody;
}
zp_handle zpKinematicBody::getMotionState() const
{
	return m_motionState;
}
