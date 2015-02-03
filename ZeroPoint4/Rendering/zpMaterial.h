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
	
	zpBlendState blend;
	zpDepthStencilState depth;

	zpBuffer globalVariables;

	zp_ushort materialId;
	zp_ushort sortBias;
	zpShaderResourceInstance shader;
	zpFixedArrayList< zpMaterialTextureSampler, zpMaterialTextureSlot_Count > textures;
};

struct zpMaterialConstantBufferSlot
{
	zp_uint index;
	zpResourceBindSlotType bindSlots;
	zpString name;
};
struct zpTextureBindSlot
{
	zp_uint index;
	zpString bindName;
	zpTextureResourceInstance texture;
	zpSamplerState sampler;
};
struct zpMaterialGlobalVariable
{
	zpString name;
	zp_uint offset;
	zp_uint size;
};
class zpNewMaterial
{
public:
	zpNewMaterial();
	~zpNewMaterial();

	zp_bool setGlobal( const zp_char* name, zp_float value );
	zp_bool setGlobal( const zp_char* name, const zpVector2f& value );
	zp_bool setGlobal( const zp_char* name, const zpVector4f& value );
	zp_bool setGlobal( const zp_char* name, const zpColor4f& value );
	zp_bool setGlobal( const zp_char* name, const zpMatrix4f& value );

	zp_bool setTexture( const zp_char* name, const zpTextureResourceInstance& texture );
	zp_bool setSampler( const zp_char* name, const zpSamplerStateDesc& samplerDesc );

private:
	zpShaderResourceInstance shader;

	zpBlendState blend;
	zpDepthStencilState depth;

	zpBuffer globalVariables; //per-material shader $Globals constant buffer



	zpArrayList< zpMaterialGlobalVariable > globalVaraiblesDef;

	zp_ushort materialId;
	zp_ushort sortBias;
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
