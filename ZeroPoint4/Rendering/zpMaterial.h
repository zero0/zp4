#pragma once
#ifndef ZP_MATERIAL_H
#define ZP_MATERIAL_H

class zpMaterialContentManager;

struct zpMaterial
{
	struct zpMaterialTextureSampler
	{
		zpTextureResourceInstance texture;
		zpResourceBindSlotType bindSlots;
		zpSamplerState sampler;
	};

	zp_ushort materialId;
	zpShaderResourceInstance shader;
	zpFixedArrayList< zpMaterialTextureSampler, zpMaterialTextureSlot_Count > textures;
};

class zpMaterialResource : public zpResource< zpMaterial >
{
private:
	zp_bool load( const zp_char* filename, zpRenderingPipeline* pipeline, zp_ushort materialId );
	void unload();

	friend class zpMaterialContentManager;
};

class zpMaterialResourceInstance : public zpResourceInstance< zpMaterialResource >
{};

class zpMaterialContentManager : public zpContentManager< zpMaterialResource, zpMaterialResourceInstance, zpMaterialContentManager, 128 >
{
public:
	zpMaterialContentManager();
	virtual ~zpMaterialContentManager() {}

private:
	zp_bool createResource( zpMaterialResource* res, const zp_char* filename );
	void destroyResource( zpMaterialResource* res );
	void initializeInstance( zpMaterialResourceInstance& instance ) {}

	zp_ushort m_currentMaterialId;

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif
