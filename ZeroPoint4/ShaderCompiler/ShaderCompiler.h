#pragma once
#ifndef SHADER_COMPILER
#define SHADER_COMPILER

typedef zpPair< const zp_char*, const zp_char* > ShaderMacro;
typedef zpArrayList< ShaderMacro > ShaderMacros;

enum zpGlobalVariableInputType
{
	ZP_GLOBAL_VARIABLE_FLOAT,
	ZP_GLOBAL_VARIABLE_FLOAT2,
	ZP_GLOBAL_VARIABLE_FLOAT3,
	ZP_GLOBAL_VARIABLE_FLOAT4,

	ZP_GLOBAL_VARIABLE_INT,
	ZP_GLOBAL_VARIABLE_INT2,
	ZP_GLOBAL_VARIABLE_INT3,
	ZP_GLOBAL_VARIABLE_INT4,

	ZP_GLOBAL_VARIABLE_UINT,
	ZP_GLOBAL_VARIABLE_UINT2,
	ZP_GLOBAL_VARIABLE_UINT3,
	ZP_GLOBAL_VARIABLE_UINT4,

	ZP_GLOBAL_VARIABLE_INT4x4,
	ZP_GLOBAL_VARIABLE_UINT4x4,
	ZP_GLOBAL_VARIABLE_FLOAT4x4,
};

struct zpGlobalVariableInput
{
	zpString name;
	zp_uint size;
	zp_uint offset;
	//zpGlobalVariableInputType type;
	zpString type;
};
struct zpTextureShaderInput
{
	zpString name;
	zp_uint index;
	zp_uint dimension;
};
struct zpConstantBufferShaderInput
{
	zpString name;
	zp_uint slot;
	zp_uint size;
};
struct zpShaderInput
{
	zp_uint globalVariablesSize;
	zpArrayList< zpTextureShaderInput > textures;
	//zpArrayList< zpConstantBufferShaderInput > constantBuffers;
	zpArrayList< zpGlobalVariableInput > globalVariables;
};
struct zpShaderInfo
{
	zp_uint totalInstructions;
	zp_uint floatInstructions;
	zp_uint intInstructions;
	zp_uint textureInstructions;
	zp_uint dynamicFlowInstructions;

	zpShaderInput shaderInput;

	zpDataBuffer data;
};

class BaseShaderCompiler
{
public:
	BaseShaderCompiler();
	virtual ~BaseShaderCompiler();

	void initialize( const zpArrayList< zpString >& args );
	zp_bool readInputFile();
	void compile();
	void shutdown();

	void compileShaderVS();
	void compileShaderPS();
	void compileShaderGS();
	void compileShaderCS();

protected:
	void parsePragmas();

	void parseShaderInfo( zpJson& shaderJson, zpShaderInfo& info );
	void parseGlobalShaderInfo( zpJson& shaderJson, zpShaderInfo& info );

	virtual void initializePlatform() {}

	virtual zp_bool compileShaderVSPlatform( zpShaderInfo& info ) = 0;
	virtual zp_bool compileShaderPSPlatform( zpShaderInfo& info ) = 0;
	virtual zp_bool compileShaderGSPlatform( zpShaderInfo& info ) = 0;
	virtual zp_bool compileShaderCSPlatform( zpShaderInfo& info ) = 0;

protected:
	zpString m_inputFile;
	zpString m_outputFile;
	zpString m_previewFile;
	zp_int m_optimizationLevel;

	zpString m_localIncludeDir;
	zpString m_systemIncludeDir;

	zpStringBuffer m_shaderText;
	zpJson m_compiledShaderDesc;

	zpHashMap< zpString, zpString > m_pragmas;
	zpHashMap< zpString, zpString > m_macros;
};

#endif
