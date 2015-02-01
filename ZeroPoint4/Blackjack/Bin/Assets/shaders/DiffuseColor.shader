#include <ZeroPoint.shaderinc>

#pragma format VC

struct v2f
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

v2f main_vs( vs_input_color input )
{
	v2f output = (v2f)0;
	output.position = input.position;
	output.position = mul( output.position, world );
	output.position = mul( output.position, viewProjection );
	output.color = input.color;

	return output;
}

float4 main_ps( v2f input ) : SV_TARGET
{
	return input.color;
}
