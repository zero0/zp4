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

	virtual void initializePlatform() {}

	virtual zp_bool compileShaderVSPlatform( zpDataBuffer& data ) = 0;
	virtual zp_bool compileShaderPSPlatform( zpDataBuffer& data ) = 0;
	virtual zp_bool compileShaderGSPlatform( zpDataBuffer& data ) = 0;
	virtual zp_bool compileShaderCSPlatform( zpDataBuffer& data ) = 0;

protected:
	zpString m_inputFile;
	zpString m_outputFile;
	zp_int m_optimizationLevel;

	zpString m_localIncludeDir;
	zpString m_systemIncludeDir;

	zpStringBuffer m_shaderText;
	zpJson m_compiledShaderDesc;

	zpHashMap< zpString, zpString > m_pragmas;
	zpHashMap< zpString, zpString > m_macros;
};

#endif
