#pragma once
#ifndef ZP_AUDIO_EMITTER_COMPONENT_H
#define ZP_AUDIO_EMITTER_COMPONENT_H

class zpAudioEmitterComponent : public zpComponent {
public:
	zpAudioEmitterComponent();
	virtual ~zpAudioEmitterComponent();

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