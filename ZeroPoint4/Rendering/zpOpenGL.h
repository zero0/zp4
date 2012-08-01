#pragma once
#ifndef ZP_OPENGL_H
#define ZP_OPENGL_H

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

class zpOpenGLRenderingEngine;
class zpOpenGLRenderingContext;

#include "zpOpenGLRenderingEngine.h"
#include "zpOpenGLRenderingContext.h"

#endif