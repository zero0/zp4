#include <ZeroPoint.shaderinc>

#pragma format VU

ZP_SAMPLER_2D( _MainTex );

struct v2f
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

v2f main_vs( vs_input_img input )
{
    v2f output = (v2f)0;
    output.position = input.position;
    output.uv = input.texcoord;

    return output;
}

float4 main_ps( v2f input ) : SV_TARGET
{
	float4 col = tex2D( _MainTex, input.uv );
    return float4( col.rgb, 1.0f );
}
