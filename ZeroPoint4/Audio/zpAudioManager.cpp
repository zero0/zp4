#include "zpAudio.h"
#include "zpFMOD.h"
#include "fmod.hpp"

#define T( t )	#t

FMOD_RESULT F_CALLBACK __fmodSystemCallback( FMOD_SYSTEM *system, FMOD_SYSTEM_CALLBACKTYPE type, void *commanddata1, void *commanddata2 ) {
	return FMOD_OK;
}

zpAudioManager::zpAudioManager() :
	m_speakerMode( ZP_AUDIO_SPEAKER_MODE_STEREO ),
	m_numChannels( 32 ),
	m_numListeners( 1 ),
	m_dopplerScale( 1.f ),
	m_distanceFactor( 1.f ),
	m_rolloffScale( 1.f )
{}
zpAudioManager::~zpAudioManager() {}

void zpAudioManager::setSpeakerMode( zpAudioSpeakerMode mode ) {
	m_speakerMode = mode;
}
zpAudioSpeakerMode zpAudioManager::getSpeakerMode() const {
	return m_speakerMode;
}

void zpAudioManager::setNumChannels( zp_uint channels ) {
	m_numChannels = channels;
}
zp_uint zpAudioManager::getNumChannels() const {
	return m_numChannels;
}

void zpAudioManager::setVolume( zpAudioChannelGroup channel, zp_float volume ) {
	( (FMOD::ChannelGroup*)m_channelGroups[ channel ] )->setVolume( volume );
}
zp_float zpAudioManager::getVolume( zpAudioChannelGroup channel ) const {
	zp_float volume;
	( (FMOD::ChannelGroup*)m_channelGroups[ channel ] )->getVolume( &volume );
	return volume;
}

void zpAudioManager::mute( zpAudioChannelGroup channel, zp_bool muted ) {
	( (FMOD::ChannelGroup*)m_channelGroups[ channel ] )->setMute( muted );
}
zp_bool zpAudioManager::isMuted( zpAudioChannelGroup channel ) const {
	zp_bool muted;
	( (FMOD::ChannelGroup*)m_channelGroups[ channel ] )->getMute( &muted );
	return muted;
}

void zpAudioManager::pause( zpAudioChannelGroup channel, zp_bool paused ) {
	( (FMOD::ChannelGroup*)m_channelGroups[ channel ] )->setPaused( paused );
}
zp_bool zpAudioManager::isPaused( zpAudioChannelGroup channel ) const {
	zp_bool paused;
	( (FMOD::ChannelGroup*)m_channelGroups[ channel ] )->getPaused( &paused );
	return paused;
}

void zpAudioManager::setNumListeners( zp_uint numListeners ) {
	m_numListeners = numListeners;
	zpFMOD::getInstance()->set3DNumListeners( m_numListeners );
}
zp_uint zpAudioManager::getNumListeners() const {
	return m_numListeners;
}

void zpAudioManager::set3DSettings( zp_float dopperScale, zp_float distanceFactor, zp_float rolloffScale ) {
	zpFMOD::getInstance()->set3DSettings( dopperScale, distanceFactor, rolloffScale );
}
void zpAudioManager::set3DListener( zp_uint listener, const zpVector4f& position, const zpVector4f& velocity, const zpVector4f& forward, const zpVector4f& up ) {
	FMOD_VECTOR p;
	FMOD_VECTOR v;
	FMOD_VECTOR f;
	FMOD_VECTOR u;
	position.store3( (zp_float*)&p );
	velocity.store3( (zp_float*)&v );
	forward.store3( (zp_float*)&f );
	up.store3( (zp_float*)&u );
	
	zpFMOD::getInstance()->set3DListenerAttributes( listener, &p, &v, &f, &u );
}

void zpAudioManager::play( zpResourceInstance<zpAudioResource>* soundInstance ) {
	if( !soundInstance ) return;
	FMOD_RESULT r;

	zpAudioResource* resource = soundInstance->getResource();

	FMOD::ChannelGroup* group = (FMOD::ChannelGroup*)m_channelGroups[ resource->getChannelGroup() ];
	
	zp_bool paused;
	group->getPaused( &paused );
	if( paused ) return;
	
	FMOD::Sound* sound = (FMOD::Sound*)resource->getSound();

	FMOD::Channel* channel;
	r = zpFMOD::getInstance()->playSound( FMOD_CHANNEL_FREE, sound, true, &channel );

	soundInstance->setChannel( channel );
	r = channel->setChannelGroup( group );

	zp_bool muted;
	r = group->getMute( &muted );
	r = channel->setMute( muted );

	zp_float volume;
	r = group->getVolume( &volume );
	r = channel->setVolume( volume );

	soundInstance->update();

	r = channel->setPaused( false );
}

void zpAudioManager::serialize( zpSerializedOutput* out ) {}
void zpAudioManager::deserialize( zpSerializedInput* in ) {}

void zpAudioManager::receiveMessage( const zpMessage& message ) {}

void zpAudioManager::onCreate() {
	if( !zpFMOD::createInstance() ) {
		zp_printfln( "Unable to create FMOD instance." );
		return;	
	}
	FMOD::System* system = zpFMOD::getInstance();
	FMOD_RESULT r;
	zp_uint version;
	zp_int numDrivers;
	FMOD_CAPS caps;
	FMOD_SPEAKERMODE ctrlSpeakerMode;
	zp_int ctrlOutputRate;

	r = system->getVersion( &version );
	if( version < FMOD_VERSION ) {
		zp_printfln( "Using an older version of FMOD %08x. Requires FMOD %08x", version, FMOD_VERSION );
		return;
	}
	
	r = system->getNumDrivers( &numDrivers );
	if( numDrivers == 0 ) {
		r = system->setOutput( FMOD_OUTPUTTYPE_NOSOUND );
	} else {
		r = system->getDriverCaps( 0, &caps, &ctrlOutputRate, &ctrlSpeakerMode );

		r = system->setSpeakerMode( ctrlSpeakerMode );

		if( caps & FMOD_CAPS_HARDWARE_EMULATED ) {
			r = system->setDSPBufferSize( 1024, 10 );
		}

		zp_char driverName[256];
		r = system->getDriverInfo( 0, driverName, 256, 0 );

		if( zp_strstr( driverName, "SigmaTel" ) ) {
			r = system->setSoftwareFormat( 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR );
		}
	}

	r = system->init( m_numChannels, FMOD_INIT_NORMAL, 0 );
	if( r == FMOD_ERR_OUTPUT_CREATEBUFFER ) {
		r = system->setSpeakerMode( FMOD_SPEAKERMODE_STEREO );

		r = system->init( m_numChannels, FMOD_INIT_NORMAL, 0 );
	}

	FMOD::ChannelGroup** groups = (FMOD::ChannelGroup**)&m_channelGroups[0];
	
	system->getMasterChannelGroup( &groups[ ZP_AUDIO_CHANNEL_GROUP_MASTER ] );
	system->createChannelGroup( T( ZP_AUDIO_CHANNEL_GROUP_MUSIC ), &groups[ ZP_AUDIO_CHANNEL_GROUP_MUSIC ] );
	system->createChannelGroup( T( ZP_AUDIO_CHANNEL_GROUP_SFX ), &groups[ ZP_AUDIO_CHANNEL_GROUP_SFX ] );
	system->createChannelGroup( T( ZP_AUDIO_CHANNEL_GROUP_DIALOG ), &groups[ ZP_AUDIO_CHANNEL_GROUP_DIALOG ] );

	groups[ ZP_AUDIO_CHANNEL_GROUP_MASTER ]->addGroup( groups[ ZP_AUDIO_CHANNEL_GROUP_MUSIC ] );
	groups[ ZP_AUDIO_CHANNEL_GROUP_MASTER ]->addGroup( groups[ ZP_AUDIO_CHANNEL_GROUP_SFX ] );
	groups[ ZP_AUDIO_CHANNEL_GROUP_MASTER ]->addGroup( groups[ ZP_AUDIO_CHANNEL_GROUP_DIALOG ] );

	system->setUserData( this );
	system->setCallback( __fmodSystemCallback );
}
void zpAudioManager::onDestroy() {
	zpFMOD::destroyInstance();
}

void zpAudioManager::onUpdate() {
	zpFMOD::getInstance()->update();
}

void zpAudioManager::onEnabled() {}
void zpAudioManager::onDisabled() {}