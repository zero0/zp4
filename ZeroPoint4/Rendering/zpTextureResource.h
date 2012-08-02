#pragma once
#ifndef ZP_TEXTURE_RESOURCE_H
#define ZP_TEXTURE_RESOURCE_H

ZP_PURE_INTERFACE zpTextureResource : public zpResource {
public:
	virtual zpTextureType getTextureType() const = 0;

	virtual zp_uint getWidth() const = 0;
	virtual zp_uint getHeight() const = 0;

	virtual zpTexture* getTexture() const = 0;
};

#endif