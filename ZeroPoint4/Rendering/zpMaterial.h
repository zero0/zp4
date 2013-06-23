#pragma once
#ifndef ZP_MATERIAL_H
#define ZP_MATERIAL_H

struct zpMaterial
{
	zpShaderResourceInstance shader;
	//zpFixedArrayList< zpResourceInstance< zpTextureResource >, ZP_MATERIAL_TEXTURE_SLOT_Count > textures;
};

class zpMaterialResource : public zpResource< zpMaterial >
{
private:
	zp_bool load( const zp_char* filename );
	void unload();
};

class zpMaterialResourceInstance : public zpResourceInstance< zpMaterialResource >
{};

class zpMaterialContentManager : public zpContentManager< zpMaterialResource, zpMaterialResourceInstance, zpMaterialContentManager, 128 >
{
private:
	zp_bool createResource( zpMaterialResource* res, const zp_char* filename );
	void destroyResource( zpMaterialResource* res );

	template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
	friend class zpContentManager;
};

#endif