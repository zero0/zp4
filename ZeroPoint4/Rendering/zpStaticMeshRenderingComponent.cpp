#include "zpRendering.h"

zpStaticMeshRenderingComponent::zpStaticMeshRenderingComponent() {}
zpStaticMeshRenderingComponent::~zpStaticMeshRenderingComponent() {}

void zpStaticMeshRenderingComponent::render() {
	if( !m_mesh ) return;

	zpRenderingContext* c = zpRenderingFactory::getRenderingEngine()->getImmediateRenderingContext();

	zp_uint numParts = m_mesh.getResource()->getNumMeshParts();
	for( zp_uint i = 0; i < numParts; ++i ) {
		const zpStaticMeshPart& part = m_mesh.getResource()->getMeshPart( i );

		if( !part.shader ) continue;

		c->setTopology( part.topology );
		c->setBuffer( part.vertexBuffer );
		if( part.indexBuffer ) c->setBuffer( part.indexBuffer );
		c->setShader( &part.shader );

		for( zp_uint t = 0; t < ZP_STATIC_MESH_PART_NUM_TEXTURES; ++t ) {
			if( part.textures[ i ] ) c->setTexture( ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, i, &part.textures[ i ] );
		}

		if( part.indexBuffer ) {
			c->drawIndexed( part.indexBuffer->getCount() );
		} else {
			c->draw( part.vertexBuffer->getCount() );
		}
	}
}

void zpStaticMeshRenderingComponent::receiveMessage( const zpMessage& message ) {}

void zpStaticMeshRenderingComponent::serialize( zpSerializedOutput* out ) {}
void zpStaticMeshRenderingComponent::deserialize( zpSerializedInput* in ) {}

void zpStaticMeshRenderingComponent::onCreate() {
	m_mesh = getGameManagerOfType<zpContentManager>()->createInstanceOfResource<zpStaticMeshResource>( "cube" );
}
void zpStaticMeshRenderingComponent::onDestroy() {}

void zpStaticMeshRenderingComponent::onUpdate() {}

void zpStaticMeshRenderingComponent::onEnabled() {}
void zpStaticMeshRenderingComponent::onDisabled() {}

void zpStaticMeshRenderingComponent::onShow() {}
void zpStaticMeshRenderingComponent::onHide() {}