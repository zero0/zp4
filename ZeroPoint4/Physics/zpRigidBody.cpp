#include "zpPhysics.h"

#include "zpPhysicsLib.inc"
#include "src/btBulletCollisionCommon.h"
#include "src/btBulletDynamicsCommon.h"

#define ZP_RIGID_BODY_STATIC_MASS	(1e-6f)

zpRigidBody::zpRigidBody()
	: m_rigidBody( ZP_NULL )
	, m_motionState( ZP_NULL )
	, m_collider( ZP_NULL )
	, m_isStatic( false )
	, m_mass( 1.0f )
	, m_group( 0 )
	, m_mask( 0 )
{
}
zpRigidBody::~zpRigidBody()
{
}

void zpRigidBody::create( const zpMatrix4f& transform, const zpBison::Value& v )
{
	m_mass = v[ "Mass" ].asFloat();

	m_group = zpCollisionMask::getInstance()->getCollisionMask( v[ "Group" ].asCString() );
	m_mask = zpCollisionMask::getInstance()->getCollisionMask( v[ "Mask" ].asCString() );
	m_collider = zpColliderCache::getInstance()->getCollider( v[ "Collider" ] );

	m_isStatic = m_mass < ZP_RIGID_BODY_STATIC_MASS;

	btCollisionShape* shape = (btCollisionShape*)m_collider->getCollider();
	btVector3 inertia( 0, 0, 0 );

	const zpVector4f& p = transform.getRow( 3 );

	btQuaternion rot( btQuaternion::getIdentity() );
	btVector3 pos( p.getX().getFloat(), p.getY().getFloat(), p.getZ().getFloat() );
	btTransform trans( rot, pos );

	if( m_isStatic )
	{
		m_mass = 0.f;

		btRigidBody::btRigidBodyConstructionInfo info( m_mass, ZP_NULL, shape, inertia );
		info.m_startWorldTransform = trans;

		m_rigidBody = new btRigidBody( info );
		m_motionState = ZP_NULL;
	}
	else
	{
		shape->calculateLocalInertia( m_mass, inertia );
		
		btMotionState* motion = new btDefaultMotionState( trans );
		btRigidBody::btRigidBodyConstructionInfo info( m_mass, motion, shape, inertia );

		m_rigidBody = new btRigidBody( info );
		m_motionState = motion;
	}
}
void zpRigidBody::destroy()
{
	zpColliderCache::getInstance()->removeCollider( (zpCollider*)m_collider );

	btMotionState* motion = (btMotionState*)m_motionState;
	btRigidBody* rigidBody = (btRigidBody*)m_rigidBody;

	ZP_SAFE_DELETE( motion );
	ZP_SAFE_DELETE( rigidBody );

	m_rigidBody = ZP_NULL;
	m_motionState = ZP_NULL;

	zpColliderCache::getInstance()->removeCollider( m_collider );
}

zp_bool zpRigidBody::getMatrix( zpMatrix4f& transform )
{
	if( m_isStatic ) return false;

	btMotionState* motion = (btMotionState*)m_motionState;

	btTransform t;
	motion->getWorldTransform( t );

	t.getOpenGLMatrix( transform.getData() );
	return true;
}

zp_bool zpRigidBody::isStatic() const
{
	return m_isStatic;
}
zp_short zpRigidBody::getGroup() const
{
	return m_group;
}
zp_short zpRigidBody::getMask() const
{
	return m_mask;
}

zp_handle zpRigidBody::getRigidBody() const
{
	return m_rigidBody;
}

zpCollisionMask::zpCollisionMask() {}
zpCollisionMask::~zpCollisionMask() {}

zpCollisionMask* zpCollisionMask::getInstance()
{
	static zpCollisionMask instance;
	return &instance;
}

zp_short zpCollisionMask::getCollisionMask( const zp_char* maskName )
{
	zp_short mask = 0;
	if( maskName != ZP_NULL )
	{
		mask = getCollisionMask( zpString( maskName ) );
	}
	return mask;
}
zp_short zpCollisionMask::getCollisionMask( const zpString& maskName )
{
	zp_short mask = 0;
	if( !maskName.isEmpty() )
	{
		zpFixedArrayList< zpString, 8 > parts;
		maskName.split( '|', parts );

		if( !parts.isEmpty() )
		{
			zp_short* foundMask;
			zpString part;
			for( zp_uint i = 0; i < parts.size(); ++i )
			{
				parts[i].trim( part );
				
				if( m_collisionMasks.find( part, &foundMask ) )
				{
					mask |= *foundMask;
				}
				else
				{
					zp_short newMask = 1 << m_collisionMasks.size();
					m_collisionMasks.put( part, newMask );
					mask |= newMask;
				}
			}
		}
	}
	return mask;
}

void zpCollisionMask::getCollisionMaskNames( zpArrayList< zpString >& names ) const
{
	m_collisionMasks.keys( names );
}
