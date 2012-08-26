#pragma once
#ifndef ZP_RENDERING_ENGINE_H
#define ZP_RENDERING_ENGINE_H

ZP_PURE_INTERFACE zpRenderingEngine {
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

	virtual zpBuffer* createBuffer() = 0;

	virtual zpTextureResource* createTextureResource() = 0;
	virtual zpShaderResource* createShaderResource() = 0;

	virtual zpRenderTarget* createRenderTarget( zpDisplayFormat format, zp_uint width, zp_uint height ) = 0;
	virtual zpRenderTarget* createMultiRenderTarget( zp_uint targetCount, zpDisplayFormat* formats, zp_uint width, zp_uint height ) = 0;
	virtual zpDepthStencilBuffer* createDepthBuffer( zpDisplayFormat format, zp_uint width, zp_uint height ) = 0;

	virtual zpVertexLayout* createVertexLayout( const zpString& desc ) = 0;

	virtual zpSamplerState* createSamplerState( const zpSamplerStateDesc& desc ) = 0;

protected:
	virtual zp_bool initialize() = 0;
	virtual void shutdown() = 0;

	friend class zpRenderingFactory;
};

#endif