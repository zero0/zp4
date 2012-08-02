#pragma once
#ifndef ZP_TEXTURE_H
#define ZP_TEXTURE_H

enum zpTextureType {
	ZP_TEXTURE_TYPE_1D,
	ZP_TEXTURE_TYPE_2D,
	ZP_TEXTURE_TYPE_3D,
	ZP_TEXTURE_TYPE_CUBE_MAP,

	//ZP_TEXTURE_TYPE_1D_ARRAY,
	//ZP_TEXTURE_TYPE_2D_ARRAY,
	//ZP_TEXTURE_TYPE_3D_ARRAY,
};

ZP_PURE_INTERFACE zpTexture : public zpReferencedObject {
public:
	virtual zpTextureType getTextureType() const = 0;

	virtual zp_uint getWidth() const = 0;
	virtual zp_uint getHeight() const = 0;
};

#endif