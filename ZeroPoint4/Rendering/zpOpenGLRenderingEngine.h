#pragma once
#ifndef ZP_OPENGL_RENDERING_ENGINE_H
#define ZP_OPENGL_RENDERING_ENGINE_H

class zpOpenGLRenderingEngine : public zpRenderingEngine {
public:
	~zpOpenGLRenderingEngine();

	zp_bool create();
	void destroy();

	zpRenderingEngineType getEngineType() const;

	zp_uint enumerateDisplayModes( zpRenderingDisplayFormat displayFormat, zpArrayList<zpRenderingDisplayMode>* outDisplayModes );
	zp_bool findClosestDisplayMode( const zpRenderingDisplayMode& displayMode, zpRenderingDisplayMode* outDisplayMode );

	void setDisplayMode( const zpRenderingDisplayMode& mode );
	const zpRenderingDisplayMode& getDisplayMode() const;

	void setScreenMode( zpRenderingScreenMode mode );
	zpRenderingScreenMode getScreenMode() const;

	void setWindow( zpWindow* window );
	zpWindow* getWindow() const;

	void setClearColor( const zpColor4f& color, zp_uint renderTargetIndex = 0 );
	const zpColor4f& getClearColor( zp_uint renderTargetIndex = 0 ) const;

	void clear();
	void present();

	zpRenderingContext* createRenderingContext();
	void setCurrentRenderingContext( zpRenderingContext* context );
	zpRenderingContext* getCurrentRenderingContext() const;

	zpBuffer* createBuffer();

	zpTextureResource* createTextureResource();

protected:
	zp_bool initialize();
	void shutdown();

private:
	zpOpenGLRenderingEngine();
	
	zpWindow* m_window;

	zpRenderingScreenMode m_screenMode;
	zpRenderingDisplayMode m_displayMode;
	zpRenderingEngineType m_engineType;

	zpColor4f m_clearColor;

	friend class zpRenderingFactory;
};

#endif