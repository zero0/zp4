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
	zpRasterState();	
	zpRasterState( zpRasterStateImpl* rasterState, zp_hash descHash );

	zpRasterStateImpl* m_rasterState;
	zp_hash m_descHash;

	friend class zpRenderingEngine;
};

#endif