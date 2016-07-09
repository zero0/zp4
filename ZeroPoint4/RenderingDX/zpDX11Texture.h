#pragma once
#ifndef ZP_DX11_TEXTURE_H
#define ZP_DX11_TEXTURE_H

struct ID3D11Resource;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;

class zpTextureImpl
{
public:
    zpTextureImpl();
    ~zpTextureImpl();

    zpTextureDimension getTextureDimension() const;
    zpTextureType getTextureType() const;

    zp_uint getWidth() const;
    zp_uint getHeight() const;

private:
    zp_uint m_width;
    zp_uint m_height;
    zpTextureDimension m_dimension;
    zpTextureType m_type;

    ID3D11Resource* m_texture;
    ID3D11ShaderResourceView* m_textureResourceView;
    ID3D11RenderTargetView* m_textureRenderTarget;

    friend class zpRenderingEngineImpl;
    friend class zpRenderingContextImpl;
};

#if 0
class zpDX11Texture : public zpTexture {
public:
    virtual ~zpDX11Texture();

    zpTextureDimension getTextureDimension() const;
    zpTextureType getTextureType() const;

    zp_uint getWidth() const;
    zp_uint getHeight() const;

private:
    zpDX11Texture();

    ID3D11Resource* getTexture() const;
    ID3D11ShaderResourceView* getResourceView() const;
    ID3D11RenderTargetView* getRenderTargetView() const;

    zp_uint m_width;
    zp_uint m_height;
    zpTextureDimension m_dimension;
    zpTextureType m_type;

    ID3D11Resource* m_texture;
    ID3D11ShaderResourceView* m_textureResourceView;
    ID3D11RenderTargetView* m_textureRenderTarget;

    friend class zpDX11RenderingEngine;
    friend class zpDX11RenderingContext;
    friend class zpDX11TextureResource;
};
#endif

#endif