#pragma once
#ifndef ZP_RENDERING_CONTEXT
#define ZP_RENDERING_CONTEXT

class zpRenderingEngineImpl;
class zpRenderingContextImpl;

class zpRenderingContext
{
	ZP_NON_COPYABLE( zpRenderingContext );
public:

	void setRenderTarget( zp_uint startIndex, zp_uint count, zpTexture** targets, zpDepthStencilBuffer* depthStencilBuffer );

	void clearRenderTarget( zpTexture* renderTarget, const zpColor4f& clearColor );
	void clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil );

	void setViewport( const zpViewport& viewport );
	void setScissorRect( const zpRecti& rect );

	void setRasterState( zpRasterState* raster );

	void processCommands();

private:
	zpRenderingContext( zpRenderingEngine* engine, zpRenderingContextImpl* impl );

	zpRenderingContextImpl* m_renderContextImpl;
	zpRenderingEngine* m_renderingEngine;

	zpRenderingCommand* m_currentCommnad;
	zpFixedArrayList< zpRenderingCommand, ZP_RENDERING_MAX_COMMNADS > m_renderingCommands;

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