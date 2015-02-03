#include <ZeroPoint.shaderinc>

#pragma format VCU

ZP_SAMPLER_2D( _MainTex );

struct v2f
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
};

v2f main_vs( vs_input_gui input )
{
	v2f output = (v2f)0;
	output.position = input.position;
	output.position = mul( output.position, world );
	output.position = mul( output.position, viewProjection );
	output.color = input.color;
	output.uv = input.texcoord;

	return output;
}

float4 main_ps( v2f input ) : SV_TARGET
{
	float4 col = tex2D( _MainTex, input.uv );
	return col * input.color;
}
