#pragma once
#ifndef ZP_COLLIDER_COMPONENT_H
#define ZP_COLLIDER_COMPONENT_H

enum zpColliderShape {
	ZP_COLLIDER_SHAPE_NONE =	0,
	ZP_COLLIDER_SHAPE_PLANE,
	ZP_COLLIDER_SHAPE_BOX,
	ZP_COLLIDER_SHAPE_CAPSULE,
	ZP_COLLIDER_SHAPE_SPHERE,
	ZP_COLLIDER_SHAPE_CYLINDER,
	ZP_COLLIDER_SHAPE_CONVEX_HULL,

	ZP_COLLIDER_SHAPE_Count
};

class btCollisionShape;

class zpColliderComponent : public zpComponent {
public:
	zpColliderComponent();
	virtual ~zpColliderComponent();

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	btCollisionShape* getShape() const;
	zpColliderShape getShapeType() const;

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	btCollisionShape* m_shape;
	
	zpColliderShape m_shapeType;
	zpVector4f m_size;
};

class zpColliderCache {
public:
	~zpColliderCache();

	static zpColliderCache* getInstance();

	btCollisionShape* getColliderShape( zpColliderShape shapeType, const zpVector4f& size );
	void removeColliderShapeReference( btCollisionShape* shape );

private:
	zpColliderCache();

	zpHashMap<zp_hash, btCollisionShape*> m_colliderShapes;
	zpHashMap<btCollisionShape*, zp_int> m_colliderRefCounts;
};

#endif