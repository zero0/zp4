#include "zpRendering.h"

zpCameraComponent::zpCameraComponent() :
	m_renderLayers( 0xFFFFFFFF ),
	m_camera()
{}
zpCameraComponent::~zpCameraComponent() {}

zpCamera& zpCameraComponent::getCamera() {
	return m_camera;
}
zpFlag32& zpCameraComponent::getRenderLayers() {
	return m_renderLayers;
}

void zpCameraComponent::receiveMessage( const zpMessage& message ) {}

void zpCameraComponent::serialize( zpSerializedOutput* out ) {}
void zpCameraComponent::deserialize( zpSerializedInput* in ) {}

void zpCameraComponent::onCreate() {}
void zpCameraComponent::onDestroy() {}

void zpCameraComponent::onUpdate() {
	m_camera.update();
}

void zpCameraComponent::onEnabled() {}
void zpCameraComponent::onDisabled() {}
