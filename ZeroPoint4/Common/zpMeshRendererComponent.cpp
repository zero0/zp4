#include "zpCommon.h"

zpMeshRendererComponent::zpMeshRendererComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
{
	const zp_char* meshFile = def[ "Mesh" ].asCString();
	getApplication()->getRenderPipeline()->getMeshContentManager()->getResource( meshFile, m_mesh );
}
zpMeshRendererComponent::~zpMeshRendererComponent() {}

void zpMeshRendererComponent::render()
{
	zpRenderingContext* i = getApplication()->getRenderPipeline()->getRenderingEngine()->getImmediateRenderingContext();
	i->drawMesh( ZP_RENDERING_LAYER_OPAQUE, &m_mesh, getParentObject()->getTransform() );
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