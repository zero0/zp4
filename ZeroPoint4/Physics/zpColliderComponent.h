#pragma once
#ifndef ZP_COLLIDER_COMPONENT
#define ZP_COLLIDER_COMPONENT

class zpColliderComponent : public zpComponent {
public:
	zpColliderComponent();
	virtual ~zpColliderComponent();

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