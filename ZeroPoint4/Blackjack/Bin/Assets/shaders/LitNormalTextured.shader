#pragma pack_matrix( row_major )
#pragma vertex main_vs
#pragma format VNU

SamplerState texSampler : register( s0 );
Texture2D<float4> tex : register( t0 );

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

cbuffer Light0 : register( b3 )
{
    float4 light_position;
    float4 light_direction;
    float4 light_color;
    float light_innerAngle;
    float light_outerAngle;
    float light_radius;
    int light_type;
};

//Camera camera : register( b0 );
//PerFrame frameData : register( b1 );
//PerDrawCall drawData : register( b2 );

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
	output.position = mul( output.position, world );
	output.position = mul( output.position, viewProjection );
	output.normal = normalize( input.normal );
	output.texCoord = input.texCoord;

	return output;
}

float4 main_ps( PS_Input input ) : SV_TARGET
{
	float4 col = tex.Sample( texSampler, input.texCoord );
    float diffuse = saturate( dot( input.normal.xyz, light_direction.xyz ) );
    col.rgb *= light_color.rgb * diffuse;
	return col;
}
