#pragma once
#ifndef ZP_PHANTOM_COMPONENT
#define ZP_PHANTOM_COMPONENT

class zpPhantomComponent : public zpComponent {
public:
	zpPhantomComponent();
	virtual ~zpPhantomComponent();

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