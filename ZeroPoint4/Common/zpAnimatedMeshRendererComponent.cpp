#include "zpCommon.h"

zpAnimatedMeshRendererComponent::zpAnimatedMeshRendererComponent( zpObject* obj, const zpBison::Value& def )
    : zpComponent( obj )
    , m_layer( ZP_RENDERING_LAYER_DEFAULT )
{
    zp_bool ok = false;

    const zpBison::Value& layer = def[ "Layer" ];
    if( layer.isInt() )
    {
        m_layer = layer.asInt();
    }

    const zpBison::Value& material = def[ "Material" ];
    if( material.isString() )
    {
        const zp_char* materialFile = material.asCString();
        ok = getApplication()->getRenderPipeline()->getMaterialContentManager()->getResource( materialFile, m_material );
        ZP_ASSERT_WARN( ok, "Unable to load material %s", materialFile );
    }

    //const zp_char* meshFile = def[ "Mesh" ].asCString();
    //ok = getApplication()->getRenderPipeline()->getMeshContentManager()->getResource( meshFile, m_mesh );
    //ZP_ASSERT_WARN( ok, "Unable to load mesh %s", meshFile );

    //const zp_char* skeleton = def[ "Skeleton" ].asCString();
    //ok = getApplication()->getRenderPipeline()->getSkeletonContentManager()->getResource( skeleton, m_skeleton );
    //ZP_ASSERT_WARN( ok, "Unable to load skeleton %s", skeleton );

    //const zp_char* animation = def[ "Animation" ].asCString();
    //ok = getApplication()->getRenderPipeline()->getAnimationContentManager()->getResource( animation, m_animation );
    //ZP_ASSERT_WARN( ok, "Unable to load animation %s", animation );
}
zpAnimatedMeshRendererComponent::~zpAnimatedMeshRendererComponent()
{

}

void zpAnimatedMeshRendererComponent::render( zpRenderingContext* i )
{
    zpTransformComponent* attachment = getParentObject()->getComponents()->getTransformComponent();
    zpMatrix4f transform = attachment->getWorldTransform();

    i->beginDrawImmediate( m_layer, ZP_RENDERING_QUEUE_OPAQUE, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_NORMAL_COLOR_UV, &m_material );
    i->setMatrix( transform );

    zp_float v = 4.0f;

    zpVector4f p0, p1, p2, p3;
    p0 = zpMath::Vector4(  v, 0,  v, 1 );
    p1 = zpMath::Vector4(  v, 0, -v, 1 );
    p2 = zpMath::Vector4( -v, 0, -v, 1 );
    p3 = zpMath::Vector4( -v, 0,  v, 1 );

    zpVector2f uv0( 0, 1 );
    zpVector2f uv1( 0, 0 );
    zpVector2f uv2( 1, 0 );
    zpVector2f uv3( 1, 1 );

    zpColor4f c0( 1, 0, 0, 1 );
    zpColor4f c1( 0, 1, 0, 1 );
    zpColor4f c2( 0, 0, 1, 1 );
    zpColor4f c3( 1, 1, 1, 1 );
    zpVector4f n = zpMath::Vector4( 0, 1, 0, 0 );

    i->addVertex( p0, n, c0, uv0 );
    i->addVertex( p1, n, c1, uv1 );
    i->addVertex( p2, n, c2, uv2 );
    i->addVertex( p3, n, c3, uv3 );

    i->addTriangleIndex( 0, 1, 2 );
    i->addTriangleIndex( 2, 3, 0 );

    i->endDrawImmediate();
}

void zpAnimatedMeshRendererComponent::setRenderLayer( zp_uint layer )
{
    m_layer = layer;
}
zp_uint zpAnimatedMeshRendererComponent::getRenderLayer() const
{
    return m_layer;
}

zp_bool zpAnimatedMeshRendererComponent::hasMaterialOverride() const
{
    return false;
}
void zpAnimatedMeshRendererComponent::setMaterialOverride( const zp_char* materialFile )
{

}
void zpAnimatedMeshRendererComponent::resetMaterialOverride()
{

}

void zpAnimatedMeshRendererComponent::onCreate()
{

}
void zpAnimatedMeshRendererComponent::onInitialize()
{

}
void zpAnimatedMeshRendererComponent::onDestroy()
{
    m_mesh.release();
    m_skeleton.release();
    m_animation.release();
}

void zpAnimatedMeshRendererComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{

}
void zpAnimatedMeshRendererComponent::onSimulate()
{

}

void zpAnimatedMeshRendererComponent::onEnabled()
{

}
void zpAnimatedMeshRendererComponent::onDisabled()
{

}



zpAnimatedMeshRendererComponentPool::zpAnimatedMeshRendererComponentPool()
{

}
zpAnimatedMeshRendererComponentPool::~zpAnimatedMeshRendererComponentPool()
{

}

void zpAnimatedMeshRendererComponentPool::update( zp_float deltaTime, zp_float realTime )
{
    m_used.foreach( [ &deltaTime, &realTime ]( zpAnimatedMeshRendererComponent* o )
    {
        o->update( deltaTime, realTime );
    } );
}
void zpAnimatedMeshRendererComponentPool::simulate()
{

}

void zpAnimatedMeshRendererComponentPool::render( zpRenderingContext* i )
{
    m_used.foreach( [ i ]( zpAnimatedMeshRendererComponent* o )
    {
        o->render( i );
    } );
}
