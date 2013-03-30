#pragma once
#ifndef ZP_RENDERING_CONTEXT
#define ZP_RENDERING_CONTEXT

class zpRenderingEngineImpl;
class zpRenderingContextImpl;

class zpRenderingContext
{
	ZP_NON_COPYABLE( zpRenderingContext );
public:
	~zpRenderingContext();

	void setRenderTarget( zp_uint startIndex, zp_uint count, zpTexture** targets, zpDepthStencilBuffer* depthStencilBuffer );

	void clearRenderTarget( zpTexture* renderTarget, const zpColor4f& clearColor );
	void clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil );
	void clearState();

	void setViewport( const zpViewport& viewport );
	void setScissorRect( const zpRecti& rect );

	void setRasterState( zpRasterState* raster );
	void setSamplerState( zp_uint bindSlots, zp_uint index, zpSamplerState* sampler );

	void beginDrawImmediate( zpRenderingLayer layer, zpTopology topology, zpVertexFormat vertexFormat );

	void addVertex( const zpVector4f& pos, const zpColor4f& color );
	void addVertex( const zpVector4f& pos, const zpVector2f& uv0 );
	void addVertex( const zpVector4f& pos, const zpVector4f& normal, const zpVector2f& uv0 );
	void addVertex( const zpVector4f& pos, const zpVector4f& normal, const zpVector2f& uv0, const zpVector2f& uv1 );

	void addLineIndex( zp_short index0, zp_short index1 );
	void addTriangleIndex( zp_short index0, zp_short index1, zp_short index2 );
	void addQuadIndex( zp_short index0, zp_short index1, zp_short index2, zp_short index3 );

	void addLine( const zpVector4f& pos0, const zpVector4f& pos1, const zpColor4f& color );
	void addLine( const zpVector4f& pos0, const zpColor4f& color0, const zpVector4f& pos1, const zpColor4f& color1 );

	void addTriangle( const zpVector4f& pos0, const zpVector4f& pos1, const zpVector4f& pos2, const zpColor4f& color );
	void addTriangle( 
		const zpVector4f& pos0, const zpColor4f& color0, 
		const zpVector4f& pos1, const zpColor4f& color1, 
		const zpVector4f& pos2, const zpColor4f& color2 );
	void addTriangle( 
		const zpVector4f& pos0, const zpVector2f& uv0, 
		const zpVector4f& pos1, const zpVector2f& uv1, 
		const zpVector4f& pos2, const zpVector2f& uv2 );
	void addTriangle( 
		const zpVector4f& pos0, const zpVector4f& normal0, const zpVector2f& uv0, 
		const zpVector4f& pos1, const zpVector4f& normal1, const zpVector2f& uv1, 
		const zpVector4f& pos2, const zpVector4f& normal2, const zpVector2f& uv2 );
	void addTriangle( 
		const zpVector4f& pos0, const zpVector4f& normal0, const zpVector2f& uv00, const zpVector2f& uv10,
		const zpVector4f& pos1, const zpVector4f& normal1, const zpVector2f& uv01, const zpVector2f& uv11,
		const zpVector4f& pos2, const zpVector4f& normal2, const zpVector2f& uv02, const zpVector2f& uv12 );

	void addQuad( const zpVector4f& pos0, const zpVector4f& pos1, const zpVector4f& pos2, const zpVector4f& pos3, const zpColor4f& color );
	void addQuad( 
		const zpVector4f& pos0, const zpColor4f& color0, 
		const zpVector4f& pos1, const zpColor4f& color1, 
		const zpVector4f& pos2, const zpColor4f& color2, 
		const zpVector4f& pos3, const zpColor4f& color3 );
	void addQuad( 
		const zpVector4f& pos0, const zpVector2f& uv0, 
		const zpVector4f& pos1, const zpVector2f& uv1, 
		const zpVector4f& pos2, const zpVector2f& uv2,
		const zpVector4f& pos3, const zpVector2f& uv3 );
	void addQuad( 
		const zpVector4f& pos0, const zpVector4f& normal0, const zpVector2f& uv0, 
		const zpVector4f& pos1, const zpVector4f& normal1, const zpVector2f& uv1, 
		const zpVector4f& pos2, const zpVector4f& normal2, const zpVector2f& uv2,
		const zpVector4f& pos3, const zpVector4f& normal3, const zpVector2f& uv3 );
	void addQuad( 
		const zpVector4f& pos0, const zpVector4f& normal0, const zpVector2f& uv00, const zpVector2f& uv10,
		const zpVector4f& pos1, const zpVector4f& normal1, const zpVector2f& uv01, const zpVector2f& uv11,
		const zpVector4f& pos2, const zpVector4f& normal2, const zpVector2f& uv02, const zpVector2f& uv12,
		const zpVector4f& pos3, const zpVector4f& normal3, const zpVector2f& uv03, const zpVector2f& uv13 );

	void endDrawImmediate();

	void drawBuffered( zpRenderingLayer layer, zpTopology topology, zpVertexFormat vertexFormat, zpBuffer* vertexBuffe, zpBuffer* indexBuffer, zp_uint vertexCount, zp_uint indexCount, zpBoundingAABB* boundingBox = 0 );

	void drawInstancedDraw( zpRenderingLayer layer, zpTopology topology, zpVertexFormat vertexFormat );

	void map( zpBuffer* buffer, void** data, zpMapType mapType = ZP_MAP_TYPE_WRITE_DISCARD, zp_uint subResource = 0 );
	void unmap( zpBuffer* buffer, zp_uint subResource = 0 );

	void update( zpBuffer* buffer, void* data, zp_uint size );

	void processCommands();

private:
	zpRenderingContext();
	zpRenderingContext( zpRenderingEngine* engine, zpRenderingContextImpl* impl );

	zpRenderingContextImpl* m_renderContextImpl;
	zpRenderingEngine* m_renderingEngine;

	zpRenderingCommand* m_currentCommnad;
	zpFixedArrayList< zpRenderingCommand, ZP_RENDERING_MAX_COMMNADS > m_renderingCommands;
	zpFixedDataBuffer< ZP_RENDERING_IMMEDIATE_SCRATCH_VERTEX_BUFFER_SIZE > m_scratchVertexBuffer;
	zpFixedDataBuffer< ZP_RENDERING_IMMEDIATE_SCRATCH_INDEX_BUFFER_SIZE > m_scratchIndexBuffer;

	zp_byte* m_immediateVertexData;
	zp_byte* m_immediateIndexData;

	zp_uint m_immediateVertexSize;
	zp_uint m_immediateIndexSize;

	zp_uint m_currentBufferIndex;
	zpBuffer* m_currentVertexBuffer;
	zpBuffer* m_currentIndexBuffer;

	zpFixedArrayList< zpBuffer*, ZP_RENDERING_MAX_IMMEDIATE_SWAP_BUFFERS > m_immediateVertexBuffers;
	zpFixedArrayList< zpBuffer*, ZP_RENDERING_MAX_IMMEDIATE_SWAP_BUFFERS > m_immediateIndexBuffers;

	friend class zpRenderingEngine;
};

/*
ZP_PURE_INTERFACE zpRenderingContext
{
public:
	virtual const zpString& getName() const = 0;

	virtual void setRenderTarget( zpTexture* target, zp_uint index = 0 ) = 0;
	virtual zpTexture* getRenderTarget( zp_uint index = 0 ) const = 0;

	virtual void setDepthStencilBuffer( zpDepthStencilBuffer* depthBuffer ) = 0;
	virtual zpDepthStencilBuffer* getDepthStencilBuffer() const = 0;

	virtual void bindRenderTargetAndDepthBuffer() = 0;
	virtual void unbindRenderTargetAndDepthBuffer() = 0;

	void clearRenderTarget( const zpColor4f& color ) {
		clearRenderTarget( &color, 1 );
	}
	template<zp_uint I>
	void clearRenderTarget( const zpColor4f (&colors)[I] ) {
		clearRenderTarget( colors, I );
	}
	virtual void clearRenderTarget( const zpColor4f* colors, zp_uint count = 1 ) = 0;
	virtual void clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil ) = 0;

	virtual void setBuffer( const zpBuffer* buffer, zp_uint slot = 0 ) = 0;
	virtual void setBuffers( const zpBuffer** buffers, zp_uint count, zp_uint slot = 0 ) = 0;

	template<zp_uint I>
	void setBuffers( const zpBuffer* (&buffers)[I], zp_uint slot = 0 ) {
		setBuffers( buffers, I, slot );
	}

	virtual void setVertexLayout( const zpVertexLayout* layout ) = 0;

	virtual void setShader( const zpResourceInstance<zpShaderResource>* shader ) = 0;
	virtual void setTexture( zpResourceBindSlot bindSlot, zp_uint slot, const zpTexture* texture ) = 0;

	virtual void setTopology( zpTopology topology ) = 0;

	virtual void setViewport( const zpViewport& viewport ) = 0;
	virtual void setScissorRect( const zpRecti& rect ) = 0;
	virtual void getScissorRect( zpRecti& rect ) const = 0;

	virtual void setSamplerState( zpResourceBindSlot bindSlot, zp_uint slot, zpSamplerState* state ) = 0;
	virtual void setRasterState( zpRasterState* raster ) = 0;

	virtual void map( zpBuffer* buffer, zpMapType mapType, zp_uint subResource, void** data ) = 0;
	virtual void unmap( zpBuffer* buffer, zp_uint subResource ) = 0;
	virtual void updateBuffer( zpBuffer* buffer, const void* data ) = 0;
	virtual void updateTexture( zpTexture* texture, const void* data ) = 0;

	virtual void draw( zp_uint vertexCount, zp_uint startIndex = 0 ) = 0;
	virtual void drawAuto() = 0;
	virtual void drawIndexed( zp_uint indexCount, zp_uint startIndex = 0, zp_uint startVertex = 0 ) = 0;
};
*/

#endif