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

    zp_bool compileShaderVSPlatform( zpShaderInfo& info );
    zp_bool compileShaderPSPlatform( zpShaderInfo& info );
    zp_bool compileShaderGSPlatform( zpShaderInfo& info );
    zp_bool compileShaderCSPlatform( zpShaderInfo& info );

private:
    zp_bool compileShaderPlatform( const zp_char* mainFunc, const zp_char* profile, zpShaderInfo& info, zpShaderCompilerType compileType ) const;

    DXShaderCompilerPlatform m_platform;
};

#endif
