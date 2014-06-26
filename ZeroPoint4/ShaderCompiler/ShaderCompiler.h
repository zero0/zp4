#pragma once
#ifndef SHADER_COMPILER
#define SHADER_COMPILER

typedef zpPair< const zp_char*, const zp_char* > ShaderMacro;
typedef zpArrayList< ShaderMacro > ShaderMacros;

class BaseShaderCompiler
{
public:
	BaseShaderCompiler();
	virtual ~BaseShaderCompiler();

	zp_bool initialize( const zpArrayList< zpString >& args );
	void compile();
	void shutdown();

	void compileShaderVS();
	void compileShaderPS();
	void compileShaderGS();
	void compileShaderCS();

protected:
	void parsePragmas();

	virtual zp_bool compileShaderVSPlatform( zpDataBuffer& data ) const = 0;
	virtual zp_bool compileShaderPSPlatform( zpDataBuffer& data ) const = 0;
	virtual zp_bool compileShaderGSPlatform( zpDataBuffer& data ) const = 0;
	virtual zp_bool compileShaderCSPlatform( zpDataBuffer& data ) const = 0;

protected:
	zpString m_inputFile;
	zpString m_outputFile;
	zp_int m_optimizationLevel;

	zpStringBuffer m_shaderText;
	zpJson m_compiledShaderDesc;

	zpHashMap< zpString, zpString > m_pragmas;
	zpHashMap< zpString, zpString > m_macros;
};

#endif
