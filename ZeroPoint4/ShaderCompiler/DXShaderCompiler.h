#pragma once
#ifndef DX_SHADER_COMPILER
#define DX_SHADER_COMPILER

#pragma comment( lib, "d3d11" )
#pragma comment( lib, "dxgi" )
#pragma comment( lib, "dxguid" )

#if ZP_DEBUG
#pragma comment( lib, "d3dx11d" )
#else
#pragma comment( lib, "d3dx11" )
#endif

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
	zp_bool compileShaderVSPlatform( zpDataBuffer& data ) const;
	zp_bool compileShaderPSPlatform( zpDataBuffer& data ) const;
	zp_bool compileShaderGSPlatform( zpDataBuffer& data ) const;
	zp_bool compileShaderCSPlatform( zpDataBuffer& data ) const;

private:
	zp_bool compileShaderPlatform( const zp_char* mainFunc, const zp_char* profile, zpDataBuffer& data ) const;

	DXShaderCompilerPlatform m_platform;
};

#endif
