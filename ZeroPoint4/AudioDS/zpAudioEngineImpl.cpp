#include "zpAudioDS.h"
#include <dsound.h>

ZP_FORCE_INLINE DWORD __zpToDS( zpAudioSpeakerMode t )
{
    switch( t )
    {
    case ZP_AUDIO_SPEAKER_MODE_MONO:        return DSSPEAKER_MONO;
    case ZP_AUDIO_SPEAKER_MODE_HEADPHONE:    return DSSPEAKER_HEADPHONE;
    case ZP_AUDIO_SPEAKER_MODE_STEREO:        return DSSPEAKER_STEREO;
    case ZP_AUDIO_SPEAKER_MODE_51_SURROUND:    return DSSPEAKER_5POINT1_SURROUND;
    case ZP_AUDIO_SPEAKER_MODE_71_SURROUND:    return DSSPEAKER_7POINT1_SURROUND;
    default:                                return DSSPEAKER_STEREO;
    }
}

ZP_FORCE_INLINE LONG __zpNormalizeVolume( zp_float volume )
{
    LONG vol = zp_lerp< LONG >( DSBVOLUME_MIN, DSBVOLUME_MAX, volume );
    return vol;
}
ZP_FORCE_INLINE zp_float __zpUnNormalizeVolume( LONG volume )
{
    return (zp_float)( DSBVOLUME_MIN + ( DSBVOLUME_MAX - volume ) ) / (zp_float)DSBVOLUME_MIN;
}
ZP_FORCE_INLINE LONG __zpNormalizePan( zp_float pan )
{
    pan = zp_clamp( pan, -1.0f, 1.0f );
    return (LONG)( pan * DSBPAN_RIGHT );
}
ZP_FORCE_INLINE zp_float __zpUnNormalizePan( LONG pan )
{
    return (zp_float)pan / (zp_float)DSBPAN_RIGHT;
}

void zpAudioEngineImpl::create( zp_handle hWnd )
{
    HRESULT h;
    LPDIRECTSOUND dsound;
    h = DirectSoundCreate( ZP_NULL, &dsound, ZP_NULL );
    ZP_ASSERT( SUCCEEDED( h ), "" );
    
    h = dsound->SetCooperativeLevel( (HWND)hWnd, DSSCL_PRIORITY );
    ZP_ASSERT( SUCCEEDED( h ), "" );

    h = dsound->SetSpeakerConfig( __zpToDS( ZP_AUDIO_SPEAKER_MODE_STEREO ) );
    ZP_ASSERT( SUCCEEDED( h ), "" );

    DSBUFFERDESC desc;
    zp_zero_memory( &desc );
    desc.dwSize = sizeof( DSBUFFERDESC );
    desc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
    desc.dwBufferBytes = 0;
    desc.dwReserved = 0;
    desc.lpwfxFormat = ZP_NULL;
    desc.guid3DAlgorithm = GUID_NULL;

    LPDIRECTSOUNDBUFFER primary;
    h = dsound->CreateSoundBuffer( &desc, &primary, ZP_NULL );
    ZP_ASSERT( SUCCEEDED( h ), "" );

    WAVEFORMATEX format;
    zp_zero_memory( &format );
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nSamplesPerSec = 44100;
    format.wBitsPerSample = 16;
    format.nChannels = 2;
    format.nBlockAlign = (format.wBitsPerSample / 8) * format.nChannels;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
    format.cbSize = 0;

    h = primary->SetFormat( &format );
    ZP_ASSERT( SUCCEEDED( h ), "" );
    
    LPDIRECTSOUND3DLISTENER listener;
    h = primary->QueryInterface( IID_IDirectSound3DListener, (void**)&listener );
    ZP_ASSERT( SUCCEEDED( h ), "" );

    m_dsound = dsound;
    m_primaryBuffer = primary;
    m_listener = listener;
}
void zpAudioEngineImpl::destroy()
{
    if( m_primaryBuffer )
    {
        ( (LPDIRECTSOUNDBUFFER)m_primaryBuffer )->Release();
        m_primaryBuffer = ZP_NULL;
    }

    if( m_dsound )
    {
        ( (LPDIRECTSOUND)m_dsound )->Release();
        m_dsound = ZP_NULL;
    }
}

void zpAudioEngineImpl::setMasterVolume( zp_float volume )
{
    LPDIRECTSOUNDBUFFER soundBuffer = (LPDIRECTSOUNDBUFFER)m_primaryBuffer;
    soundBuffer->SetVolume( __zpNormalizeVolume( volume ) );
}
void zpAudioEngineImpl::getMasterVolume( zp_float& volume )
{
    LONG vol;
    LPDIRECTSOUNDBUFFER soundBuffer = (LPDIRECTSOUNDBUFFER)m_primaryBuffer;
    soundBuffer->GetVolume( &vol );

    volume = __zpUnNormalizeVolume( vol );
}

void zpAudioEngineImpl::setListenerPosition( zpVector4fParamF pos )
{
    zp_float p[4];
    zpMath::Vector4Store4( pos, p );

    LPDIRECTSOUND3DLISTENER listener = (LPDIRECTSOUND3DLISTENER)m_listener;
    listener->SetPosition( p[0], p[1], p[2], DS3D_DEFERRED );
    
    m_isDirty = true;
}
void zpAudioEngineImpl::setListenerVelocity( zpVector4fParamF vel )
{
    zp_float v[4];
    zpMath::Vector4Store4( vel, v );

    LPDIRECTSOUND3DLISTENER listener = (LPDIRECTSOUND3DLISTENER)m_listener;
    listener->SetVelocity( v[0], v[1], v[2], DS3D_DEFERRED );
    
    m_isDirty = true;
}
void zpAudioEngineImpl::setListenerOrientation( zpVector4fParamF forward, zpVector4fParamF up )
{
    zp_float f[4], u[4];
    zpMath::Vector4Store4( forward, f );
    zpMath::Vector4Store4( up, u );

    LPDIRECTSOUND3DLISTENER listener = (LPDIRECTSOUND3DLISTENER)m_listener;
    listener->SetOrientation( f[0], f[1], f[2], u[0], u[1], u[2], DS3D_DEFERRED );

    m_isDirty = true;
}
void zpAudioEngineImpl::setListenerRolloff( zp_float rolloff )
{
    LPDIRECTSOUND3DLISTENER listener = (LPDIRECTSOUND3DLISTENER)m_listener;
    listener->SetRolloffFactor( rolloff, DS3D_DEFERRED );

    m_isDirty = true;
}
void zpAudioEngineImpl::setListenerDistance( zp_float dist )
{
    LPDIRECTSOUND3DLISTENER listener = (LPDIRECTSOUND3DLISTENER)m_listener;
    listener->SetDistanceFactor( dist, DS3D_DEFERRED );

    m_isDirty = true;
}
void zpAudioEngineImpl::setListenerDoppler( zp_float doppler )
{
    LPDIRECTSOUND3DLISTENER listener = (LPDIRECTSOUND3DLISTENER)m_listener;
    listener->SetDopplerFactor( doppler, DS3D_DEFERRED );

    m_isDirty = true;
}

void zpAudioEngineImpl::getListenerPosition( zpVector4f& pos )
{
    D3DVECTOR p;

    LPDIRECTSOUND3DLISTENER listener = (LPDIRECTSOUND3DLISTENER)m_listener;
    listener->GetPosition( &p );

    pos = zpMath::Vector4( p.x, p.y, p.z, 1 );
}
void zpAudioEngineImpl::getListenerVelocity( zpVector4f& vel )
{
    D3DVECTOR v;

    LPDIRECTSOUND3DLISTENER listener = (LPDIRECTSOUND3DLISTENER)m_listener;
    listener->GetPosition( &v );

    vel = zpMath::Vector4( v.x, v.y, v.z, 0 );
}
void zpAudioEngineImpl::getListenerOrientation( zpVector4f& forward, zpVector4f& up )
{
    D3DVECTOR f, u;

    LPDIRECTSOUND3DLISTENER listener = (LPDIRECTSOUND3DLISTENER)m_listener;
    listener->GetOrientation( &f, &u );

    forward = zpMath::Vector4( f.x, f.y, f.z, 0 );
    up = zpMath::Vector4( u.x, u.y, u.z, 0 );
}
void zpAudioEngineImpl::getListenerRolloff( zp_float& rolloff )
{
    LPDIRECTSOUND3DLISTENER listener = (LPDIRECTSOUND3DLISTENER)m_listener;
    listener->GetRolloffFactor( &rolloff );
}
void zpAudioEngineImpl::getListenerDistance( zp_float& dist )
{
    LPDIRECTSOUND3DLISTENER listener = (LPDIRECTSOUND3DLISTENER)m_listener;
    listener->GetDistanceFactor( &dist );
}
void zpAudioEngineImpl::getListenerDoppler( zp_float& doppler )
{
    LPDIRECTSOUND3DLISTENER listener = (LPDIRECTSOUND3DLISTENER)m_listener;
    listener->GetDopplerFactor( &doppler );
}

zp_bool zpAudioEngineImpl::createSoundBuffer( zpAudioBuffer& buffer, zpAudioType type, zp_size_t bufferSize, zp_uint samplesPerSec, zp_uint bitsBerSample, zp_uint channels )
{
    LPDIRECTSOUND dsound = (LPDIRECTSOUND)m_dsound;
    
    HRESULT h;
    WAVEFORMATEX format;
    zp_zero_memory( &format );
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nSamplesPerSec = samplesPerSec;
    format.wBitsPerSample = bitsBerSample;
    format.nChannels = channels;
    format.nBlockAlign = (format.wBitsPerSample / 8) * format.nChannels;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
    format.cbSize = 0;

    DSBUFFERDESC desc;
    zp_zero_memory( &desc );
    desc.dwSize = sizeof( DSBUFFERDESC );
    desc.dwFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
    desc.dwBufferBytes = zp_clamp< DWORD >( (DWORD)bufferSize, DSBSIZE_MIN, DSBSIZE_MAX );
    desc.dwReserved = 0;
    desc.lpwfxFormat = &format;
    desc.guid3DAlgorithm = GUID_NULL;

    switch( type )
    {
    case ZP_AUDIO_TYPE_2D:
        break;
    case ZP_AUDIO_TYPE_3D:
        desc.guid3DAlgorithm = DS3DALG_HRTF_LIGHT;
        desc.dwFlags |= DSBCAPS_CTRL3D;
        break;
    }

    LPDIRECTSOUNDBUFFER soundBuffer;
    h = dsound->CreateSoundBuffer( &desc, &soundBuffer, ZP_NULL );
    ZP_ASSERT( SUCCEEDED( h ), "" );

    buffer.bufferSize = desc.dwBufferBytes;
    buffer.soundBuffer = soundBuffer;

    switch( type )
    {
    case ZP_AUDIO_TYPE_2D:
        buffer.soundBuffer3D = ZP_NULL;
        break;
    case ZP_AUDIO_TYPE_3D:
        soundBuffer->QueryInterface( IID_IDirectSound3DBuffer, (void**)&buffer.soundBuffer3D );
        break;
    }

    return SUCCEEDED( h );
}
void zpAudioEngineImpl::cloneSoundBuffer( const zpAudioBuffer& buffer, zpAudioBuffer& copyBuffer )
{
    LPDIRECTSOUND dsound = (LPDIRECTSOUND)m_dsound;
    LPDIRECTSOUNDBUFFER soundBuffer = (LPDIRECTSOUNDBUFFER)buffer.soundBuffer;
    LPDIRECTSOUNDBUFFER dupBuffer;
    HRESULT h;

    h = dsound->DuplicateSoundBuffer( soundBuffer, &dupBuffer );
    ZP_ASSERT( SUCCEEDED( h ), "" );

    // to fix duplicate buffers not playing at the correct volume
    LONG vol;
    soundBuffer->GetVolume( &vol );
    vol--;
    dupBuffer->SetVolume( vol );

    copyBuffer.soundBuffer = dupBuffer;
    copyBuffer.soundBuffer3D = ZP_NULL;
    copyBuffer.soundFilenameHash = buffer.soundFilenameHash;

    if( buffer.soundBuffer3D != ZP_NULL )
    {
        dupBuffer->QueryInterface( IID_IDirectSound3DBuffer, (void**)&copyBuffer.soundBuffer3D );
    }
}
void zpAudioEngineImpl::destroySoundBuffer( zpAudioBuffer& buffer )
{
    LPDIRECTSOUNDBUFFER soundBuffer = (LPDIRECTSOUNDBUFFER)buffer.soundBuffer;
    ZP_SAFE_RELEASE( soundBuffer );

    buffer.soundBuffer = ZP_NULL;
    buffer.soundBuffer3D = ZP_NULL;
}

void zpAudioEngineImpl::fillSoundBuffer( const zpAudioBuffer& buffer, const void* data, zp_size_t size )
{
    LPDIRECTSOUNDBUFFER soundBuffer = (LPDIRECTSOUNDBUFFER)buffer.soundBuffer;
    
    void *audioPtr1 = ZP_NULL, *audioPtr2 = ZP_NULL;
    DWORD audioSize1 = 0, audioSize2 = 0;
    HRESULT h;
    
    h = soundBuffer->Lock( 0, (DWORD)size, &audioPtr1, &audioSize1, &audioPtr2, &audioSize2, 0 );
    ZP_ASSERT( SUCCEEDED( h ), "" );

    if( audioPtr2 == ZP_NULL )
    {
        zp_memcpy( audioPtr1, audioSize1, data, size );
    }
    else
    {
        zp_byte* d = (zp_byte*)data;
        zp_memcpy( audioPtr1, audioSize1, d, audioSize1 );
        zp_memcpy( audioPtr2, audioSize2, d + audioSize1, audioSize2 );
    }

    h = soundBuffer->Unlock( audioPtr1, audioSize1, audioPtr2, audioSize2 );
    ZP_ASSERT( SUCCEEDED( h ), "" );
}

void zpAudioEngineImpl::setSoundBufferVolume( const zpAudioBuffer& buffer, zp_float volume )
{
    LPDIRECTSOUNDBUFFER soundBuffer = (LPDIRECTSOUNDBUFFER)buffer.soundBuffer;
    soundBuffer->SetVolume( __zpNormalizeVolume( volume ) );
}
void zpAudioEngineImpl::setSoundBufferPan( const zpAudioBuffer& buffer, zp_float pan )
{
    LPDIRECTSOUNDBUFFER soundBuffer = (LPDIRECTSOUNDBUFFER)buffer.soundBuffer;
    soundBuffer->SetPan( __zpNormalizePan( pan ) );
}
void zpAudioEngineImpl::setSoundBufferPosition( const zpAudioBuffer& buffer, zpVector4fParamF pos )
{
    zp_float p[4];
    zpMath::Vector4Store4( pos, p );

    LPDIRECTSOUND3DBUFFER soundBuffer = (LPDIRECTSOUND3DBUFFER)buffer.soundBuffer3D;
    soundBuffer->SetPosition( p[0], p[1], p[2], DS3D_DEFERRED );

    m_isDirty = true;
}
void zpAudioEngineImpl::setSoundBufferVelocity( const zpAudioBuffer& buffer, zpVector4fParamF vel )
{
    zp_float v[4];
    zpMath::Vector4Store4( vel, v );

    LPDIRECTSOUND3DBUFFER soundBuffer = (LPDIRECTSOUND3DBUFFER)buffer.soundBuffer3D;
    soundBuffer->SetVelocity( v[0], v[1], v[2], DS3D_DEFERRED );

    m_isDirty = true;
}
void zpAudioEngineImpl::setSoundBufferDistances( const zpAudioBuffer& buffer, zp_float minDistance, zp_float maxDistance )
{
    LPDIRECTSOUND3DBUFFER soundBuffer = (LPDIRECTSOUND3DBUFFER)buffer.soundBuffer3D;
    soundBuffer->SetMinDistance( minDistance, DS3D_DEFERRED );
    soundBuffer->SetMaxDistance( maxDistance, DS3D_DEFERRED );

    m_isDirty = true;
}

void zpAudioEngineImpl::getSoundBufferVolume( const zpAudioBuffer& buffer, zp_float& volume )
{
    LONG vol;
    LPDIRECTSOUNDBUFFER soundBuffer = (LPDIRECTSOUNDBUFFER)buffer.soundBuffer;
    soundBuffer->GetVolume( &vol );

    volume = __zpUnNormalizeVolume( vol );
}
void zpAudioEngineImpl::getSoundBufferPan( const zpAudioBuffer& buffer, zp_float& pan )
{
    LONG p;
    LPDIRECTSOUNDBUFFER soundBuffer = (LPDIRECTSOUNDBUFFER)buffer.soundBuffer;
    soundBuffer->GetPan( &p );

    pan = __zpUnNormalizePan( p );
}
void zpAudioEngineImpl::getSoundBufferPosition( const zpAudioBuffer& buffer, zpVector4f& pos )
{
    D3DVECTOR p;
    LPDIRECTSOUND3DBUFFER soundBuffer = (LPDIRECTSOUND3DBUFFER)buffer.soundBuffer3D;
    soundBuffer->GetPosition( &p );

    pos = zpMath::Vector4( p.x, p.y, p.z, 1 );
}
void zpAudioEngineImpl::getSoundBufferVelocity( const zpAudioBuffer& buffer, zpVector4f& vel )
{
    D3DVECTOR v;
    LPDIRECTSOUND3DBUFFER soundBuffer = (LPDIRECTSOUND3DBUFFER)buffer.soundBuffer3D;
    soundBuffer->GetVelocity( &v );

    vel = zpMath::Vector4( v.x, v.y, v.z, 0 );
}
void zpAudioEngineImpl::getSoundBufferDistances( const zpAudioBuffer& buffer, zp_float& minDistance, zp_float& maxDistance )
{
    LPDIRECTSOUND3DBUFFER soundBuffer = (LPDIRECTSOUND3DBUFFER)buffer.soundBuffer3D;
    soundBuffer->GetMinDistance( &minDistance );
    soundBuffer->GetMaxDistance( &maxDistance );
}

void zpAudioEngineImpl::playSoundBuffer( const zpAudioBuffer& buffer, zp_bool loop )
{
    LPDIRECTSOUNDBUFFER soundBuffer = (LPDIRECTSOUNDBUFFER)buffer.soundBuffer;
    soundBuffer->Play( 0, 0, loop ? DSBPLAY_LOOPING : 0 );
}
void zpAudioEngineImpl::stopSoundBuffer( const zpAudioBuffer& buffer )
{
    LPDIRECTSOUNDBUFFER soundBuffer = (LPDIRECTSOUNDBUFFER)buffer.soundBuffer;
    soundBuffer->Stop();
}
zp_bool zpAudioEngineImpl::isSoundBufferPlaying( const zpAudioBuffer& buffer )
{
    DWORD status;
    LPDIRECTSOUNDBUFFER soundBuffer = (LPDIRECTSOUNDBUFFER)buffer.soundBuffer;
    soundBuffer->GetStatus( &status );

    return ( status & DSBSTATUS_PLAYING ) == DSBSTATUS_PLAYING;
}

void zpAudioEngineImpl::getCurrentPlayWritePosition( const zpAudioBuffer& buffer, zp_uint& playPosition, zp_size_t& writePosition )
{
    LPDIRECTSOUNDBUFFER soundBuffer = (LPDIRECTSOUNDBUFFER)buffer.soundBuffer;
    DWORD cp, cw;

    soundBuffer->GetCurrentPosition( &cp, &cw );

    playPosition = cp;
    writePosition = cw;
}

void zpAudioEngineImpl::update()
{
    if( m_isDirty )
    {
        m_isDirty = false;

        LPDIRECTSOUND3DLISTENER listener = (LPDIRECTSOUND3DLISTENER)m_listener;
        listener->CommitDeferredSettings();
    }
}
