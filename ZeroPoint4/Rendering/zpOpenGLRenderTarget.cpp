#include "zpRendering.h"
#include "zpOpenGL.h"


zpOpenGLRenderTarget::zpOpenGLRenderTarget() :
	m_referenceCount( 1 ),
	m_width( 0 ),
	m_height( 0 ),
	m_framebuffers( 0 ),
	m_textures( 0 )
{}
zpOpenGLRenderTarget::zpOpenGLRenderTarget( zp_uint* framebuffers, zp_uint* textures, zp_uint count, zp_uint width, zp_uint height ) :
	m_referenceCount( 1 ),
	m_width( width ),
	m_height( height ),
	m_framebuffers( count ),
	m_textures( count )
{
	m_framebuffers.fill( framebuffers, count );
	m_textures.fill( textures, count );
}
zpOpenGLRenderTarget::~zpOpenGLRenderTarget() {}

zp_uint zpOpenGLRenderTarget::getWidth() const {
	return m_width;
}
zp_uint zpOpenGLRenderTarget::getHeight() const {
	return m_height;
}

zp_uint zpOpenGLRenderTarget::getDisplayFormat( zpDisplayFormat* formats ) const {
	return 0;
}

zp_uint zpOpenGLRenderTarget::getNumberOfTargets() const {
	return m_framebuffers.size();
}

void zpOpenGLRenderTarget::addReference() const {
	++m_referenceCount;
}
zp_bool zpOpenGLRenderTarget::removeReference() const {
	if( m_referenceCount == 0 ) return true;
	--m_referenceCount;

	return m_referenceCount == 0;
}

zp_uint zpOpenGLRenderTarget::getReferenceCount() const {
	return m_referenceCount;
}

void zpOpenGLRenderTarget::markForAutoDelete( zp_bool marked ) const {}
zp_bool zpOpenGLRenderTarget::isMarkedForAutoDelete() const {
	return false;
}

zpArray<zp_uint>& zpOpenGLRenderTarget::getFramebuffers() {
	return m_framebuffers;
}
zpArray<zp_uint>& zpOpenGLRenderTarget::getTextures() {
	return m_textures;
}