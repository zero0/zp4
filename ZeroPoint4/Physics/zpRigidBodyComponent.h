#pragma once
#ifndef ZP_RIGID_BODY_COMPONENT
#define ZP_RIGID_BODY_COMPONENT

class zpRigidBodyComponent : public zpComponent {
public:
	zpRigidBodyComponent();
	virtual ~zpRigidBodyComponent();

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
};

#endif