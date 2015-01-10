#pragma once
#ifndef ZP_TRANSFORM_COMPONENT_H
#define ZP_TRANSFORM_COMPONENT_H

class zpTransformComponent : public zpComponent
{
public:
	zpTransformComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpTransformComponent();

	//void setLocalTransform( const zpMatrix4f& localTransform );
	void setLocalTransform( const zpVector4f& localPosition, const zpQuaternion4f& localRotation, const zpVector4f& localScale );

	void setLocalPosition( const zpVector4f& localPosition );
	void setLocalScale( const zpVector4f& localScale );
	void setLocalRotation( const zpQuaternion4f& localRotation );

	const zpMatrix4f& getWorldTransform() const;
	const zpMatrix4f& getLocalTransform() const;

	const zpArrayList< zpTransformComponent* >& getChildren() const;

	zpTransformComponent* getParent() const;

	zp_uint getChildCount() const;
	zpTransformComponent* getChild( zp_uint index ) const;

	zpTransformComponent* removeChild( zp_uint index, zp_bool shouldDestroy = false );

	void addChild( zpTransformComponent* child );
	void addChild( zpTransformComponent* child, const zpMatrix4f& localTransform );

	void translate( const zpVector4f& dir );

	const zpVector4f& getLocalPosition() const;
	const zpVector4f& getWorldPosition() const;

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate();
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	zpVector4f m_localPosition;
	zpQuaternion4f m_localRotation;
	zpVector4f m_localScale;

	zpMatrix4f m_localTransform;
	zpMatrix4f m_worldTransform;

	zpTransformComponent* m_parent;
	zpArrayList< zpTransformComponent* > m_children;
};

class zpTransformComponentPool : public zpContentPool< zpTransformComponent, 8 >
{
public:
	zpTransformComponentPool();
	virtual ~zpTransformComponentPool();

	void update();
};

#endif
