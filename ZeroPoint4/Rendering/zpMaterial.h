#pragma once
#ifndef ZP_MATERIAL_H
#define ZP_MATERIAL_H

enum zpMaterialShaderType
{
    ZP_MATERIAL_SHADER_TYPE_VERTEX =        0,
    ZP_MATERIAL_SHADER_TYPE_PIXEL,
    ZP_MATERIAL_SHADER_TYPE_GEOMETRY,
    ZP_MATERIAL_SHADER_TYPE_COMPUTE,

    zpMaterialShaderType_Count
};

class zpMaterialContentManager;

struct zpMaterialGlobalVariable
{
    zpString name;
    zp_size_t offset;
    zp_size_t size;
};
struct zpMaterialTexture
{
    zpString name;
    zpTextureResourceInstance texture;
    zpSamplerState sampler;
    zpVector4f scaleOffset;
    zp_size_t slot;
};
struct zpMaterial
{
    zpShaderResourceInstance shader;

    zpBlendState blend;
    zpDepthStencilState depth;
    zpRasterState raster;

    zpBuffer globalVariables; //per-material shader $Globals constant buffer
    zpArrayList< zpMaterialGlobalVariable > globalVariablesDef;

    zpArrayList< zpMaterialTexture > materialTextures;

    zp_uint materialId;
    zp_int sortBias;

    zpRenderingQueue queue;

    friend class zpMaterialResource;
};

class zpMaterialResource : public zpResource< zpMaterial >
{
private:
    zp_bool load( const zp_char* filename, zpRenderingPipeline* pipeline, zp_uint materialId );
    void unload( zpRenderingPipeline* pipeline );

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

    zp_uint m_currentMaterialId;

    template<typename Resource, typename ResourceInstance, typename ImplManager, zp_uint ResourceCount>
    friend class zpContentManager;
};

#endif
