#pragma once
#ifndef ZP_DX11_TEXTURE_RESOURCE_H
#define ZP_DX11_TEXTURE_RESOURCE_H

class zpDX11TextureResource : public zpTextureResource {
public:
    ~zpDX11TextureResource();

    zp_bool load();
    void unload();

    zpTexture* getTexture() const;

private:
    zpDX11TextureResource();

    //zpDX11Texture m_texture;

    //friend class zpDX11RenderingEngine;
};

#endif