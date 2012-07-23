#pragma once
#ifndef ZP_RENDERING_ENGINE_H
#define ZP_RENDERING_ENGINE_H

ZP_PURE_INTERFACE zpRenderingEngine {
public:
	virtual zp_bool create() = 0;
	virtual void destroy() = 0;

	virtual zpRenderingEngineType getEngineType() const = 0;

	virtual zp_uint enumerateDisplayModes( zpRenderingDisplayFormat displayFormat, zpArrayList<zpRenderingDisplayMode>* outDisplayModes ) = 0;
	virtual zp_bool findClosestDisplayMode( const zpRenderingDisplayMode& displayMode, zpRenderingDisplayMode* outDisplayMode ) = 0;
	
	virtual void setDisplayMode( const zpRenderingDisplayMode& mode ) = 0;
	virtual const zpRenderingDisplayMode& getDisplayMode() const = 0;

	virtual void setScreenMode( zpRenderingScreenMode mode ) = 0;
	virtual zpRenderingScreenMode getScreenMode() const = 0;

	virtual void setWindow( zpWindow* window ) = 0;
	virtual zpWindow* getWindow() const = 0;

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

	virtual zpRenderTarget* createRenderTarget( zpRenderingDisplayFormat format, zp_uint width, zp_uint height ) = 0;
	virtual zpRenderTarget* createMultiRenderTarget( zp_uint targetCount, zpRenderingDisplayFormat* formats, zp_uint width, zp_uint height ) = 0;
	virtual zpDepthStencilBuffer* createDepthBuffer( zpRenderingDisplayFormat format, zp_uint width, zp_uint height ) = 0;

protected:
	virtual zp_bool initialize() = 0;
	virtual void shutdown() = 0;

	friend class zpRenderingFactory;
};

#endif