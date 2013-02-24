#pragma once
#ifndef ZP_TEXTURE_H
#define ZP_TEXTURE_H

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

ZP_PURE_INTERFACE zpTexture
{
public:
	virtual zpTextureDimension getTextureDimension() const = 0;
	virtual zpTextureType getTextureType() const = 0;

	virtual zp_uint getWidth() const = 0;
	virtual zp_uint getHeight() const = 0;
};

#endif