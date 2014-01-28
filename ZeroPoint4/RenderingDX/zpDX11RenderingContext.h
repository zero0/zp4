#pragma once
#ifndef ZP_DX11_RENDERING_CONTEXT_H
#define ZP_DX11_RENDERING_CONTEXT_H

struct ID3D11DeviceContext;

class zpRenderingContextImpl
{
public:
	zpRenderingContextImpl();
	~zpRenderingContextImpl();

	void setRenderTarget( zp_uint startIndex, zp_uint count, zpTexture* const* targets, zpDepthStencilBuffer* depthStencilBuffer );

	void clearRenderTarget( zpTexture* renderTarget, const zpColor4f& clearColor );
	void clearDepthStencilBuffer( zpDepthStencilBuffer* depthStencilBuffer, zp_float clearDepth, zp_uint clearStencil );
	void clearDepthBuffer( zpDepthStencilBuffer* depthStencilBuffer, zp_float clearDepth );
	void clearStencilBuffer( zpDepthStencilBuffer* depthStencilBuffer, zp_uint clearStencil );
	void clearState();

	void setViewport( const zpViewport& viewport );
	void setScissorRect( const zpRecti& rect );

	void setRasterState( zpRasterState* raster );
	void setSamplerState( zp_uint bindSlots, zp_uint index, zpSamplerState* sampler );
	void setConstantBuffer( zp_uint bindSlots, zp_uint index, zpBuffer* buffer );

	void map( zpBufferImpl* buffer, void** data, zpMapType mapType = ZP_MAP_TYPE_WRITE_DISCARD, zp_uint subResource = 0 );
	void unmap( zpBufferImpl* buffer, zp_uint subResource = 0 );

	void update( zpBufferImpl* buffer, const void* data, zp_uint size );

	void processCommand( zpRenderingEngineImpl* engine, const zpRenderingCommand* command );
	void processCommands( zpRenderingEngineImpl* engine, const zpArrayList< zpRenderingCommand* >& renderCommands );

	void set( ID3D11DeviceContext* context ) { m_context = context; }

private:
	void bindMaterial( const zpMaterial* material );

	ID3D11DeviceContext* m_context;

	const zpMaterial* m_prevMaterial;
};

#if 0
class zpDX11RenderingContext : public zpRenderingContext {
public:
	virtual ~zpDX11RenderingContext();

	const zpString& getName() const;

	void setRenderTarget( zpTexture* target, zp_uint index = 0 );
	zpTexture* getRenderTarget( zp_uint index = 0 ) const;

	void setDepthStencilBuffer( zpDepthStencilBuffer* depthBuffer );
	zpDepthStencilBuffer* getDepthStencilBuffer() const;

	void bindRenderTargetAndDepthBuffer();
	void unbindRenderTargetAndDepthBuffer();

	void clearRenderTarget( const zpColor4f* colors, zp_uint count = 1 );
	void clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil );

	void setBuffer( const zpBuffer* buffer, zp_uint slot = 0 );
	void setBuffers( const zpBuffer** buffers, zp_uint count, zp_uint slot = 0 );

	void setVertexLayout( const zpVertexLayout* layout );

	void setShader( const zpResourceInstance<zpShaderResource>* shader );
	void setTexture( zpResourceBindSlot bindSlot, zp_uint slot, const zpTexture* texture );

	void setTopology( zpTopology topology );

	void setViewport( const zpViewport& viewport );
	void setScissorRect( const zpRecti& rect );
	void getScissorRect( zpRecti& rect ) const;

	void setSamplerState( zpResourceBindSlot bindSlot, zp_uint slot, zpSamplerState* state );
	void setRasterState( zpRasterState* raster );

	void map( zpBuffer* buffer, zpMapType mapType, zp_uint subResource, void** data );
	void unmap( zpBuffer* buffer, zp_uint subResource );
	void updateBuffer( zpBuffer* buffer, const void* data );
	void updateTexture( zpTexture* texture, const void* data );
	
	void draw( zp_uint vertexCount, zp_uint startIndex = 0 );
	void drawAuto();
	void drawIndexed( zp_uint indexCount, zp_uint startIndex = 0, zp_uint startVertex = 0 );


	void addReference() const;
	zp_bool removeReference() const;

	zp_uint getReferenceCount() const;

	void markForAutoDelete( zp_bool marked ) const;
	zp_bool isMarkedForAutoDelete() const;

private:
	zpDX11RenderingContext();
	zpDX11RenderingContext( ID3D11DeviceContext* context, const zpString& name );
	
	ID3D11DeviceContext* getContext() const;

	ID3D11DeviceContext* m_context;

	zpArray<zpTexture*, ZP_RENDER_TARGET_MAX_COUNT> m_renderTargets;
	zpArray<ID3D11RenderTargetView*, ZP_RENDER_TARGET_MAX_COUNT> m_targets;

	zpDepthStencilBuffer* m_depthStencilBuffer;

	mutable zp_uint m_referenceCount;

	zpString m_name;

	friend class zpDX11RenderingEngine;
};
#endif

#endif