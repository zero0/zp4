#pragma once
#ifndef ZP_RENDERING_ENGINE_H
#define ZP_RENDERING_ENGINE_H

ZP_PURE_INTERFACE zpRenderingEngine {
	virtual void clear() = 0;
	virtual void present() = 0;

	//virtual void setVSyncEnabled( zp_bool enableVSync ) = 0;
	//virtual zp_bool isVSyncEnabled() const = 0;
};

#endif