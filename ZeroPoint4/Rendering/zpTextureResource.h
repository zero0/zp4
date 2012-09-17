#pragma once
#ifndef ZP_TEXTURE_RESOURCE_H
#define ZP_TEXTURE_RESOURCE_H

ZP_PURE_INTERFACE zpTextureResource : public zpResource {
public:
	virtual zpTexture* getTexture() const = 0;
};

#endif