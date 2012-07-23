#pragma once
#ifndef ZP_DEPTH_STENCIL_BUFFER_H
#define ZP_DEPTH_STENCIL_BUFFER_H

ZP_PURE_INTERFACE zpDepthStencilBuffer : public zpReferencedObject {
public:
	virtual zp_uint getWidth() const = 0;
	virtual zp_uint getHeight() const = 0;

	virtual zpRenderingDisplayFormat getDisplayFormat() const = 0;
};

#endif