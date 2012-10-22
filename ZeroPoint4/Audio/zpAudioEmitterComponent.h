#pragma once
#ifndef ZP_AUDIO_EMITTER_COMPONENT_H
#define ZP_AUDIO_EMITTER_COMPONENT_H

class zpAudioEmitterComponent : public zpComponent {
public:
	zpAudioEmitterComponent();
	virtual ~zpAudioEmitterComponent();

	void playSound( const zpString& soundAlias );
	void stopSound( const zpString& soundAlias );
	void stopAllSounds();

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
	zp_bool m_isMoving;
	zp_bool m_isPlaying;

	zpAudioManager* m_manager;

	zpVector4f m_oldPosition;

	zpHashMap<zpString, zpResourceInstance<zpAudioResource> > m_audioInstances;
};

#endif