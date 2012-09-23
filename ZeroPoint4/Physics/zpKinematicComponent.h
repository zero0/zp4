#pragma once
#ifndef ZP_KINEMATIC_COMPONENT
#define ZP_KINEMATIC_COMPONENT

class zpKinematicComponent : public zpComponent {
public:
	zpKinematicComponent();
	virtual ~zpKinematicComponent();

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