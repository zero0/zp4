#pragma pack_matrix( row_major )
#pragma vertex main_vs
#pragma fragment main_ps
#pragma format VCU

SamplerState diffuseSampler : register( s0 );
Texture2D<float4> diffuse : register( t0 );

cbuffer Camera : register( b0 )
{
	float4x4 viewProjection;
	float4x4 invViewProjection;

	float4 up;
	float4 lookTo;
	float4 position;
    
	float zNear;
	float zFar;
	float fovy;
	float aspectRatio;
};

cbuffer PerFrame : register( b1 )
{
	float deltaTime;
	float actualDeltaTime;
	float fixedDeltaTime;
	float timeFromStart;
};

cbuffer PerDrawCall : register( b2 )
{
	float4x4 world;
};

struct VS_Input
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
};

struct PS_Input
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
};

PS_Input main_vs( VS_Input input )
{
	PS_Input output = (PS_Input)0;
	output.position = input.position;
	output.position = mul( output.position, world );
	output.position = mul( output.position, viewProjection );
	output.color = input.color;
	output.uv0 = input.uv0;

	return output;
}

float4 main_ps( PS_Input input ) : SV_TARGET
{
	float4 col = diffuse.Sample( diffuseSampler, input.uv0 );
	return col * input.color;
}
