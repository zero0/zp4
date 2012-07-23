#include "zpRendering.h"

zpRenderingManager::zpRenderingManager() :
	m_engine( ZP_NULL ),
	m_type( ZP_RENDERING_ENGINE_NONE )
{}
zpRenderingManager::~zpRenderingManager() {}

void zpRenderingManager::serialize( zpSerializedOutput* out ) {}
void zpRenderingManager::deserialize( zpSerializedInput* in ) {}

zpRenderingEngine* zpRenderingManager::getRenderingEngine() const {
	return m_engine;
}

void zpRenderingManager::setRenderingEngineType( zpRenderingEngineType type ) {
	m_type = type;
}
zpRenderingEngineType zpRenderingManager::getRenderingEngineType() const {
	return m_type;
}

void zpRenderingManager::onCreate() {
	m_engine = zpRenderingFactory::createRenderingEngine( m_type );
	m_engine->setWindow( getGame()->getWindow() );
	m_engine->create();
}
void zpRenderingManager::onDestroy() {
	m_engine->destroy();
	zpRenderingFactory::destroyRenderingEngine();
}

void zpRenderingManager::onUpdate() {
	//m_engine->clear();
	zpColor4f c(.23f, .15f, .88f, 1.f );
	m_engine->getImmediateRenderingContext()->clearRenderTarget( &c );

	m_engine->present();
}

void zpRenderingManager::onEnabled() {}
void zpRenderingManager::onDisabled() {}

void zpRenderingManager::receiveMessage( const zpMessage& message ) {}