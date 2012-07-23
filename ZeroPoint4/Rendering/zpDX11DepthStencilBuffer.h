#pragma once
#ifndef ZP_DX11_DEPTH_STENCIL_BUFFER_H
#define ZP_DX11_DEPTH_STENCIL_BUFFER_H

struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

class zpDX11DepthStencilBuffer : zpDepthStencilBuffer {
public:
	virtual ~zpDX11DepthStencilBuffer();

	zp_uint getWidth() const;
	zp_uint getHeight() const;

	zpRenderingDisplayFormat getDisplayFormat() const;

	void addReference() const;
	zp_bool removeReference() const;

	zp_uint getReferenceCount() const;

	void markForAutoDelete( zp_bool marked ) const;
	zp_bool isMarkedForAutoDelete() const;

private:
	zpDX11DepthStencilBuffer();
	zpDX11DepthStencilBuffer( ID3D11Texture2D* texture, ID3D11DepthStencilView* view, zp_uint width, zp_uint height );

	ID3D11DepthStencilView* getDepthStencilView();
	ID3D11Texture2D* getDepthTexture();

	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11Texture2D* m_depthTexture;

	mutable zp_uint m_referenceCount;
	zp_uint m_width;
	zp_uint m_height;

	friend class zpDX11RenderingContext;
	friend class zpDX11RenderingEngine;
};

#endif