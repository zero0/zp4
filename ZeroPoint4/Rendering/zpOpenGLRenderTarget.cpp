#include "zpRendering.h"
#include "zpOpenGL.h"


zpOpenGLRenderTarget::zpOpenGLRenderTarget() :
	m_referenceCount( 1 ),
	m_width( 0 ),
	m_height( 0 ),
	m_frameBuffer( 0 ),
	m_renderBuffers( 0 )
{}
zpOpenGLRenderTarget::zpOpenGLRenderTarget( zp_uint frameBuffer, zp_uint* renderBuffers, zp_uint count, zp_uint width, zp_uint height ) :
	m_referenceCount( 1 ),
	m_width( width ),
	m_height( height ),
	m_frameBuffer( frameBuffer ),
	m_renderBuffers( count )
{
	m_renderBuffers.fill( renderBuffers, count );
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
	return m_renderBuffers.size();
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

zp_uint zpOpenGLRenderTarget::getFrameBuffer() {
	return m_frameBuffer;
}
zpArray<zp_uint>& zpOpenGLRenderTarget::getRenderBuffers() {
	return m_renderBuffers;
}