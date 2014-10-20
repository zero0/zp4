#include "zpPhysics.h"

#include "zpPhysicsLib.inc"
#include "src/btBulletCollisionCommon.h"
#include "src/btBulletDynamicsCommon.h"
#include "src/BulletDynamics/Character/btCharacterControllerInterface.h"
#include "src/BulletDynamics/Character/btKinematicCharacterController.h"
#include "src/BulletCollision/CollisionDispatch/btGhostObject.h"

zpKinematicBody::zpKinematicBody()
	: m_collider( ZP_NULL )
	, m_ghost( ZP_NULL )
	, m_mass( 1.0f )
	, m_group( 0 )
	, m_mask( 0 )
{
}
zpKinematicBody::~zpKinematicBody()
{
}

//
// http://bulletphysics.org/Bullet/phpBB3/viewtopic.php?f=9&t=5684
//
class zpKinematicClosestNotMeConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback
{
public:
	zpKinematicClosestNotMeConvexResultCallback( btCollisionObject* me, const btVector3& up, btScalar minSlopeDot )
		: btCollisionWorld::ClosestConvexResultCallback( btVector3( 0.0f, 0.0f, 0.0f ), btVector3( 0.0f, 0.0f, 0.0f ) )
		, m_me( me )
		, m_up( up )
		, m_minSlopeDot( minSlopeDot )
	{
		m_collisionFilterGroup = me->getBroadphaseHandle()->m_collisionFilterGroup;
		m_collisionFilterMask = me->getBroadphaseHandle()->m_collisionFilterMask;
	}

	btScalar addSingleResult( btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace )
	{
		if( convexResult.m_hitCollisionObject == m_me ) return btScalar( 1.0f );

		//for trigger filtering
		if( !convexResult.m_hitCollisionObject->hasContactResponse() ) return btScalar( 1.0f );

		btVector3 hitNormalWorld;
		if( normalInWorldSpace )
		{
			hitNormalWorld = convexResult.m_hitNormalLocal;
		}
		else
		{
			///need to transform normal into world space
			hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform().getBasis() * convexResult.m_hitNormalLocal;
		}

		btScalar dotUp = m_up.dot( hitNormalWorld );
		if( dotUp < m_minSlopeDot ) return btScalar( 1.0f );

		return ClosestConvexResultCallback::addSingleResult( convexResult, normalInWorldSpace );
	}

protected:
	btCollisionObject* m_me;
	const btVector3 m_up;
	btScalar m_minSlopeDot;
};

class zpKinematicCharacterController : public btCharacterControllerInterface
{
public:
	void setup( btPairCachingGhostObject* ghost, btConvexShape* shape )
	{
		m_ghost = ghost;
		m_shape = shape;

		m_useWalkDirection = true;
		m_wasOnGround = false;
		m_wasJumping = false;
		m_useGhostObjectSweepTest = true;
		m_touchingContact = false;

		m_halfHeight = 1.f;
		m_jumpVelocity = 10.f;
		m_fallVelocity = 55.f;
		m_maxSlopeHeight = 0.7071f;
		m_walkVelocity = 1.f;
		m_turnVelocity = 1.f;
		m_velocityTimeInterval;
		m_gravity = 9.8f;
		m_verticalVelocity = 0.f;
		m_verticalOffset = 0.f;
		m_stepHeight = 0.5f;
		m_currentStepOffset = 0.f;

		m_walkDirection.setZero();
		m_up.setValue( 0, 1, 0 );
	}
	void destroy()
	{
	}

	void updateAction( btCollisionWorld* collisionWorld, btScalar deltaTimeStep )
	{
		preStep( collisionWorld );
		playerStep( collisionWorld, deltaTimeStep );
	}

	void debugDraw( btIDebugDraw* debugDrawer )
	{
	}

	void setWalkDirection( const btVector3& walkDirection )
	{
		m_useWalkDirection = true;
		m_walkDirection = walkDirection;
		m_normalizedWalkDirection = walkDirection;
		m_normalizedWalkDirection.normalize();
		m_velocityTimeInterval = 0.f;
	}
	void setVelocityForTimeInterval( const btVector3& velocity, btScalar timeInterval )
	{
		m_useWalkDirection = false;
		m_walkDirection = velocity;
		m_normalizedWalkDirection = velocity;
		m_normalizedWalkDirection.normalize();
		m_velocityTimeInterval = timeInterval;
	}
	void reset()
	{
	}
	void warp( const btVector3& origin )
	{
		btTransform xform;
		xform.setIdentity();
		xform.setOrigin( origin );

		m_ghost->setWorldTransform( xform );
	}

	void preStep( btCollisionWorld* collisionWorld )
	{
		zp_int numPentrationLoops = 4;
		m_touchingContact = false;
		while( recoverFromPenetration( collisionWorld ) && numPentrationLoops-- > 0 )
		{
			m_touchingContact = true;
		}

		m_currentPosition = m_ghost->getWorldTransform().getOrigin();
		m_targetPosition = m_currentPosition;
	}

	void playerStep( btCollisionWorld* collisionWorld, btScalar dt )
	{
		if( !m_useWalkDirection && m_velocityTimeInterval < ZP_EPSILON )
		{
			return;
		}

		m_wasOnGround = onGround();

		m_verticalVelocity -= m_gravity * dt;
		if( m_verticalVelocity > 0.f && m_verticalVelocity > m_jumpVelocity )
		{
			m_verticalVelocity = m_jumpVelocity;
		}
		else if( m_verticalVelocity < 0.f && m_verticalVelocity < -m_fallVelocity )
		{
			m_verticalVelocity = -m_fallVelocity;
		}
		m_verticalOffset = m_verticalVelocity * dt;

		btTransform xform;
		xform = m_ghost->getWorldTransform();

		// 1 stepUp
		stepUp( collisionWorld, dt );

		// 2 walk forward
		btVector3 move = m_walkDirection;
		if( !m_useWalkDirection )
		{
			btScalar time = ( dt < m_velocityTimeInterval ) ? dt : m_velocityTimeInterval;
			m_velocityTimeInterval -= dt;

			move *= time;
		}
		stepForwardAndStrafe( collisionWorld, dt, move );


		// 3 stepDown
		stepDown( collisionWorld, dt );


		xform.setOrigin( m_currentPosition );
		m_ghost->setWorldTransform( xform );
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

		m_verticalVelocity = m_jumpVelocity;
		m_wasJumping = true;
	}

	bool onGround() const
	{
		return zp_approximate( m_verticalVelocity, 0.f );
	}

	zp_bool recoverFromPenetration( btCollisionWorld* collisionWorld )
	{
		zp_bool penetration = false;

		collisionWorld->getDispatcher()->dispatchAllCollisionPairs( m_ghost->getOverlappingPairCache(), collisionWorld->getDispatchInfo(), collisionWorld->getDispatcher() );

		m_currentPosition = m_ghost->getWorldTransform().getOrigin();

		btManifoldArray manifoldArray;

		btScalar maxPen = 0;
		btBroadphasePairArray& overlappingPairs = m_ghost->getOverlappingPairCache()->getOverlappingPairArray();
		for( zp_int i = 0; i < overlappingPairs.size(); ++i )
		{
			manifoldArray.resize( 0 );

			btBroadphasePair& collisionPair = overlappingPairs[ i ];

			//for trigger filtering
			if( !static_cast<btCollisionObject*>( collisionPair.m_pProxy0->m_clientObject )->hasContactResponse() ||
				!static_cast<btCollisionObject*>( collisionPair.m_pProxy1->m_clientObject )->hasContactResponse() )
			{
				continue;
			}

			if( collisionPair.m_algorithm )
			{
				collisionPair.m_algorithm->getAllContactManifolds( manifoldArray );
			}

			for( zp_int j = 0; j < manifoldArray.size(); ++j )
			{
				btPersistentManifold* manifold = manifoldArray[ j ];
				btScalar directionSign = manifold->getBody0() == m_ghost ? btScalar( -1.0f ) : btScalar( 1.0f );
				for( zp_int p = 0; p < manifold->getNumContacts(); ++p )
				{
					const btManifoldPoint& pt = manifold->getContactPoint( p );

					btScalar dist = pt.getDistance();

					if( dist < 0.f )
					{
						if( dist < maxPen )
						{
							maxPen = dist;
							m_touchingNormal = pt.m_normalWorldOnB * directionSign;//??
						}
						m_currentPosition += pt.m_normalWorldOnB * directionSign * dist * btScalar( 0.2f );
						penetration = true;
					}
					else
					{
						//printf("touching %f\n", dist);
					}
				}

				//manifold->clearManifold();
			}
		}

		if( penetration )
		{
			m_ghost->getWorldTransform().setOrigin( m_currentPosition );
		}
		
		return penetration;
	}

	void stepUp( btCollisionWorld* collisionWorld, btScalar dt )
	{
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();

		m_targetPosition = m_currentPosition + m_up * ( m_stepHeight + ( m_verticalOffset > 0.f ? m_verticalOffset : 0.f ) );

		start.setOrigin( m_currentPosition + m_up * m_shape->getMargin() );
		end.setOrigin( m_targetPosition );

		zpKinematicClosestNotMeConvexResultCallback cb( m_ghost, -m_up, 0.7071f );
		if( m_useGhostObjectSweepTest )
		{
			m_ghost->convexSweepTest( m_shape, start, end, cb );
		}
		else
		{
			collisionWorld->convexSweepTest( m_shape, start, end, cb );
		}

		if( cb.hasHit() )
		{
			if( cb.m_hitNormalWorld.dot( m_up ) > 0.f )
			{
				m_currentStepOffset = m_stepHeight * cb.m_closestHitFraction;
				m_currentPosition.setInterpolate3( m_currentPosition, m_targetPosition, cb.m_closestHitFraction );
			}

			m_verticalVelocity = 0.f;
			m_verticalOffset = 0.f;
		}
		else
		{
			m_currentStepOffset = m_stepHeight;
			m_currentPosition = m_targetPosition;
		}
	}

	void stepForwardAndStrafe( btCollisionWorld* collisionWorld, btScalar dt, const btVector3& move )
	{
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();

		m_targetPosition = m_currentPosition + move;

		zp_float fraction = 1.f;

		if( m_touchingContact )
		{
			if( m_normalizedWalkDirection.dot( m_touchingNormal ) > 0.f )
			{
				updateTargetPositionBasedOnCollision( m_touchingNormal );
			}
		}

		zp_int maxIterations = 10;
		while( fraction > 0.01f && maxIterations-- > 0 )
		{
			start.setOrigin( m_currentPosition );
			end.setOrigin( m_targetPosition );

			zpKinematicClosestNotMeConvexResultCallback cb( m_ghost, m_currentPosition - m_targetPosition, 0.f );
			if( m_useGhostObjectSweepTest )
			{
				m_ghost->convexSweepTest( m_shape, start, end, cb );
			}
			else
			{
				collisionWorld->convexSweepTest( m_shape, start, end, cb );
			}

			fraction -= cb.m_closestHitFraction;

			if( cb.hasHit() )
			{
				updateTargetPositionBasedOnCollision( cb.m_hitNormalWorld );
				btVector3 currentDir = m_targetPosition - m_currentPosition;
				btScalar len = currentDir.length();
				if( len > ZP_EPSILON )
				{
					currentDir /= len;
					if( currentDir.dot( m_normalizedWalkDirection ) < ZP_EPSILON )
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				m_currentPosition = m_targetPosition;
			}
		}
	}

	void stepDown( btCollisionWorld* collisionWorld, btScalar dt )
	{
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();

		zp_float downVelocity = ( m_verticalVelocity < 0.f ? -m_verticalVelocity : 0.f ) * dt;
		if( downVelocity > 0.f && ( m_wasOnGround || !m_wasJumping ) )
		{
			if( downVelocity > m_fallVelocity )
			{
				downVelocity = m_fallVelocity;
			}
		}

		btVector3 drop = m_up * ( m_currentStepOffset + downVelocity );
		m_targetPosition -= drop;

		start.setOrigin( m_currentPosition );
		end.setOrigin( m_targetPosition );

		zpKinematicClosestNotMeConvexResultCallback cb( m_ghost, m_up, m_maxSlopeHeight );
		if( m_useGhostObjectSweepTest )
		{
			m_ghost->convexSweepTest( m_shape, start, end, cb );
		}
		else
		{
			collisionWorld->convexSweepTest( m_shape, start, end, cb );
		}

		if( cb.hasHit() )
		{
			m_currentPosition.setInterpolate3( m_currentPosition, m_targetPosition, cb.m_closestHitFraction );
			m_verticalVelocity = 0.f;
			m_verticalOffset = 0.f;
			m_wasJumping = false;
		}
		else
		{
			m_currentPosition = m_targetPosition;
		}
	}

	void updateTargetPositionBasedOnCollision( const btVector3& hitNormal )
	{
		btVector3 movementDirection = m_targetPosition - m_currentPosition;
		btScalar movementLength = movementDirection.length();

		if( movementLength > ZP_EPSILON )
		{
			movementDirection /= movementLength;
			btVector3 reflectionDir = movementDirection - (btScalar(2.0) * movementDirection.dot(hitNormal)) * hitNormal;
			btVector3 parallelDir = hitNormal * reflectionDir.dot( hitNormal );
			btVector3 perpendicularDir = reflectionDir - parallelDir;

			m_targetPosition = m_currentPosition;
			btVector3 perpComop = perpendicularDir * movementLength;
			m_targetPosition += perpendicularDir;
		}
	}

private:
	zp_bool m_useWalkDirection;
	zp_bool m_wasOnGround;
	zp_bool m_wasJumping;
	zp_bool m_useGhostObjectSweepTest;
	zp_bool m_touchingContact;

	zp_float m_halfHeight;
	zp_float m_jumpVelocity;
	zp_float m_fallVelocity;
	zp_float m_maxSlopeHeight;
	zp_float m_walkVelocity;
	zp_float m_turnVelocity;
	zp_float m_velocityTimeInterval;
	zp_float m_gravity;
	zp_float m_verticalVelocity;
	zp_float m_verticalOffset;
	zp_float m_stepHeight;
	zp_float m_currentStepOffset;

	btVector3 m_walkDirection;
	btVector3 m_normalizedWalkDirection;
	btVector3 m_currentPosition;
	btVector3 m_targetPosition;
	btVector3 m_up;
	btVector3 m_touchingNormal;

	btPairCachingGhostObject* m_ghost;
	btConvexShape* m_shape;
};

void zpKinematicBody::create( const zpBison::Value& v )
{
	m_mass = v[ "Mass" ].asFloat();

	m_group = zpCollisionMask::getInstance()->getCollisionMask( v[ "Group" ].asCString() );
	m_mask = zpCollisionMask::getInstance()->getCollisionMask( v[ "Mask" ].asCString() );
	m_collider = zpColliderCache::getInstance()->getCollider( v[ "Collider" ] );
	zp_float stepHeight = v[ "StepHeight" ].asFloat();

}
void zpKinematicBody::initialize( const zpMatrix4f& transform )
{
	btCollisionShape* colliderShape = (btCollisionShape*)m_collider->getCollider();

	btQuaternion rot( btQuaternion::getIdentity() );
	btVector3 pos;
	transform.getRow( 3 ).store4( pos.m_floats );

	btTransform startTransform( rot, pos );

	btPairCachingGhostObject* ghost = new btPairCachingGhostObject;
	ghost->setCollisionShape( colliderShape );
	ghost->setCollisionFlags( btCollisionObject::CF_CHARACTER_OBJECT );
	ghost->setWorldTransform( startTransform );

	zpKinematicCharacterController* controller = new zpKinematicCharacterController;
	controller->setup( ghost, static_cast< btConvexShape* >( colliderShape ) );

	m_controller = controller;
	m_ghost = ghost;
}
void zpKinematicBody::destroy()
{
	zpColliderCache::getInstance()->removeCollider( (zpCollider*)m_collider );
	m_collider = ZP_NULL;

	zpKinematicCharacterController* controller = (zpKinematicCharacterController*)m_controller;
	btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_ghost;

	controller->destroy();

	ZP_SAFE_DELETE( controller );
	ZP_SAFE_DELETE( ghost );

	m_controller = ZP_NULL;
	m_ghost = ZP_NULL;
}

zp_bool zpKinematicBody::getMatrix( zpMatrix4f& transform )
{
	btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_ghost;

	const btTransform& t = ghost->getWorldTransform();

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
zp_handle zpKinematicBody::getCollisionGhost() const
{
	return m_ghost;
}
