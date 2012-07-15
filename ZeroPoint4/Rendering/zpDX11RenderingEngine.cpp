#include "zpRendering.h"
#include "zpDX11.h"
#include <D3D11.h>

#define HR( r )						if( FAILED( (r) ) ) { return false; }
#define HR_( r )					if( FAILED( (r) ) ) { return; }
#define HR_V( r, v )				if( FAILED( (r) ) ) { return (v); }

#define HR_MSG( r, msg, ... )		if( FAILED( (r) ) ) { zp_printfln( (msg), __VA_ARGS__ ); return false; }
#define HR_MSG_( r, msg, ... )		if( FAILED( (r) ) ) { zp_printfln( (msg), __VA_ARGS__ ); return; }
#define HR_MSG_V( r, v, msg, ... )	if( FAILED( (r) ) ) { zp_printfln( (msg), __VA_ARGS__ ); return (v); }

DXGI_FORMAT __zpDisplayFormatToDxgiFormat( zpRenderingDisplayFormat format ) {
	switch( format ) {
		case ZP_RENDERING_DISPLAY_FORMAT_RGBA8_UNORM:		return DXGI_FORMAT_R8G8B8A8_UNORM;
		case ZP_RENDERING_DISPLAY_FORMAT_RGBA8_SNORM:		return DXGI_FORMAT_R8G8B8A8_SNORM;

		case ZP_RENDERING_DISPLAY_FORMAT_R32_UINT:			return DXGI_FORMAT_R32_UINT;
		case ZP_RENDERING_DISPLAY_FORMAT_R32_SINT:			return DXGI_FORMAT_R32_SINT;
		case ZP_RENDERING_DISPLAY_FORMAT_R32_FLOAT:			return DXGI_FORMAT_R32_FLOAT;

		default:											return DXGI_FORMAT_UNKNOWN;
	}
}
zpRenderingDisplayFormat __dxgiFormatToZpDisplayFormat(DXGI_FORMAT format ) {
	switch( format ) {
		case DXGI_FORMAT_R8G8B8A8_UNORM:	return ZP_RENDERING_DISPLAY_FORMAT_RGBA8_UNORM;
		case DXGI_FORMAT_R8G8B8A8_SNORM:	return ZP_RENDERING_DISPLAY_FORMAT_RGBA8_SNORM;

		case DXGI_FORMAT_R32_UINT:			return ZP_RENDERING_DISPLAY_FORMAT_R32_UINT;
		case DXGI_FORMAT_R32_SINT:			return ZP_RENDERING_DISPLAY_FORMAT_R32_SINT;
		case DXGI_FORMAT_R32_FLOAT:			return ZP_RENDERING_DISPLAY_FORMAT_R32_FLOAT;

		default:							return ZP_RENDERING_DISPLAY_FORMAT_UNKNOWN;
	}
}

zpDX11RenderingEngine::zpDX11RenderingEngine() : 
	m_dxgiFactory( ZP_NULL ),
	m_dxgiAdapter( ZP_NULL ),
	m_swapChain( ZP_NULL ),
	m_d3dDevice( ZP_NULL ),
	m_immediateContext( ZP_NULL ),
	m_window( ZP_NULL ),
	m_screenMode( ZP_RENDERING_SCREEN_MODE_WINDOWED ),
	m_displayMode(),
	m_clearColor()
{}
zpDX11RenderingEngine::~zpDX11RenderingEngine() {
	destroy();
	shutdown();
}

zp_bool zpDX11RenderingEngine::create() {
	if( m_d3dDevice ) return true;

	HRESULT hr;
	zp_uint flags = 0;
#if ZP_DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL actualFeatureLevel;
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
	zp_uint featureLevelCount = sizeof( featureLevels ) / sizeof( featureLevels[0] );

	hr = D3D11CreateDevice( m_dxgiAdapter, D3D_DRIVER_TYPE_UNKNOWN, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &m_d3dDevice, &actualFeatureLevel, &m_immediateContext );
	if( FAILED( hr ) ) {
		hr = D3D11CreateDevice( ZP_NULL, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &m_d3dDevice, &actualFeatureLevel, &m_immediateContext );
		if( FAILED( hr ) ) {
			hr = D3D11CreateDevice( ZP_NULL, D3D_DRIVER_TYPE_WARP, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &m_d3dDevice, &actualFeatureLevel, &m_immediateContext );
			if( FAILED( hr ) ) {
				hr = D3D11CreateDevice( ZP_NULL, D3D_DRIVER_TYPE_REFERENCE, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &m_d3dDevice, &actualFeatureLevel, &m_immediateContext );
				HR_MSG( hr, "Unable to create DirectX device." );
			}
		}
	}

	// get the feature level of the rendering engine
	switch( m_d3dDevice->GetFeatureLevel() ) {
		case D3D_FEATURE_LEVEL_11_0: m_engineType = ZP_RENDERING_ENGINE_DX11; break;
		case D3D_FEATURE_LEVEL_10_1: m_engineType = ZP_RENDERING_ENGINE_DX10_1; break;
		case D3D_FEATURE_LEVEL_10_0: m_engineType = ZP_RENDERING_ENGINE_DX10; break;
	}

	// if neither of the display mode's dimensions are set, set to the window screen size
	if( !m_displayMode.width || !m_displayMode.height ) {
		m_displayMode.width = m_window->getScreenSize().getX();
		m_displayMode.height = m_window->getScreenSize().getY();
	}
	// if the display mode is set to unknow, default to RGBA unorm
	if( m_displayMode.displayFormat == ZP_RENDERING_DISPLAY_FORMAT_UNKNOWN ) {
		m_displayMode.displayFormat = ZP_RENDERING_DISPLAY_FORMAT_RGBA8_UNORM;
	}
	// if the refresh rate is not set, find the closest display mode that matches
	if( !m_displayMode.refreshRate ) {
		findClosestDisplayMode( m_displayMode, &m_displayMode );
	}

	DXGI_FORMAT format = __zpDisplayFormatToDxgiFormat( m_displayMode.displayFormat );

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	zp_zero_memory( &swapChainDesc );
	swapChainDesc.BufferDesc.Width = m_displayMode.width;
	swapChainDesc.BufferDesc.Height = m_displayMode.height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = m_displayMode.refreshRate;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = format;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = (HWND)m_window->getWindowHandle();
	swapChainDesc.Windowed = m_screenMode == ZP_RENDERING_SCREEN_MODE_WINDOWED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;//DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	hr = m_dxgiFactory->CreateSwapChain( m_d3dDevice, &swapChainDesc, &m_swapChain );
	HR_MSG( hr, "Unable to create swap chain." );

	m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&m_backBuffer );
	m_d3dDevice->CreateRenderTargetView( m_backBuffer, ZP_NULL, &m_backBufferView );
	
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	zp_zero_memory( &depthStencilDesc );
	depthStencilDesc.Width = m_displayMode.width;
	depthStencilDesc.Height = m_displayMode.height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	memset( &depthStencilViewDesc, 0, sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC ) );
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	m_d3dDevice->CreateTexture2D( &depthStencilDesc, ZP_NULL, &m_depthBuffer );
	m_d3dDevice->CreateDepthStencilView( m_depthBuffer, ZP_NULL, &m_depthBufferView );
	
	return true;
}
void zpDX11RenderingEngine::destroy() {
	ZP_SAFE_RELEASE( m_depthBuffer );
	ZP_SAFE_RELEASE( m_depthBufferView );

	ZP_SAFE_RELEASE( m_backBuffer );
	ZP_SAFE_RELEASE( m_backBufferView );

	ZP_SAFE_RELEASE( m_immediateContext );
	ZP_SAFE_RELEASE( m_swapChain );
	ZP_SAFE_RELEASE( m_d3dDevice );
}

zpRenderingEngineType zpDX11RenderingEngine::getEngineType() const {
	return m_engineType;
}

zp_uint zpDX11RenderingEngine::enumerateDisplayModes( zpRenderingDisplayFormat displayFormat, zpArrayList<zpRenderingDisplayMode>* outDisplayModes ) {
	if( !m_dxgiAdapter ) return 0;

	ZP_ASSERT_RETURN_( outDisplayModes, 0, "Out parameter not set." );

	IDXGIOutput* adapterOutput;
	HRESULT hr;

	hr = m_dxgiAdapter->EnumOutputs( 0, &adapterOutput );
	HR_MSG_V( hr, 0, "Unable to get display modes." );

	zp_uint numModes;
	DXGI_FORMAT format = __zpDisplayFormatToDxgiFormat( displayFormat );
	hr = adapterOutput->GetDisplayModeList( format, DXGI_ENUM_MODES_INTERLACED, &numModes, ZP_NULL );
	HR_MSG_V( hr, 0, "Unable to list display modes." );

	zpArray<DXGI_MODE_DESC> modes( numModes );
	hr = adapterOutput->GetDisplayModeList( format, DXGI_ENUM_MODES_INTERLACED, &numModes, modes.data() );
	HR_MSG_V( hr, 0, "Unable to list display modes." );

	zp_uint count = 0;
	outDisplayModes->clear();
	modes.foreach( [ &outDisplayModes, &count ]( const DXGI_MODE_DESC& mode ) {
		if( mode.RefreshRate.Denominator == 1 ) {
			zpRenderingDisplayMode m = { mode.Width, mode.Height, mode.RefreshRate.Numerator, __dxgiFormatToZpDisplayFormat( mode.Format ) };
			outDisplayModes->pushBack( m );
			++count;
		}
	});

	ZP_SAFE_RELEASE( adapterOutput );

	return count;
}
zp_bool zpDX11RenderingEngine::findClosestDisplayMode( const zpRenderingDisplayMode& displayMode, zpRenderingDisplayMode* outDisplayMode ) {
	if( !m_dxgiAdapter ) return false;

	ZP_ASSERT_RETURN_( outDisplayMode, false, "Out parameter not set." );

	DXGI_FORMAT format = __zpDisplayFormatToDxgiFormat( displayMode.displayFormat );
	ZP_ASSERT_RETURN_( format != DXGI_FORMAT_UNKNOWN, false, "Display mode format must be set." );

	IDXGIOutput* adapterOutput;
	HRESULT hr;

	hr = m_dxgiAdapter->EnumOutputs( 0, &adapterOutput );
	HR_MSG( hr, "Unable to get display modes." );

	DXGI_MODE_DESC inMode, outMode;
	ZeroMemory( &inMode, sizeof( DXGI_MODE_DESC ) );

	zp_bool widthHeightSet = displayMode.width && displayMode.height;

	inMode.Width = widthHeightSet ? displayMode.width : 0;
	inMode.Height = widthHeightSet ? displayMode.height : 0;
	inMode.Format = format;
	inMode.RefreshRate.Numerator = displayMode.refreshRate;
	inMode.RefreshRate.Denominator = displayMode.refreshRate ? 1 : 0;

	hr = adapterOutput->FindClosestMatchingMode( &inMode, &outMode, ZP_NULL );
	HR_MSG( hr, "Unable to find display mode." );

	ZP_SAFE_RELEASE( adapterOutput );
	
	if( outDisplayMode ) {
		zpRenderingDisplayMode mode = { outMode.Width, outMode.Height, outMode.RefreshRate.Numerator, __dxgiFormatToZpDisplayFormat( outMode.Format ) };
		*outDisplayMode = mode;
	}

	return true;
}

void zpDX11RenderingEngine::setDisplayMode( const zpRenderingDisplayMode& mode ) {
	m_displayMode = mode;
}
const zpRenderingDisplayMode& zpDX11RenderingEngine::getDisplayMode() const {
	return m_displayMode;
}

void zpDX11RenderingEngine::setScreenMode( zpRenderingScreenMode mode ) {
	m_screenMode = mode;
}
zpRenderingScreenMode zpDX11RenderingEngine::getScreenMode() const {
	return m_screenMode;
}

void zpDX11RenderingEngine::setWindow( zpWindow* window ) {
	m_window = window;
}
zpWindow* zpDX11RenderingEngine::getWindow() const {
	return m_window;
}

void zpDX11RenderingEngine::setClearColor( const zpColor4f& color, zp_uint renderTargetIndex ) {
	m_clearColor = color;
}
const zpColor4f& zpDX11RenderingEngine::getClearColor( zp_uint renderTargetIndex ) const {
	return m_clearColor;
}

void zpDX11RenderingEngine::clear() {
	m_clearColor.clamp();
	m_immediateContext->ClearRenderTargetView( m_backBufferView, m_clearColor );
	m_immediateContext->OMSetRenderTargets( 1, &m_backBufferView, m_depthBufferView );
}
void zpDX11RenderingEngine::present() {
	m_swapChain->Present( 0, 0 );
}


zp_bool zpDX11RenderingEngine::initialize() {
	if( m_dxgiAdapter ) return true;

	HRESULT hr;

	hr = CreateDXGIFactory( __uuidof(IDXGIFactory), (void**)&m_dxgiFactory );
	HR_MSG( hr, "Unable to Create DXGI Factory" );

	hr = m_dxgiFactory->EnumAdapters( 0, &m_dxgiAdapter );
	HR_MSG( hr, "Unable to Get Adapter 0" );
	
	zp_zero_memory( &m_displayMode );
	
	return true;
}
void zpDX11RenderingEngine::shutdown() {
	ZP_SAFE_RELEASE( m_dxgiAdapter );
	ZP_SAFE_RELEASE( m_dxgiFactory );
}

zpRenderingContext* zpDX11RenderingEngine::createRenderingContext() {
	ID3D11DeviceContext* context;
	m_d3dDevice->CreateDeferredContext( 0, &context );
	return new zpDX11RenderingContext( context );
}
void zpDX11RenderingEngine::setCurrentRenderingContext( zpRenderingContext* context ) {
}
zpRenderingContext* zpDX11RenderingEngine::getCurrentRenderingContext() const {
	return ZP_NULL;
}

zpBuffer* zpDX11RenderingEngine::createBuffer() {
	return new zpDX11Buffer();
}
zpTextureResource* zpDX11RenderingEngine::createTextureResource() {
	return new zpDX11TextureResource();
}

ID3D11Device* zpDX11RenderingEngine::getDevice() const {
	return m_d3dDevice;
}