#ifndef ZP_RIGID_BODY_COMPONENT_H
#define ZP_RIGID_BODY_COMPONENT_H

class zpRigidBodyComponent : public zpComponent
{
public:
	zpRigidBodyComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpRigidBodyComponent();

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

	zpRigidBody m_rigidBody;
};

class zpRigidBodyComponentPool : public zpContentPool< zpRigidBodyComponent, 64 >
{
public:
	zpRigidBodyComponentPool();
	virtual ~zpRigidBodyComponentPool();

	void update( zp_float deltaTime, zp_float realTime );
	void simulate();
};

#endif
