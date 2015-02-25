#include <ZeroPoint.shaderinc>

#pragma format VCU

ZP_SAMPLER_2D( _MainTex );

struct v2f
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

v2f main_vs( vs_input_gui v )
{
    v2f o = (v2f)0;
    o.position = v.position + float4( cameraPosition.xyz, 0 );
    o.position = mul( o.position, world );
    o.position = mul( o.position, viewProjection );
    o.position = o.position.xyww;
    o.color = v.color;
    o.uv = v.texcoord;

    return o;
}

float4 main_ps( v2f v ) : SV_TARGET
{
    return v.color;
}
