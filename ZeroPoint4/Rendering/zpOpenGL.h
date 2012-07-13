#pragma once
#ifndef ZP_OPENGL_H
#define ZP_OPENGL_H

#if ZP_DEBUG
#pragma comment( lib, "glew32s.lib" )
#else
#pragma comment( lib, "glew32.lib" )
#endif

#pragma comment( lib, "opengl32.lib" )

class zpOpenGLRenderingEngine;

#include "zpOpenGLRenderingEngine.h"

#endif