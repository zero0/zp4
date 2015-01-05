#include <ZeroPoint.shaderinc>

#pragma vertex main_vs
#pragma fragment main_ps
#pragma format VC

struct v2f
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

v2f main_vs( vs_input_color v )
{
	v2f o = (v2f)0;
	o.position = v.position;
	o.position = mul( o.position, world );
	o.position = mul( o.position, viewProjection );
	o.color = v.color;
	
	return o;
}

float4 main_ps( v2f input ) : COLOR
{
	return input.color;
}