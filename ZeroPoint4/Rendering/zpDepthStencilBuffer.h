#pragma once
#ifndef ZP_DEPTH_STENCIL_BUFFER_H
#define ZP_DEPTH_STENCIL_BUFFER_H

class zpDepthStencilBufferImpl;

class zpDepthStencilBuffer
{
public:
	~zpDepthStencilBuffer();

	zp_uint getWidth() const;
	zp_uint getHeight() const;

	zpDisplayFormat getDisplayFormat() const;

	zpDepthStencilBufferImpl* getDepthStencilBufferImpl() const;

private:
	zpDepthStencilBuffer( zpDepthStencilBufferImpl* impl );

	zpDepthStencilBufferImpl* m_impl;

	friend class zpRenderingEngine;
};

#endif