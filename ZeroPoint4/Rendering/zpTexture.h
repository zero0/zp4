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
public:
	zpTexture();
	~zpTexture();

	zpTextureDimension getTextureDimension() const;
	zpTextureType getTextureType() const;

	zp_uint getWidth() const;
	zp_uint getHeight() const;

	zpTextureImpl* getTextureImpl() const;

private:
	zpTextureImpl* m_textureImpl;

	friend class zpRenderingEngine;
};


class zpTextureResource : public zpResource< zpTexture >
{
private:
	zp_bool load( const zp_char* filename, zpRenderingEngine* engine );
	void unload();

	friend class zpTextureContentManager;
};

class zpTextureResourceInstance : public zpResourceInstance< zpTextureResource >
{};

class zpTextureContentManager : public zpContentManager< zpTextureResource, zpTextureResourceInstance, zpTextureContentManager, 128 >
{
private:
	zp_bool createResource( zpTextureResource* res, const zp_char* filename );
	void destroyResource( zpTextureResource* res );

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif