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

protected:
	zp_bool initialize();
	void shutdown();

private:
	zpDX11RenderingEngine();

	ID3D11Device* getDevice() const;

	IDXGIFactory* m_dxgiFactory;
	IDXGIAdapter* m_dxgiAdapter;
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_d3dDevice;

	zpWindow* m_window;

	zpRenderingContext* m_immediateContext;
	zpRenderTarget* m_immediateRenderTarget;
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