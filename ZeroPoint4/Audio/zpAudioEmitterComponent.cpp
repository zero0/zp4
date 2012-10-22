#include "zpAudio.h"

zpAudioEmitterComponent::zpAudioEmitterComponent() :
	m_isMoving( false ),
	m_isPlaying( false )
{}
zpAudioEmitterComponent::~zpAudioEmitterComponent() {}

void zpAudioEmitterComponent::playSound( const zpString& soundAlias ) {
	zpResourceInstance<zpAudioResource>* instance = ZP_NULL;
	if( !m_audioInstances.find( soundAlias, instance ) ) {
		zpContentManager* content = getGameManagerOfType<zpContentManager>();

		m_audioInstances[ soundAlias ] = content->createInstanceOfResource<zpAudioResource>( soundAlias );
	}
	if( !instance->isPlaying() ) m_manager->play( instance );
}
void zpAudioEmitterComponent::stopSound( const zpString& soundAlias ) {
	zpResourceInstance<zpAudioResource>* instance = ZP_NULL;
	if( m_audioInstances.find( soundAlias, instance ) ) {
		instance->stop();
	}
}
void zpAudioEmitterComponent::stopAllSounds() {
	m_audioInstances.foreach( []( const zpString& key, zpResourceInstance<zpAudioResource>& instance ) {
		instance.stop();
	} );
}

void zpAudioEmitterComponent::receiveMessage( const zpMessage& message ) {
	switch( message.getMessageType() ) {
	case zpMessageTypes::PLAY_SOUND:
		playSound( message.getMessageData<zpString>() );
		break;
	case zpMessageTypes::STOP_SOUND:
		stopSound( message.getMessageData<zpString>() );
		break;
	case zpMessageTypes::STOP_ALL_SOUNDS:
		stopAllSounds();
		break;
	}
}

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

	zpVector4f position;
	getParentGameObject()->getWorldTransform().getPosition( position );

	zpVector4f velocity = position - m_oldPosition;
	m_oldPosition = position;

	m_audioInstances.foreach( [ &position, &velocity ]( const zpString& key, zpResourceInstance<zpAudioResource>& instance ) {
		if( instance.isPlaying() ) {
			instance.setPosition( position );
			instance.setVelocity( velocity );
			instance.update();
		}
	} );
}

void zpAudioEmitterComponent::onEnabled() {}
void zpAudioEmitterComponent::onDisabled() {}
