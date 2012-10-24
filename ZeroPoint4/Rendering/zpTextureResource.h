#pragma once
#ifndef ZP_TEXTURE_RESOURCE_H
#define ZP_TEXTURE_RESOURCE_H

ZP_PURE_INTERFACE zpTextureResource : public zpResource {
public:
	virtual zpTexture* getTexture() const = 0;
};


ZP_RESOURCE_INSTANCE_TEMPLATE_START( zpTextureResource )
ZP_RESOURCE_INSTANCE_TEMPLATE_END


#endif