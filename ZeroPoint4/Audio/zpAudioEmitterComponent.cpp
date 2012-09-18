#include "zpAudio.h"

zpAudioEmitterComponent::zpAudioEmitterComponent() :
	m_isMoving( false ),
	m_isPlaying( false )
{}
zpAudioEmitterComponent::~zpAudioEmitterComponent() {}

void zpAudioEmitterComponent::playSound( const zpString& soundAlias ) {
	//zpAudioInstance* instance = ZP_NULL;
	zpResourceInstance<zpAudioResource>* instance = ZP_NULL;
	if( !m_audioInstances.find( soundAlias, instance ) ) {
		zpContentManager* content = getGameManagerOfType<zpContentManager>();
		zpAudioResource* resource = content->getResourceOfType<zpAudioResource>( soundAlias );

		//instance = new zpAudioInstance( resource );
		//m_audioInstances[ soundAlias ] = instance;
		m_audioInstances[ soundAlias ] = zpResourceInstance<zpAudioResource>( resource );
	}
	if( !instance->isPlaying() ) m_manager->play( instance );
}
void zpAudioEmitterComponent::stopSound( const zpString& soundAlias ) {
	//zpAudioInstance* instance = ZP_NULL;
	zpResourceInstance<zpAudioResource>* instance = ZP_NULL;
	if( m_audioInstances.find( soundAlias, instance ) ) {
		instance->stop();
	}
}
void zpAudioEmitterComponent::stopAllSounds() {
	//m_audioInstances.foreach( []( const zpString& key, zpAudioInstance* instance ) {
	m_audioInstances.foreach( []( const zpString& key, zpResourceInstance<zpAudioResource>& instance ) {
		instance.stop();
	} );
}

void zpAudioEmitterComponent::receiveMessage( const zpMessage& message ) {}

void zpAudioEmitterComponent::serialize( zpSerializedOutput* out ) {}
void zpAudioEmitterComponent::deserialize( zpSerializedInput* in ) {}

void zpAudioEmitterComponent::onCreate() {
	m_manager = getGameManagerOfType<zpAudioManager>();
}
void zpAudioEmitterComponent::onDestroy() {
	m_audioInstances.clear();
}

void zpAudioEmitterComponent::onUpdate() {
	if( !m_isMoving ) return;

	m_audioInstances.foreach( []( const zpString& key, zpResourceInstance<zpAudioResource>& instance ) {
		if( instance.isPlaying() ) {
			instance.update();
		}
	} );
}

void zpAudioEmitterComponent::onEnabled() {}
void zpAudioEmitterComponent::onDisabled() {}
