#pragma once
#ifndef ZP_DX11_RENDERING_ENGINE_H
#define ZP_DX11_RENDERING_ENGINE_H

struct ID3D11Device;

class zpDX11RenderingEngine : public zpRenderingEngine {
public:
	~zpDX11RenderingEngine();

	void clear();
	void present();

private:
	zpDX11RenderingEngine();

	ID3D11Device* m_d3dDevice;

	friend class zpRenderingFactory;
};

#endif