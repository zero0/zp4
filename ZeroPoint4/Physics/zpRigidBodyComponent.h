#pragma once
#ifndef ZP_RIGID_BODY_COMPONENT
#define ZP_RIGID_BODY_COMPONENT

class btRigidBody;
class btMotionState;

class zpRigidBodyComponent : public zpComponent {
public:
	zpRigidBodyComponent();
	virtual ~zpRigidBodyComponent();

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	void addToWorld() const;
	void removeFromWorld() const;
	zp_bool isInWorld() const;

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	btRigidBody* m_body;
	btMotionState* m_motionState;

	zpPhysicsManager* m_manager;

	zp_bool m_shouldAddOnCreate;
	zp_bool m_shouldAddOnEnableDisable;

	zp_short m_collisionGroup;
	zp_short m_collisionMask;

	zp_float m_mass;

	zpString m_groupName;
	zpString m_maskName;
};

#endif