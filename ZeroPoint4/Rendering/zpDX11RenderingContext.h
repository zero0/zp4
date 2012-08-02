#pragma once
#ifndef ZP_DX11_RENDERING_CONTEXT_H
#define ZP_DX11_RENDERING_CONTEXT_H

struct ID3D11DeviceContext;

class zpDX11RenderingContext : public zpRenderingContext {
public:
	virtual ~zpDX11RenderingContext();

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

	void setVertexLayout( zpVertexLayout* layout );

	void map( zpBuffer* buffer, zpMapType mapType, zp_uint subResource, void** data );
	void unmap( zpBuffer* buffer, zp_uint subResource );

	void bindShader( zpShaderResource* shader );
	void unbindShader( zpShaderResource* shader );

	void setTopology( zpTopology topology );

	void draw( zp_uint vertexCount, zp_uint startIndex = 0 );

	void setViewport( const zpViewport& viewport );


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

	zpRenderTarget* m_renderTarget;
	zpDepthStencilBuffer* m_depthStencilBuffer;

	mutable zp_uint m_referenceCount;

	zpString m_name;

	friend class zpDX11RenderingEngine;
};

#endif