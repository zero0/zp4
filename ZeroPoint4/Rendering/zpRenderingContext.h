#pragma once
#ifndef ZP_RENDERING_CONTEXT
#define ZP_RENDERING_CONTEXT

ZP_PURE_INTERFACE zpRenderingContext : public zpReferencedObject {
public:
	virtual const zpString& getName() const = 0;

	virtual void setRenderTarget( zpRenderTarget* target ) = 0;
	virtual zpRenderTarget* getRenderTarget() const = 0;

	virtual void setDepthStencilBuffer( zpDepthStencilBuffer* depthBuffer ) = 0;
	virtual zpDepthStencilBuffer* getDepthStencilBuffer() const = 0;

	virtual void bindRenderTargetAndDepthBuffer() = 0;
	virtual void unbindRenderTargetAndDepthBuffer() = 0;

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
	virtual void setTexture( zpResourceBindSlot bindSlot, zp_uint slot, const zpResourceInstance<zpTextureResource>* texture ) = 0;

	virtual void setTopology( zpTopology topology ) = 0;

	virtual void setViewport( const zpViewport& viewport ) = 0;

	virtual void setSamplerState( zpResourceBindSlot bindSlot, zp_uint slot, zpSamplerState* state ) = 0;
	virtual void setRasterState( zpRasterState* raster ) = 0;

	virtual void map( zpBuffer* buffer, zpMapType mapType, zp_uint subResource, void** data ) = 0;
	virtual void unmap( zpBuffer* buffer, zp_uint subResource ) = 0;
	virtual void updateBuffer( zpBuffer* buffer, const void* data ) = 0;

	virtual void draw( zp_uint vertexCount, zp_uint startIndex = 0 ) = 0;
	virtual void drawAuto() = 0;
	virtual void drawIndexed( zp_uint indexCount, zp_uint startIndex = 0, zp_uint startVertex = 0 ) = 0;
};

#endif