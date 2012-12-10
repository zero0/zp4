#include "zpAudio.h"

zpAudioListenerComponent::zpAudioListenerComponent()
	: m_listener( 0 )
	, m_manager( ZP_NULL )
{}
zpAudioListenerComponent::~zpAudioListenerComponent() {}

void zpAudioListenerComponent::receiveMessage( const zpMessage& message ) {}

void zpAudioListenerComponent::serialize( zpSerializedOutput* out ) {}
void zpAudioListenerComponent::deserialize( zpSerializedInput* in ) {
	if( in->readBlock( ZP_SERIALIZE_TYPE_THIS ) ) {

		in->endBlock();
	}
}

void zpAudioListenerComponent::onCreate() {
	m_manager = getGame()->getGameManagers()->getAudioManager();
}
void zpAudioListenerComponent::onDestroy() {
	m_manager = ZP_NULL;
}

void zpAudioListenerComponent::onUpdate() {
	zpVector4f position;
	getParentGameObject()->getTransform().getPosition( position );

	zpVector4f velocity( position - m_oldPosition );
	m_oldPosition = position;

	zpVector4f forward( 0, 0, -1 );
	zpVector4f up( 0, 1, 0 );

	m_manager->set3DListener( m_listener, position, velocity, forward, up );
}

void zpAudioListenerComponent::onEnabled() {}
void zpAudioListenerComponent::onDisabled() {}