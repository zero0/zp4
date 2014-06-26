#pragma once
#ifndef SHADER_COMPILER_MAIN
#define SHADER_COMPILER_MAIN

#include "Core/zpCore.h"

#define SHADER_DX9			"DX9"
#define SHADER_DX10			"DX10"
#define SHADER_DX11			"DX11"
#define SHADER_GL2			"GL2"

#define SHADER_COMPILE_VS	"COMPILE_VS"
#define SHADER_COMPILE_PS	"COMPILE_PS"
#define SHADER_COMPILE_GS	"COMPILE_GS"
#define SHADER_COMPILE_CS	"COMPILE_CS"

#define DEFAULT_PS_MAIN		"main_ps"
#define DEFAULT_VS_MAIN		"main_vs"

#define PRAGMA_VERTEX		"vertex"
#define PRAGMA_FRAG			"frag"
#define PRAGMA_FORMAT		"format"

#include "ShaderCompiler.h"
#include "DXShaderCompiler.h"

#endif
