#pragma once
#ifndef ZP_DX11_DEPTH_STENCIL_BUFFER_H
#define ZP_DX11_DEPTH_STENCIL_BUFFER_H

struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

class zpDepthStencilBufferImpl
{
public:
	~zpDepthStencilBufferImpl();

	zp_uint getWidth() const;
	zp_uint getHeight() const;

	zpDisplayFormat getDisplayFormat() const;

private:
	zpDepthStencilBufferImpl();
	zpDepthStencilBufferImpl( zpDisplayFormat format, ID3D11Texture2D* texture, ID3D11DepthStencilView* view, zp_uint width, zp_uint height );

	ID3D11DepthStencilView* getDepthStencilView();
	ID3D11Texture2D* getDepthTexture();

	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11Texture2D* m_depthTexture;

	zpDisplayFormat m_format;
	zp_uint m_width;
	zp_uint m_height;

	friend class zpRenderingContextImpl;
	friend class zpRenderingEngineImpl;
};

#endif