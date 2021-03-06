#include "zpPhysics.h"

#include "zpPhysicsLib.inl"
#include "src/btBulletCollisionCommon.h"
#include "src/btBulletDynamicsCommon.h"
#include "src/BulletCollision/CollisionDispatch/btGhostObject.h"

zpPhantom::zpPhantom()
    : m_phantom( ZP_NULL )
    , m_collider( ZP_NULL )
    , m_group( 0 )
    , m_mask( 0 )
    , m_collisionCallback( ZP_NULL )
{}
zpPhantom::~zpPhantom()
{}

void zpPhantom::create( zpMatrix4fParamF transform, zp_short group, zp_short mask, zpCollider* collider, zpPhysicsEngine* engine )
{
    m_group = group;
    m_mask = mask;
    m_collider = collider;
    btCollisionShape* shape = (btCollisionShape*)m_collider->getCollider();

    btPairCachingGhostObject* ghost = new btPairCachingGhostObject();
    ghost->setUserPointer( this );
    ghost->setCollisionShape( shape );
    ghost->setCollisionFlags( ghost->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );

    ZP_ALIGN16 zp_float matrix[16];
    zpMath::MatrixStoreOpenGL( transform, matrix );

    btTransform trans;
    trans.setFromOpenGLMatrix( matrix );
    ghost->setWorldTransform( trans );

    m_phantom = ghost;
}
void zpPhantom::destroy( zpPhysicsEngine* engine )
{
    m_collider = ZP_NULL;

    btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_phantom;
    ZP_SAFE_DELETE( ghost );

    m_phantom = ZP_NULL;
}

void zpPhantom::setMatrix( zpMatrix4fParamF transform )
{
    btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_phantom;

    ZP_ALIGN16 zp_float matrix[16];
    zpMath::MatrixStoreOpenGL( transform, matrix );

    btTransform t;
    t.setFromOpenGLMatrix( matrix );

    ghost->setWorldTransform( t );
}
zpMatrix4f zpPhantom::getMatrix() const
{
    btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_phantom;

    const btTransform& t = ghost->getWorldTransform();

    ZP_ALIGN16 zp_float matrix[ 16 ];
    t.getOpenGLMatrix( matrix );

    zpMatrix4f transform = zpMath::MatrixLoadOpenGL( matrix );
    return transform;
}

zp_short zpPhantom::getGroup() const
{
    return m_group;
}
zp_short zpPhantom::getMask() const
{
    return m_mask;
}

zp_handle zpPhantom::getPhantom() const
{
    return m_phantom;
}
zpCollider* zpPhantom::getCollider() const
{
    return m_collider;
}

zp_bool zpPhantom::rayTest( zpVector4fParamF fromWorld,zpVector4fParamF toWorld, zpCollisionHitResult& hit ) const
{
    btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_phantom;

    btVector3 from, to;
    btCollisionWorld::ClosestRayResultCallback cb( from, to );

    ghost->rayTest( from, to, cb );

    zp_bool isHit = cb.hasHit();
    if( isHit )
    {
        hit.position = zpMath::Vector4Load4( cb.m_hitPointWorld.m_floats );
        hit.normal = zpMath::Vector4Load4( cb.m_hitNormalWorld.m_floats );
        hit.hitObject = cb.m_collisionObject ? cb.m_collisionObject->getUserPointer() : ZP_NULL;
    }

    return isHit;
}

void zpPhantom::processCollisions( zp_handle dymaicsWorld, zp_float timeStep )
{
    btDynamicsWorld* world = static_cast< btDynamicsWorld* >( dymaicsWorld );
    btOverlappingPairCache* cache = world->getPairCache();

    btManifoldArray manifolds;
    btPairCachingGhostObject* ghost = static_cast< btPairCachingGhostObject* >( m_phantom );

    zpFixedArrayList< zpPhantomCollisionHitInfo, ZP_PHANTON_MAX_TRACKED_OBJECTS > hits;

    zpFixedArrayList< zp_handle, ZP_PHANTON_MAX_TRACKED_OBJECTS > currentlyTracked;
    zp_handle* bh = m_trackedObjects.begin();
    zp_handle* eh = m_trackedObjects.end();
    for( ; bh != eh; ++bh )
    {
        currentlyTracked.pushBack( bh );
    }

    // determine collisions
    const btBroadphasePairArray& pairs = ghost->getOverlappingPairCache()->getOverlappingPairArray();
    for( zp_int i = 0, imax = pairs.size(); i < imax; ++i )
    {
        const btBroadphasePair& pair = pairs[ i ];
        const btBroadphasePair* p = cache->findPair( pair.m_pProxy0, pair.m_pProxy1 );
        if( p == ZP_NULL ) continue;

        manifolds.resize( 0 );
        p->m_algorithm->getAllContactManifolds( manifolds );

        for( zp_int j = 0, jmax = manifolds.size(); j < jmax; ++j )
        {
            btPersistentManifold* manifold = manifolds[ j ];
            btCollisionObject* b0 = static_cast< btCollisionObject* >( manifold->getBody0() );
            btCollisionObject* b1 = static_cast< btCollisionObject* >( manifold->getBody1() );

            btCollisionObject* otherObject = m_phantom == b0 ? b1 : b0;

            for( zp_int m = 0, mmax = manifold->getNumContacts(); m < mmax; ++m )
            {
                btManifoldPoint& point = manifold->getContactPoint( m );
                
                zpPhantomCollisionHitInfo& hit = hits.pushBackEmpty();
                hit.otherObject = otherObject->getUserPointer();
                hit.worldPositionOnA = zpMath::Vector4Load4( point.m_positionWorldOnA.m_floats );
                hit.worldPositionOnB = zpMath::Vector4Load4( point.m_positionWorldOnB.m_floats );
                hit.worldNormalOnB = zpMath::Vector4Load4( point.m_normalWorldOnB );
            }
        }
    }

    // track new and persisting collisions
    zpPhantomCollisionHitInfo* b = hits.begin();
    zpPhantomCollisionHitInfo* e = hits.end();
    for( ; b != e; ++b )
    {
        currentlyTracked.eraseAll( b->otherObject );

        if( m_trackedObjects.indexOf( b->otherObject ) == zpFixedArrayList< zp_handle, ZP_PHANTON_MAX_TRACKED_OBJECTS >::npos )
        {
            onCollisionEnter( *b );

            m_trackedObjects.pushBack( b->otherObject );
        }
        else
        {
            onCollisionStay( *b );
        }
    }

    // determine the collisions that have left
    zp_handle* cb = currentlyTracked.begin();
    zp_handle* ce = currentlyTracked.end();
    for( ; cb != ce; ++cb )
    {
        onCollisionLeave( *cb );

        m_trackedObjects.eraseAll( *cb );
    }
}

void zpPhantom::setCollisionCallback( zpPhantomCollisionCallback* callback )
{
    m_collisionCallback = callback;
}
zpPhantomCollisionCallback* zpPhantom::getCollisionCallback() const
{
    return m_collisionCallback;
}

void zpPhantom::onCollisionEnter( const zpPhantomCollisionHitInfo& hit )
{
    if( m_collisionCallback ) m_collisionCallback->onCollisionEnter( hit );
}
void zpPhantom::onCollisionStay( const zpPhantomCollisionHitInfo& hit )
{
    if( m_collisionCallback ) m_collisionCallback->onCollisionStay( hit );
}
void zpPhantom::onCollisionLeave( zp_handle otherObject )
{
    if( m_collisionCallback ) m_collisionCallback->onCollisionLeave( otherObject );
}
