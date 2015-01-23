#include <ZeroPoint.shaderinc>

#pragma format VCU

zpSampler2D _MainTex;

struct v2f
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

v2f main_vs( vs_input_gui v )
{
    v2f o = (v2f)0;
    o.position = v.position;
    o.position = mul( o.position, world );
    o.position = mul( o.position, viewProjection );
    o.color = v.color;
    o.uv = v.texcoord;

    return o;
}

float4 main_ps( v2f v ) : SV_TARGET
{
    float4 col = tex2D( _MainTex, v.uv );
    return col * v.color;
}
