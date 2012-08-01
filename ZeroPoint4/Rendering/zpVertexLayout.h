#pragma once
#ifndef ZP_VERTEX_LAYOUT
#define ZP_VERTEX_LAYOUT

ZP_PURE_INTERFACE zpVertexLayout {
public:
	virtual zp_uint getStride() const = 0;
};

#endif