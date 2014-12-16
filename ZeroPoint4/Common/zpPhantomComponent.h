#pragma once
#ifndef ZP_PHANTOM_COMPONENT_H
#define ZP_PHANTOM_COMPONENT_H

class zpPhantomComponent : public zpComponent
{
public:
	zpPhantomComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpPhantomComponent();

protected:
	void onCreate();
	void onInitialize();
	void onDestroy();

	void onUpdate();
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	zp_bool m_addOnEnable;
	zp_bool m_addOnCreate;
	zp_bool m_isAdded;
	
	zpPhantom m_phantom;

	zpString m_messageOnEnter;
	zpString m_messageOnOverlap;
	zpString m_messageOnLeave;
};

class zpPhantomComponentPool : public zpContentPool< zpPhantomComponent, 8 >
{
public:
	zpPhantomComponentPool();
	virtual ~zpPhantomComponentPool();

	void update();
};

#endif
