#pragma once
#ifndef ZP_RENDERING_CONTEXT
#define ZP_RENDERING_CONTEXT

ZP_PURE_INTERFACE zpRenderingContext {
public:
	
	virtual void map( zpBuffer* buffer, zpRenderingMapType mapType, zp_uint subResource, void** data ) = 0;
	virtual void unmap( zpBuffer* buffer, zp_uint subResource ) = 0;
};

#endif