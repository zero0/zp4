#pragma once
#ifndef ZP_TEXTURE_H
#define ZP_TEXTURE_H

class zpTextureImpl;

enum zpTextureDimension
{
	ZP_TEXTURE_DIMENSION_UNKNOWN,
	ZP_TEXTURE_DIMENSION_1D,
	ZP_TEXTURE_DIMENSION_2D,
	ZP_TEXTURE_DIMENSION_3D,
	ZP_TEXTURE_DIMENSION_CUBE_MAP,
};

enum zpTextureType
{
	ZP_TEXTURE_TYPE_TEXTURE,
	ZP_TEXTURE_TYPE_RENDER_TARGET,
	ZP_TEXTURE_TYPE_RENDER_TEXTURE
};

class zpTexture
{
	ZP_NON_COPYABLE( zpTexture );

public:
	~zpTexture();

	zpTextureDimension getTextureDimension() const;
	zpTextureType getTextureType() const;

	zp_uint getWidth() const;
	zp_uint getHeight() const;

	zpTextureImpl* getTextureImpl() const;

private:
	zpTexture( zpTextureImpl* textureImpl );

	zpTextureImpl* m_textureImpl;

	friend class zpRenderingEngine;
};

#endif