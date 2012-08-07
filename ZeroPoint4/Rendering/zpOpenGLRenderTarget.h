#pragma once
#ifndef ZP_OPENGL_RENDER_TARGET_H
#define ZP_OPENGL_RENDER_TARGET_H

class zpOpenGLRenderTarget : public zpRenderTarget {
public:
	virtual ~zpOpenGLRenderTarget();

	zp_uint getWidth() const;
	zp_uint getHeight() const;

	zp_uint getDisplayFormat( zpDisplayFormat* formats ) const;

	zp_uint getNumberOfTargets() const;

	void addReference() const;
	zp_bool removeReference() const;

	zp_uint getReferenceCount() const;

	void markForAutoDelete( zp_bool marked ) const;
	zp_bool isMarkedForAutoDelete() const;

private:
	zpOpenGLRenderTarget();
	zpOpenGLRenderTarget( zp_uint frameBuffer, zp_uint* renderBuffers, zp_uint count, zp_uint width, zp_uint height );

	zp_uint getFrameBuffer();
	zpArray<zp_uint>& getRenderBuffers();

	mutable zp_uint m_referenceCount;
	zp_uint m_width;
	zp_uint m_height;

	zp_uint m_frameBuffer;
	zpArray<zp_uint> m_renderBuffers;

	friend class zpOpenGLRenderingEngine;
	friend class zpOpenGLRenderingContext;
};

#endif