//#pragma pack_matrix( row_major )

struct VS_Input {
	float4 position : POSITION;
	float4 color : COLOR;
};

cbuffer Camera : register( b4 ) {
	float4x4 viewProjection;
	float4x4 invViewProjection;
};

struct PS_Input {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PS_Input main_vs( VS_Input input ) {
	PS_Input output = (PS_Input)0;
	//output.position = mul( input.position, world );
	//output.position = mul( output.position, view );
	//output.position = mul( output.position, projection );
	
	output.position = mul( input.position, ( viewProjection ) );
	output.color = input.color;
	
	return output;
}

float4 main_ps( PS_Input input ) : SV_TARGET {
	return input.color;
}