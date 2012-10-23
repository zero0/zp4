#include "zpRendering.h"

zpLightComponent::zpLightComponent() 
	: m_isLocalToGameObject( true )
	, m_castsShadow( false )
{}
zpLightComponent::~zpLightComponent() {}

void zpLightComponent::receiveMessage( const zpMessage& message ) {}

void zpLightComponent::serialize( zpSerializedOutput* out ) {
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	out->writeInt( m_lightData.type, "@type" );
	out->writeBoolean( m_isLocalToGameObject, "@is-local" );
	out->writeBoolean( m_castsShadow, "@casts-shadow" );

	out->endBlock();
}
void zpLightComponent::deserialize( zpSerializedInput* in ) {
	in->readBlock( ZP_SERIALIZE_TYPE_THIS );

	in->readInt( &m_lightData.type, "@type" );
	in->readBoolean( &m_isLocalToGameObject, "@is-local" );
	in->readBoolean( &m_castsShadow, "@casts-shadow" );

	in->endBlock();
}

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
void zpLightComponent::setCastsShadow( zp_bool castsShadow ) {
	m_castsShadow = castsShadow;
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
zp_bool zpLightComponent::getCastsShadow() const {
	return m_castsShadow;
}

const zpLightBufferData& zpLightComponent::getLightBufferData() const {
	return m_lightData;
}

void zpLightComponent::onCreate() {}
void zpLightComponent::onDestroy() {}

void zpLightComponent::onUpdate() {}

void zpLightComponent::onEnabled() {}
void zpLightComponent::onDisabled() {}

