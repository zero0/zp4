#include "zpCommon.h"

zpMeshRendererComponent::zpMeshRendererComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_layer( ZP_RENDERING_LAYER_DEFAULT )
{
	zp_bool ok = false;
	const zp_char* meshFile = def[ "Mesh" ].asCString();
	ok = getApplication()->getRenderPipeline()->getMeshContentManager()->getResource( meshFile, m_mesh );
	ZP_ASSERT_WARN( ok, "Unable to load mesh %s", meshFile );

	zpBison::Value layer = def[ "Layer" ];
	if( layer.isInt() )
	{
		m_layer = layer.asInt();
	}
}
zpMeshRendererComponent::~zpMeshRendererComponent() {}

void zpMeshRendererComponent::render()
{
	zpRenderingContext* i = getApplication()->getRenderPipeline()->getRenderingEngine()->getImmediateRenderingContext();
	i->drawMesh( m_layer, ZP_RENDERING_QUEUE_OPAQUE, &m_mesh, getParentObject()->getTransform() );
}

void zpMeshRendererComponent::setRenderLayer( zp_uint layer )
{
	m_layer = layer;
}
zp_uint zpMeshRendererComponent::getRenderLayer() const
{
	return m_layer;
}

void zpMeshRendererComponent::onCreate() {}
void zpMeshRendererComponent::onInitialize() {}
void zpMeshRendererComponent::onDestroy() {}

void zpMeshRendererComponent::onUpdate() {}
void zpMeshRendererComponent::onSimulate() {}

void zpMeshRendererComponent::onEnabled() {}
void zpMeshRendererComponent::onDisabled() {}


zpMeshRendererComponentPool::zpMeshRendererComponentPool() {}
zpMeshRendererComponentPool::~zpMeshRendererComponentPool() {}

void zpMeshRendererComponentPool::update()
{
	m_used.foreach( []( zpMeshRendererComponent* o )
	{
		o->update();
	} );
}
void zpMeshRendererComponentPool::simulate() {}

void zpMeshRendererComponentPool::render()
{
	m_used.foreach( []( zpMeshRendererComponent* o )
	{
		o->render();
	} );
}