#pragma once
#ifndef ZP_KINEMATIC_COMPONENT
#define ZP_KINEMATIC_COMPONENT

class btKinematicCharacterController;
class btPairCachingGhostObject;

class zpKinematicComponent : public zpComponent {
public:
	zpKinematicComponent();
	virtual ~zpKinematicComponent();

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	void addToWorld() const;
	void removeFromWorld() const;
	zp_bool isInWorld() const;

	void setMovementVelocity( zp_float velocity );
	zp_float getMovementVelocity() const;

	void jump();
	void setJumpSpeed( zp_float jumpSpeed );
	zp_float getJumpSpeed() const;
	void setJumpHeight( zp_float jumpHeight );
	zp_float getJumpHeight() const;

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	btKinematicCharacterController* m_character;
	btPairCachingGhostObject* m_ghostObject;

	zpPhysicsManager* m_manager;

	zp_bool m_jump;
	zp_float m_jumpHeight;
	zp_float m_jumpSpeed;

	zp_bool m_shouldAddOnCreate;
	zp_bool m_shouldAddOnEnableDisable;
	mutable zp_bool m_isInWorld;

	zp_float m_stepHeight;
	zp_float m_movementVelocity;

	zpVector2f m_inputDirection;
};

#endif