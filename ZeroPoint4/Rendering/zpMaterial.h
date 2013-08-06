#pragma once
#ifndef ZP_MATERIAL_H
#define ZP_MATERIAL_H

class zpMaterialContentManager;

struct zpMaterial
{
	struct zpMaterialTextureSampler
	{
		zpTextureResourceInstance texture;
		zpSamplerState* sampler;
	};

	zpShaderResourceInstance shader;
	zpFixedArrayList< zpMaterialTextureSampler, zpMaterialTextureSlot_Count > textures;
};

class zpMaterialResource : public zpResource< zpMaterial >
{
private:
	zp_bool load( const zp_char* filename, zpRenderingPipeline* pipeline );
	void unload();

	friend class zpMaterialContentManager;
};

class zpMaterialResourceInstance : public zpResourceInstance< zpMaterialResource >
{
public:
	zpMaterialResourceInstance();
	~zpMaterialResourceInstance();

	void setTextureOverride( zpMaterialTextureSlot slot, const zpTextureResourceInstance& texture );
	void resetTexture( zpMaterialTextureSlot slot );

	const zpTexture* getTexture( zpMaterialTextureSlot slot ) const;
	const zpSamplerState* getSampler( zpMaterialTextureSlot slot ) const;

private:
	zpFixedArrayList< zpTextureResourceInstance, zpMaterialTextureSlot_Count > m_textureOverides;
};

class zpMaterialContentManager : public zpContentManager< zpMaterialResource, zpMaterialResourceInstance, zpMaterialContentManager, 128 >
{
private:
	zp_bool createResource( zpMaterialResource* res, const zp_char* filename );
	void destroyResource( zpMaterialResource* res );

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif
