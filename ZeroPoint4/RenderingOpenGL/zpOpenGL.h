#pragma once
#ifndef ZP_OPENGL_H
#define ZP_OPENGL_H

#include "Rendering/zpRendering.h"

#pragma comment( lib, "glew32s.lib" )
#pragma comment( lib, "opengl32.lib" )
// fixes linker errors with glew32s.lib
#pragma comment( linker, "/NODEFAULTLIB:libcmt.lib" )

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\wglew.h>
#include <GL\GL.h>

#include "zpOpenGLTexture.h"
#include "zpOpenGLShaderResource.h"

#include "zpOpenGLBuffer.h"
#include "zpOpenGLDepthStencilBuffer.h"
#include "zpOpenGLShaderResource.h"

#include "zpOpenGLRenderTarget.h"
#include "zpOpenGLRenderingEngine.h"
#include "zpOpenGLRenderingContext.h"

#include "zpOpenGLUtil.h"

#endif
