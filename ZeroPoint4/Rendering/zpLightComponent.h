#pragma once
#ifndef ZP_LIGHT_COMPONENT_H
#define ZP_LIGHT_COMPONENT_H

enum zpLightType : zp_uint
{
	ZP_LIGHT_TYPE_DIRECTIONAL = 1,
	ZP_LIGHT_TYPE_POINT,
	ZP_LIGHT_TYPE_SPOT,
};

struct zpLightBufferData
{
	zpVector4f position;
	zpVector4f direction;
	zpColor4f color;
	zpColor4f specular;
	zp_float innerAngle;
	zp_float outerAngle;
	zp_float radius;
	zpLightType type;
};

class zpLightComponent {
public:
	zpLightComponent();
	virtual ~zpLightComponent();

	void receiveMessage( const zpMessage& message );

	void serialize( zpSerializedOutput* out );
	void deserialize( zpSerializedInput* in );

	zpLightType getLightType() const;

	void setColor( const zpColor4f& color );
	void setSpecularColor( const zpColor4f& specular );
	void setPosition( const zpVector4f& position );
	void setDirection( const zpVector4f& direction );
	void setSpotAngles( zp_float innerAngle, zp_float outerAngle );
	void setPointRadius( zp_float radius );
	void setCastsShadow( zp_bool castsShadow );

	const zpColor4f& getColor() const;
	const zpColor4f& getSpecularColor() const;
	const zpVector4f& getPosition() const;
	const zpVector4f& getDirection() const;
	zp_float getInnerAngle() const;
	zp_float getOuterAngle() const;
	zp_float getPointRadius() const;
	zp_bool getCastsShadow() const;

	const zpLightBufferData& getLightBufferData() const;

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

private:
	zp_bool m_isLocalToGameObject;
	zp_bool m_castsShadow;

	zpVector4f m_localPosition;
	zpLightBufferData m_lightData;
};

#endif