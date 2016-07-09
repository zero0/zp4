#include "zpPhysics.h"

#include "zpPhysicsLib.inl"
#include "src/btBulletCollisionCommon.h"
#include "src/btBulletDynamicsCommon.h"
#include "src/BulletDynamics/Character/btCharacterControllerInterface.h"
#include "src/BulletDynamics/Character/btKinematicCharacterController.h"
#include "src/BulletCollision/CollisionDispatch/btGhostObject.h"

ZP_FORCE_INLINE zpVector4f _btVector3ToVector4Position( const btVector3& s )
{
    zpVector4f a = zpMath::Vector4( s.x(), s.y(), s.z(), 1.f );
    return a;
}

ZP_FORCE_INLINE zpVector4f _btVector3ToVector4Normal( const btVector3& s )
{
    zpVector4f a = zpMath::Vector4( s.x(), s.y(), s.z(), 0.f );
    return a;
}

ZP_FORCE_INLINE btVector3 ZP_VECTORCALL _zpVector4ToVector3( zpVector4fParamF s )
{
    ZP_ALIGN16 btVector3FloatData data;
    zpMath::Vector4Store4( s, data.m_floats );

    btVector3 a;
    a.deSerializeFloat( data );
    return a;
}

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
    zpKinematicClosestNotMeConvexResultCallback( btCollisionObject* me, const zpVector4f& up, zp_float minSlopeDot )
        : btCollisionWorld::ClosestConvexResultCallback( btVector3( 0.0f, 0.0f, 0.0f ), btVector3( 0.0f, 0.0f, 0.0f ) )
        , m_me( me )
        , m_up( up )
        , m_minSlopeDot( zpMath::Scalar( minSlopeDot ) )
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

        //btScalar dotUp = m_up.dot( hitNormalWorld );
        //if( dotUp < m_minSlopeDot ) return btScalar( 1.0f );

        zpScalar dotUp;
        zpVector4f hitNormal;
        hitNormal = _btVector3ToVector4Normal( hitNormalWorld );

        dotUp = zpMath::Vector4Dot3( m_up, hitNormal );
        if( zpMath::ScalarCmp( dotUp, m_minSlopeDot ) < 0 ) return btScalar( 1.0f );

        btScalar ret = ClosestConvexResultCallback::addSingleResult( convexResult, normalInWorldSpace );
        return ret;
    }

protected:
    btCollisionObject* m_me;
    const zpVector4f m_up;
    zpScalar m_minSlopeDot;
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

        m_mass = 1.f;
        m_jumpVelocity = 10.f;
        m_fallVelocity = 55.f;
        m_maxSlopeHeight = 0.7071f;
        m_walkVelocity = .5f;
        m_turnVelocity = 1.f;
        m_velocityTimeInterval = 0.f;
        m_gravity = 9.8f;
        m_verticalVelocity = 0.f;
        m_verticalOffset = 0.f;
        m_stepHeight = 0.5f;
        m_currentStepOffset = 0.f;

        m_dampenAcceleration = 0.1f;
        m_velocity = zpMath::Vector4( 0, 0, 0, 0 );
        m_acceleration = zpMath::Vector4( 0, 0, 0, 0 );

        m_walkDirection = zpMath::Vector4( 0, 0, 0, 0 );
        m_up = zpMath::Vector4( 0, 1, 0, 0 );
    }
    void destroy()
    {
        m_ghost = ZP_NULL;
        m_shape = ZP_NULL;
    }

    void updateAction( btCollisionWorld* collisionWorld, btScalar deltaTimeStep )
    {
        preStep( collisionWorld );
        playerStep( collisionWorld, deltaTimeStep );
    }

    void debugDraw( btIDebugDraw* debugDrawer )
    {
        //if( !m_walkDirection.isZero() )
        //{
        //    const btVector3& pos = m_ghost->getWorldTransform().getOrigin();
        //    debugDrawer->drawLine( pos, pos + m_walkDirection, btVector3( 0, 1, 0 ) );
        //}
    }

    void setAcceleration( zpVector4fParamF acceleration )
    {
        m_acceleration = acceleration;
    }

    void setWalkDirection( const btVector3& walkDirection )
    {
        zpVector4f dir = _btVector3ToVector4Normal( walkDirection );
        setWalkDirection( dir );
    }
    void setWalkDirection( zpVector4fParamF walkDirection )
    {
        m_useWalkDirection = true;
        m_walkVelocity = zpMath::AsFloat( zpMath::Vector4Length3( walkDirection ) );
        if( zp_approximate( m_walkVelocity, 0.f ) )
        {
            m_normalizedWalkDirection = zpMath::Vector4( 0, 0, 0, 0 );
        }
        else
        {
            m_normalizedWalkDirection = zpMath::Vector4Normalize3( walkDirection );
        }
        m_walkDirection = m_normalizedWalkDirection;
        m_velocityTimeInterval = 0.f;
    }
    void setVelocityForTimeInterval( const btVector3& velocity, btScalar timeInterval )
    {
        zpVector4f v = _btVector3ToVector4Normal( velocity );
        setVelocityForTimeInterval( v, timeInterval );
    }
    void setVelocityForTimeInterval( zpVector4fParamF velocity, zp_float timeInterval )
    {
        m_useWalkDirection = false;
        m_walkVelocity = zpMath::AsFloat( zpMath::Vector4Length3( velocity ) );
        if( zp_approximate( m_walkVelocity, 0.f ) )
        {
            m_normalizedWalkDirection = zpMath::Vector4( 0, 0, 0, 0 );
        }
        else
        {
            m_normalizedWalkDirection = zpMath::Vector4Normalize3( m_walkDirection );
        }
        m_walkDirection = m_normalizedWalkDirection;
        m_velocity = velocity;
        m_velocityTimeInterval = timeInterval;
    }
    void reset()
    {
        m_acceleration =  zpMath::Vector4( 0, 0, 0, 0 );
        m_velocity =      zpMath::Vector4( 0, 0, 0, 0 );
        m_walkDirection = zpMath::Vector4( 0, 0, 0, 0 );
        m_verticalVelocity = 0.f;
        m_verticalOffset = 0.f;
    }
    void warp( const btVector3& origin )
    {
        reset();

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
            break;
        }

        const btVector3& currentPos = m_ghost->getWorldTransform().getOrigin();

        m_currentPosition = _btVector3ToVector4Position( currentPos );
        m_targetPosition = m_currentPosition;
    }

    void playerStep( btCollisionWorld* collisionWorld, btScalar dt )
    {
        //if( !m_useWalkDirection && m_velocityTimeInterval < ZP_EPSILON )
        //{
        //    return;
        //}

        m_wasOnGround = onGround();

        m_verticalVelocity -= m_gravity * m_mass * dt;
        if( m_verticalVelocity > 0.f && m_verticalVelocity > m_jumpVelocity )
        {
            m_verticalVelocity = m_jumpVelocity;
        }
        else if( m_verticalVelocity < 0.f && m_verticalVelocity < -m_fallVelocity )
        {
            m_verticalVelocity = -m_fallVelocity;
        }
        m_verticalOffset = m_verticalVelocity * dt;

        // 1 stepUp
        stepUp( collisionWorld, dt );

        // 2 walk forward and strafe
        stepForwardAndStrafe( collisionWorld, dt );

        // 3 stepDown
        stepDown( collisionWorld, dt );

        btVector3 pos;
        pos = _zpVector4ToVector3( m_currentPosition );

        btTransform xform;
        xform = m_ghost->getWorldTransform();
        xform.setOrigin( pos );
        m_ghost->setWorldTransform( xform );
    }

    bool canJump() const
    {
        zp_bool grounded = onGround();
        return grounded;
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
        zp_bool grounded = zp_approximate( m_verticalVelocity, 0.f ) && zp_approximate( m_verticalOffset, 0.f );
        return grounded;
    }

    zp_bool recoverFromPenetration( btCollisionWorld* collisionWorld )
    {
        zp_bool penetration = false;

        collisionWorld->getDispatcher()->dispatchAllCollisionPairs( m_ghost->getOverlappingPairCache(), collisionWorld->getDispatchInfo(), collisionWorld->getDispatcher() );

        const btVector3& pos = m_ghost->getWorldTransform().getOrigin();
        m_currentPosition = _btVector3ToVector4Position( pos );

        btScalar maxPen = 0;
        btBroadphasePairArray& overlappingPairs = m_ghost->getOverlappingPairCache()->getOverlappingPairArray();
        for( zp_int i = 0; i < overlappingPairs.size(); ++i )
        {
            m_manifoldArray.resize( 0 );

            btBroadphasePair& collisionPair = overlappingPairs[ i ];

            //for trigger filtering
            if( !static_cast<btCollisionObject*>( collisionPair.m_pProxy0->m_clientObject )->hasContactResponse() ||
                !static_cast<btCollisionObject*>( collisionPair.m_pProxy1->m_clientObject )->hasContactResponse() )
            {
                continue;
            }

            if( collisionPair.m_algorithm )
            {
                collisionPair.m_algorithm->getAllContactManifolds( m_manifoldArray );
            }

            for( zp_int j = 0; j < m_manifoldArray.size(); ++j )
            {
                btPersistentManifold* manifold = m_manifoldArray[ j ];
                zpScalar directionSign = zpMath::Scalar( manifold->getBody0() == m_ghost ? -1.f : 1.f );

                for( zp_int p = 0; p < manifold->getNumContacts(); ++p )
                {
                    const btManifoldPoint& pt = manifold->getContactPoint( p );

                    btScalar dist = pt.getDistance();

                    if( dist < 0.f )
                    {
                        if( dist < maxPen )
                        {
                            maxPen = dist;

                            zpVector4f touchNorm;
                            touchNorm = _btVector3ToVector4Normal( pt.m_normalWorldOnB );

                            m_touchingNormal = zpMath::Vector4Mul( touchNorm, directionSign );
                            m_touchingNormal = zpMath::Vector4Normalize3( m_touchingNormal );
                            //m_touchingNormal = pt.m_normalWorldOnB * directionSign;//??
                            //m_touchingNormal.normalize();
                        }

                        //m_currentPosition += pt.m_normalWorldOnB * directionSign * dist * btScalar( 0.2f );
                        zpVector4f worldOnB;
                        zpScalar d;

                        worldOnB = _btVector3ToVector4Normal( pt.m_normalWorldOnB );
                        worldOnB = zpMath::Vector4Normalize3( worldOnB );

                        d = zpMath::ScalarMul( directionSign, zpMath::Scalar( dist ) );
                        d = zpMath::ScalarMul( d, zpMath::Scalar( 0.2f ) );
                        m_currentPosition = zpMath::Vector4Madd( m_currentPosition, d, worldOnB );

                        penetration = true;
                    }
                    else
                    {
                        //printf("touching %f\n", dist);
                    }
                }

                manifold->clearManifold();
            }
        }

        if( penetration )
        {
            btVector3 pos;
            pos = _zpVector4ToVector3( m_currentPosition );

            m_ghost->getWorldTransform().setOrigin( pos );
        }
        
        return penetration;
    }

    void stepUp( btCollisionWorld* collisionWorld, btScalar dt )
    {
        btTransform start, end;
        start.setIdentity();
        end.setIdentity();

        //m_targetPosition = m_currentPosition + m_up * ( m_stepHeight + ( m_verticalOffset > 0.f ? m_verticalOffset : 0.f ) );
        zpScalar h;
        h = zpMath::ScalarAdd( zpMath::Scalar( m_stepHeight ), zpMath::Scalar( m_verticalOffset > 0.f ? m_verticalOffset : 0.f ) );
        m_targetPosition = zpMath::Vector4Madd( m_currentPosition, m_up, h );

        btVector3 s, e;

        zpVector4f pos;
        pos = zpMath::Vector4Madd( m_currentPosition, m_up, zpMath::Scalar( m_shape->getMargin() ) );

        s = _zpVector4ToVector3( pos );
        e = _zpVector4ToVector3( m_targetPosition );

        start.setOrigin( s );
        end.setOrigin( e );
        //start.setOrigin( m_currentPosition + m_up * m_shape->getMargin() );
        //end.setOrigin( m_targetPosition );

        zpVector4f negUp;
        negUp = zpMath::Vector4Neg( m_up );

        zpKinematicClosestNotMeConvexResultCallback cb( m_ghost, negUp, 0.7071f );
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
            zpScalar d;
            zpVector4f hitNormalWorld;
            hitNormalWorld = _btVector3ToVector4Normal( cb.m_hitNormalWorld );
            d = zpMath::Vector4Dot3( m_up, hitNormalWorld );

            //if( cb.m_hitNormalWorld.dot( m_up ) > 0.f )
            if( zpMath::ScalarCmp0( d ) > 0 )
            {
                m_currentStepOffset = m_stepHeight * cb.m_closestHitFraction;
                //m_currentPosition.setInterpolate3( m_currentPosition, m_targetPosition, cb.m_closestHitFraction );
                m_currentPosition = zpMath::Vector4Lerp( m_currentPosition, m_targetPosition, zpMath::Scalar( cb.m_closestHitFraction ) );
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

    void stepForwardAndStrafe( btCollisionWorld* collisionWorld, btScalar dt )
    {
        zpScalar sdt;

        zpVector4f a;
        zpVector4f move;

        if( m_useWalkDirection )
        {
            sdt = zpMath::Scalar( dt );

            //btVector3 v = m_acceleration * dt + m_velocity;
            m_velocity = zpMath::Vector4ScaleAdd( m_acceleration, sdt, m_velocity );

            //btVector3 move = v * dt + btScalar( 0.5f ) * m_acceleration * dt * dt;
            zpScalar sdt2;
            sdt2 = zpMath::ScalarMul( sdt, sdt );
            a = zpMath::Vector4Scale( m_acceleration, sdt2 );
            a = zpMath::Vector4Scale( a, zpMath::Scalar( 0.5f ) );

            //btVector3 move = m_walkDirection * m_walkVelocity;
            move = zpMath::Vector4Scale( m_velocity, sdt );
            move = zpMath::Vector4Sub( move, a );
        }
        else
        {
            zp_float time = ( dt < m_velocityTimeInterval ) ? dt : m_velocityTimeInterval;
            m_velocityTimeInterval -= dt;

            if( m_velocityTimeInterval < 0.f )
            {
                return;
            }
            
            //move *= time;
            move = zpMath::Vector4Scale( m_velocity, zpMath::Scalar( time ) );
            //sdt = zpMath::Scalar( time );
        }


        zpScalar d = zpMath::Vector4Dot3( move, move );
        if( zpMath::ScalarCmp0( d ) == 0 )
        {
            return;
        }

        btTransform start, end;
        start.setIdentity();
        end.setIdentity();

        //m_targetPosition = m_currentPosition + move;
        m_targetPosition = zpMath::Vector4Add( m_currentPosition, move );

        zp_float fraction = 1.f;

        if( m_touchingContact )
        {
            //btScalar d = m_normalizedWalkDirection.dot( m_touchingNormal );
            zpScalar d;
            d = zpMath::Vector4Dot3( m_normalizedWalkDirection, m_touchingNormal );

            //if( d > 0.1f )
            if( zpMath::ScalarCmp( d, zpMath::Scalar( 0.1f ) ) > 0 )
            {
                updateTargetPositionBasedOnCollision( m_touchingNormal );
            }
        }

        zp_int maxIterations = 10;
        while( fraction > 0.01f && maxIterations-- > 0 )
        {
            btVector3 s, e;
            s = _zpVector4ToVector3( m_currentPosition );
            e = _zpVector4ToVector3( m_targetPosition );

            start.setOrigin( s );
            end.setOrigin( e );

            zpVector4f neg;
            neg = zpMath::Vector4Neg( move );
            neg = zpMath::Vector4Normalize3( neg );

            //btVector3 negDirection = m_currentPosition - m_targetPosition;
            //negDirection.normalize();

            zpKinematicClosestNotMeConvexResultCallback cb( m_ghost, neg, 0.f );
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
                zpVector4f hitNorm;
                hitNorm = _btVector3ToVector4Normal( cb.m_hitNormalWorld );

                updateTargetPositionBasedOnCollision( hitNorm );

                zpScalar len;
                zpVector4f curDir;
                curDir = zpMath::Vector4Sub( m_targetPosition, m_currentPosition );
                len = zpMath::Vector4Length3( curDir );

                //btVector3 currentDir = m_targetPosition - m_currentPosition;
                //btScalar len = currentDir.length();
                
                //if( len > ZP_EPSILON )
                if( zpMath::ScalarCmp0( len ) > 0 )
                {
                    //currentDir /= len;
                    curDir = zpMath::Vector4Div( curDir, len );

                    //btScalar d = currentDir.dot( m_normalizedWalkDirection );
                    //if( d < ZP_EPSILON )
                    len = zpMath::Vector4Dot3( curDir, m_normalizedWalkDirection );
                    if( zpMath::ScalarCmp0( len ) < 0 )
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

        //btVector3 drop = m_up * ( m_currentStepOffset + downVelocity );
        //m_targetPosition -= drop;
        zpScalar v;
        zpVector4f drop;
        v = zpMath::ScalarAdd( zpMath::Scalar( m_currentStepOffset ), zpMath::Scalar( downVelocity ) );
        drop = zpMath::Vector4Scale( m_up, v );
        m_targetPosition = zpMath::Vector4Sub( m_targetPosition, drop );

        btVector3 s, e;
        s = _zpVector4ToVector3( m_currentPosition );
        e = _zpVector4ToVector3( m_targetPosition );

        start.setOrigin( s );
        end.setOrigin( e );

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
            //m_currentPosition.setInterpolate3( m_currentPosition, m_targetPosition, cb.m_closestHitFraction );
            m_currentPosition = zpMath::Vector4Lerp( m_currentPosition, m_targetPosition, zpMath::Scalar( cb.m_closestHitFraction ) );
            m_verticalVelocity = 0.f;
            m_verticalOffset = 0.f;
            m_wasJumping = false;
        }
        else
        {
            m_currentPosition = m_targetPosition;
        }
    }

    void updateTargetPositionBasedOnCollision( const zpVector4f& hitNormal )
    {
        zpScalar len;
        zpVector4f moveDir;

        moveDir = zpMath::Vector4Sub( m_targetPosition, m_currentPosition );
        len = zpMath::Vector4Length3( moveDir );

        if( zpMath::ScalarCmp0( len ) > 0 )
        {
            moveDir = zpMath::Vector4Div( moveDir, len );

            zpVector4f reflDir;
            reflDir = zpMath::Vector4Reflect( moveDir, hitNormal );
            reflDir = zpMath::Vector4Normalize3( reflDir );

            zpScalar d;
            d = zpMath::Vector4Dot3( reflDir, hitNormal );

            zpVector4f paralDir;
            paralDir = zpMath::Vector4Scale( hitNormal, d );

            zpVector4f perpDir;
            perpDir = zpMath::Vector4Sub( reflDir, paralDir );

            m_targetPosition = m_currentPosition;

            m_targetPosition = zpMath::Vector4Madd( m_targetPosition, perpDir, len );
        }

#if 0
        btVector3 movementDirection = m_targetPosition - m_currentPosition;
        btScalar movementLength = movementDirection.length();

        if( movementLength > ZP_EPSILON )
        {
            movementDirection /= movementLength;

            btVector3 reflectionDir = movementDirection - (btScalar(2.0f) * movementDirection.dot(hitNormal)) * hitNormal;
            reflectionDir.normalize();

            btVector3 parallelDir = hitNormal * reflectionDir.dot( hitNormal );
            btVector3 perpendicularDir = reflectionDir - parallelDir;

            m_targetPosition = m_currentPosition;

            btVector3 perpComop = perpendicularDir * movementLength;
            m_targetPosition += perpendicularDir;
        }
#endif
    }

private:
    zp_bool m_useWalkDirection;
    zp_bool m_wasOnGround;
    zp_bool m_wasJumping;
    zp_bool m_useGhostObjectSweepTest;
    zp_bool m_touchingContact;

    zp_float m_mass;
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

    zp_float m_dampenAcceleration;
    zpVector4f m_velocity;
    zpVector4f m_acceleration;

    zpVector4f m_walkDirection;
    zpVector4f m_normalizedWalkDirection;
    zpVector4f m_currentPosition;
    zpVector4f m_targetPosition;
    zpVector4f m_up;
    zpVector4f m_touchingNormal;

    btManifoldArray m_manifoldArray;

    btPairCachingGhostObject* m_ghost;
    btConvexShape* m_shape;
};

void zpKinematicBody::create( zpPhysicsEngine* engine, const zpBison::Value& v )
{
    m_mass = v[ "Mass" ].asFloat();

    m_group = engine->getCollisionMask()->getCollisionMask( v[ "Group" ].asCString() );
    m_mask =  engine->getCollisionMask()->getCollisionMask( v[ "Mask" ].asCString() );
    m_collider = engine->getColliderCache()->getCollider( v[ "Collider" ] );
    zp_float stepHeight = v[ "StepHeight" ].asFloat();

}
void zpKinematicBody::initialize( const zpMatrix4f& transform )
{
    btCollisionShape* colliderShape = (btCollisionShape*)m_collider->getCollider();

    btQuaternion rot( btQuaternion::getIdentity() );
    btVector3 pos;
    pos = _zpVector4ToVector3( transform.r[ 3 ] );

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
void zpKinematicBody::destroy( zpPhysicsEngine* engine )
{
    engine->getColliderCache()->removeCollider( m_collider );
    m_collider = ZP_NULL;

    zpKinematicCharacterController* controller = (zpKinematicCharacterController*)m_controller;
    btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_ghost;

    controller->destroy();

    ZP_SAFE_DELETE( controller );
    ZP_SAFE_DELETE( ghost );

    m_controller = ZP_NULL;
    m_ghost = ZP_NULL;
}

void zpKinematicBody::getMatrix( zpMatrix4f& transform ) const
{
    btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_ghost;

    const btTransform& t = ghost->getWorldTransform();

    zp_float matrix[ 16 ];
    t.getOpenGLMatrix( matrix );

    transform = zpMath::MatrixLoadOpenGL( matrix );
}
void zpKinematicBody::getPositionRotation( zpVector4f& position, zpQuaternion4f& rotation ) const
{
    btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_ghost;

    const btTransform& t = ghost->getWorldTransform();

    const btVector3& o = t.getOrigin();
    const btQuaternion& q = t.getRotation();

    position = zpMath::Vector4( o.x(), o.y(), o.z(), 1.f );
    rotation = zpMath::Quaternion( q.x(), q.y(), q.z(), q.w() );
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

void zpKinematicBody::warp( zpVector4fParamF position )
{
    btVector3 pos;
    pos = _zpVector4ToVector3( position );

    zpKinematicCharacterController* controller = (zpKinematicCharacterController*)m_controller;
    controller->warp( pos );
}

void zpKinematicBody::setWalkDirection( zpVector4fParamF direction )
{
    zpKinematicCharacterController* controller = (zpKinematicCharacterController*)m_controller;
    controller->setWalkDirection( direction );
}
void zpKinematicBody::setAcceleration( zpVector4fParamF acceleration )
{
    zpKinematicCharacterController* controller = (zpKinematicCharacterController*)m_controller;
    controller->setAcceleration( acceleration );
}

zp_handle zpKinematicBody::getKinematicController() const
{
    return m_controller;
}
zp_handle zpKinematicBody::getCollisionGhost() const
{
    return m_ghost;
}
