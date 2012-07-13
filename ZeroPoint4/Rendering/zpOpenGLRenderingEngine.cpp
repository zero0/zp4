#include "zpRendering.h"
#include "zpOpenGL.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <GL\glew.h>
#include <GL\wglew.h>

zpOpenGLRenderingEngine::zpOpenGLRenderingEngine() {}
zpOpenGLRenderingEngine::~zpOpenGLRenderingEngine() {}

zp_bool zpOpenGLRenderingEngine::create() {
	HWND hwnd = (HWND)m_window->getWindowHandle();

	HDC hdc = GetDC( hwnd );

	PIXELFORMATDESCRIPTOR pfd;
	zp_zero_memory( &pfd );
	
	pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	zp_int pixelFormatIndex = ChoosePixelFormat( hdc, &pfd );
	if( pixelFormatIndex == 0 ) return false;

	zp_bool formatSet = SetPixelFormat( hdc, pixelFormatIndex, &pfd ) == 1;
	if( !formatSet ) return false;

	return true;
}
void zpOpenGLRenderingEngine::destroy() {}

zpRenderingEngineType zpOpenGLRenderingEngine::getEngineType() const { return ZP_RENDERING_ENGINE_OPENGL; }

zp_uint zpOpenGLRenderingEngine::enumerateDisplayModes( zpRenderingDisplayFormat displayFormat, zpArrayList<zpRenderingDisplayMode>* outDisplayModes ) { return 0; }
zp_bool zpOpenGLRenderingEngine::findClosestDisplayMode( const zpRenderingDisplayMode& displayMode, zpRenderingDisplayMode* outDisplayMode ) { return false; }

void zpOpenGLRenderingEngine::setDisplayMode( const zpRenderingDisplayMode& mode ) {}
const zpRenderingDisplayMode& zpOpenGLRenderingEngine::getDisplayMode() const { return zpRenderingDisplayMode(); }

void zpOpenGLRenderingEngine::setScreenMode( zpRenderingScreenMode mode ) {}
zpRenderingScreenMode zpOpenGLRenderingEngine::getScreenMode() const { return ZP_RENDERING_SCREEN_MODE_WINDOWED; }

void zpOpenGLRenderingEngine::setWindow( zpWindow* window ) {
	m_window = window;
}
zpWindow* zpOpenGLRenderingEngine::getWindow() const {
	return m_window;
}

void zpOpenGLRenderingEngine::setClearColor( const zpColor4f& color, zp_uint renderTargetIndex ) {}
const zpColor4f& zpOpenGLRenderingEngine::getClearColor( zp_uint renderTargetIndex ) const { return m_clearColor; }

void zpOpenGLRenderingEngine::clear() {}
void zpOpenGLRenderingEngine::present() {}

zpRenderingContext* zpOpenGLRenderingEngine::createRenderingContext() { return ZP_NULL; }
void zpOpenGLRenderingEngine::setCurrentRenderingContext( zpRenderingContext* context ) {}
zpRenderingContext* zpOpenGLRenderingEngine::getCurrentRenderingContext() const { return ZP_NULL; }

zpBuffer* zpOpenGLRenderingEngine::createBuffer() { return ZP_NULL; }

zpTextureResource* zpOpenGLRenderingEngine::createTextureResource() { return ZP_NULL; }

zp_bool zpOpenGLRenderingEngine::initialize() {
	return true;
}
void zpOpenGLRenderingEngine::shutdown() {}
