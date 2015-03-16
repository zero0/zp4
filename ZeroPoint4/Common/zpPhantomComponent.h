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

	void onUpdate( zp_float deltaTime, zp_float realTime );
	void onSimulate();

	void onEnabled();
	void onDisabled();

private:
	zp_bool m_addOnEnable;
	zp_bool m_addOnCreate;
	zp_bool m_isAdded;
	
	zpPhantom m_phantom;

	zpString m_eventOnEnter;
	zpString m_eventOnOverlap;
	zpString m_eventOnLeave;

	zpString m_messageOnEnter;
	zpString m_messageOnOverlap;
	zpString m_messageOnLeave;
};

class zpPhantomComponentPool : public zpContentPool< zpPhantomComponent, 8 >
{
public:
	zpPhantomComponentPool();
	virtual ~zpPhantomComponentPool();

	void update( zp_float deltaTime, zp_float realTime );
};

#endif
