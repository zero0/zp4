#pragma once
#ifndef ZP_RASTER_STATE_H
#define ZP_RASTER_STATE_H

class zpRasterStateImpl;

class zpRasterState
{
	ZP_NON_COPYABLE( zpRasterState );
public:
	~zpRasterState();

	zpRasterStateImpl* getRasterStateImpl() const;

private:
	zpRasterState( zpRasterStateImpl* rasterState );

	zpRasterStateImpl* m_rasterState;

	friend class zpRenderingEngine;
};

#endif