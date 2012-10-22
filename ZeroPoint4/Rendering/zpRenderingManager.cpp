#include "zpRendering.h"
#include "Awesomium\awesomium_capi.h"

zpRenderingManager::zpRenderingManager()
	: m_isFrustimCullingEnabled( true )
	, m_engine( ZP_NULL )
	, m_currentCamera( ZP_NULL )
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
		components.foreach( [ this ]( zpRenderingComponent* component ) {
			if( component->isVisible() ) {
				if( m_isFrustimCullingEnabled && m_currentCamera ) {
					if( ZP_IS_COLLISION( zpCollision::testCollision( m_currentCamera->getFrustum(), component->getBoundingSphere() ) ) ) {
						component->render();
					}
				} else {
					component->render();
				}
			}
		} );
	}
}

zpBuffer* zpRenderingManager::getGlobalBuffer( zp_uint index ) {
	return m_globalBuffers[ index ];
}

void zpRenderingManager::setFrustumCullingEnabled( zp_bool frustumCulling ) {
	m_isFrustimCullingEnabled = frustumCulling;
}
zp_bool zpRenderingManager::isFrustumCullingEnabled() const {
	return m_isFrustimCullingEnabled;
}

void zpRenderingManager::setCamera( zpCamera* camera ) {
	m_currentCamera = camera;
}
zpCamera* zpRenderingManager::getCamera() const {
	return m_currentCamera;
}

void zpRenderingManager::onCreate() {
	m_engine = zpRenderingFactory::getRenderingEngine();
	m_engine->setWindow( getGame()->getWindow() );
	m_engine->create();

	for( zp_uint i = ZP_RENDERING_GLOBAL_BUFFER_Count; i --> 0; ) m_globalBuffers[ i ] = m_engine->createBuffer();

	m_globalBuffers[ ZP_RENDERING_GLOBAL_BUFFER_WORLD ]->create( ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, 1, sizeof( zpWorldBufferData ) );
	m_globalBuffers[ ZP_RENDERING_GLOBAL_BUFFER_CAMERA ]->create( ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, 1, sizeof( zpCameraBufferData ) );
	m_globalBuffers[ ZP_RENDERING_GLOBAL_BUFFER_LIGHT ]->create( ZP_BUFFER_TYPE_CONSTANT, ZP_BUFFER_BIND_DEFAULT, 1, sizeof( zpLightBufferData ) );

	zpContentManager* content = this->getGame()->getGameManagerOfType<zpContentManager>();
	
	zpString root;
	content->getRootDirectoryForExtension( "html", root );

	awe_webcore_initialize_default();

	/*
	awe_webcore_initialize(
	false,
	true,
	false,
	awe_string_empty(),
	awe_string_empty(),
	awe_string_empty(),
	awe_string_empty(),
	awe_string_empty(),
	ll,
	false,
	awe_string_empty(),
	true,
	awe_string_empty(),
	awe_string_empty(),
	awe_string_empty(),
	awe_string_empty(),
	awe_string_empty(),
	awe_string_empty(),
	true,
	0,
	false,
	false,
	customCSS
	);
	*/
	
}
void zpRenderingManager::onDestroy() {
	m_engine->destroy();
	zpRenderingFactory::destroyRenderingEngine();
}

void zpRenderingManager::onUpdate() {
	if( m_currentCamera && m_currentCamera->isDirty() ) {
		m_globalBuffers[ ZP_RENDERING_GLOBAL_BUFFER_CAMERA ]->update( m_currentCamera->getCameraBufferData() );
		m_currentCamera->unmarkDirty();
	}

	awe_webcore_update();
}

void zpRenderingManager::onEnabled() {}
void zpRenderingManager::onDisabled() {}
