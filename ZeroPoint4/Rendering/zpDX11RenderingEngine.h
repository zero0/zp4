#pragma once
#ifndef ZP_DX11_RENDERING_ENGINE_H
#define ZP_DX11_RENDERING_ENGINE_H

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

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

	zp_bool initialize();
	zp_bool create();

	zp_uint enumerateDisplayModes( zpRenderingDisplayFormat displayFormat, zpArrayList<zpRenderingDisplayMode>* outDisplayModes );
	zp_bool findClosestDisplayMode( const zpRenderingDisplayMode& displayMode, zpRenderingDisplayMode* outDisplayMode );

	void setDisplayMode( const zpRenderingDisplayMode& mode );
	const zpRenderingDisplayMode& getDisplayMode() const;

	void setScreenMode( zpRenderingScreenMode mode );
	zpRenderingScreenMode getScreenMode() const;

	void setWindow( zpWindow* window );
	zpWindow* getWindow() const;

	void clear();
	void present();

private:
	zpDX11RenderingEngine();

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

	friend class zpRenderingFactory;
};

#endif