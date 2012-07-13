#include "zpRendering.h"

#include "zpDX11RenderingEngine.h"
#include "zpOpenGLRenderingEngine.h"

zpRenderingFactory::zpRenderingFactory() {}
zpRenderingFactory::~zpRenderingFactory() {}

zpRenderingEngine* zpRenderingFactory::s_renderingEngine = ZP_NULL;
zpRenderingEngine* zpRenderingFactory::createRenderingEngine( zpRenderingEngineType type ) {
	if( !s_renderingEngine ) {
		switch( type ) {
		case ZP_RENDERING_ENGINE_DX:
			s_renderingEngine = new zpDX11RenderingEngine;
			break;
		case ZP_RENDERING_ENGINE_OPENGL:
			s_renderingEngine = new zpOpenGLRenderingEngine;
			break;
		}
		if( s_renderingEngine ) s_renderingEngine->initialize();
	}
	return s_renderingEngine;
}

zpRenderingEngine* zpRenderingFactory::getRenderingEngine() {
	ZP_ASSERT_RETURN_( s_renderingEngine, ZP_NULL, "Rendering Engine not created!" );
	return s_renderingEngine;
}

void zpRenderingFactory::destroyRenderingEngine() {
	if( s_renderingEngine ) {
		s_renderingEngine->shutdown();
		delete s_renderingEngine;
		s_renderingEngine = ZP_NULL;
	}
	
}