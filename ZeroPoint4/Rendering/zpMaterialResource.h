#pragma once
#ifndef ZP_MATERIAL_RESOURCE_H
#define ZP_MATERIAL_RESOURCE_H

struct zpMaterial {
	zpResourceInstance< zpShader > shader;
	zpArray< zpResourceInstance< zpTextureResource >, ZP_MATERIAL_TEXTURE_SLOT_Count > textures;
};

class zpMaterialResource : public zpResource {
public:
	zpMaterialResource();
	virtual ~zpMaterialResource();

	zp_bool load();
	void unload();

	const zpMaterial& getMaterial() const;

private:
	zpMaterial m_material;
};

ZP_RESOURCE_INSTANCE_TEMPLATE_START( zpMaterialResource )
ZP_RESOURCE_INSTANCE_TEMPLATE_END

#endif