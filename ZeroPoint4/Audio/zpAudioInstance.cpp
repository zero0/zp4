#include "zpAudio.h"
#include "zpFMOD.h"
#include "fmod.hpp"

FMOD_RESULT F_CALLBACK __fmodChannelCallback( FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2 ) {
	FMOD::Channel *chan = (FMOD::Channel*)channel;

	switch( type ) {
	case FMOD_CHANNEL_CALLBACKTYPE_END:
		zpResourceInstance<zpAudioResource>* sound = ZP_NULL;
		chan->getUserData( (void**)&sound );
		if( sound ) {
			sound->setChannel( ZP_NULL );
		}
		break;
	}
	
	return FMOD_OK;
}

void zpResourceInstance<zpAudioResource>::copy( const zpResourceInstance<zpAudioResource>& copy ) {
	m_channel = copy.m_channel;
	m_minDistance = copy.m_minDistance;
	m_maxDistance = copy.m_maxDistance;

	m_position = copy.m_position;
	m_velocity = copy.m_velocity;
	m_coneOrientation = copy.m_coneOrientation;
}

zp_bool zpResourceInstance<zpAudioResource>::isPlaying() const {
	if( m_channel ) {
		zp_bool playing = false;
		( (FMOD::Channel*)m_channel )->isPlaying( &playing );
		return playing;
	}
	return false;
}

void zpResourceInstance<zpAudioResource>::setPosition( const zpVector4f& position ) {
	m_position = position;
}
const zpVector4f& zpResourceInstance<zpAudioResource>::getPosition() const {
	return m_position;
}

void zpResourceInstance<zpAudioResource>::setVelocity( const zpVector4f& velocity ) {
	m_velocity = velocity;
}
const zpVector4f& zpResourceInstance<zpAudioResource>::getVelocity() const {
	return m_velocity;
}

void zpResourceInstance<zpAudioResource>::setConeOrientation( const zpVector4f& orientation ) {
	m_coneOrientation = orientation;
}
const zpVector4f& zpResourceInstance<zpAudioResource>::getConeOrientation() const {
	return m_coneOrientation;
}

void zpResourceInstance<zpAudioResource>::setVolume( zp_float volume ) {
	if( m_channel ) ( (FMOD::Channel*)m_channel )->setVolume( volume );
}
zp_float zpResourceInstance<zpAudioResource>::getVolume() const {
	zp_float volume = -1.f;
	if( m_channel ) ( (FMOD::Channel*)m_channel )->getVolume( &volume );
	return volume;
}

void zpResourceInstance<zpAudioResource>::setMinMaxDistance( zp_float minDistance, zp_float maxDistance ) {
	m_minDistance = minDistance;
	m_maxDistance = maxDistance;
}
zp_float zpResourceInstance<zpAudioResource>::getMinDistance() const {
	return m_minDistance;
}
zp_float zpResourceInstance<zpAudioResource>::getMaxDistance() const {
	return m_maxDistance;
}

void zpResourceInstance<zpAudioResource>::update() {
	if( m_channel && m_resource && m_resource->is3DSound() ) {
		FMOD::Channel* channel = (FMOD::Channel*)m_channel;

		zp_float pos[ 3 ];
		zp_float vel[ 3 ];
		m_position.store3( pos );
		m_velocity.store3( vel );

		channel->set3DAttributes( (const FMOD_VECTOR*)pos, (const FMOD_VECTOR*)vel );
		channel->set3DMinMaxDistance( m_minDistance, m_maxDistance );
	}
}
void zpResourceInstance<zpAudioResource>::stop() {
	if( m_channel && isPlaying() ) ( (FMOD::Channel*)m_channel )->stop();
}

void zpResourceInstance<zpAudioResource>::setChannel( void* channel ) {
	m_channel = channel;
	if( m_channel ) {
		FMOD::Channel* channel = (FMOD::Channel*)m_channel;
		channel->setUserData( this );
		channel->setCallback( __fmodChannelCallback );
	}
}