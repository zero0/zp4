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

ZP_ABSTRACT_CLASS zpComponent : public zpMessageReceiver, public zpSerializable
{
public:
	zpComponent();
	virtual ~zpComponent();

	void setParentGameObject( zpGameObject* parent );
	zpGameObject* getParentGameObject() const;

	void setEnabled( zp_bool enabled );
	zp_bool isEnabled() const;
	zp_bool isCreated() const;

	void update();
	void simulate();

	void create();
	void destroy();

	void sendMessageToParentGameObject( const zpMessage& message );
	void sendMessageToSiblingComponents( const zpMessage& message );

	zpWorld* getWorld() const;
	zpGame* getGame() const;

protected:
	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void onUpdate() = 0;
	virtual void onSimulate() {};

	virtual void onEnabled() = 0;
	virtual void onDisabled() = 0;

private:
	zpFlag8 m_flags;

	zpGameObject* m_parentGameObject;
};

#endif