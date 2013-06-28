#pragma once
#ifndef ZP_MATERIAL_H
#define ZP_MATERIAL_H

struct zpMaterial
{
	zpShaderResourceInstance shader;
	zpFixedArrayList< zpTextureResourceInstance, zpMaterialTextureSlot_Count > textures;
};

class zpMaterialResource : public zpResource< zpMaterial >
{
private:
	zp_bool load( const zp_char* filename );
	void unload();
};

class zpMaterialResourceInstance : public zpResourceInstance< zpMaterialResource >
{
public:
	void setTextureOverride( zpMaterialTextureSlot slot, const zpTextureResourceInstance& texture );
	void resetTexture( zpMaterialTextureSlot slot );

	void setBuffer( zpResourceBindSlot slot, zp_uint index, zpBuffer* buffer );

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