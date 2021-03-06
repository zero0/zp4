#pragma once
#ifndef SHADER_COMPILER_MAIN
#define SHADER_COMPILER_MAIN

#include "Core/zpCore.h"

enum zpShaderCompilerType
{
    ZP_SHADER_COMPILE_VS,
    ZP_SHADER_COMPILE_PS,
    ZP_SHADER_COMPILE_GS,
    ZP_SHADER_COMPILE_CS,

    zpShaderCompilerType_Count
};

enum zpShaderInputSigniture : zp_uint
{
    ZP_SHADER_INPUT_SIG_POSITION    = ( 1 << 0 ),
    ZP_SHADER_INPUT_SIG_NORMAL        = ( 1 << 1 ),
    ZP_SHADER_INPUT_SIG_BINORMAL    = ( 1 << 2 ),
    ZP_SHADER_INPUT_SIG_TANGENT        = ( 1 << 3 ),
    ZP_SHADER_INPUT_SIG_COLOR        = ( 1 << 4 ),
    ZP_SHADER_INPUT_SIG_TEXCOORD0    = ( 1 << 5 ),
    ZP_SHADER_INPUT_SIG_TEXCOORD1    = ( 1 << 6 ),
};

#define SHADER_D3D            "D3D"
#define SHADER_DX9            "DX9"
#define SHADER_DX10            "DX10"
#define SHADER_DX11            "DX11"
#define SHADER_HLSL            "HLSL"

#define SHADER_GL            "GL"
#define SHADER_GL2            "GL2"
#define SHADER_GLES            "GLES"
#define SHADER_GLES2        "GLES2"
#define SHADER_GLSL            "GLSL"

#define SHADER_COMPILE_VS    "COMPILE_VS"
#define SHADER_COMPILE_PS    "COMPILE_PS"
#define SHADER_COMPILE_GS    "COMPILE_GS"
#define SHADER_COMPILE_CS    "COMPILE_CS"

#define MACRO_SHADER        "ZP_SHADER_"
#define MACRO_SHADER_D3D    MACRO_SHADER SHADER_D3D
#define MACRO_SHADER_DX9    MACRO_SHADER SHADER_DX9
#define MACRO_SHADER_DX10    MACRO_SHADER SHADER_DX10
#define MACRO_SHADER_DX11    MACRO_SHADER SHADER_DX11

#define MACRO_SHADER_GL        MACRO_SHADER SHADER_GL
#define MACRO_SHADER_GL2    MACRO_SHADER SHADER_GL2
#define MACRO_SHADER_GLES    MACRO_SHADER SHADER_GLES
#define MACRO_SHADER_GLES2    MACRO_SHADER SHADER_GLES2

#define MACRO_SHADER_API_HLSL    MACRO_SHADER "API_" SHADER_HLSL
#define MACRO_SHADER_API_GLSL    MACRO_SHADER "API_" SHADER_GLSL

#define DEFAULT_PS_MAIN        "main_ps"
#define DEFAULT_VS_MAIN        "main_vs"

#define PRAGMA_VERTEX        "vertex"
#define PRAGMA_FRAGMENT        "fragment"
#define PRAGMA_FORMAT        "format"

#include "ShaderCompiler.h"
#include "DXShaderCompiler.h"

#endif
