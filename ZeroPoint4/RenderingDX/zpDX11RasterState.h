#pragma once
#ifndef ZP_DX11_RASTER_STATE_H
#define ZP_DX11_RASTER_STATE_H

struct ID3D11RasterizerState;

class zpRasterStateImpl
{
public:
	zpRasterStateImpl();
	~zpRasterStateImpl();

private:
	ID3D11RasterizerState* m_raster;
	zp_hash m_descHash;
	zpRasterStateDesc m_desc;

	friend class zpRenderingEngineImpl;
	friend class zpRenderingContextImpl;
};

#endif