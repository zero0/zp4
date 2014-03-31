#pragma once
#ifndef ZP_ATTACHMENT_COMPONENT_H
#define ZP_ATTACHMENT_COMPONENT_H

class zpAttachmentComponent : public zpComponent
{
public:
	zpAttachmentComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpAttachmentComponent();

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate();
	void onSimulate();

	void onEnabled();
	void onDisabled();

	const zpArrayList< zpObject* >& getChildren() const;

	zp_uint getChildCount() const;
	zpObject* getChildAt( zp_uint index ) const;

	zpObject* removeChild( zp_uint index );
	void addChild( zpObject* child );

private:
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
