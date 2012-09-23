#pragma once
#ifndef ZP_DX11_RASTER_STATE_H
#define ZP_DX11_RASTER_STATE_H

struct ID3D11RasterizerState;

class zpDX11RasterState : public zpRasterState {
public:
	virtual ~zpDX11RasterState();

private:
	zpDX11RasterState( ID3D11RasterizerState* state = ZP_NULL );

	ID3D11RasterizerState* m_raster;

	friend class zpDX11RenderingEngine;
	friend class zpDX11RenderingContext;
};

#endif