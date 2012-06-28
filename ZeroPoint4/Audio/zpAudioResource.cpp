#include "zpAudio.h"
#include "zpFMOD.h"
#include "fmod.hpp"

#define FR( r )	if( r != FMOD_OK ) { return false; }

zpAudioResource::zpAudioResource() :
m_isStreaming( false ),
	m_isLooping( false ),
	m_sound( ZP_NULL ),
	m_group( ZP_AUDIO_CHANNEL_GROUP_SFX ),
	m_loopFrom( ZP_AUDIO_LOOP_POINT_NONE ),
	m_loopTo( ZP_AUDIO_LOOP_POINT_NONE )
{}
zpAudioResource::~zpAudioResource() {}

zp_bool zpAudioResource::load() {
	FMOD::System* system = zpFMOD::getInstance();
	FMOD_RESULT r;

	zp_uint mode = FMOD_HARDWARE;
	mode |= m_is3DSound ? FMOD_3D : FMOD_2D;
	mode |= m_isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

	FMOD::Sound* sound = *(FMOD::Sound**)&m_sound;
	if( m_isStreaming ) {
		r = system->createStream( getFilename().c_str(), mode, 0, &sound );
	} else {
		r = system->createSound( getFilename().c_str(), mode, 0, &sound );
	}
	FR( r );

	if( m_isLooping ) {
		r = sound->setLoopPoints( m_loopFrom, FMOD_TIMEUNIT_MS, m_loopTo, FMOD_TIMEUNIT_MS );
		FR( r );
	}

	r = sound->setUserData( this );
	FR( r );

	return true;
}
void zpAudioResource::unload() {
	if( m_sound ) {
		( (FMOD::Sound*)m_sound )->release();
		m_sound = ZP_NULL;
	}
}


zp_bool zpAudioResource::isStreaming() const {
	return m_isStreaming;
}
zp_bool zpAudioResource::isLooping() const {
	return m_isLooping;
}
zp_bool zpAudioResource::is3DSound() const {
	return m_is3DSound;
}
zpAudioChannelGroup zpAudioResource::getChannelGroup() const {
	return m_group;
}
zp_uint zpAudioResource::getLoopFrom() const {
	return m_loopFrom;
}
zp_uint zpAudioResource::getLoopTo() const {
	return m_loopTo;
}

void* zpAudioResource::getSound() const {
	return m_sound;
}

void zpAudioResource::setIsStreaming( zp_bool isStreaming ) {
	m_isStreaming = isStreaming;
}
void zpAudioResource::setIsLooping( zp_bool isLooping ) {
	m_isLooping = isLooping;
}
void zpAudioResource::setIs3DSound( zp_bool is3DSound ) {
	m_is3DSound = is3DSound;
}
void zpAudioResource::setChannelGroup( zpAudioChannelGroup group ) {
	m_group = group;
}
void zpAudioResource::setLoopFromTo( zp_uint loopFrom, zp_uint loopTo ) {
	m_loopFrom = loopFrom;
	m_loopTo = loopTo;
}
