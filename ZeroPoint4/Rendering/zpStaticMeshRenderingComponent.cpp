#include "zpRendering.h"

zpStaticMeshRenderingComponent::zpStaticMeshRenderingComponent()
	: m_manager( ZP_NULL )
{}
zpStaticMeshRenderingComponent::~zpStaticMeshRenderingComponent() {}

void zpStaticMeshRenderingComponent::render() {
	if( !m_mesh ) return;

	zpRenderingContext* c = zpRenderingFactory::getRenderingEngine()->getImmediateRenderingContext();

	zpMatrix4f trans;
	m_localTransform.mul( getParentGameObject()->getWorldTransform(), trans );

	m_manager->getGlobalBuffer( ZP_RENDERING_GLOBAL_BUFFER_WORLD )->update( (zp_float*)trans );

	zp_uint numParts = m_mesh.getResource()->getNumMeshParts();
	for( zp_uint i = 0; i < numParts; ++i ) {
		const zpStaticMeshPart& part = m_mesh.getResource()->getMeshPart( i );

		if( !part.shader ) continue;

		c->setTopology( part.topology );
		c->setBuffer( part.vertexBuffer );
		if( part.indexBuffer ) c->setBuffer( part.indexBuffer );
		c->setShader( &part.shader );

		for( zp_uint t = 0; t < ZP_STATIC_MESH_PART_NUM_TEXTURES; ++t ) {
			if( part.textures[ i ] ) c->setTexture( ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, i, part.textures[ i ].getResource()->getTexture() );
		}

		if( part.indexBuffer ) {
			c->drawIndexed( part.indexBuffer->getCount() );
		} else {
			c->draw( part.vertexBuffer->getCount() );
		}
	}
}

void zpStaticMeshRenderingComponent::receiveMessage( const zpMessage& message ) {}

void zpStaticMeshRenderingComponent::serialize( zpSerializedOutput* out ) {
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	out->writeString( m_meshAlias, "@mesh" );

	out->endBlock();
}
void zpStaticMeshRenderingComponent::deserialize( zpSerializedInput* in ) {
	in->readBlock( ZP_SERIALIZE_TYPE_THIS );

	in->readString( &m_meshAlias, "@mesh" );

	in->endBlock();
}

void zpStaticMeshRenderingComponent::onCreate() {
	zpContentManager* content = getGameManagerOfType<zpContentManager>();
	m_manager = getGameManagerOfType<zpRenderingManager>();
	m_meshAlias = "cube";
	m_mesh = content->createInstanceOfResource<zpStaticMeshResource>( m_meshAlias );
}
void zpStaticMeshRenderingComponent::onDestroy() {}

void zpStaticMeshRenderingComponent::onUpdate() {
	zpVector4f pos;
	getParentGameObject()->getWorldTransform().getPosition( pos );

	m_boundingSphere.setCenter( pos );
}

void zpStaticMeshRenderingComponent::onEnabled() {}
void zpStaticMeshRenderingComponent::onDisabled() {}

void zpStaticMeshRenderingComponent::onShow() {}
void zpStaticMeshRenderingComponent::onHide() {}