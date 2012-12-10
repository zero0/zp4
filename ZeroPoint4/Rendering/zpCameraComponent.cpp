#include "zpRendering.h"

zpCameraComponent::zpCameraComponent()
	: m_isLocalToGameObject( true )
	, m_renderLayers()
	, m_manager( ZP_NULL )
	, m_camera()
{}
zpCameraComponent::~zpCameraComponent() {}

zp_bool zpCameraComponent::isLocalToGameObject() const {
	return m_isLocalToGameObject;
}

zpCamera& zpCameraComponent::getCamera() {
	return m_camera;
}

void zpCameraComponent::setRenderLayers( const zpRenderLayer& layers ) {
	m_renderLayers = layers;
}
const zpRenderLayer& zpCameraComponent::getRenderLayers() const {
	return m_renderLayers;
}

void zpCameraComponent::receiveMessage( const zpMessage& message ) {}

void zpCameraComponent::serialize( zpSerializedOutput* out ) {
	
}
void zpCameraComponent::deserialize( zpSerializedInput* in ) {}

void zpCameraComponent::onCreate() {
	m_manager = getGame()->getGameManagers()->getRenderingManager();
}
void zpCameraComponent::onDestroy() {}

void zpCameraComponent::onUpdate() {
	m_camera.update();
}

void zpCameraComponent::onEnabled() {}
void zpCameraComponent::onDisabled() {}
