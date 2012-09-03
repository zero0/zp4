//#pragma pack_matrix( row_major )

struct VS_Input {
	float4 position : POSITION;
	float4 normal : NORMAL;
	float2 texCoord0 : TEXCOORD0;
};

struct PS_Input {
	float4 position : SV_POSITION;
	float4 normal : COLOR;
	float2 texCoord0 : TEXCOORD0;
};

cbuffer Camera : register( b0 ) {
	float4x4 view;
	float4x4 projection;
	float4x4 viewProjection;
	float4x4 world;
};

Texture2D diffuseMap : register( t0 );
SamplerState linearSampler : register( s0 );

PS_Input main_vs( VS_Input input ) {
	PS_Input output = (PS_Input)0;
	output.position = mul( input.position, world );
	output.position = mul( output.position, view );
	output.position = mul( output.position, projection );
	
	output.normal = input.normal;
	output.texCoord0 = input.texCoord0;
	
	return output;
}

float4 main_ps( PS_Input input ) : SV_TARGET {
	return diffuseMap.Sample( linearSampler, input.texCoord0 );
}