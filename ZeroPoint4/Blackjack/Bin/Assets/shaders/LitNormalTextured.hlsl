#pragma pack_matrix( row_major )

SamplerState texSampler : register( s0 );
Texture2D<float4> tex : register( t0 );

struct Camera
{
	float4x4 viewProjection;
	float4x4 invViewProjection;
	
	float4 up;
	float4 lookTo;
	float4 position;
	//float4 zNearFarFovAspect;
	
	float zNear;
	float zFar;
	float fovy;
	float aspectRatio;
};

struct PerFrame
{
	float deltaTime;
	float actualDeltaTime;
	float fixedDeltaTime;
	float timeFromStart;
};

struct PerDrawCall
{
	float4x4 world;
};

Camera camera : register( b0 );
PerFrame frameData : register( b1 );
PerDrawCall drawData : register( b2 );

struct VS_Input
{
	float4 position : POSITION;
	float4 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

struct PS_Input
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

PS_Input main_vs( VS_Input input )
{
	PS_Input output = (PS_Input)0;
	output.position = input.position;
	output.position = mul( output.position, drawData.world );
	output.position = mul( output.position, camera.viewProjection );
	output.normal = input.normal;
	output.texCoord = input.texCoord;
	
	return output;
}

float4 main_ps( PS_Input input ) : SV_TARGET
{
	float4 col = tex.Sample( texSampler, input.texCoord );
	return col;
}