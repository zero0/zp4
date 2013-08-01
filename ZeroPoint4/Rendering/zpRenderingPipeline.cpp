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

void zpRenderingPipeline::beginFrame()
{
	zpRenderingContext* i = m_engine->getImmediateRenderingContext();
	i->clearState();

	i->preprocessCommands();
}

void zpRenderingPipeline::submitRendering()
{
	zpTexture* t = m_engine->getBackBufferRenderTarget();
	zpDepthStencilBuffer* d = m_engine->getBackBufferDepthStencilBuffer();

	zpRenderingContext* i = m_engine->getImmediateRenderingContext();

	i->setRenderTarget( 0, 1, &t, d );
	i->clearRenderTarget( t, zpColor4f( 1, 0, 0, 1 ) );
	//i->clearDepthStencilBuffer( 1.0f, 0 );
}
void zpRenderingPipeline::submitDebugRendering()
{

}

void zpRenderingPipeline::endFrame()
{
	zpRenderingContext* i = m_engine->getImmediateRenderingContext();
	i->finalizeCommands();

	m_engine->present();
}

zpMaterialContentManager* zpRenderingPipeline::getMaterialContentManager()
{
	return &m_materialContent;
}

zpShaderContentManager* zpRenderingPipeline::getShaderContentManager()
{
	return &m_shaderContent;
}
zpTextureContentManager* zpRenderingPipeline::getTextureContentManager()
{
	return &m_textureContent;
}

void zpRenderingPipeline::onFocusGained()
{

}
void zpRenderingPipeline::onFocusLost()
{

}
