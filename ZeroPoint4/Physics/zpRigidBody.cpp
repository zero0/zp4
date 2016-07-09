#include "zpPhysics.h"

#include "zpPhysicsLib.inl"
#include "src/btBulletCollisionCommon.h"
#include "src/btBulletDynamicsCommon.h"

#define ZP_RIGID_BODY_STATIC_MASS    (1e-6f)

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

void zpRigidBody::create( zpPhysicsEngine* engine, const zpBison::Value& v, zp_bool isStatic )
{
    m_mass = v[ "Mass" ].asFloat();

    m_group = engine->getCollisionMask()->getCollisionMask( v[ "Group" ].asCString() );
    m_mask =  engine->getCollisionMask()->getCollisionMask( v[ "Mask" ].asCString() );
    m_collider = engine->getColliderCache()->getCollider( v[ "Collider" ] );

    m_isStatic = m_mass < ZP_RIGID_BODY_STATIC_MASS || isStatic;
}
void zpRigidBody::initialize( const zpMatrix4f& transform )
{
    btCollisionShape* shape = (btCollisionShape*)m_collider->getCollider();
    btVector3 inertia( 0, 0, 0 );

    btQuaternion rot( btQuaternion::getIdentity() );
    btVector3 pos;
    zpMath::Vector4Store4( transform.r[ 3 ], pos.m_floats );

    btTransform trans( rot, pos );

    if( m_isStatic )
    {
        m_mass = 0.f;

        btRigidBody::btRigidBodyConstructionInfo info( m_mass, ZP_NULL, shape, inertia );
        info.m_startWorldTransform = trans;

        btRigidBody* body = new btRigidBody( info );
        body->setCollisionFlags( btRigidBody::CF_STATIC_OBJECT );
        body->setUserPointer( this );

        m_rigidBody = body;
        m_motionState = ZP_NULL;
    }
    else
    {
        shape->calculateLocalInertia( m_mass, inertia );

        btMotionState* motion = new btDefaultMotionState( trans );
        btRigidBody::btRigidBodyConstructionInfo info( m_mass, motion, shape, inertia );

        btRigidBody* body = new btRigidBody( info );
        body->setUserPointer( this );

        m_rigidBody = body;
        m_motionState = motion;
    }
}
void zpRigidBody::destroy( zpPhysicsEngine* engine )
{
    engine->getColliderCache()->removeCollider( m_collider );
    m_collider = ZP_NULL;

    btMotionState* motion = (btMotionState*)m_motionState;
    btRigidBody* rigidBody = (btRigidBody*)m_rigidBody;
    rigidBody->setUserPointer( ZP_NULL );

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

    zp_float matrix[ 16 ];
    t.getOpenGLMatrix( matrix );

    transform = zpMath::MatrixLoadOpenGL( matrix );
}
void zpRigidBody::getPositionRotation( zpVector4f& position, zpQuaternion4f& rotation ) const
{
    btMotionState* motion = (btMotionState*)m_motionState;

    btTransform t;
    motion->getWorldTransform( t );

    const btVector3 o = t.getOrigin();
    const btQuaternion q = t.getRotation();

    position = zpMath::Vector4( o.x(), o.y(), o.z(), 1.f );
    rotation = zpMath::Quaternion( q.x(), q.y(), q.z(), q.w() );
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
