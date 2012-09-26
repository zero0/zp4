#include "zpRendering.h"

zpRenderingManager::zpRenderingManager()
	: m_engine( ZP_NULL )
	, m_currentCamera( ZP_NULL )
	, m_cameraStack()
	, m_renderingComponents()
{}
zpRenderingManager::~zpRenderingManager() {}

void zpRenderingManager::serialize( zpSerializedOutput* out ) {}
void zpRenderingManager::deserialize( zpSerializedInput* in ) {}

zpRenderingEngine* zpRenderingManager::getRenderingEngine() const {
	return m_engine;
}

void zpRenderingManager::receiveMessage( const zpMessage& message ) {}

void zpRenderingManager::render() {}

zp_bool zpRenderingManager::addRenderingComponent( zpRenderingComponent* component ) {
	if( !component ) return false;

	const zpRenderLayer& layers = component->getRenderLayers();
	if( layers.isZero() ) return false;

	for( zp_uint i = ZP_RENDERING_LAYER_COUNT; i --> 0; ) {
		if( layers.isMarked( i ) ) {
			component->addReference();
			m_renderingComponents[ i ].pushBack( component );
		}
	}

	return true;
}
zp_bool zpRenderingManager::removeRenderingComponent( zpRenderingComponent* component ) {
	if( !component ) return false;

	const zpRenderLayer& layers = component->getRenderLayers();
	if( layers.isZero() ) return false;

	zp_uint numRemoved = 0;
	for( zp_uint i = ZP_RENDERING_LAYER_COUNT; i --> 0; ) {
		if( layers.isMarked( i ) ) {
			zpRenderingComponent* found = ZP_NULL;
			zp_uint count = m_renderingComponents[ i ].removeAll( component );
			numRemoved += count;

			for( ; count --> 0; ) {
				component->removeReference();
			}
		}
	}

	return numRemoved > 0;
}

void zpRenderingManager::renderLayer( zp_uint layer ) {
	zpArrayList<zpRenderingComponent*>& components = m_renderingComponents[ layer ];
	if( !components.isEmpty() ) {
		components.foreach( []( zpRenderingComponent* component ) {
			if( component->isVisible() ) component->render();
		} );
	}
}

void zpRenderingManager::onCreate() {
	m_engine = zpRenderingFactory::getRenderingEngine();
	m_engine->setWindow( getGame()->getWindow() );
	m_engine->create();
}
void zpRenderingManager::onDestroy() {
	m_engine->destroy();
	zpRenderingFactory::destroyRenderingEngine();
}

void zpRenderingManager::onUpdate() {}

void zpRenderingManager::onEnabled() {}
void zpRenderingManager::onDisabled() {}
