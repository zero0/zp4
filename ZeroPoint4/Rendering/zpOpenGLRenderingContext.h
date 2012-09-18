#pragma once
#ifndef ZP_OPENGL_RENDERING_CONTEXT_H
#define ZP_OPENGL_RENDERING_CONTEXT_H

class zpOpenGLRenderingContext : public zpRenderingContext {
public:
	virtual ~zpOpenGLRenderingContext();

	const zpString& getName() const;

	void setRenderTarget( zpRenderTarget* target );
	zpRenderTarget* getRenderTarget() const;

	void setDepthStencilBuffer( zpDepthStencilBuffer* depthBuffer );
	zpDepthStencilBuffer* getDepthStencilBuffer() const;

	void bindRenderTargetAndDepthBuffer();
	void unbindRenderTargetAndDepthBuffer();

	void clearRenderTarget( const zpColor4f* colors, zp_uint count = 1 );
	void clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil );

	void bindBuffer( zpBuffer* buffer, zp_uint slot );
	void unbindBuffer( zpBuffer* buffer, zp_uint slot );

	void bindBuffers( zp_uint count, zpBuffer** buffers, zp_uint slot );
	void unbindBuffers( zp_uint count, zpBuffer** buffers, zp_uint slot );

	void bindTexture( zpResourceBindSlot bindType, zp_uint slot, zpResourceInstance<zpTextureResource>* texture );
	void unbindTexture( zpResourceBindSlot bindType, zp_uint slot, zpResourceInstance<zpTextureResource>* texture );

	void setVertexLayout( zpVertexLayout* layout );

	void map( zpBuffer* buffer, zpMapType mapType, zp_uint subResource, void** data );
	void unmap( zpBuffer* buffer, zp_uint subResource );

	void bindShader( zpResourceInstance<zpShaderResource>* shader );
	void unbindShader( zpResourceInstance<zpShaderResource>* shader );

	void setTopology( zpTopology topology );

	void draw( zp_uint vertexCount, zp_uint startIndex = 0 );

	void setViewport( const zpViewport& viewport );

	void setSamplerState( zpResourceBindSlot bindSlot, zp_uint slot, zpSamplerState* state );
	void setRasterState( zpRasterState* raster );

	void addReference() const;
	zp_bool removeReference() const;

	zp_uint getReferenceCount() const;

	void markForAutoDelete( zp_bool marked ) const;
	zp_bool isMarkedForAutoDelete() const;

private:
	zpOpenGLRenderingContext();
	zpOpenGLRenderingContext( void* context, void* hdc, const zpString& name );

	void* m_context;
	void* m_hdc;

	zpRenderTarget* m_renderTarget;
	zpDepthStencilBuffer* m_depthStencilBuffer;

	zp_uint m_topology;
	mutable zp_uint m_referenceCount;

	zpString m_name;

	friend class zpOpenGLRenderingEngine;
};

#endif