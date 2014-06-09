#include "zpRendering.h"
#include "zpRenderingImpl.inl"

zpRenderingEngine::zpRenderingEngine()
	: m_renderingEngine( new zpRenderingEngineImpl )
{}
zpRenderingEngine::~zpRenderingEngine()
{
	ZP_SAFE_DELETE( m_renderingEngine );
}

void zpRenderingEngine::initialize()
{
	m_renderingEngine->initialize();
}
void zpRenderingEngine::create( zp_handle hWindow, const zpVector2i& size )
{
	m_screenSize = size;

	zpRenderingContextImpl* immediateContext;
	zpTextureImpl* immediateRenderTarget;
	m_renderingEngine->create( hWindow, size.getX(), size.getY(), m_displayMode, m_screenMode, m_renderingEngineType, immediateContext, immediateRenderTarget );

	m_immediateContext.setup( this, immediateContext );

	m_immediateRenderTarget.m_textureImpl = immediateRenderTarget;

	createDepthBuffer( m_immediateDepthStencilBuffer, size.getX(), size.getY(), ZP_DISPLAY_FORMAT_D24S8_UNORM_UINT );

	//m_renderingContexts.pushBack( new zpRenderingContext( this, immediateContext ) );
}
void zpRenderingEngine::destroy()
{
	destroyTexture( m_immediateRenderTarget );
	
	destroyDepthBuffer( m_immediateDepthStencilBuffer );

	m_immediateContext.destroy();
	//m_renderingContexts.foreach( []( zpRenderingContext* cxt ) {
	//	delete cxt;
	//} );

	//m_renderingContexts.clear();
	m_rasterStates.clear();
	m_samplerStates.clear();

	m_renderingEngine->destroy();
}
void zpRenderingEngine::shutdown()
{
	m_renderingEngine->shutdown();
}

zpRenderingEngineType zpRenderingEngine::getEngineType() const
{
	return m_renderingEngineType;
}

void zpRenderingEngine::setDisplayMode( const zpDisplayMode& mode )
{
	m_displayMode = mode;
}
const zpDisplayMode& zpRenderingEngine::getDisplayMode() const
{
	return m_displayMode;
}

void zpRenderingEngine::setScreenMode( zpScreenMode mode )
{
	m_screenMode = mode;
}
zpScreenMode zpRenderingEngine::getScreenMode() const
{
	return m_screenMode;
}

void zpRenderingEngine::setVSyncEnabled( zp_bool enabled )
{
	m_isVSyncEnabled = enabled;
}
zp_bool zpRenderingEngine::isVSyncEnabled() const
{
	return m_isVSyncEnabled;
}

void zpRenderingEngine::present()
{
	m_renderingEngine->present( m_isVSyncEnabled );
}

zpRenderingContext* zpRenderingEngine::getImmediateRenderingContext()
{
	return &m_immediateContext; //m_renderingContexts[ 0 ];
}
zpTexture* zpRenderingEngine::getBackBufferRenderTarget()
{
	return &m_immediateRenderTarget;
}
zpDepthStencilBuffer* zpRenderingEngine::getBackBufferDepthStencilBuffer()
{
	return &m_immediateDepthStencilBuffer;
}

zp_uint zpRenderingEngine::getNumRenderingContexts() const
{
	return 1; //m_renderingContexts.size();
}
zpRenderingContext* zpRenderingEngine::getRenderingContext( zp_uint index ) const
{
	return ZP_NULL; //m_renderingContexts[ index ];
}
zpRenderingContext* zpRenderingEngine::createRenderingContext()
{
	//m_renderingContexts.pushBack( new zpRenderingContext( this ) );
	//return m_renderingContexts.back();
	return ZP_NULL;
}

void zpRenderingEngine::destroyRenderingContext( zpRenderingContext& context )
{
	
}

void zpRenderingEngine::createBuffer( zpBuffer& outBuffer, zpBufferType type, zpBufferBindType bind, zp_uint size, zp_uint stride, const void* data )
{
	outBuffer.m_buffer = m_renderingEngine->createBuffer( type, bind, size, stride, data );
}
zp_bool zpRenderingEngine::destroyBuffer( zpBuffer& buffer )
{
	return m_renderingEngine->destroyBuffer( buffer.m_buffer );
}

zp_bool zpRenderingEngine::createTexture( zpTexture& texture, zp_uint width, zp_uint height, zpTextureType type, zpTextureDimension dimension, zpDisplayFormat format, zpCpuAccess access, const void* data, zp_uint strideInBytes, zp_uint mipLevels )
{
	if( texture.m_textureImpl )
	{
		destroyTexture( texture );
	}
	texture.m_textureImpl = m_renderingEngine->createTexture( width, height, type, dimension, format, access, data, strideInBytes, mipLevels );

	return texture.m_textureImpl != ZP_NULL;
}
zp_bool zpRenderingEngine::createTextureFromFile( zpTexture& texture, const zpString& filename )
{
	if( texture.m_textureImpl )
	{
		destroyTexture( texture );
	}
	texture.m_textureImpl = m_renderingEngine->createTextureFromFile( filename );

	return texture.m_textureImpl != ZP_NULL;
}
zp_bool zpRenderingEngine::destroyTexture( zpTexture& texture )
{
	if( m_renderingEngine->destroyTexture( texture.m_textureImpl ) )
	{
		texture.m_textureImpl = ZP_NULL;
	}

	return texture.m_textureImpl == ZP_NULL;
}

void zpRenderingEngine::createDepthBuffer( zpDepthStencilBuffer& depthStencilBuffer, zp_uint width, zp_uint height, zpDisplayFormat format )
{
	depthStencilBuffer.m_impl = m_renderingEngine->createDepthStencilBuffer( width, height, format );
}
void zpRenderingEngine::destroyDepthBuffer( zpDepthStencilBuffer& depthStencilBuffer )
{
	m_renderingEngine->destroyDepthStencilBuffer( depthStencilBuffer.m_impl );
}

void zpRenderingEngine::createRasterState( zpRasterState& state, const zpRasterStateDesc& desc )
{
	state.m_rasterState = m_renderingEngine->createRasterState( desc );
}
void zpRenderingEngine::createSamplerState( zpSamplerState& state, const zpSamplerStateDesc& desc )
{
	state.m_samplerState = m_renderingEngine->createSamplerState( desc );
}
void zpRenderingEngine::createDepthStencilState( zpDepthStencilState& state, const zpDepthStencilStateDesc& desc )
{
	state.m_depthStencilState = m_renderingEngine->createDepthStencilState( desc );
}
void zpRenderingEngine::createBlendState( zpBlendState& state, const zpBlendStateDesc& desc )
{
	state.m_blendState = m_renderingEngine->createBlendState( desc );
}


zp_bool zpRenderingEngine::createShader( zpShader& shader )
{
	if( shader.m_shader == ZP_NULL )
	{
		shader.m_shader = m_renderingEngine->createShader();
	}
	return shader.m_shader != ZP_NULL;
}
zp_bool zpRenderingEngine::loadShader( zpShader& shader, const zpBison::Value& shaderfile )
{
	return m_renderingEngine->loadShader( shader.getShaderImpl(), shaderfile );
}
zp_bool zpRenderingEngine::destroyShader( zpShader& shader )
{
	if( m_renderingEngine->destroyShader( shader.getShaderImpl() ) )
	{
		shader.m_shader = ZP_NULL;
	}
	return shader.m_shader == ZP_NULL;
}
