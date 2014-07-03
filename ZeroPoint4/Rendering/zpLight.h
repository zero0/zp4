#pragma once
#ifndef ZP_LIGHT_H
#define ZP_LIGHT_H

enum zpLightType : zp_uint
{
	ZP_LIGHT_TYPE_DIRECTIONAL = 0,
	ZP_LIGHT_TYPE_POINT,
	ZP_LIGHT_TYPE_SPOT,

	zpLightType_Count,
};

struct zpLightBufferData
{
	zpVector4f position;
	zpVector4f direction;
	zpColor4f color;
	zp_float innerAngle;
	zp_float outerAngle;
	zp_float radius;
	zpLightType type;
};

#endif
