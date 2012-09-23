#pragma once
#ifndef ZP_OPENGL_DEPTH_STENCIL_BUFFER_H
#define ZP_OPENGL_DEPTH_STENCIL_BUFFER_H

class zpOpenGLDepthStencilBuffer : public zpDepthStencilBuffer {
public:
	virtual ~zpOpenGLDepthStencilBuffer();

	zp_uint getWidth() const;
	zp_uint getHeight() const;

	zpDisplayFormat getDisplayFormat() const;

	void addReference() const;
	zp_bool removeReference() const;

	zp_uint getReferenceCount() const;

	void markForAutoDelete( zp_bool marked ) const;
	zp_bool isMarkedForAutoDelete() const;

private:
	zpOpenGLDepthStencilBuffer();
	zpOpenGLDepthStencilBuffer( zpDisplayFormat format, zp_uint framebuffer, zp_uint texture, zp_uint width, zp_uint height );

	zp_uint getFramebuffer() const;
	zp_uint getTexture() const;

	zpDisplayFormat m_format;
	mutable zp_uint m_referenceCount;
	zp_uint m_width;
	zp_uint m_height;

	zp_uint m_framebuffer;
	zp_uint m_texture;

	friend class zpOpenGLRenderingContext;
	friend class zpOpenGLRenderingEngine;
};

#endif