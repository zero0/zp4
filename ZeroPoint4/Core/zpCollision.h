#pragma once
#ifndef ZP_COLLISION_H
#define ZP_COLLISION_H

#define ZP_IS_COLLISION( c ) ( (c) != ZP_COLLISION_TYPE_NONE )

enum zpCollisionType {
	ZP_COLLISION_TYPE_NONE = 0,
	ZP_COLLISION_TYPE_INTERSECT,
	ZP_COLLISION_TYPE_CONTAINS
};

class zpCollision {
public:
	~zpCollision();

	static zpCollisionType testCollision( const zpBoundingAABB& a, const zpVector4f& b );
	static zpCollisionType testCollision( const zpBoundingAABB& a, const zpRay& b );
	static zpCollisionType testCollision( const zpBoundingAABB& a, const zpBoundingAABB& b );
	static zpCollisionType testCollision( const zpBoundingAABB& a, const zpBoundingSphere& b );
	
	static zpCollisionType testCollision( const zpBoundingSphere& a, const zpVector4f& b );
	static zpCollisionType testCollision( const zpBoundingSphere& a, const zpRay& b );
	static zpCollisionType testCollision( const zpBoundingSphere& a, const zpBoundingAABB& b );
	static zpCollisionType testCollision( const zpBoundingSphere& a, const zpBoundingSphere& b );

	static zpCollisionType testCollision( const zpFrustum& a, const zpVector4f& b );
	static zpCollisionType testCollision( const zpFrustum& a, const zpRay& b );
	static zpCollisionType testCollision( const zpFrustum& a, const zpBoundingAABB& b );
	static zpCollisionType testCollision( const zpFrustum& a, const zpBoundingSphere& b );

private:
	zpCollision();
};

#endif