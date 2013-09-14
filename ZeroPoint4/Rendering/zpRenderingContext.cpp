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
		m_immediateIndexBuffers.pushBack( engine->createBuffer( ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_DYNAMIC, ZP_RENDERING_IMMEDIATE_INDEX_BUFFER_SIZE, sizeof( zp_short ) ) );
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

	m_renderContextImpl->setRenderTarget( startIndex, count, targets, depthStencilBuffer );
}
	 
void zpRenderingContext::clearRenderTarget( zpTexture* renderTarget, const zpColor4f& clearColor )
{
	m_renderContextImpl->clearRenderTarget( renderTarget, clearColor );
}
void zpRenderingContext::clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil )
{
	m_renderContextImpl->clearDepthStencilBuffer( clearDepth, clearStencil );
}
void zpRenderingContext::clearState()
{
	m_renderContextImpl->clearState();
}

void zpRenderingContext::setViewport( const zpViewport& viewport )
{
	m_renderContextImpl->setViewport( viewport );
}
void zpRenderingContext::setScissorRect( const zpRecti& rect )
{
	m_renderContextImpl->setScissorRect( rect );
	//m_scissorRectQueue.pushBack( rect );
}
void zpRenderingContext::resetScissorRect()
{
	//const zpRecti& rect = m_scissorRectQueue.back();
	//m_renderContextImpl->setScissorRect( rect );
	//m_scissorRectQueue.popBack();
}

void zpRenderingContext::setRasterState( zpRasterState* raster )
{
	m_renderContextImpl->setRasterState( raster );
}
void zpRenderingContext::setSamplerState( zp_uint bindSlots, zp_uint index, zpSamplerState* sampler )
{
	m_renderContextImpl->setSamplerState( bindSlots, index, sampler );
}

void zpRenderingContext::beginDrawImmediate( zpRenderingLayer layer, zpTopology topology, zpVertexFormat vertexFormat, zpMaterialResourceInstance* material )
{
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );
	m_currentCommnad = &m_renderingCommands.pushBackEmpty();
	m_currentCommnad->type = ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE;
	m_currentCommnad->layer = layer;
	m_currentCommnad->sortKey = 0;

	m_currentCommnad->topology = topology;
	m_currentCommnad->vertexBuffer = m_currentVertexBuffer->getBufferImpl();
	m_currentCommnad->indexBuffer = m_currentIndexBuffer->getBufferImpl();
	m_currentCommnad->material = material;
	m_currentCommnad->vertexFormat = vertexFormat;
	m_currentCommnad->vertexCount = 0;
	m_currentCommnad->indexCount = 0;
	m_currentCommnad->vertexOffset = 0;
	m_currentCommnad->indexOffset = 0;

	switch( vertexFormat )
	{
	case ZP_VERTEX_FORMAT_VERTEX_COLOR:
		m_currentCommnad->vertexStride = ZP_VERTEX_FORMAT_STRIDE_VERTEX_COLOR;
		break;
	case ZP_VERTEX_FORMAT_VERTEX_UV:
		m_currentCommnad->vertexStride = ZP_VERTEX_FORMAT_STRIDE_VERTEX_UV;
		break;
	case ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV:
		m_currentCommnad->vertexStride = ZP_VERTEX_FORMAT_STRIDE_VERTEX_NORMAL_UV;
		break;
	case ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2:
		m_currentCommnad->vertexStride = ZP_VERTEX_FORMAT_STRIDE_VERTEX_NORMAL_UV2;
		break;
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
	m_scratchIndexBuffer.write( index0 );
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

void zpRenderingContext::addQuad( const zpVector4f& pos0, const zpVector4f& pos1, const zpVector4f& pos2, const zpVector4f& pos3, const zpColor4f& color )
{

}
void zpRenderingContext::addQuad( 
	const zpVector4f& pos0, const zpColor4f& color0, 
	const zpVector4f& pos1, const zpColor4f& color1, 
	const zpVector4f& pos2, const zpColor4f& color2, 
	const zpVector4f& pos3, const zpColor4f& color3 )
{

}
void zpRenderingContext::addQuad( 
	const zpVector4f& pos0, const zpVector2f& uv0, 
	const zpVector4f& pos1, const zpVector2f& uv1, 
	const zpVector4f& pos2, const zpVector2f& uv2,
	const zpVector4f& pos3, const zpVector2f& uv3 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_UV, "" );

	m_scratchVertexBuffer.write( pos0 );
	m_scratchVertexBuffer.write(  uv0 );
	m_scratchVertexBuffer.write( pos1 );
	m_scratchVertexBuffer.write(  uv1 );
	m_scratchVertexBuffer.write( pos2 );
	m_scratchVertexBuffer.write(  uv2 );
	m_scratchVertexBuffer.write( pos3 );
	m_scratchVertexBuffer.write(  uv3 );

	m_scratchIndexBuffer.write< zp_short >( m_currentCommnad->vertexCount + 0 );
	m_scratchIndexBuffer.write< zp_short >( m_currentCommnad->vertexCount + 1 );
	m_scratchIndexBuffer.write< zp_short >( m_currentCommnad->vertexCount + 2 );

	m_scratchIndexBuffer.write< zp_short >( m_currentCommnad->vertexCount + 2 );
	m_scratchIndexBuffer.write< zp_short >( m_currentCommnad->vertexCount + 3 );
	m_scratchIndexBuffer.write< zp_short >( m_currentCommnad->vertexCount + 0 );

	m_currentCommnad->vertexCount += 4;
	m_currentCommnad->indexCount += 6;

	m_currentCommnad->boundingBox.add( pos0 );
	m_currentCommnad->boundingBox.add( pos1 );
	m_currentCommnad->boundingBox.add( pos2 );
	m_currentCommnad->boundingBox.add( pos3 );
}
void zpRenderingContext::addQuad( 
	const zpVector4f& pos0, const zpVector4f& normal0, const zpVector2f& uv0, 
	const zpVector4f& pos1, const zpVector4f& normal1, const zpVector2f& uv1, 
	const zpVector4f& pos2, const zpVector4f& normal2, const zpVector2f& uv2,
	const zpVector4f& pos3, const zpVector4f& normal3, const zpVector2f& uv3 )
{

}
void zpRenderingContext::addQuad( 
	const zpVector4f& pos0, const zpVector4f& normal0, const zpVector2f& uv00, const zpVector2f& uv10,
	const zpVector4f& pos1, const zpVector4f& normal1, const zpVector2f& uv01, const zpVector2f& uv11,
	const zpVector4f& pos2, const zpVector4f& normal2, const zpVector2f& uv02, const zpVector2f& uv12,
	const zpVector4f& pos3, const zpVector4f& normal3, const zpVector2f& uv03, const zpVector2f& uv13 )

{

}


void zpRenderingContext::endDrawImmediate()
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );

	m_currentCommnad->vertexOffset = m_immediateVertexSize;
	m_currentCommnad->indexOffset = m_immediateIndexSize;

	m_currentCommnad = ZP_NULL;

	m_immediateVertexSize = m_scratchVertexBuffer.size();
	m_immediateIndexSize = m_scratchIndexBuffer.size();
}

void zpRenderingContext::drawBuffered( zpRenderingLayer layer, zpTopology topology, zpVertexFormat vertexFormat, zpBuffer* vertexBuffer, zpBuffer* indexBuffer, zp_uint vertexCount, zp_uint indexCount, zpMaterialResourceInstance* material, zpBoundingAABB* boundingBox )
{
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );

	zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
	command.type = ZP_RENDERING_COMMNAD_DRAW_BUFFERED;
	command.layer = layer;
	command.sortKey = 0;
	
	command.topology = topology;
	command.vertexBuffer = vertexBuffer->getBufferImpl();
	command.indexBuffer = indexBuffer->getBufferImpl();
	command.material = material;
	command.vertexFormat = vertexFormat;
	command.vertexCount = vertexCount;
	command.indexCount = indexCount;
	command.vertexOffset = 0;
	command.indexOffset = 0;

	if( boundingBox )
	{
		command.boundingBox = *boundingBox;
	}
}

void zpRenderingContext::drawMesh( zpRenderingLayer layer, zpMeshResourceInstance* mesh, zpMaterialResourceInstance* material )
{
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );

	const zpMesh* m = mesh->getResource()->getData();
	const zpMeshPart* b = m->m_parts.begin();
	const zpMeshPart* e = m->m_parts.end();
	for( ; b != e; ++b )
	{
		zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
		command.type = ZP_RENDERING_COMMNAD_DRAW_BUFFERED;
		command.layer = layer;
		command.sortKey = 0;

		command.topology = ZP_TOPOLOGY_TRIANGLE_LIST;
		command.vertexBuffer = m->m_vertex->getBufferImpl();
		command.indexBuffer = m->m_index->getBufferImpl();
		command.material = material == ZP_NULL ? &b->m_material : material;
		command.vertexFormat = m->m_format;
		command.vertexCount = b->m_vertexCount;
		command.indexCount = b->m_vertexCount;
		command.vertexOffset = b->m_vertexOffset;
		command.indexOffset = b->m_indexOffset;
		
		switch( command.vertexFormat )
		{
		case ZP_VERTEX_FORMAT_VERTEX_COLOR:
			command.vertexStride = ZP_VERTEX_FORMAT_STRIDE_VERTEX_COLOR;
			break;
		case ZP_VERTEX_FORMAT_VERTEX_UV:
			command.vertexStride = ZP_VERTEX_FORMAT_STRIDE_VERTEX_UV;
			break;
		case ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV:
			command.vertexStride = ZP_VERTEX_FORMAT_STRIDE_VERTEX_NORMAL_UV;
			break;
		case ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2:
			command.vertexStride = ZP_VERTEX_FORMAT_STRIDE_VERTEX_NORMAL_UV2;
			break;
		}
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

void zpRenderingContext::preprocessCommands()
{
	map( m_currentVertexBuffer, (void**)&m_immediateVertexData );
	zp_memcpy( m_immediateVertexData, ZP_RENDERING_IMMEDIATE_VERTEX_BUFFER_SIZE, m_scratchVertexBuffer.getData(), m_scratchVertexBuffer.size() );
	unmap( m_currentVertexBuffer );

	map( m_currentIndexBuffer, (void**)&m_immediateIndexData );
	zp_memcpy( m_immediateIndexData, ZP_RENDERING_IMMEDIATE_INDEX_BUFFER_SIZE, m_scratchIndexBuffer.getData(), m_scratchIndexBuffer.size() );
	unmap( m_currentIndexBuffer );

	m_scratchVertexBuffer.reset();
	m_scratchIndexBuffer.reset();

	// clear filtered commands
	for( zp_uint i = 0; i < zpRenderingLayer_Count; ++i )
	{
		m_filteredCommands[ i ].reset();
	}

	// filter all commands into layer buckets
	for( zp_uint i = 0; i < m_renderingCommands.size(); ++i )
	{
		zpRenderingCommand* cmd = &m_renderingCommands[ i ];
		m_filteredCommands[ cmd->layer ].pushBack( cmd );
	}

	// sort layer buckets
	for( zp_uint i = 0; i < zpRenderingLayer_Count; ++i )
	{
		m_filteredCommands[ i ].sort( []( zpRenderingCommand* cmd0, zpRenderingCommand* cmd1 ) {
			return cmd0->sortKey < cmd1->sortKey;
		} );
	}
}

void zpRenderingContext::processCommands( zpRenderingLayer layer )
{
	m_renderContextImpl->processCommands( m_renderingEngine->getRenderingEngineImpl(), m_filteredCommands[ layer ] );
}

void zpRenderingContext::finalizeCommands()
{
	m_renderingCommands.reset();

	m_currentBufferIndex = ( m_currentBufferIndex + 1 ) % ZP_RENDERING_MAX_IMMEDIATE_SWAP_BUFFERS;
	m_currentVertexBuffer = m_immediateVertexBuffers[ m_currentBufferIndex ];
	m_currentIndexBuffer = m_immediateIndexBuffers[ m_currentBufferIndex ];

	m_immediateVertexSize = 0;
	m_immediateIndexSize = 0;
}
