#ifndef ZP_KINEMATIC_BODY_COMPONENT_H
#define ZP_KINEMATIC_BODY_COMPONENT_H

class zpKinematicBodyComponent : public zpComponent
{
public:
	zpKinematicBodyComponent( zpObject* obj, const zpBison::Value& def );
	virtual ~zpKinematicBodyComponent();

	zp_bool canJump() const;

	void jump();
	void walk( const zpVector4f& direction, const zpScalar& speed );
	void stop();

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

	zpKinematicBody m_kinematicBody;
};

class zpKinematicBodyComponentPool : public zpContentPool< zpKinematicBodyComponent, 8 >
{
public:
	zpKinematicBodyComponentPool();
	virtual ~zpKinematicBodyComponentPool();

	void update( zp_float deltaTime, zp_float realTime );
	void simulate();
};

#endif
