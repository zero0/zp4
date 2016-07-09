#ifndef ZP_LIGHT_COMPONENT_H
#define ZP_LIGHT_COMPONENT_H

class zpLightComponent : public zpComponent
{
public:
    zpLightComponent( zpObject* obj, const zpBison::Value& def );
    virtual ~zpLightComponent();

    const zpLightBufferData* getLight() const { return m_light; }
    zpLightBufferData* getLight() { return m_light; }

protected:
    void onCreate();
    void onInitialize();
    void onDestroy();

    void onUpdate( zp_float deltaTime, zp_float realTime );
    void onSimulate();

    void onEnabled();
    void onDisabled();

private:
    zpLightBufferData* m_light;
};


class zpLightComponentPool : public zpContentPool< zpLightComponent, 8 >
{
public:
    zpLightComponentPool();
    virtual ~zpLightComponentPool();

    void update( zp_float deltaTime, zp_float realTime );
    void simulate();
};

#endif