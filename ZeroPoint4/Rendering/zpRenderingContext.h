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

	template<zp_uint I>
	void clearRenderTarget( const zpColor4f (&colors)[I] ) {
		clearRenderTarget( colors, I );
	}
	virtual void clearRenderTarget( const zpColor4f* colors, zp_uint count = 1 ) = 0;
	virtual void clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil ) = 0;

	virtual void bindBuffer( zpBuffer* buffer, zp_uint slot = 0 ) = 0;
	virtual void unbindBuffer( zpBuffer* buffer, zp_uint slot = 0 ) = 0;

	virtual void bindBuffers( zp_uint count, zpBuffer** buffers, zp_uint slot = 0 ) = 0;
	virtual void unbindBuffers( zp_uint count, zpBuffer** buffers, zp_uint slot = 0 ) = 0;

	virtual void setVertexLayout( zpVertexLayout* layout ) = 0;

	virtual void map( zpBuffer* buffer, zpMapType mapType, zp_uint subResource, void** data ) = 0;
	virtual void unmap( zpBuffer* buffer, zp_uint subResource ) = 0;

	virtual void bindShader( zpShaderResource* shader ) = 0;
	virtual void unbindShader( zpShaderResource* shader ) = 0;

	virtual void setTopology( zpTopology topology ) = 0;

	virtual void draw( zp_uint vertexCount, zp_uint startIndex = 0 ) = 0;

	virtual void setViewport( const zpViewport& viewport ) = 0;
};

#endif