#pragma once
#ifndef ZP_AUDIO_ENGINE_IMPL_H
#define ZP_AUDIO_ENGINE_IMPL_H

class zpAudioEngineImpl
{
public:
	void create( zp_handle hWnd );
	void destroy();

	void setMasterVolume( zp_float volume );
	void getMasterVolume( zp_float& volume );

	void setListenerPosition( const zpVector4f& pos );
	void setListenerVelocity( const zpVector4f& vel );
	void setListenerOrientation( const zpVector4f& forward, const zpVector4f& up );

	void getListenerPosition( zpVector4f& pos );
	void getListenerVelocity( zpVector4f& vel );
	void getListenerOrientation( zpVector4f& forward, zpVector4f& up );

	zp_bool createSoundBuffer( zpAudioBuffer& buffer, zpAudioType type, zp_uint bufferSize, zp_uint samplesPerSec, zp_uint bitsBerSample, zp_uint channels );
	void cloneSoundBuffer( const zpAudioBuffer& buffer, zpAudioBuffer& copyBuffer );
	void destroySoundBuffer( zpAudioBuffer& buffer );

	void fillSoundBuffer( const zpAudioBuffer& buffer, const void* data, zp_uint size );

	void setSoundBufferVolume( const zpAudioBuffer& buffer, zp_float volume );
	void setSoundBufferPan( const zpAudioBuffer& buffer, zp_float pan );
	void setSoundBufferPosition( const zpAudioBuffer& buffer, const zpVector4f& pos );
	void setSoundBufferVelocity( const zpAudioBuffer& buffer, const zpVector4f& vel );
	void setSoundBufferDistances( const zpAudioBuffer& buffer, zp_float minDistance, zp_float maxDistance );

	void getSoundBufferVolume( const zpAudioBuffer& buffer, zp_float& volume );
	void getSoundBufferPan( const zpAudioBuffer& buffer, zp_float& pan );
	void getSoundBufferPosition( const zpAudioBuffer& buffer, zpVector4f& pos );
	void getSoundBufferVelocity( const zpAudioBuffer& buffer, zpVector4f& vel );
	void getSoundBufferDistances( const zpAudioBuffer& buffer, zp_float& minDistance, zp_float& maxDistance );

	void playSoundBuffer( const zpAudioBuffer& buffer, zp_bool loop );
	void stopSoundBuffer( const zpAudioBuffer& buffer );
	zp_bool isSoundBufferPlaying( const zpAudioBuffer& buffer );

	void update();

private:
	zp_handle m_dsound;
	zp_handle m_primaryBuffer;
	zp_handle m_listener;

	zp_bool m_isDirty;
};

#endif
