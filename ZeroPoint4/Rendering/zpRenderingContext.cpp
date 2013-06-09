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
	, m_immediateVertexData( ZP_NULL )
	, m_immediateIndexData( ZP_NULL )
	, m_immediateVertexSize( 0 )
	, m_immediateIndexSize( 0 )
	, m_currentBufferIndex( 0 )
	, m_currentVertexBuffer( ZP_NULL )
	, m_currentIndexBuffer( ZP_NULL )
{
	for( zp_uint i = 0; i < ZP_RENDERING_MAX_IMMEDIATE_SWAP_BUFFERS; ++i )
	{
		m_immediateVertexBuffers.pushBack( engine->createBuffer( ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_DYNAMIC, ZP_RENDERING_IMMEDIATE_VERTEX_BUFFER_SIZE ) );
		m_immediateIndexBuffers.pushBack( engine->createBuffer( ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_DYNAMIC, ZP_RENDERING_IMMEDIATE_VERTEX_BUFFER_SIZE, sizeof( zp_short ) ) );
	}

	m_currentVertexBuffer = m_immediateVertexBuffers[ m_currentBufferIndex ];
	m_currentIndexBuffer = m_immediateIndexBuffers[ m_currentBufferIndex ];
}
zpRenderingContext::~zpRenderingContext()
{
	m_renderingCommands.clear();
	m_immediateVertexBuffers.clear();
	m_immediateIndexBuffers.clear();
}

void zpRenderingContext::setRenderTarget( zp_uint startIndex, zp_uint count, zpTexture** targets, zpDepthStencilBuffer* depthStencilBuffer )
{
	ZP_ASSERT( ( startIndex + count ) < ZP_RENDER_TARGET_MAX_COUNT, "Too many render targets set, max of %d", ZP_RENDER_TARGET_MAX_COUNT );
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );

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
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );

	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_CLEAR_RT;

	command.clearColor = clearColor;
	command.clearRenderTarget = renderTarget;
}
void zpRenderingContext::clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil )
{
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );

	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_CLEAR_DEPTH_STENCIL;

	command.clearDepth = clearDepth;
	command.clearStencil = clearStencil;
}
void zpRenderingContext::clearState()
{
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );

	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_CLEAR_STATE;
}

void zpRenderingContext::setViewport( const zpViewport& viewport )
{
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );

	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_SET_VIEWPORT;

	command.viewport = viewport;
}
void zpRenderingContext::setScissorRect( const zpRecti& rect )
{
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );

	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_SET_SCISSOR_RECT;

	command.scissor = rect;
}

void zpRenderingContext::setRasterState( zpRasterState* raster )
{
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );

	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_SET_RASTER_STATE;

	command.rasterState = raster;
}
void zpRenderingContext::setSamplerState( zp_uint bindSlots, zp_uint index, zpSamplerState* sampler )
{
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );

	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_SET_SAMPLER_STATE;

	command.samplerStateBind = bindSlots;
	command.samplerIndex = index;
	command.samplerState = sampler;
}

void zpRenderingContext::beginDrawImmediate( zpRenderingLayer layer, zpTopology topology, zpVertexFormat vertexFormat )
{
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );
	m_currentCommnad = &m_renderingCommands.pushBackEmpty();
	m_currentCommnad->type = ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE;

	m_currentCommnad->layer = layer;
	m_currentCommnad->topology = topology;
	m_currentCommnad->vertexBuffer = m_currentVertexBuffer->getBufferImpl();
	m_currentCommnad->indexBuffer = m_currentIndexBuffer->getBufferImpl();
	m_currentCommnad->vertexFormat = vertexFormat;
	m_currentCommnad->vertexCount = 0;
	m_currentCommnad->indexCount = 0;
	m_currentCommnad->vertexOffset = 0;
	m_currentCommnad->indexOffset = 0;

	switch( vertexFormat )
	{
	case ZP_VERTEX_FORMAT_VERTEX_COLOR:
		m_currentCommnad->vertexStride = sizeof( zpVertexPositionColor );
		break;
	case ZP_VERTEX_FORMAT_VERTEX_UV:
		m_currentCommnad->vertexStride = sizeof( zpVertexPositionUV );
		break;
	case ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV:
		m_currentCommnad->vertexStride = sizeof( zpVertexPositionNormalUV );
		break;
	case ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2:
		m_currentCommnad->vertexStride = sizeof( zpVertexPositionNormalUV2 );
		break;
	}

	if( m_immediateVertexData == ZP_NULL )
	{
		map( m_currentVertexBuffer, (void**)&m_immediateVertexData );
		map( m_currentIndexBuffer, (void**)&m_immediateIndexData );
	}
}

void zpRenderingContext::addVertex( const zpVector4f& pos, const zpColor4f& color )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

	m_scratchVertexBuffer.write( pos );
	m_scratchVertexBuffer.write( color );
	m_currentCommnad->vertexCount += 1;

	m_currentCommnad->boundingBox.add( pos );
}
void zpRenderingContext::addVertex( const zpVector4f& pos, const zpVector2f& uv0 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_UV, "" );

	m_scratchVertexBuffer.write( pos );
	m_scratchVertexBuffer.write( uv0 );
	m_currentCommnad->vertexCount += 1;

	m_currentCommnad->boundingBox.add( pos );
}
void zpRenderingContext::addVertex( const zpVector4f& pos, const zpVector4f& normal, const zpVector2f& uv0 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV, "" );

	m_scratchVertexBuffer.write( pos );
	m_scratchVertexBuffer.write( normal );
	m_scratchVertexBuffer.write( uv0 );
	m_currentCommnad->vertexCount += 1;

	m_currentCommnad->boundingBox.add( pos );
}
void zpRenderingContext::addVertex( const zpVector4f& pos, const zpVector4f& normal, const zpVector2f& uv0, const zpVector2f& uv1 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2, "" );

	m_scratchVertexBuffer.write( pos );
	m_scratchVertexBuffer.write( normal );
	m_scratchVertexBuffer.write( uv0 );
	m_scratchVertexBuffer.write( uv1 );
	m_currentCommnad->vertexCount += 1;

	m_currentCommnad->boundingBox.add( pos );
}

void zpRenderingContext::addLineIndex( zp_short index0, zp_short index1 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );

	m_scratchIndexBuffer.write( index0 );
	m_scratchIndexBuffer.write( index1 );
	m_currentCommnad->indexCount += 2;
}
void zpRenderingContext::addTriangleIndex( zp_short index0, zp_short index1, zp_short index2 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );

	m_scratchIndexBuffer.write( index0 );
	m_scratchIndexBuffer.write( index1 );
	m_scratchIndexBuffer.write( index2 );
	m_currentCommnad->indexCount += 3;
}
void zpRenderingContext::addQuadIndex( zp_short index0, zp_short index1, zp_short index2, zp_short index3 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );

	m_scratchIndexBuffer.write( index0 );
	m_scratchIndexBuffer.write( index1 );
	m_scratchIndexBuffer.write( index2 );

	m_scratchIndexBuffer.write( index2 );
	m_scratchIndexBuffer.write( index3 );
	m_scratchIndexBuffer.write( index1 );
	m_currentCommnad->indexCount += 6;
}

void zpRenderingContext::addLine( const zpVector4f& pos0, const zpVector4f& pos1, const zpColor4f& color )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

	m_scratchVertexBuffer.write( pos0 );
	m_scratchVertexBuffer.write( color );
	m_scratchVertexBuffer.write( pos1 );
	m_scratchVertexBuffer.write( color );

	m_scratchIndexBuffer.write< zp_short >( m_currentCommnad->vertexCount + 0 );
	m_scratchIndexBuffer.write< zp_short >( m_currentCommnad->vertexCount + 1 );

	m_currentCommnad->vertexCount += 2;
	m_currentCommnad->indexCount += 2;

	m_currentCommnad->boundingBox.add( pos0 );
	m_currentCommnad->boundingBox.add( pos1 );
}

void zpRenderingContext::addLine( const zpVector4f& pos0, const zpColor4f& color0, const zpVector4f& pos1, const zpColor4f& color1 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

	m_scratchVertexBuffer.write( pos0 );
	m_scratchVertexBuffer.write( color0 );
	m_scratchVertexBuffer.write( pos1 );
	m_scratchVertexBuffer.write( color1 );

	m_scratchIndexBuffer.write< zp_short >( m_currentCommnad->vertexCount + 0 );
	m_scratchIndexBuffer.write< zp_short >( m_currentCommnad->vertexCount + 1 );
	
	m_currentCommnad->vertexCount += 2;
	m_currentCommnad->indexCount += 2;

	m_currentCommnad->boundingBox.add( pos0 );
	m_currentCommnad->boundingBox.add( pos1 );
}

void zpRenderingContext::endDrawImmediate()
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );

	zp_memcpy( m_immediateVertexData + m_immediateVertexSize, ZP_RENDERING_IMMEDIATE_VERTEX_BUFFER_SIZE - m_immediateVertexSize, m_scratchVertexBuffer.getData(), m_scratchVertexBuffer.size() );
	zp_memcpy( m_immediateIndexData + m_immediateIndexSize, ZP_RENDERING_IMMEDIATE_INDEX_BUFFER_SIZE - m_immediateIndexSize, m_scratchIndexBuffer.getData(), m_scratchIndexBuffer.size() );

	m_currentCommnad->vertexOffset = m_immediateVertexSize;
	m_currentCommnad->indexOffset = m_immediateIndexSize;

	m_immediateVertexSize += m_scratchVertexBuffer.size();
	m_immediateIndexSize += m_scratchIndexBuffer.size();

	m_currentCommnad = ZP_NULL;
	m_scratchVertexBuffer.reset();
	m_scratchIndexBuffer.reset();
}

void zpRenderingContext::drawBuffered( zpRenderingLayer layer, zpTopology topology, zpVertexFormat vertexFormat, zpBuffer* vertexBuffer, zpBuffer* indexBuffer, zp_uint vertexCount, zp_uint indexCount, zpBoundingAABB* boundingBox )
{
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );

	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_DRAW_BUFFERED;

	command.layer = layer;
	command.topology = topology;
	command.vertexBuffer = vertexBuffer->getBufferImpl();
	command.indexBuffer = indexBuffer->getBufferImpl();
	command.vertexFormat = vertexFormat;
	command.vertexCount = vertexCount;
	command.indexCount = indexCount;
	command.vertexOffset = 0;
	command.indexOffset = 0;

	if( boundingBox )
	{
		command.boundingBox.add( *boundingBox );
	}
}

void zpRenderingContext::map( zpBuffer* buffer, void** data, zpMapType mapType, zp_uint subResource )
{
	m_renderContextImpl->map( buffer->getBufferImpl(), data, mapType, subResource );
}
void zpRenderingContext::unmap( zpBuffer* buffer, zp_uint subResource )
{
	m_renderContextImpl->unmap( buffer->getBufferImpl(), subResource );
}

void zpRenderingContext::update( zpBuffer* buffer, void* data, zp_uint size )
{
	m_renderContextImpl->update( buffer->getBufferImpl(), data, size );
}

void zpRenderingContext::processCommands()
{
	if( m_immediateVertexData )
	{
		unmap( m_currentVertexBuffer );
		unmap( m_currentIndexBuffer );

		m_immediateVertexData = ZP_NULL;
		m_immediateIndexData = ZP_NULL;
	}

	m_renderContextImpl->processCommands( m_renderingEngine->getRenderingEngineImpl(), m_renderingCommands );

	m_renderingCommands.reset();

	m_currentBufferIndex = ( m_currentBufferIndex + 1 ) % ZP_RENDERING_MAX_IMMEDIATE_SWAP_BUFFERS;
	m_currentVertexBuffer = m_immediateVertexBuffers[ m_currentBufferIndex ];
	m_currentIndexBuffer = m_immediateIndexBuffers[ m_currentBufferIndex ];

	m_immediateVertexSize = 0;
	m_immediateIndexSize = 0;
}
