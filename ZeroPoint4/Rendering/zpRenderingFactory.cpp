#include "zpRendering.h"

zpRenderingFactory::zpRenderingFactory() {}
zpRenderingFactory::~zpRenderingFactory() {}

zpRenderingEngine* zpRenderingFactory::s_renderingEngine = ZP_NULL;
zpRenderingEngine* zpRenderingFactory::createRenderingEngine( zpRenderingEngineType type ) {
	if( !s_renderingEngine ) {
		switch( type ) {
		case ZP_RENDERING_ENGINE_DX:
			s_renderingEngine = new zpDX11RenderingEngine;
			break;
		}
		if( s_renderingEngine ) s_renderingEngine->initialize();
	}
	return s_renderingEngine;
}

void zpRenderingFactory::destroyRenderingEngine( zpRenderingEngine* engine ) {
	delete engine;
}