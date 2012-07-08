#pragma once
#ifndef ZP_DX11_TEXTURE_RESOURCE_H
#define ZP_DX11_TEXTURE_RESOURCE_H

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

class zpDX11TextureResource : public zpTextureResource {
public:
	~zpDX11TextureResource();

	zp_bool load();
	void unload();

	zpTextureType getTextureType() const;
	zp_uint getWidth() const;
	zp_uint getHeight() const;

private:
	zpDX11TextureResource();

	ID3D11Resource* getTexture() const;
	ID3D11ShaderResourceView* getTextureResourceView() const;

	zp_uint m_width;
	zp_uint m_height;
	zpTextureType m_type;

	ID3D11Resource* m_texture;
	ID3D11ShaderResourceView* m_textureRV;

	friend class zpDX11RenderingEngine;
};

#endif