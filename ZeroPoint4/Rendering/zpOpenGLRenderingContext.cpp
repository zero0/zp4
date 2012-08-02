#include "zpRendering.h"
#include "zpOpenGL.h"

zpOpenGLRenderingContext::zpOpenGLRenderingContext() :
	m_context( ZP_NULL ),
	m_hdc( ZP_NULL ),
	m_renderTarget( ZP_NULL ),
	m_depthStencilBuffer( ZP_NULL ),
	m_referenceCount( 1 ),
	m_name()
{}
zpOpenGLRenderingContext::zpOpenGLRenderingContext( void* context, void* hdc, const zpString& name ) :
	m_context( context ),
	m_hdc( hdc ),
	m_renderTarget( ZP_NULL ),
	m_depthStencilBuffer( ZP_NULL ),
	m_referenceCount( 1 ),
	m_name( name )
{}
zpOpenGLRenderingContext::~zpOpenGLRenderingContext() {}

const zpString& zpOpenGLRenderingContext::getName() const {
	return m_name;
}

void zpOpenGLRenderingContext::setRenderTarget( zpRenderTarget* target ) {
	if( m_renderTarget ) m_renderTarget->removeReference();
	if( target ) target->addReference();

	m_renderTarget = target;

	wglMakeCurrent( (HDC)m_hdc, (HGLRC)m_context );
}
zpRenderTarget* zpOpenGLRenderingContext::getRenderTarget() const {
	return m_renderTarget;
}

void zpOpenGLRenderingContext::setDepthStencilBuffer( zpDepthStencilBuffer* depthBuffer ) {
	if( m_depthStencilBuffer ) m_depthStencilBuffer->removeReference();
	if( depthBuffer ) depthBuffer->addReference();

	m_depthStencilBuffer = depthBuffer;
}
zpDepthStencilBuffer* zpOpenGLRenderingContext::getDepthStencilBuffer() const {
	return m_depthStencilBuffer;
}

void zpOpenGLRenderingContext::bindRenderTargetAndDepthBuffer() {}
void zpOpenGLRenderingContext::unbindRenderTargetAndDepthBuffer() {}

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