#include "zpRendering.h"
#include "zpOpenGL.h"

zpOpenGLRenderingContext::zpOpenGLRenderingContext() :
	m_context( ZP_NULL ),
	m_renderTarget( ZP_NULL ),
	m_depthStencilBuffer( ZP_NULL ),
	m_referenceCount( 1 ),
	m_name()
{}
zpOpenGLRenderingContext::zpOpenGLRenderingContext( void* context, const zpString& name ) :
	m_context( context ),
	m_renderTarget( ZP_NULL ),
	m_depthStencilBuffer( ZP_NULL ),
	m_referenceCount( 1 ),
	m_name( name )
{}
zpOpenGLRenderingContext::~zpOpenGLRenderingContext() {}

const zpString& zpOpenGLRenderingContext::getName() const {
	return m_name;
}

void zpOpenGLRenderingContext::setRenderTarget( zpRenderTarget* target ) {}
zpRenderTarget* zpOpenGLRenderingContext::getRenderTarget() const {
	return m_renderTarget;
}

void zpOpenGLRenderingContext::setDepthStencilBuffer( zpDepthStencilBuffer* depthBuffer ) {}
zpDepthStencilBuffer* zpOpenGLRenderingContext::getDepthStencilBuffer() const {
	return m_depthStencilBuffer;
}

void zpOpenGLRenderingContext::clearRenderTarget( const zpColor4f* colors, zp_uint count ) {}
void zpOpenGLRenderingContext::clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil ) {}

void zpOpenGLRenderingContext::bindBuffer( zpBuffer* buffer, zp_uint slot ) {}
void zpOpenGLRenderingContext::unbindBuffer( zpBuffer* buffer, zp_uint slot ) {}

void zpOpenGLRenderingContext::bindBuffers( zp_uint count, zpBuffer** buffers, zp_uint slot ) {}
void zpOpenGLRenderingContext::unbindBuffers( zp_uint count, zpBuffer** buffers, zp_uint slot ) {}

void zpOpenGLRenderingContext::setVertexLayout( zpVertexLayout* layout ) {}

void zpOpenGLRenderingContext::map( zpBuffer* buffer, zpMapType mapType, zp_uint subResource, void** data ) {}
void zpOpenGLRenderingContext::unmap( zpBuffer* buffer, zp_uint subResource ) {}

void zpOpenGLRenderingContext::bindShader( zpShaderResource* shader ) {}
void zpOpenGLRenderingContext::unbindShader( zpShaderResource* shader ) {}

void zpOpenGLRenderingContext::setTopology( zpTopology topology ) {}

void zpOpenGLRenderingContext::draw( zp_uint vertexCount, zp_uint startIndex ) {}

void zpOpenGLRenderingContext::setViewport( const zpViewport& viewport ) {}


void zpOpenGLRenderingContext::addReference() const {
	++m_referenceCount;
}
zp_bool zpOpenGLRenderingContext::removeReference() const {
	if( m_referenceCount == 0 ) return true;
	--m_referenceCount;
	return m_referenceCount > 0;
}

zp_uint zpOpenGLRenderingContext::getReferenceCount() const {
	return m_referenceCount;
}

void zpOpenGLRenderingContext::markForAutoDelete( zp_bool marked ) const {}
zp_bool zpOpenGLRenderingContext::isMarkedForAutoDelete() const {
	return false;
}