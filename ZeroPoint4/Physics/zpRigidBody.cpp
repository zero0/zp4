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

void zpRigidBody::create( const zpBison::Value& v, zp_bool isStatic )
{
	m_mass = v[ "Mass" ].asFloat();

	m_group = zpCollisionMask::getInstance()->getCollisionMask( v[ "Group" ].asCString() );
	m_mask = zpCollisionMask::getInstance()->getCollisionMask( v[ "Mask" ].asCString() );
	m_collider = zpColliderCache::getInstance()->getCollider( v[ "Collider" ] );

	m_isStatic = m_mass < ZP_RIGID_BODY_STATIC_MASS || isStatic;
}
void zpRigidBody::initialize( const zpMatrix4f& transform )
{
	btCollisionShape* shape = (btCollisionShape*)m_collider->getCollider();
	btVector3 inertia( 0, 0, 0 );

	btQuaternion rot( btQuaternion::getIdentity() );
	btVector3 pos;
	transform.getRow( 3 ).store4( pos.m_floats );

	btTransform trans( rot, pos );

	if( m_isStatic )
	{
		m_mass = 0.f;

		btRigidBody::btRigidBodyConstructionInfo info( m_mass, ZP_NULL, shape, inertia );
		info.m_startWorldTransform = trans;

		btRigidBody* body = new btRigidBody( info );
		body->setCollisionFlags( btRigidBody::CF_STATIC_OBJECT );

		m_rigidBody = body;
		m_motionState = ZP_NULL;
	}
	else
	{
		shape->calculateLocalInertia( m_mass, inertia );

		btMotionState* motion = new btDefaultMotionState( trans );
		btRigidBody::btRigidBodyConstructionInfo info( m_mass, motion, shape, inertia );

		btRigidBody* body = new btRigidBody( info );

		m_rigidBody = body;
		m_motionState = motion;
	}
}
void zpRigidBody::destroy()
{
	zpColliderCache::getInstance()->removeCollider( (zpCollider*)m_collider );
	m_collider = ZP_NULL;

	btMotionState* motion = (btMotionState*)m_motionState;
	btRigidBody* rigidBody = (btRigidBody*)m_rigidBody;

	ZP_SAFE_DELETE( motion );
	ZP_SAFE_DELETE( rigidBody );

	m_rigidBody = ZP_NULL;
	m_motionState = ZP_NULL;
}

void zpRigidBody::getMatrix( zpMatrix4f& transform ) const
{
	btMotionState* motion = (btMotionState*)m_motionState;

	btTransform t;
	motion->getWorldTransform( t );

	t.getOpenGLMatrix( transform.getData() );
}
void zpRigidBody::getPositionRotation( zpVector4f& position, zpQuaternion4f& rotation ) const
{
	btMotionState* motion = (btMotionState*)m_motionState;

	btTransform t;
	motion->getWorldTransform( t );

	const btVector3& o = t.getOrigin();
	position = zpVector4f( o.x(), o.y(), o.z(), 1.f );

	rotation.load4( t.getRotation() );
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
				part = parts[i].trim();
				
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
