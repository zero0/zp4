#include "zpCommon.h"

zpAnimatedMeshRendererComponent::zpAnimatedMeshRendererComponent( zpObject* obj, const zpBison::Value& def )
	: zpComponent( obj )
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

	const zp_char* meshFile = def[ "Mesh" ].asCString();
	ok = getApplication()->getRenderPipeline()->getMeshContentManager()->getResource( meshFile, m_mesh );
	ZP_ASSERT_WARN( ok, "Unable to load mesh %s", meshFile );

	const zp_char* skeleton = def[ "Skeleton" ].asCString();
	ok = getApplication()->getRenderPipeline()->getSkeletonContentManager()->getResource( skeleton, m_skeleton );
	ZP_ASSERT_WARN( ok, "Unable to load skeleton %s", skeleton );

	const zp_char* animation = def[ "Animation" ].asCString();
	ok = getApplication()->getRenderPipeline()->getAnimationContentManager()->getResource( animation, m_animation );
	ZP_ASSERT_WARN( ok, "Unable to load animation %s", animation );
}
zpAnimatedMeshRendererComponent::~zpAnimatedMeshRendererComponent()
{

}

void zpAnimatedMeshRendererComponent::render( zpRenderingContext* i )
{

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

}
