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
	zpOpenGLRenderTarget( zp_uint* framebuffers, zp_uint* textures, zp_uint count, zp_uint width, zp_uint height );

	zpArray<zp_uint>& getFramebuffers();
	zpArray<zp_uint>& getTextures();

	mutable zp_uint m_referenceCount;
	zp_uint m_width;
	zp_uint m_height;

	zpArray<zp_uint> m_framebuffers;
	zpArray<zp_uint> m_textures;

	friend class zpOpenGLRenderingEngine;
	friend class zpOpenGLRenderingContext;
};

#endif