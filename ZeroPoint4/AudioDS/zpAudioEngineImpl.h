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

    void setListenerPosition( zpVector4fParamF pos );
    void setListenerVelocity( zpVector4fParamF vel );
    void setListenerOrientation( zpVector4fParamF forward, zpVector4fParamF up );
    void setListenerRolloff( zp_float rolloff );
    void setListenerDistance( zp_float dist );
    void setListenerDoppler( zp_float dopper );

    void getListenerPosition( zpVector4f& pos );
    void getListenerVelocity( zpVector4f& vel );
    void getListenerOrientation( zpVector4f& forward, zpVector4f& up );
    void getListenerRolloff( zp_float& rolloff );
    void getListenerDistance( zp_float& dist );
    void getListenerDoppler( zp_float& doppler );


    zp_bool createSoundBuffer( zpAudioBuffer& buffer, zpAudioType type, zp_size_t bufferSize, zp_uint samplesPerSec, zp_uint bitsBerSample, zp_uint channels );
    void cloneSoundBuffer( const zpAudioBuffer& buffer, zpAudioBuffer& copyBuffer );
    void destroySoundBuffer( zpAudioBuffer& buffer );

    void fillSoundBuffer( const zpAudioBuffer& buffer, const void* data, zp_size_t size );

    void setSoundBufferVolume( const zpAudioBuffer& buffer, zp_float volume );
    void setSoundBufferPan( const zpAudioBuffer& buffer, zp_float pan );
    void setSoundBufferPosition( const zpAudioBuffer& buffer, zpVector4fParamF pos );
    void setSoundBufferVelocity( const zpAudioBuffer& buffer, zpVector4fParamF vel );
    void setSoundBufferDistances( const zpAudioBuffer& buffer, zp_float minDistance, zp_float maxDistance );

    void getSoundBufferVolume( const zpAudioBuffer& buffer, zp_float& volume );
    void getSoundBufferPan( const zpAudioBuffer& buffer, zp_float& pan );
    void getSoundBufferPosition( const zpAudioBuffer& buffer, zpVector4f& pos );
    void getSoundBufferVelocity( const zpAudioBuffer& buffer, zpVector4f& vel );
    void getSoundBufferDistances( const zpAudioBuffer& buffer, zp_float& minDistance, zp_float& maxDistance );

    void playSoundBuffer( const zpAudioBuffer& buffer, zp_bool loop );
    void stopSoundBuffer( const zpAudioBuffer& buffer );
    zp_bool isSoundBufferPlaying( const zpAudioBuffer& buffer );

    void getCurrentPlayWritePosition( const zpAudioBuffer& buffer, zp_uint& playPosition, zp_size_t& writePosition );

    void update();

private:
    zp_handle m_dsound;
    zp_handle m_primaryBuffer;
    zp_handle m_listener;

    zp_bool m_isDirty;
};

#endif
