#pragma once
#ifndef ZP_AUDIO_LISTENER_COMPONENT_H
#define ZP_AUDIO_LISTENER_COMPONENT_H

class zpAudioListenerComponent : public zpComponent {
public:
	zpAudioListenerComponent();
	virtual ~zpAudioListenerComponent();

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
	zp_uint m_listener;
	zpAudioManager* m_manager;

	zpVector4f m_oldPosition;
};

#endif