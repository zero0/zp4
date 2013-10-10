#pragma once
#ifndef ZP_RASTER_STATE_H
#define ZP_RASTER_STATE_H

class zpRasterStateImpl;

class zpRasterState
{
public:
	zpRasterState();
	~zpRasterState();

	zpRasterStateImpl* getRasterStateImpl() const;

private:
	zpRasterStateImpl* m_rasterState;

	friend class zpRenderingEngine;
};

#endif