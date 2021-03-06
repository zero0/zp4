#include "zpAudio.h"
#include "AudioDS\zpAudioDS.h"

zpAudioEngine::zpAudioEngine()
    : m_engine( new zpAudioEngineImpl )
{}
zpAudioEngine::~zpAudioEngine()
{
    ZP_SAFE_DELETE( m_engine );
}

void zpAudioEngine::create( zp_handle hWnd )
{
    m_engine->create( hWnd );
}
void zpAudioEngine::destroy()
{
    m_engine->destroy();
}

void zpAudioEngine::setMasterVolume( zp_float volume )
{
    m_engine->setMasterVolume( volume );
}
void zpAudioEngine::getMasterVolume( zp_float& volume )
{
    m_engine->getMasterVolume( volume );
}

void zpAudioEngine::setListenerPosition( zpVector4fParamF pos )
{
    m_engine->setListenerPosition( pos );
}
void zpAudioEngine::setListenerVelocity( zpVector4fParamF vel )
{
    m_engine->setListenerVelocity( vel );
}
void zpAudioEngine::setListenerOrientation( zpVector4fParamF forward, zpVector4fParamF up )
{
    m_engine->setListenerOrientation( forward, up );
}
void zpAudioEngine::setListenerRolloff( zp_float rolloff )
{
    m_engine->setListenerRolloff( rolloff );
}
void zpAudioEngine::setListenerDistance( zp_float dist )
{
    m_engine->setListenerDistance( dist );
}
void zpAudioEngine::setListenerDoppler( zp_float dopper )
{
    m_engine->setListenerDoppler( dopper );
}

void zpAudioEngine::getListenerPosition( zpVector4f& pos )
{
    m_engine->getListenerPosition( pos );
}
void zpAudioEngine::getListenerVelocity( zpVector4f& vel )
{
    m_engine->getListenerVelocity( vel );
}
void zpAudioEngine::getListenerOrientation( zpVector4f& forward, zpVector4f& up )
{
    m_engine->getListenerOrientation( forward, up );
}
void zpAudioEngine::getListenerRolloff( zp_float& rolloff )
{
    m_engine->getListenerRolloff( rolloff );
}
void zpAudioEngine::getListenerDistance( zp_float& dist )
{
    m_engine->getListenerDistance( dist );
}
void zpAudioEngine::getListenerDoppler( zp_float& doppler )
{
    m_engine->getListenerDoppler( doppler );
}

zp_bool zpAudioEngine::createSoundBuffer( zpAudioBuffer& buffer, zpAudioType type, zp_size_t bufferSize, zp_uint samplesPerSec, zp_uint bitsBerSample, zp_uint channels )
{
    return m_engine->createSoundBuffer( buffer, type, bufferSize, samplesPerSec, bitsBerSample, channels );
}
void zpAudioEngine::cloneSoundBuffer( const zpAudioBuffer& buffer, zpAudioBuffer& copyBuffer )
{
    m_engine->cloneSoundBuffer( buffer, copyBuffer );
}
void zpAudioEngine::destroySoundBuffer( zpAudioBuffer& buffer )
{
    m_engine->destroySoundBuffer( buffer );
}

void zpAudioEngine::fillSoundBuffer( const zpAudioBuffer& buffer, const void* data, zp_size_t size )
{
    m_engine->fillSoundBuffer( buffer, data, size );
}

void zpAudioEngine::setVolume( const zpAudioBuffer& buffer, zp_float volume )
{
    m_engine->setSoundBufferVolume( buffer, volume );
}
void zpAudioEngine::setPan( const zpAudioBuffer& buffer, zp_float pan )
{
    m_engine->setSoundBufferPan( buffer, pan );
}
void zpAudioEngine::setPosition( const zpAudioBuffer& buffer, zpVector4fParamF pos )
{
    m_engine->setSoundBufferPosition( buffer, pos );
}
void zpAudioEngine::setVelocity( const zpAudioBuffer& buffer, zpVector4fParamF vel )
{
    m_engine->setSoundBufferVelocity( buffer, vel );
}
void zpAudioEngine::setDistances( const zpAudioBuffer& buffer, zp_float minDistance, zp_float maxDistance )
{
    m_engine->setSoundBufferDistances( buffer, minDistance, maxDistance );
}

void zpAudioEngine::getVolume( const zpAudioBuffer& buffer, zp_float& volume )
{
    m_engine->getSoundBufferVolume( buffer, volume );
}
void zpAudioEngine::getPan( const zpAudioBuffer& buffer, zp_float& pan )
{
    m_engine->getSoundBufferPan( buffer, pan );
}
void zpAudioEngine::getPosition( const zpAudioBuffer& buffer, zpVector4f& pos )
{
    m_engine->getSoundBufferPosition( buffer, pos );
}
void zpAudioEngine::getVelocity( const zpAudioBuffer& buffer, zpVector4f& vel )
{
    m_engine->getSoundBufferVelocity( buffer, vel );
}
void zpAudioEngine::getDistances( const zpAudioBuffer& buffer, zp_float& minDistance, zp_float& maxDistance )
{
    m_engine->getSoundBufferDistances( buffer, minDistance, maxDistance );
}

void zpAudioEngine::play( const zpAudioBuffer& buffer, zp_bool repeat )
{
    m_engine->playSoundBuffer( buffer, repeat );
}
void zpAudioEngine::stop( const zpAudioBuffer& buffer )
{
    m_engine->stopSoundBuffer( buffer );
}
zp_bool zpAudioEngine::isPlaying( const zpAudioBuffer& buffer )
{
    return m_engine->isSoundBufferPlaying( buffer );
}

void zpAudioEngine::getCurrentPlayWritePosition( const zpAudioBuffer& buffer, zp_uint& playPosition, zp_size_t& writePosition )
{
    m_engine->getCurrentPlayWritePosition( buffer, playPosition, writePosition );
}

void zpAudioEngine::update()
{
    m_engine->update();
}

