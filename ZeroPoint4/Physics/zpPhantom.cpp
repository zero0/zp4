#include "zpPhysics.h"

#include "zpPhysicsLib.inl"
#include "src/btBulletCollisionCommon.h"
#include "src/btBulletDynamicsCommon.h"
#include "src/BulletCollision/CollisionDispatch/btGhostObject.h"

zpPhantom::zpPhantom()
{
	
}
zpPhantom::~zpPhantom()
{
	
}

void zpPhantom::create( zpPhysicsEngine* engine, const zpMatrix4f& transform, const zpBison::Value& v )
{
	m_group = engine->getCollisionMask()->getCollisionMask( v[ "Group" ].asCString() );
	m_mask =  engine->getCollisionMask()->getCollisionMask( v[ "Mask" ].asCString() );
	m_collider = engine->getColliderCache()->getCollider( v[ "Collider" ] );
	btCollisionShape* shape = (btCollisionShape*)m_collider->getCollider();

	btPairCachingGhostObject* ghost = new btPairCachingGhostObject();
	ghost->setUserPointer( this );
	ghost->setCollisionShape( shape );
	ghost->setCollisionFlags( ghost->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );

	zp_float matrix[16];
	zpMath::MatrixStoreOpenGL( transform, matrix );

	btTransform trans;
	trans.setFromOpenGLMatrix( matrix );
	ghost->setWorldTransform( trans );

	m_phantom = ghost;
}
void zpPhantom::destroy( zpPhysicsEngine* engine )
{
	engine->getColliderCache()->removeCollider( m_collider );
	m_collider = ZP_NULL;

	btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_phantom;
	ZP_SAFE_DELETE( ghost );

	m_phantom = ZP_NULL;
}

void zpPhantom::setMatrix( const zpMatrix4f& transform )
{
	btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_phantom;

	zp_float matrix[16];
	zpMath::MatrixStoreOpenGL( transform, matrix );

	btTransform t;
	t.setFromOpenGLMatrix( matrix );

	ghost->setWorldTransform( t );
}
zp_bool zpPhantom::getMatrix( zpMatrix4f& transform ) const
{
	btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_phantom;

	const btTransform& t = ghost->getWorldTransform();

	zp_float matrix[ 16 ];
	t.getOpenGLMatrix( matrix );

	transform = zpMath::MatrixLoadOpenGL( matrix );
	return true;
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

zp_bool zpPhantom::rayTest( const zpVector4f& fromWorld, const zpVector4f& toWorld, zpCollisionHitResult& hit ) const
{
	btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_phantom;

	btVector3 from, to;
	btCollisionWorld::ClosestRayResultCallback cb( from, to );

	ghost->rayTest( from, to, cb );

	if( cb.hasHit() )
	{
		hit.position = zpMath::Vector4Load4( cb.m_hitPointWorld.m_floats );
		hit.normal = zpMath::Vector4Load4( cb.m_hitNormalWorld.m_floats );
		hit.hitObject = cb.m_collisionObject ? cb.m_collisionObject->getUserPointer() : ZP_NULL;
		return true;
	}

	return false;
}

void zpPhantom::processCollisions( zp_handle dymaicsWorld ) const
{
	btDynamicsWorld* world = static_cast< btDynamicsWorld* >( dymaicsWorld );
	btOverlappingPairCache* cache = world->getPairCache();

	btManifoldArray manifolds;
	btPairCachingGhostObject* ghost = static_cast< btPairCachingGhostObject* >( m_phantom );

	const btBroadphasePairArray& pairs = ghost->getOverlappingPairCache()->getOverlappingPairArray();
	for( zp_int i = 0; i < pairs.size(); ++i )
	{
		manifolds.resize( 0 );

		const btBroadphasePair& pair = pairs[ i ];
		const btBroadphasePair* p = cache->findPair( pair.m_pProxy0, pair.m_pProxy1 );
		if( p == ZP_NULL ) continue;

		p->m_algorithm->getAllContactManifolds( manifolds );

		for( zp_int j = 0; j < manifolds.size(); ++j )
		{
			btPersistentManifold* manifold = manifolds[ j ];
			btCollisionObject* b0 = static_cast< btCollisionObject* >( manifold->getBody0() );
			btCollisionObject* b1 = static_cast< btCollisionObject* >( manifold->getBody1() );

			btCollisionObject* otherObject = m_phantom == b0 ? b1 : b0;

			for( zp_int m = 0; m < manifold->getNumContacts(); ++m )
			{
				btManifoldPoint& point = manifold->getContactPoint( m );
				
			}
		}
	}
}
