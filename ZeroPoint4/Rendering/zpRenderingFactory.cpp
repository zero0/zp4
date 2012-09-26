#include "zpRendering.h"

#if ZP_RENDERING_TYPE == ZP_DX
#include "RenderingDX/zpDX11.h"
#elif ZP_RENDERING_TYPE == ZP_OPENGL
#include "RenderingOpenGL/zpOpenGL.h"
#else
#error( "No rendering engine selected!" )
#endif

zpRenderingFactory::zpRenderingFactory() {}
zpRenderingFactory::~zpRenderingFactory() {}

zpRenderingEngine* zpRenderingFactory::s_renderingEngine = ZP_NULL;
zpRenderingEngine* zpRenderingFactory::getRenderingEngine() {
	if( !s_renderingEngine ) {
#if ZP_RENDERING_TYPE == ZP_DX
		s_renderingEngine = new zpDX11RenderingEngine;
#elif ZP_RENDERING_TYPE == ZP_OPENGL
		s_renderingEngine = new zpOpenGLRenderingEngine;
#endif
		s_renderingEngine->initialize();
	}
	return s_renderingEngine;
}

void zpRenderingFactory::destroyRenderingEngine() {
	if( s_renderingEngine ) {
		s_renderingEngine->shutdown();
		delete s_renderingEngine;
		s_renderingEngine = ZP_NULL;
	}
	
}