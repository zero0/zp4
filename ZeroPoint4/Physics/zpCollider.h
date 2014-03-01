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

class zpColliderCache
{
public:
	~zpColliderCache();

	static zpColliderCache* getInstance();

	zpCollider* getCollider( const zpBison::Value& v );
	void removeCollider( zpCollider* collider );

private:
	zpColliderCache();

	zp_hash generateColliderHash( const zpBison::Value& v, zpColliderShape& shape, zp_float* p, const zp_float*& d, zp_uint& s ) const;

	zpHashMap< zp_hash, zpCollider > m_colliders;
	zpHashMap< zp_hash, zp_int > m_colliderRefCounts;
};

#endif
