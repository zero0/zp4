#pragma once
#ifndef ZP_OPENGL_DEPTH_STENCIL_BUFFER_H
#define ZP_OPENGL_DEPTH_STENCIL_BUFFER_H

class zpDepthStencilBufferImpl
{
public:
	zpDepthStencilBufferImpl();
	~zpDepthStencilBufferImpl();

	zp_uint getWidth() const;
	zp_uint getHeight() const;

	zpDisplayFormat getDisplayFormat() const;

private:
	zp_uint m_depthStencilView;
	zp_uint m_depthTexture;

	zpDisplayFormat m_format;
	zp_uint m_width;
	zp_uint m_height;

	friend class zpRenderingContextImpl;
	friend class zpRenderingEngineImpl;
};

#endif