#include "zpRendering.h"

zpRenderingPipeline::zpRenderingPipeline()
	: m_engine( zpRenderingFactory::getRenderingEngine() )
{

}
zpRenderingPipeline::~zpRenderingPipeline()
{
	m_engine = ZP_NULL;
}

zpRenderingEngine* zpRenderingPipeline::getRenderingEngine() const
{
	return m_engine;
}

void zpRenderingPipeline::submitRendering()
{
	zpTexture* t = m_engine->getBackBufferRenderTarget();
	zpRenderingContext* i = m_engine->getImmediateRenderingContext();

	i->clearRenderTarget( t, zpColor4f( 1, 0, 0, 1 ) );

	i->processCommands();
}
void zpRenderingPipeline::submitDebugRendering()
{

}

void zpRenderingPipeline::finalize()
{
	m_engine->present();
}