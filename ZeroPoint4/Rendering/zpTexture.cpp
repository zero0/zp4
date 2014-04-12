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
	loaded = engine->createTextureFromFile( m_resource, m_filename );

	ZP_ASSERT( loaded, "Failed to load texture '%s'", m_filename.str() );
	return loaded;
}
void zpTextureResource::unload( zpRenderingEngine* engine )
{
	zp_bool ok;
	ZP_UNUSED( ok );

	ok = engine->destroyTexture( m_resource );
	ZP_ASSERT( ok, "Failed to unload texture '%s'", m_filename.str() );
}


zp_bool zpTextureContentManager::getResourceWithoutLoadTexture( zpTextureResourceInstance& outInstance, zp_uint width, zp_uint height, zpTextureType type, zpTextureDimension dimension, zpDisplayFormat format, zpCpuAccess access, void* data, zp_uint mipLevels )
{
	zp_bool ok;

	zpTexture* texture;
	ok = getResourceWithoutLoad( outInstance, texture );
	ZP_ASSERT( ok, "Failed to get texture resource without load" );

	ok = getApplication()->getRenderPipeline()->getRenderingEngine()->createTexture( *texture, width, height, type, dimension, format, access, data, mipLevels );
	ZP_ASSERT( ok, "Failed to create texture" );

	return ok;
}

zp_bool zpTextureContentManager::createResource( zpTextureResource* res, const zp_char* filename )
{
	zpRenderingEngine* engine = getApplication()->getRenderPipeline()->getRenderingEngine();
	return res->load( filename, engine );
}
void zpTextureContentManager::destroyResource( zpTextureResource* res )
{
	zpRenderingEngine* engine = getApplication()->getRenderPipeline()->getRenderingEngine();
	res->unload( engine );
}
