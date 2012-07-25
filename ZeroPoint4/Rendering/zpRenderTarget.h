#pragma once
#ifndef ZP_RENDER_TARGET_H
#define ZP_RENDER_TARGET_H

ZP_PURE_INTERFACE zpRenderTarget : public zpReferencedObject {
public:
	virtual zp_uint getWidth() const = 0;
	virtual zp_uint getHeight() const = 0;

	virtual zp_uint getDisplayFormat( zpDisplayFormat* formats ) const = 0;

	virtual zp_uint getNumberOfTargets() const = 0;
};

#endif