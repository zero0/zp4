#pragma once
#ifndef ZP_AUDIO_MANAGER_H
#define ZP_AUDIO_MANAGER_H

class zpAudioManager : public zpGameManager {
public:
	zpAudioManager();
	virtual ~zpAudioManager();

	void setSpeakerMode( zpAudioSpeakerMode mode );
	zpAudioSpeakerMode getSpeakerMode() const;

	void setNumChannels( zp_uint channels );
	zp_uint getNumChannels() const;

	void setVolume( zpAudioChannelGroup channel, zp_float volume );
	zp_float getVolume( zpAudioChannelGroup channel ) const;

	void mute( zpAudioChannelGroup channel, zp_bool muted );
	zp_bool isMuted( zpAudioChannelGroup channel ) const;

	void pause( zpAudioChannelGroup channel, zp_bool paused );
	zp_bool isPaused( zpAudioChannelGroup channel ) const;

	void setNumListeners( zp_uint numListeners );
	zp_uint getNumListeners() const;

	//void set3DSettings( zp_float dopperScale, zp_float distanceFactor, zp_float rolloffScale );
	//void set3DListener( int listener, const Point3f& position, const Vector3f& velocity, const Vector3f& forward, const Vector3f& up );

	//void stop( zpAudioChannelGroup channel );
	//void play( zpAudioInstance* sound );
	void play( zpResourceInstance<zpAudioResource>* sound );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	void receiveMessage( const zpMessage& message );

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zpAudioSpeakerMode m_speakerMode;

	zp_uint m_numChannels;
	zp_uint m_numListeners;
	zp_float m_dopplerScale;
	zp_float m_distanceFactor;
	zp_float m_rolloffScale;

	void* m_channelGroups[4];
};

#endif