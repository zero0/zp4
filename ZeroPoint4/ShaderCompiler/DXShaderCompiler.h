#pragma once
#ifndef DX_SHADER_COMPILER
#define DX_SHADER_COMPILER

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
	void initializePlatform();

	zp_bool compileShaderVSPlatform( zpDataBuffer& data );
	zp_bool compileShaderPSPlatform( zpDataBuffer& data );
	zp_bool compileShaderGSPlatform( zpDataBuffer& data );
	zp_bool compileShaderCSPlatform( zpDataBuffer& data );

private:
	zp_bool compileShaderPlatform( const zp_char* mainFunc, const zp_char* profile, zpDataBuffer& data ) const;

	DXShaderCompilerPlatform m_platform;
};

#endif
