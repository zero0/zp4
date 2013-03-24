#include "zpRendering.h"

#if ZP_RENDERING_TYPE == ZP_DX11
#include "RenderingDX/zpDX11.h"
#elif ZP_RENDERING_TYPE == ZP_GL2
#include "RenderingOpenGL/zpOpenGL.h"
#else
#error( "No rendering engine selected!" )
#endif

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