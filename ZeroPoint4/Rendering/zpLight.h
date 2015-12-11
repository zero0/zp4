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

union zpSphericalHarmonicsData
{
	//zpVector4f SHAr;
	//zpVector4f SHAg;
	//zpVector4f SHAb;
	//
	//zpVector4f SHBr;
	//zpVector4f SHBg;
	//zpVector4f SHBb;
	//
	//zpVector4f SHC;

	struct
	{
		zpVector4f SH0;
		zpVector4f SH1;
		zpVector4f SH2;
		zpVector4f SH3;
		zpVector4f SH4;
		zpVector4f SH5;
		zpVector4f SH6;
		zpVector4f SH7;
		zpVector4f SH8;
	};

	zpVector4f SH[ 9 ];
};

class zpSH
{
	ZP_NON_COPYABLE( zpSH );
public:
	static void eval( zpSphericalHarmonicsData& result );

private:
};

#endif
