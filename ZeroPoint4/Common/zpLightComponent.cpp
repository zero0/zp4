#include "zpCommon.h"

const zp_char* g_lightTypes[ zpLightType_Count ] =
{
    "Directional",
    "Point",
    "Spot",
};

zpLightComponent::zpLightComponent( zpObject* obj, const zpBison::Value& def )
    : zpComponent( obj )
    , m_light( ZP_NULL )
{
    zpLightType type = ZP_LIGHT_TYPE_DIRECTIONAL;

    const zpBison::Value& typeDef = def[ "Type" ];
    zpBisonStringToEnum( g_lightTypes, typeDef, type );

    m_light = getApplication()->getRenderPipeline()->getLight( type );

    const zpBison::Value& colorDef = def[ "Color" ];
    zpBisonArray4ToColor4( colorDef, m_light->color );

    switch( type )
    {
    case ZP_LIGHT_TYPE_POINT:
        {
            const zpBison::Value& radiusDef = def[ "Radius" ];
            m_light->radius = radiusDef.asFloat();
        }
        break;
    case ZP_LIGHT_TYPE_SPOT:
        {
            const zpBison::Value& radiusDef = def[ "Radius" ];
            const zpBison::Value& innerAngle = def[ "InnerAngle" ];
            const zpBison::Value& outerAngle = def[ "OuterAngle" ];

            m_light->radius = radiusDef.asFloat();
            m_light->innerAngle = innerAngle.asFloat();
            m_light->outerAngle = outerAngle.asFloat();
        }
        break;
    }
}
zpLightComponent::~zpLightComponent() {}


void zpLightComponent::onCreate() {}
void zpLightComponent::onInitialize()
{
    onUpdate( 0.f, 0.f );
}
void zpLightComponent::onDestroy()
{
    if( m_light != ZP_NULL )
    {
        getApplication()->getRenderPipeline()->releaseLight( m_light );
        m_light = ZP_NULL;
    }
}

void zpLightComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
    const zpMatrix4f& transform = getParentObject()->getComponents()->getTransformComponent()->getWorldTransform();

    switch( m_light->type )
    {
    case ZP_LIGHT_TYPE_DIRECTIONAL:
        {
            m_light->direction = zpMath::MatrixTransform( transform, zpMath::Vector4( 0, 0, 1, 0 ) );
            m_light->direction = zpMath::Vector4Normalize3( m_light->direction );
        }
        break;
    case ZP_LIGHT_TYPE_POINT:
        {
            m_light->position = transform.r[ 3 ];
        }
        break;
    case ZP_LIGHT_TYPE_SPOT:
        {
            m_light->direction = zpMath::MatrixTransform( transform, zpMath::Vector4( 0, 0, 1, 0 ) );
            m_light->direction = zpMath::Vector4Normalize3( m_light->direction );

            m_light->position = transform.r[ 3 ];
        }
        break;
    }
}
void zpLightComponent::onSimulate() {}

void zpLightComponent::onEnabled() {}
void zpLightComponent::onDisabled() {}


zpLightComponentPool::zpLightComponentPool() {}
zpLightComponentPool::~zpLightComponentPool() {}

void zpLightComponentPool::update( zp_float deltaTime, zp_float realTime )
{
    m_used.foreach( [ &deltaTime, &realTime ]( zpLightComponent* o )
    {
        o->update( deltaTime, realTime );
    } );
}
void zpLightComponentPool::simulate() {}
