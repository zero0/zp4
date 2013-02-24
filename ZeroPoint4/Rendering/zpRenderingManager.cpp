#include "zpRendering.h"

zpRenderingManager::zpRenderingManager()
	: m_isFrustimCullingEnabled( true )
	, m_engine( ZP_NULL )
	, m_currentCamera( ZP_NULL )
	, m_renderingComponents()
{}
zpRenderingManager::~zpRenderingManager() {}

void zpRenderingManager::serialize( zpSerializedOutput* out ) {
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	out->endBlock();
}
void zpRenderingManager::deserialize( zpSerializedInput* in ) {
	if( in->readBlock( ZP_SERIALIZE_TYPE_THIS ) )
	{
		in->endBlock();
	}
}

zpRenderingEngine* zpRenderingManager::getRenderingEngine() const {
	return m_engine;
}

void zpRenderingManager::receiveMessage( const zpMessage& message ) {}

zp_bool zpRenderingManager::addRenderingComponent( zpRenderingComponent* component ) {
	if( !component ) return false;

	const zpRenderLayer& layers = component->getRenderLayers();
	if( layers.isZero() ) return false;

	for( zp_uint i = ZP_RENDERING_LAYER_Count; i --> 0; ) {
		if( layers.isMarked( i ) ) {
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
	for( zp_uint i = ZP_RENDERING_LAYER_Count; i --> 0; ) {
		if( layers.isMarked( i ) ) {
			zpRenderingComponent* found = ZP_NULL;
			zp_uint count = m_renderingComponents[ i ].eraseAll( component );
			numRemoved += count;
		}
	}

	return numRemoved > 0;
}

zp_bool zpRenderingManager::addLightComponent( zpLightComponent* light ) {
	if( !light ) return false;

	m_lightComponents.pushBack( light );
	return true;
}
zp_bool zpRenderingManager::removeLightComponent( zpLightComponent* light ) {
	if( !light ) return false;

	zp_uint numRemoved = m_lightComponents.eraseAll( light );

	return numRemoved > 0;
}

void zpRenderingManager::renderLayer( zp_uint layer ) {
	zpArrayList<zpRenderingComponent*>& components = m_renderingComponents[ layer ];
	if( !components.isEmpty() ) {
		components.foreach( [ this ]( zpRenderingComponent* component ) {
			zp_bool shouldRender = false;
			if( component->isVisible() ) {
				if( m_isFrustimCullingEnabled && m_currentCamera ) {
					if( ZP_IS_COLLISION( zpCollision::testCollision( m_currentCamera->getFrustum(), component->getBoundingSphere() ) ) ) {
						shouldRender = true;
					}
				} else {
					shouldRender = true;
				}
			}

			if( shouldRender ) component->render();
		} );
	}
}
void zpRenderingManager::renderLights() {
	zpBuffer* lightBuffer = getGlobalBuffer( ZP_RENDERING_GLOBAL_BUFFER_LIGHT );

	m_lightComponents.foreach( [ this, lightBuffer ]( zpLightComponent* light ) {
		zp_bool shouldRender = false;
		if( light->isEnabled() ) {
			if( m_isFrustimCullingEnabled && m_currentCamera ) {
				switch( light->getLightType() ) {
				case ZP_LIGHT_TYPE_DIRECTIONAL:
					shouldRender = true;
					break;
				case ZP_LIGHT_TYPE_POINT:
				case ZP_LIGHT_TYPE_SPOT:
					{
						zpBoundingSphere lightSphere( light->getPosition(), zpScalar( light->getPointRadius() ) );
						if( zpCollision::testCollision( m_currentCamera->getFrustum(), lightSphere ) ) {
							shouldRender = true;
						}
					}
					break;
				}
			} else {
				shouldRender = true;
			}
		}

		if( shouldRender ) {
			lightBuffer->update( light->getLightBufferData() );
		}
	} );
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
}

void zpRenderingManager::onEnabled() {}
void zpRenderingManager::onDisabled() {}
