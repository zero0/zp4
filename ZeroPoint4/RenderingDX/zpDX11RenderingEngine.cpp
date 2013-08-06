#include "zpDX11.h"
#include <D3D11.h>
#include <D3DX11.h>
#include "zpDX11Util.h"
#include "Core/zpCore.h"

zpRenderingEngineImpl::zpRenderingEngineImpl()
	: m_dxgiFactory( ZP_NULL )
	, m_dxgiAdapter( ZP_NULL )
	, m_swapChain( ZP_NULL )
	, m_d3dDevice( ZP_NULL )
{}
zpRenderingEngineImpl::~zpRenderingEngineImpl()
{}

void zpRenderingEngineImpl::initialize()
{
	HRESULT hr;

	hr = CreateDXGIFactory( __uuidof(IDXGIFactory), (void**)&m_dxgiFactory );
	ZP_ASSERT( SUCCEEDED( hr ), "Unable to Create DXGI Factory" );

	hr = m_dxgiFactory->EnumAdapters( 0, &m_dxgiAdapter );
	ZP_ASSERT( SUCCEEDED( hr ), "Unable to Get Adapter 0" );
}
void zpRenderingEngineImpl::create( zpWindow* window, zpDisplayMode& displayMode, zpScreenMode screenMode, zpRenderingEngineType& outEngineType, zpRenderingContextImpl*& outImmediateContext, zpTextureImpl*& outImmediateRenderTarget )
{
	HRESULT hr;
	zp_uint flags = 0;
#if ZP_DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// if neither of the display mode's dimensions are set, set to the window screen size
	if( !displayMode.width || !displayMode.height )
	{
		displayMode.width = window->getScreenSize().getX();
		displayMode.height = window->getScreenSize().getY();
	}
	// if the display mode is set to unknown, default to RGBA8_UNORM
	if( displayMode.displayFormat == ZP_DISPLAY_FORMAT_UNKNOWN )
	{
		displayMode.displayFormat = ZP_DISPLAY_FORMAT_RGBA8_UNORM;
	}
	// if the refresh rate is not set, find the closest display mode that matches
	if( !displayMode.refreshRate )
	{
		//findClosestDisplayMode( displayMode, &displayMode );
		displayMode.refreshRate = 60;
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	zp_zero_memory( &swapChainDesc );
	swapChainDesc.BufferDesc.Width = displayMode.width;
	swapChainDesc.BufferDesc.Height = displayMode.height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = displayMode.refreshRate;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = __zpToDX( displayMode.displayFormat );
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = (HWND)window->getWindowHandle();
	swapChainDesc.Windowed = screenMode == ZP_SCREEN_MODE_WINDOWED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;//DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL actualFeatureLevel;
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	zp_uint featureLevelCount = ZP_ARRAY_LENGTH( featureLevels );
	ID3D11DeviceContext* immediate = ZP_NULL;

	//hr = D3D11CreateDevice( m_dxgiAdapter, D3D_DRIVER_TYPE_UNKNOWN, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &m_d3dDevice, &actualFeatureLevel, &immediate );
	hr = D3D11CreateDeviceAndSwapChain( ZP_NULL, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_d3dDevice, &actualFeatureLevel, &immediate );
	if( FAILED( hr ) )
	{
		//hr = D3D11CreateDevice( ZP_NULL, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &m_d3dDevice, &actualFeatureLevel, &immediate );
		hr = D3D11CreateDeviceAndSwapChain( m_dxgiAdapter, D3D_DRIVER_TYPE_UNKNOWN, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_d3dDevice, &actualFeatureLevel, &immediate );
		if( FAILED( hr ) )
		{
			//hr = D3D11CreateDevice( ZP_NULL, D3D_DRIVER_TYPE_WARP, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &m_d3dDevice, &actualFeatureLevel, &immediate );
			hr = D3D11CreateDeviceAndSwapChain( ZP_NULL, D3D_DRIVER_TYPE_WARP, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_d3dDevice, &actualFeatureLevel, &immediate );
			if( FAILED( hr ) )
			{
				//hr = D3D11CreateDevice( ZP_NULL, D3D_DRIVER_TYPE_REFERENCE, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &m_d3dDevice, &actualFeatureLevel, &immediate );
				hr = D3D11CreateDeviceAndSwapChain( ZP_NULL, D3D_DRIVER_TYPE_REFERENCE, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_d3dDevice, &actualFeatureLevel, &immediate );

				ZP_ASSERT( SUCCEEDED( hr ), "Unable to create DirectX device." );
			}
		}
	}

	// create the immediate context wrapper
	//m_immediateContext = new zpDX11RenderingContext( immediate, zpString( "immediate" ) );
	outImmediateContext = new zpRenderingContextImpl( immediate );

	// get the actual feature level of the rendering engine
	switch( actualFeatureLevel )
	{
		case D3D_FEATURE_LEVEL_11_0: outEngineType = ZP_RENDERING_ENGINE_DX11; break;
		case D3D_FEATURE_LEVEL_10_1: outEngineType = ZP_RENDERING_ENGINE_DX10_1; break;
		case D3D_FEATURE_LEVEL_10_0: outEngineType = ZP_RENDERING_ENGINE_DX10; break;
	}


	// get the back buffer
	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* backBufferView;

	m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer );
	m_d3dDevice->CreateRenderTargetView( backBuffer, ZP_NULL, &backBufferView );

	// remove reference to texture so render target now owns pointer
	backBuffer->Release();

	// create the render target and depth buffer for the immediate context
	outImmediateRenderTarget = new zpTextureImpl;
	outImmediateRenderTarget->m_width = displayMode.width;
	outImmediateRenderTarget->m_height = displayMode.height;
	outImmediateRenderTarget->m_dimension = ZP_TEXTURE_DIMENSION_2D;
	outImmediateRenderTarget->m_type = ZP_TEXTURE_TYPE_RENDER_TARGET;
	outImmediateRenderTarget->m_texture = backBuffer;
	outImmediateRenderTarget->m_textureResourceView = ZP_NULL;
	outImmediateRenderTarget->m_textureRenderTarget = backBufferView;
	//m_immediateRenderTarget = target;
	//
	//m_immediateDepthStencilBuffer = createDepthBuffer( ZP_DISPLAY_FORMAT_D24S8_UNORM_UINT, displayMode.width, displayMode.height );
	//
	//// set render target and depth buffer for the immediate context automatically
	//m_immediateContext->setRenderTarget( m_immediateRenderTarget );
	//m_immediateContext->setDepthStencilBuffer( m_immediateDepthStencilBuffer );
	while( m_inputLayouts.size() < zpVertexFormat_Count )
	{
		m_inputLayouts.pushBack( ZP_NULL );
	}
}
void zpRenderingEngineImpl::destroy()
{
	m_inputLayouts.foreach( []( ID3D11InputLayout* layout )
	{
		ZP_SAFE_RELEASE( layout );
	} );
	m_inputLayouts.clear();

	ZP_SAFE_RELEASE( m_swapChain );
	ZP_SAFE_RELEASE( m_d3dDevice );
}
void zpRenderingEngineImpl::shutdown()
{
	ZP_SAFE_RELEASE( m_dxgiAdapter );
	ZP_SAFE_RELEASE( m_dxgiFactory );
}

zpBufferImpl* zpRenderingEngineImpl::createBuffer( zpBufferType type, zpBufferBindType bind, zp_uint size, zp_uint stride, void* data )
{
	HRESULT hr;
	zpDisplayFormat format = ZP_DISPLAY_FORMAT_UNKNOWN;

	if( type == ZP_BUFFER_TYPE_INDEX )
	{
		switch( stride )
		{
		case 2: format = ZP_DISPLAY_FORMAT_R16_UINT; break;
		case 4: format = ZP_DISPLAY_FORMAT_R32_UINT; break;
		}
	}

	D3D11_BUFFER_DESC desc;
	zp_zero_memory( &desc );
	desc.Usage = __zpToDX( bind );
	desc.ByteWidth = size;
	desc.StructureByteStride = stride;
	desc.BindFlags = __zpToDX( type );
	desc.CPUAccessFlags = bind == ZP_BUFFER_BIND_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
	desc.MiscFlags = 0;

	ID3D11Buffer* buffer;
	if( data )
	{
		D3D11_SUBRESOURCE_DATA sub;
		zp_zero_memory( &sub );
		sub.pSysMem = data;

		hr = m_d3dDevice->CreateBuffer( &desc, &sub, &buffer );
	}
	else
	{
		hr = m_d3dDevice->CreateBuffer( &desc, ZP_NULL, &buffer );
	}

	ZP_ASSERT( SUCCEEDED( hr ), "Unable to create buffer." );

	zpBufferImpl* bufferImpl = new zpBufferImpl;
	bufferImpl->m_size = size;
	bufferImpl->m_stride = stride;
	bufferImpl->m_type = type;
	bufferImpl->m_format = format;
	bufferImpl->m_bind = bind;
	bufferImpl->m_buffer = buffer;

	return bufferImpl;
}

zpTextureImpl* zpRenderingEngineImpl::createTexture( zp_uint width, zp_uint height, zpTextureType type, zpTextureDimension dimension, zpDisplayFormat format, zpCpuAccess access, void* data, zp_uint mipLevels )
{
	HRESULT hr;
	ID3D11Resource* texture = ZP_NULL;
	ID3D11ShaderResourceView* srv = ZP_NULL;
	ID3D11RenderTargetView* rtv = ZP_NULL;

	D3D11_TEXTURE2D_DESC texDesc;
	zp_zero_memory( &texDesc );
	texDesc.ArraySize = 1;
	switch( type )
	{
	case ZP_TEXTURE_TYPE_TEXTURE:
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		break;
	case ZP_TEXTURE_TYPE_RENDER_TARGET:
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
		break;
	case ZP_TEXTURE_TYPE_RENDER_TEXTURE:
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		break;
	}

	texDesc.CPUAccessFlags = __zpToDX( ZP_CPU_ACCESS_NONE );
	texDesc.Format = __zpToDX( format );
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = type == ZP_TEXTURE_TYPE_TEXTURE ? mipLevels : 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	
	// create the texture with or without data
	if( data )
	{
		D3D11_SUBRESOURCE_DATA subData;
		zp_zero_memory( &subData );
		subData.pSysMem = data;

		hr = m_d3dDevice->CreateTexture2D( &texDesc, &subData, (ID3D11Texture2D**)&texture );
	}
	else
	{
		hr = m_d3dDevice->CreateTexture2D( &texDesc, ZP_NULL, (ID3D11Texture2D**)&texture );
	}

	if( FAILED( hr ) )
	{
		ZP_ASSERT( false, "Unable to create texture." );
		return ZP_NULL;
	}


	// create the shader resource view if the texture is a normal texture or a render texture
	if( type == ZP_TEXTURE_TYPE_TEXTURE || type == ZP_TEXTURE_TYPE_RENDER_TEXTURE )
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		zp_zero_memory( &srvDesc );
		srvDesc.Format = texDesc.Format;
		switch( type )
		{
		case ZP_TEXTURE_DIMENSION_1D:
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
			srvDesc.Texture1D.MipLevels = texDesc.MipLevels;
			break;
		case ZP_TEXTURE_DIMENSION_2D:
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
			break;
		case ZP_TEXTURE_DIMENSION_CUBE_MAP:
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MipLevels = texDesc.MipLevels;
			break;
		}

		hr = m_d3dDevice->CreateShaderResourceView( texture, &srvDesc, &srv );
		ZP_ASSERT_WARN( SUCCEEDED( hr ), "Unable to creat shader resouce view for texture" );
	}

	// create the render target view if the texture is a render target or a render texture
	if( type == ZP_TEXTURE_TYPE_RENDER_TARGET || type == ZP_TEXTURE_TYPE_RENDER_TEXTURE )
	{
		// TODO: assert if trying to create a 1D or cube map render target
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		zp_zero_memory( &rtvDesc );
		rtvDesc.Format = texDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		hr = m_d3dDevice->CreateRenderTargetView( texture, &rtvDesc, &rtv );
		ZP_ASSERT_WARN( SUCCEEDED( hr ), "Unable to create render target view for texture" );
	}

	// remove references so texture now owns pointers
	ZP_SAFE_RELEASE( texture );
	ZP_SAFE_RELEASE( srv );
	ZP_SAFE_RELEASE( rtv );

	zpTextureImpl* tex = new zpTextureImpl;
	tex->m_width = width;
	tex->m_height = height;
	tex->m_dimension = dimension;
	tex->m_type = type;
	tex->m_texture = texture;
	tex->m_textureResourceView = srv;
	tex->m_textureRenderTarget = rtv;

	return tex;
}
zpTextureImpl* zpRenderingEngineImpl::createTextureFromFile( const zpString& filename )
{
	HRESULT hr;
	ID3D11Resource* texture;
	ID3D11ShaderResourceView* srv;
	D3DX11_IMAGE_INFO info;

	hr = D3DX11CreateTextureFromFile( m_d3dDevice, filename.str(), ZP_NULL, ZP_NULL, &texture, ZP_NULL );
	ZP_ASSERT( SUCCEEDED( hr ), "" );

	hr = D3DX11CreateShaderResourceViewFromFile( m_d3dDevice, filename.str(), ZP_NULL, ZP_NULL, &srv, ZP_NULL );
	ZP_ASSERT( SUCCEEDED( hr ), "" );

	hr = D3DX11GetImageInfoFromFile( filename.str(), ZP_NULL, &info, ZP_NULL );
	ZP_ASSERT( SUCCEEDED( hr ), "" );

	zpTextureImpl* tex = new zpTextureImpl;
	tex->m_width = info.Width;
	tex->m_height = info.Height;
	tex->m_dimension = ZP_TEXTURE_DIMENSION_2D;
	tex->m_type = ZP_TEXTURE_TYPE_TEXTURE;
	tex->m_texture = texture;
	tex->m_textureResourceView = srv;
	tex->m_textureRenderTarget = ZP_NULL;

	return tex;
}
zp_bool zpRenderingEngineImpl::destroyTexture( zpTextureImpl* texture )
{
	if( texture )
	{
		ZP_SAFE_RELEASE( texture->m_texture );
		ZP_SAFE_RELEASE( texture->m_textureResourceView );
		ZP_SAFE_RELEASE( texture->m_textureRenderTarget );
		ZP_SAFE_DELETE( texture );
	}
	return texture == ZP_NULL;
}

zpDepthStencilBufferImpl* zpRenderingEngineImpl::createDepthStencilBuffer( zp_uint width, zp_uint height, zpDisplayFormat format )
{
	HRESULT hr;
	ID3D11Texture2D* texture;
	ID3D11DepthStencilView* view;

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	zp_zero_memory( &depthStencilDesc );
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = __zpToDX( format );
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	zp_zero_memory( &depthStencilViewDesc );
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_d3dDevice->CreateTexture2D( &depthStencilDesc, ZP_NULL, &texture );
	ZP_ASSERT( SUCCEEDED( hr ), "" );

	hr = m_d3dDevice->CreateDepthStencilView( texture, ZP_NULL, &view );
	ZP_ASSERT( SUCCEEDED( hr ), "" );

	// remove reference to texture so view now owns pointer
	texture->Release();

	return new zpDepthStencilBufferImpl( format, texture, view, width, height );
}
zpRasterStateImpl* zpRenderingEngineImpl::createRasterState( const zpRasterStateDesc& desc )
{
	HRESULT hr;
	D3D11_RASTERIZER_DESC rasterDesc;
	zp_zero_memory( &rasterDesc );
	rasterDesc.FillMode = __zpToDX( desc.fillMode );
	rasterDesc.CullMode = __zpToDX( desc.cullMode );
	rasterDesc.FrontCounterClockwise = desc.frontFace == ZP_FRONT_FACE_CCW;
	rasterDesc.DepthBias = desc.depthBias;
	rasterDesc.DepthBiasClamp = desc.depthBiasClamp;
	rasterDesc.SlopeScaledDepthBias = desc.slopeScaledDepthBias;
	rasterDesc.DepthClipEnable = desc.depthClipEnable;
	rasterDesc.ScissorEnable = desc.scissorEnable;
	rasterDesc.MultisampleEnable = desc.multisampleEnable;
	rasterDesc.AntialiasedLineEnable = desc.antialiasedLineEnable;

	ID3D11RasterizerState* state;
	hr = m_d3dDevice->CreateRasterizerState( &rasterDesc, &state );
	ZP_ASSERT_WARN( SUCCEEDED( hr ), "Unable to create Raster State" );

	zpRasterStateImpl* impl = new zpRasterStateImpl;
	impl->m_raster = state;
	impl->m_desc = desc;

	return impl;
}
zpSamplerStateImpl* zpRenderingEngineImpl::createSamplerState( const zpSamplerStateDesc& desc )
{
	HRESULT hr;
	D3D11_SAMPLER_DESC samplerDesc;
	zp_zero_memory( &samplerDesc );
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;	// TODO: update with real value
	samplerDesc.AddressU = __zpToDX( desc.texWrapU );
	samplerDesc.AddressV = __zpToDX( desc.texWrapV );
	samplerDesc.AddressW = __zpToDX( desc.texWrapW );
	samplerDesc.MipLODBias = desc.lodBias;
	samplerDesc.MaxAnisotropy = desc.maxAnisotrpy;
	samplerDesc.ComparisonFunc = __zpToDX( desc.cmpFunc );
	desc.borderColor.store4( samplerDesc.BorderColor );
	samplerDesc.MinLOD = desc.lodMin;
	samplerDesc.MaxLOD = desc.lodMax;

	ID3D11SamplerState* state;
	hr = m_d3dDevice->CreateSamplerState( &samplerDesc, &state );
	ZP_ASSERT_WARN( SUCCEEDED( hr ), "Unable to create Sampler State" );

	zpSamplerStateImpl* impl = new zpSamplerStateImpl;
	impl->m_sampler = state;
	impl->m_desc = desc;

	return impl;
}
zpShaderImpl* zpRenderingEngineImpl::createShader()
{
	zpShaderImpl* shader = new zpShaderImpl;

	//loadShader( shader, shaderfileName );

	return shader;
}
zp_bool zpRenderingEngineImpl::loadShader( zpShaderImpl* shader, const zpBison& shaderfile )
{
	const zpBison::Value& root = shaderfile.root();

	const zpBison::Value vs = root[ "VS" ];
	const zpBison::Value ps = root[ "PS" ];
	const zpBison::Value gs = root[ "GS" ];
	const zpBison::Value gsso = root[ "GSSO" ];
	const zpBison::Value cs = root[ "CS" ];
	
	zpDataBuffer data;
	HRESULT hr;

	const void* encodedShader;
	zp_uint encodedLength;

	if( !vs.isNull() )
	{
		const zpBison::Value d = vs[ "Data" ];
		encodedShader = d.asData();
		encodedLength = d.size();

		hr = m_d3dDevice->CreateVertexShader( encodedShader, encodedLength, ZP_NULL, &shader->m_vertexShader );
		ZP_ASSERT( SUCCEEDED( hr ), "Failed to create Vertex Shader %x", hr );

		const zpBison::Value format = vs[ "Format" ];
		const zp_char* formatStr = format.asCString();
		const zp_uint formatStrLen = format.size();

		zp_char formatDesc[ 4 ];
		zp_uint i = 0;
		for( ; i < formatStrLen; ++i )
		{
			formatDesc[ i ] = formatStr[ i ];
		}
		for( ; i < 4; ++i )
		{
			formatDesc[ i ] = '\0';
		}

		shader->m_vertexLayout = (zpVertexFormatDesc)ZP_MAKE_UINT( formatDesc[0], formatDesc[1], formatDesc[2], formatDesc[3] );

		createVertexLayout( shader->m_vertexLayout, encodedShader, encodedLength );
	}

	if( !ps.isNull() )
	{
		const zpBison::Value d = ps[ "Data" ];
		encodedShader = d.asData();
		encodedLength = d.size();

		hr = m_d3dDevice->CreatePixelShader( encodedShader, encodedLength, ZP_NULL, &shader->m_pixelShader );
		ZP_ASSERT( SUCCEEDED( hr ), "Failed to create Pixel Shader %x", hr );
	}

	if( !gs.isNull() )
	{
		const zpBison::Value d = gs[ "Data" ];
		encodedShader = d.asData();
		encodedLength = d.size();

		hr = m_d3dDevice->CreateGeometryShader( encodedShader, encodedLength, ZP_NULL, &shader->m_geometryShader );
		ZP_ASSERT( SUCCEEDED( hr ), "Failed to create Geometry Shader %x", hr );
	}

	if( !gsso.isNull() )
	{
		const zpBison::Value d = gsso[ "Data" ];
		encodedShader = d.asData();
		encodedLength = d.size();

		const zpBison::Value stridesValue = gsso[ "Strides" ];
		const zpBison::Value entriesValue = gsso[ "Entries" ];

		zpArrayList< zp_uint > strides;
		strides.reserve( stridesValue.size() );
		for( zp_uint i = 0; i < strides.size(); ++i )
		{
			strides.pushBackEmpty() = stridesValue[ i ].asInt();
		}

		zpArrayList< D3D11_SO_DECLARATION_ENTRY > entries;
		entries.reserve( entriesValue.size() );
		for( zp_uint i = 0; i < entries.size(); ++i )
		{
			const zpBison::Value entryValue = entriesValue[ i ];

			D3D11_SO_DECLARATION_ENTRY& entry = entries.pushBackEmpty();
			entry.Stream =			entryValue[ "Stream" ].asInt();
			entry.SemanticIndex =	entryValue[ "SemanticIndex" ].asInt();
			entry.StartComponent =	entryValue[ "StartComponent" ].asInt();
			entry.ComponentCount =	entryValue[ "ComponentCount" ].asInt();
			entry.OutputSlot =		entryValue[ "OutputSlot" ].asInt();
			entry.SemanticName =	entryValue[ "SemanticName" ].asCString();
		}

		hr = m_d3dDevice->CreateGeometryShaderWithStreamOutput( encodedShader, encodedLength, entries.begin(), entries.size(), strides.begin(), strides.size(), D3D11_SO_NO_RASTERIZED_STREAM, ZP_NULL, &shader->m_geometryShader );
		ZP_ASSERT( SUCCEEDED( hr ), "Failed to create Geometry Shader %x", hr );
	}

	if( !cs.isNull() )
	{
		const zpBison::Value d = cs[ "Data" ];
		encodedShader = d.asData();
		encodedLength = d.size();

		hr = m_d3dDevice->CreateComputeShader( encodedShader, encodedLength, ZP_NULL, &shader->m_computeShader );
		ZP_ASSERT( SUCCEEDED( hr ), "Failed to create Compute Shader %x", hr );
	}

	return false;
}
zp_bool zpRenderingEngineImpl::destroyShader( zpShaderImpl* shader )
{
	ZP_SAFE_RELEASE( shader->m_vertexShader );
	ZP_SAFE_RELEASE( shader->m_pixelShader );
	ZP_SAFE_RELEASE( shader->m_geometryShader );
	ZP_SAFE_RELEASE( shader->m_computeShader );

	return true;
}

void zpRenderingEngineImpl::present( zp_bool vsync )
{
	m_swapChain->Present( vsync ? 1 : 0, 0 );
}

void zpRenderingEngineImpl::createVertexLayout( zpVertexFormatDesc format, const void* data, zp_uint size )
{
	ID3D11InputLayout* inputLayout;
	HRESULT hr;

	switch( format )
	{
	case ZP_VERTEX_FORMAT_DESC_VERTEX_COLOR:
		{
			if( !m_inputLayouts[ ZP_VERTEX_FORMAT_VERTEX_COLOR ] )
			{
				D3D11_INPUT_ELEMENT_DESC desc[] =
				{
					{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};

				hr = m_d3dDevice->CreateInputLayout( desc, ZP_ARRAY_LENGTH( desc ), data, size, &inputLayout );
				ZP_ASSERT( SUCCEEDED( hr ), "" );

				m_inputLayouts[ ZP_VERTEX_FORMAT_VERTEX_COLOR ] = inputLayout;
			}
		}
		break;

	case ZP_VERTEX_FORMAT_DESC_VERTEX_UV:
		{
			if( !m_inputLayouts[ ZP_VERTEX_FORMAT_VERTEX_UV ] )
			{
				D3D11_INPUT_ELEMENT_DESC desc[] =
				{
					{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD0",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};

				hr = m_d3dDevice->CreateInputLayout( desc, ZP_ARRAY_LENGTH( desc ), data, size, &inputLayout );
				ZP_ASSERT( SUCCEEDED( hr ), "" );

				m_inputLayouts[ ZP_VERTEX_FORMAT_VERTEX_UV ] = inputLayout;
			}
		}
		break;

	case ZP_VERTEX_FORMAT_DESC_VERTEX_NORMAL_UV:
		{
			if( !m_inputLayouts[ ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV ] )
			{
				D3D11_INPUT_ELEMENT_DESC desc[] =
				{
					{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD0",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};

				hr = m_d3dDevice->CreateInputLayout( desc, ZP_ARRAY_LENGTH( desc ), data, size, &inputLayout );
				ZP_ASSERT( SUCCEEDED( hr ), "" );

				m_inputLayouts[ ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV ] = inputLayout;
			}
		}
		break;

	case ZP_VERTEX_FORMAT_DESC_VERTEX_NORMAL_UV2:
		{
			if( !m_inputLayouts[ ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2 ] )
			{
				D3D11_INPUT_ELEMENT_DESC desc[] =
				{
					{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD0",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD1",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};

				hr = m_d3dDevice->CreateInputLayout( desc, ZP_ARRAY_LENGTH( desc ), data, size, &inputLayout );
				ZP_ASSERT( SUCCEEDED( hr ), "" );

				m_inputLayouts[ ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2 ] = inputLayout;
			}
		}
		break;

	default:
		ZP_ASSERT( false, "" );
		break;
	}
}

#if 0
#define HR( r )						if( FAILED( (r) ) ) { return false; }
#define HR_( r )					if( FAILED( (r) ) ) { return; }
#define HR_V( r,   )				if( FAILED( (r) ) ) { return ( ); }

#define HR_MSG( r, msg, ... )		if( FAILED( (r) ) ) { zp_printfln( (msg), __VA_ARGS__ ); return false; }
#define HR_MSG_( r, msg, ... )		if( FAILED( (r) ) ) { zp_printfln( (msg), __VA_ARGS__ ); return; }
#define HR_MSG_V( r,  , msg, ... )	if( FAILED( (r) ) ) { zp_printfln( (msg), __VA_ARGS__ ); return ( ); }

zpDX11RenderingEngine::zpDX11RenderingEngine() : 
	m_vsyncEnabled( false ),
	m_dxgiFactory( ZP_NULL ),
	m_dxgiAdapter( ZP_NULL ),
	m_swapChain( ZP_NULL ),
	m_d3dDevice( ZP_NULL ),
	m_immediateContext( ZP_NULL ),
	m_window( ZP_NULL ),
	m_screenMode( ZP_SCREEN_MODE_WINDOWED ),
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

	// if neither of the display mode's dimensions are set, set to the window screen size
	if( !m_displayMode.width || !m_displayMode.height ) {
		m_displayMode.width = m_window->getScreenSize().getX();
		m_displayMode.height = m_window->getScreenSize().getY();
	}
	// if the display mode is set to unknown, default to RGBA8_UNORM
	if( m_displayMode.displayFormat == ZP_DISPLAY_FORMAT_UNKNOWN ) {
		m_displayMode.displayFormat = ZP_DISPLAY_FORMAT_RGBA8_UNORM;
	}
	// if the refresh rate is not set, find the closest display mode that matches
	if( !m_displayMode.refreshRate ) {
		//findClosestDisplayMode( m_displayMode, &m_displayMode );
		m_displayMode.refreshRate = 60;
	}
	
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	zp_zero_memory( &swapChainDesc );
	swapChainDesc.BufferDesc.Width = m_displayMode.width;
	swapChainDesc.BufferDesc.Height = m_displayMode.height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = m_displayMode.refreshRate;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = __zpToDX( m_displayMode.displayFormat );
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = (HWND)m_window->getWindowHandle();
	swapChainDesc.Windowed = m_screenMode == ZP_SCREEN_MODE_WINDOWED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;//DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL actualFeatureLevel;
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
	zp_uint featureLevelCount = sizeof( featureLevels ) / sizeof( featureLevels[0] );
	ID3D11DeviceContext* immediate = ZP_NULL;

	//hr = D3D11CreateDevice( m_dxgiAdapter, D3D_DRIVER_TYPE_UNKNOWN, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &m_d3dDevice, &actualFeatureLevel, &immediate );
	hr = D3D11CreateDeviceAndSwapChain( ZP_NULL, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_d3dDevice, &actualFeatureLevel, &immediate );
	if( FAILED( hr ) ) {
		//hr = D3D11CreateDevice( ZP_NULL, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &m_d3dDevice, &actualFeatureLevel, &immediate );
		hr = D3D11CreateDeviceAndSwapChain( m_dxgiAdapter, D3D_DRIVER_TYPE_UNKNOWN, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_d3dDevice, &actualFeatureLevel, &immediate );
		if( FAILED( hr ) ) {
			//hr = D3D11CreateDevice( ZP_NULL, D3D_DRIVER_TYPE_WARP, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &m_d3dDevice, &actualFeatureLevel, &immediate );
			hr = D3D11CreateDeviceAndSwapChain( ZP_NULL, D3D_DRIVER_TYPE_WARP, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_d3dDevice, &actualFeatureLevel, &immediate );
			if( FAILED( hr ) ) {
				//hr = D3D11CreateDevice( ZP_NULL, D3D_DRIVER_TYPE_REFERENCE, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &m_d3dDevice, &actualFeatureLevel, &immediate );
				hr = D3D11CreateDeviceAndSwapChain( ZP_NULL, D3D_DRIVER_TYPE_REFERENCE, 0, flags, featureLevels, featureLevelCount, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_d3dDevice, &actualFeatureLevel, &immediate );

				HR_MSG( hr, "Unable to create DirectX device." );
			}
		}
	}

	// create the immediate context wrapper
	m_immediateContext = new zpDX11RenderingContext( immediate, zpString( "immediate" ) );

	// get the actual feature level of the rendering engine
	switch( m_d3dDevice->GetFeatureLevel() ) {
		case D3D_FEATURE_LEVEL_11_0: m_engineType = ZP_RENDERING_ENGINE_DX11; break;
		case D3D_FEATURE_LEVEL_10_1: m_engineType = ZP_RENDERING_ENGINE_DX10_1; break;
		case D3D_FEATURE_LEVEL_10_0: m_engineType = ZP_RENDERING_ENGINE_DX10; break;
	}

	
	// create the swap chain
	//hr = m_dxgiFactory->CreateSwapChain( m_d3dDevice, &swapChainDesc, &m_swapChain );
	//HR_MSG( hr, "Unable to create swap chain." );

	// get the back buffer
	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* backBufferView;

	m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer );
	m_d3dDevice->CreateRenderTargetView( backBuffer, ZP_NULL, &backBufferView );
	
	// remove reference to texture so render target now owns pointer
	backBuffer->Release();

	// create the render target and depth buffer for the immediate context
	zpDX11Texture* target = new zpDX11Texture();
	target->m_dimension = ZP_TEXTURE_DIMENSION_2D;
	target->m_height = m_displayMode.height;
	target->m_texture = backBuffer;
	target->m_textureRenderTarget = backBufferView;
	target->m_textureResourceView = ZP_NULL;
	target->m_type = ZP_TEXTURE_TYPE_RENDER_TARGET;
	target->m_width = m_displayMode.width;
	m_immediateRenderTarget = target;
	
	m_immediateDepthStencilBuffer = createDepthBuffer( ZP_DISPLAY_FORMAT_D24S8_UNORM_UINT, m_displayMode.width, m_displayMode.height );

	// set render target and depth buffer for the immediate context automatically
	m_immediateContext->setRenderTarget( m_immediateRenderTarget );
	m_immediateContext->setDepthStencilBuffer( m_immediateDepthStencilBuffer );
	
	return true;
}
void zpDX11RenderingEngine::destroy() {
	//ZP_SAFE_RELEASE( m_depthBuffer );
	//ZP_SAFE_RELEASE( m_depthBufferView );

	//ZP_SAFE_RELEASE( m_backBuffer );
	//ZP_SAFE_RELEASE( m_backBufferView );

	//ZP_SAFE_RELEASE( m_immediateContext );
	ZP_SAFE_RELEASE( m_swapChain );
	ZP_SAFE_RELEASE( m_d3dDevice );

	ZP_SAFE_REMOVE_REF( m_immediateContext );
}

zpRenderingEngineType zpDX11RenderingEngine::getEngineType() const {
	return m_engineType;
}

zp_uint zpDX11RenderingEngine::enumerateDisplayModes( zpDisplayFormat displayFormat, zpArrayList<zpDisplayMode>* outDisplayModes ) {
	if( !m_dxgiAdapter ) return 0;

	ZP_ASSERT( outDisplayModes != ZP_NULL, "Out parameter not set." );

	IDXGIOutput* adapterOutput;
	HRESULT hr;

	hr = m_dxgiAdapter->EnumOutputs( 0, &adapterOutput );
	HR_MSG_V( hr, 0, "Unable to get display modes." );

	zp_uint numModes;
	DXGI_FORMAT format = __zpToDX( displayFormat );
	hr = adapterOutput->GetDisplayModeList( format, DXGI_ENUM_MODES_INTERLACED, &numModes, ZP_NULL );
	HR_MSG_V( hr, 0, "Unable to list display modes." );

	zpArrayList<DXGI_MODE_DESC> modes;
	modes.ensureCapacity( numModes );
	hr = adapterOutput->GetDisplayModeList( format, DXGI_ENUM_MODES_INTERLACED, &numModes, modes.begin() );
	HR_MSG_V( hr, 0, "Unable to list display modes." );

	zp_uint count = 0;
	outDisplayModes->clear();
	modes.foreach( [ &outDisplayModes, &count ]( const DXGI_MODE_DESC& mode ) {
		if( mode.RefreshRate.Denominator == 1 ) {
			zpDisplayMode m = { mode.Width, mode.Height, mode.RefreshRate.Numerator, __dxToZP( mode.Format ) };
			outDisplayModes->pushBack( m );
			++count;
		}
	});

	ZP_SAFE_RELEASE( adapterOutput );

	return count;
}
zp_bool zpDX11RenderingEngine::findClosestDisplayMode( const zpDisplayMode& displayMode, zpDisplayMode* outDisplayMode ) {
	if( !m_dxgiAdapter ) return false;

	ZP_ASSERT( outDisplayMode != ZP_NULL, "Out parameter not set." );

	DXGI_FORMAT format = __zpToDX( displayMode.displayFormat );
	ZP_ASSERT( format != DXGI_FORMAT_UNKNOWN, "Display mode format must be set." );

	IDXGIOutput* adapterOutput;
	HRESULT hr;

	hr = m_dxgiAdapter->EnumOutputs( 0, &adapterOutput );
	HR_MSG( hr, "Unable to get display modes." );

	DXGI_MODE_DESC inMode, outMode;
	zp_zero_memory( &inMode );

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
		zpDisplayMode mode = { outMode.Width, outMode.Height, outMode.RefreshRate.Numerator, __dxToZP( outMode.Format ) };
		*outDisplayMode = mode;
	}

	return true;
}

void zpDX11RenderingEngine::setDisplayMode( const zpDisplayMode& mode ) {
	m_displayMode = mode;
}
const zpDisplayMode& zpDX11RenderingEngine::getDisplayMode() const {
	return m_displayMode;
}

void zpDX11RenderingEngine::setScreenMode( zpScreenMode mode ) {
	m_screenMode = mode;
}
zpScreenMode zpDX11RenderingEngine::getScreenMode() const {
	return m_screenMode;
}

void zpDX11RenderingEngine::setWindow( zpWindow* window ) {
	m_window = window;
}
zpWindow* zpDX11RenderingEngine::getWindow() const {
	return m_window;
}

void zpDX11RenderingEngine::setVSyncEnabled( zp_bool enabled ) {
	m_vsyncEnabled = enabled;
}

void zpDX11RenderingEngine::present() {
	m_swapChain->Present( m_vsyncEnabled ? 1 : 0, 0 );
}

zpRenderingContext* zpDX11RenderingEngine::createRenderingContext( const zpString& name ) {
	ID3D11DeviceContext* context;
	m_d3dDevice->CreateDeferredContext( 0, &context );

	zpRenderingContext* ctx = new zpDX11RenderingContext( context, name );
	m_contexts.pushBack( ctx );

	return ctx;
}
zp_bool zpDX11RenderingEngine::removeRenderingContext( const zpString& name ) {
	//zpRenderingContext* ctx = ZP_NULL;
	//
	//m_contexts.indexOf( [ &name ]( zpRenderingContext* c ) -> zp_bool {
	//	return c->getName() == name;
	//}, &ctx );
	//
	//if( ctx ) {
	//	ctx->removeReference();
	//	return true;
	//}
	return false;
}
zpRenderingContext* zpDX11RenderingEngine::getRenderingContextByIndex( zp_uint index ) const {
	return m_contexts[ index ];
}
zpRenderingContext* zpDX11RenderingEngine::getRenderingContext( const zpString& name ) const {
	//const zpRenderingContext** ctx = ZP_NULL;
	//
	//m_contexts.findIf( [ &name ]( zpRenderingContext* c ) {
	//	return c->getName() == name;
	//}, &ctx );
	//
	//return *ctx;
	return ZP_NULL;
}
zp_uint zpDX11RenderingEngine::getRenderingContextCount() const {
	return m_contexts.size();
}

zpRenderingContext* zpDX11RenderingEngine::getImmediateRenderingContext() const {
	return m_immediateContext;
}
zpTexture* zpDX11RenderingEngine::getBackBufferRenderTarget() const {
	return m_immediateRenderTarget;
}
zpDepthStencilBuffer* zpDX11RenderingEngine::getBackBufferDepthStencilBuffer() const {
	return m_immediateDepthStencilBuffer;
}

zpBuffer* zpDX11RenderingEngine::createBuffer() {
	return new zpDX11Buffer();
}
zpTexture* zpDX11RenderingEngine::createTexture( zp_uint width, zp_uint height, zpTextureType type, zpTextureDimension dimension, zpDisplayFormat format, zpCpuAccess access, void* data, zp_uint mipLevels ) {
	HRESULT hr;
	ID3D11Texture2D* texture = ZP_NULL;
	ID3D11ShaderResourceView* srv = ZP_NULL;
	ID3D11RenderTargetView* rtv = ZP_NULL;

	D3D11_TEXTURE2D_DESC texDesc;
	zp_zero_memory( &texDesc );
	texDesc.ArraySize = 1;
	switch( type ) {
	case ZP_TEXTURE_TYPE_TEXTURE:
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		break;
	case ZP_TEXTURE_TYPE_RENDER_TARGET:
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
		break;
	case ZP_TEXTURE_TYPE_RENDER_TEXTURE:
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		break;
	}
	texDesc.CPUAccessFlags = __zpToDX( access );
	texDesc.Format = __zpToDX( format );
	texDesc.Height = height;
	texDesc.MipLevels = type == ZP_TEXTURE_TYPE_TEXTURE ? mipLevels : 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Width = width;

	// create the texture with or without data
	if( data ) {
		D3D11_SUBRESOURCE_DATA subData;
		zp_zero_memory( &subData );
		subData.pSysMem = data;

		hr = m_d3dDevice->CreateTexture2D( &texDesc, &subData, &texture );
	} else {
		hr = m_d3dDevice->CreateTexture2D( &texDesc, ZP_NULL, &texture );
	}
	HR_V( hr, ZP_NULL );

	// create the shader resource view if the texture is a normal texture or a render texture
	if( type == ZP_TEXTURE_TYPE_TEXTURE || type == ZP_TEXTURE_TYPE_RENDER_TEXTURE ) {
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		zp_zero_memory( &srvDesc );
		srvDesc.Format = texDesc.Format;
		switch( type ) {
		case ZP_TEXTURE_DIMENSION_1D:
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
			srvDesc.Texture1D.MipLevels = texDesc.MipLevels;
			break;
		case ZP_TEXTURE_DIMENSION_2D:
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
			break;
		case ZP_TEXTURE_DIMENSION_CUBE_MAP:
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MipLevels = texDesc.MipLevels;
			break;
		}

		hr = m_d3dDevice->CreateShaderResourceView( texture, &srvDesc, &srv );
		HR_V( hr, ZP_NULL );
	}

	// create the render target view if the texture is a render target or a render texture
	if( type == ZP_TEXTURE_TYPE_RENDER_TARGET || type == ZP_TEXTURE_TYPE_RENDER_TEXTURE ) {
		// TODO: assert if trying to create a 1D or cube map render target
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		zp_zero_memory( &rtvDesc );
		rtvDesc.Format = texDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		hr = m_d3dDevice->CreateRenderTargetView( texture, &rtvDesc, &rtv );
		HR_V( hr, ZP_NULL );
	}

	// remove references so texture now owns pointers
	ZP_SAFE_RELEASE( texture );
	ZP_SAFE_RELEASE( srv );
	ZP_SAFE_RELEASE( rtv );

	zpDX11Texture* tex = new zpDX11Texture();
	tex->m_width = width;
	tex->m_height = height;
	tex->m_dimension = dimension;
	tex->m_type = type;
	tex->m_texture = texture;
	tex->m_textureResourceView = srv;
	tex->m_textureRenderTarget = rtv;
	
	return tex;
}

zpTextureResource* zpDX11RenderingEngine::createTextureResource() {
	return new zpDX11TextureResource();
}
zpShader* zpDX11RenderingEngine::createShaderResource() {
	return new zpDX11ShaderResource();
}
/*
zpRenderTarget* zpDX11RenderingEngine::createRenderTarget( zpDisplayFormat format, zp_uint width, zp_uint height ) {
	HRESULT hr;
	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;

	D3D11_TEXTURE2D_DESC texDesc;
	zp_zero_memory( &texDesc );
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = __zpToDX( format );
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 0;
	texDesc.SampleDesc.Quality = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Width = width;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	zp_zero_memory( &rtvDesc );
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	zp_zero_memory( &srvDesc );
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	hr = m_d3dDevice->CreateTexture2D( &texDesc, ZP_NULL, &texture );
	HR_V( hr, ZP_NULL );

	hr = m_d3dDevice->CreateRenderTargetView( texture, &rtvDesc, &rtv );
	HR_V( hr, ZP_NULL );

	hr = m_d3dDevice->CreateShaderResourceView( texture, &srvDesc, &srv );
	HR_V( hr, ZP_NULL );

	// remove reference to texture so render target now owns pointer
	texture->Release();

	return new zpDX11RenderTarget( &texture, &rtv, 1, width, height );
}
zpRenderTarget* zpDX11RenderingEngine::createMultiRenderTarget( zp_uint targetCount, zpDisplayFormat* formats, zp_uint width, zp_uint height ) {
	if( targetCount == 1 ) return createRenderTarget( formats[ 0 ], width, height );

	HRESULT hr;
	zpArrayList<ID3D11RenderTargetView*> targets( targetCount );
	zpArrayList<ID3D11Texture2D*> textures( targetCount );

	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* rtv;

	D3D11_TEXTURE2D_DESC texDesc;
	D3D11_RENDER_TARGET_VIEW_DESC desc;

	for( zp_uint i = 0; i < targetCount; ++i ) {
		zp_zero_memory( &texDesc );
		texDesc.ArraySize = 1;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.Format = __zpToDX( formats[ i ] );
		texDesc.Height = height;
		texDesc.MipLevels = 1;
		texDesc.MiscFlags = 0;
		texDesc.SampleDesc.Count = 0;
		texDesc.SampleDesc.Quality = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.Width = width;

		zp_zero_memory( &desc );
		desc.Format = texDesc.Format;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		hr = m_d3dDevice->CreateTexture2D( &texDesc, ZP_NULL, &texture );
		HR_V( hr, ZP_NULL );

		hr = m_d3dDevice->CreateRenderTargetView( texture, &desc, &rtv );
		HR_V( hr, ZP_NULL );

		// remove reference to texture so render target now owns pointer
		texture->Release();

		textures.pushBack( texture );
		targets.pushBack( rtv );
	}

	return new zpDX11RenderTarget( textures.begin(), targets.begin(), targets.size(), width, height );
}
*/
zpDepthStencilBuffer* zpDX11RenderingEngine::createDepthBuffer( zpDisplayFormat format, zp_uint width, zp_uint height ) {
	HRESULT hr;
	ID3D11Texture2D* texture;
	ID3D11DepthStencilView* view;
	
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	zp_zero_memory( &depthStencilDesc );
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = __zpToDX( format );
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	zp_zero_memory( &depthStencilViewDesc );
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_d3dDevice->CreateTexture2D( &depthStencilDesc, ZP_NULL, &texture );
	HR_V( hr, ZP_NULL );

	hr = m_d3dDevice->CreateDepthStencilView( texture, ZP_NULL, &view );
	HR_V( hr, ZP_NULL );
	
	// remove reference to texture so view now owns pointer
	texture->Release();
	
	return new zpDepthStencilBufferImpl( format, texture, view, width, height );
}

zpVertexLayout* zpDX11RenderingEngine::createVertexLayout( const zpString& desc ) {
	return new zpDX11VertexLayout();
}

zpSamplerState* zpDX11RenderingEngine::createSamplerState( const zpSamplerStateDesc& desc ) {
	HRESULT hr;
	D3D11_SAMPLER_DESC samplerDesc;
	zp_zero_memory( &samplerDesc );
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;	// @TODO: implement mapping
	samplerDesc.AddressU = __zpToDX( desc.texWrapU );
	samplerDesc.AddressV = __zpToDX( desc.texWrapV );
	samplerDesc.AddressW = __zpToDX( desc.texWrapW );
	samplerDesc.MipLODBias = desc.lodBias;
	samplerDesc.MaxAnisotropy = desc.maxAnisotrpy;
	samplerDesc.ComparisonFunc = __zpToDX( desc.cmpFunc );
	desc.borderColor.store4( samplerDesc.BorderColor );
	samplerDesc.MinLOD = desc.lodMin;
	samplerDesc.MaxLOD = desc.lodMax;

	ID3D11SamplerState* state;
	hr = m_d3dDevice->CreateSamplerState( &samplerDesc, &state );

	return new zpDX11SamplerState( state );
}

zpRasterState* zpDX11RenderingEngine::createRasterState( const zpRasterStateDesc& desc ) {
	HRESULT hr;
	D3D11_RASTERIZER_DESC rasterDesc;
	zp_zero_memory( &rasterDesc );
	rasterDesc.FillMode = __zpToDX( desc.fillMode );
	rasterDesc.CullMode = __zpToDX( desc.cullMode );
	rasterDesc.FrontCounterClockwise = desc.frontFace == ZP_FRONT_FACE_CCW;
	rasterDesc.DepthBias = desc.depthBias;
	rasterDesc.DepthBiasClamp = desc.depthBiasClamp;
	rasterDesc.SlopeScaledDepthBias = desc.slopeScaledDepthBias;
	rasterDesc.DepthClipEnable = desc.depthClipEnable;
	rasterDesc.ScissorEnable = desc.scissorEnable;
	rasterDesc.MultisampleEnable = desc.multisampleEnable;
	rasterDesc.AntialiasedLineEnable = desc.antialiasedLineEnable;

	ID3D11RasterizerState* state;
	hr = m_d3dDevice->CreateRasterizerState( &rasterDesc, &state );

	return new zpDX11RasterState( state );
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

ID3D11Device* zpDX11RenderingEngine::getDevice() const {
	return m_d3dDevice;
}
#endif
