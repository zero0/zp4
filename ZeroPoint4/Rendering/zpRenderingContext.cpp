#include "zpRendering.h"

#if ZP_RENDERING_TYPE == ZP_DX11
#include "RenderingDX/zpDX11.h"
#elif ZP_RENDERING_TYPE == ZP_GL2
#include "RenderingOpenGL/zpOpenGL.h"
#else
#error( "No rendering engine selected!" )
#endif

zpRenderingContext::zpRenderingContext()
	: m_renderContextImpl( ZP_NULL )
	, m_renderingEngine( ZP_NULL )
	, m_currentCommnad( ZP_NULL )
	, m_immediateVertexSize( 0 )
	, m_immediateIndexSize( 0 )
	, m_currentBufferIndex( 0 )
	, m_currentVertexBuffer( ZP_NULL )
	, m_currentIndexBuffer( ZP_NULL )
	, m_currentFont( ZP_NULL )
{}
zpRenderingContext::~zpRenderingContext()
{
	m_renderingCommands.clear();
	m_immediateVertexBuffers.clear();
	m_immediateIndexBuffers.clear();
}

void zpRenderingContext::setup( zpRenderingEngine* engine, zpRenderingContextImpl* impl )
{
	m_renderingEngine = engine;
	m_renderContextImpl = impl;

	for( zp_uint i = 0; i < ZP_RENDERING_MAX_IMMEDIATE_SWAP_BUFFERS; ++i )
	{
		engine->createBuffer( m_immediateVertexBuffers.pushBackEmpty(), ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_DYNAMIC, ZP_RENDERING_IMMEDIATE_VERTEX_BUFFER_SIZE );
		engine->createBuffer( m_immediateIndexBuffers.pushBackEmpty(), ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_DYNAMIC, ZP_RENDERING_IMMEDIATE_INDEX_BUFFER_SIZE, sizeof( zp_ushort ) );

		m_renderingStats.pushBackEmpty();
	}

	m_currentVertexBuffer = &m_immediateVertexBuffers[ m_currentBufferIndex ];
	m_currentIndexBuffer = &m_immediateIndexBuffers[ m_currentBufferIndex ];

	m_scratchVertexBuffer.reserve( ZP_RENDERING_IMMEDIATE_VERTEX_BUFFER_SIZE );
	m_scratchIndexBuffer.reserve( ZP_RENDERING_IMMEDIATE_INDEX_BUFFER_SIZE );
}
void zpRenderingContext::destroy()
{
	for( zp_uint i = 0; i < ZP_RENDERING_MAX_IMMEDIATE_SWAP_BUFFERS; ++i )
	{
		m_renderingEngine->destroyBuffer( m_immediateVertexBuffers[i] );
		m_renderingEngine->destroyBuffer( m_immediateIndexBuffers[i] );
	}

	clearState();
	flush();

	m_renderingStats.clear();

	m_scratchVertexBuffer.destroy();
	m_scratchIndexBuffer.destroy();
}

void zpRenderingContext::setRenderTarget( zp_uint startIndex, zp_uint count, zpTexture* const* targets, zpDepthStencilBuffer* depthStencilBuffer )
{
	ZP_ASSERT( ( startIndex + count ) < ZP_RENDER_TARGET_MAX_COUNT, "Too many render targets set, max of %d", ZP_RENDER_TARGET_MAX_COUNT );

	m_renderContextImpl->setRenderTarget( startIndex, count, targets, depthStencilBuffer );
}
	 
void zpRenderingContext::clearRenderTarget( zpTexture* renderTarget, const zpColor4f& clearColor )
{
	m_renderContextImpl->clearRenderTarget( renderTarget, clearColor );
}
void zpRenderingContext::clearDepthStencilBuffer( zpDepthStencilBuffer* depthStencilBuffer, zp_float clearDepth, zp_uint clearStencil )
{
	m_renderContextImpl->clearDepthStencilBuffer( depthStencilBuffer, clearDepth, clearStencil );
}
void zpRenderingContext::clearDepthBuffer( zpDepthStencilBuffer* depthStencilBuffer, zp_float clearDepth )
{
	m_renderContextImpl->clearDepthBuffer( depthStencilBuffer, clearDepth );
}
void zpRenderingContext::clearStencilBuffer( zpDepthStencilBuffer* depthStencilBuffer, zp_uint clearStencil )
{
	m_renderContextImpl->clearStencilBuffer( depthStencilBuffer, clearStencil );
}
void zpRenderingContext::clearState()
{
	m_renderContextImpl->clearState();

	zpRenderingStats& stats = m_renderingStats[ m_currentBufferIndex ];
	zp_zero_memory( &stats );
}
void zpRenderingContext::flush()
{
	m_renderContextImpl->flush();
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
void zpRenderingContext::setDepthStencilState( zpDepthStencilState* depthStencil, zp_uint stencilRef )
{
	m_renderContextImpl->setDepthStencilState( depthStencil, stencilRef );
}
void zpRenderingContext::setBlendState( zpBlendState* blend, zp_float blendFactor[4], zp_uint sampleMask )
{
	m_renderContextImpl->setBlendState( blend, blendFactor, sampleMask );
}

void zpRenderingContext::setSamplerState( zp_uint bindSlots, zp_uint index, zpSamplerState* sampler )
{
	m_renderContextImpl->setSamplerState( bindSlots, index, sampler );
}
void zpRenderingContext::setConstantBuffer( zp_uint bindSlots, zp_uint index, zpBuffer* buffer )
{
	m_renderContextImpl->setConstantBuffer( bindSlots, index, buffer );
}

void zpRenderingContext::beginDrawImmediate( zp_uint layer, zpRenderingQueue queue, zpTopology topology, zpVertexFormat vertexFormat, const zpMaterialResourceInstance* materialInstance )
{
	beginDrawImmediate( layer, queue, topology, vertexFormat, materialInstance == ZP_NULL ? ZP_NULL : materialInstance->getResource() );
}
void zpRenderingContext::beginDrawImmediate( zp_uint layer, zpRenderingQueue queue, zpTopology topology, zpVertexFormat vertexFormat, const zpMaterialResource* material )
{
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );
	m_currentCommnad = &m_renderingCommands.pushBackEmpty();
	m_currentCommnad->type = ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE;
	m_currentCommnad->layer = layer;
	m_currentCommnad->queue = queue;
	m_currentCommnad->sortKey = material->getData()->materialId;
	m_currentCommnad->sortBias = material->getData()->sortBias;

	m_currentCommnad->topology = topology;
	m_currentCommnad->vertexBuffer = m_currentVertexBuffer->getBufferImpl();
	m_currentCommnad->indexBuffer = m_currentIndexBuffer->getBufferImpl();
	m_currentCommnad->material = material;
	m_currentCommnad->vertexFormatHash = zp_fnv1_32_string( VertexFormatShortNames[ vertexFormat ], 0 );
	m_currentCommnad->vertexFormat = vertexFormat;
	m_currentCommnad->vertexCount = 0;
	m_currentCommnad->indexCount = 0;
	m_currentCommnad->vertexOffset = 0;
	m_currentCommnad->indexOffset = 0;
	m_currentCommnad->matrix = zpMath::MatrixIdentity();
	m_currentCommnad->boundingBox.reset();
}

void zpRenderingContext::setBoundingBox( const zpBoundingAABB& bounding )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );

	m_currentCommnad->boundingBox = bounding;
}
void zpRenderingContext::setBoundingBoxCenter( const zpVector4f& center )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );

	m_currentCommnad->boundingBox.setCenter( center );
}

void zpRenderingContext::setMatrix( const zpMatrix4f& matrix )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );

	m_currentCommnad->matrix = matrix;
}
void zpRenderingContext::setSortBias( zp_ushort bias )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );

	m_currentCommnad->sortBias = bias;
}

void zpRenderingContext::addVertex( const zpVector4f& pos, const zpColor4f& color )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

	ZP_ALIGN16 zp_float v[ 4 ];
	zpMath::Vector4Store4( pos, v + 0 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.write( color );
	m_currentCommnad->vertexCount += 1;

	m_currentCommnad->boundingBox.add( pos );
}
void zpRenderingContext::addVertex( const zpVector4f& pos, const zpVector2f& uv0 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_UV, "" );

	ZP_ALIGN16 zp_float v[ 4 ];
	zpMath::Vector4Store4( pos, v + 0 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.write( uv0 );
	m_currentCommnad->vertexCount += 1;

	m_currentCommnad->boundingBox.add( pos );
}
void zpRenderingContext::addVertex( const zpVector4f& pos, const zpVector2f& uv0, const zpColor4f& color )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, "" );

	ZP_ALIGN16 zp_float v[ 4 ];
	zpMath::Vector4Store4( pos, v + 0 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.write( color );
	m_scratchVertexBuffer.write( uv0 );
	m_currentCommnad->vertexCount += 1;

	m_currentCommnad->boundingBox.add( pos );
}
void zpRenderingContext::addVertex( const zpVector4f& pos, const zpVector4f& normal, const zpVector2f& uv0 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV, "" );

	ZP_ALIGN16 zp_float v[ 8 ];
	zpMath::Vector4Store4( pos, v + 0 );
	zpMath::Vector4Store4( normal, v + 4 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.writeBulk( v + 4, 4 );
	m_scratchVertexBuffer.write( uv0 );
	m_currentCommnad->vertexCount += 1;

	m_currentCommnad->boundingBox.add( pos );
}
void zpRenderingContext::addVertex( const zpVector4f& pos, const zpVector4f& normal, const zpVector2f& uv0, const zpVector2f& uv1 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2, "" );

	ZP_ALIGN16 zp_float v[ 8 ];
	zpMath::Vector4Store4( pos, v + 0 );
	zpMath::Vector4Store4( normal, v + 4 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.writeBulk( v + 4, 4 );
	m_scratchVertexBuffer.write( uv0 );
	m_scratchVertexBuffer.write( uv1 );
	m_currentCommnad->vertexCount += 1;

	m_currentCommnad->boundingBox.add( pos );
}
void zpRenderingContext::addVertex( const zpVector4f& pos, const zpVector4f& normal, const zpColor4f& color, const zpVector2f& uv )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_NORMAL_COLOR_UV, "" );

	ZP_ALIGN16 zp_float v[ 8 ];
	zpMath::Vector4Store4( pos, v + 0 );
	zpMath::Vector4Store4( normal, v + 4 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.writeBulk( v + 4, 4 );
	m_scratchVertexBuffer.write( color );
	m_scratchVertexBuffer.write( uv );
	m_currentCommnad->vertexCount += 1;

	m_currentCommnad->boundingBox.add( pos );
}

void zpRenderingContext::addLineIndex( zp_ushort index0, zp_ushort index1 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );

	m_scratchIndexBuffer.write( index0 );
	m_scratchIndexBuffer.write( index1 );
	m_currentCommnad->indexCount += 2;
}
void zpRenderingContext::addTriangleIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );

	m_scratchIndexBuffer.write( index0 );
	m_scratchIndexBuffer.write( index1 );
	m_scratchIndexBuffer.write( index2 );
	m_currentCommnad->indexCount += 3;
}
void zpRenderingContext::addQuadIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2, zp_ushort index3 )
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

	ZP_ALIGN16 zp_float v[ 8 ];
	zpMath::Vector4Store4( pos0, v + 0 );
	zpMath::Vector4Store4( pos1, v + 4 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.write( color );
	m_scratchVertexBuffer.writeBulk( v + 4, 4 );
	m_scratchVertexBuffer.write( color );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 1 );

	m_currentCommnad->vertexCount += 2;
	m_currentCommnad->indexCount += 2;

	m_currentCommnad->boundingBox.add( pos0 );
	m_currentCommnad->boundingBox.add( pos1 );
}

void zpRenderingContext::addLine( const zpVector4f& pos0, const zpColor4f& color0, const zpVector4f& pos1, const zpColor4f& color1 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

	ZP_ALIGN16 zp_float v[ 8 ];
	zpMath::Vector4Store4( pos0, v + 0 );
	zpMath::Vector4Store4( pos1, v + 4 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.write( color0 );
	m_scratchVertexBuffer.writeBulk( v + 4, 4 );
	m_scratchVertexBuffer.write( color1 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 1 );
	
	m_currentCommnad->vertexCount += 2;
	m_currentCommnad->indexCount += 2;

	m_currentCommnad->boundingBox.add( pos0 );
	m_currentCommnad->boundingBox.add( pos1 );
}

void zpRenderingContext::addQuad( const zpVector4f& pos0, const zpVector4f& pos1, const zpVector4f& pos2, const zpVector4f& pos3, const zpColor4f& color )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

	ZP_ALIGN16 zp_float v[ 16 ];
	zpMath::Vector4Store4( pos0, v + 0 );
	zpMath::Vector4Store4( pos1, v + 4 );
	zpMath::Vector4Store4( pos2, v + 8 );
	zpMath::Vector4Store4( pos3, v + 12 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.write( color );
	m_scratchVertexBuffer.writeBulk( v + 4, 4 );
	m_scratchVertexBuffer.write( color );
	m_scratchVertexBuffer.writeBulk( v + 8, 4 );
	m_scratchVertexBuffer.write( color );
	m_scratchVertexBuffer.writeBulk( v + 12, 4 );
	m_scratchVertexBuffer.write( color );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 1 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 3 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );

	m_currentCommnad->vertexCount += 4;
	m_currentCommnad->indexCount += 6;

	m_currentCommnad->boundingBox.add( pos0 );
	m_currentCommnad->boundingBox.add( pos1 );
	m_currentCommnad->boundingBox.add( pos2 );
	m_currentCommnad->boundingBox.add( pos3 );
}
void zpRenderingContext::addQuad( 
	const zpVector4f& pos0, const zpColor4f& color0, 
	const zpVector4f& pos1, const zpColor4f& color1, 
	const zpVector4f& pos2, const zpColor4f& color2, 
	const zpVector4f& pos3, const zpColor4f& color3 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

	ZP_ALIGN16 zp_float v[ 16 ];
	zpMath::Vector4Store4( pos0, v + 0 );
	zpMath::Vector4Store4( pos1, v + 4 );
	zpMath::Vector4Store4( pos2, v + 8 );
	zpMath::Vector4Store4( pos3, v + 12 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.write( color0 );
	m_scratchVertexBuffer.writeBulk( v + 4, 4 );
	m_scratchVertexBuffer.write( color1 );
	m_scratchVertexBuffer.writeBulk( v + 8, 4 );
	m_scratchVertexBuffer.write( color2 );
	m_scratchVertexBuffer.writeBulk( v + 12, 4 );
	m_scratchVertexBuffer.write( color3 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 1 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 3 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );

	m_currentCommnad->vertexCount += 4;
	m_currentCommnad->indexCount += 6;

	m_currentCommnad->boundingBox.add( pos0 );
	m_currentCommnad->boundingBox.add( pos1 );
	m_currentCommnad->boundingBox.add( pos2 );
	m_currentCommnad->boundingBox.add( pos3 );
}
void zpRenderingContext::addQuad( 
	const zpVector4f& pos0, const zpVector2f& uv0, 
	const zpVector4f& pos1, const zpVector2f& uv1, 
	const zpVector4f& pos2, const zpVector2f& uv2,
	const zpVector4f& pos3, const zpVector2f& uv3 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_UV, "" );

	ZP_ALIGN16 zp_float v[ 16 ];
	zpMath::Vector4Store4( pos0, v + 0 );
	zpMath::Vector4Store4( pos1, v + 4 );
	zpMath::Vector4Store4( pos2, v + 8 );
	zpMath::Vector4Store4( pos3, v + 12 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.write( uv0 );
	m_scratchVertexBuffer.writeBulk( v + 4, 4 );
	m_scratchVertexBuffer.write( uv1 );
	m_scratchVertexBuffer.writeBulk( v + 8, 4 );
	m_scratchVertexBuffer.write( uv2 );
	m_scratchVertexBuffer.writeBulk( v + 12, 4 );
	m_scratchVertexBuffer.write( uv3 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 1 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 3 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );

	m_currentCommnad->vertexCount += 4;
	m_currentCommnad->indexCount += 6;

	m_currentCommnad->boundingBox.add( pos0 );
	m_currentCommnad->boundingBox.add( pos1 );
	m_currentCommnad->boundingBox.add( pos2 );
	m_currentCommnad->boundingBox.add( pos3 );
}
void zpRenderingContext::addQuad( 
	const zpVector4f& pos0, const zpVector2f& uv0, 
	const zpVector4f& pos1, const zpVector2f& uv1, 
	const zpVector4f& pos2, const zpVector2f& uv2,
	const zpVector4f& pos3, const zpVector2f& uv3, const zpColor4f& color )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, "" );

	ZP_ALIGN16 zp_float v[ 16 ];
	zpMath::Vector4Store4( pos0, v + 0 );
	zpMath::Vector4Store4( pos1, v + 4 );
	zpMath::Vector4Store4( pos2, v + 8 );
	zpMath::Vector4Store4( pos3, v + 12 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.write( color );
	m_scratchVertexBuffer.write( uv0 );
	m_scratchVertexBuffer.writeBulk( v + 4, 4 );
	m_scratchVertexBuffer.write( color );
	m_scratchVertexBuffer.write( uv1 );
	m_scratchVertexBuffer.writeBulk( v + 8, 4 );
	m_scratchVertexBuffer.write( color );
	m_scratchVertexBuffer.write( uv2 );
	m_scratchVertexBuffer.writeBulk( v + 12, 4 );
	m_scratchVertexBuffer.write( color );
	m_scratchVertexBuffer.write( uv3 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 1 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 3 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );

	m_currentCommnad->vertexCount += 4;
	m_currentCommnad->indexCount += 6;

	m_currentCommnad->boundingBox.add( pos0 );
	m_currentCommnad->boundingBox.add( pos1 );
	m_currentCommnad->boundingBox.add( pos2 );
	m_currentCommnad->boundingBox.add( pos3 );
}
void zpRenderingContext::addQuad( 
	const zpVector4f& pos0, const zpVector2f& uv0, const zpColor4f& color0,
	const zpVector4f& pos1, const zpVector2f& uv1, const zpColor4f& color1,
	const zpVector4f& pos2, const zpVector2f& uv2, const zpColor4f& color2,
	const zpVector4f& pos3, const zpVector2f& uv3, const zpColor4f& color3 )
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, "" );

	ZP_ALIGN16 zp_float v[ 16 ];
	zpMath::Vector4Store4( pos0, v + 0 );
	zpMath::Vector4Store4( pos1, v + 4 );
	zpMath::Vector4Store4( pos2, v + 8 );
	zpMath::Vector4Store4( pos3, v + 12 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.write( color0 );
	m_scratchVertexBuffer.write( uv0 );
	m_scratchVertexBuffer.writeBulk( v + 4, 4 );
	m_scratchVertexBuffer.write( color1 );
	m_scratchVertexBuffer.write( uv1 );
	m_scratchVertexBuffer.writeBulk( v + 8, 4 );
	m_scratchVertexBuffer.write( color2 );
	m_scratchVertexBuffer.write( uv2 );
	m_scratchVertexBuffer.writeBulk( v + 12, 4 );
	m_scratchVertexBuffer.write( color3 );
	m_scratchVertexBuffer.write( uv3 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 1 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 3 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );

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
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV, "" );

	ZP_ALIGN16 zp_float v[ 16 ];
	zpMath::Vector4Store4( pos0, v + 0 );
	zpMath::Vector4Store4( pos1, v + 4 );
	zpMath::Vector4Store4( pos2, v + 8 );
	zpMath::Vector4Store4( pos3, v + 12 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.write( normal0 );
	m_scratchVertexBuffer.write( uv0 );
	m_scratchVertexBuffer.writeBulk( v + 4, 4 );
	m_scratchVertexBuffer.write( normal1 );
	m_scratchVertexBuffer.write( uv1 );
	m_scratchVertexBuffer.writeBulk( v + 8, 4 );
	m_scratchVertexBuffer.write( normal2 );
	m_scratchVertexBuffer.write( uv2 );
	m_scratchVertexBuffer.writeBulk( v + 12, 4 );
	m_scratchVertexBuffer.write( normal3 );
	m_scratchVertexBuffer.write( uv3 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 1 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 3 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );

	m_currentCommnad->vertexCount += 4;
	m_currentCommnad->indexCount += 6;

	m_currentCommnad->boundingBox.add( pos0 );
	m_currentCommnad->boundingBox.add( pos1 );
	m_currentCommnad->boundingBox.add( pos2 );
	m_currentCommnad->boundingBox.add( pos3 );
}
void zpRenderingContext::addQuad( 
	const zpVector4f& pos0, const zpVector4f& normal0, const zpVector2f& uv00, const zpVector2f& uv10,
	const zpVector4f& pos1, const zpVector4f& normal1, const zpVector2f& uv01, const zpVector2f& uv11,
	const zpVector4f& pos2, const zpVector4f& normal2, const zpVector2f& uv02, const zpVector2f& uv12,
	const zpVector4f& pos3, const zpVector4f& normal3, const zpVector2f& uv03, const zpVector2f& uv13 )

{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );
	ZP_ASSERT( m_currentCommnad->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2, "" );

	ZP_ALIGN16 zp_float v[ 16 ];
	zpMath::Vector4Store4( pos0, v + 0 );
	zpMath::Vector4Store4( pos1, v + 4 );
	zpMath::Vector4Store4( pos2, v + 8 );
	zpMath::Vector4Store4( pos3, v + 12 );

	m_scratchVertexBuffer.writeBulk( v + 0, 4 );
	m_scratchVertexBuffer.write( normal0 );
	m_scratchVertexBuffer.write( uv00 );
	m_scratchVertexBuffer.write( uv10 );
	m_scratchVertexBuffer.writeBulk( v + 4, 4 );
	m_scratchVertexBuffer.write( normal1 );
	m_scratchVertexBuffer.write( uv01 );
	m_scratchVertexBuffer.write( uv11 );
	m_scratchVertexBuffer.writeBulk( v + 8, 4 );
	m_scratchVertexBuffer.write( normal2 );
	m_scratchVertexBuffer.write( uv02 );
	m_scratchVertexBuffer.write( uv12 );
	m_scratchVertexBuffer.writeBulk( v + 12, 4 );
	m_scratchVertexBuffer.write( normal3 );
	m_scratchVertexBuffer.write( uv03 );
	m_scratchVertexBuffer.write( uv13 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 1 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );

	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 2 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 3 );
	m_scratchIndexBuffer.write< zp_ushort >( m_currentCommnad->vertexCount + 0 );

	m_currentCommnad->vertexCount += 4;
	m_currentCommnad->indexCount += 6;

	m_currentCommnad->boundingBox.add( pos0 );
	m_currentCommnad->boundingBox.add( pos1 );
	m_currentCommnad->boundingBox.add( pos2 );
	m_currentCommnad->boundingBox.add( pos3 );
}

void zpRenderingContext::endDrawImmediate()
{
	ZP_ASSERT( m_currentCommnad != ZP_NULL, "" );

	m_currentCommnad->boundingBox.setCenter( m_currentCommnad->matrix.m_m4 );

	m_currentCommnad->vertexOffset = m_immediateVertexSize;
	m_currentCommnad->indexOffset = m_immediateIndexSize;

	m_currentCommnad = ZP_NULL;

	m_immediateVertexSize = m_scratchVertexBuffer.size();
	m_immediateIndexSize = m_scratchIndexBuffer.size();
}

void zpRenderingContext::drawMesh( zp_uint layer, zpRenderingQueue queue, zpMeshResourceInstance* mesh, const zpMatrix4f& matrix, zpMaterialResourceInstance* material )
{
	ZP_ASSERT( m_currentCommnad == ZP_NULL, "" );

	zpVector4f c, center( matrix.r[ 3 ] );

	const zpMesh* m = mesh->getResource()->getData();
	const zpMeshPart* b = m->m_parts.begin();
	const zpMeshPart* e = m->m_parts.end();
	for( ; b != e; ++b )
	{
		zpRenderingCommand& command = m_renderingCommands.pushBackEmpty();
		command.type = ZP_RENDERING_COMMNAD_DRAW_BUFFERED;
		command.layer = layer;
		command.queue = queue;

		command.topology = ZP_TOPOLOGY_TRIANGLE_LIST;
		command.vertexBuffer = m->m_vertex.getBufferImpl();
		command.indexBuffer = m->m_index.getBufferImpl();
		command.material = material->getResource();
		command.vertexFormat = zpVertexFormat_None;
		command.vertexFormatHash = m->m_format;
		command.vertexCount = b->m_vertexCount;
		command.indexCount = b->m_indexCount;
		command.vertexOffset = b->m_vertexOffset;
		command.indexOffset = b->m_indexOffset;
		command.boundingBox = b->m_boundingBox;
		command.matrix = matrix;

		command.sortKey = command.material->getData()->materialId;
		command.sortBias = command.material->getData()->sortBias;

		command.boundingBox.setCenter( matrix.m_m4 );
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

void zpRenderingContext::update( zpBuffer* buffer, const void* data, zp_uint size )
{
	m_renderContextImpl->update( buffer->getBufferImpl(), data, size );
}

void zpRenderingContext::fillBuffers()
{
	void* mapData;

	if( m_scratchVertexBuffer.size() )
	{
		map( m_currentVertexBuffer, &mapData );
		zp_memcpy( mapData, m_scratchVertexBuffer.size(), m_scratchVertexBuffer.getData(), m_scratchVertexBuffer.size() );
		unmap( m_currentVertexBuffer );
	}

	if( m_scratchIndexBuffer.size() )
	{
		map( m_currentIndexBuffer, &mapData );
		zp_memcpy( mapData, m_scratchIndexBuffer.size(), m_scratchIndexBuffer.getData(), m_scratchIndexBuffer.size() );
		unmap( m_currentIndexBuffer );
	}
	
	m_scratchVertexBuffer.reset();
	m_scratchIndexBuffer.reset();
}

void zpRenderingContext::preprocessCommands( zpCamera* camera, zp_uint layer )
{
	// clear filtered commands
	for( zp_uint i = 0; i < zpRenderingQueue_Count; ++i )
	{
		m_filteredCommands[ i ].reset();
		m_filteredCommands[ i ].reserve( 10 );
	}

	const zpFrustum& frustum = camera->getFrustum();
	zpRenderingStats& stats = m_renderingStats[ m_currentBufferIndex ];

	// filter all commands into layer buckets if camera is present
	zpRenderingCommand* cmd = m_renderingCommands.begin();
	zpRenderingCommand* end = m_renderingCommands.end();
	for( ; cmd != end; ++cmd )
	{
		stats.numDrawCommands[ cmd->queue ]++;

		// if the command has no verts, don't add it
		// if the camera does not support any layer the command is not, don't add it
		if( cmd->vertexCount == 0 || ( layer & cmd->layer ) == 0 ) continue;

		// filter commands into buckets
		switch( cmd->queue )
		{
			// no sort key needed for non-sorted queues
		case ZP_RENDERING_QUEUE_SKYBOX:
		case ZP_RENDERING_QUEUE_UI:
		case ZP_RENDERING_QUEUE_UI_DEBUG:
			stats.visibleDrawCommands[ cmd->queue ]++;
			m_filteredCommands[ cmd->queue ].pushBack( cmd );
			break;

			// otherwise, sort and cull commands based on the camera and its frustum
		default:
			if( ZP_IS_COLLISION( frustum, cmd->boundingBox ) )
			{
				generateSortKeyForCommand( cmd, camera );
				stats.visibleDrawCommands[ cmd->queue ]++;
				m_filteredCommands[ cmd->queue ].pushBack( cmd );
			}
		}
	}

	// sort opaque front to back
	m_filteredCommands[ ZP_RENDERING_QUEUE_OPAQUE ].sort( []( zpRenderingCommand* cmd0, zpRenderingCommand* cmd1 ) {
		return cmd0->sortKey < cmd1->sortKey;
	} );
	m_filteredCommands[ ZP_RENDERING_QUEUE_OPAQUE_DEBUG ].sort( []( zpRenderingCommand* cmd0, zpRenderingCommand* cmd1 ) {
		return cmd0->sortKey < cmd1->sortKey;
	} );

	// sort transparent back to front
	m_filteredCommands[ ZP_RENDERING_QUEUE_TRANSPARENT ].sort( []( zpRenderingCommand* cmd0, zpRenderingCommand* cmd1 ) {
		return cmd0->sortKey > cmd1->sortKey;
	} );
	m_filteredCommands[ ZP_RENDERING_QUEUE_TRANSPARENT_DEBUG ].sort( []( zpRenderingCommand* cmd0, zpRenderingCommand* cmd1 ) {
		return cmd0->sortKey > cmd1->sortKey;
	} );
}

void zpRenderingContext::processCommands( zpRenderingQueue layer )
{
	ZP_ASSERT( false, "don't use" );
	//m_numTotalDrawCommands += m_filteredCommands[ layer ].size();
	//
	//const zpRenderingCommand* const* cmd = m_filteredCommands[ layer ].begin();
	//const zpRenderingCommand* const* end = m_filteredCommands[ layer ].end();
	//
	//if( cmd != end )
	//{
	//	setConstantBuffer( ZP_RESOURCE_BIND_SLOT_VERTEX_SHADER | ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, ZP_CONSTANT_BUFFER_SLOT_PER_DRAW_CALL, &m_perDratCallBuffer );
	//
	//	zpDrawCallBufferData drawCallData;
	//	for( ; cmd != end; ++cmd )
	//	{
	//		drawCallData.world = (*cmd)->matrix;
	//		update( &m_perDratCallBuffer, &drawCallData, sizeof( zpDrawCallBufferData ) );
	//
	//		m_renderContextImpl->processCommand( m_renderingEngine->getRenderingEngineImpl(), *cmd );
	//
	//		m_numTotalVerticies += (*cmd)->vertexCount;
	//	}
	//}
}
void zpRenderingContext::processCommand( const zpRenderingCommand* cmd )
{
	m_renderContextImpl->processCommand( m_renderingEngine->getRenderingEngineImpl(), cmd );
}

void zpRenderingContext::finalizeCommands()
{
	zpRenderingStats& stats = m_renderingStats[ m_currentBufferIndex ];
	stats.totalDrawCommands = m_renderingCommands.size();
	stats.totalVerticies = m_immediateVertexSize;

	m_renderingCommands.reset();

	m_currentBufferIndex = ( m_currentBufferIndex + 1 ) % ZP_RENDERING_MAX_IMMEDIATE_SWAP_BUFFERS;
	m_currentVertexBuffer = &m_immediateVertexBuffers[ m_currentBufferIndex ];
	m_currentIndexBuffer = &m_immediateIndexBuffers[ m_currentBufferIndex ];

	m_immediateVertexSize = 0;
	m_immediateIndexSize = 0;
}

void zpRenderingContext::beginDrawFont( zp_uint layer, zpRenderingQueue queue, const zpFontResourceInstance* font )
{
	ZP_ASSERT( m_currentFont == ZP_NULL, "Did not finish using previous font" );

	m_currentFont = font;
	const zpMaterialResourceInstance* mat = &m_currentFont->getResource()->getData()->pages[0];
	beginDrawImmediate( layer, queue, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, mat );
}
void zpRenderingContext::calculateArea( const zp_char* text, zp_float size, zpRectf& area )
{
	ZP_ASSERT( m_currentFont != ZP_NULL, "BeginDrawFont not called" );

}
void zpRenderingContext::addText( const zp_char* text, zp_float size, const zpVector2f& position, zpFontAlignment alignment, const zpColor4f& color )
{
	addText( text, size, position, alignment, color, color );
}
void zpRenderingContext::addText( const zp_char* text, zp_float size, const zpVector2f& position, zpFontAlignment alignment, const zpColor4f& colorTop, const zpColor4f& colorBottom )
{
	ZP_ASSERT( m_currentFont != ZP_NULL, "BeginDrawFont not called" );

	const zpFontSet* font = m_currentFont->getResource()->getData();

	zp_float scale = size / (zp_float)font->size;

	zp_char prev = '\0', curr = '\0';
	zpVector2f u0, u1, u2, u3, invSize( 1.f / font->scaleW, 1.f / font->scaleH ), cursor( position );
	zpVector4f p0, p1, p2, p3, offset = zpMath::Vector4( 0, 0, 0, 1 );
	for( ; *text != '\0'; ++text )
	{
		curr = *text;
		const zpFontGlyph& glyph = font->glyphs[ (zp_int)curr ];

		zpRectf uv( (zp_float)glyph.x, (zp_float)glyph.y, (zp_float)glyph.width, (zp_float)glyph.height );
		zpRectf p( scale * (zp_float)glyph.xOffset, scale * (zp_float)glyph.yOffset, scale * (zp_float)glyph.width, scale * (zp_float)glyph.height );

		p0 = ( p.getTopLeft()     + cursor ).asVector4();
		p1 = ( p.getTopRight()    + cursor ).asVector4();
		p2 = ( p.getBottomRight() + cursor ).asVector4();
		p3 = ( p.getBottomLeft()  + cursor ).asVector4();

		u0 = uv.getTopLeft()     * invSize;
		u1 = uv.getTopRight()    * invSize;
		u2 = uv.getBottomRight() * invSize;
		u3 = uv.getBottomLeft()  * invSize;

		addQuad(
			p0, u0, colorTop,
			p1, u1, colorTop,
			p2, u2, colorBottom,
			p3, u3, colorBottom
			);

		cursor += zpVector2f( scale * (zp_float)glyph.xAdvance, 0 );
	}
}
void zpRenderingContext::addText( const zp_char* text, zp_float size, const zpVector4f& position, const zpVector4f& direction, zpFontAlignment alignment, const zpColor4f& color )
{
	addText( text, size, position, direction, alignment, color, color );
}
void zpRenderingContext::addText( const zp_char* text, zp_float size, const zpVector4f& position, const zpVector4f& direction, zpFontAlignment alignment, const zpColor4f& colorTop, const zpColor4f& colorBottom )
{
	ZP_ASSERT( m_currentFont != ZP_NULL, "BeginDrawFont not called" );

	const zpFontSet* font = m_currentFont->getResource()->getData();

	zpVector2f u0, u1, u2, u3, invSize( 1.f / font->scaleW, 1.f / font->scaleH );
	zpVector4f cursor( position ), p0, p1, p2, p3, offset = zpMath::Vector4( 0, 0, 0, 1 );
	for( ; *text != '\0'; ++text )
	{
		zp_int c = (zp_int)*text;

		const zpFontGlyph& glyph = font->glyphs[ c ];
		
		zpRectf uv( (zp_float)glyph.x, (zp_float)glyph.y, (zp_float)glyph.width, (zp_float)glyph.height );
		zpRectf v;
	}
}
void zpRenderingContext::endDrawFont()
{
	ZP_ASSERT( m_currentFont != ZP_NULL, "BeginDrawFont not called" );

	endDrawImmediate();
	m_currentFont = ZP_NULL;
}

const zpRenderingStats& zpRenderingContext::getPreviousFrameStats() const
{
	zp_uint index = ( ( ( (zp_int)m_currentBufferIndex ) - 1 ) + ZP_RENDERING_MAX_IMMEDIATE_SWAP_BUFFERS ) % ZP_RENDERING_MAX_IMMEDIATE_SWAP_BUFFERS;
	return m_renderingStats[ index ];
}

void zpRenderingContext::generateSortKeyForCommand( zpRenderingCommand* command, zpCamera* camera )
{
	zpScalar len, dist = zpMath::Scalar( camera->getFar() );
	zpVector4f center = command->boundingBox.getCenter();

	center = zpMath::Vector4Sub( center, camera->getPosition() );
	len = zpMath::Vector4LengthSquared3( center );
	dist = zpMath::ScalarMul( dist, dist );
	len = zpMath::ScalarDiv( len, dist );

	zp_ushort distKey = command->sortBias + (zp_ushort)( zpMath::AsFloat( len ) * (zp_float)zp_limit_max<zp_ushort>() );
	zp_uint matKey = command->sortKey;

	command->sortKey = ( distKey << 16 ) | ( matKey & 0xFFFF );
}
