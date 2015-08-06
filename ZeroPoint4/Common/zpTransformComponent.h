#pragma once
#ifndef ZP_TRANSFORM_COMPONENT_H
#define ZP_TRANSFORM_COMPONENT_H

class zpTransformComponent : public zpComponent
{
public:
	zpTransformComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpTransformComponent();

	void setLocalTransform( const zpVector4f& localPosition, const zpQuaternion4f& localRotation );
	void setLocalTransform( const zpVector4f& localPosition, const zpQuaternion4f& localRotation, const zpVector4f& localScale );

	void setLocalPosition( const zpVector4f& localPosition );
	void setLocalScale( const zpVector4f& localScale );
	void setLocalRotation( const zpQuaternion4f& localRotation );

	const zpVector4f& getLocalPosition() const;
	const zpQuaternion4f& getLocalRotation() const;
	const zpVector4f& getLocalScale() const;

	const zpVector4f& getWorldPosition() const;

	const zpMatrix4f& getWorldTransform() const;
	const zpMatrix4f& getLocalTransform() const;

	const zpArrayList< zpTransformComponent* >& getChildren() const;

	void setParent( zpTransformComponent* parent );
	zpTransformComponent* getParent() const;

	zp_uint getChildCount() const;
	zpTransformComponent* getChild( zp_uint index ) const;

	zpTransformComponent* removeChild( zp_uint index );

	void addChild( zpTransformComponent* child );
	void addChild( zpTransformComponent* child, const zpVector4f& localPosition );
	void addChild( zpTransformComponent* child, const zpVector4f& localPosition, const zpQuaternion4f& localRotation );
	void addChild( zpTransformComponent* child, const zpVector4f& localPosition, const zpQuaternion4f& localRotation, const zpVector4f& localScale );

	void translate( const zpVector4f& dir );

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate( zp_float deltaTime, zp_float realTime );
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	zp_int findChildDirect( zpTransformComponent* child );

	zpVector4f m_localPosition;
	zpQuaternion4f m_localRotation;
	zpVector4f m_localScale;

	zpMatrix4f m_localTransform;
	zpMatrix4f m_worldTransform;

	zpTransformComponent* m_parent;
	zpArrayList< zpTransformComponent* > m_children;
};

class zpTransformComponentPool : public zpContentPool< zpTransformComponent, 64 >
{
public:
	zpTransformComponentPool();
	virtual ~zpTransformComponentPool();

	void update( zp_float deltaTime, zp_float realTime );
};

#endif
