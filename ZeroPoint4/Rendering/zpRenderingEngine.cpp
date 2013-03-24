#include "zpRendering.h"

#if ZP_RENDERING_TYPE == ZP_DX11
#include "RenderingDX/zpDX11.h"
#elif ZP_RENDERING_TYPE == ZP_GL2
#include "RenderingOpenGL/zpOpenGL.h"
#else
#error( "No rendering engine selected!" )
#endif

zpRenderingEngine::zpRenderingEngine()
	: m_rendingEngine( new zpRenderingEngineImpl )
{}
zpRenderingEngine::~zpRenderingEngine()
{
	ZP_SAFE_DELETE( m_rendingEngine );
}

void zpRenderingEngine::initialize()
{
	m_rendingEngine->initialize();
}
void zpRenderingEngine::create()
{
	zpRenderingContextImpl* immediateContext;
	zpTextureImpl* immediateRenderTarget;
	m_rendingEngine->create( m_window, m_displayMode, m_screenMode, m_renderingEngineType, immediateContext, immediateRenderTarget );

	m_immediateRenderTarget = new zpTexture( immediateRenderTarget );
	m_renderingContexts.pushBack( new zpRenderingContext( this, immediateContext ) );
}
void zpRenderingEngine::destroy()
{
	m_rendingEngine->destroy();
}
void zpRenderingEngine::shutdown()
{
	m_rendingEngine->shutdown();
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

void zpRenderingEngine::setWindow( zpWindow* window )
{
	m_window = window;
}
zpWindow* zpRenderingEngine::getWindow() const
{
	return m_window;
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
	m_rendingEngine->present();
}

zpRenderingContext* zpRenderingEngine::getImmediateRenderingContext() const
{
	return m_renderingContexts[ 0 ];
}
zpTexture* zpRenderingEngine::getBackBufferRenderTarget() const
{
	return m_immediateRenderTarget;
}

zp_uint zpRenderingEngine::getNumRenderingContexts() const
{
	return m_renderingContexts.size();
}
zpRenderingContext* zpRenderingEngine::getRenderingContext( zp_uint index ) const
{
	return m_renderingContexts[ index ];
}
zpRenderingContext* zpRenderingEngine::createRenderingContext()
{
	//m_renderingContexts.pushBack( new zpRenderingContext( this ) );
	//return m_renderingContexts.back();
	return ZP_NULL;
}

zpTexture* zpRenderingEngine::createTexture( zp_uint width, zp_uint height, zpTextureType type, zpTextureDimension dimension, zpDisplayFormat format, zpCpuAccess access, void* data, zp_uint mipLevels )
{
	zpTextureImpl* texture;

	return new zpTexture( texture );
}