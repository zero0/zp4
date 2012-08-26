#pragma once
#ifndef ZP_OPENGL_RENDERING_ENGINE_H
#define ZP_OPENGL_RENDERING_ENGINE_H

class zpOpenGLRenderingEngine : public zpRenderingEngine {
public:
	~zpOpenGLRenderingEngine();

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

	zpBuffer* createBuffer();

	zpTextureResource* createTextureResource();
	zpShaderResource* createShaderResource();

	zpRenderTarget* createRenderTarget( zpDisplayFormat format, zp_uint width, zp_uint height );
	zpRenderTarget* createMultiRenderTarget( zp_uint targetCount, zpDisplayFormat* formats, zp_uint width, zp_uint height );
	zpDepthStencilBuffer* createDepthBuffer( zpDisplayFormat format, zp_uint width, zp_uint height );

	zpVertexLayout* createVertexLayout( const zpString& desc );

	zpSamplerState* createSamplerState( const zpSamplerStateDesc& desc );

protected:
	zp_bool initialize();
	void shutdown();

private:
	zpOpenGLRenderingEngine();
	
	zp_bool m_vsyncEnabled;

	zpWindow* m_window;

	zpRenderingContext* m_immediateContext;
	zpRenderTarget* m_immediateRenderTarget;
	zpDepthStencilBuffer* m_immediateDepthStencilBuffer;

	zpScreenMode m_screenMode;
	zpDisplayMode m_displayMode;
	zpRenderingEngineType m_engineType;

	zpColor4f m_clearColor;

	friend class zpRenderingFactory;
};

#endif