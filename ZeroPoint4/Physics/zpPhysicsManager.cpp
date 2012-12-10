#include "zpPhysics.h"
#include "src\btBulletCollisionCommon.h"
#include "src\BulletCollision\CollisionDispatch\btGhostObject.h"
#include "src\btBulletDynamicsCommon.h"

zpPhysicsManager::zpPhysicsManager() 
	: m_broadphase( ZP_NULL )
	, m_collisionConfiguration( ZP_NULL )
	, m_dispatcher( ZP_NULL )
	, m_solver( ZP_NULL )
	, m_world( ZP_NULL )
	, m_fixedTimeStep( 1.f / 60.f )
	, m_maxSubSteps( 6 )
	, m_gravity( 0, -9.81f, 0 )
{}
zpPhysicsManager::~zpPhysicsManager() {}

void zpPhysicsManager::serialize( zpSerializedOutput* out ) {
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	out->writeFloat( m_fixedTimeStep, "@fixed-timestep" );
	out->writeInt( m_maxSubSteps, "@max-sub-steps" );

	out->writeBlock( "Gravity" );
	{
		zpSerializableObject<zpVector4f>::serializeFrom( out, m_gravity );
	}
	out->endBlock();

	out->writeBlock( "CollisionGroups" );
	{
		m_collisionGroups.foreach( [ out ]( const zpString& group ) {
			out->writeBlock( "CollisionGroup" );
			{
				out->writeString( group, "@name" );
			}
			out->endBlock();
		} );
	}
	out->endBlock();

	out->writeBlock( "CollisionMasks" );
	{
		m_collisionMasks.foreach( [ out, this ]( const zpString& maskName, zp_short mask ) {
			out->writeBlock( "CollisionMask" );
			{
				out->writeString( maskName, "@name" );
				
				zpStringBuffer maskBuffer;
				if( !m_collisionGroups.isEmpty() ) {
					if( ( mask & ( 1 << 0 ) ) != 0 ) {
						maskBuffer << m_collisionMasks[ 0 ];
					}
					for( zp_uint i = 1; i < m_collisionGroups.size(); ++i ) {
						if( ( mask & ( 1 << i ) ) != 0 ) {
							maskBuffer << '|' << m_collisionGroups[ i ];
						}
					}
				}
				zpString maskText( maskBuffer.toString() );

				out->writeString( maskText, "@mask" );
			}
			out->endBlock();
		} );
	}
	out->endBlock();

	out->endBlock();
}
void zpPhysicsManager::deserialize( zpSerializedInput* in ) {
	if( in->readBlock( ZP_SERIALIZE_TYPE_THIS ) )
	{
		in->readFloat( &m_fixedTimeStep, "@fixed-timestep" );
		in->readInt( &m_maxSubSteps, "@max-sub-steps" );

		if( in->readBlock( "Gravity" ) )
		{
			zpSerializableObject<zpVector4f>::deserializeTo( in, m_gravity );
			in->endBlock();
		}

		if( in->readBlock( "CollisionGroups" ) )
		{
			zpString groupName;	
			in->readEachBlock( "CollisionGroup", [ this, &groupName ]( zpSerializedInput* in ) {
				in->readString( &groupName, "@name" );

				m_collisionGroups.pushBack( groupName );
			} );
			in->endBlock();
		}

		if( in->readBlock( "CollisionMasks" ) )
		{
			zpString maskText;
			zpString maskName;
			in->readEachBlock( "CollisionMask", [ this, &maskText, &maskName ]( zpSerializedInput* in ) {
				in->readString( &maskName, "@name" );
				in->readString( &maskText, "@mask" );

				zp_short mask = 0;
				zp_uint start = 0;
				zp_uint end = maskText.indexOf( '|' );
				do {
					zp_short group;
					if( end == zpString::npos ) {
						group = getCollisionGroup( maskText.substring( start ) );
					} else {
						group = getCollisionGroup( maskText.substring( start, end ) );
					}
					mask |= group;

					start = end;
					if( start != zpString::npos ) {
						end = maskText.indexOf( '|', start );
					}

				} while( end != zpString::npos );

				m_collisionMasks[ maskName ] = mask;
			} );
			in->endBlock();
		}

		in->endBlock();
	}
}

void zpPhysicsManager::receiveMessage( const zpMessage& message ) {}

btDynamicsWorld* zpPhysicsManager::getWorld() const {
	return m_world;
}

zp_short zpPhysicsManager::getCollisionGroup( const zpString& groupName ) {
	zp_short group = m_collisionGroups.indexOf( groupName );
	if( group == zpArrayList<zpString>::npos ) {
		group = m_collisionGroups.size();
		m_collisionGroups.pushBack( groupName );
	}
	return ( 1 << group );
}
zp_short zpPhysicsManager::getCollisionMask( const zpString& mask ) {
	return m_collisionMasks[ mask ];
}

void zpPhysicsManager::setGravity( const zpVector4f& gravity ) {
	m_gravity = gravity;
	if( m_world ) {
		btVector3 g;
		m_gravity.store3( g );
		m_world->setGravity( g );
	}
}
const zpVector4f& zpPhysicsManager::getGravity() const {
	return m_gravity;
}

void zpPhysicsManager::onCreate() {
	m_broadphase = new btDbvtBroadphase();
	m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback( new btGhostPairCallback() );
	
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher( m_collisionConfiguration );
	
	m_solver = new btSequentialImpulseConstraintSolver();

	m_world = new btDiscreteDynamicsWorld( m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration );
	
	btVector3 gravity;
	m_gravity.store3( gravity );
	m_world->setGravity( gravity );
}
void zpPhysicsManager::onDestroy() {
	ZP_SAFE_DELETE( m_world );
	ZP_SAFE_DELETE( m_solver );
	ZP_SAFE_DELETE( m_dispatcher );
	ZP_SAFE_DELETE( m_collisionConfiguration );
	ZP_SAFE_DELETE( m_broadphase );
}

void zpPhysicsManager::onUpdate() {
	m_world->stepSimulation( zpTime::getInstance()->getDeltaSeconds(), m_maxSubSteps, m_fixedTimeStep );
}

void zpPhysicsManager::onEnabled() {}
void zpPhysicsManager::onDisabled() {}