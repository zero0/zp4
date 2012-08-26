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

Texture2D diffuseMap : register( t0 );
SamplerState linearSampler {
	Filter = MIN_MAG_MIP_LINEAR;
};
SamplerState pointSampler {
	Filter = MIN_MAG_MIP_POINT;
};

PS_Input main_vs( VS_Input input ) {
	PS_Input output = (PS_Input)0;
	output.position = input.position;
	output.normal = input.normal;
	output.texCoord0 = input.texCoord0;
	
	return output;
}

float4 main_ps( PS_Input input ) : SV_TARGET {
	return diffuseMap.Sample( linearSampler, input.texCoord0 );
}