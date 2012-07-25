#pragma once
#ifndef ZP_DX11_RENDER_TARGET_H
#define ZP_DX11_RENDER_TARGET_H

struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

class zpDX11RenderTarget : public zpRenderTarget {
public:
	virtual ~zpDX11RenderTarget();

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
	zpDX11RenderTarget();
	zpDX11RenderTarget( ID3D11Texture2D** textures, ID3D11RenderTargetView** targets, zp_uint count, zp_uint width, zp_uint height );

	ID3D11RenderTargetView** getRenderTargets();
	ID3D11Texture2D** getTextures();

	mutable zp_uint m_referenceCount;
	zp_uint m_width;
	zp_uint m_height;

	zpArray<ID3D11RenderTargetView*> m_renderTargets;
	zpArray<ID3D11Texture2D*> m_textures;;

	friend class zpDX11RenderingContext;
	friend class zpDX11RenderingEngine;
};

#endif