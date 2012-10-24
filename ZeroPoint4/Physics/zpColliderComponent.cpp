#include "zpPhysics.h"
#include "src\btBulletCollisionCommon.h"

zpColliderComponent::zpColliderComponent() 
	: m_shape( ZP_NULL )
	, m_shapeType( ZP_COLLIDER_SHAPE_NONE )
{}
zpColliderComponent::~zpColliderComponent() {}

void zpColliderComponent::receiveMessage( const zpMessage& message ) {}

const zp_char* s_types[] = {
	"",
	"plane",
	"box",
	"capsule",
	"sphere",
	"cylinder",
	"convex",
};

void zpColliderComponent::serialize( zpSerializedOutput* out ) {
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	zpString type( s_types[ m_shapeType ] );
	out->writeString( type, "@type" );

	out->writeBlock( "Size" );
	{
		zpSerializableObject<zpVector4f> size( m_size );
		size.serialize( out );
	}
	out->endBlock();

	out->endBlock();
}
void zpColliderComponent::deserialize( zpSerializedInput* in ) {
	in->readBlock( ZP_SERIALIZE_TYPE_THIS );

	zpString type;
	in->readString( &type, "@type" );
	
	for( zp_uint i = ZP_COLLIDER_SHAPE_Count; i --> 0; ) {
		if( zp_strcmp( s_types[ i ], type.c_str() ) == 0 ) {
			m_shapeType = (zpColliderShape)i;
			break;
		}
	}

	if( in->readBlock( "Size" ) )
	{
		zpSerializableObject<zpVector4f> size;
		size.deserialize( in );
		m_size = size;
	}
	in->endBlock();
	
	in->endBlock();
}


btCollisionShape* zpColliderComponent::getShape() const {
	return m_shape;
}
zpColliderShape zpColliderComponent::getShapeType() const {
	return m_shapeType;
}

void zpColliderComponent::onCreate() {
	if( m_shape ) return;

	m_shape = zpColliderCache::getInstance()->getColliderShape( m_shapeType, m_size );
}
void zpColliderComponent::onDestroy() {
	zpColliderCache::getInstance()->removeColliderShapeReference( m_shape );
	m_shape = ZP_NULL;
}

void zpColliderComponent::onUpdate() {}

void zpColliderComponent::onEnabled() {}
void zpColliderComponent::onDisabled() {}


zpColliderCache::zpColliderCache() {}
zpColliderCache::~zpColliderCache() {}

zpColliderCache* zpColliderCache::getInstance() {
	static zpColliderCache cache;
	return &cache;
}

btCollisionShape* zpColliderCache::getColliderShape( zpColliderShape shapeType, const zpVector4f& size ) {
	btCollisionShape** shape = ZP_NULL;

	zp_float s[4];
	size.store4( s );

	zp_hash colliderName = (zp_hash)shapeType;
	colliderName = 31 * colliderName + *(zp_uint*)(zp_float*)&s[0];
	colliderName = 31 * colliderName + *(zp_uint*)(zp_float*)&s[1];
	colliderName = 31 * colliderName + *(zp_uint*)(zp_float*)&s[2];
	colliderName = 31 * colliderName + *(zp_uint*)(zp_float*)&s[3];

	if( m_colliderShapes.find( colliderName, &shape ) ) {
		m_colliderRefCounts[ *shape ]++;
	} else {
		switch( shapeType ) {
		case ZP_COLLIDER_SHAPE_NONE:
			*shape = new btEmptyShape();
			break;
		case ZP_COLLIDER_SHAPE_PLANE:
			{
				btVector3 normal;
				size.store3( normal );
				*shape = new btStaticPlaneShape( normal, zp_real_to_float( size.getW() ) );
			}
			break;
		case ZP_COLLIDER_SHAPE_BOX:
			{
				btVector3 halfSize;
				size.store3( halfSize );
				*shape = new btBoxShape( halfSize );
			}
			break;
		case ZP_COLLIDER_SHAPE_CAPSULE:
			*shape = new btCapsuleShape( zp_real_to_float( size.getX() ), zp_real_to_float( size.getY() ) );
			break;
		case ZP_COLLIDER_SHAPE_SPHERE:
			*shape = new btSphereShape( zp_real_to_float( size.getX() ) );
			break;
		case ZP_COLLIDER_SHAPE_CYLINDER:
			{
				btVector3 halfSize;
				size.store3( halfSize );
				*shape = new btCylinderShape( halfSize );
			}
			break;
		case ZP_COLLIDER_SHAPE_CONVEX_HULL:
			*shape = new btConvexHullShape();
			break;
		}

		(*shape)->setUserPointer( (void*)colliderName );
		m_colliderShapes[ colliderName ] = *shape;
		m_colliderRefCounts[ *shape ] = 1;
	}

	return *shape;
}
void zpColliderCache::removeColliderShapeReference( btCollisionShape* shape ) {
	zp_int* count = ZP_NULL;
	if( m_colliderRefCounts.find( shape, &count ) ) {
		if( --(*count) <= 0 ) {
			zp_hash colliderName = (zp_hash)shape->getUserPointer();

			m_colliderRefCounts.erase( shape );
			m_colliderShapes.erase( colliderName );

			ZP_SAFE_DELETE( shape );
		}
	}
}
