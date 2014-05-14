#pragma once
#ifndef ZP_COMPONENT_H
#define ZP_COMPONENT_H

enum zpComponentFlag
{
	ZP_COMPONENT_FLAG_ENABLED,
	ZP_COMPONENT_FLAG_CREATED,
	ZP_COMPONENT_FLAG_DESTROYED,
	ZP_COMPONENT_FLAG_SHOULD_DESTROY
};

ZP_ABSTRACT_CLASS zpComponent
{
public:
	zpComponent( zpObject* obj );
	virtual ~zpComponent();

	zpObject* getParentObject() const;

	void setEnabled( zp_bool enabled );
	zp_bool isEnabled() const;
	zp_bool isCreated() const;

	void update();
	void simulate();

	void create();
	void initialize();
	void destroy();

	void sendMessageToParentGameObject( const zpMessage& message );
	void sendMessageToSiblingComponents( const zpMessage& message );

	zpWorld* getWorld() const;
	zpApplication* getApplication() const;

protected:
	virtual void onCreate() {};
	virtual void onInitialize() {};
	virtual void onDestroy() {};

	virtual void onUpdate() {};
	virtual void onSimulate() {};

	virtual void onEnabled() {};
	virtual void onDisabled() {};

	zpFlag8 m_flags;

	zpObject* m_parentObject;
};

#endif