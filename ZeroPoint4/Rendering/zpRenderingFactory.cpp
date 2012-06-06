#include "zpRendering.h"

zpRenderingFactory::zpRenderingFactory() {}
zpRenderingFactory::~zpRenderingFactory() {}

zpRenderingEngine* zpRenderingFactory::createRenderingEngine( zpRenderingEngineType type ) {
	switch( type ) {
	case ZP_RENDERING_ENGINE_DX:
		return new zpDX11RenderingEngine;
	}
	return ZP_NULL;
}

void zpRenderingFactory::destroyRenderingEngine( zpRenderingEngine* engine ) {
	delete engine;
}