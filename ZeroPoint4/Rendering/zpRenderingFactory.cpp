#include "zpRendering.h"

#if 0
zpRenderingFactory::zpRenderingFactory()
{}
zpRenderingFactory::~zpRenderingFactory()
{}

zpRenderingEngine* zpRenderingFactory::s_renderingEngine = ZP_NULL;
zpRenderingEngine* zpRenderingFactory::getRenderingEngine()
{
    if( !s_renderingEngine )
    {
        s_renderingEngine = new zpRenderingEngine;
        s_renderingEngine->initialize();
    }
    return s_renderingEngine;
}

void zpRenderingFactory::destroyRenderingEngine()
{
    if( s_renderingEngine )
    {
        s_renderingEngine->shutdown();
        delete s_renderingEngine;
        s_renderingEngine = ZP_NULL;
    }
    
}
#endif
