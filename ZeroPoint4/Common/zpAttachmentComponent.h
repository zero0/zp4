#pragma once
#ifndef ZP_ATTACHMENT_COMPONENT_H
#define ZP_ATTACHMENT_COMPONENT_H

class zpAttachmentComponent : public zpComponent
{
public:
	zpAttachmentComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpAttachmentComponent();

	const zpMatrix4f& getWorldTransform() const;

	const zpArrayList< zpObject* >& getChildren() const;

	zpObject* getParent() const;

	zp_uint getChildCount() const;
	zpObject* getChild( zp_uint index ) const;

	zpObject* removeChild( zp_uint index, zp_bool shouldDestroy = false );
	void addChild( zpObject* child );
	void addChild( zpObject* child, const zpMatrix4f& localTransform );

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate();
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	zpObject* m_parent;
	zpMatrix4f m_worldTransform;
	zpArrayList< zpObject* > m_children;
};

class zpAttachmentComponentPool : public zpContentPool< zpAttachmentComponent, 8 >
{
public:
	zpAttachmentComponentPool();
	virtual ~zpAttachmentComponentPool();

	void update();
};

#endif
