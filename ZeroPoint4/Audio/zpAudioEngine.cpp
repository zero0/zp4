#include "zpAudio.h"
#include "AudioDS\zpAudioDS.h"

zpAudioEngine* zpAudioEngine::getInstance()
{
	static zpAudioEngine instance;
	return &instance;
}

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

void zpAudioEngine::setListenerPosition( const zpVector4f& pos )
{
	m_engine->setListenerPosition( pos );
}
void zpAudioEngine::setListenerVelocity( const zpVector4f& vel )
{
	m_engine->setListenerVelocity( vel );
}
void zpAudioEngine::setListenerOrientation( const zpVector4f& forward, const zpVector4f& up )
{
	m_engine->setListenerOrientation( forward, up );
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

zp_bool zpAudioEngine::createSoundBuffer( zpAudioBuffer& buffer, zpAudioType type, zp_uint bufferSize, zp_uint samplesPerSec, zp_uint bitsBerSample, zp_uint channels )
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

void zpAudioEngine::fillSoundBuffer( const zpAudioBuffer& buffer, const void* data, zp_uint size )
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
void zpAudioEngine::setPosition( const zpAudioBuffer& buffer, const zpVector4f& pos )
{
	m_engine->setSoundBufferPosition( buffer, pos );
}
void zpAudioEngine::setVelocity( const zpAudioBuffer& buffer, const zpVector4f& vel )
{
	m_engine->setSoundBufferVelocity( buffer, vel );
}
void zpAudioEngine::setDistances( const zpAudioBuffer& buffer, zp_float minDistance, zp_float maxDistance )
{
	m_engine->setSoundBufferDistances( buffer, minDistance, maxDistance );
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

void zpAudioEngine::update()
{
	m_engine->update();
}

