#pragma once
#ifndef ZP_DX11_TEXTURE_H
#define ZP_DX11_TEXTURE_H

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

class zpDX11Texture : public zpTexture {
public:
	virtual ~zpDX11Texture();

	zpTextureType getTextureType() const;

	zp_uint getWidth() const;
	zp_uint getHeight() const;

	void addReference() const;
	zp_bool removeReference() const;

	zp_uint getReferenceCount() const;

	void markForAutoDelete( zp_bool marked ) const;
	zp_bool isMarkedForAutoDelete() const;

private:
	zpDX11Texture();

	ID3D11Resource* getTexture() const;
	ID3D11ShaderResourceView* getResourceView() const;

	mutable zp_uint m_referenceCount;
	zp_uint m_width;
	zp_uint m_height;
	zpTextureType m_type;

	ID3D11Resource* m_texture;
	ID3D11ShaderResourceView* m_textureResourceView;

	friend class zpDX11RenderingEngine;
	friend class zpDX11RenderingContext;
	friend class zpDX11TextureResource;
};

#endif