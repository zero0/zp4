#include <ZeroPoint.shaderinc>

#pragma format VNU

ZP_SAMPLER_2D( _MainTex );

float4 _Color;

struct v2f
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

v2f main_vs( vs_input_base v )
{
    v2f o = (v2f)0;
    o.position = v.position;
    o.position = mul( o.position, world );
    o.position = mul( o.position, viewProjection );
    o.normal = normalize( v.normal );
    o.uv = v.texcoord;

    return o;
}

float4 main_ps( v2f input ) : SV_TARGET
{
    float4 col = tex2D( _MainTex, input.uv );

    float diffuse = saturate( dot( input.normal.xyz, light_direction.xyz ) );
    col.rgb *= light_color.rgb * diffuse * _Color.rgb;

    return col;
}
