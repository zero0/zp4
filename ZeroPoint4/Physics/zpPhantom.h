#pragma once
#ifndef ZP_PHANTOM_H
#define ZP_PHANTOM_H

enum
{
	ZP_PHANTON_MAX_TRACKED_OBJECTS = 16
};

ZP_PURE_INTERFACE zpPhantomCallback
{
public:
	virtual void onObjectEnter() = 0;
	virtual void onObjectOverlap() = 0;
	virtual void onObjectLeave() = 0;
};

struct zpPhantomCollisionHitInfo
{
	zpVector4f worldPositionOnA;
	zpVector4f worldPositionOnB;
	zpVector4f worldNormalOnB;

	zp_handle otherObject;
};

class zpPhantom
{
public:
	zpPhantom();
	~zpPhantom();

	void create( zpPhysicsEngine* engine, const zpMatrix4f& transform, const zpBison::Value& v );
	void destroy( zpPhysicsEngine* engine );

	void setMatrix( const zpMatrix4f& transform );
	zp_bool getMatrix( zpMatrix4f& transform ) const;

	zp_short getGroup() const;
	zp_short getMask() const;

	zp_handle getPhantom() const;

	zp_bool rayTest( const zpVector4f& fromWorld, const zpVector4f& toWorld, zpCollisionHitResult& hit ) const;

	void processCollisions( zp_handle dymaicsWorld, zp_float timeStep );

	void onCollisionEnter( const zpPhantomCollisionHitInfo& hit );
	void onCollisionStay( const zpPhantomCollisionHitInfo& hit );
	void onCollisionLeave( zp_handle otherObject );

private:
	zp_handle m_phantom;
	zpCollider* m_collider;

	zp_short m_group;
	zp_short m_mask;

	zpFixedArrayList< zp_handle, ZP_PHANTON_MAX_TRACKED_OBJECTS > m_trackedObjects;
};

#endif
