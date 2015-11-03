#pragma once
#ifndef ZP_PHANTOM_H
#define ZP_PHANTOM_H

enum
{
	ZP_PHANTON_MAX_TRACKED_OBJECTS = 8
};


struct zpPhantomCollisionHitInfo
{
	zpVector4f worldPositionOnA;
	zpVector4f worldPositionOnB;
	zpVector4f worldNormalOnB;

	zp_handle otherObject;
};

ZP_PURE_INTERFACE zpPhantomCollisionCallback
{
public:
	virtual void onCollisionEnter( const zpPhantomCollisionHitInfo& hit ) = 0;
	virtual void onCollisionStay( const zpPhantomCollisionHitInfo& hit ) = 0;
	virtual void onCollisionLeave( zp_handle otherObject ) = 0;
};

class zpPhantom
{
public:
	zpPhantom();
	~zpPhantom();

	void create( zpMatrix4fParamF transform, zp_short group, zp_short mask, zpCollider* collider, zpPhysicsEngine* engine );
	void destroy( zpPhysicsEngine* engine );

	void setMatrix( zpMatrix4fParamF transform );
	zpMatrix4f getMatrix() const;

	zp_short getGroup() const;
	zp_short getMask() const;

	zp_handle getPhantom() const;
	zpCollider* getCollider() const;

	zp_bool rayTest( zpVector4fParamF fromWorld, zpVector4fParamF toWorld, zpCollisionHitResult& hit ) const;

	void processCollisions( zp_handle dymaicsWorld, zp_float timeStep );

	void setCollisionCallback( zpPhantomCollisionCallback* callback );
	zpPhantomCollisionCallback* getCollisionCallback() const;

private:
	void onCollisionEnter( const zpPhantomCollisionHitInfo& hit );
	void onCollisionStay( const zpPhantomCollisionHitInfo& hit );
	void onCollisionLeave( zp_handle otherObject );

	zp_handle m_phantom;
	zpCollider* m_collider;

	zp_short m_group;
	zp_short m_mask;

	zpPhantomCollisionCallback* m_collisionCallback;

	zpFixedArrayList< zp_handle, ZP_PHANTON_MAX_TRACKED_OBJECTS > m_trackedObjects;
};

#endif
