#pragma pack_matrix( row_major )
#pragma vertex main_vs
#pragma fragment main_ps
#pragma format VC

struct VS_Input {
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PS_Input {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PS_Input main_vs( VS_Input input ) {
	PS_Input output = (PS_Input)0;
	output.position = input.position;
	output.color = input.color;
	
	return output;
}

float4 main_ps( PS_Input input ) : SV_TARGET {
	return input.color;
}