#include "zpOpenGL.h"

zpOpenGLDepthStencilBuffer::zpOpenGLDepthStencilBuffer() :
	m_format( ZP_DISPLAY_FORMAT_UNKNOWN ),
	m_referenceCount( 1 ),
	m_width( 0 ),
	m_height( 0 ),
	m_framebuffer( 0 ),
	m_texture( 0 )
{}
zpOpenGLDepthStencilBuffer::zpOpenGLDepthStencilBuffer( zpDisplayFormat format, zp_uint framebuffer, zp_uint texture, zp_uint width, zp_uint height )  :
	m_format( format ),
	m_referenceCount( 1 ),
	m_width( width ),
	m_height( height ),
	m_framebuffer( framebuffer ),
	m_texture( texture )
{}
zpOpenGLDepthStencilBuffer::~zpOpenGLDepthStencilBuffer() {}

zp_uint zpOpenGLDepthStencilBuffer::getWidth() const {
	return m_width;
}
zp_uint zpOpenGLDepthStencilBuffer::getHeight() const {
	return m_height;
}

zpDisplayFormat zpOpenGLDepthStencilBuffer::getDisplayFormat() const {
	return m_format;
}

void zpOpenGLDepthStencilBuffer::addReference() const {
	++m_referenceCount;
}
zp_bool zpOpenGLDepthStencilBuffer::removeReference() const {
	if( m_referenceCount == 0 ) return true;
	--m_referenceCount;

	return m_referenceCount == 0;
}

zp_uint zpOpenGLDepthStencilBuffer::getReferenceCount() const {
	return m_referenceCount;
}

void zpOpenGLDepthStencilBuffer::markForAutoDelete( zp_bool marked ) const {}
zp_bool zpOpenGLDepthStencilBuffer::isMarkedForAutoDelete() const {
	return false;
}

zp_uint zpOpenGLDepthStencilBuffer::getFramebuffer() const {
	return m_framebuffer;
}
zp_uint zpOpenGLDepthStencilBuffer::getTexture() const {
	return m_texture;
}