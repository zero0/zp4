#include "zpRendering.h"

#if ZP_RENDERING_TYPE == ZP_DX11
#include "RenderingDX/zpDX11.h"
#elif ZP_RENDERING_TYPE == ZP_GL2
#include "RenderingOpenGL/zpOpenGL.h"
#else
#error( "No rendering engine selected!" )
#endif

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
void zpRenderingEngine::create()
{
	zpRenderingContextImpl* immediateContext;
	zpTextureImpl* immediateRenderTarget;
	m_renderingEngine->create( m_window, m_displayMode, m_screenMode, m_renderingEngineType, immediateContext, immediateRenderTarget );

	m_immediateRenderTarget = new zpTexture( immediateRenderTarget );
	m_renderingContexts.pushBack( new zpRenderingContext( this, immediateContext ) );
}
void zpRenderingEngine::destroy()
{
	m_renderingContexts.clear();
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
	m_renderingEngine->present();
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

zpBuffer* zpRenderingEngine::createBuffer( zpBufferType type, zpBufferBindType bind, zp_uint size, zp_uint stride, void* data )
{
	zpBufferImpl* buffer;
	buffer = m_renderingEngine->createBuffer( type, bind, size, stride, data );
	return new zpBuffer( buffer );
}

zpTexture* zpRenderingEngine::createTexture(  zp_uint width, zp_uint height, zpTextureType type, zpTextureDimension dimension, zpDisplayFormat format, zpCpuAccess access, void* data, zp_uint mipLevels )
{
	zpTextureImpl* texture;
	texture = m_renderingEngine->createTexture( width, height, type, dimension, format, access, data, mipLevels );
	return new zpTexture( texture );
}

zpRasterState* zpRenderingEngine::createRasterState( const zpRasterStateDesc& desc )
{
	zp_hash descHash = zp_fnv1_32_data( &desc, sizeof( zpRasterStateDesc ), 0 );

	zpRasterState* raster = ZP_NULL;
	for( zp_uint i = 0; i < m_rasterStates.size(); ++i )
	{
		zpRasterState* r = m_rasterStates[ i ];
		if( r->m_descHash == descHash )
		{
			raster = r;
			break;
		}
	}

	if( raster == ZP_NULL )
	{
		raster = new zpRasterState( m_renderingEngine->createRasterState( desc ), descHash );
		m_rasterStates.pushBack( raster );
	}

	return raster;
}
zpSamplerState* zpRenderingEngine::createSamplerState( const zpSamplerStateDesc& desc )
{
	zp_hash descHash = zp_fnv1_32_data( &desc, sizeof( zpSamplerStateDesc ), 0 );

	zpSamplerState* sampler = ZP_NULL;
	for( zp_uint i = 0; i < m_samplerStates.size(); ++i )
	{
		zpSamplerState* s = m_samplerStates[ i ];
		if( s->m_descHash == descHash )
		{
			sampler = s;
			break;
		}
	}

	if( sampler == ZP_NULL )
	{
		sampler = new zpSamplerState( m_renderingEngine->createSamplerState( desc ), descHash );
		m_samplerStates.pushBack( sampler );
	}

	return sampler;
}