#include "zpRendering.h"

#if ZP_RENDERING_TYPE == ZP_DX11
#include "RenderingDX/zpDX11.h"
#elif ZP_RENDERING_TYPE == ZP_GL2
#include "RenderingOpenGL/zpOpenGL.h"
#else
#error( "No rendering engine selected!" )
#endif

zpRenderingContext::zpRenderingContext( zpRenderingEngine* engine, zpRenderingContextImpl* impl )
	: m_renderContextImpl( impl )
	, m_renderingEngine( engine )
	, m_currentCommnad( ZP_NULL )
{}

void zpRenderingContext::setRenderTarget( zp_uint startIndex, zp_uint count, zpTexture** targets, zpDepthStencilBuffer* depthStencilBuffer )
{
	ZP_ASSERT( ( startIndex + count ) < ZP_RENDER_TARGET_MAX_COUNT, "" );

	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_SET_RT;

	zp_uint i;
	for( i = 0; i < startIndex; ++i )
	{
		command.renderTargets[ i ] = ZP_NULL;
	}
	for( ; i < count; ++i )
	{
		command.renderTargets[ i ] = targets[ i ];
	}
	for( ; i < ZP_RENDER_TARGET_MAX_COUNT; ++i )
	{
		command.renderTargets[ i ] = ZP_NULL;
	}
	
	command.depthStencilBuffer = depthStencilBuffer;
}
	 
void zpRenderingContext::clearRenderTarget( zpTexture* renderTarget, const zpColor4f& clearColor )
{
	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_CLEAR_RT;

	command.clearColor = clearColor;
	command.clearRenderTarget = renderTarget;
}
void zpRenderingContext::clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil )
{
	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_CLEAR_DEPTH_STENCIL;

	command.clearDepth = clearDepth;
	command.clearStencil = clearStencil;
}

void zpRenderingContext::setViewport( const zpViewport& viewport )
{
	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_SET_VIEWPORT;

	command.viewport = viewport;
}
void zpRenderingContext::setScissorRect( const zpRecti& rect )
{
	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_SET_SCISSOR_RECT;

	command.scissor = rect;
}

void zpRenderingContext::setRasterState( zpRasterState* raster )
{
	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_SET_RASTER_STATE;

	command.rasterState = raster;
}

void zpRenderingContext::processCommands()
{
	m_renderContextImpl->processCommands( m_renderingCommands );

	m_renderingCommands.reset();
}
