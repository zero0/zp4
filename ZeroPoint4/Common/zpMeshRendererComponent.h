#pragma once
#ifndef ZP_MESH_RENDERER_COMPONENT_H
#define ZP_MESH_RENDERER_COMPONENT_H

class zpMeshRendererComponent : public zpComponent
{
public:
    zpMeshRendererComponent( zpObject* obj, const zpBison::Value& def );
    virtual ~zpMeshRendererComponent();

    void render( zpRenderingContext* i );

    void setRenderLayer( zp_uint layer );
    zp_uint getRenderLayer() const;

    zp_bool hasMaterialOverride() const;
    void setMaterialOverride( const zp_char* materialFile );
    void resetMaterialOverride();

    void setCastsShadow( zp_uint shadowIndex, zp_bool castsShadow );
    zp_bool getCastsShadow( zp_uint shadowIndex ) const;

    void setReceivesShadow( zp_uint shadowIndex, zp_bool receivesShadow );
    zp_bool getReceivesShadow( zp_uint shadowIndex ) const;

protected:
    void onCreate();
    void onInitialize();
    void onDestroy();

    void onUpdate( zp_float deltaTime, zp_float realTime );
    void onSimulate();

    void onEnabled();
    void onDisabled();

private:
    zp_uint m_layer;

    zpFlag8 m_castsShadowMask;
    zpFlag8 m_receivesShadowMask;

    zpMeshResourceInstance m_mesh;
    zpMaterialResourceInstance m_material;
};


class zpMeshRendererComponentPool : public zpContentPool< zpMeshRendererComponent, 64 >
{
public:
    zpMeshRendererComponentPool();
    virtual ~zpMeshRendererComponentPool();

    void update( zp_float deltaTime, zp_float realTime );
    void simulate();

    void render( zpRenderingContext* i );
};

#endif
