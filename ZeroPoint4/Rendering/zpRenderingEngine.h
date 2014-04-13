#pragma once
#ifndef ZP_RENDERING_ENGINE_H
#define ZP_RENDERING_ENGINE_H

class zpRenderingEngineImpl;

class zpRenderingEngine
{
	ZP_NON_COPYABLE( zpRenderingEngine );

public:
	zpRenderingEngine();
	~zpRenderingEngine();

	void initialize();
	void create( zp_handle hWindow, const zpVector2i& size );
	void destroy();
	void shutdown();

	zpRenderingEngineType getEngineType() const;

	void setDisplayMode( const zpDisplayMode& mode );
	const zpDisplayMode& getDisplayMode() const;

	void setScreenMode( zpScreenMode mode );
	zpScreenMode getScreenMode() const;

	void setVSyncEnabled( zp_bool enabled );
	zp_bool isVSyncEnabled() const;

	void present();

	zpRenderingContext* getImmediateRenderingContext();
	zpTexture* getBackBufferRenderTarget();
	zpDepthStencilBuffer* getBackBufferDepthStencilBuffer();

	zp_uint getNumRenderingContexts() const;
	zpRenderingContext* getRenderingContext( zp_uint index ) const;
	zpRenderingContext* createRenderingContext();

	void destroyRenderingContext( zpRenderingContext& context );

	void createBuffer( zpBuffer& outBuffer, zpBufferType type, zpBufferBindType bind, zp_uint size, zp_uint stride = 0, const void* data = 0 );
	zp_bool destroyBuffer( zpBuffer& buffer );

	zp_bool createTexture( zpTexture& texture, zp_uint width, zp_uint height, zpTextureType type, zpTextureDimension dimension, zpDisplayFormat format, zpCpuAccess access, void* data = ZP_NULL, zp_uint mipLevels = 1 );
	zp_bool createTextureFromFile( zpTexture& texture, const zpString& filename );
	zp_bool destroyTexture( zpTexture& texture );

	void createDepthBuffer( zpDepthStencilBuffer& depthStencilBuffer, zp_uint width, zp_uint height, zpDisplayFormat format );
	void destroyDepthBuffer( zpDepthStencilBuffer& depthStencilBuffer );

	void createRasterState( zpRasterState& state, const zpRasterStateDesc& desc );
	void createSamplerState( zpSamplerState& state, const zpSamplerStateDesc& desc );
	void createDepthStencilState( zpDepthStencilState& state, const zpDepthStencilStateDesc& desc );
	void createBlendState( zpBlendState& state, const zpBlendStateDesc& desc );

	zp_bool createShader( zpShader& shader );
	zp_bool loadShader( zpShader& shader, const zpBison::Value& shaderfile );
	zp_bool destroyShader( zpShader& shader );

	zpRenderingEngineImpl* getRenderingEngineImpl() const { return m_renderingEngine; }

	const zpVector2i& getScreenSize() const { return m_screenSize; }

private:
	zpRenderingEngineImpl* m_renderingEngine;
	
	zpVector2i m_screenSize;
	zpRenderingEngineType m_renderingEngineType;
	zpDisplayMode m_displayMode;
	zpScreenMode m_screenMode;
	zp_bool m_isVSyncEnabled;

	zpTexture m_immediateRenderTarget;
	zpDepthStencilBuffer m_immediateDepthStencilBuffer;
	zpRenderingContext m_immediateContext;

	//zpFixedArrayList< zpRenderingContext*, ZP_RENDERING_MAX_RENDERING_CONTEXTS > m_renderingContexts;

	//zpHashMap< zp_hash, zpRasterState* >  m_rasterStates;
	//zpHashMap< zp_hash, zpSamplerState* > m_samplerStates;
	zpFixedArrayList< zpRasterState, ZP_RENDERING_MAX_RASTER_STATES > m_rasterStates;
	zpFixedArrayList< zpSamplerState, ZP_RENDERING_MAX_SAMPLER_STATES > m_samplerStates;
};

/*
ZP_PURE_INTERFACE zpRenderingEngine
{
public:
	virtual zp_bool create() = 0;
	virtual void destroy() = 0;

	virtual zpRenderingEngineType getEngineType() const = 0;

	virtual zp_uint enumerateDisplayModes( zpDisplayFormat displayFormat, zpArrayList<zpDisplayMode>* outDisplayModes ) = 0;
	virtual zp_bool findClosestDisplayMode( const zpDisplayMode& displayMode, zpDisplayMode* outDisplayMode ) = 0;
	
	virtual void setDisplayMode( const zpDisplayMode& mode ) = 0;
	virtual const zpDisplayMode& getDisplayMode() const = 0;

	virtual void setScreenMode( zpScreenMode mode ) = 0;
	virtual zpScreenMode getScreenMode() const = 0;

	virtual void setWindow( zpWindow* window ) = 0;
	virtual zpWindow* getWindow() const = 0;

	virtual void setVSyncEnabled( zp_bool enabled ) = 0;

	virtual void present() = 0;

	virtual zpRenderingContext* createRenderingContext( const zpString& name ) = 0;
	virtual zp_bool removeRenderingContext( const zpString& name ) = 0;
	virtual zpRenderingContext* getRenderingContextByIndex( zp_uint index ) const = 0;
	virtual zpRenderingContext* getRenderingContext( const zpString& name ) const = 0;
	virtual zp_uint getRenderingContextCount() const = 0;

	virtual zpRenderingContext* getImmediateRenderingContext() const = 0;
	virtual zpTexture* getBackBufferRenderTarget() const = 0;
	virtual zpDepthStencilBuffer* getBackBufferDepthStencilBuffer() const = 0;

	virtual zpBuffer* createBuffer() = 0;
	virtual zpTexture* createTexture( zp_uint width, zp_uint height, zpTextureType type, zpTextureDimension dimension, zpDisplayFormat format, zpCpuAccess access, void* data = ZP_NULL, zp_uint mipLevels = 10 ) = 0;

	virtual zpTextureResource* createTextureResource() = 0;
	virtual zpShaderResource* createShaderResource() = 0;

	//virtual zpRenderTarget* createRenderTarget( zpDisplayFormat format, zp_uint width, zp_uint height ) = 0;
	//virtual zpRenderTarget* createMultiRenderTarget( zp_uint targetCount, zpDisplayFormat* formats, zp_uint width, zp_uint height ) = 0;
	virtual zpDepthStencilBuffer* createDepthBuffer( zpDisplayFormat format, zp_uint width, zp_uint height ) = 0;

	virtual zpVertexLayout* createVertexLayout( const zpString& desc ) = 0;

	virtual zpSamplerState* createSamplerState( const zpSamplerStateDesc& desc ) = 0;
	virtual zpRasterState* createRasterState( const zpRasterStateDesc& desc ) = 0;

protected:
	virtual zp_bool initialize() = 0;
	virtual void shutdown() = 0;

	friend class zpRenderingFactory;
};
*/

#endif