#include "zpRendering.h"
#include "zpRenderingImpl.inl"

zpTexture::zpTexture( zpTextureImpl* textureImpl )
	: m_textureImpl( textureImpl )
{}
zpTexture::~zpTexture()
{
	ZP_SAFE_DELETE( m_textureImpl );
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


zp_bool zpTextureResource::load( const zp_char* filename )
{
	zpRenderingEngine* engine = zpRenderingFactory::getRenderingEngine();
	if( m_resource == ZP_NULL )
	{
	}

	return false;
}
void zpTextureResource::unload()
{
	if( m_resource != ZP_NULL )
	{

	}
}


zp_bool zpTextureContentManager::createResource( zpTextureResource* res, const zp_char* filename )
{
	return res->load( filename );
}
void zpTextureContentManager::destroyResource( zpTextureResource* res )
{
	res->unload();
}
