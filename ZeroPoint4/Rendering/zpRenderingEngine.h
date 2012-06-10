#pragma once
#ifndef ZP_RENDERING_ENGINE_H
#define ZP_RENDERING_ENGINE_H

ZP_PURE_INTERFACE zpRenderingEngine {
	
	virtual zp_bool create() = 0;
	virtual void destroy() = 0;

	virtual zp_uint enumerateDisplayModes( zpRenderingDisplayFormat displayFormat, zpArrayList<zpRenderingDisplayMode>* outDisplayModes ) = 0;
	virtual zp_bool findClosestDisplayMode( const zpRenderingDisplayMode& displayMode, zpRenderingDisplayMode* outDisplayMode ) = 0;
	
	virtual void setDisplayMode( const zpRenderingDisplayMode& mode ) = 0;
	virtual const zpRenderingDisplayMode& getDisplayMode() const = 0;

	virtual void setScreenMode( zpRenderingScreenMode mode ) = 0;
	virtual zpRenderingScreenMode getScreenMode() const = 0;

	virtual void setWindow( zpWindow* window ) = 0;
	virtual zpWindow* getWindow() const = 0;

	virtual void setClearColor( const zpColor4f& color, zp_uint renderTargetIndex = 0 ) = 0;
	virtual const zpColor4f& getClearColor( zp_uint renderTargetIndex = 0 ) const = 0;

	virtual void clear() = 0;
	virtual void present() = 0;

protected:
	virtual zp_bool initialize() = 0;
	virtual void shutdown() = 0;

	friend class zpRenderingFactory;
};

#endif