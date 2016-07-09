#pragma once
#ifndef ZP_SKY_BOX_COMPONENT_H
#define ZP_SKY_BOX_COMPONENT_H

class zpSkyBoxComponent : public zpComponent
{
public:
    zpSkyBoxComponent( zpObject* obj, const zpBison::Value& def );
    virtual ~zpSkyBoxComponent();

    void render( zpRenderingContext* i );

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
    zpMaterialResourceInstance m_material;
    zpSphericalHarmonicsData m_shData;
};

class zpSkyBoxComponentPool : public zpContentPool< zpSkyBoxComponent, 4 >
{
public:
    zpSkyBoxComponentPool();
    virtual ~zpSkyBoxComponentPool();

    void update( zp_float deltaTime, zp_float realTime );
    void simulate();

    void render( zpRenderingContext* i );
};

#endif
