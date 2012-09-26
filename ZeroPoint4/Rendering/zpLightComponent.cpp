#include "zpRendering.h"

zpLightComponent::zpLightComponent() 
	: m_type( ZP_LIGHT_TYPE_DIRECTIONAL )
	, m_isLocalToGameObject( true )
{}
zpLightComponent::~zpLightComponent() {}

void zpLightComponent::render() {
	zpVector4f position;
	if( m_isLocalToGameObject ) {
		getParentGameObject()->getTransform().getPosition( position );
	}

	position.add4( m_position );
}

void zpLightComponent::receiveMessage( const zpMessage& message ) {}

void zpLightComponent::serialize( zpSerializedOutput* out ) {}
void zpLightComponent::deserialize( zpSerializedInput* in ) {}

zpLightType zpLightComponent::getLightType() const {
	return m_type;
}

void zpLightComponent::setColor( const zpColor4f& color ) {
	m_color = color;
}
void zpLightComponent::setSpecularColor( const zpColor4f& specular ) {
	m_specular = specular;
}
void zpLightComponent::setPosition( const zpVector4f& position ) {
	m_position = position;
}
void zpLightComponent::setDirection( const zpVector4f& direction ) {
	m_direction = direction;
	m_direction.normalize3();
}
void zpLightComponent::setSpotAngles( zp_float innerAngle, zp_float outerAngle ) {
	m_innerAngle = innerAngle;
	m_outerAngle = outerAngle;
}
void zpLightComponent::setPointRadius( zp_float radius ) {
	m_radius = radius;
}

const zpColor4f& zpLightComponent::getColor() const {
	return m_color;
}
const zpColor4f& zpLightComponent::getSpecularColor() const {
	return m_specular;
}
const zpVector4f& zpLightComponent::getPosition() const {
	return m_position;
}
const zpVector4f& zpLightComponent::getDirection() const {
	return m_direction;
}
zp_float zpLightComponent::getInnerAngle() const {
	return m_innerAngle;
}
zp_float zpLightComponent::getOuterAngle() const {
	return m_outerAngle;
}
zp_float zpLightComponent::getPointRadius() const {
	return m_radius;
}

void zpLightComponent::onCreate() {}
void zpLightComponent::onDestroy() {}

void zpLightComponent::onUpdate() {}

void zpLightComponent::onEnabled() {}
void zpLightComponent::onDisabled() {}

void zpLightComponent::onShow() {}
void zpLightComponent::onHide() {}
