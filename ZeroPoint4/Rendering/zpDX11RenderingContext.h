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

	void clearRenderTarget( const zpColor4f* colors, zp_uint count = 1 );
	void clearDepthStencilBuffer( zp_float clearDepth, zp_uint clearStencil );

	void map( zpBuffer* buffer, zpRenderingMapType mapType, zp_uint subResource, void** data );
	void unmap( zpBuffer* buffer, zp_uint subResource );

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