#include "zpPhysics.h"
#include "src\btBulletCollisionCommon.h"

const zp_char* s_colliderNames[] =
{
	"",
	"Plane",
	"Box",
	"Capsule",
	"Sphere",
	"Cylinder",
	"Hull",
	"Mesh",
};

zpColliderShape _strToCollider( const zp_char* str )
{
	if( str != ZP_NULL )
	{
		for( zp_int i = 0; i < zpColliderShape_Count; ++i )
		{
			if( zp_strcmp( s_colliderNames[i], str ) == 0 )
			{
				return (zpColliderShape)i;
			}
		}
	}
	return ZP_COLLIDER_SHAPE_NONE;
}

zpCollider::zpCollider()
	: m_collider( ZP_NULL )
	, m_shape( ZP_COLLIDER_SHAPE_NONE )
{
}
zpCollider::~zpCollider()
{
}

zp_handle zpCollider::getCollider() const
{
	return m_collider;
}
zpColliderShape zpCollider::getColliderShape() const
{
	return m_shape;
}


zpColliderCache::zpColliderCache() {}
zpColliderCache::~zpColliderCache() {}

zpCollider* zpColliderCache::getCollider( const zpBison::Value& v )
{
	zpColliderShape shape;
	zp_float p[4];
	const zp_float* d;
	zp_uint s;
	zp_hash h = generateColliderHash( v, shape, p, d, s );

	zpColliderPair* cp = ZP_NULL;
	zp_bool found = m_colliders.findIf( [ h ]( const zpColliderPair& pair ) {
		return pair.hash == h;
	}, &cp );

	if( found )
	{
		cp->refCount++;
	}
	else
	{
		btCollisionShape* collider;
		switch( shape )
		{
		case ZP_COLLIDER_SHAPE_NONE:
			{
				collider = new btEmptyShape();
			}
			break;
		case ZP_COLLIDER_SHAPE_PLANE:
			{
				btVector3 n( p[0], p[1], p[2] );
				btScalar d = p[3];
				collider = new btStaticPlaneShape( n, d );
			}
			break;
		case ZP_COLLIDER_SHAPE_BOX:
			{
				btVector3 e( p[0], p[1], p[2] );
				collider = new btBoxShape( e );
			}
			break;
		case ZP_COLLIDER_SHAPE_CAPSULE:
			{
				collider = new btCapsuleShape( p[0], p[1] );
			}
			break;
		case ZP_COLLIDER_SHAPE_SPHERE:
			{
				collider = new btSphereShape( p[0] );
			}
			break;
		case ZP_COLLIDER_SHAPE_CYLINDER:
			{
				btVector3 e( p[0], p[1], p[2] );
				collider = new btCylinderShape( e );
			}
			break;
		case ZP_COLLIDER_SHAPE_HULL:
			{
				collider = new btConvexHullShape( d, s, sizeof( btVector3 ) );
			}
			break;
		case ZP_COLLIDER_SHAPE_MESH:
			{
			}
			break;
		}

		collider->setUserPointer( (void*)h );

		// find a zero-ref count object first
		zp_bool found = m_colliders.findIf( []( const zpColliderPair& pair ) {
			return pair.refCount == 0;
		}, &cp );

		// otherwise, push back a new collider
		if( !found )
		{
			cp = &m_colliders.pushBackEmpty();
		}
		
		cp->hash = h;
		cp->refCount = 1;
		cp->collider = new zpCollider;
		cp->collider->m_collider = collider;
		cp->collider->m_shape = shape;
	}

	return cp->collider;
}
void zpColliderCache::removeCollider( zpCollider* collider )
{
	btCollisionShape* c = (btCollisionShape*)collider->getCollider();
	zp_hash h = (zp_hash)c->getUserPointer();

	zp_size_t index;
	zp_bool found = m_colliders.findIndexIf( [ h ]( const zpColliderPair& pair ) {
		return pair.hash == h;
	}, index );

	if( found )
	{
		zpColliderPair* cp = &m_colliders[ index ];

		--cp->refCount;
		if( cp->refCount == 0 )
		{
			ZP_SAFE_DELETE( c );
			ZP_SAFE_DELETE( cp->collider );
			m_colliders.erase( index );
		}
	}
}

void zpColliderCache::create()
{
	m_colliders.clear();
}
void zpColliderCache::destroy()
{
	ZP_ASSERT( m_colliders.isEmpty(), "Not all colliders destroyed" );
	m_colliders.clear();
}

zp_hash zpColliderCache::generateColliderHash( const zpBison::Value& v, zpColliderShape& shape, zp_float* p, const zp_float*& d, zp_uint& s ) const
{
	const zpBison::Value& size = v[ "Size" ];
	const zp_char* shapeStr = v[ "Shape" ].asCString();
	shape = _strToCollider( shapeStr );

	zpFixedStringBuffer< 256 > buff;
	buff << shapeStr;

	switch( shape )
	{
	case ZP_COLLIDER_SHAPE_NONE:
		{
		}
		break;
	case ZP_COLLIDER_SHAPE_PLANE:
		{
			const zpBison::Value& norm = size[ "Normal" ];
			p[0] = norm[0].asFloat();
			p[1] = norm[1].asFloat();
			p[2] = norm[2].asFloat();
			p[3] = size[ "Constant" ].asFloat();

			buff << p[0] << p[1] << p[2] << p[3];
		}
		break;
	case ZP_COLLIDER_SHAPE_BOX:
		{
			const zpBison::Value& extents = size[ "Extents" ];
			p[0] = extents[0].asFloat();
			p[1] = extents[1].asFloat();
			p[2] = extents[2].asFloat();

			buff << p[0] << p[1] << p[2];
		}
		break;
	case ZP_COLLIDER_SHAPE_CAPSULE:
		{
			const zpBison::Value& r = size[ "Radius" ];
			const zpBison::Value& h = size[ "Height" ];
			p[0] = r.asFloat();
			p[1] = h.asFloat();

			buff << p[0] << p[1];
		}
		break;
	case ZP_COLLIDER_SHAPE_SPHERE:
		{
			const zpBison::Value& r = size[ "Radius" ];
			p[0] = r.asFloat();

			buff << p[0];
		}
		break;
	case ZP_COLLIDER_SHAPE_CYLINDER:
		{
			const zpBison::Value& extents = size[ "Extents" ];
			p[0] = extents[0].asFloat();
			p[1] = extents[1].asFloat();
			p[2] = extents[2].asFloat();

			buff << p[0] << p[1] << p[2];
		}
		break;
	case ZP_COLLIDER_SHAPE_HULL:
		{
			const zpBison::Value& data = size[ "Points" ];

			d = (const zp_float*)data.asData();
			s = data.size() / sizeof( zp_float );

			buff << p[0] << p[1] << p[2] << p[3];
		}
		break;
	}

	zp_hash h = 0;
	h = zp_fnv1_32_string( buff.str(), h );

	return h;
}



zpCollisionMask::zpCollisionMask() {}
zpCollisionMask::~zpCollisionMask() {}

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

void zpCollisionMask::create()
{
	m_collisionMasks.clear();
}
void zpCollisionMask::destroy()
{
	m_collisionMasks.clear();
}
