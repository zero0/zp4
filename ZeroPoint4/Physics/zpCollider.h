#ifndef ZP_COLLIDER_H
#define ZP_COLLIDER_H

enum zpColliderShape
{
	ZP_COLLIDER_SHAPE_NONE,
	ZP_COLLIDER_SHAPE_PLANE,
	ZP_COLLIDER_SHAPE_BOX,
	ZP_COLLIDER_SHAPE_CAPSULE,
	ZP_COLLIDER_SHAPE_SPHERE,
	ZP_COLLIDER_SHAPE_CYLINDER,
	ZP_COLLIDER_SHAPE_HULL,
	ZP_COLLIDER_SHAPE_MESH,

	zpColliderShape_Count,
};

class zpColliderCache;

class zpCollider
{
public:
	zpCollider();
	~zpCollider();

	zp_handle getCollider() const;
	zpColliderShape getColliderShape() const;

private:
	zp_handle m_collider;
	zpColliderShape m_shape;

	friend class zpColliderCache;
};

struct zpColliderPair
{
	zpCollider collider;
	zp_int refCount;
};

class zpColliderCache
{
public:
	zpColliderCache();
	~zpColliderCache();

	zpCollider* getCollider( const zpBison::Value& v );
	void removeCollider( zpCollider* collider );

	void create();
	void destroy();

private:

	zp_hash generateColliderHash( const zpBison::Value& v, zpColliderShape& shape, zp_float* p, const zp_float*& d, zp_uint& s ) const;

	zpHashMap< zp_hash, zpColliderPair > m_colliders;
};

class zpCollisionMask
{
public:
	zpCollisionMask();
	~zpCollisionMask();

	zp_short getCollisionMask( const zpString& maskName );
	zp_short getCollisionMask( const zp_char* maskName );

	void getCollisionMaskNames( zpArrayList< zpString >& names ) const;

	void create();
	void destroy();

private:
	zpHashMap< zpString, zp_short > m_collisionMasks;
};

#endif
