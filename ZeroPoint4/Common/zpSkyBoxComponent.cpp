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
}
zpSkyBoxComponent::~zpSkyBoxComponent()
{}

void zpSkyBoxComponent::render( zpRenderingContext* i )
{
	i->beginDrawImmediate( m_layer, ZP_RENDERING_QUEUE_SKYBOX, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, &m_material );

	i->addVertex( zpVector4f( -1, -1, -1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 1, 1, 1, 1 ) );
	i->addVertex( zpVector4f( -1,  1, -1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 1, 0, 0, 1 ) );
	i->addVertex( zpVector4f(  1,  1, -1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 0, 1, 0, 1 ) );
	i->addVertex( zpVector4f(  1, -1, -1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 0, 0, 1, 1 ) );
	
	i->addVertex( zpVector4f(  1, -1,  1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 0, 0, 0, 1 ) );
	i->addVertex( zpVector4f(  1,  1,  1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 0, 1, 1, 1 ) );
	i->addVertex( zpVector4f( -1,  1,  1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 1, 0, 1, 1 ) );
	i->addVertex( zpVector4f( -1, -1,  1,  1 ), zpVector2f( 0, 0 ), zpColor4f( 1, 1, 0, 1 ) );
	
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
{}
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