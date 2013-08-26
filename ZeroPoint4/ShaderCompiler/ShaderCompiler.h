#include "Core/zpCore.h"

#pragma comment( lib, "d3d11" )
#pragma comment( lib, "dxgi" )
#pragma comment( lib, "dxguid" )

#if ZP_DEBUG
#pragma comment( lib, "d3dx11d" )
#else
#pragma comment( lib, "d3dx11" )
#endif

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

	void prepareShaderMacros( const zpJson& desc, ShaderMacros& macros ) const;

	void compileShaderVS();
	void compileShaderPS();

protected:
	virtual zp_bool compileShaderVSPlatform( const zpJson& vs, const ShaderMacros& macros, zpDataBuffer& data ) const = 0;
	virtual zp_bool compileShaderPSPlatform( const zpJson& ps, const ShaderMacros& macros, zpDataBuffer& data ) const = 0;

protected:
	zpString m_inputFile;
	zpString m_outputFile;
	zp_int m_optimizationLevel;

	zpStringBuffer m_shaderText;
	zpJson m_shaderDesc;
	zpJson m_compiledShaderDesc;

	ShaderMacros m_globalMacros;
};

class DXShaderCompiler : public BaseShaderCompiler
{
	ZP_NON_COPYABLE( DXShaderCompiler );

public:
	enum DXShaderCompilerPlatform
	{
		DX_SHADER_COMPILER_PLATFORM_DX9,
		DX_SHADER_COMPILER_PLATFORM_DX10,
		DX_SHADER_COMPILER_PLATFORM_DX11,
	};

	DXShaderCompiler( DXShaderCompilerPlatform platform );
	virtual ~DXShaderCompiler();

protected:
	zp_bool compileShaderVSPlatform( const zpJson& vs, const ShaderMacros& macros, zpDataBuffer& data ) const;
	zp_bool compileShaderPSPlatform( const zpJson& ps, const ShaderMacros& macros, zpDataBuffer& data ) const;

private:
	zp_bool compileShaderPlatform( const zpJson& shader, const zp_char* profile, const ShaderMacros& macros, zpDataBuffer& data ) const;

	DXShaderCompilerPlatform m_platform;
};