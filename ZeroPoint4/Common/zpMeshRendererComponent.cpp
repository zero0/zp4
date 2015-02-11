#include "zpCommon.h"

zpMeshRendererComponent::zpMeshRendererComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
	, m_layer( ZP_RENDERING_LAYER_DEFAULT )
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
		ok = getApplication()->getRenderPipeline()->getMaterialContentManager()->getResource( materialFile, m_material );
		ZP_ASSERT_WARN( ok, "Unable to load material %s", materialFile );
	}
	//else if( material.isArray() && material.isEmpty() )
	//{
	//	material.foreachArray( [ this, &ok ]( zpBison::Value& mat ) {
	//		const zp_char* materialFile = mat.asCString();
	//		ok = getApplication()->getRenderPipeline()->getMaterialContentManager()->getResource( materialFile, m_material );
	//		ZP_ASSERT_WARN( ok, "Unable to load material %s", materialFile );
	//	} );
	//}
}
zpMeshRendererComponent::~zpMeshRendererComponent() {}

void zpMeshRendererComponent::render( zpRenderingContext* i )
{
	// get the world transform from the object or the calculated one from the attachment component
	zpTransformComponent* attachment = getParentObject()->getComponents()->getTransformComponent();
	const zpMatrix4f& transform = attachment->getWorldTransform();

	// draw mesh
	i->drawMesh( m_layer, ZP_RENDERING_QUEUE_OPAQUE, &m_mesh, transform, &m_material );
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
	return false; //m_hasMaterialOverride;
}
void zpMeshRendererComponent::setMaterialOverride( const zp_char* materialFile )
{
	//m_hasMaterialOverride = getApplication()->getRenderPipeline()->getMaterialContentManager()->getResource( materialFile, m_materials );
	//ZP_ASSERT_WARN( m_hasMaterialOverride, "Unable to load material %s", materialFile );
}
void zpMeshRendererComponent::resetMaterialOverride()
{
	

	//m_materials.release();
	//m_hasMaterialOverride = false;
}

void zpMeshRendererComponent::onCreate() {}
void zpMeshRendererComponent::onInitialize() {}
void zpMeshRendererComponent::onDestroy()
{
	m_material.release();
	m_mesh.release();
	//m_materials.foreach( []( zpMaterialResourceInstance& mat ) { mat.release(); } );
	//m_materials.destroy();
}

void zpMeshRendererComponent::onUpdate( zp_float deltaTime, zp_float realTime ) {}
void zpMeshRendererComponent::onSimulate() {}

void zpMeshRendererComponent::onEnabled() {}
void zpMeshRendererComponent::onDisabled() {}


zpMeshRendererComponentPool::zpMeshRendererComponentPool() {}
zpMeshRendererComponentPool::~zpMeshRendererComponentPool() {}

void zpMeshRendererComponentPool::update( zp_float deltaTime, zp_float realTime )
{
	m_used.foreach( [ &deltaTime, &realTime ]( zpMeshRendererComponent* o )
	{
		o->update( deltaTime, realTime );
	} );
}
void zpMeshRendererComponentPool::simulate() {}

void zpMeshRendererComponentPool::render( zpRenderingContext* i )
{
	m_used.foreach( [ i ]( zpMeshRendererComponent* o )
	{
		if( o->isEnabled() ) o->render( i );
	} );
}