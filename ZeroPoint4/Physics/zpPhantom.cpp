#include "zpPhysics.h"

#include "zpPhysicsLib.inc"
#include "src/btBulletCollisionCommon.h"
#include "src/btBulletDynamicsCommon.h"
#include "src/BulletCollision/CollisionDispatch/btGhostObject.h"

zpPhantom::zpPhantom()
{
	
}
zpPhantom::~zpPhantom()
{
	
}

void zpPhantom::create( const zpMatrix4f& transform, const zpBison::Value& v )
{
	m_group = zpCollisionMask::getInstance()->getCollisionMask( v[ "Group" ].asCString() );
	m_mask = zpCollisionMask::getInstance()->getCollisionMask( v[ "Mask" ].asCString() );
	m_collider = zpColliderCache::getInstance()->getCollider( v[ "Collider" ] );
	btCollisionShape* shape = (btCollisionShape*)m_collider->getCollider();

	btQuaternion rot( btQuaternion::getIdentity() );
	btVector3 pos;
	transform.getRow( 3 ).store4( pos.m_floats );

	btTransform trans( rot, pos );

	btPairCachingGhostObject* ghost = new btPairCachingGhostObject();
	ghost->setCollisionShape( shape );
	ghost->setCollisionFlags( ghost->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );

	ghost->setWorldTransform( trans );

	m_phantom = ghost;
}
void zpPhantom::destroy()
{

}

zp_bool zpPhantom::getMatrix( zpMatrix4f& transform )
{
	btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_phantom;

	const btTransform& t = ghost->getWorldTransform();

	t.getOpenGLMatrix( transform.getData() );
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
		hit.position.load4( cb.m_hitPointWorld.m_floats );
		hit.normal.load4( cb.m_hitNormalWorld.m_floats );
		hit.hitObject = cb.m_collisionObject ? cb.m_collisionObject->getUserPointer() : ZP_NULL;
		return true;
	}

	return false;
}

void zpPhantom::processCollisions( zp_handle dymaicsWorld ) const
{
	btDynamicsWorld* world = (btDynamicsWorld*)dymaicsWorld;

	btManifoldArray manifolds;
	btPairCachingGhostObject* ghost = (btPairCachingGhostObject*)m_phantom;

	const btBroadphasePairArray& pairs = ghost->getOverlappingPairCache()->getOverlappingPairArray();
	for( zp_int i = 0; i < pairs.size(); ++i )
	{
		manifolds.resize( 0 );

		const btBroadphasePair& pair = pairs[ i ];
		const btBroadphasePair* p = world->getPairCache()->findPair( pair.m_pProxy0, pair.m_pProxy1 );
		if( p == ZP_NULL )
		{
			p = &pair;
		}
		p->m_algorithm->getAllContactManifolds( manifolds );

		for( zp_int j = 0; j < manifolds.size(); ++j )
		{
			btPersistentManifold* manifold = manifolds[ j ];
			if( manifold->getNumContacts() > 0 )
			{
				btCollisionObject* b0 = static_cast< btCollisionObject* >( manifold->getBody0() );
				btCollisionObject* b1 = static_cast< btCollisionObject* >( manifold->getBody1() );

				zp_printfln( "%d contacts ", manifold->getNumContacts() );
			}
		}
	}
}
