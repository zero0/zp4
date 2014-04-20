#include "zpCommon.h"

zpMeshRendererComponent::zpMeshRendererComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_layer( ZP_RENDERING_LAYER_DEFAULT )
	, m_hasMaterialOverride( false )
{
	zp_bool ok = false;
	const zp_char* meshFile = def[ "Mesh" ].asCString();
	ok = getApplication()->getRenderPipeline()->getMeshContentManager()->getResource( meshFile, m_mesh );
	ZP_ASSERT_WARN( ok, "Unable to load mesh %s", meshFile );

	const zpBison::Value& layer = def[ "Layer" ];
	if( layer.isInt() )
	{
		m_layer = layer.asInt();
	}

	const zpBison::Value& material = def[ "Material" ];
	if( material.isString() )
	{
		const zp_char* materialFile = material.asCString();
		m_hasMaterialOverride = getApplication()->getRenderPipeline()->getMaterialContentManager()->getResource( materialFile, m_materialOverride );
		ZP_ASSERT_WARN( m_hasMaterialOverride, "Unable to load material %s", materialFile );
	}
}
zpMeshRendererComponent::~zpMeshRendererComponent() {}

void zpMeshRendererComponent::render( zpRenderingContext* i )
{
	i->drawMesh( m_layer, ZP_RENDERING_QUEUE_OPAQUE, &m_mesh, getParentObject()->getTransform(), m_hasMaterialOverride ? &m_materialOverride : ZP_NULL );
}

void zpMeshRendererComponent::setRenderLayer( zp_uint layer )
{
	m_layer = layer;
}
zp_uint zpMeshRendererComponent::getRenderLayer() const
{
	return m_layer;
}

zp_bool zpMeshRendererComponent::hasMaterialOverride() const
{
	return m_hasMaterialOverride;
}
void zpMeshRendererComponent::setMaterialOverride( const zp_char* materialFile )
{
	m_hasMaterialOverride = getApplication()->getRenderPipeline()->getMaterialContentManager()->getResource( materialFile, m_materialOverride );
	ZP_ASSERT_WARN( m_hasMaterialOverride, "Unable to load material %s", materialFile );
}
void zpMeshRendererComponent::resetMaterialOverride()
{
	m_materialOverride.release();
	m_hasMaterialOverride = false;
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

void zpMeshRendererComponentPool::render( zpRenderingContext* i )
{
	m_used.foreach( [ i ]( zpMeshRendererComponent* o )
	{
		o->render( i );
	} );
}