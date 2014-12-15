#pragma once
#ifndef ZP_TRIGGER_COMPONENT_H
#define ZP_TRIGGER_COMPONENT_H

class zpTriggerComponent : public zpComponent, public zpEventListener
{
public:
	zpTriggerComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpTriggerComponent();

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate();
	void onSimulate();

	void onEnabled();
	void onDisabled();

	const zpBoundingAABB& getBounds() const;

private:
	zpBoundingAABB m_bounds;

	zpString m_messageOnEnter;
	zpString m_messageOnOverlap;
	zpString m_messageOnLeave;
};

class zpTriggerComponentPool : public zpContentPool< zpTriggerComponent, 8 >
{
public:
	zpTriggerComponentPool();
	virtual ~zpTriggerComponentPool();

	void update();
};

#endif
