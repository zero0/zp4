#include "zpRendering.h"

zpLightComponent::zpLightComponent() 
	: m_isLocalToGameObject( true )
{}
zpLightComponent::~zpLightComponent() {}

void zpLightComponent::render() {
	m_manager->getGlobalBuffer( ZP_RENDERING_GLOBAL_BUFFER_LIGHT )->update( m_lightData );
}

void zpLightComponent::receiveMessage( const zpMessage& message ) {}

void zpLightComponent::serialize( zpSerializedOutput* out ) {}
void zpLightComponent::deserialize( zpSerializedInput* in ) {}

zpLightType zpLightComponent::getLightType() const {
	return (zpLightType)m_lightData.type;
}

void zpLightComponent::setColor( const zpColor4f& color ) {
	m_lightData.color = color;
}
void zpLightComponent::setSpecularColor( const zpColor4f& specular ) {
	m_lightData.specular = specular;
}
void zpLightComponent::setPosition( const zpVector4f& position ) {
	m_localPosition = position;
	m_lightData.position = m_localPosition;

	if( m_isLocalToGameObject ) {
		zpVector4f worldPosition;
		getParentGameObject()->getWorldTransform().getPosition( worldPosition );

		m_lightData.position.add4( worldPosition );
	}
}
void zpLightComponent::setDirection( const zpVector4f& direction ) {
	m_lightData.direction = direction;
	m_lightData.direction.normalize3();
}
void zpLightComponent::setSpotAngles( zp_float innerAngle, zp_float outerAngle ) {
	m_lightData.innerAngle = innerAngle;
	m_lightData.outerAngle = outerAngle;
}
void zpLightComponent::setPointRadius( zp_float radius ) {
	m_lightData.radius = radius;
}

const zpColor4f& zpLightComponent::getColor() const {
	return m_lightData.color;
}
const zpColor4f& zpLightComponent::getSpecularColor() const {
	return m_lightData.specular;
}
const zpVector4f& zpLightComponent::getPosition() const {
	return m_localPosition;
}
const zpVector4f& zpLightComponent::getDirection() const {
	return m_lightData.direction;
}
zp_float zpLightComponent::getInnerAngle() const {
	return m_lightData.innerAngle;
}
zp_float zpLightComponent::getOuterAngle() const {
	return m_lightData.outerAngle;
}
zp_float zpLightComponent::getPointRadius() const {
	return m_lightData.radius;
}

void zpLightComponent::onCreate() {
	m_manager = getGameManagerOfType<zpRenderingManager>();
}
void zpLightComponent::onDestroy() {}

void zpLightComponent::onUpdate() {}

void zpLightComponent::onEnabled() {}
void zpLightComponent::onDisabled() {}

void zpLightComponent::onShow() {}
void zpLightComponent::onHide() {}
