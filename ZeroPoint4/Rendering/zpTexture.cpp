#include "zpRendering.h"
#include "zpRenderingImpl.inl"
#include "Common/zpCommon.h"

zpTexture::zpTexture()
	: m_textureImpl( ZP_NULL )
{}
zpTexture::~zpTexture()
{
	m_textureImpl = ZP_NULL;
}

zpTextureDimension zpTexture::getTextureDimension() const
{
	return m_textureImpl->getTextureDimension();
}
zpTextureType zpTexture::getTextureType() const
{
	return m_textureImpl->getTextureType();
}

zp_uint zpTexture::getWidth() const
{
	return m_textureImpl->getWidth();
}
zp_uint zpTexture::getHeight() const
{
	return m_textureImpl->getHeight();
}

zpTextureImpl* zpTexture::getTextureImpl() const
{
	return m_textureImpl;
}


zp_bool zpTextureResource::load( const zp_char* filename, zpRenderingEngine* engine )
{
	m_filename = filename;
	zp_bool loaded;

	loaded = engine->createTextureFromFile( &m_resource, m_filename );

	return loaded;
}
void zpTextureResource::unload()
{
	zpRenderingEngine* engine = zpRenderingFactory::getRenderingEngine();
	engine->destroyTexture( &m_resource );
}


zp_bool zpTextureContentManager::createResource( zpTextureResource* res, const zp_char* filename )
{
	return res->load( filename, getApplication()->getRenderPipeline()->getRenderingEngine() );
}
void zpTextureContentManager::destroyResource( zpTextureResource* res )
{
	res->unload();
}
