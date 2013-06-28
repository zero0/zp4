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
	zpDepthStencilBuffer* d = m_engine->getBackBufferDepthStencilBuffer();

	zpRenderingContext* i = m_engine->getImmediateRenderingContext();

	i->setRenderTarget( ZP_RENDERING_LAYER_OPAQUE, 0, 1, &t, d );
	i->clearRenderTarget( ZP_RENDERING_LAYER_OPAQUE, t, zpColor4f( 1, 0, 0, 1 ) );



	i->setRenderTarget( ZP_RENDERING_LAYER_UI_OPAQUE, 0, 1, &t, d );
	i->clearRenderTarget( ZP_RENDERING_LAYER_UI_OPAQUE, t, zpColor4f( 0, 1, 0, 1 ) );

	i->processCommands();
}
void zpRenderingPipeline::submitDebugRendering()
{

}

void zpRenderingPipeline::finalize()
{
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
