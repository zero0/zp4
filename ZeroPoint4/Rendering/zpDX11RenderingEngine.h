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

	zp_uint enumerateDisplayModes( zpRenderingDisplayFormat displayFormat, zpArrayList<zpRenderingDisplayMode>* outDisplayModes );
	zp_bool findClosestDisplayMode( const zpRenderingDisplayMode& displayMode, zpRenderingDisplayMode* outDisplayMode );

	void setDisplayMode( const zpRenderingDisplayMode& mode );
	const zpRenderingDisplayMode& getDisplayMode() const;

	void setScreenMode( zpRenderingScreenMode mode );
	zpRenderingScreenMode getScreenMode() const;

	void setWindow( zpWindow* window );
	zpWindow* getWindow() const;

	void setClearColor( const zpColor4f& color, zp_uint renderTargetIndex = 0 );
	const zpColor4f& getClearColor( zp_uint renderTargetIndex = 0 ) const;

	void clear();
	void present();

	zpRenderingContext* createRenderingContext();
	void setCurrentRenderingContext( zpRenderingContext* context );
	zpRenderingContext* getCurrentRenderingContext() const;

	zpBuffer* createBuffer();

	zpTextureResource* createTextureResource();

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
	ID3D11DeviceContext* m_immediateContext;

	ID3D11Texture2D* m_backBuffer;
	ID3D11RenderTargetView* m_backBufferView;

	ID3D11Texture2D* m_depthBuffer;
	ID3D11DepthStencilView* m_depthBufferView;

	zpWindow* m_window;

	zpRenderingScreenMode m_screenMode;
	zpRenderingDisplayMode m_displayMode;
	zpRenderingEngineType m_engineType;

	zpColor4f m_clearColor;

	friend class zpRenderingFactory;
	friend class zpDX11Buffer;
	friend class zpDX11TextureResource;
	friend class zpDX11ShaderResource;
};

#endif