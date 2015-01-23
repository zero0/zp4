#pragma once
#ifndef ZP_AUDIO_ENGINE
#define ZP_AUDIO_ENGINE

class zpAudioEngineImpl;

class zpAudioEngine
{
	ZP_NON_COPYABLE( zpAudioEngine );

public:
	zpAudioEngine();
	~zpAudioEngine();

	void create( zp_handle hWnd );
	void destroy();

	void setMasterVolume( zp_float volume );
	void getMasterVolume( zp_float& volume );

	void setListenerPosition( const zpVector4f& pos );
	void setListenerVelocity( const zpVector4f& vel );
	void setListenerOrientation( const zpVector4f& forward, const zpVector4f& up );
	void setListenerRolloff( zp_float rolloff );
	void setListenerDistance( zp_float dist );
	void setListenerDoppler( zp_float dopper );

	void getListenerPosition( zpVector4f& pos );
	void getListenerVelocity( zpVector4f& vel );
	void getListenerOrientation( zpVector4f& forward, zpVector4f& up );
	void getListenerRolloff( zp_float& rolloff );
	void getListenerDistance( zp_float& dist );
	void getListenerDoppler( zp_float& doppler );

	zp_bool createSoundBuffer( zpAudioBuffer& buffer, zpAudioType type, zp_uint bufferSize, zp_uint samplesPerSec, zp_uint bitsBerSample, zp_uint channels );
	void cloneSoundBuffer( const zpAudioBuffer& buffer, zpAudioBuffer& copyBuffer );
	void destroySoundBuffer( zpAudioBuffer& buffer );

	void fillSoundBuffer( const zpAudioBuffer& buffer, const void* data, zp_uint size );

	void setVolume( const zpAudioBuffer& buffer, zp_float volume );
	void setPan( const zpAudioBuffer& buffer, zp_float pan );
	void setPosition( const zpAudioBuffer& buffer, const zpVector4f& pos );
	void setVelocity( const zpAudioBuffer& buffer, const zpVector4f& vel );
	void setDistances( const zpAudioBuffer& buffer, zp_float minDistance, zp_float maxDistance );

	void getVolume( const zpAudioBuffer& buffer, zp_float& volume );
	void getPan( const zpAudioBuffer& buffer, zp_float& pan );
	void getPosition( const zpAudioBuffer& buffer, zpVector4f& pos );
	void getVelocity( const zpAudioBuffer& buffer, zpVector4f& vel );
	void getDistances( const zpAudioBuffer& buffer, zp_float& minDistance, zp_float& maxDistance );

	void play( const zpAudioBuffer& buffer, zp_bool repeat = false );
	void stop( const zpAudioBuffer& buffer );
	zp_bool isPlaying( const zpAudioBuffer& buffer );

	void getCurrentPlayWritePosition( const zpAudioBuffer& buffer, zp_uint& playPosition, zp_uint& writePosition );

	void update();

private:
	zpAudioEngineImpl* m_engine;
};

#endif
