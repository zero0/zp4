#pragma once
#ifndef ZP_LIGHT_COMPONENT_H
#define ZP_LIGHT_COMPONENT_H

struct zpDirectionalLightData {
	zpVector4f direction;
	zpColor4f color;
	zpColor4f specular;
};

struct zpPointLightData {
	zpVector4f position;
	zpColor4f color;
	zpColor4f specular;
};

struct zpSpotLightData {
	zpVector4f position;
	zpVector4f direction;
	zpColor4f color;
	zpColor4f specular;
	zp_float innerAngle;
	zp_float outerAngle;
};

enum zpLightType {
	ZP_LIGHT_TYPE_DIRECTIONAL = 1,
	ZP_LIGHT_TYPE_POINT,
	ZP_LIGHT_TYPE_SPOT,
};

class zpLightComponent : public zpRenderingComponent {
public:
	zpLightComponent();
	virtual ~zpLightComponent();

	void render();

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

	const zpColor4f& getColor() const;
	const zpColor4f& getSpecularColor() const;
	const zpVector4f& getPosition() const;
	const zpVector4f& getDirection() const;
	zp_float getInnerAngle() const;
	zp_float getOuterAngle() const;
	zp_float getPointRadius() const;

protected:
	void onCreate();
	void onDestroy();

	void onUpdate();

	void onEnabled();
	void onDisabled();

	void onShow();
	void onHide();

private:
	zpLightType m_type;
	zp_bool m_isLocalToGameObject;
	
	zpColor4f m_color;
	zpColor4f m_specular;
	zpVector4f m_position;
	zpVector4f m_direction;
	zp_float m_innerAngle;
	zp_float m_outerAngle;
	zp_float m_radius;
};

#endif