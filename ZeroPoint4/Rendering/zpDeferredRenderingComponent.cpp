#include "zpRendering.h"

enum zpScreenQuad {
	ZP_SCREEN_QUAD_FULLSCREEN =			0,
	ZP_SCREEN_QUAD_FULLSCREEN_FRONT =	4,

	ZP_SCREEN_QUAD_LOWER_LEFT =			8,
	ZP_SCREEN_QUAD_LOWER_RIGHT =		12,
	ZP_SCREEN_QUAD_UPPER_LEFT =			16,
	ZP_SCREEN_QUAD_UPPER_RIGHT =		20,
};

zpDeferredRenderingComponent::zpDeferredRenderingComponent() 
	: m_renderingManager( ZP_NULL )
	, m_renderingEngine( ZP_NULL )
	, m_contentManager( ZP_NULL )
	, m_gbuffer()
	, m_screenBuffer( ZP_NULL )
{}
zpDeferredRenderingComponent::~zpDeferredRenderingComponent() {}

void zpDeferredRenderingComponent::render() {
	zpRenderingContext* c = m_renderingEngine->getImmediateRenderingContext();

	c->setSamplerState( ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, 0, m_pointSampler );
	c->setSamplerState( ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, 1, m_linearSampler );

	c->setViewport( m_viewport );

	// deferred render
	if( 0 )
	{
		m_gbuffer.foreachIndexed( [ c ]( zp_uint index, zpTexture* buffer ) {
			c->setRenderTarget( buffer, index );
		} );
		c->setDepthStencilBuffer( m_renderingEngine->getBackBufferDepthStencilBuffer() );

		c->clearRenderTarget( m_clearColors.data(), m_clearColors.size() );
		c->clearDepthStencilBuffer( 1.f, 0 );

		// render layers
		for( zp_uint i = 0; i < ZP_RENDERING_LAYER_Count; ++i ) {
			m_renderingManager->renderLayer( i );
		}

		// render lights
		m_renderingManager->renderLights();
	}

	// combine and render screen
	{
		c->setRenderTarget( m_renderingEngine->getBackBufferRenderTarget() );
		c->setDepthStencilBuffer( m_renderingEngine->getBackBufferDepthStencilBuffer() );
		c->bindRenderTargetAndDepthBuffer();

		c->clearRenderTarget( zpColor4f( 1, 0, 0, 1 ) );
		c->clearDepthStencilBuffer( 1.f, 0 );
		if( m_texture ) c->setTexture( ZP_RESOURCE_BIND_SLOT_PIXEL_SHADER, 0, m_texture.getResource()->getTexture() );
		if( m_fullscreenShader ) {
			c->setShader( &m_fullscreenShader );
			c->setTopology( ZP_TOPOLOGY_TRIANGLE_STRIP );
			c->setBuffer( m_screenBuffer );

			c->draw( 4, ZP_SCREEN_QUAD_FULLSCREEN );
		}
	}
	
	m_renderingEngine->present();
}

void zpDeferredRenderingComponent::receiveMessage( const zpMessage& message ) {}

void zpDeferredRenderingComponent::serialize( zpSerializedOutput* out ) {
	out->writeBlock( ZP_SERIALIZE_TYPE_THIS );

	out->endBlock();
}
void zpDeferredRenderingComponent::deserialize( zpSerializedInput* in ) {
	in->readBlock( ZP_SERIALIZE_TYPE_THIS );

	in->endBlock();
}

void zpDeferredRenderingComponent::onCreate() {
	m_renderingManager = getGame()->getGameManagers()->getRenderingManager();
	m_contentManager = getGame()->getGameManagers()->getContentManager();
	m_renderingEngine = zpRenderingFactory::getRenderingEngine();
	
	const zpVector2i& size = this->getGame()->getWindow()->getScreenSize();
	zp_uint width = size.getX();
	zp_uint height = size.getY();

	m_viewport = zpViewport( (zp_float)width, (zp_float)height );

	zpDisplayFormat displayFormats[] = {
		ZP_DISPLAY_FORMAT_RGBA8_UNORM,
		ZP_DISPLAY_FORMAT_RGBA8_UNORM,
		ZP_DISPLAY_FORMAT_R32_FLOAT,
		ZP_DISPLAY_FORMAT_RGBA8_UNORM
	};

	m_gbuffer[ ZP_GBUFFER_TARGET_DIFFUSE ] = m_renderingEngine->createTexture( width, height, ZP_TEXTURE_TYPE_RENDER_TEXTURE, ZP_TEXTURE_DIMENSION_2D, displayFormats[ ZP_GBUFFER_TARGET_DIFFUSE ], ZP_CPU_ACCESS_NONE, ZP_NULL, 1 );
	m_gbuffer[ ZP_GBUFFER_TARGET_NORMAL ] = m_renderingEngine->createTexture( width, height, ZP_TEXTURE_TYPE_RENDER_TEXTURE, ZP_TEXTURE_DIMENSION_2D, displayFormats[ ZP_GBUFFER_TARGET_NORMAL ], ZP_CPU_ACCESS_NONE, ZP_NULL, 1 );
	m_gbuffer[ ZP_GBUFFER_TARGET_DEPTH ] = m_renderingEngine->createTexture( width, height, ZP_TEXTURE_TYPE_RENDER_TEXTURE, ZP_TEXTURE_DIMENSION_2D, displayFormats[ ZP_GBUFFER_TARGET_DEPTH ], ZP_CPU_ACCESS_NONE, ZP_NULL, 1 );
	m_gbuffer[ ZP_GBUFFER_TARGET_LIGHT ] = m_renderingEngine->createTexture( width, height, ZP_TEXTURE_TYPE_RENDER_TEXTURE, ZP_TEXTURE_DIMENSION_2D, displayFormats[ ZP_GBUFFER_TARGET_LIGHT ], ZP_CPU_ACCESS_NONE, ZP_NULL, 1 );

	m_clearColors[ ZP_GBUFFER_TARGET_DIFFUSE ].set( 0, 0, 0, 1 );
	m_clearColors[ ZP_GBUFFER_TARGET_NORMAL ].set( .5f, .5f, .5f, 1 );
	m_clearColors[ ZP_GBUFFER_TARGET_DEPTH ].set( 1, 1, 1, 1 );
	m_clearColors[ ZP_GBUFFER_TARGET_LIGHT ].set( 0, 0, 0, 1 );

	const zp_float f = 1.0f;
	const zp_float h = 0.5f;

	zpVertexPositionTexture screen[] = {
		// full screen
		{ zpVector4f( -f, -f, h, 1.f ), zpVector2f( 0, f ) },
		{ zpVector4f( -f,  f, h, 1.f ), zpVector2f( 0, 0 ) },
		{ zpVector4f(  f, -f, h, 1.f ), zpVector2f( f, f ) },
		{ zpVector4f(  f,  f, h, 1.f ), zpVector2f( f, 0 ) },

		// full screen front
		{ zpVector4f( -f, -f, 0, 1.f ), zpVector2f( 0, f ) },
		{ zpVector4f( -f,  f, 0, 1.f ), zpVector2f( 0, 0 ) },
		{ zpVector4f(  f, -f, 0, 1.f ), zpVector2f( f, f ) },
		{ zpVector4f(  f,  f, 0, 1.f ), zpVector2f( f, 0 ) },

		// lower left
		{ zpVector4f( -f, -f, h, 1.f ), zpVector2f( 0, f ) },
		{ zpVector4f( -f,  0, h, 1.f ), zpVector2f( 0, 0 ) },
		{ zpVector4f(  0, -f, h, 1.f ), zpVector2f( f, f ) },
		{ zpVector4f(  0,  0, h, 1.f ), zpVector2f( f, 0 ) },

		// lower right
		{ zpVector4f(  0, -f, h, 1.f ), zpVector2f( 0, f ) },
		{ zpVector4f(  0,  0, h, 1.f ), zpVector2f( 0, 0 ) },
		{ zpVector4f(  f, -f, h, 1.f ), zpVector2f( f, f ) },
		{ zpVector4f(  f,  0, h, 1.f ), zpVector2f( f, 0 ) },

		// upper left
		{ zpVector4f( -f,  0, h, 1.f ), zpVector2f( 0, f ) },
		{ zpVector4f( -f,  f, h, 1.f ), zpVector2f( 0, 0 ) },
		{ zpVector4f(  0,  0, h, 1.f ), zpVector2f( f, f ) },
		{ zpVector4f(  0,  f, h, 1.f ), zpVector2f( f, 0 ) },

		// upper right
		{ zpVector4f(  0,  0, h, 1.f ), zpVector2f( 0, f ) },
		{ zpVector4f(  0,  f, h, 1.f ), zpVector2f( 0, 0 ) },
		{ zpVector4f(  f,  0, h, 1.f ), zpVector2f( f, f ) },
		{ zpVector4f(  f,  f, h, 1.f ), zpVector2f( f, 0 ) }
	};
	
	m_screenBuffer = m_renderingEngine->createBuffer();
	m_screenBuffer->create( ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_IMMUTABLE, screen );

	zpSamplerStateDesc pointDesc;
	m_pointSampler = m_renderingEngine->createSamplerState( pointDesc );

	zpSamplerStateDesc linearDesc;
	linearDesc.minFilter = ZP_TEXTURE_FILTER_LINEAR;
	linearDesc.magFilter = ZP_TEXTURE_FILTER_LINEAR;
	linearDesc.mipFilter = ZP_TEXTURE_FILTER_LINEAR;
	m_linearSampler = m_renderingEngine->createSamplerState( linearDesc );

	getGame()->setRenderable( this );

	m_fullscreenShader = m_contentManager->createInstanceOfResource<zpShaderResource>( "fullscreenNoAlpha" );
	m_texture = m_contentManager->createInstanceOfResource<zpTextureResource>( "test_texture" );
}
void zpDeferredRenderingComponent::onDestroy() {
	getGame()->setRenderable( ZP_NULL );
}

void zpDeferredRenderingComponent::onUpdate() {}

void zpDeferredRenderingComponent::onEnabled() {}
void zpDeferredRenderingComponent::onDisabled() {}
