#include "zpAudio.h"

zpAudioEmitterComponent::zpAudioEmitterComponent() :
	m_isMoving( false ),
	m_isPlaying( false ) {}
zpAudioEmitterComponent::~zpAudioEmitterComponent() {}

void zpAudioEmitterComponent::playSound( const zpString& soundAlias ) {
	zpResourceInstance<zpAudioResource>* instance = ZP_NULL;
	if( !m_audioInstances.find( soundAlias, &instance ) ) {
		zpContentManager* content = getGame()->getGameManagers()->getContentManager();

		m_audioInstances[ soundAlias ] = content->createInstanceOfResource<zpAudioResource>( soundAlias );
	}
	if( !instance->isPlaying() ) m_manager->play( instance );
}
void zpAudioEmitterComponent::stopSound( const zpString& soundAlias ) {
	zpResourceInstance<zpAudioResource>* instance = ZP_NULL;
	if( m_audioInstances.find( soundAlias, &instance ) ) {
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

void zpAudioEmitterComponent::serialize( zpSerializedOutput* out ) {
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	out->writeBoolean( m_isMoving, "@moveable" );

	out->endBlock();
}
void zpAudioEmitterComponent::deserialize( zpSerializedInput* in ) {
	if( in->readBlock( ZP_SERIALIZE_TYPE_THIS ) ) {

		in->readBoolean( &m_isMoving, "@moveable" );

		in->endBlock();
	}
}

void zpAudioEmitterComponent::onCreate() {
	m_manager = getGame()->getGameManagers()->getAudioManager();
}
void zpAudioEmitterComponent::onDestroy() {
	stopAllSounds();

	m_audioInstances.clear();
	m_manager = ZP_NULL;
	m_isPlaying = false;
}

void zpAudioEmitterComponent::onUpdate() {
	if( !m_isMoving ) return;

	zpVector4f position;
	getParentGameObject()->getTransform().getPosition( position );

	zpVector4f velocity( position - m_oldPosition );
	m_oldPosition = position;

	m_audioInstances.foreach( [ &position, &velocity ]( const zpString& key, zpResourceInstance<zpAudioResource>& instance ) {
		if( instance.isPlaying() )
		{
			instance.setPosition( position );
			instance.setVelocity( velocity );
			instance.update();
		}
	} );
}

void zpAudioEmitterComponent::onEnabled() {}
void zpAudioEmitterComponent::onDisabled() {
	stopAllSounds();
}
