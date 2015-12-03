#pragma once
#ifndef ZP_DX11_RENDERING_ENGINE_H
#define ZP_DX11_RENDERING_ENGINE_H

struct IDXGIFactory;
struct IDXGIAdapter;
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

struct zpDynamicInputLayout
{
	zp_hash hash;
	zp_uint stride;
	ID3D11InputLayout* layout;
};

class zpRenderingEngineImpl
{
public:
	zpRenderingEngineImpl();
	~zpRenderingEngineImpl();

	void initialize();
	void create( zp_handle hWindow, zp_uint width, zp_uint height, zpDisplayMode& displayMode, zpScreenMode screenMode, zpRenderingEngineType& outEngineType, zpRenderingContextImpl*& outImmediateContext, zpTextureImpl*& outImmediateRenderTarget );
	void destroy();
	void shutdown();

	zpBufferImpl* createBuffer( zpBufferType type, zpBufferBindType bind, zp_uint size, zp_uint stride, const void* data );
	zp_bool destroyBuffer( zpBufferImpl* buffer );

	zpTextureImpl* createTexture( zp_uint width, zp_uint height, zpTextureType type, zpTextureDimension dimension, zpDisplayFormat format, zpCpuAccess access, const void* data = ZP_NULL, zp_uint strideInBytes = 0, zp_uint mipLevels = 1,zp_uint maxMipLevel = 0, zp_uint arraySize = 1 );
	zpTextureImpl* createTextureFromFile( const zpString& filename );
	zp_bool destroyTexture( zpTextureImpl* texture );

	zpDepthStencilBufferImpl* createDepthStencilBuffer( zp_uint width, zp_uint height, zpDisplayFormat format );
	zp_bool destroyDepthStencilBuffer( zpDepthStencilBufferImpl* depthStencil );

	zpRasterStateImpl* createRasterState( const zpRasterStateDesc& desc );
	zpSamplerStateImpl* createSamplerState( const zpSamplerStateDesc& desc );
	zpDepthStencilStateImpl* createDepthStencilState( const zpDepthStencilStateDesc& desc );
	zpBlendStateImpl* createBlendState( const zpBlendStateDesc& desc );

	zpShaderImpl* createShader();
	zp_bool loadShader( zpShaderImpl* shader, const zpBison::Value& shaderFile );
	zp_bool destroyShader( zpShaderImpl* shader );

	void present( zp_bool vsync );

	zp_bool getInputLayoutFormatAndStride( const zp_char* format, zp_hash& outHash, zp_uint& outStride ) const;

	const zpDynamicInputLayout* getDynamicInputLayout( zp_hash format ) const;

	zp_bool performScreenshot();
	zp_bool takeScreenshot( zp_uint width, zp_uint height, zpDataBuffer& screenBuffer );

private:
	zp_hash createDynamicVertexLayout( const zp_char* format, zp_uint formatLength, const void* data, zp_uint size );

	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_d3dDevice;

	ID3D11Texture2D* m_screenshotTexture;

	zpRenderingContextImpl m_immidiateContext;
	zpRenderingEngineType m_actualRenderingEngineType;
	
	zpArrayList< zpDynamicInputLayout > m_dynamicInputLayouts;

	zpFixedArrayList< zpSamplerStateImpl, ZP_RENDERING_MAX_SAMPLER_STATES > m_samplerStates;
	zpFixedArrayList< zpRasterStateImpl, ZP_RENDERING_MAX_RASTER_STATES > m_rasterStates;
	zpFixedArrayList< zpDepthStencilStateImpl, ZP_RENDERING_MAX_DEPTH_STENCIL_STATES > m_depthStencilStates;
	zpFixedArrayList< zpBlendStateImpl, ZP_RENDERING_MAX_BLEND_STATES > m_blendStates;

	zpFixedArrayList< zpTextureImpl, ZP_RENDERING_MAX_TEXTURES > m_textures;
	zpFixedArrayList< zpTextureImpl*, ZP_RENDERING_MAX_TEXTURES > m_freeTextures;
	zpFixedArrayList< zpTextureImpl*, ZP_RENDERING_MAX_TEXTURES > m_usedTextures;

	zpFixedArrayList< zpBufferImpl,  ZP_RENDERING_MAX_BUFFERS > m_buffers;
	zpFixedArrayList< zpBufferImpl*, ZP_RENDERING_MAX_BUFFERS > m_freeBuffers;
	zpFixedArrayList< zpBufferImpl*, ZP_RENDERING_MAX_BUFFERS > m_usedBuffers;

	zpFixedArrayList< zpShaderImpl, ZP_RENDERING_MAX_SHADERS > m_shaders;
	zpFixedArrayList< zpShaderImpl*, ZP_RENDERING_MAX_SHADERS > m_freeShaders;
	zpFixedArrayList< zpShaderImpl*, ZP_RENDERING_MAX_SHADERS > m_usedShaders;
};

#if 0
struct IDXGIFactory;
struct IDXGIAdapter;
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

class zpDX11RenderingEngine : public zpRenderingEngine {
public:
	~zpDX11RenderingEngine();

	zp_bool create();
	void destroy();

	zpRenderingEngineType getEngineType() const;

	zp_uint enumerateDisplayModes( zpDisplayFormat displayFormat, zpArrayList<zpDisplayMode>* outDisplayModes );
	zp_bool findClosestDisplayMode( const zpDisplayMode& displayMode, zpDisplayMode* outDisplayMode );

	void setDisplayMode( const zpDisplayMode& mode );
	const zpDisplayMode& getDisplayMode() const;

	void setScreenMode( zpScreenMode mode );
	zpScreenMode getScreenMode() const;

	void setWindow( zpWindow* window );
	zpWindow* getWindow() const;

	void setVSyncEnabled( zp_bool enabled );

	void present();

	zpRenderingContext* createRenderingContext( const zpString& name );
	zp_bool removeRenderingContext( const zpString& name );
	zpRenderingContext* getRenderingContextByIndex( zp_uint index ) const;
	zpRenderingContext* getRenderingContext( const zpString& name ) const;
	zp_uint getRenderingContextCount() const;

	zpRenderingContext* getImmediateRenderingContext() const;
	zpTexture* getBackBufferRenderTarget() const;
	zpDepthStencilBuffer* getBackBufferDepthStencilBuffer() const;

	zpBuffer* createBuffer();
	zpTexture* createTexture( zp_uint width, zp_uint height, zpTextureType type, zpTextureDimension dimension, zpDisplayFormat format, zpCpuAccess access, void* data = ZP_NULL, zp_uint mipLevels = 10 );

	zpTextureResource* createTextureResource();
	zpShaderResource* createShaderResource();

	//zpRenderTarget* createRenderTarget( zpDisplayFormat format, zp_uint width, zp_uint height );
	//zpRenderTarget* createMultiRenderTarget( zp_uint targetCount, zpDisplayFormat* formats, zp_uint width, zp_uint height );
	zpDepthStencilBuffer* createDepthBuffer( zpDisplayFormat format, zp_uint width, zp_uint height );

	zpVertexLayout* createVertexLayout( const zpString& desc );

	zpSamplerState* createSamplerState( const zpSamplerStateDesc& desc );
	zpRasterState* createRasterState( const zpRasterStateDesc& desc );

protected:
	zp_bool initialize();
	void shutdown();

private:
	zpDX11RenderingEngine();

	ID3D11Device* getDevice() const;

	zp_bool m_vsyncEnabled;

	IDXGIFactory* m_dxgiFactory;
	IDXGIAdapter* m_dxgiAdapter;
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_d3dDevice;

	zpWindow* m_window;

	zpRenderingContext* m_immediateContext;
	zpTexture* m_immediateRenderTarget;
	zpDepthStencilBuffer* m_immediateDepthStencilBuffer;

	zpScreenMode m_screenMode;
	zpDisplayMode m_displayMode;
	zpRenderingEngineType m_engineType;

	zpColor4f m_clearColor;

	zpArrayList<zpRenderingContext*> m_contexts;

	friend class zpRenderingFactory;
	friend class zpDX11Buffer;
	friend class zpDX11TextureResource;
	friend class zpDX11ShaderResource;
	friend class zpDX11VertexLayout;
};
#endif

#endif