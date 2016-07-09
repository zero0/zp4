#include "zpCommon.h"

zpSkyBoxComponent::zpSkyBoxComponent( zpObject* obj, const zpBison::Value& def )
    : zpComponent( obj )
    , m_layer( ZP_RENDERING_LAYER_DEFAULT )
{
    zp_bool ok = false;

    const zpBison::Value& layer = def[ "Layer" ];
    if( layer.isInt() )
    {
        m_layer = layer.asInt();
    }

    const zp_char* materialFile = def[ "Material" ].asCString();
    ok = getApplication()->getRenderPipeline()->getMaterialContentManager()->getResource( materialFile, m_material );
    ZP_ASSERT_WARN( ok, "Unable to load material %s", materialFile );

    m_shData.SH0 = zpMath::Vector4(  0.690826f,  0.791972f,  1.156998f, 0.f );
    m_shData.SH1 = zpMath::Vector4( -0.032214f, -0.032154f, -0.031143f, 0.f );
    m_shData.SH2 = zpMath::Vector4(  0.181760f,  0.213785f,  0.350721f, 0.f );
    m_shData.SH3 = zpMath::Vector4(  0.174773f,  0.165067f,  0.134813f, 0.f );
    m_shData.SH4 = zpMath::Vector4( -0.140021f, -0.133054f, -0.109773f, 0.f );
    m_shData.SH5 = zpMath::Vector4(  0.003920f,  0.002716f, -0.001993f, 0.f );
    m_shData.SH6 = zpMath::Vector4( -0.285459f, -0.332236f, -0.476629f, 0.f );
    m_shData.SH7 = zpMath::Vector4( -0.021688f, -0.022507f, -0.025049f, 0.f );
    m_shData.SH8 = zpMath::Vector4(  0.056419f,  0.054559f,  0.047959f, 0.f );
}
zpSkyBoxComponent::~zpSkyBoxComponent()
{}

void zpSkyBoxComponent::render( zpRenderingContext* i )
{
    zpBuffer& shBuffer = getApplication()->getRenderPipeline()->getConstantBuffer( ZP_CONSTANT_BUFFER_SLOT_SH );
    i->update( &shBuffer, &m_shData, sizeof( zpSphericalHarmonicsData ) );

    i->beginDrawImmediate( m_layer, ZP_RENDERING_QUEUE_SKYBOX, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, &m_material );

    i->addVertex( zpMath::Vector4( -1, -1, -1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 1, 1, 1, 1 ) );
    i->addVertex( zpMath::Vector4( -1,  1, -1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 1, 0, 0, 1 ) );
    i->addVertex( zpMath::Vector4(  1,  1, -1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 0, 1, 0, 1 ) );
    i->addVertex( zpMath::Vector4(  1, -1, -1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 0, 0, 1, 1 ) );

    i->addVertex( zpMath::Vector4(  1, -1,  1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 0, 0, 0, 1 ) );
    i->addVertex( zpMath::Vector4(  1,  1,  1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 0, 1, 1, 1 ) );
    i->addVertex( zpMath::Vector4( -1,  1,  1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 1, 0, 1, 1 ) );
    i->addVertex( zpMath::Vector4( -1, -1,  1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 1, 1, 0, 1 ) );
    
    i->addTriangleIndex( 0, 1, 2 ); //   1: face back
    i->addTriangleIndex( 0, 2, 3 ); 
    i->addTriangleIndex( 3, 2, 5 ); //   2: face right
    i->addTriangleIndex( 3, 5, 4 ); 
    i->addTriangleIndex( 5, 2, 1 ); //   3: face top
    i->addTriangleIndex( 5, 1, 6 ); 
    i->addTriangleIndex( 3, 4, 7 ); //   4: face bottom
    i->addTriangleIndex( 3, 7, 0 ); 
    i->addTriangleIndex( 0, 7, 6 ); //   5: face left
    i->addTriangleIndex( 0, 6, 1 ); 
    i->addTriangleIndex( 4, 5, 6 ); //   6: face front
    i->addTriangleIndex( 4, 6, 7 ); 

    //i->addVertex( zpVector4f( -1, -1, -1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 1, 1, 1, 1 ) );
    //i->addVertex( zpVector4f( -1,  1, -1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 1, 0, 0, 1 ) );
    //i->addVertex( zpVector4f(  1,  1, -1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 0, 1, 0, 1 ) );
    //i->addVertex( zpVector4f(  1, -1, -1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 0, 0, 1, 1 ) );
    //
    //i->addVertex( zpVector4f( -1, -1,  1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 0, 0, 0, 1 ) );
    //i->addVertex( zpVector4f( -1,  1,  1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 0, 1, 1, 1 ) );
    //i->addVertex( zpVector4f(  1,  1,  1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 1, 0, 1, 1 ) );
    //i->addVertex( zpVector4f(  1, -1,  1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 1, 1, 0, 1 ) );
    //
    //i->addTriangleIndex( 0, 1, 2 );
    //i->addTriangleIndex( 0, 2, 3 );
    //
    //i->addTriangleIndex( 4, 6, 5 );
    //i->addTriangleIndex( 4, 7, 6 );
    //
    //i->addTriangleIndex( 4, 5, 1 );
    //i->addTriangleIndex( 4, 1, 0 );
    //
    //i->addTriangleIndex( 3, 2, 6 );
    //i->addTriangleIndex( 3, 6, 7 );
    //
    //i->addTriangleIndex( 1, 5, 6 );
    //i->addTriangleIndex( 1, 6, 2 );
    //
    //i->addTriangleIndex( 4, 0, 3 );
    //i->addTriangleIndex( 4, 3, 7 );

    i->endDrawImmediate();
}

void zpSkyBoxComponent::onCreate()
{}
void zpSkyBoxComponent::onInitialize()
{}
void zpSkyBoxComponent::onDestroy()
{
    m_material.release();
}

void zpSkyBoxComponent::onUpdate( zp_float deltaTime, zp_float realTime )
{
    
}
void zpSkyBoxComponent::onSimulate()
{}

void zpSkyBoxComponent::onEnabled()
{}
void zpSkyBoxComponent::onDisabled()
{}



zpSkyBoxComponentPool::zpSkyBoxComponentPool()
{}
zpSkyBoxComponentPool::~zpSkyBoxComponentPool()
{}

void zpSkyBoxComponentPool::update( zp_float deltaTime, zp_float realTime )
{}
void zpSkyBoxComponentPool::simulate()
{}

void zpSkyBoxComponentPool::render( zpRenderingContext* i )
{
    m_used.foreach( [ i ]( zpSkyBoxComponent* o )
    {
        if( o->isEnabled() ) o->render( i );
    } );
}
