#pragma once
#ifndef ZP_PHANTOM_H
#define ZP_PHANTOM_H

class zpPhantom
{
public:
	zpPhantom();
	~zpPhantom();

	void create( const zpMatrix4f& transform, const zpBison::Value& v );
	void destroy();

	void setMatrix( const zpMatrix4f& transform );
	zp_bool getMatrix( zpMatrix4f& transform ) const;

	zp_short getGroup() const;
	zp_short getMask() const;

	zp_handle getPhantom() const;

	zp_bool rayTest( const zpVector4f& fromWorld, const zpVector4f& toWorld, zpCollisionHitResult& hit ) const;

	void processCollisions( zp_handle dymaicsWorld ) const;

private:
	zp_handle m_phantom;
	zpCollider* m_collider;

	zp_short m_group;
	zp_short m_mask;
};

#endif
