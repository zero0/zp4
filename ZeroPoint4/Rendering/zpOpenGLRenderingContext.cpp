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

void zpOpenGLRenderingContext::bindRenderTargetAndDepthBuffer() {
	wglMakeCurrent( (HDC)m_hdc, (HGLRC)m_context );

	//( (zpOpenGLRenderTarget*)m_renderTarget )->getFramebuffers().foreachIndexed( []( zp_uint index, zp_uint framebuffer ) {
	//	glBindFramebuffer( GL_COLOR_ATTACHMENT0 + index, framebuffer );
	//} );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, ( (zpOpenGLRenderTarget*)m_renderTarget )->getFrameBuffer() );
	//glBindFramebuffer( GL_DRAW_FRAMEBUFFER, ( (zpOpenGLDepthStencilBuffer*)m_depthStencilBuffer )->getFramebuffer() );
	glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ( (zpOpenGLRenderTarget*)m_renderTarget )->getRenderBuffers()[0] );
	glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ( (zpOpenGLDepthStencilBuffer*)m_depthStencilBuffer )->getTexture() );
	
	checkError();
}
void zpOpenGLRenderingContext::unbindRenderTargetAndDepthBuffer() {
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
}

void zpOpenGLRenderingContext::clearRenderTarget( const zpColor4f* colors, zp_uint count ) {
	//for( zp_uint i = 0; i < count; ++i ) {
	//	glClearBufferfv( GL_COLOR, GL_DRAW_BUFFER0, colors[0] );
	//}
	glClearColor( colors[0].getRed(), colors[0].getGreen(), colors[0].getBlue(), colors[0].getAlpha() );
	glClear( GL_COLOR_BUFFER_BIT );

	checkError();
}
void zpOpenGLRenderingContext::clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil ) {
	//glClearBufferfi( GL_DEPTH_STENCIL, 0, clearDepth, clearStencil );
	glClearDepth( clearDepth );
	glClearStencil( clearStencil );
	glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	checkError();
}

void zpOpenGLRenderingContext::bindBuffer( zpBuffer* buffer, zp_uint slot ) {
	zpOpenGLBuffer* buff = (zpOpenGLBuffer*)buffer;
	glBindBuffer( buff->getTarget(), buff->getBuffer() );

	checkError();
}
void zpOpenGLRenderingContext::unbindBuffer( zpBuffer* buffer, zp_uint slot ) {
	zpOpenGLBuffer* buff = (zpOpenGLBuffer*)buffer;
	glBindBuffer( buff->getTarget(), 0 );
}

void zpOpenGLRenderingContext::bindBuffers( zp_uint count, zpBuffer** buffers, zp_uint slot ) {
}
void zpOpenGLRenderingContext::unbindBuffers( zp_uint count, zpBuffer** buffers, zp_uint slot ) {}

void zpOpenGLRenderingContext::bindTexture( zpResourceBindType bindType, zp_uint slot, zpTexture* texture ) {}
void zpOpenGLRenderingContext::unbindTexture( zpResourceBindType bindType, zp_uint slot, zpTexture* texture ) {}

void zpOpenGLRenderingContext::setVertexLayout( zpVertexLayout* layout ) {}

void zpOpenGLRenderingContext::map( zpBuffer* buffer, zpMapType mapType, zp_uint subResource, void** data ) {
	buffer->map( data, mapType, subResource );
}
void zpOpenGLRenderingContext::unmap( zpBuffer* buffer, zp_uint subResource ) {
	buffer->unmap( subResource );
}

void zpOpenGLRenderingContext::bindShader( zpShaderResource* shader ) {
	glUseProgram( ( (zpOpenGLShaderResource*)shader )->getShaderProgram() );
}
void zpOpenGLRenderingContext::unbindShader( zpShaderResource* shader ) {
	glUseProgram( 0 );
}

void zpOpenGLRenderingContext::setTopology( zpTopology topology ) {
	m_topology = __zpToGL( topology );
}

void zpOpenGLRenderingContext::draw( zp_uint vertexCount, zp_uint startIndex ) {
	glDrawArrays( m_topology, startIndex, vertexCount );

	checkError();
}

void zpOpenGLRenderingContext::setViewport( const zpViewport& viewport ) {
	glViewport( (zp_int)viewport.getTopX(), (zp_int)viewport.getTopY(), (zp_uint)viewport.getWidth(), (zp_uint)viewport.getHeight() );
}

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