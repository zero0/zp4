#pragma pack_matrix( row_major )
#pragma vertex main_vs
#pragma fragment main_ps
#pragma format VU

Texture2D<float4> colorMap : register( t0 );
SamplerState pointSampler : register( s0 );

struct VS_Input
{
    float4 position : POSITION;
    float2 tex0 : TEXCOORD0;
};

struct PS_Input
{
    float4 position : SV_POSITION;
    float2 tex0 : TEXCOORD0;
};

PS_Input main_vs( VS_Input input )
{
    PS_Input output = (PS_Input)0;
    output.position = input.position;
    output.tex0 = input.tex0;
    
    return output;
}

float4 main_ps( PS_Input input ) : SV_TARGET
{
    return float4( colorMap.Sample( pointSampler, input.tex0 ).rgb, 1.f );
}
