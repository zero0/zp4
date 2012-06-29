#include "zpAudio.h"
#include "zpFMOD.h"
#include "fmod.hpp"

FMOD_RESULT F_CALLBACK __fmodChannelCallback( FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2 ) {
	FMOD::Channel *chan = (FMOD::Channel*)channel;

	switch( type ) {
	case FMOD_CHANNEL_CALLBACKTYPE_END:
		zpAudioInstance* sound = ZP_NULL;
		chan->getUserData( (void**)&sound );
		if( sound ) {
			sound->setChannel( ZP_NULL );
		}
		break;
	}
	
	return FMOD_OK;
}

zpAudioInstance::zpAudioInstance() :
	m_resource( ZP_NULL )
{}
zpAudioInstance::zpAudioInstance( zpAudioResource* resource ) :
	m_resource( resource )
{}
zpAudioInstance::~zpAudioInstance() {
	m_resource = ZP_NULL;
}

zpAudioResource* zpAudioInstance::getAudioResource() const {
	return m_resource;
}

zp_bool zpAudioInstance::isPlaying() const {
	if( m_channel ) {
		zp_bool playing = false;
		( (FMOD::Channel*)m_channel )->isPlaying( &playing );
		return playing;
	}
	return false;
}

void zpAudioInstance::setPosition( const zpVector4f& position ) {
	m_position = position;
}
const zpVector4f& zpAudioInstance::getPosition() const {
	return m_position;
}

void zpAudioInstance::setVelocity( const zpVector4f& velocity ) {
	m_velocity = velocity;
}
const zpVector4f& zpAudioInstance::getVelocity() const {
	return m_velocity;
}

void zpAudioInstance::setConeOrientation( const zpVector4f& orientation ) {
	m_coneOrientation = orientation;
}
const zpVector4f& zpAudioInstance::getConeOrientation() const {
	return m_coneOrientation;
}

void zpAudioInstance::setVolume( zp_float volume ) {
	if( m_channel ) ( (FMOD::Channel*)m_channel )->setVolume( volume );
}
zp_float zpAudioInstance::getVolume() const {
	zp_float volume = -1.f;
	if( m_channel ) ( (FMOD::Channel*)m_channel )->getVolume( &volume );
	return volume;
}

void zpAudioInstance::setMinMaxDistance( zp_float minDistance, zp_float maxDistance ) {
	m_minDistance = minDistance;
	m_maxDistance = maxDistance;
}
zp_float zpAudioInstance::getMinDistance() const {
	return m_minDistance;
}
zp_float zpAudioInstance::getMaxDistance() const {
	return m_maxDistance;
}
	
void zpAudioInstance::update() {
	if( m_channel && m_resource && m_resource->is3DSound() ) {
		FMOD::Channel* channel = (FMOD::Channel*)m_channel;

		zp_float pos[ 3 ];
		zp_float vel[ 3 ];
		m_position.storef3( pos );
		m_velocity.storef3( vel );

		channel->set3DAttributes( (const FMOD_VECTOR*)pos, (const FMOD_VECTOR*)vel );
		channel->set3DMinMaxDistance( m_minDistance, m_maxDistance );
	}
}
void zpAudioInstance::stop() {
	if( m_channel && isPlaying() ) ( (FMOD::Channel*)m_channel )->stop();
}

void zpAudioInstance::setChannel( void* channel ) {
	m_channel = channel;
	if( m_channel ) {
		FMOD::Channel* channel = (FMOD::Channel*)m_channel;
		channel->setUserData( this );
		channel->setCallback( __fmodChannelCallback );
	}
}